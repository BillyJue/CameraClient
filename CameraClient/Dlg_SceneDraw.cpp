#include "Dlg_SceneDraw.h"
#include "global.h"
#include <QListWidgetItem>
#include "MgrData.h"
#include <QMessageBox>
#include "Mgr_PictureSave.h"
#include "Dlg_SelectAttr.h"
Dlg_SceneDraw::Dlg_SceneDraw(QWidget *parent)
	: QDialog(parent), m_bScenePark(false)
{
	ui.setupUi(this);
	setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
	connect(ui.btn_load, SIGNAL(clicked()), this, SLOT(Slot_BtnLoadSceneClicked()));
	connect(ui.btn_ok, SIGNAL(clicked()), this, SLOT(Slot_BtnOKClicked()));
	connect(ui.btn_cancel, SIGNAL(clicked()), this, SLOT(Slot_BtnCancelClicked()));
	connect(ui.btn_repaint, SIGNAL(clicked()), this, SLOT(Slot_BtnRepaintClicked()));
	connect(ui.btn_repaintAll, SIGNAL(clicked()), this, SLOT(Slot_BtnRepaintAllClicked()));
	connect(ui.btn_addRuleLine, SIGNAL(clicked()), this, SLOT(Slot_BtnAddRuleLineClicked()));
	connect(ui.btn_addTrapeizum, SIGNAL(clicked()), this, SLOT(Slot_BtnAddTrapeizumClicked()));
	connect(ui.btn_plogon, SIGNAL(clicked()), this, SLOT(Slot_BtnAddPolygonClicked()));
	connect(ui.btn_delete, SIGNAL(clicked()), this, SLOT(Slot_BtnDeleteClicked()));
	connect(ui.btn_savePic, SIGNAL(clicked()), this, SLOT(Slot_BtnSavePicClicked()));
	connect(ui.listWidget, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(Slot_ListWidgetItemClicked(QListWidgetItem*)));
	connect(ui.widget_2, SIGNAL(signal_popAttrShow()), this, SLOT(Slot_AttrShowDialog()));
	MgrData::getInstance()->GetMgrSubject()->Add(this);
	// 添加电警过程数据的接收通知窗口
	if (MgrData::getInstance()->GetMgrEvtLst())
	{
		MgrData::getInstance()->GetMgrEvtLst()->AddSender(static_cast<CObserver*>(this));
	}
	OnSnap();

}

void Dlg_SceneDraw::Slot_AttrShowDialog()
{
	bool isShow = false;
	switch (m_nSelType)
	{
	case scene::EMLINETYPE_LANE_LEFT:
	case scene::EMLINETYPE_LANE_RIGHT:
	case scene::EMLINETYPE_LANE_12:
	case scene::EMLINETYPE_LANE_23:
	case scene::EMLINETYPE_LANE_34:
	{
		tagDrawData *pData = ui.widget_2->GetCurDrawData();
		if (ui.widget_2->GetCurDrawData()->attribute != 0)
		{
			isShow = true;
		}
		break;
	}

	}
	if (!isShow)
	{
		return;
	}
	Dlg_SelectAttr dlg;
	dlg.setWindowTitle(GBUTF8("车道属性"));
	dlg.exec();
	Uint32 nAttr = dlg.GetData();
	ui.widget_2->SetAddLineAttr(m_nSelType, nAttr);
}

void Dlg_SceneDraw::SetData(TIPC_illgParkPara& tData, int w, int h)
{
	memcpy(&m_tIllgParkPara, &tData, sizeof(TIPC_illgParkPara));

	// 设置场景的宽高系数
	ui.widget_2->SetCoefficient(w, h);

	//m_picWidth = w;
	//m_picHeight = h;
	// 设置显示区域数据
	ClearListWidgetItem();
	TRoiArea_S apArea;
	wchar_t ws[65];
	for (int i = 0; i < m_tIllgParkPara.RoiAreaNum; i++)
	{
		apArea.dwRoiLabel = i;
		apArea.wPointNum = m_tIllgParkPara.tRoiArea[i].pointNum;
		memcpy(apArea.tPoint, m_tIllgParkPara.tRoiArea[i].tPoint, sizeof(TPoint_S)*apArea.wPointNum);
		ui.widget_2->AddRgn(&apArea, i, apArea.dwRoiLabel);

		QListWidgetItem *pItem = new QListWidgetItem;
		pItem->setText(GBUTF8("区域%1").arg(i + 1));
		pItem->setData(Qt::UserRole + 1, i);
		ui.listWidget->addItem(pItem);
	}
}
void Dlg_SceneDraw::GetData(TIPC_illgParkPara& tData)
{
	// 设置显示区域数据
	TRoiArea_S apArea;
	for (int i = 0; i < m_tIllgParkPara.RoiAreaNum; i++)
	{
		memset(&apArea, 0, sizeof(apArea));
		ui.widget_2->GetRgnOut(&apArea, i);
		m_tIllgParkPara.tRoiArea[i].pointNum = apArea.wPointNum;
		memcpy(m_tIllgParkPara.tRoiArea[i].tPoint, apArea.tPoint, sizeof(TPoint_S)*apArea.wPointNum);
	}

	memcpy(&tData, &m_tIllgParkPara, sizeof(TIPC_illgParkPara));
}

void Dlg_SceneDraw::SetIsParkScene(bool is)
{
	m_bScenePark = is;
	if (is)
	{
		ui.btn_addRuleLine->setVisible(false);
		ui.btn_addTrapeizum->setVisible(false);
		ui.btn_plogon->setVisible(false);
		ui.btn_delete->setVisible(false);
	}
	else
	{
		ui.btn_savePic->setVisible(false);
	}
}

void Dlg_SceneDraw::SetScenceIndex(int nIndex)
{
	m_scenceIndex = nIndex;
}

void Dlg_SceneDraw::Slot_BtnAddPolygonClicked()
{
	int nIndex = -1;
	tagDrawData tData;

	for (int i = 0; i < USER_RULE_REGION_MAX - 2; i++)
	{
		if (m_tSenceData.tPolygon[i].area.pointNum == 0 && m_tSenceData.tPolygon[i].enable == false)
		{
			nIndex = i;
			m_tSenceData.tPolygon[i].enable = true;
			break;
		}
	}

	if (nIndex != -1)
	{
		m_tSenceData.tPolygon[nIndex].enable = true;
		int nCreateType = nIndex + scene::EMCUSTOMTYPE_POLYGON_1;
		memset(&(m_tSenceData.tPolygon[nIndex].area), 0, sizeof(TIpcPolygon));
		ui.widget_2->AddPolygon(&(m_tSenceData.tPolygon[nIndex].area), nCreateType,
			m_tSenceData.tPolygon[nIndex].Label);
		ui.widget_2->SetColorByType(nCreateType, COLOR_PURPLE);
	}
	else
	{
		QMessageBox box;
		box.setText(GBUTF8("		超过自定义多边形的最大个数		"));
		box.setWindowTitle(GBUTF8("提示"));
		box.exec();
	}
}

void Dlg_SceneDraw::Slot_BtnSavePicClicked()
{
	Mgr_PictureSave dlg(nullptr);
	dlg.SetImage(ui.widget_2->GetPixmap());
	std::map<u32, tagDrawData*>& tempMap = ui.widget_2->GetDrawAllData();
	dlg.SetDrawData(tempMap);
	dlg.SetCurrentScenceIndex(m_scenceIndex);
	dlg.SavePicture();
	//TLocalCfgInfo localcfg;
	//m_pMgrData->GetMgrLocalCfg()->GetLocalCfg(&localcfg);
	
}

void Dlg_SceneDraw::Slot_BtnAddTrapeizumClicked()
{
	int nIndex = -1;
	tagDrawData tData;
	for (int i = 0; i < USER_RULE_REGION_MAX; i++)
	{
		if (m_tSenceData.tRuleRegion[i].bEnable == false)
		{
			nIndex = i;
			break;
		}
	}

	wchar_t ws[33];
	wsprintf(ws, TEXT("自定义梯形%d"), nIndex + 1);
	if (nIndex != -1)
	{
		m_tSenceData.tRuleRegion[nIndex].bEnable = true;
		int nCreateType = nIndex + scene::EMCUSTOMTYPE_AREA_1;
		memset(&(m_tSenceData.tRuleRegion[nIndex].echelonArea), 0, sizeof(TECHELON_S));
		ui.widget_2->AddTrapezium(&(m_tSenceData.tRuleRegion[nIndex].echelonArea), nCreateType, m_tSenceData.tRuleRegion[nIndex].Label);
		ui.widget_2->SetColorByType(nCreateType, COLOR_PURPLE);
	}
	else
	{
		QMessageBox box;
		box.setText(GBUTF8("		规则线已满，添加失败		"));
		box.setWindowTitle(GBUTF8("提示"));
		box.exec();
	}
}

void Dlg_SceneDraw::Slot_BtnAddRuleLineClicked()
{
	// 设置添加的类型和名称，id
	// 先找线数据
	int nLineIndex = -1;
	tagDrawData tData;
	for (int i = 0; i < USER_RULE_LINE_NUM_MAX; i++)
	{
		if (m_tSenceData.tRuleLine[i].bEnable == false)
		{
			nLineIndex = i;
			break;;
		}
	}

	wchar_t ws[33];
	wsprintf(ws, TEXT("自定义规则线%d"), nLineIndex + 1);
	if (nLineIndex != -1)
	{
		m_tSenceData.tRuleLine[nLineIndex].bEnable = true;
		int nCreateType = nLineIndex + scene::EMCUSTOMTYPE_1;
		memset(&(m_tSenceData.tRuleLine[nLineIndex].tLine), 0, sizeof(TLine_S));
		ui.widget_2->AddLine(&(m_tSenceData.tRuleLine[nLineIndex].tLine), nCreateType, m_tSenceData.tRuleLine[nLineIndex].LineLabel);
		ui.widget_2->SetColorByType(nCreateType, COLOR_PURPLE);
	}
	else
	{
		QMessageBox box;
		box.setText(GBUTF8("		规则线已满，添加失败		"));
		box.setWindowTitle(GBUTF8("提示"));
		box.exec();
	}
}

void Dlg_SceneDraw::Slot_BtnRepaintAllClicked()
{
	ui.widget_2->OnRedrawAll();
}

void Dlg_SceneDraw::Slot_BtnRepaintClicked()
{
	ui.widget_2->OnRedraw();
}

void Dlg_SceneDraw::Slot_BtnDeleteClicked()
{
	int nSelType = ui.widget_2->GetSelType();
	if (m_bEnable)
	{
		ui.widget_2->DeleteSelPoint();
	}
	else
	{
		int nCurRow = ui.listWidget->currentRow();
		QListWidgetItem *pItem = ui.listWidget->takeItem(nCurRow);
		delete pItem;
		pItem = nullptr;
		ui.widget_2->DelDrawData(nSelType);
	}
	ui.widget_2->update();
}

void Dlg_SceneDraw::Slot_ListWidgetItemClicked(QListWidgetItem *pItem)
{
	m_nSelType = pItem->data(Qt::UserRole + 1).toInt();
	int nTemp = pItem->data(Qt::UserRole + 2).toInt();
	m_bEnable = (nTemp == 111 ? true : false);
	ui.widget_2->SetSelType(m_nSelType);
}

void Dlg_SceneDraw::Slot_BtnLoadSceneClicked()
{
	OnSnap();
}

void Dlg_SceneDraw::Slot_BtnOKClicked()
{
	this->done(1);
}

void Dlg_SceneDraw::Slot_BtnCancelClicked()
{
	this->done(0);
}

Dlg_SceneDraw::~Dlg_SceneDraw()
{
	MgrData::getInstance()->GetMgrSubject()->Del(this);
	MgrData::getInstance()->GetMgrEvtLst()->DelSender(static_cast<CObserver*>(this));
}

void Dlg_SceneDraw::ClearListWidgetItem()
{
	while (ui.listWidget->count() > 0)
	{
		QListWidgetItem *pItem = ui.listWidget->takeItem(0);
		delete pItem;
		pItem = nullptr;
	}
}

void Dlg_SceneDraw::OnSnap()
{
	u32 dwIP = MgrData::getInstance()->GetIP();
	ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dwIP);
	if (pCtrl != NULL)
	{
		pCtrl->IpcEvt_Snap();
	}
}

void Dlg_SceneDraw::SetData(TIPC_SceneConfig& tData, int w, int h)
{
	memcpy(&m_tSenceData, &tData, sizeof(TIPC_SceneConfig));
	ui.btn_addRuleLine->setVisible(true);
	ui.btn_addTrapeizum->setVisible(true);
	ui.btn_delete->setVisible(true);

	// 设置场景的宽高系数
	ui.widget_2->Clear();
	ui.widget_2->SetCoefficient(w, h);

	int i;
	// 设置参数->车道配置
	int anTypeShow[scene::EMCUSTOMTYPE_AREA_COUNT];
	memset(anTypeShow, 0, sizeof(anTypeShow));
	ClearListWidgetItem();
	QListWidgetItem *pItem = new QListWidgetItem(GBUTF8("车道左边线"));
	pItem->setData(Qt::UserRole + 1, scene::EMLINETYPE_LANE_LEFT);
	pItem->setData(Qt::UserRole + 2, 111);
	ui.listWidget->addItem(pItem);
	QListWidgetItem *pItem2 = new QListWidgetItem(GBUTF8("车道右边线"));
	pItem2->setData(Qt::UserRole + 1, scene::EMLINETYPE_LANE_RIGHT);
	pItem2->setData(Qt::UserRole + 2, 111);
	ui.listWidget->addItem(pItem2);
	if (tData.dwLaneNum > 1)
	{
		QListWidgetItem *pItem3 = new QListWidgetItem(GBUTF8("车道12分割线"));
		pItem3->setData(Qt::UserRole + 1, scene::EMLINETYPE_LANE_12);
		pItem3->setData(Qt::UserRole + 2, 111);
		ui.listWidget->addItem(pItem3);
	}
	if (tData.dwLaneNum > 2)
	{
		QListWidgetItem *pItem3 = new QListWidgetItem(GBUTF8("车道23分割线"));
		pItem3->setData(Qt::UserRole + 1, scene::EMLINETYPE_LANE_23);
		pItem3->setData(Qt::UserRole + 2, 111);
		ui.listWidget->addItem(pItem3);
	}
	if (tData.dwLaneNum > 3)
	{
		QListWidgetItem *pItem3 = new QListWidgetItem(GBUTF8("车道34分割线"));
		pItem3->setData(Qt::UserRole + 1, scene::EMLINETYPE_LANE_34);
		pItem3->setData(Qt::UserRole + 2, 111);
		ui.listWidget->addItem(pItem3);
	}
	
	{
		QListWidgetItem *pItem3 = new QListWidgetItem(GBUTF8("检测触发线"));
		pItem3->setData(Qt::UserRole + 1, scene::EMLINETYPE_TRIGGER);
		pItem3->setData(Qt::UserRole + 2, 111);
		ui.listWidget->addItem(pItem3);
	}

	{
		QListWidgetItem *pItem3 = new QListWidgetItem(GBUTF8("左转判定线"));
		pItem3->setData(Qt::UserRole + 1, scene::EMLINETYPE_JUDGE_LEFT);
		pItem3->setData(Qt::UserRole + 2, 111);
		ui.listWidget->addItem(pItem3);
	}
	
	{
		QListWidgetItem *pItem3 = new QListWidgetItem(GBUTF8("直行判定线"));
		pItem3->setData(Qt::UserRole + 1, scene::EMLINETYPE_JUDGE_ZX);
		pItem3->setData(Qt::UserRole + 2, 111);
		ui.listWidget->addItem(pItem3);
	}

	{
		QListWidgetItem *pItem3 = new QListWidgetItem(GBUTF8("右转判定线"));
		pItem3->setData(Qt::UserRole + 2, 111);
		pItem3->setData(Qt::UserRole + 1, scene::EMLINETYPE_JUDGE_RIGHT);
		ui.listWidget->addItem(pItem3);
	}
	
	if (tData.dwLaneNum > 0)
	{
		QListWidgetItem *pItem3 = new QListWidgetItem(GBUTF8("停车线1"));
		pItem3->setData(Qt::UserRole + 1, scene::EMLINETYPE_STOP1);
		pItem3->setData(Qt::UserRole + 2, 111);
		ui.listWidget->addItem(pItem3);
	}
	if (tData.dwLaneNum > 1)
	{
		QListWidgetItem *pItem3 = new QListWidgetItem(GBUTF8("停车线2"));
		pItem3->setData(Qt::UserRole + 1, scene::EMLINETYPE_STOP2);
		pItem3->setData(Qt::UserRole + 2, 111);
		ui.listWidget->addItem(pItem3);
	}
	if (tData.dwLaneNum > 2)
	{
		QListWidgetItem *pItem3 = new QListWidgetItem(GBUTF8("停车线3"));
		pItem3->setData(Qt::UserRole + 1, scene::EMLINETYPE_STOP3);
		pItem3->setData(Qt::UserRole + 2, 111);
		ui.listWidget->addItem(pItem3);
	}
	if (tData.dwLaneNum > 3)
	{
		QListWidgetItem *pItem3 = new QListWidgetItem(GBUTF8("停车线4"));
		pItem3->setData(Qt::UserRole + 1, scene::EMLINETYPE_STOP4);
		pItem3->setData(Qt::UserRole + 2, 111);
		ui.listWidget->addItem(pItem3);
	}

	{
		QListWidgetItem *pItem3 = new QListWidgetItem(GBUTF8("跟踪区域"));
		pItem3->setData(Qt::UserRole + 1, scene::EMAREATYPE_TRACK);
		pItem3->setData(Qt::UserRole + 2, 111);
		ui.listWidget->addItem(pItem3);
	}

	{
		QListWidgetItem *pItem3 = new QListWidgetItem(GBUTF8("行人等候区域"));
		pItem3->setData(Qt::UserRole + 1, scene::EMAREATYPE_PERSON_WAIT);
		pItem3->setData(Qt::UserRole + 2, 111);
		ui.listWidget->addItem(pItem3);
	}

	{
		QListWidgetItem *pItem3 = new QListWidgetItem(GBUTF8("行人检测区域"));
		pItem3->setData(Qt::UserRole + 1, scene::EMAREATYPE_PERSON);
		pItem3->setData(Qt::UserRole + 2, 111);
		ui.listWidget->addItem(pItem3);
	}

	{
		QListWidgetItem *pItem3 = new QListWidgetItem(GBUTF8("预警区域"));
		pItem3->setData(Qt::UserRole + 1, scene::EMAREATYPE_WARNING);
		pItem3->setData(Qt::UserRole + 2, 111);
		ui.listWidget->addItem(pItem3);
	}
	
	// 自定义规则线
	for (int i = 0; i < USER_RULE_LINE_NUM_MAX; i++)
	{
		if (tData.tRuleLine[i].bEnable)
		{
			QListWidgetItem *pItem3 = new QListWidgetItem(GBUTF8("自定义规则线%1").arg(i+1));
			pItem3->setData(Qt::UserRole + 1, scene::EMCUSTOMTYPE_1 + i);
			ui.listWidget->addItem(pItem3);
		}
	}
	// 自定义区域
	for (int i = 0; i < USER_RULE_REGION_MAX; i++)
	{
		if (tData.tRuleRegion[i].bEnable)
		{
			QListWidgetItem *pItem3 = new QListWidgetItem(GBUTF8("自定义区域%1").arg(i + 1));
			pItem3->setData(Qt::UserRole + 1, scene::EMCUSTOMTYPE_COUNT + i);
			ui.listWidget->addItem(pItem3);
		}
	}

	//根据point判断使能
	for (i = 0; i < USER_RULE_REGION_MAX; i++)
	{
		m_tSenceData.tPolygon[i].enable = m_tSenceData.tPolygon[i].area.pointNum > 0 ? true : false;
	}
	// 自定义多边形区域
	for (int i = 0; i < USER_RULE_REGION_MAX - 3; i++)
	{
		if (tData.tPolygon[i].area.pointNum > 0)
		{
			QListWidgetItem *pItem3 = new QListWidgetItem(GBUTF8("自定义多边形区域%1").arg(i + 1));
			pItem3->setData(Qt::UserRole + 1, scene::EMCUSTOMTYPE_POLYGON_1 + i);
			ui.listWidget->addItem(pItem3);
		}
	}

	// 设置显示线条数据
	TRuleLine_S* apLine[scene::EMCUSTOMTYPE_COUNT];
	memset(apLine, 0, sizeof(apLine));
	apLine[scene::EMLINETYPE_LANE_LEFT] = &(tData.tRdLineL);
	apLine[scene::EMLINETYPE_LANE_RIGHT] = &(tData.tRdLineR);
	apLine[scene::EMLINETYPE_LANE_12] = &(tData.tRdLine12);
	apLine[scene::EMLINETYPE_LANE_23] = &(tData.tRdLine23);
	apLine[scene::EMLINETYPE_LANE_34] = &(tData.tRdLine34);
	apLine[scene::EMLINETYPE_TRIGGER] = &(tData.tScanLine);
	apLine[scene::EMLINETYPE_JUDGE_LEFT] = &(tData.tTurnLeftLine);
	apLine[scene::EMLINETYPE_JUDGE_ZX] = &(tData.tStraightLine);
	apLine[scene::EMLINETYPE_JUDGE_RIGHT] = &(tData.tTurnRightLine);
	apLine[scene::EMLINETYPE_STOP1] = &(tData.tStopLine[0]);
	apLine[scene::EMLINETYPE_STOP2] = &(tData.tStopLine[1]);
	apLine[scene::EMLINETYPE_STOP3] = &(tData.tStopLine[2]);
	apLine[scene::EMLINETYPE_STOP4] = &(tData.tStopLine[3]);
	// 自定义规则线
	for (int i = 0; i < USER_RULE_LINE_NUM_MAX; i++)
	{
		if (tData.tRuleLine[i].bEnable)
		{
			apLine[scene::EMCUSTOMTYPE_1 + i] = &(tData.tRuleLine[i]);
		}
	}

	// 显示
	for (i = 0; i < scene::EMCUSTOMTYPE_COUNT; i++)
	{
		if (apLine[i] != NULL)
		{
			ui.widget_2->AddLine(&(apLine[i]->tLine), i, apLine[i]->LineLabel);
			if (i <= scene::EMLINETYPE_LANE_34)
			{
				ui.widget_2->SetColorByType(i, COLOR_BLUE);
			}
			else if (i >= scene::EMLINETYPE_TRIGGER && i < scene::EMLINETYPE_COUNT)
			{
				ui.widget_2->SetColorByType(i, COLOR_RED);
			}
			else if (i >= scene::EMAREATYPE_TRACK && i < scene::EMAREATYPE_COUNT)
			{
				ui.widget_2->SetColorByType(i, COLOR_YELLOW);
			}
			else if (i >= scene::EMCUSTOMTYPE_1 && i < scene::EMCUSTOMTYPE_COUNT)
			{
				ui.widget_2->SetColorByType(i, COLOR_PURPLE);
			}
		}
	}

	// 跟踪区域	
	ui.widget_2->AddRgn(&tData.tTrackArea, scene::EMAREATYPE_TRACK, tData.tTrackArea.dwRoiLabel);

	//this->GetCtrl_ScenceDraw()->AddRect(&tRect, scene::EMAREATYPE_TRACK, tData.tTrackArea.dwRoiLabel);
	ui.widget_2->SetColorByType(scene::EMAREATYPE_TRACK, COLOR_YELLOW);


	// 自定义梯形区域
	for (i = 0; i < USER_RULE_REGION_MAX; i++)
	{
		if (tData.tRuleRegion[i].bEnable)
		{
			ui.widget_2->AddTrapezium(&(tData.tRuleRegion[i].echelonArea), scene::EMCUSTOMTYPE_AREA_1 + i, tData.tRuleRegion[i].Label);
			ui.widget_2->SetColorByType(scene::EMCUSTOMTYPE_AREA_1 + i, COLOR_PURPLE);
		}
	}
	
	for (int i = 0; i < USER_RULE_REGION_MAX; i++)
	{
		tData.tPolygon[i].enable = tData.tPolygon[i].area.pointNum > 0 ? true : false;
	}

	// 自定义多边形区域
	for (i = 0; i < USER_RULE_REGION_MAX - 3; i++)
	{
		if (tData.tPolygon[i].area.pointNum > 0)
		{
			ui.widget_2->AddPolygon(&(tData.tPolygon[i].area), scene::EMCUSTOMTYPE_POLYGON_1 + i, tData.tPolygon[i].Label);
			ui.widget_2->SetColorByType(scene::EMCUSTOMTYPE_POLYGON_1 + i, COLOR_PURPLE);
		}
		//如果pointNum 为0，则enable为false;
		if (m_tSenceData.tPolygon[i].area.pointNum == 0)
		{
			m_tSenceData.tPolygon[i].enable = false;
		}
	}

	//行人等候区域

	ui.widget_2->AddPolygon2Rect(&tData.tPolygon[3].area, scene::EMAREATYPE_PERSON_WAIT, 10000 + tData.tPolygon[3].Label);
	ui.widget_2->SetColorByType(scene::EMAREATYPE_PERSON_WAIT, COLOR_YELLOW);

	ui.widget_2->AddPolygon(&tData.tPolygon[2].area, scene::EMAREATYPE_PERSON, 20000 + tData.tPolygon[2].Label);
	ui.widget_2->SetColorByType(scene::EMAREATYPE_PERSON, COLOR_YELLOW);

	ui.widget_2->AddPolygon(&(tData.tPolygon[1].area), scene::EMAREATYPE_WARNING, tData.tPolygon[1].Label);
	ui.widget_2->SetColorByType(scene::EMAREATYPE_WARNING, COLOR_YELLOW);

}

void Dlg_SceneDraw::OnObserverNotify(LPARAM lHint, LPVOID pHint)
{
	u32 dwEvtType = (u32)lHint;
	u32 dwId = (u32)pHint;

	if (dwEvtType == NMC_EVT_VIDEO_PIC)
	{
		//m_dwId = dwId;
		ui.widget_2->SetNewPic2(dwId, 0);
	}
}

void Dlg_SceneDraw::GetData(TIPC_SceneConfig& tData)
{
	// 设置显示线条数据
	TLine_S* apLine[scene::EMCUSTOMTYPE_COUNT];
	memset(apLine, 0, sizeof(apLine));
	apLine[scene::EMLINETYPE_LANE_LEFT] = &(tData.tRdLineL.tLine);
	apLine[scene::EMLINETYPE_LANE_RIGHT] = &(tData.tRdLineR.tLine);
	apLine[scene::EMLINETYPE_LANE_12] = &(tData.tRdLine12.tLine);
	apLine[scene::EMLINETYPE_LANE_23] = &(tData.tRdLine23.tLine);
	apLine[scene::EMLINETYPE_LANE_34] = &(tData.tRdLine34.tLine);
	apLine[scene::EMLINETYPE_TRIGGER] = &(tData.tScanLine.tLine);
	apLine[scene::EMLINETYPE_JUDGE_LEFT] = &(tData.tTurnLeftLine.tLine);
	apLine[scene::EMLINETYPE_JUDGE_ZX] = &(tData.tStraightLine.tLine);
	apLine[scene::EMLINETYPE_JUDGE_RIGHT] = &(tData.tTurnRightLine.tLine);
	apLine[scene::EMLINETYPE_STOP1] = &(tData.tStopLine[0].tLine);
	apLine[scene::EMLINETYPE_STOP2] = &(tData.tStopLine[1].tLine);
	apLine[scene::EMLINETYPE_STOP3] = &(tData.tStopLine[2].tLine);
	apLine[scene::EMLINETYPE_STOP4] = &(tData.tStopLine[3].tLine);
	// 自定义的规则线
	for (int i = 0; i < USER_RULE_LINE_NUM_MAX; i++)
	{
		tData.tRuleLine[i].bEnable = m_tSenceData.tRuleLine[i].bEnable;

		if (m_tSenceData.tRuleLine[i].bEnable)
		{
			apLine[scene::EMCUSTOMTYPE_1 + i] = &(tData.tRuleLine[i].tLine);
		}
	}
	// 获取检测线数据
	int i;
	for (i = 0; i < scene::EMCUSTOMTYPE_COUNT; i++)
	{
		if (apLine[i] != NULL)
		{
			ui.widget_2->GetLineOut(apLine[i], i);
		}
	}

	// 获取显示区域数据
	TRoiArea_S apArea;
	ui.widget_2->GetRgnOut(&apArea, scene::EMAREATYPE_TRACK);
	memcpy(&tData.tTrackArea, &apArea, sizeof(TRoiArea_S));
	//data::Rect2Rgn4(&apArea, &tData.tTrackArea);


	// 获取自定义区域数据
	for (i = 0; i < USER_RULE_REGION_MAX; i++)
	{
		tData.tRuleRegion[i].bEnable = m_tSenceData.tRuleRegion[i].bEnable;
		if (tData.tRuleRegion[i].bEnable)
		{
			ui.widget_2->GetTrapeziumOut(&(tData.tRuleRegion[i].echelonArea), i + scene::EMCUSTOMTYPE_COUNT);
		}
	}

	// 获取多边形数据
	for (i = 0; i < USER_RULE_REGION_MAX; i++)
	{
		tData.tPolygon[i].enable = m_tSenceData.tPolygon[i].enable;
		ui.widget_2->GetPolygonOut(&(tData.tPolygon[i].area), i + scene::EMCUSTOMTYPE_POLYGON_1);
	}
	//获取等待区域
	TRoiArea_S apArea2;
	ui.widget_2->GetRgnOut(&apArea2, scene::EMAREATYPE_PERSON_WAIT);
	tData.tPolygon[3].area.pointNum = apArea2.wPointNum;
	for (int i = 0; i < apArea2.wPointNum; i++)
	{
		tData.tPolygon[3].area.point[i].X = apArea2.tPoint[i].X;
		tData.tPolygon[3].area.point[i].Y = apArea2.tPoint[i].Y;
	}

	//获取检测区域
	TRoiArea_S apArea3;
	ui.widget_2->GetRgnOut(&apArea3, scene::EMAREATYPE_PERSON);
	tData.tPolygon[2].area.pointNum = apArea3.wPointNum;
	for (int i = 0; i < apArea3.wPointNum; i++)
	{
		tData.tPolygon[2].area.point[i].X = apArea3.tPoint[i].X;
		tData.tPolygon[2].area.point[i].Y = apArea3.tPoint[i].Y;
	}

}

