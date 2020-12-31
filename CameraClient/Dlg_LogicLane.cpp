#include "Dlg_LogicLane.h"
#include "MgrData.h"
#include <QTreeWidget>
#include "global.h"
#include "Dlg_LogicLane_Detail.h"
#include <QVariant>
#include <QListView>
#include <QTimer>
Dlg_LogicLane::Dlg_LogicLane(QWidget *parent)
	: MyWidget(parent), m_nSceneIndex(0), m_messageBox(nullptr)
{
	ui.setupUi(this);
	memset(&m_tLogicLaneInfo, 0, sizeof(TIPC_SetLogicLaneReq));
	memset(&m_tVideoLoopInfo, 0, sizeof(TIPC_SetVideoLoopReq));
	memset(&m_tSeceneCfg, 0, sizeof(TIPC_SetSceneCfgReq));
	memset(&m_tRecogAreaInfo, 0, sizeof(TIPC_SetRecogAreaReq));

	connect(this, SIGNAL(signal_logicLaneInfoChanged()), this, SLOT(Slot_LogicLaneInfoChanged()));
	connect(this, SIGNAL(signal_sceneInfoChanged()), this, SLOT(Slot_SceneInfoChanged()));
	connect(this, SIGNAL(signal_recogAreaChanged()), this, SLOT(Slot_RecogAreaChanged()));
	connect(this, SIGNAL(signal_videoLoopInfoChanged()), this, SLOT(Slot_VideoLoopInfoChanged()));
	connect(ui.cmb_sence, SIGNAL(currentIndexChanged(int)), this, SLOT(Slot_CmbSenceIndexChanged(int)));
	connect(ui.treeWidget, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(Slot_TreeWidgetItemClicked(QTreeWidgetItem*, int)));
	connect(ui.btn_load, SIGNAL(clicked()), this, SLOT(Slot_BtnLoadSceneClicked()));
	connect(ui.btn_save, SIGNAL(clicked()), this, SLOT(Slot_BtnSaveClicked()));
	connect(ui.btn_fresh, SIGNAL(clicked()), this, SLOT(Slot_BtnFreshClicked()));
	connect(ui.btn_createLogicLane, SIGNAL(clicked()), this, SLOT(Slot_BtnCreateLogicLaneClicked()));
	m_labSeq = new LabelDelegate;
	m_chkEnable = new CheckBoxDelegate;
	m_cmbTestType = new ComboboxDelegate;
	m_cmbSensitive = new ComboboxDelegate;
	m_cmbRecogArea = new ComboboxDelegate;
	m_cmbLaneID = new ComboboxDelegate;
	m_cmbWhiteLight = new ComboboxDelegate;
	m_cmbNightLight = new ComboboxDelegate;
	m_chkFaceExposure = new CheckBoxDelegate;
	m_mapType.insert(0, GBUTF8("卡口"));
	m_mapType.insert(1, GBUTF8("超速"));
	m_mapType.insert(2, GBUTF8("不按规定行驶"));
	m_mapType.insert(3, GBUTF8("实线变道"));
	m_mapType.insert(4, GBUTF8("闯红灯"));
	m_mapType.insert(5, GBUTF8("不按导向行驶"));
	m_mapType.insert(6, GBUTF8("禁止左转"));
	m_mapType.insert(7, GBUTF8("逆行"));
	m_mapType.insert(8, GBUTF8("压实线"));
	m_mapType.insert(9, GBUTF8("违法掉头"));
	m_mapType.insert(10, GBUTF8("越线停车"));
	m_mapType.insert(11, GBUTF8("路口滞留"));
	m_mapType.insert(12, GBUTF8("黄网格停车"));
	m_mapType.insert(13, GBUTF8("机动车未礼让行人"));
	m_mapType.insert(14, GBUTF8("快速路违法倒车"));
	m_mapType.insert(15, GBUTF8("转弯未让直行"));
	m_mapType.insert(16, GBUTF8("行人闯红灯"));
	m_mapType.insert(17, GBUTF8("机动车占用非机动车道"));
	m_mapType.insert(18, GBUTF8("占用应急车道"));
	m_mapType.insert(19, GBUTF8("占用公交车道"));
	m_mapType.insert(20, GBUTF8("大型车占用小型车车道"));
	m_mapType.insert(21, GBUTF8("禁止货车车辆通行"));
	m_mapType.insert(22, GBUTF8("大弯小转"));
	m_mapType.insert(23, GBUTF8("未保持行车安全距离"));
	m_mapType.insert(24, GBUTF8("司乘违法"));
	m_mapType.insert(25, GBUTF8("车辆限行"));
	m_mapType.insert(26, GBUTF8("违章鸣笛"));
	m_mapType.insert(27, GBUTF8("超高"));
	m_mapType.insert(28, GBUTF8("非机动车占用机动车道"));
	m_mapType.insert(29, GBUTF8("非机动车载人"));
	m_mapType.insert(30, GBUTF8("非机动车加装伞具"));
	m_mapType.insert(31, GBUTF8("越线预警"));
	m_mapType.insert(32, GBUTF8("未识别"));

	ui.cmb_sence->setView(new QListView);
	QStringList sList;
	QMap<int, QString>::iterator itor;
	for (itor = m_mapType.begin(); itor != m_mapType.end(); itor++)
	{
		sList.append(itor.value());
	}
	m_cmbTestType->setComboxData(sList);
	QStringList sList2;
	for (int i = 0; i < 10; i++)
	{
		sList2.append(GBUTF8("%1").arg(i + 1));
	}
	m_cmbSensitive->setComboxData(sList2);

	QStringList sList3;
	for (int i = 0; i < 4; i++)
	{
		sList3.append(GBUTF8("%1").arg(i + 1));
	}
	m_cmbRecogArea->setComboxData(sList3);
	m_cmbLaneID->setComboxData(sList3);

	QStringList sList4;
	sList4.append(GBUTF8("视频流截图"));
	sList4.append(GBUTF8("闪光灯"));
	sList4.append(GBUTF8("LED增亮补光"));
	sList4.append(GBUTF8("红外闪光灯"));
	sList4.append(GBUTF8("红外LED灯"));
	sList4.append(GBUTF8("闪光灯+LED增亮补光"));
	sList4.append(GBUTF8("闪光灯+红外LED增亮"));
	sList4.append(GBUTF8("红外闪光灯+红外LED增亮"));
	sList4.append(GBUTF8("红外闪光灯+LED增亮补光"));
	m_cmbWhiteLight->setComboxData(sList4);
	m_cmbNightLight->setComboxData(sList4);

	connect(m_chkEnable, SIGNAL(signal_dataChanged(int, int)), this, SLOT(Slot_CmbIndexChanged(int, int)));
	connect(m_chkFaceExposure, SIGNAL(signal_dataChanged(int, int)), this, SLOT(Slot_CmbIndexChanged(int, int)));

	connect(m_cmbTestType, SIGNAL(signal_combDataChanged(int, int)), this, SLOT(Slot_CmbIndexChanged(int, int)));
	connect(m_cmbSensitive, SIGNAL(signal_combDataChanged(int, int)), this, SLOT(Slot_CmbIndexChanged(int, int)));
	connect(m_cmbRecogArea, SIGNAL(signal_combDataChanged(int, int)), this, SLOT(Slot_CmbIndexChanged(int, int)));
	connect(m_cmbLaneID, SIGNAL(signal_combDataChanged(int, int)), this, SLOT(Slot_CmbIndexChanged(int, int)));
	connect(m_cmbWhiteLight, SIGNAL(signal_combDataChanged(int, int)), this, SLOT(Slot_CmbIndexChanged(int, int)));
	connect(m_cmbNightLight, SIGNAL(signal_combDataChanged(int, int)), this, SLOT(Slot_CmbIndexChanged(int, int)));

	ui.treeWidget->setItemDelegateForColumn(0, m_labSeq);
	ui.treeWidget->setItemDelegateForColumn(1, m_chkEnable);
	ui.treeWidget->setItemDelegateForColumn(2, m_cmbTestType);
	ui.treeWidget->setItemDelegateForColumn(3, m_cmbSensitive);
	ui.treeWidget->setItemDelegateForColumn(4, m_cmbRecogArea);
	ui.treeWidget->setItemDelegateForColumn(5, m_cmbLaneID);
	ui.treeWidget->setItemDelegateForColumn(6, m_cmbWhiteLight);
	ui.treeWidget->setItemDelegateForColumn(7, m_cmbNightLight);
	ui.treeWidget->setItemDelegateForColumn(8, m_chkFaceExposure);

	//m_pixDraw = new Dlg_PictureDraw(nullptr);
	// 添加电警过程数据的接收通知窗口
	if (MgrData::getInstance()->GetMgrEvtLst())
	{
		MgrData::getInstance()->GetMgrEvtLst()->AddSender(static_cast<CObserver*>(this));
	}
	MgrData::getInstance()->GetMgrSubject()->Add(this);

	//TEST
	//InitData(true);
}

void Dlg_LogicLane::OnSetTxtVisible()
{
	if (m_messageBox)
	{
		m_messageBox->close();
		SAFE_DELETE(m_messageBox);
	}
}

void Dlg_LogicLane::InitData(bool is)
{
	if (is)
	{
		memset(&m_tLogicLaneInfo, 0, sizeof(m_tLogicLaneInfo));
		memset(&m_tVideoLoopInfo, 0, sizeof(m_tVideoLoopInfo));
		memset(&m_tSeceneCfg, 0, sizeof(m_tSeceneCfg));
		memset(&m_tRecogAreaInfo, 0, sizeof(m_tRecogAreaInfo));

		// 获取场景信息成功后获取逻辑车道信息
		TIPC_GetUICapsRsp *m_pUICap = MgrData::getInstance()->GetDevCap();
		if (m_pUICap->szCaps[EMUI_CAP_TYPE_ILLGPARK] == 0)
		{
			GetParamScenceInfo();
		}
		else
		{
			GetScenceID();
		}

		if (m_tLocalCfgInfo.ispic)
		{
			OnSnap();
		}
		ui.video->ControlPlayVideo(is);
	}
	else
	{
		ui.video->ControlPlayVideo(false);
	}
}

void Dlg_LogicLane::OnSnap()
{
	u32 dwIP = MgrData::getInstance()->GetIP();
	ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dwIP);
	if (pCtrl != NULL)
	{
		pCtrl->IpcEvt_Snap();
	}
}

void Dlg_LogicLane::GetScenceID()
{
	u32 dwIP = MgrData::getInstance()->GetIP();
	ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dwIP);
	if (pCtrl != NULL)
	{
		bool bRet = pCtrl->GetScenceID(NULL, (u32)(CObserver*)this);
		if (!bRet)
		{
		}
	}
}

Dlg_LogicLane::~Dlg_LogicLane()
{
	ui.video->ControlPlayVideo(false);
	if (MgrData::getInstance()->GetMgrEvtLst())
	{
		MgrData::getInstance()->GetMgrEvtLst()->DelSender(static_cast<CObserver*>(this));
	}
	MgrData::getInstance()->GetMgrSubject()->Del(this);
	
	//SAFE_DELETE(m_pixDraw);
}

bool Dlg_LogicLane::GetParamLogicLane()
{
	u32 dwIP = MgrData::getInstance()->GetIP();
	ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dwIP);
	if (pCtrl != NULL)
	{
		int sceneid = m_nSceneIndex + 1;
		bool bRet = pCtrl->GetIpcLogicLane(&sceneid, (u32)(CObserver*)this);

		if (!bRet)
		{
		}
		return bRet;
	}

	return false;
}

QWidget* Dlg_LogicLane::CreateWidget(int nIndex)
{
	QWidget *pWidget = new QWidget;
	pWidget->setFixedSize(100, 30);
	QPushButton *btn = new QPushButton;
	btn->setProperty("nIndex", nIndex);
	btn->setText(" ...");
	btn->setFixedSize(80, 25);
	//btn->setStyleSheet("color:rgb(255,255,0);background-color:rbg(255,255,255);border-radius:2px");
	QVBoxLayout *lay = new QVBoxLayout;
	lay->setContentsMargins(0, 0, 0, 0);
	QSpacerItem *itemer1 = new QSpacerItem(10, 1, QSizePolicy::Maximum, QSizePolicy::Fixed);
	QSpacerItem *itemer2 = new QSpacerItem(10, 1, QSizePolicy::Maximum, QSizePolicy::Fixed);
	lay->addItem(itemer1);
	lay->addWidget(btn);
	lay->addItem(itemer2);
	pWidget->setLayout(lay);
	pWidget->setContentsMargins(0, 0, 0, 0);
	pWidget->setProperty("btn", QVariant::fromValue(btn));
	return pWidget;
}

void Dlg_LogicLane::UpdateData(bool bUpdate)
{
	if (!bUpdate)
	{
		int nLen = LOGIC_ROAD_NUM_MAX;
		while (ui.treeWidget->topLevelItemCount() > 0)
		{
			QTreeWidgetItem *pItem = ui.treeWidget->takeTopLevelItem(0);
			delete pItem;
			pItem = nullptr;
		}
		for (int i = 0; i < nLen; i++)
		{
			
			TIPC_LogicLane* ptLogicLane = &m_tLogicLaneInfo.tLogicLane[i];
			QTreeWidgetItem *pListItem = new QTreeWidgetItem;
			//pListItem->setData(0, Qt::UserRole + 1, i);
			//pListItem->setData(0, Qt::UserRole + 2, QVariant::fromValue(ptLogicLane));
			pListItem->setText(0, QString::fromStdWString(logiclane::GetListValue(ptLogicLane, logiclane::EMLOGICLANE_SN)));
			pListItem->setText(1, QString::fromStdWString(logiclane::GetListValue(ptLogicLane, logiclane::EMLOGICLANE_ENABLED)));
			pListItem->setText(2, QString::fromStdWString(logiclane::GetListValue(ptLogicLane, logiclane::EMLOGICLANE_CHKTYPE)));
			pListItem->setText(3, QString::fromStdWString(logiclane::GetListValue(ptLogicLane, logiclane::EMLOGICLANE_SENSITIVITY)));
			pListItem->setText(4, QString::fromStdWString(logiclane::GetListValue(ptLogicLane, logiclane::EMLOGICLANE_IDENTID)));
			pListItem->setText(5, QString::fromStdWString(logiclane::GetListValue(ptLogicLane, logiclane::EMLOGICLANE_LANEID)));
			pListItem->setText(6, QString::fromStdWString(logiclane::GetListValue(ptLogicLane, logiclane::EMLOGICLANE_FILLLIGHT_DAY)));
			pListItem->setText(7, QString::fromStdWString(logiclane::GetListValue(ptLogicLane, logiclane::EMLOGICLANE_FILLLIGHT_NIGHT)));
			pListItem->setText(8, QString::fromStdWString(logiclane::GetListValue(ptLogicLane, logiclane::EMLOGICLANE_EXPOSURE_PEOPLE)));
			
			ui.treeWidget->addTopLevelItem(pListItem);
			QWidget *pWidget = CreateWidget(i);
			QPushButton *btn = pWidget->property("btn").value<QPushButton*>();
			connect(btn, SIGNAL(clicked()), this, SLOT(Slot_ShowLogicLaneDetailClicked()));
			ui.treeWidget->setItemWidget(pListItem, 9, pWidget);

		}
	}

}

void Dlg_LogicLane::Slot_ShowLogicLaneDetailClicked()
{
	QPushButton *pBtn = dynamic_cast<QPushButton*>(this->sender());
	int nIndex = pBtn->property("nIndex").toInt();
	Dlg_LogicLane_Detail dlg;
	TIPC_LogicLane &tLogicLane = m_tLogicLaneInfo.tLogicLane[nIndex];
	dlg.SetData(&tLogicLane);
	dlg.exec();

}



void Dlg_LogicLane::Slot_LogicLaneInfoChanged()
{
	UpdateData(false);

	GetParamVideoLoopInfo();
}

// 检测器信息获取
void Dlg_LogicLane::GetParamVideoLoopInfo()
{
	MgrItsCtrl* pMgrItsCtrl = MgrData::getInstance()->GetMgrItsCtrl();
	if (pMgrItsCtrl != NULL)
	{
		u32 dwIP = MgrData::getInstance()->GetIP();
		ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dwIP);
		if (pCtrl != NULL)
		{
			int sceneid = m_nSceneIndex + 1;
			bool bRet = pCtrl->GetIpcVideoLoop(&sceneid, (u32)(CObserver*)this);
			if (!bRet)
			{
			}
		}
	}
}

void Dlg_LogicLane::OnChangeScenceNum()
{
	int nNum = m_tSeceneCfg.dwSceneSize;
	if (ui.cmb_sence->currentIndex() != nNum)
	{
		/*while (ui.cmb_sence->count() > 0)
		{
			ui.cmb_sence->setProperty("id", 111);
			ui.cmb_sence->removeItem(0);
		}
		*/
		ui.cmb_sence->clear();
		if (nNum < m_nSceneIndex) m_nSceneIndex = 0;

		QStringList sList;
		for (int i = 1; i <= nNum; i++)
		{
			sList.append(QString("%1").arg(i));
		}
		ui.cmb_sence->setProperty("id", 111);
		ui.cmb_sence->addItems(sList);
		ui.cmb_sence->setProperty("id", 111);
		ui.cmb_sence->setCurrentIndex(m_nSceneIndex);
		Sleep(1);
		ui.cmb_sence->setProperty("id", 0);
	}


}


void Dlg_LogicLane::OnChangePreset(int nPreset)
{
	MgrItsCtrl* pMgrItsCtrl = MgrData::getInstance()->GetMgrItsCtrl();
	if (pMgrItsCtrl != NULL)
	{
		u32 dwIP = MgrData::getInstance()->GetIP();
		ItsCtrl* pCtrl = MgrData::getInstance() ->GetMgrItsCtrl()->GetCtrl(dwIP);
		if (pCtrl != NULL)
		{
			TIPC_PTZCtrl tData;
			tData.wEventType = PTZ_EVENT_TYPE::enum_ptz_event_type_preset;
			tData.wP1 = (Uint16)nPreset;
			tData.wP4 = (Uint16)m_nSceneIndex;
			bool bRet = pCtrl->IpcPtz(&tData);
		}
	}
}

// 设置绘制显示数据
void Dlg_LogicLane::SetScenceDrawData()
{
	/*
	m_pixDraw->Clear();
	m_pixDraw->SetCoefficient(m_tSeceneCfg.tMaxParam.dwMaxWidth, m_tSeceneCfg.tMaxParam.dwMaxHeight);

	TRuleLine_S* apLine[scene::EMCUSTOMTYPE_COUNT];
	memset(apLine, 0, sizeof(apLine));
	apLine[scene::EMLINETYPE_LANE_LEFT] = &(m_tSeceneCfg.tSceneCfg.tRdLineL);
	apLine[scene::EMLINETYPE_LANE_RIGHT] = &(m_tSeceneCfg.tSceneCfg.tRdLineR);
	apLine[scene::EMLINETYPE_LANE_12] = &(m_tSeceneCfg.tSceneCfg.tRdLine12);
	apLine[scene::EMLINETYPE_LANE_23] = &(m_tSeceneCfg.tSceneCfg.tRdLine23);
	apLine[scene::EMLINETYPE_LANE_34] = &(m_tSeceneCfg.tSceneCfg.tRdLine34);
	apLine[scene::EMLINETYPE_TRIGGER] = &(m_tSeceneCfg.tSceneCfg.tScanLine);
	apLine[scene::EMLINETYPE_JUDGE_LEFT] = &(m_tSeceneCfg.tSceneCfg.tTurnLeftLine);
	apLine[scene::EMLINETYPE_JUDGE_ZX] = &(m_tSeceneCfg.tSceneCfg.tStraightLine);
	apLine[scene::EMLINETYPE_JUDGE_RIGHT] = &(m_tSeceneCfg.tSceneCfg.tTurnRightLine);
	apLine[scene::EMLINETYPE_STOP1] = &(m_tSeceneCfg.tSceneCfg.tStopLine[0]);
	apLine[scene::EMLINETYPE_STOP2] = &(m_tSeceneCfg.tSceneCfg.tStopLine[1]);
	apLine[scene::EMLINETYPE_STOP3] = &(m_tSeceneCfg.tSceneCfg.tStopLine[2]);
	apLine[scene::EMLINETYPE_STOP4] = &(m_tSeceneCfg.tSceneCfg.tStopLine[3]);
	// 自定义的规则线
	for (int i = 0; i < USER_RULE_LINE_NUM_MAX; i++)
	{
		if (m_tSeceneCfg.tSceneCfg.tRuleLine[i].bEnable)
		{
			apLine[scene::EMCUSTOMTYPE_1 + i] = &(m_tSeceneCfg.tSceneCfg.tRuleLine[i]);
		}
	}

	for (int i = 0; i < scene::EMCUSTOMTYPE_COUNT; i++)
	{
		if (apLine[i] != NULL)
		{
			m_pixDraw->AddLine(&(apLine[i]->tLine), i, apLine[i]->LineLabel);
			if (i <= scene::EMLINETYPE_LANE_34)
			{
				m_pixDraw->SetColorByType(i, COLOR_BLUE);
			}
			else if (i >= scene::EMLINETYPE_TRIGGER && i < scene::EMLINETYPE_COUNT)
			{
				m_pixDraw->SetColorByType(i, COLOR_RED);
			}
			else if (i >= scene::EMAREATYPE_TRACK && i < scene::EMAREATYPE_COUNT)
			{
				m_pixDraw->SetColorByType(i, COLOR_YELLOW);
			}
			else if (i >= scene::EMCUSTOMTYPE_1 && i < scene::EMCUSTOMTYPE_COUNT)
			{
				m_pixDraw->SetColorByType(i, COLOR_PURPLE);
			}
		}
	}

	TRoiArea_S* apArea[5];
	memset(apArea, 0, sizeof(apArea));
	apArea[0] = &(m_tSeceneCfg.tSceneCfg.tTrackArea);
	for (int i = 0; i < 5; i++)
	{
		if (apArea[i] != NULL)
		{
			m_pixDraw->AddRgn4(apArea[i], i + scene::EMAREATYPE_TRACK, apArea[i]->dwRoiLabel);
			m_pixDraw->SetColorByType(i + scene::EMAREATYPE_TRACK, COLOR_YELLOW);
		}
	}

	for (int i = 0; i < USER_RULE_REGION_MAX; i++)
	{
		if (m_tSeceneCfg.tSceneCfg.tRuleRegion[i].bEnable)
		{
			m_pixDraw->AddTrapezium(&(m_tSeceneCfg.tSceneCfg.tRuleRegion[i].echelonArea), scene::EMCUSTOMTYPE_AREA_1 + i, m_tSeceneCfg.tSceneCfg.tRuleRegion[i].Label);
			m_pixDraw->SetColorByType(scene::EMCUSTOMTYPE_AREA_1 + i, COLOR_PURPLE);
		}
	}

	for (int i = 0; i < USER_RULE_REGION_MAX; i++)
	{
		if (m_tSeceneCfg.tSceneCfg.tPolygon[i].enable)
		{
			m_pixDraw->AddPolygon(&(m_tSeceneCfg.tSceneCfg.tPolygon[i].area), scene::EMCUSTOMTYPE_POLYGON_1 + i,
				m_tSeceneCfg.tSceneCfg.tPolygon[i].Label);
			m_pixDraw->SetColorByType(scene::EMCUSTOMTYPE_POLYGON_1 + i, COLOR_PURPLE);
		}
	}
	*/
}


void Dlg_LogicLane::Slot_SceneInfoChanged()
{
	OnChangeScenceNum();
	OnChangePreset(m_tSeceneCfg.tSceneCfg.wPrePosId);
	GetParamLogicLane();

	if (m_tLocalCfgInfo.ispic == 0)
	{
		ui.video->SetPixSize(m_tSeceneCfg.tMaxParam.dwMaxWidth, m_tSeceneCfg.tMaxParam.dwMaxHeight);
		ui.video->SetPicDrawSize(600, 400);
		ui.video->SetScenceDrawData(&m_tSeceneCfg);
	}
	else
	{
		 SetScenceDrawData();
	}
}

void Dlg_LogicLane::Slot_CmbIndexChanged(int nIndex, int nColmn)
{
	QTreeWidgetItem *pItem = ui.treeWidget->topLevelItem(nIndex);
	TIPC_LogicLane& tLogicLane = m_tLogicLaneInfo.tLogicLane[nIndex];
	int nType = logiclane::EMLOGICLANE_SN;
	
	if (this->sender() == m_chkEnable)
	{
		tLogicLane.bEnable = (pItem->text(1) == GBUTF8("是") ? true : false);
		nType = logiclane::EMLOGICLANE_ENABLED;
	}
	else if (this->sender() == m_chkFaceExposure)
	{
		tLogicLane.bFaceMode = (pItem->text(8) == GBUTF8("是") ? true : false);
		nType = logiclane::EMLOGICLANE_EXPOSURE_PEOPLE;
	}
	else if (this->sender() == m_cmbTestType)
	{
		tLogicLane.dwLgRdType = logiclane::AnalyIndex2CheckType(nColmn);
		tLogicLane.dwExtLgRdType = logiclane::AnalyIndex2CheckExtType(nColmn);
		nType = logiclane::EMLOGICLANE_CHKTYPE;
	}
	else if (this->sender() == m_cmbSensitive)
	{
		tLogicLane.dwSensitivity = nColmn+1; //m_tTipCtrl.pCmb_Sensitivity->GetCmbValue() + 1;
		nType = logiclane::EMLOGICLANE_SENSITIVITY;
	}
	else if (this->sender() == m_cmbRecogArea)
	{
		tLogicLane.dwRecAreaID = nColmn+1;
		nType = logiclane::EMLOGICLANE_IDENTID;
	}
	else if (this->sender() == m_cmbLaneID)
	{
		tLogicLane.dwLaneId = nColmn+1;
		nType = logiclane::EMLOGICLANE_LANEID;
	}
	else if (this->sender() == m_cmbWhiteLight)
	{
		tLogicLane.dwDaySnapLight = logiclane::AnalyIndex2LightMode(nColmn);
		nType = logiclane::EMLOGICLANE_FILLLIGHT_DAY;
	}
	else if (this->sender() == m_cmbNightLight)
	{
		tLogicLane.dwNightSnapLight = logiclane::AnalyIndex2LightMode(nColmn);
		nType = logiclane::EMLOGICLANE_FILLLIGHT_NIGHT;
	}
	
	//UpdateListItem(nIndex, nType);
}

void Dlg_LogicLane::OnBuildLoginLane()
{
	u32 dwIP = MgrData::getInstance()->GetIP();
	ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dwIP);
	if (pCtrl != NULL)
	{
		int sceneid = m_nSceneIndex + 1;
		bool bRet = pCtrl->BuildLogicLane(&sceneid, (u32)(CObserver*)this);
		if (bRet)
		{
			//GetParamLogicLane();
		}
	}
}


void Dlg_LogicLane::Slot_BtnLoadSceneClicked()
{
	if (m_tLocalCfgInfo.ispic)
	{
		OnSnap();
	}
}

void Dlg_LogicLane::SetParamLogiclane()
{
	MgrItsCtrl* pMgrItsCtrl = MgrData::getInstance()->GetMgrItsCtrl();
	if (pMgrItsCtrl != NULL)
	{
		u32 dwIP = MgrData::getInstance()->GetIP();
		ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dwIP);
		if (pCtrl != NULL)
		{
			bool bRet = pCtrl->SetIpcLogicLane(&m_tLogicLaneInfo);

			if (!bRet)
			{
			}
		}
	}
}


void Dlg_LogicLane::Slot_BtnCreateLogicLaneClicked()
{
	OnBuildLoginLane();
}

void Dlg_LogicLane::Slot_BtnFreshClicked()
{
	GetParamScenceInfo();
}

void Dlg_LogicLane::Slot_BtnSaveClicked()
{
	SetParamLogiclane();
	if (!m_messageBox)
	{
		QTimer::singleShot(1500, this, &Dlg_LogicLane::OnSetTxtVisible);
		m_messageBox = new Dlg_MessageBox;
		m_messageBox->SetInfoText(GBUTF8("保存成功！"));
		m_messageBox->SetBtnNoVisible(false);
		m_messageBox->exec();
	}
}

void Dlg_LogicLane::Slot_TreeWidgetItemClicked(QTreeWidgetItem*pItem, int column)
{
	if (column != 0)
	{
		pItem->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
	}
	else
	{
		pItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
	}
}

void Dlg_LogicLane::Slot_CmbSenceIndexChanged(int nIndex)
{
	if ((nIndex == -1) || (m_nSceneIndex == nIndex))
	{
		this->sender()->setProperty("id", 0);
		return;
	}
	if (this->sender()->property("id").toInt() == 111)
	{
		this->sender()->setProperty("id", 0);
		return;
	}
	m_nSceneIndex = nIndex;
	GetParamScenceInfo();
}

void Dlg_LogicLane::Slot_VideoLoopInfoChanged()
{
	if (m_tLocalCfgInfo.ispic == 0)
	{
		ui.video->SetPixSize(m_tSeceneCfg.tMaxParam.dwMaxWidth, m_tSeceneCfg.tMaxParam.dwMaxHeight);
		ui.video->SetPicDrawSize(600, 400);
		ui.video->SetLoopDrawData(&m_tVideoLoopInfo);
	}
	else
	{
		//SetLoopDrawData();
	}
}

// 识别区域信息获取
void Dlg_LogicLane::GetParamRecogAreaInfo()
{
	MgrItsCtrl* pMgrItsCtrl = MgrData::getInstance()->GetMgrItsCtrl();
	if (pMgrItsCtrl != NULL)
	{
		u32 dwIP = MgrData::getInstance()->GetIP();
		ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dwIP);
		if (pCtrl != NULL)
		{
			int sceneid = m_nSceneIndex + 1;
			bool bRet = pCtrl->GetIpcRecogArea(&sceneid, (u32)(CObserver*)this);
			if (!bRet)
			{
			}
		}
	}
}

void Dlg_LogicLane::GetParamScenceInfo()
{
	MgrItsCtrl* pMgrItsCtrl = MgrData::getInstance()->GetMgrItsCtrl();
	if (pMgrItsCtrl != NULL)
	{
		u32 dwIP = MgrData::getInstance()->GetIP();
		ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dwIP);
		if (pCtrl != NULL)
		{
			int sceneid = m_nSceneIndex + 1;
			bool bRet = pCtrl->GetIpcScence(&sceneid, (u32)(CObserver*)this);
			if (!bRet)
			{
			}
		}
	}
}

void Dlg_LogicLane::Slot_RecogAreaChanged()
{

}
void Dlg_LogicLane::OnObserverNotify(LPARAM lHint, LPVOID pHint)
{
	u32 dwEvtType = (u32)lHint;
	u32 dwId = (u32)pHint;

	switch (dwEvtType)
	{

	case NMC_GET_IPC_LOGICLANEINFO:
	{
		TIPC_GetLogicLaneRsp* p = (TIPC_GetLogicLaneRsp*)pHint;
		if (p->nRetCode == 0)
		{
			m_tLogicLaneInfo.dwSceneId = p->dwSceneId;
			m_tLogicLaneInfo.wLogicLaneNum = p->wLogicLaneNum;
			for (int i = 0; i < LOGIC_ROAD_NUM_MAX; i++)
			{
				memcpy(&(m_tLogicLaneInfo.tLogicLane[i]), &(p->tLogicLane[i]), sizeof(TIPC_LogicLane));
			}
			emit signal_logicLaneInfoChanged();
		}
	}
		break;
	case NMC_GET_IPC_SCENCEINFO:
	{
		TIPC_GetSceneCfgRsp* p = (TIPC_GetSceneCfgRsp*)pHint;
		if (p->nRetCode == 0)
		{
			memcpy(&m_tSeceneCfg, p, sizeof(TIPC_GetSceneCfgRsp));
			emit signal_sceneInfoChanged();
		}
	}
		break;
	case NMC_GET_IPC_VIDEOLOOPINFO:
	{
		TIPC_GetVideoLoopRsp* p = (TIPC_GetVideoLoopRsp*)pHint;
		if (p->nRetCode == 0)
		{
			m_tVideoLoopInfo.dwSceneId = p->dwSceneId;

			memcpy(&(m_tVideoLoopInfo.tVideoLoop), &(p->tVideoLoop), sizeof(p->tVideoLoop));
			emit signal_videoLoopInfoChanged();
		}
	}
		break;

	case NMC_GET_IPC_RECOGAREA:
	{
		TIPC_GetRecogAreaRsp* p = (TIPC_GetRecogAreaRsp*)pHint;
		if (p->nRetCode == 0)
		{
			memcpy(&(m_tRecogAreaInfo.tRecogArea), &(p->tRecogArea), sizeof(TIPC_RecogArea) * ROAD_NUM_MAX);

			BOOL bHandled = TRUE;
			emit signal_recogAreaChanged();
			
		}
	}
		break;
	case NMC_EVT_BUILDLOGICLANE:
	{
		GetParamLogicLane();
	}
		break;
	case NMC_EVT_VIDEO_PIC:
	{
		BOOL bHandled;
		OnRefreshPic(0, (WPARAM)0, 0, bHandled);
	}
		break;
	case NMC_GET_IPC_SCENCE_ID:
	{
		u32 scenceId = *(u32*)pHint;
		m_nSceneIndex = scenceId;
		ui.cmb_sence->setCurrentIndex(m_nSceneIndex);
		//GetParamScenceInfo();
	}
		break;
	default:
		break;
	}
}



LRESULT Dlg_LogicLane::OnRefreshPic(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

	u32 dwId = (u32)wParam;
	// 图片浏览取缓冲区中的图片
	if (uMsg == NMC_EVT_VIDEO_PIC)
		dwId = 0;
	/*if (m_pScenceDrawUI && m_pScenceDrawUI->IsVisible())
	{
	m_pScenceDrawUI->SetNewPic(dwId, lParam);
	CRect rcPaint = m_pScenceDrawUI->GetPos();
	InvalidateRect(*this, rcPaint, TRUE);
	}*/

	//
	bHandled = TRUE;
	return 0;
}

