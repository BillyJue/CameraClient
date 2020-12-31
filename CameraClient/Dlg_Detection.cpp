#include "Dlg_Detection.h"
#include "MgrData.h"
#include <QTreeWidget>
#include <QPushButton>
#include "Dlg_PictureDraw.h"
#include "Dlg_MessageBox.h"
#include "Dlg_Detection_Area.h"
#include "Dlg_Detection_Phase.h"
#include <QVBoxLayout>
#include <QListView>
#include <QTimer>
Dlg_Detection::Dlg_Detection(QWidget *parent)
	: MyWidget(parent), m_bTrapezium(false), m_nTabIndex(0)
	, m_nSceneIndex(0), m_bChange(false), m_messageBox(nullptr)
{
	memset(&m_tVideoLoopInfo, 0, sizeof(TIPC_SetVideoLoopReq));
	memset(&m_tSeceneCfg, 0, sizeof(TIPC_SetSceneCfgReq));
	memset(&m_tRecogAreaInfo, 0, sizeof(TIPC_SetRecogAreaReq));
	memset(&m_tCoilLoopInfo, 0, sizeof(TIPC_SetCoilLoopReq));
	memset(&m_tRedarLoopInfo, 0, sizeof(TIPC_SetRadarLoopReq));
	memset(&m_tRedLampSigInfo, 0, sizeof(TIPC_SetRedLampSigReq));
	memset(&m_tRecogAreaParam, 0, sizeof(m_tRecogAreaParam));
	memset(&m_tSetRedEnInfo, 0, sizeof(m_tSetRedEnInfo));

	ui.setupUi(this);
	ui.cmb_scence->setView(new QListView);
	MgrData::getInstance()->GetMgrSubject()->Add(this);
	// 添加电警过程数据的接收通知窗口
	if (MgrData::getInstance()->GetMgrEvtLst())
	{
		MgrData::getInstance()->GetMgrEvtLst()->AddSender(static_cast<CObserver*>(this));
	}
	InitVideoLoop();
	InitCoil();
	InitRadar();
	InitRedLampSig();
	InitRedLampIncrease();
	InitRecogArea();
	
	connect(this, SIGNAL(signal_videoLoopChanged()), this, SLOT(Slot_VideoLoopDataChanged()));
	connect(ui.tabWidget, SIGNAL(currentChanged(int)), this, SLOT(Slot_TabWidgetCurrentChanged(int)));
	connect(this, SIGNAL(signal_scenceinfo()), this, SLOT(Slot_ScenceInfoChanged()));
	connect(this, SIGNAL(signal_recogAreaInfo()), this, SLOT(Slot_RecogAreaInfoChanged()));
	connect(this, SIGNAL(signal_redarLoop()), this, SLOT(Slot_RedarLoopChanged()));
	connect(this, SIGNAL(signal_redEnhloop()), this, SLOT(Slot_ReadEnhloopDataChanged()));
	connect(this, SIGNAL(signal_coilLoop()), this, SLOT(Slot_CoilLoopChanged()));
	connect(this, SIGNAL(signal_redSigLoop()), this, SLOT(Slot_RedSigLoopChanged()));
	connect(this, SIGNAL(signal_createLoop()), this, SLOT(Slot_CreateLoopChanged()));
	connect(ui.cmb_scence, SIGNAL(currentIndexChanged(int)), this, SLOT(Slot_ScenceIndexChanged(int)));
	connect(ui.btn_loadScene, SIGNAL(clicked()), this, SLOT(Slot_LoadSceneClicked()));
	connect(ui.btn_save, SIGNAL(clicked()), this, SLOT(Slot_BtnSaveClicked()));
	connect(ui.btn_fresh, SIGNAL(clicked()), this, SLOT(Slot_BtnFreshClicked()));
	connect(ui.widget_3, SIGNAL(signal_dataDrawChanged()), this, SLOT(Slot_DataDrawChanged()));
	connect(ui.btn_generatingDetector, SIGNAL(clicked()), this, SLOT(Slot_BtnGenerator()));
	

	//test
	//InitData();

}

void Dlg_Detection::OnSetTxtVisible()
{
	if (m_messageBox)
	{
		m_messageBox->close();
		SAFE_DELETE(m_messageBox);
	}
}

void Dlg_Detection::Slot_BtnGenerator()
{
	OnCreateVideoLoop();
}

void Dlg_Detection::OnCreateVideoLoop()
{
	u32 dwIP = MgrData::getInstance()->GetIP();
	ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dwIP);
	if (pCtrl == NULL)
	{
		return;
	}

	m_tVideoLoopInfo.dwSceneId = m_nSceneIndex + 1;
	pCtrl->SetIpcCreateLoop(&m_tVideoLoopInfo.dwSceneId, (u32)(CObserver*)this);
}

void Dlg_Detection::Slot_DataDrawChanged()
{
	DrawDataToList();
}

void Dlg_Detection::SetParamVideoLoopInfo()
{
	u32 dwIP = MgrData::getInstance()->GetIP();
	ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dwIP);
	if (pCtrl == NULL)
	{
		return;
	}

	TIPC_SetVideoLoopReq tVideoLoop;
	m_tVideoLoopInfo.dwSceneId = m_nSceneIndex + 1;
	memcpy(&tVideoLoop, &m_tVideoLoopInfo, sizeof(tVideoLoop));
	pCtrl->SetIpcVideoLoop(&tVideoLoop);
}

void Dlg_Detection::SetParamCoilLoopInfo()
{
	u32 dwIP = MgrData::getInstance()->GetIP();
	ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dwIP);
	if (pCtrl == NULL)
	{
		return;
	}

	m_tCoilLoopInfo.dwSceneId = m_nSceneIndex + 1;
	pCtrl->SetIpcCoilLoop(&m_tCoilLoopInfo);
}

void Dlg_Detection::SetParamRedarLoopInfo()
{
	u32 dwIP = MgrData::getInstance()->GetIP();
	ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dwIP);
	if (pCtrl == NULL)
	{
		return;
	}

	m_tRedarLoopInfo.dwSceneId = m_nSceneIndex + 1;
	pCtrl->SetIpcRedarLoop(&m_tRedarLoopInfo);
}




void Dlg_Detection::OnSave()
{
	switch (m_nTabIndex)
	{
	case 0:
		SetParamVideoLoopInfo();
		break;
	case 1:
		SetParamCoilLoopInfo();
		break;
	case 2:
		SetParamRedarLoopInfo();
		break;
	case 3:
		SetParamRedLampSigInfo();
		break;
	case 4:
		SetParamRecogAreaInfo();
		break;
	case 5:
		SetParamRedEnhanceInfo();
		break;
	}

	m_bChange = false;
}

void Dlg_Detection::SetParamRedEnhanceInfo()
{
	u32 dwIP = MgrData::getInstance()->GetIP();
	ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dwIP);
	if (pCtrl == NULL)
	{
		return;
	}

	m_tSetRedEnInfo.dwSceneId = m_nSceneIndex + 1;
	pCtrl->SetIpcRedEnhLoop(&m_tSetRedEnInfo);
}



void Dlg_Detection::RecogAreaConvert2(recogarea::TRecogAreaParam *ptRecogAreaParam, TIPC_SetRecogAreaReq *ptIPC_SetRecogAreaReq)
{
	for (int i = 0; i < ROAD_NUM_MAX; i++)
	{
		ptIPC_SetRecogAreaReq->tRecogArea[i].tArea.X1 = ptRecogAreaParam[i].X1;
		ptIPC_SetRecogAreaReq->tRecogArea[i].tArea.Y1 = ptRecogAreaParam[i].Y1;
		ptIPC_SetRecogAreaReq->tRecogArea[i].tArea.X2 = ptRecogAreaParam[i].X2;
		ptIPC_SetRecogAreaReq->tRecogArea[i].tArea.Y2 = ptRecogAreaParam[i].Y2;
		ptIPC_SetRecogAreaReq->tRecogArea[i].tArea.W1 = ptRecogAreaParam[i].W1;
		ptIPC_SetRecogAreaReq->tRecogArea[i].tArea.W2 = ptRecogAreaParam[i].W2;
		ptIPC_SetRecogAreaReq->tRecogArea[i].dwPlateWidth = ptRecogAreaParam[i].dwPlateWidth;

		switch (ptRecogAreaParam[i].dwShootRear)
		{
		case 0:
			ptIPC_SetRecogAreaReq->tRecogArea[i].dwShootRear = DETECT_HEAD_VIEW;
			break;
		case 1:
			ptIPC_SetRecogAreaReq->tRecogArea[i].dwShootRear = DETECT_TAIL_VIEW;
			break;
		default:
			ptIPC_SetRecogAreaReq->tRecogArea[i].dwShootRear = DETECT_HEAD_VIEW;
			break;
		}

		switch (ptRecogAreaParam[i].dwPlateAngle)
		{
		case 0:
			ptIPC_SetRecogAreaReq->tRecogArea[i].dwPlateAngle = PLATE_ANGLE_C;
			break;
		case 1:
			ptIPC_SetRecogAreaReq->tRecogArea[i].dwPlateAngle = PLATE_ANGLE_A_R;
			break;
		case 2:
			ptIPC_SetRecogAreaReq->tRecogArea[i].dwPlateAngle = PLATE_ANGLE_A_L;
			break;
		case 3:
			ptIPC_SetRecogAreaReq->tRecogArea[i].dwPlateAngle = PLATE_ANGLE_UN;
			break;
		case 4:
			ptIPC_SetRecogAreaReq->tRecogArea[i].dwPlateAngle = PLATE_ANGLE_A_R_R;
			break;
		case 5:
			ptIPC_SetRecogAreaReq->tRecogArea[i].dwPlateAngle = PLATE_ANGLE_A_L_L;
			break;
		case 6:
			ptIPC_SetRecogAreaReq->tRecogArea[i].dwPlateAngle = PLATE_ANGLE_A_UN_UN;
			break;
		default:
			ptIPC_SetRecogAreaReq->tRecogArea[i].dwPlateAngle = PLATE_ANGLE_C;
			break;
		}

	}
}




void Dlg_Detection::SetParamRecogAreaInfo()
{
	u32 dwIP = MgrData::getInstance()->GetIP();
	ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dwIP);
	if (pCtrl == NULL)
	{
		return;
	}

	RecogAreaConvert2(m_tRecogAreaParam, &m_tRecogAreaInfo);
	m_tRecogAreaInfo.dwSceneId = m_nSceneIndex + 1;
	pCtrl->SetIpcRecogArea(&m_tRecogAreaInfo);
}

void Dlg_Detection::SetParamRedLampSigInfo()
{
	u32 dwIP = MgrData::getInstance()->GetIP();
	ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dwIP);
	if (pCtrl == NULL)
	{
		return;
	}

	m_tRedLampSigInfo.dwSceneId = m_nSceneIndex + 1;
	pCtrl->SetIpcRedSigLoop(&m_tRedLampSigInfo);
}

void Dlg_Detection::Slot_BtnFreshClicked()
{
	switch (m_nTabIndex)
	{
	case 0:
		GetParamVideoLoopInfo();
		break;
	case 1:
		GetParamCoilLoopInfo();
		break;
	case 2:
		GetParamRedarLoopInfo();
		break;
	case 3:
		GetParamRedLampSigInfo();
		break;
	case 4:
		GetParamRecogAreaInfo();
		break;
	case 5:
		GetParamRedEnhanceInfo();
		break;
	}

	m_bChange = false;
}

void Dlg_Detection::Slot_BtnSaveClicked()
{
	OnSave();
	if (!m_messageBox)
	{
		QTimer::singleShot(1500, this, &Dlg_Detection::OnSetTxtVisible);
		m_messageBox = new Dlg_MessageBox;
		m_messageBox->SetInfoText(GBUTF8("保存成功！"));
		m_messageBox->SetBtnNoVisible(false);
		m_messageBox->exec();
	}
}

void Dlg_Detection::Slot_LoadSceneClicked()
{
	OnSnap();
}

void Dlg_Detection::Slot_ScenceIndexChanged(int nIndex)
{
	if (this->sender()->property("id").toInt() == 111 || nIndex == -1)
	{
		this->sender()->setProperty("id", 0);
		return;
	}
	m_nSceneIndex = nIndex;
	OnSwitchScene();
}

void Dlg_Detection::ReleaseVideoLoop()
{
	SAFE_DELETE(m_labDelegate);
	SAFE_DELETE(m_cheDelegate);
	SAFE_DELETE(m_editDelegate2);
	SAFE_DELETE(m_editDelegate3);
	SAFE_DELETE(m_editDelegate4);
	SAFE_DELETE(m_editDelegate5);
	SAFE_DELETE(m_editDelegate6);
	SAFE_DELETE(m_editDelegate7);
	SAFE_DELETE(m_editDelegate8);
	SAFE_DELETE(m_editDelegate9);
	SAFE_DELETE(m_editDelegate10);
	SAFE_DELETE(m_editDelegate11);
	SAFE_DELETE(m_editDelegate15);
	SAFE_DELETE(m_editDelegate16);
	SAFE_DELETE(m_editDelegate17);
	SAFE_DELETE(m_editDelegate18);
	SAFE_DELETE(m_editDelegate19);
	SAFE_DELETE(m_editDelegate20);
	SAFE_DELETE(m_comboDelegate12);
	SAFE_DELETE(m_comboDelegate13);
	SAFE_DELETE(m_comboDelegate14);
}

void Dlg_Detection::InitCoil()
{
	m_coil_labDelegate = new LabelDelegate;
	m_coil_chkDelegate = new CheckBoxDelegate;
	m_coil_cmbDelegate2 = new ComboboxDelegate;
	m_coil_cmbDelegate3 = new ComboboxDelegate;
	m_coil_cmbDelegate4 = new ComboboxDelegate;
	m_coil_editDelegate5 = new EditDelegate;
	m_coil_editDelegate6 = new EditDelegate;
	m_coil_editDelegate6->SetValidatorType(1);
	m_coil_cmbDelegate7 = new ComboboxDelegate;
	m_coil_cmbDelegate8 = new ComboboxDelegate;
	m_coil_cmbDelegate9 = new ComboboxDelegate;
	QStringList sList2;
	sList2.append(GBUTF8("串口1"));
	sList2.append(GBUTF8("串口2"));
	sList2.append(GBUTF8("串口3"));
	sList2.append(GBUTF8("串口4"));
	sList2.append(GBUTF8("IO1"));
	sList2.append(GBUTF8("IO2"));
	sList2.append(GBUTF8("IO3"));
	sList2.append(GBUTF8("IO4"));
	m_coil_cmbDelegate2->setComboxData(sList2);
	for (int i = 0; i < sList2.size(); i++)
	{
		m_coil_serial_map[i] = sList2.at(i);
	}
	QStringList sList3;
	sList3.append(GBUTF8("通道1"));
	sList3.append(GBUTF8("通道2"));
	sList3.append(GBUTF8("通道3"));
	sList3.append(GBUTF8("通道4"));
	sList3.append(GBUTF8("通道5"));
	sList3.append(GBUTF8("通道6"));
	sList3.append(GBUTF8("通道7"));
	sList3.append(GBUTF8("通道8"));
	sList3.append(GBUTF8("通道9"));
	for (int i = 0; i < sList3.size(); i++)
	{
		m_coil_way_map[i] = sList3.at(i);
	}
	m_coil_cmbDelegate3->setComboxData(sList2);

	QStringList sList4;
	sList4.append(GBUTF8("车头"));
	sList4.append(GBUTF8("车尾"));
	for (int i = 0; i < sList4.size(); i++)
	{
		m_coil_direction_map[i] = sList4.at(i);
	}
	m_coil_cmbDelegate4->setComboxData(sList4);

	QStringList sList7;
	sList7.append(GBUTF8("IO1"));
	sList7.append(GBUTF8("IO2"));
	sList7.append(GBUTF8("IO3"));
	sList7.append(GBUTF8("IO4"));
	for (int i = 0; i < sList7.size(); i++)
	{
		m_coil_io_map[i] = sList7.at(i);
	}
	m_coil_cmbDelegate7->setComboxData(sList7);

	QStringList sList8;
	sList8.append(GBUTF8("车道1"));
	sList8.append(GBUTF8("车道2"));
	sList8.append(GBUTF8("车道3"));
	sList8.append(GBUTF8("车道4"));
	for (int i = 0; i < sList8.size(); i++)
	{
		m_coil_lane_map[i] = sList8.at(i);
	}
	m_coil_cmbDelegate8->setComboxData(sList8);

	QStringList sList9;
	sList9.append(GBUTF8("前线圈"));
	sList9.append(GBUTF8("中线圈"));
	sList9.append(GBUTF8("后线圈"));
	for (int i = 0; i < sList9.size(); i++)
	{
		m_coil_coilType_map[i] = sList9.at(i);
	}
	m_coil_cmbDelegate9->setComboxData(sList9);
	ui.treeWidget_coil->setItemDelegateForColumn(0, m_coil_labDelegate);
	ui.treeWidget_coil->setItemDelegateForColumn(1, m_coil_chkDelegate);
	ui.treeWidget_coil->setItemDelegateForColumn(2, m_coil_cmbDelegate2);
	ui.treeWidget_coil->setItemDelegateForColumn(3, m_coil_cmbDelegate3);
	ui.treeWidget_coil->setItemDelegateForColumn(4, m_coil_cmbDelegate4);
	ui.treeWidget_coil->setItemDelegateForColumn(5, m_coil_editDelegate5);
	ui.treeWidget_coil->setItemDelegateForColumn(6, m_coil_editDelegate6);
	ui.treeWidget_coil->setItemDelegateForColumn(7, m_coil_cmbDelegate7);
	ui.treeWidget_coil->setItemDelegateForColumn(8, m_coil_cmbDelegate8);
	ui.treeWidget_coil->setItemDelegateForColumn(9, m_coil_cmbDelegate9);

	connect(m_coil_chkDelegate, SIGNAL(signal_dataChanged(int, int)), this, SLOT(Slot_CoilDataChanged(int, int)));
	connect(m_coil_cmbDelegate2, SIGNAL(signal_combDataChanged(int, int)), this, SLOT(Slot_CoilDataChanged(int, int)));
	connect(m_coil_cmbDelegate3, SIGNAL(signal_combDataChanged(int, int)), this, SLOT(Slot_CoilDataChanged(int, int)));
	connect(m_coil_cmbDelegate4, SIGNAL(signal_combDataChanged(int, int)), this, SLOT(Slot_CoilDataChanged(int, int)));
	connect(m_coil_editDelegate5, SIGNAL(signal_dataChanged(int, int)), this, SLOT(Slot_CoilDataChanged(int, int)));
	connect(m_coil_editDelegate6, SIGNAL(signal_dataChanged(int, int)), this, SLOT(Slot_CoilDataChanged(int, int)));
	connect(m_coil_cmbDelegate7, SIGNAL(signal_combDataChanged(int, int)), this, SLOT(Slot_CoilDataChanged(int, int)));
	connect(m_coil_cmbDelegate8, SIGNAL(signal_combDataChanged(int, int)), this, SLOT(Slot_CoilDataChanged(int, int)));
	connect(m_coil_cmbDelegate9, SIGNAL(signal_combDataChanged(int, int)), this, SLOT(Slot_CoilDataChanged(int, int)));
	connect(ui.treeWidget_coil, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(Slot_TreeCoilWidgetItemClicked(QTreeWidgetItem*, int)));

}

void Dlg_Detection::ReleaseCoil()
{
	SAFE_DELETE(m_coil_labDelegate);
	SAFE_DELETE(m_coil_chkDelegate);
	SAFE_DELETE(m_coil_cmbDelegate2);
	SAFE_DELETE(m_coil_cmbDelegate3);
	SAFE_DELETE(m_coil_cmbDelegate4);
	SAFE_DELETE(m_coil_editDelegate5);
	SAFE_DELETE(m_coil_editDelegate6);
	SAFE_DELETE(m_coil_cmbDelegate7);
	SAFE_DELETE(m_coil_cmbDelegate8);
	SAFE_DELETE(m_coil_cmbDelegate9);
}

void Dlg_Detection::ReleaseRedLampIncrease()
{
	SAFE_DELETE(m_redLampIncrease_chkDelegate);
	SAFE_DELETE(m_redLampIncrease_editDelegate);
	SAFE_DELETE(m_redLampIncrease_editDelegate2);
	SAFE_DELETE(m_redLampIncrease_editDelegate3);
	SAFE_DELETE(m_redLampIncrease_editDelegate4);
	SAFE_DELETE(m_redLampIncrease_editDelegate5);
	SAFE_DELETE(m_redLampIncrease_editDelegate6);
	SAFE_DELETE(m_redLampIncrease_editDelegate7);
}

void Dlg_Detection::InitRedLampIncrease()
{
	m_redLampIncrease_chkDelegate = new CheckBoxDelegate;
	m_redLampIncrease_editDelegate = new EditDelegate;
	m_redLampIncrease_editDelegate2 = new EditDelegate;
	m_redLampIncrease_editDelegate3 = new EditDelegate;
	m_redLampIncrease_editDelegate4 = new EditDelegate;
	m_redLampIncrease_editDelegate5 = new EditDelegate;
	m_redLampIncrease_editDelegate6 = new EditDelegate;
	m_redLampIncrease_editDelegate7 = new EditDelegate;
	
	ui.treeWidget_RedLamp->setItemDelegateForColumn(0, m_redLampIncrease_chkDelegate);
	ui.treeWidget_RedLamp->setItemDelegateForColumn(1, m_redLampIncrease_editDelegate);
	ui.treeWidget_RedLamp->setItemDelegateForColumn(2, m_redLampIncrease_editDelegate2);
	ui.treeWidget_RedLamp->setItemDelegateForColumn(3, m_redLampIncrease_editDelegate3);
	ui.treeWidget_RedLamp->setItemDelegateForColumn(4, m_redLampIncrease_editDelegate4);
	ui.treeWidget_RedLamp->setItemDelegateForColumn(5, m_redLampIncrease_editDelegate5);
	ui.treeWidget_RedLamp->setItemDelegateForColumn(6, m_redLampIncrease_editDelegate6);
	ui.treeWidget_RedLamp->setItemDelegateForColumn(7, m_redLampIncrease_editDelegate7);

	connect(m_redLampIncrease_chkDelegate, SIGNAL(signal_dataChanged(int, int)), this, SLOT(Slot_RedLampIncreaseDataChanged(int, int)));
	connect(m_redLampIncrease_editDelegate, SIGNAL(signal_dataChanged(int, int)), this, SLOT(Slot_RedLampIncreaseDataChanged(int, int)));
	connect(m_redLampIncrease_editDelegate2, SIGNAL(signal_dataChanged(int, int)), this, SLOT(Slot_RedLampIncreaseDataChanged(int, int)));
	connect(m_redLampIncrease_editDelegate3, SIGNAL(signal_dataChanged(int, int)), this, SLOT(Slot_RedLampIncreaseDataChanged(int, int)));
	connect(m_redLampIncrease_editDelegate4, SIGNAL(signal_dataChanged(int, int)), this, SLOT(Slot_RedLampIncreaseDataChanged(int, int)));
	connect(m_redLampIncrease_editDelegate5, SIGNAL(signal_dataChanged(int, int)), this, SLOT(Slot_RedLampIncreaseDataChanged(int, int)));
	connect(m_redLampIncrease_editDelegate6, SIGNAL(signal_dataChanged(int, int)), this, SLOT(Slot_RedLampIncreaseDataChanged(int, int)));
	connect(m_redLampIncrease_editDelegate7, SIGNAL(signal_dataChanged(int, int)), this, SLOT(Slot_RedLampIncreaseDataChanged(int, int)));

	connect(ui.treeWidget_RedLamp, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(Slot_TreeRedLampIncreaseWidgetItemClicked(QTreeWidgetItem*, int)));

}

void Dlg_Detection::InitVideoLoop()
{
	m_labDelegate = new LabelDelegate;
	m_cheDelegate = new CheckBoxDelegate;
	m_editDelegate2 = new EditDelegate;
	m_editDelegate3 = new EditDelegate;
	m_editDelegate4 = new EditDelegate;
	m_editDelegate5 = new EditDelegate;
	m_editDelegate6 = new EditDelegate;
	m_editDelegate7 = new EditDelegate;
	m_editDelegate8 = new EditDelegate;
	m_editDelegate9 = new EditDelegate;
	m_editDelegate10 = new EditDelegate;
	m_editDelegate11 = new EditDelegate;
	m_editDelegate15 = new EditDelegate;
	m_editDelegate16 = new EditDelegate;
	m_editDelegate17 = new EditDelegate;
	m_editDelegate18 = new EditDelegate;
	m_editDelegate19 = new EditDelegate;
	m_editDelegate20 = new EditDelegate;
	m_comboDelegate12 = new ComboboxDelegate;
	m_comboDelegate13 = new ComboboxDelegate;
	m_comboDelegate14 = new ComboboxDelegate;
	QStringList sList;
	sList.append(GBUTF8("运动检测"));
	sList.append(GBUTF8("号牌检测"));
	sList.append(GBUTF8("尾灯检测"));
	sList.append(GBUTF8("头灯检测"));
	sList.append(GBUTF8("简单运动检测"));
	sList.append(GBUTF8("红绿灯检测"));
	sList.append(GBUTF8("滞留检测"));
	sList.append(GBUTF8("车头检测器"));
	sList.append(GBUTF8("车尾检测器"));
	sList.append(GBUTF8("行人检测器"));
	m_comboDelegate12->setComboxData(sList);
	m_map1.insert(0, GBUTF8("运动检测"));
	m_map1.insert(1, GBUTF8("号牌检测"));
	m_map1.insert(2, GBUTF8("尾灯检测"));
	m_map1.insert(3, GBUTF8("头灯检测"));
	m_map1.insert(4, GBUTF8("简单运动检测"));
	m_map1.insert(5, GBUTF8("红绿灯检测"));
	m_map1.insert(6, GBUTF8("滞留检测"));
	m_map1.insert(7, GBUTF8("车头检测器"));
	m_map1.insert(8, GBUTF8("车尾检测器"));
	m_map1.insert(9, GBUTF8("行人检测器"));


	QStringList sList2;
	sList2.append(GBUTF8("车头"));
	sList2.append(GBUTF8("车尾"));
	sList2.append(GBUTF8("右转"));
	sList2.append(GBUTF8("直行"));
	sList2.append(GBUTF8("左转"));
	sList2.append(GBUTF8("右转三色"));
	sList2.append(GBUTF8("直行三色"));
	sList2.append(GBUTF8("左转三色"));
	sList2.append(GBUTF8("水平方向"));
	sList2.append(GBUTF8("垂直方向"));
	sList2.append(GBUTF8("任意方向"));

	m_map2.insert(0, GBUTF8("车头"));
	m_map2.insert(1, GBUTF8("车尾"));
	m_map2.insert(2, GBUTF8("右转"));
	m_map2.insert(3, GBUTF8("直行"));
	m_map2.insert(4, GBUTF8("左转"));
	m_map2.insert(5, GBUTF8("右转三色"));
	m_map2.insert(6, GBUTF8("直行三色"));
	m_map2.insert(7, GBUTF8("左转三色"));
	m_map2.insert(8, GBUTF8("水平方向"));
	m_map2.insert(9, GBUTF8("垂直方向"));
	m_map2.insert(10, GBUTF8("任意方向"));
	m_comboDelegate13->setComboxData(sList2);

	QStringList sList3;
	sList3.append(GBUTF8("龙门架(C杆)"));
	sList3.append(GBUTF8("A杆摄像左牌照右"));
	sList3.append(GBUTF8("A杆摄像右牌照左"));
	sList3.append(GBUTF8("角度方向不定"));
	sList3.append(GBUTF8("A杆摄像左牌照右严重变形"));
	sList3.append(GBUTF8("A杆摄像右牌照左严重变形"));
	sList3.append(GBUTF8("角度方向不定严重变形"));
	m_comboDelegate14->setComboxData(sList3);
	m_map3.insert(0, GBUTF8("龙门架(C杆)"));
	m_map3.insert(1, GBUTF8("A杆摄像左牌照右"));
	m_map3.insert(2, GBUTF8("A杆摄像右牌照左"));
	m_map3.insert(3, GBUTF8("角度方向不定"));
	m_map3.insert(4, GBUTF8("A杆摄像左牌照右严重"));
	m_map3.insert(5, GBUTF8("A杆摄像右牌照左严重"));
	m_map3.insert(6, GBUTF8("角度方向不定严重变形"));
	ui.treeWidget_video->setItemDelegateForColumn(0, m_labDelegate);
	ui.treeWidget_video->setItemDelegateForColumn(1, m_cheDelegate);
	ui.treeWidget_video->setItemDelegateForColumn(2, m_editDelegate2);
	ui.treeWidget_video->setItemDelegateForColumn(3, m_editDelegate3);
	ui.treeWidget_video->setItemDelegateForColumn(4, m_editDelegate4);
	ui.treeWidget_video->setItemDelegateForColumn(5, m_editDelegate5);
	ui.treeWidget_video->setItemDelegateForColumn(6, m_editDelegate6);
	ui.treeWidget_video->setItemDelegateForColumn(7, m_editDelegate7);
	ui.treeWidget_video->setItemDelegateForColumn(8, m_editDelegate8);
	ui.treeWidget_video->setItemDelegateForColumn(9, m_editDelegate9);
	ui.treeWidget_video->setItemDelegateForColumn(10, m_editDelegate10);
	ui.treeWidget_video->setItemDelegateForColumn(11, m_editDelegate11);
	ui.treeWidget_video->setItemDelegateForColumn(15, m_editDelegate15);
	ui.treeWidget_video->setItemDelegateForColumn(16, m_editDelegate16);
	ui.treeWidget_video->setItemDelegateForColumn(17, m_editDelegate17);
	ui.treeWidget_video->setItemDelegateForColumn(18, m_editDelegate18);
	ui.treeWidget_video->setItemDelegateForColumn(19, m_editDelegate19);
	ui.treeWidget_video->setItemDelegateForColumn(20, m_editDelegate20);
	ui.treeWidget_video->setItemDelegateForColumn(12, m_comboDelegate12);
	ui.treeWidget_video->setItemDelegateForColumn(13, m_comboDelegate13);
	ui.treeWidget_video->setItemDelegateForColumn(14, m_comboDelegate14);

	connect(m_cheDelegate, SIGNAL(signal_dataChanged(int, int)), this, SLOT(Slot_VideoDataChanged(int, int)));
	connect(m_editDelegate2, SIGNAL(signal_dataChanged(int, int)), this, SLOT(Slot_VideoDataChanged(int, int)));
	connect(m_editDelegate3, SIGNAL(signal_dataChanged(int, int)), this, SLOT(Slot_VideoDataChanged(int, int)));
	connect(m_editDelegate4, SIGNAL(signal_dataChanged(int, int)), this, SLOT(Slot_VideoDataChanged(int, int)));
	connect(m_editDelegate5, SIGNAL(signal_dataChanged(int, int)), this, SLOT(Slot_VideoDataChanged(int, int)));
	connect(m_editDelegate6, SIGNAL(signal_dataChanged(int, int)), this, SLOT(Slot_VideoDataChanged(int, int)));
	connect(m_editDelegate7, SIGNAL(signal_dataChanged(int, int)), this, SLOT(Slot_VideoDataChanged(int, int)));
	connect(m_editDelegate8, SIGNAL(signal_dataChanged(int, int)), this, SLOT(Slot_VideoDataChanged(int, int)));
	connect(m_editDelegate9, SIGNAL(signal_dataChanged(int, int)), this, SLOT(Slot_VideoDataChanged(int, int)));
	connect(m_editDelegate10, SIGNAL(signal_dataChanged(int, int)), this, SLOT(Slot_VideoDataChanged(int, int)));
	connect(m_editDelegate11, SIGNAL(signal_dataChanged(int, int)), this, SLOT(Slot_VideoDataChanged(int, int)));
	connect(m_editDelegate15, SIGNAL(signal_dataChanged(int, int)), this, SLOT(Slot_VideoDataChanged(int, int)));
	connect(m_editDelegate16, SIGNAL(signal_dataChanged(int, int)), this, SLOT(Slot_VideoDataChanged(int, int)));
	connect(m_editDelegate17, SIGNAL(signal_dataChanged(int, int)), this, SLOT(Slot_VideoDataChanged(int, int)));
	connect(m_editDelegate18, SIGNAL(signal_dataChanged(int, int)), this, SLOT(Slot_VideoDataChanged(int, int)));
	connect(m_editDelegate19, SIGNAL(signal_dataChanged(int, int)), this, SLOT(Slot_VideoDataChanged(int, int)));
	connect(m_editDelegate20, SIGNAL(signal_dataChanged(int, int)), this, SLOT(Slot_VideoDataChanged(int, int)));
	connect(m_comboDelegate12, SIGNAL(signal_combDataChanged(int, int)), this, SLOT(Slot_VideoDataChanged(int, int)));
	connect(m_comboDelegate13, SIGNAL(signal_combDataChanged(int, int)), this, SLOT(Slot_VideoDataChanged(int, int)));
	connect(m_comboDelegate14, SIGNAL(signal_combDataChanged(int, int)), this, SLOT(Slot_VideoDataChanged(int, int)));
	
	connect(ui.treeWidget_video, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(Slot_TreeVideoWidgetItemClicked(QTreeWidgetItem*, int)));
	
}

void Dlg_Detection::ReleaseRecogArea()
{
	SAFE_DELETE(m_recogArea_labDelegate);
	SAFE_DELETE(m_recogArea_chkDelegate);
	SAFE_DELETE(m_recogArea_editDelegate2);
	SAFE_DELETE(m_recogArea_editDelegate3);
	SAFE_DELETE(m_recogArea_editDelegate4);
	SAFE_DELETE(m_recogArea_editDelegate5);
	SAFE_DELETE(m_recogArea_editDelegate6);
	SAFE_DELETE(m_recogArea_editDelegate7);
	SAFE_DELETE(m_recogArea_editDelegate8);

	SAFE_DELETE(m_recogArea_cmbDelegat9);
	SAFE_DELETE(m_recogArea_cmbDelegat10);

}

void Dlg_Detection::InitRecogArea()
{
	m_recogArea_labDelegate = new LabelDelegate;
	m_recogArea_chkDelegate = new CheckBoxDelegate;
	m_recogArea_editDelegate2 = new EditDelegate;
	m_recogArea_editDelegate3 = new EditDelegate;
	m_recogArea_editDelegate4 = new EditDelegate;
	m_recogArea_editDelegate5 = new EditDelegate;
	m_recogArea_editDelegate6 = new EditDelegate;
	m_recogArea_editDelegate7 = new EditDelegate;
	m_recogArea_editDelegate8 = new EditDelegate;
	m_recogArea_cmbDelegat9 = new ComboboxDelegate;
	m_recogArea_cmbDelegat10 = new ComboboxDelegate;

	ui.treeWidget_recogArea->setItemDelegateForColumn(0, m_recogArea_labDelegate);
	ui.treeWidget_recogArea->setItemDelegateForColumn(1, m_recogArea_chkDelegate);
	ui.treeWidget_recogArea->setItemDelegateForColumn(2, m_recogArea_editDelegate2);
	ui.treeWidget_recogArea->setItemDelegateForColumn(3, m_recogArea_editDelegate3);
	ui.treeWidget_recogArea->setItemDelegateForColumn(4, m_recogArea_editDelegate4);
	ui.treeWidget_recogArea->setItemDelegateForColumn(5, m_recogArea_editDelegate5);
	ui.treeWidget_recogArea->setItemDelegateForColumn(6, m_recogArea_editDelegate6);
	ui.treeWidget_recogArea->setItemDelegateForColumn(7, m_recogArea_editDelegate7);
	ui.treeWidget_recogArea->setItemDelegateForColumn(8, m_recogArea_editDelegate8);
	ui.treeWidget_recogArea->setItemDelegateForColumn(9, m_recogArea_cmbDelegat9);
	ui.treeWidget_recogArea->setItemDelegateForColumn(10, m_recogArea_cmbDelegat10);

	QStringList sList9;
	sList9.append(GBUTF8("龙门架(C杆)"));
	sList9.append(GBUTF8("A杆摄像左牌照右"));
	sList9.append(GBUTF8("A杆摄像右牌照左"));
	sList9.append(GBUTF8("角度方向不定"));
	sList9.append(GBUTF8("A杆摄像左牌照右严重变形"));
	sList9.append(GBUTF8("A杆摄像右牌照左严重变形"));
	sList9.append(GBUTF8("角度方向不定严重变形"));
	for (int i = 0; i < sList9.size(); i++)
	{
		m_recogArea_map1[i] = sList9.at(i);
	}
	m_recogArea_cmbDelegat9->setComboxData(sList9);

	QStringList sList10;
	sList10.append(GBUTF8("车头"));
	sList10.append(GBUTF8("车尾"));
	for (int i = 0; i < sList10.size(); i++)
	{
		m_recogArea_map2[i] = sList10.at(i);
	}
	m_recogArea_cmbDelegat10->setComboxData(sList10);

	connect(m_recogArea_chkDelegate, SIGNAL(signal_dataChanged(int, int)), this, SLOT(Slot_RecogAreaDataChanged(int, int)));
	connect(m_recogArea_editDelegate2, SIGNAL(signal_dataChanged(int, int)), this, SLOT(Slot_RecogAreaDataChanged(int, int)));
	connect(m_recogArea_editDelegate3, SIGNAL(signal_dataChanged(int, int)), this, SLOT(Slot_RecogAreaDataChanged(int, int)));
	connect(m_recogArea_editDelegate4, SIGNAL(signal_dataChanged(int, int)), this, SLOT(Slot_RecogAreaDataChanged(int, int)));
	connect(m_recogArea_editDelegate5, SIGNAL(signal_dataChanged(int, int)), this, SLOT(Slot_RecogAreaDataChanged(int, int)));
	connect(m_recogArea_editDelegate6, SIGNAL(signal_dataChanged(int, int)), this, SLOT(Slot_RecogAreaDataChanged(int, int)));
	connect(m_recogArea_editDelegate7, SIGNAL(signal_dataChanged(int, int)), this, SLOT(Slot_RecogAreaDataChanged(int, int)));
	connect(m_recogArea_editDelegate8, SIGNAL(signal_dataChanged(int, int)), this, SLOT(Slot_RecogAreaDataChanged(int, int)));
	connect(m_recogArea_cmbDelegat9, SIGNAL(signal_combDataChanged(int, int)), this, SLOT(Slot_RecogAreaDataChanged(int, int)));
	connect(m_recogArea_cmbDelegat10, SIGNAL(signal_combDataChanged(int, int)), this, SLOT(Slot_RecogAreaDataChanged(int, int)));

	connect(ui.treeWidget_recogArea, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(Slot_TreeRecogAreaWidgetItemClicked(QTreeWidgetItem*, int)));

}

void Dlg_Detection::ReleaseRedLampSig()
{
	SAFE_DELETE(m_redLampSig_labDelegate);
	SAFE_DELETE(m_redLampSig_chkDelegate);
	SAFE_DELETE(m_redLampSig_cmbDelegate2);
	SAFE_DELETE(m_redLampSig_cmbDelegate3);
}

void Dlg_Detection::InitRedLampSig()
{
	m_redLampSig_labDelegate = new LabelDelegate;
	m_redLampSig_chkDelegate = new CheckBoxDelegate;
	m_redLampSig_cmbDelegate2 = new ComboboxDelegate;
	m_redLampSig_cmbDelegate3 = new ComboboxDelegate;

	QStringList sList2;
	for (int i = 0; i < 4; i++)
	{
		sList2.append(GBUTF8("串口%1").arg(i+1));
	}
	m_redLampSig_cmbDelegate2->setComboxData(sList2);

	QStringList sList3;
	for (int i = 0; i < 16; i++)
	{
		sList3.append(GBUTF8("通道%1").arg(i + 1));
	}
	m_redLampSig_cmbDelegate3->setComboxData(sList3);
	connect(m_redLampSig_chkDelegate, SIGNAL(signal_dataChanged(int, int)), this, SLOT(Slot_RedLampSigDataChanged(int, int)));
	connect(m_redLampSig_cmbDelegate2, SIGNAL(signal_combDataChanged(int, int)), this, SLOT(Slot_RedLampSigDataChanged(int, int)));
	connect(m_redLampSig_cmbDelegate3, SIGNAL(signal_combDataChanged(int, int)), this, SLOT(Slot_RedLampSigDataChanged(int, int)));
	ui.treeWidget_RedLampSig->setItemDelegateForColumn(0, m_redLampSig_labDelegate);
	ui.treeWidget_RedLampSig->setItemDelegateForColumn(1, m_redLampSig_chkDelegate);
	ui.treeWidget_RedLampSig->setItemDelegateForColumn(2, m_redLampSig_cmbDelegate2);
	ui.treeWidget_RedLampSig->setItemDelegateForColumn(3, m_redLampSig_cmbDelegate3);

	connect(ui.treeWidget_RedLampSig, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(Slot_TreeRedLampSigWidgetItemClicked(QTreeWidgetItem*, int)));

}

void Dlg_Detection::ReleaseRadar()
{
	SAFE_DELETE(m_radar_labDelegate);
	SAFE_DELETE(m_radar_chkDelegate);
	SAFE_DELETE(m_radar_cmbDelegate2);
	SAFE_DELETE(m_radar_cmbDelegate3);
	SAFE_DELETE(m_radar_editDelegate4);
	SAFE_DELETE(m_radar_editDelegate5);
	SAFE_DELETE(m_radar_cmbDelegate6);
	
}

void Dlg_Detection::InitRadar()
{
	m_radar_labDelegate = new LabelDelegate;
	m_radar_chkDelegate = new CheckBoxDelegate;
	m_radar_cmbDelegate2 = new ComboboxDelegate;
	m_radar_cmbDelegate3 = new ComboboxDelegate;
	m_radar_editDelegate4 = new EditDelegate;
	m_radar_editDelegate5 = new EditDelegate;
	m_radar_cmbDelegate6 = new ComboboxDelegate;
	m_radar_editDelegate4->SetValidatorType(1);
	m_radar_editDelegate5->SetValidatorType(1);

	QStringList sList2;
	sList2.append(GBUTF8("串口1"));
	sList2.append(GBUTF8("串口2"));
	sList2.append(GBUTF8("串口3"));
	m_radar_cmbDelegate2->setComboxData(sList2);

	QStringList sList3;
	sList3.append(GBUTF8("单目标雷达"));
	sList3.append(GBUTF8("多目标雷达"));
	sList3.append(GBUTF8("声呐雷达"));
	m_radar_cmbDelegate3->setComboxData(sList3);

	QStringList sList6;
	sList6.append(GBUTF8("车道1"));
	sList6.append(GBUTF8("车道2"));
	sList6.append(GBUTF8("车道3"));
	m_radar_cmbDelegate6->setComboxData(sList6);

	connect(m_radar_chkDelegate, SIGNAL(signal_dataChanged(int, int)), this, SLOT(Slot_RadarDataChanged(int, int)));
	connect(m_radar_cmbDelegate2, SIGNAL(signal_combDataChanged(int, int)), this, SLOT(Slot_RadarDataChanged(int, int)));
	connect(m_radar_cmbDelegate3, SIGNAL(signal_combDataChanged(int, int)), this, SLOT(Slot_RadarDataChanged(int, int)));
	connect(m_radar_editDelegate4, SIGNAL(signal_dataChanged(int, int)), this, SLOT(Slot_RadarDataChanged(int, int)));
	connect(m_radar_editDelegate5, SIGNAL(signal_dataChanged(int, int)), this, SLOT(Slot_RadarDataChanged(int, int)));
	connect(m_radar_cmbDelegate6, SIGNAL(signal_combDataChanged(int, int)), this, SLOT(Slot_RadarDataChanged(int, int)));

	ui.treeWidget_redar->setItemDelegateForColumn(0, m_radar_labDelegate);
	ui.treeWidget_redar->setItemDelegateForColumn(1, m_radar_chkDelegate);
	ui.treeWidget_redar->setItemDelegateForColumn(2, m_radar_cmbDelegate2);
	ui.treeWidget_redar->setItemDelegateForColumn(3, m_radar_cmbDelegate3);
	ui.treeWidget_redar->setItemDelegateForColumn(4, m_radar_editDelegate4);
	ui.treeWidget_redar->setItemDelegateForColumn(5, m_radar_editDelegate5);
	ui.treeWidget_redar->setItemDelegateForColumn(6, m_radar_cmbDelegate6);

	connect(ui.treeWidget_redar, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(Slot_TreeRadarWidgetItemClicked(QTreeWidgetItem*, int)));

}

void Dlg_Detection::Slot_RedLampIncreaseDataChanged(int nRow, int nColumn)
{
	m_bChange = true;
	int nCol = 0;
	TIPC_RedSigEnhance &tRedEnLoop = m_tSetRedEnInfo.tRedEnLoop[nRow];
	QTreeWidgetItem *pItem = ui.treeWidget_redar->topLevelItem(nRow);
	if (this->sender() == m_redLampIncrease_chkDelegate)
	{
		nCol = 0;
		tRedEnLoop.bEnable = (pItem->text(0) == GBUTF8("是") ? true : false);
	}
	else if (this->sender() == m_redLampIncrease_editDelegate)
	{
		nCol = 1;
		tRedEnLoop.dwStartX = pItem->text(1).toInt();
	}
	else if (this->sender() == m_redLampIncrease_editDelegate2)
	{
		nCol = 2;
		tRedEnLoop.dwStartY = pItem->text(2).toInt();
	}
	else if (this->sender() == m_redLampIncrease_editDelegate3)
	{
		nCol = 3;
		tRedEnLoop.dwWidth = pItem->text(3).toInt();
	}
	else if (this->sender() == m_redLampIncrease_editDelegate4)
	{
		nCol = 4;
		tRedEnLoop.dwHeight = pItem->text(4).toInt();
	}
	else if (this->sender() == m_redLampIncrease_editDelegate5)
	{
		nCol = 5;
		tRedEnLoop.dwPixMin = pItem->text(5).toInt();
	}
	else if (this->sender() == m_redLampIncrease_editDelegate6)
	{
		nCol = 6;
		tRedEnLoop.dwLevelDay = pItem->text(6).toInt();
	}
	else if (this->sender() == m_redLampIncrease_editDelegate7)
	{
		nCol = 7;
		tRedEnLoop.dwLevelNight = pItem->text(7).toInt();
	}
	pItem->setData(1, Qt::UserRole + 1, QVariant::fromValue(m_tSetRedEnInfo.tRedEnLoop[nRow]));
	RedLampIncreaseUpdateDraw(nRow, nCol);
	ui.widget_3->update();
}


void Dlg_Detection::Slot_RadarDataChanged(int nRow, int nColumn)
{
	m_bChange = true;
	QTreeWidgetItem *pItem = ui.treeWidget_redar->topLevelItem(nRow);
	TIPC_RadarLoopCfg& tRadarLoopCfg = m_tRedarLoopInfo.tRadarLoop[nRow];
	if (this->sender() == m_radar_chkDelegate)
	{
		tRadarLoopCfg.bEnable = (pItem->text(1) == GBUTF8("是") ? true : false);
	}
	else if (this->sender() == m_radar_cmbDelegate2)
	{
		tRadarLoopCfg.dwSerialChnId = nColumn;
	}
	else if (this->sender() == m_radar_cmbDelegate3)
	{
		tRadarLoopCfg.dwRadarType = nColumn;
	}
	else if (this->sender() == m_radar_cmbDelegate6)
	{
		tRadarLoopCfg.dwPhyLaneNo = nColumn + 1;
	}
	else if (this->sender() == m_radar_editDelegate4)
	{
		tRadarLoopCfg.fH_distance = pItem->text(4).toFloat();
	}
	else if (this->sender() == m_radar_editDelegate5)
	{
		tRadarLoopCfg.fCorrectRate = pItem->text(5).toFloat();
	}
	RedarLoopUpdateDraw(nRow);
	ui.widget_3->update();
}

void Dlg_Detection::Slot_CoilDataChanged(int nRow, int nColumn)
{
	m_bChange = true;
	TIPC_CoilLoopCfg& tCoilLoop = m_tCoilLoopInfo.tCoilLoop[nRow];
	QTreeWidgetItem *pItem = ui.treeWidget_coil->topLevelItem(nRow);
	if (this->sender() == m_coil_chkDelegate)
	{
		QString sEnable = pItem->text(1);
		tCoilLoop.bEnable = (sEnable == GBUTF8("是") ? true : false);
	}
	else if (this->sender() == m_coil_cmbDelegate2)
	{
		tCoilLoop.dwSerialChnId = nColumn;
	}
	else if (this->sender() == m_coil_cmbDelegate3)
	{
		tCoilLoop.dwCoilChnId = nColumn;
	}
	else if (this->sender() == m_coil_cmbDelegate4)
	{
		tCoilLoop.dwDirection = nColumn + 1;
	}
	else if (this->sender() == m_coil_cmbDelegate7)
	{
		tCoilLoop.dwIOLabel = nColumn;
	}
	else if (this->sender() == m_coil_cmbDelegate8)
	{ 
		tCoilLoop.dwPhyLaneNo = nColumn + 1;
	}
	else if (this->sender() == m_coil_cmbDelegate9)
	{
		tCoilLoop.dwCoilType = nColumn;
	}
	else if (this->sender() == m_coil_editDelegate5)
	{
		tCoilLoop.dwV_distance = pItem->text(5).toInt();
	}
	else if (this->sender() == m_coil_editDelegate6)
	{
		tCoilLoop.fCorrectRate = pItem->text(6).toFloat();
	}
	pItem->setData(1, Qt::UserRole + 1, QVariant::fromValue(m_tCoilLoopInfo.tCoilLoop[nRow]));
}

void Dlg_Detection::Slot_RecogAreaDataChanged(int nRow, int nColumn)
{
	m_bChange = true;
	int nCol = 0;
	QTreeWidgetItem *pItem = ui.treeWidget_RedLampSig->topLevelItem(nRow);
	recogarea::TRecogAreaParam& tRecogAreaParam = m_tRecogAreaParam[nRow];
	if (this->sender() == m_recogArea_chkDelegate)
	{
		nCol = 1;
		tRecogAreaParam.bEnable = (pItem->text(1) == GBUTF8("是") ? true : false);
	}
	else if (this->sender() == m_recogArea_editDelegate2)
	{
		nCol = 2;
		tRecogAreaParam.X1 = pItem->text(2).toInt();
	}
	else if (this->sender() == m_recogArea_editDelegate3)
	{
		nCol = 3;
		tRecogAreaParam.Y1 = pItem->text(3).toInt();
	}
	else if (this->sender() == m_recogArea_editDelegate4)
	{
		nCol = 4;
		tRecogAreaParam.W1 = pItem->text(4).toInt();
	}
	else if (this->sender() == m_recogArea_editDelegate5)
	{
		nCol = 5;
		tRecogAreaParam.X2 = pItem->text(5).toInt();
	}
	else if (this->sender() == m_recogArea_editDelegate6)
	{
		nCol = 6;
		tRecogAreaParam.Y2 = pItem->text(6).toInt();
	}
	else if (this->sender() == m_recogArea_editDelegate7)
	{
		nCol = 7;
		tRecogAreaParam.W2 = pItem->text(7).toInt();
	}
	else if (this->sender() == m_recogArea_editDelegate8)
	{
		nCol = 8;
		tRecogAreaParam.dwPlateWidth = pItem->text(8).toInt();
	}
	else if(this->sender() == m_recogArea_cmbDelegat9)
	{
		nCol = 9;
		tRecogAreaParam.dwPlateAngle = nColumn;
	}
	else if (this->sender() == m_recogArea_cmbDelegat10)
	{
		nCol = 10;
		tRecogAreaParam.dwShootRear = nColumn;
	}
	pItem->setData(1, Qt::UserRole + 1, QVariant::fromValue(m_tRecogAreaParam[nRow]));
	RecogAreaUpdateDraw(nRow, nCol);
	ui.widget_3->update();
}

void Dlg_Detection::Slot_RedLampSigDataChanged(int nRow, int nColumn)
{
	m_bChange = true;
	QTreeWidgetItem *pItem = ui.treeWidget_RedLampSig->topLevelItem(nRow);
	TIPC_RedLampSigLoop& tRedLampSigLoop = m_tRedLampSigInfo.tRedLoop[nRow];

	if (this->sender() == m_redLampSig_chkDelegate)
	{
		tRedLampSigLoop.bEnable = (pItem->text(1) == GBUTF8("是") ? true : false);
	}
	else if (this->sender() == m_redLampSig_cmbDelegate2)
	{
		tRedLampSigLoop.dwChnId = nColumn;
	}
	else if (this->sender() == m_redLampSig_cmbDelegate3)
	{
		tRedLampSigLoop.dwChnId = nColumn;
	}
	
}

void Dlg_Detection::Slot_VideoDataChanged(int nRow, int nColumn)
{
	m_bChange = true;
	TIPC_VidLoopCfg *ptVideoLoop = &(m_tVideoLoopInfo.tVideoLoop[nRow]);
	QTreeWidgetItem *pItem = ui.treeWidget_video->topLevelItem(nRow);
	int nCol = 0;
	if (this->sender() == m_cheDelegate)
	{
		nCol = 1;
		QString sEnable = pItem->text(1);
		ptVideoLoop->bEnable = (sEnable == GBUTF8("是") ? true : false);
	}
	else if (this->sender() == m_editDelegate2)
	{
		nCol = 2;
		int nStartX = pItem->text(2).toInt();
		ptVideoLoop->dwStartX = nStartX;
	}
	else if (this->sender() == m_editDelegate3)
	{
		nCol = 3;
		int nStartY = pItem->text(3).toInt();
		ptVideoLoop->dwStartY = nStartY;
	}
	else if (this->sender() == m_editDelegate4)
	{
		nCol = 4;
		int nWidth = pItem->text(4).toInt();
		ptVideoLoop->dwWidth = nWidth;
	}
	else if (this->sender() == m_editDelegate5)
	{
		nCol = 5;
		ptVideoLoop->dwHeight = pItem->text(5).toInt();
	}
	else if (this->sender() == m_editDelegate6)
	{
		nCol = 6;
		ptVideoLoop->X1 = pItem->text(6).toInt();
	}
	else if (this->sender() == m_editDelegate7)
	{
		nCol = 7;
		ptVideoLoop->Y1 = pItem->text(7).toInt();
	}
	else if (this->sender() == m_editDelegate8)
	{
		nCol = 8;
		ptVideoLoop->W1 = pItem->text(8).toInt();
	}
	else if (this->sender() == m_editDelegate9)
	{
		nCol = 9;
		ptVideoLoop->X2 = pItem->text(9).toInt();
	}
	else if (this->sender() == m_editDelegate10)
	{
		nCol = 10;
		ptVideoLoop->Y2 = pItem->text(10).toInt();
	}
	else if (this->sender() == m_editDelegate11)
	{
		nCol = 11;
		ptVideoLoop->W2 = pItem->text(11).toInt();
	}
	else if (this->sender() == m_editDelegate15)
	{
		nCol = 15;
		ptVideoLoop->dwPresentTime = pItem->text(15).toInt();
	}
	else if (this->sender() == m_editDelegate16)
	{
		nCol = 16;
		ptVideoLoop->dwTargetWidth = pItem->text(16).toInt();
	}
	else if (this->sender() == m_editDelegate17)
	{
		nCol = 17;
		ptVideoLoop->dwTargetHeight = pItem->text(17).toInt();
	}
	else if (this->sender() == m_editDelegate18)
	{
		nCol = 18;
		ptVideoLoop->dwSensitive = pItem->text(18).toInt();
	}
	else if (this->sender() == m_editDelegate19)
	{
		nCol = 19;
		ptVideoLoop->dwFilterOnTimes = pItem->text(19).toInt();
	}
	else if (this->sender() == m_editDelegate20)
	{
		nCol = 20;
		ptVideoLoop->dwFilterOffTimes = pItem->text(20).toInt();
	}
	else if (this->sender() == m_comboDelegate12)
	{
		nCol = 12;
		ptVideoLoop->dwMethod = videoloop::GetMethodValue(nColumn, m_bTrapezium);
		ChangeMethod(ptVideoLoop, nRow);
	}
	else if (this->sender() == m_comboDelegate13)
	{
		nCol = 13;
		ptVideoLoop->dwDirection = videoloop::GetDirectionModeValue(nColumn);
	}
	else if (this->sender() == m_comboDelegate14)
	{
		nCol = 14;
		ptVideoLoop->wPlateAngle = videoloop::GetAngleModeValue(nColumn);

	}
	pItem->setData(1, Qt::UserRole + 1, QVariant::fromValue(m_tVideoLoopInfo.tVideoLoop[nRow]));

	VideoLoopUpdateDraw(nRow, nCol);
	ui.widget_3->update();
}

/*
//暂时没用到
void Dlg_Detection::Slot_VideoCmbDataChanged(int nRow, int nColumn)
{
	TIPC_VidLoopCfg *ptVideoLoop = &(m_tVideoLoopInfo.tVideoLoop[nRow]);
	QTreeWidgetItem *pItem = ui.treeWidget_video->topLevelItem(nRow);
	if (this->sender() == m_comboDelegate12)
	{
		ptVideoLoop->dwMethod = videoloop::GetMethodValue(nColumn, m_bTrapezium);
		ChangeMethod(ptVideoLoop, nRow);
	}
	else if (this->sender() == m_comboDelegate13)
	{
		ptVideoLoop->dwDirection = videoloop::GetDirectionModeValue(nColumn);
	}
	else if (this->sender() == m_comboDelegate14)
	{
		ptVideoLoop->wPlateAngle = videoloop::GetAngleModeValue(nColumn);
		
	}
	pItem->setData(1, Qt::UserRole + 1, QVariant::fromValue(m_tVideoLoopInfo.tVideoLoop[nRow]));
}
*/
void Dlg_Detection::ChangeMethod(TIPC_VidLoopCfg *ptLoop, int nIndex)
{
	QTreeWidgetItem *pItem = ui.treeWidget_video->topLevelItem(nIndex);
	switch (ptLoop->dwMethod)
	{
	case DETECT_METHOD_GRAY_ENHANCE:
	case DETECT_METHOD_GRAY:
	{

		switch (ptLoop->dwDirection)
		{
		case DETECT_HEAD_VIEW:
		case DETECT_TAIL_VIEW:
			break;
		default:
			ptLoop->dwDirection = DETECT_HEAD_VIEW;
			VideoLoopUpdateListItem(nIndex, videoloop::EVIDEOLOOP_DIR);
			int temp = 0;
			temp = videoloop::GetDirectionModeIndex(ptLoop->dwDirection);
			pItem->setText(13, m_map2[temp]);
			//pCmb->setCurrentIndex(0);
			break;
		}

		break;
	}

	case DETECT_METHOD_PLATE_ENHANCE:
	case DETECT_METHOD_PLATE:
	{

		switch (ptLoop->dwDirection)
		{
		case DETECT_HEAD_VIEW:
		case DETECT_TAIL_VIEW:
			break;
		default:
			ptLoop->dwDirection = DETECT_HEAD_VIEW;
			VideoLoopUpdateListItem(nIndex, videoloop::EVIDEOLOOP_DIR);
			int temp = 0;
			temp = videoloop::GetDirectionModeIndex(ptLoop->dwDirection);
			pItem->setText(13, m_map2[temp]);
			break;
		}
	}
		break;
	case DETECT_METHOD_GRAY_SIMPLE:
	case DETECT_METHOD_GRAY_SIMPLE_ENHANCE:
	{
		switch (ptLoop->dwDirection)
		{
		case DETECT_HEAD_VIEW:
		case DETECT_TAIL_VIEW:
			break;
		default:
			ptLoop->dwDirection = DETECT_HEAD_VIEW;
			VideoLoopUpdateListItem(nIndex, videoloop::EVIDEOLOOP_DIR);
			int temp = 0;
			temp = videoloop::GetDirectionModeIndex(ptLoop->dwDirection);
			pItem->setText(13, m_map2[temp]);
			break;
		}

	}
		break;
	case DETECT_METHOD_HEADLAMP:
	case DETECT_METHOD_HEADLAMP_ENHANCE:
	{
		ptLoop->dwDirection = DETECT_HEAD_VIEW;
		VideoLoopUpdateListItem(nIndex, videoloop::EVIDEOLOOP_DIR);
		int temp = 0;
		temp = videoloop::GetDirectionModeIndex(ptLoop->dwDirection);
		pItem->setText(13, m_map2[temp]);
	}
		break;
	case DETECT_METHOD_VEH_HEAD:
	{

		ptLoop->dwDirection = DETECT_HEAD_VIEW;
		VideoLoopUpdateListItem(nIndex, videoloop::EVIDEOLOOP_DIR);
		int temp = 0;
		temp = videoloop::GetDirectionModeIndex(ptLoop->dwDirection);
		pItem->setText(13, m_map2[temp]);

	}
		break;
	case DETECT_METHOD_TAILLAMP:
	case DETECT_METHOD_TAILLAMP_ENHANCE:
	{

		ptLoop->dwDirection = DETECT_TAIL_VIEW;
		VideoLoopUpdateListItem(nIndex, videoloop::EVIDEOLOOP_DIR);
		int temp = 0;
		temp = videoloop::GetDirectionModeIndex(ptLoop->dwDirection);
		pItem->setText(13, m_map2[temp]);

	}
		break;
	case DETECT_METHOD_VEH_TAIL:
	{
		ptLoop->dwDirection = DETECT_TAIL_VIEW;
		VideoLoopUpdateListItem(nIndex, videoloop::EVIDEOLOOP_DIR);
		int temp = 0;
		temp = videoloop::GetDirectionModeIndex(ptLoop->dwDirection);
		pItem->setText(13, m_map2[temp]);

	}
		break;
	case DETECT_METHOD_REDLAMP:
	{

		switch (ptLoop->dwDirection)
		{
		case REDLAMP_PHASE_TURNRIGHT:
		case REDLAMP_PHASE_STRA_AHEAD:
		case REDLAMP_PHASE_TURNLEFT:
		case REDLAMP_PHASE_THREECOLOR_TURNRIGHT:
		case REDLAMP_PHASE_THREECOLOR_STRA_AHEAD:
		case REDLAMP_PHASE_THREECOLOR_TURNLEFT:
			break;
		default:
			ptLoop->dwDirection = REDLAMP_PHASE_STRA_AHEAD;
			VideoLoopUpdateListItem(nIndex, videoloop::EVIDEOLOOP_DIR);
			int temp = 0;
			temp = videoloop::GetDirectionModeIndex(ptLoop->dwDirection);
			pItem->setText(0, m_map2[temp + 2]);
			break;
		}

	}
		break;
	case DETECT_METHOD_PEDES:
	{
		switch (ptLoop->dwDirection)
		{
		case PEDES_MOVE_HORIZONTAL:
		case PEDES_MOVE_VERTICAL:
		case PEDES_MOVE_ANYDIR:
			break;
		default:
			ptLoop->dwDirection = PEDES_MOVE_HORIZONTAL;
			VideoLoopUpdateListItem(nIndex, videoloop::EVIDEOLOOP_DIR);
			int temp = 0;
			temp = videoloop::GetDirectionModeIndex(ptLoop->dwDirection);
			pItem->setText(0, m_map2[temp + 8]);
			break;
		}

	}
		break;
	default:
		break;

	}
}

void Dlg_Detection::Slot_TreeRecogAreaWidgetItemClicked(QTreeWidgetItem *pItem, int column)
{
	QTreeWidget *pWidget = dynamic_cast<QTreeWidget*>(this->sender());
	int nSel = pWidget->currentIndex().row();
	ui.widget_3->SetSelType(nSel);

	if (column != 0)
	{
		pItem->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
	}
	else
	{
		pItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
	}
}

void Dlg_Detection::Slot_TreeRedLampSigWidgetItemClicked(QTreeWidgetItem *pItem, int column)
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

void Dlg_Detection::Slot_TreeRadarWidgetItemClicked(QTreeWidgetItem*pItem, int column)
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

void Dlg_Detection::Slot_TreeCoilWidgetItemClicked(QTreeWidgetItem*pItem, int column)
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

void Dlg_Detection::Slot_TreeRedLampIncreaseWidgetItemClicked(QTreeWidgetItem *pItem, int column)
{
	pItem->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
}

void Dlg_Detection::Slot_TreeVideoWidgetItemClicked(QTreeWidgetItem *pItem, int column)
{
	QTreeWidget *pWidget = dynamic_cast<QTreeWidget*>(this->sender());
	int nSel = pWidget->currentIndex().row();
	ui.widget_3->SetSelType(nSel);
	if (column != 0 )
	{
		pItem->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
	}
	else
	{
		pItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
	}
	TIPC_VidLoopCfg ptLoop = pItem->data(1, Qt::UserRole + 1).value<TIPC_VidLoopCfg>();
	switch (ptLoop.dwMethod)
	{
	case DETECT_METHOD_GRAY_ENHANCE:
	case DETECT_METHOD_GRAY:
	{
		QStringList sList2;
		sList2.append(GBUTF8("车头"));
		sList2.append(GBUTF8("车尾"));
		m_comboDelegate13->setComboxData(sList2);
		
		if (column == 14)
		{
			pItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
		}

		break;
	}

	case DETECT_METHOD_PLATE_ENHANCE:
	case DETECT_METHOD_PLATE:
	{
		QStringList sList2;
		sList2.append(GBUTF8("车头"));
		sList2.append(GBUTF8("车尾"));
		m_comboDelegate13->setComboxData(sList2);

		if (column == 14)
		{
			pItem->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
		}
	}
		break;
	case DETECT_METHOD_GRAY_SIMPLE:
	case DETECT_METHOD_GRAY_SIMPLE_ENHANCE:
	{
		QStringList sList2;
		sList2.append(GBUTF8("车头"));
		sList2.append(GBUTF8("车尾"));
		m_comboDelegate13->setComboxData(sList2);
		
		if (column == 14)
		{
			pItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
		}
	}
		break;
	case DETECT_METHOD_HEADLAMP:
	case DETECT_METHOD_HEADLAMP_ENHANCE:
	{
		QStringList sList2;
		sList2.append(GBUTF8("车头"));
		m_comboDelegate13->setComboxData(sList2);

		if (column == 14)
		{
			pItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
		}
	}
		break;
	case DETECT_METHOD_VEH_HEAD:
	{
		QStringList sList2;
		sList2.append(GBUTF8("车头"));
		m_comboDelegate13->setComboxData(sList2);
		if (column == 14)
		{
			pItem->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
		}
	}
		break;
	case DETECT_METHOD_TAILLAMP:
	case DETECT_METHOD_TAILLAMP_ENHANCE:
	{
		QStringList sList2;
		sList2.append(GBUTF8("车尾"));
		m_comboDelegate13->setComboxData(sList2);
		if (column == 14)
		{
			pItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
		}
	}
		break;
	case DETECT_METHOD_VEH_TAIL:
	{
		QStringList sList2;
		sList2.append(GBUTF8("车尾"));
		m_comboDelegate13->setComboxData(sList2);
		if (column == 14)
		{
			pItem->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
		}
	}
		break;
	case DETECT_METHOD_REDLAMP:
	{
		QStringList sList2;
		sList2.append(GBUTF8("右转"));
		sList2.append(GBUTF8("直行"));
		sList2.append(GBUTF8("左转"));
		sList2.append(GBUTF8("右转三色"));
		sList2.append(GBUTF8("直行三色"));
		sList2.append(GBUTF8("左转三色"));
		m_comboDelegate13->setComboxData(sList2);
		
		if (column == 14)
		{
			pItem->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable);
		}
		
	}
		break;
	case DETECT_METHOD_PEDES:
	{
		QStringList sList2;
		sList2.append(GBUTF8("水平方向"));
		sList2.append(GBUTF8("垂直方向"));
		sList2.append(GBUTF8("任意方向"));
		m_comboDelegate13->setComboxData(sList2);
		if (column == 14)
		{
			pItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
		}

	}
		break;
	default:
		break;

	}
}

void Dlg_Detection::OnReset()
{
	switch (m_nTabIndex)
	{
	case 0:
		GetParamVideoLoopInfo();
		break;
	case 1:
		GetParamCoilLoopInfo();
		break;
	case 2:
		GetParamRedarLoopInfo();
		break;
	case 3:
		GetParamRedLampSigInfo();
		break;
	case 4:
		GetParamRecogAreaInfo();
		break;
	case 5:
		GetParamRedEnhanceInfo();
		break;
	}

	m_bChange = false;
}


void Dlg_Detection::Slot_TabWidgetCurrentChanged(int nTabIndex)
{
	if (this->sender()->property("id").toInt() == 111)
	{
		this->sender()->setProperty("id", 0);
		return;
	}
	if (m_bChange)
	{
		Dlg_MessageBox dlg;
		dlg.SetBtnNoVisible(true);
		dlg.SetTitleInfo(GBUTF8("提示"));
		dlg.SetInfoText(GBUTF8("是否保存参数?"));
		if (dlg.exec() == 1)
		{
			OnSave();
		}
		else
		{
			
			OnReset();
		}
	}
	if (nTabIndex == 0)
	{
		ui.widget_4->setVisible(true);
	}
	else
	{
		ui.widget_4->setVisible(false);
	}
	//ui.widget_3->Clear();
	m_nTabIndex = nTabIndex;
	OnSwitchScene();
	//ui.widget_3->update();
}

void Dlg_Detection::OnChangePreset(int nPreset)
{
	MgrItsCtrl* pMgrItsCtrl = MgrData::getInstance()->GetMgrItsCtrl();
	if (pMgrItsCtrl != NULL)
	{
		u32 dwIP = MgrData::getInstance()->GetIP();
		ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dwIP);
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

bool Dlg_Detection::GetParamRecogAreaInfo()
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

	return true;
}

bool Dlg_Detection::GetParamVideoLoopInfo()
{
	u32 dwIP = MgrData::getInstance()->GetIP();
	ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dwIP);
	if (pCtrl == NULL)
	{
		return false;
	}

	int sceneid = m_nSceneIndex + 1;
	bool bRet = pCtrl->GetIpcVideoLoop(&sceneid, (u32)(CObserver*)this);
	if (!bRet)
	{
		return false;
	}
	return true;
}

void Dlg_Detection::Slot_RedarLoopChanged()
{
	UpdateData(false);
}

void Dlg_Detection::Slot_RecogAreaInfoChanged()
{
	RecogAreaConvert1(&m_tRecogAreaInfo, m_tRecogAreaParam);
	UpdateData(false);
}

void Dlg_Detection::Slot_RedSigLoopChanged()
{
	UpdateData(false);
}

void Dlg_Detection::Slot_ReadEnhloopDataChanged()
{
	UpdateData(false);
}


void Dlg_Detection::Slot_CreateLoopChanged()
{
	GetParamVideoLoopInfo();
	GetParamRecogAreaInfo();
}

void Dlg_Detection::Slot_ScenceInfoChanged()
{
	OnChangeScenceNum();
	// 切换预制位
	int nPreset = m_tSeceneCfg.tSceneCfg.wPrePosId;
	OnChangePreset(nPreset);
	Dlg_PictureDraw *pShow = ui.widget_3;
	pShow->SetCoefficient(m_tSeceneCfg.tMaxParam.dwMaxWidth, m_tSeceneCfg.tMaxParam.dwMaxHeight);

}

void Dlg_Detection::Slot_VideoLoopDataChanged()
{
	for (int i = 0; i < VIDEO_LOOP_NUM_MAX; i++)
	{
		if (m_tVideoLoopInfo.tVideoLoop[i].dwMethod >= 100)
		{
			m_bTrapezium = true;
			break;
		}
	}

	UpdateData(false);
}

void Dlg_Detection::ClearAllTreeWidgetItem(QTreeWidget *pTree)
{
	while (pTree->topLevelItemCount() > 0)
	{ 
		delete pTree->topLevelItem(0);
	}
}

void Dlg_Detection::VideoLoopUpdate()
{
	ClearAllTreeWidgetItem(ui.treeWidget_video);
	//清空绘图
	ui.widget_3->Clear();
	
	for (int i = 0; i < VIDEO_LOOP_NUM_MAX; i++)
	{
		QTreeWidgetItem *pItem = new QTreeWidgetItem;

		pItem->setData(1, Qt::UserRole + 1, QVariant::fromValue(m_tVideoLoopInfo.tVideoLoop[i]));
		pItem->setData(0, Qt::UserRole, i + 1);
		int j = 0;
		ui.treeWidget_video->insertTopLevelItem(i, pItem);
		
		//pItem->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
		pItem->setText(j++, QString::fromStdWString(videoloop::GetListValue(&m_tVideoLoopInfo.tVideoLoop[i], videoloop::EVIDEOLOOP_SN)));
		pItem->setText(j++, QString::fromStdWString(videoloop::GetListValue(&m_tVideoLoopInfo.tVideoLoop[i], videoloop::EVIDEOLOOP_ENABLED)));
		pItem->setText(j++, QString::fromStdWString(videoloop::GetListValue(&m_tVideoLoopInfo.tVideoLoop[i], videoloop::EVIDEOLOOP_STARTX)));
		pItem->setText(j++, QString::fromStdWString(videoloop::GetListValue(&m_tVideoLoopInfo.tVideoLoop[i], videoloop::EVIDEOLOOP_STARTY)));
		pItem->setText(j++, QString::fromStdWString(videoloop::GetListValue(&m_tVideoLoopInfo.tVideoLoop[i], videoloop::EVIDEOLOOP_WIDTH)));
		pItem->setText(j++, QString::fromStdWString(videoloop::GetListValue(&m_tVideoLoopInfo.tVideoLoop[i], videoloop::EVIDEOLOOP_HEIGHT)));
		pItem->setText(j++, QString::fromStdWString(videoloop::GetListValue(&m_tVideoLoopInfo.tVideoLoop[i], videoloop::EVIDEOLOOP_STARTX1)));
		pItem->setText(j++, QString::fromStdWString(videoloop::GetListValue(&m_tVideoLoopInfo.tVideoLoop[i], videoloop::EVIDEOLOOP_STARTY1)));
		pItem->setText(j++, QString::fromStdWString(videoloop::GetListValue(&m_tVideoLoopInfo.tVideoLoop[i], videoloop::EVIDEOLOOP_WIDTH1)));
		pItem->setText(j++, QString::fromStdWString(videoloop::GetListValue(&m_tVideoLoopInfo.tVideoLoop[i], videoloop::EVIDEOLOOP_STARTX2)));
		pItem->setText(j++, QString::fromStdWString(videoloop::GetListValue(&m_tVideoLoopInfo.tVideoLoop[i], videoloop::EVIDEOLOOP_STARTY2)));
		pItem->setText(j++, QString::fromStdWString(videoloop::GetListValue(&m_tVideoLoopInfo.tVideoLoop[i], videoloop::EVIDEOLOOP_WIDTH2)));
		pItem->setText(j++, QString::fromStdWString(videoloop::GetListValue(&m_tVideoLoopInfo.tVideoLoop[i], videoloop::EVIDEOLOOP_MODE)));
		pItem->setText(j++, QString::fromStdWString(videoloop::GetListValue(&m_tVideoLoopInfo.tVideoLoop[i], videoloop::EVIDEOLOOP_DIR)));
		pItem->setText(j++, QString::fromStdWString(videoloop::GetListValue(&m_tVideoLoopInfo.tVideoLoop[i], videoloop::EVIDEOLOOP_ANGLE)));
		pItem->setText(j++, QString::fromStdWString(videoloop::GetListValue(&m_tVideoLoopInfo.tVideoLoop[i], videoloop::EVIDEOLOOP_TIME)));
		pItem->setText(j++, QString::fromStdWString(videoloop::GetListValue(&m_tVideoLoopInfo.tVideoLoop[i], videoloop::EVIDEOLOOP_TARGET_WIDHT)));
		pItem->setText(j++, QString::fromStdWString(videoloop::GetListValue(&m_tVideoLoopInfo.tVideoLoop[i], videoloop::EVIDEOLOOP_TARGET_HEIGTH)));
		pItem->setText(j++, QString::fromStdWString(videoloop::GetListValue(&m_tVideoLoopInfo.tVideoLoop[i], videoloop::EVIDEOLOOP_SENSITIVE)));
		pItem->setText(j++, QString::fromStdWString(videoloop::GetListValue(&m_tVideoLoopInfo.tVideoLoop[i], videoloop::EVIDEOLOOP_FILTERON)));
		pItem->setText(j++, QString::fromStdWString(videoloop::GetListValue(&m_tVideoLoopInfo.tVideoLoop[i], videoloop::EVIDEOLOOP_FILTEROFF)));
		
		QWidget *pWidget = CreateWidget(i);
		QPushButton *btn = pWidget->property("btn").value<QPushButton*>();
		ui.treeWidget_video->setItemWidget(pItem, j, pWidget);
		
		connect(btn, SIGNAL(clicked()), this, SLOT(Slot_VideoLoopSelectArea()));
		
		VideoLoopUpdateDraw(i, -1);
	}
	ui.widget_3->update();
	
}

void Dlg_Detection::Slot_VideoLoopSelectArea()
{
	QPushButton *pBtn = (QPushButton *)this->sender();
	int nIndex = pBtn->property("nIndex").toInt();
	switch (m_nTabIndex)
	{
	case 0:
	{
		Dlg_Detection_Area dlg(this, m_tSeceneCfg.tMaxParam.dwMaxWidth, m_tSeceneCfg.tMaxParam.dwMaxHeight);
		dlg.setWindowTitle(GBUTF8("检测区域选择"));
		dlg.SetData(&m_tVideoLoopInfo.tVideoLoop[nIndex].dwStartX,
			&m_tVideoLoopInfo.tVideoLoop[nIndex].dwStartY,
			&m_tVideoLoopInfo.tVideoLoop[nIndex].dwWidth,
			&m_tVideoLoopInfo.tVideoLoop[nIndex].dwHeight);
		dlg.exec();

		//OnVideoLoopSetListTipCtrl();
		VideoLoopUpdateListItem(nIndex, 2);
		VideoLoopUpdateListItem(nIndex, 3);
		VideoLoopUpdateListItem(nIndex, 4);
		VideoLoopUpdateListItem(nIndex, 5);
		VideoLoopUpdateDraw(nIndex, -1);
		m_bChange = false;
	}
		break;
		/*
	case 2:
	{
		RECT rc;
		SystemParametersInfo(SPI_GETWORKAREA, 0, &rc, 0);
		CDlgIpc_Detection_Area dlg(m_pMgrData, m_dwDevIP, m_tSeceneCfg.tMaxParam.dwMaxWidth, m_tSeceneCfg.tMaxParam.dwMaxHeight);
		m_dlgArea = &dlg;
		HWND hWnd = dlg.Create(*this, _T("雷达触发线选择"), UI_WNDSTYLE_DIALOG | WS_POPUP, NULL, rc);
		dlg.SetCaption(wstring(_T("雷达触发线选择")));
		dlg.SetVideoHandle(m_dwVideoHandle);

		dlg.SetData(&m_tRedarLoopInfo.tRadarLoop[nIndex].tRuleLine);
		// 显示
		dlg.CenterWindow();
		dlg.ShowModal();
		m_dlgArea = NULL;

		RedarLoopUpdateDraw(nIndex);
	}
		break;
	case 5:
	{
		RECT rc;
		SystemParametersInfo(SPI_GETWORKAREA, 0, &rc, 0);
		CDlgIpc_Detection_Area dlg(m_pMgrData, m_dwDevIP, m_tSeceneCfg.tMaxParam.dwMaxWidth, m_tSeceneCfg.tMaxParam.dwMaxHeight);
		m_dlgArea = &dlg;
		HWND hWnd = dlg.Create(*this, _T("红灯区域选择"), UI_WNDSTYLE_DIALOG | WS_POPUP, NULL, rc);
		dlg.SetCaption(wstring(_T("红灯区域选择")));
		dlg.SetVideoHandle(m_dwVideoHandle);

		dlg.SetData(&m_tSetRedEnInfo.tRedEnLoop[nIndex].dwStartX,
			&m_tSetRedEnInfo.tRedEnLoop[nIndex].dwStartY,
			&m_tSetRedEnInfo.tRedEnLoop[nIndex].dwWidth,
			&m_tSetRedEnInfo.tRedEnLoop[nIndex].dwHeight);
		// 显示
		dlg.CenterWindow();
		dlg.ShowModal();
		m_dlgArea = NULL;

		OnRedLampIncreaseSetListTipCtrl();
		RedLampIncreaseUpdateListItem(nIndex, 1);
		RedLampIncreaseUpdateListItem(nIndex, 2);
		RedLampIncreaseUpdateListItem(nIndex, 3);
		RedLampIncreaseUpdateListItem(nIndex, 4);
		RedLampIncreaseUpdateDraw(nIndex, -1);
	}
		break;
		*/
	}
}

void Dlg_Detection::CoilLoopUpdate()
{
	ClearAllTreeWidgetItem(ui.treeWidget_coil);
	ui.widget_3->Clear();
	for (int i = 0; i < COIL_LOOP_CHANNEL_MAX; i++)
	{
		QTreeWidgetItem *pItem = new QTreeWidgetItem;
		pItem->setData(1, Qt::UserRole + 1, QVariant::fromValue(m_tCoilLoopInfo.tCoilLoop[i]));
		pItem->setData(0, Qt::UserRole, i + 1);
		pItem->setSizeHint(0, QSize(60, 30));
		pItem->setText(0, QString::fromStdWString(coilloop::GetListValue(&m_tCoilLoopInfo.tCoilLoop[i], 0)));
		pItem->setText(1, QString::fromStdWString(coilloop::GetListValue(&m_tCoilLoopInfo.tCoilLoop[i], 1)));
		pItem->setText(2, QString::fromStdWString(coilloop::GetListValue(&m_tCoilLoopInfo.tCoilLoop[i], 2)));
		pItem->setText(3, QString::fromStdWString(coilloop::GetListValue(&m_tCoilLoopInfo.tCoilLoop[i], 3)));
		pItem->setText(4, QString::fromStdWString(coilloop::GetListValue(&m_tCoilLoopInfo.tCoilLoop[i], 4)));
		pItem->setText(5, QString::fromStdWString(coilloop::GetListValue(&m_tCoilLoopInfo.tCoilLoop[i], 5)));
		pItem->setText(6, QString::fromStdWString(coilloop::GetListValue(&m_tCoilLoopInfo.tCoilLoop[i], 6)));
		pItem->setText(7, QString::fromStdWString(coilloop::GetListValue(&m_tCoilLoopInfo.tCoilLoop[i], 7)));
		pItem->setText(8, QString::fromStdWString(coilloop::GetListValue(&m_tCoilLoopInfo.tCoilLoop[i], 8)));
		pItem->setText(9, QString::fromStdWString(coilloop::GetListValue(&m_tCoilLoopInfo.tCoilLoop[i], 9)));
		ui.treeWidget_coil->insertTopLevelItem(i, pItem);

	}
	ui.widget_3->update();
}

QWidget* Dlg_Detection::CreateWidget(int nIndex)
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

void Dlg_Detection::RedarLoopUpdate()
{
	ClearAllTreeWidgetItem(ui.treeWidget_redar);
	ui.widget_3->Clear();
	for (int i = 0; i < RADAR_NUM_MAX; i++)
	{
		QTreeWidgetItem *pItem = new QTreeWidgetItem;
		ui.treeWidget_redar->insertTopLevelItem(i, pItem);

		pItem->setData(0, Qt::UserRole, i + 1);
		pItem->setData(1, Qt::UserRole + 1, QVariant::fromValue(m_tRedarLoopInfo.tRadarLoop[i]));
		int j = 0;
		pItem->setText(j++, QString::fromStdWString(redarloop::GetListValue(&m_tRedarLoopInfo.tRadarLoop[i], redarloop::EREDARLOOP_SN)));
		pItem->setText(j++, QString::fromStdWString(redarloop::GetListValue(&m_tRedarLoopInfo.tRadarLoop[i], redarloop::EREDARLOOP_ENABLED)));
		pItem->setText(j++, QString::fromStdWString(redarloop::GetListValue(&m_tRedarLoopInfo.tRadarLoop[i], redarloop::EREDARLOOP_SERIAL)));
		pItem->setText(j++, QString::fromStdWString(redarloop::GetListValue(&m_tRedarLoopInfo.tRadarLoop[i], redarloop::EREDARLOOP_TYPE)));
		pItem->setText(j++, QString::fromStdWString(redarloop::GetListValue(&m_tRedarLoopInfo.tRadarLoop[i], redarloop::EREDARLOOP_DISTANCE)));
		pItem->setText(j++, QString::fromStdWString(redarloop::GetListValue(&m_tRedarLoopInfo.tRadarLoop[i], redarloop::EREDARLOOP_SPEED)));
		pItem->setText(j++, QString::fromStdWString(redarloop::GetListValue(&m_tRedarLoopInfo.tRadarLoop[i], redarloop::EREDARLOOP_LANE)));
		
		QWidget *pWidget = CreateWidget(i);
		QPushButton *btn = pWidget->property("btn").value<QPushButton*>();
		connect(btn, SIGNAL(clicked()), this, SLOT(Slot_Radar_AreaSelectClicked()));
		ui.treeWidget_redar->setItemWidget(pItem, j, pWidget);
		RedarLoopUpdateDraw(i);
	}
	ui.widget_3->update();
	
}


void Dlg_Detection::Slot_Radar_AreaSelectClicked()
{
	Dlg_Detection_Area dlg(this, 0, 0);
	dlg.setWindowTitle(GBUTF8("雷达触发线选择"));
	int nIndex = this->sender()->property("nIndex").toInt();
	dlg.SetData(&m_tRedarLoopInfo.tRadarLoop[nIndex].tRuleLine);
	dlg.exec();
	RedarLoopUpdateDraw(nIndex);
	m_bChange = false;
}

void Dlg_Detection::RedarLoopUpdateDraw(int nIndex)
{
	TIPC_RadarLoopCfg *pRadar = &m_tRedarLoopInfo.tRadarLoop[nIndex];

	if (pRadar->bEnable == false)
	{
		ui.widget_3->DelDrawData(nIndex);
	}
	else
	{
		ui.widget_3->AddLine(&(pRadar->tRuleLine.tLine), nIndex, pRadar->tRuleLine.LineLabel);
		ui.widget_3->SetColorByType(nIndex, COLOR_RED);
	}
}



void Dlg_Detection::RedLampSigUpdate()
{
	ClearAllTreeWidgetItem(ui.treeWidget_RedLampSig);
	ui.widget_3->Clear();
	for (int i = 0; i < MAX_REDLAMP_DET_NUM; i++)
	{
		QTreeWidgetItem *pItem = new QTreeWidgetItem;
		pItem->setData(0, Qt::UserRole, i + 1);
		pItem->setData(1, Qt::UserRole + 1, QVariant::fromValue(m_tRedLampSigInfo.tRedLoop[i]));
		int j = 0;

		pItem->setText(j++, QString::fromStdWString(RedLampSig::GetListValue(&m_tRedLampSigInfo.tRedLoop[i], j)));
		pItem->setText(j++, QString::fromStdWString(RedLampSig::GetListValue(&m_tRedLampSigInfo.tRedLoop[i], j)));
		pItem->setText(j++, QString::fromStdWString(RedLampSig::GetListValue(&m_tRedLampSigInfo.tRedLoop[i], j)));
		pItem->setText(j++, QString::fromStdWString(RedLampSig::GetListValue(&m_tRedLampSigInfo.tRedLoop[i], j)));

		QWidget *pWidget = CreateWidget(i);
		QPushButton *btn = pWidget->property("btn").value<QPushButton*>();
		connect(btn, SIGNAL(clicked()), this, SLOT(Slot_RedLampSigItemButtonClicked()));
		ui.treeWidget_RedLampSig->insertTopLevelItem(i, pItem);
		ui.treeWidget_RedLampSig->setItemWidget(pItem, j, pWidget);
		
	}
	ui.widget_3->update();
}

void Dlg_Detection::Slot_RedLampSigItemButtonClicked()
{
	int nIndex = this->sender()->property("nIndex").toInt();
	Dlg_Detection_Phase dlg;
	dlg.SetData(&m_tRedLampSigInfo.tRedLoop[nIndex].dwDetPhase);
	dlg.exec();
}

void Dlg_Detection::RecogAreaUpdate()
{
	ClearAllTreeWidgetItem(ui.treeWidget_recogArea);
	ui.widget_3->Clear();
	for (int i = 0; i < ROAD_NUM_MAX; i++)
	{
		QTreeWidgetItem *pItem = new QTreeWidgetItem;
		pItem->setData(0, Qt::UserRole, i + 1);
		pItem->setData(1, Qt::UserRole + 1, QVariant::fromValue(m_tRecogAreaParam[i]));
		pItem->setSizeHint(0, QSize(60, 30));
		pItem->setText(0, QString::fromStdWString(recogarea::GetListValue(&m_tRecogAreaParam[i], 0)));
		pItem->setText(1, QString::fromStdWString(recogarea::GetListValue(&m_tRecogAreaParam[i], 1)));
		pItem->setText(2, QString::fromStdWString(recogarea::GetListValue(&m_tRecogAreaParam[i], 2)));
		pItem->setText(3, QString::fromStdWString(recogarea::GetListValue(&m_tRecogAreaParam[i], 3)));
		pItem->setText(4, QString::fromStdWString(recogarea::GetListValue(&m_tRecogAreaParam[i], 4)));
		pItem->setText(5, QString::fromStdWString(recogarea::GetListValue(&m_tRecogAreaParam[i], 5)));
		pItem->setText(6, QString::fromStdWString(recogarea::GetListValue(&m_tRecogAreaParam[i], 6)));
		pItem->setText(7, QString::fromStdWString(recogarea::GetListValue(&m_tRecogAreaParam[i], 7)));
		pItem->setText(8, QString::fromStdWString(recogarea::GetListValue(&m_tRecogAreaParam[i], 8)));
		pItem->setText(9, QString::fromStdWString(recogarea::GetListValue(&m_tRecogAreaParam[i], 9)));
		pItem->setText(10, QString::fromStdWString(recogarea::GetListValue(&m_tRecogAreaParam[i], 10)));
		ui.treeWidget_recogArea->insertTopLevelItem(i, pItem);
		RecogAreaUpdateDraw(i, -1);
		
	}
	ui.widget_3->update();
}

void Dlg_Detection::RecogAreaUpdateDraw(int nIndex, int nEUpdateItem)
{
	if (nEUpdateItem != -1
		&& nEUpdateItem != 2
		&& nEUpdateItem != 3
		&& nEUpdateItem != 4
		&& nEUpdateItem != 5
		&& nEUpdateItem != 6
		&& nEUpdateItem != 7)
	{
		return;
	}

	if (m_tRecogAreaParam[nIndex].bEnable == false)
	{
		ui.widget_3->DelDrawData(nIndex);
		return;
	}

	TECHELON_S trapezium;
	trapezium.X1 = m_tRecogAreaParam[nIndex].X1;
	trapezium.Y1 = m_tRecogAreaParam[nIndex].Y1;
	trapezium.W1 = m_tRecogAreaParam[nIndex].W1;
	trapezium.X2 = m_tRecogAreaParam[nIndex].X2;
	trapezium.Y2 = m_tRecogAreaParam[nIndex].Y2;
	trapezium.W2 = m_tRecogAreaParam[nIndex].W2;
	ui.widget_3->AddTrapezium(&trapezium, nIndex, nIndex + 1);
	ui.widget_3->SetColorByType(nIndex, COLOR_YELLOW);
}



void Dlg_Detection::RedLampIncreaseUpdate()
{
	ClearAllTreeWidgetItem(ui.treeWidget_RedLamp);
	ui.widget_3->Clear();
	for (int i = 0; i < MAX_REDLAMP_DET_NUM; i++)
	{
		QTreeWidgetItem *pListItem = new QTreeWidgetItem;
		pListItem->setData(0, Qt::UserRole, i + 1);
		pListItem->setSizeHint(0, QSize(60, 30));
		pListItem->setData(1, Qt::UserRole + 1, QVariant::fromValue(m_tSetRedEnInfo.tRedEnLoop[i]));
		pListItem->setText(0, QString::fromStdWString(redincrease::GetListValue(&m_tSetRedEnInfo.tRedEnLoop[i], 0)));
		pListItem->setText(1, QString::fromStdWString(redincrease::GetListValue(&m_tSetRedEnInfo.tRedEnLoop[i], 1)));
		pListItem->setText(2, QString::fromStdWString(redincrease::GetListValue(&m_tSetRedEnInfo.tRedEnLoop[i], 2)));
		pListItem->setText(3, QString::fromStdWString(redincrease::GetListValue(&m_tSetRedEnInfo.tRedEnLoop[i], 3)));
		pListItem->setText(4, QString::fromStdWString(redincrease::GetListValue(&m_tSetRedEnInfo.tRedEnLoop[i], 4)));
		pListItem->setText(5, QString::fromStdWString(redincrease::GetListValue(&m_tSetRedEnInfo.tRedEnLoop[i], 5)));
		pListItem->setText(6, QString::fromStdWString(redincrease::GetListValue(&m_tSetRedEnInfo.tRedEnLoop[i], 6)));
		pListItem->setText(7, QString::fromStdWString(redincrease::GetListValue(&m_tSetRedEnInfo.tRedEnLoop[i], 7)));

		ui.treeWidget_RedLamp->insertTopLevelItem(i, pListItem);
		QWidget*pWidget = CreateWidget(i);
		QWidget*pWidget2 = CreateWidget(i);
		QPushButton *btn1 = pWidget->property("btn").value<QPushButton*>();
		QPushButton *btn2 = pWidget2->property("btn").value<QPushButton*>();
		btn1->setProperty("nCol", 8);
		btn2->setProperty("nCol", 9);
		connect(btn1, SIGNAL(clicked()), this, SLOT(Slot_RedLampItemButtonClicked()));
		connect(btn2, SIGNAL(clicked()), this, SLOT(Slot_RedLampItemButtonClicked()));
		ui.treeWidget_RedLamp->setItemWidget(pListItem, 8, pWidget);
		ui.treeWidget_RedLamp->setItemWidget(pListItem, 9, pWidget2);
		
		RedLampIncreaseUpdateDraw(i, -1);
	}
	ui.widget_3->update();
}

void Dlg_Detection::RedLampIncreaseUpdateListItem(int nIndex, int nEUpdateItem)
{
	TIPC_RedSigEnhance &tRedEnLoop = m_tSetRedEnInfo.tRedEnLoop[nIndex];

	QTreeWidgetItem *pListItem = ui.treeWidget_RedLamp->topLevelItem(nIndex);
	if (pListItem == NULL)
		return;

	for (int i = 0; i < 8; i++)
	{
		if (i == nEUpdateItem)
		{
			pListItem->setText(i, QString::fromStdWString(redincrease::GetListValue(&tRedEnLoop, i)));
		}
	}
}

void Dlg_Detection::Slot_RedLampItemButtonClicked()
{
	int nIndex = this->sender()->property("nIndex").toInt();
	int nCol = this->sender()->property("nCol").toInt();
	if (nCol == 8)
	{
		Dlg_Detection_Area dlg(this, m_tSeceneCfg.tMaxParam.dwMaxWidth, m_tSeceneCfg.tMaxParam.dwMaxHeight);
		dlg.setWindowTitle(GBUTF8("红灯区域选择"));
		dlg.SetData(&m_tSetRedEnInfo.tRedEnLoop[nIndex].dwStartX,
			&m_tSetRedEnInfo.tRedEnLoop[nIndex].dwStartY,
			&m_tSetRedEnInfo.tRedEnLoop[nIndex].dwWidth,
			&m_tSetRedEnInfo.tRedEnLoop[nIndex].dwHeight);
		dlg.exec();

		//OnRedLampIncreaseSetListTipCtrl();
		RedLampIncreaseUpdateListItem(nIndex, 1);
		RedLampIncreaseUpdateListItem(nIndex, 2);
		RedLampIncreaseUpdateListItem(nIndex, 3);
		RedLampIncreaseUpdateListItem(nIndex, 4);
		RedLampIncreaseUpdateDraw(nIndex, -1);
		m_bChange = false;
	}
	else if (nCol == 9)
	{
		Dlg_Detection_Phase dlg;
		dlg.SetData(&m_tSetRedEnInfo.tRedEnLoop[nIndex].dwRefPhase);
		dlg.exec();
	}
}

void Dlg_Detection::RedLampIncreaseUpdateDraw(int nIndex, int nEUpdateItem)
{
	TIPC_RedSigEnhance& tRedSigEnhance = m_tSetRedEnInfo.tRedEnLoop[nIndex];

	if (nEUpdateItem != -1
		&& nEUpdateItem != 0
		&& nEUpdateItem != 1
		&& nEUpdateItem != 2
		&& nEUpdateItem != 3
		&& nEUpdateItem != 4)
	{
		return;
	}

	if (tRedSigEnhance.bEnable == false)
	{
		ui.widget_3->DelDrawData(nIndex);
		return;
	}

	TRECTANGLE_S area;
	int nType = nIndex;
	unsigned long color = 0;
	area.X = tRedSigEnhance.dwStartX;
	area.Y = tRedSigEnhance.dwStartY;
	area.W = tRedSigEnhance.dwWidth;
	area.H = tRedSigEnhance.dwHeight;
	ui.widget_3->AddRect(&area, nType);
	ui.widget_3->SetColorByType(nType, COLOR_RED);
}




void Dlg_Detection::UpdateData(bool bUpdate)
{
	if (!bUpdate)
	{
		switch (m_nTabIndex)
		{
		case 0:
			VideoLoopUpdate();
			break;
		case 1:
			CoilLoopUpdate();
			break;
		case 2:
			RedarLoopUpdate();
			break;
		case 3:
			RedLampSigUpdate();
			break;
		case 4:
			RecogAreaUpdate();
			break;
		case 5:
			RedLampIncreaseUpdate();
			break;
		default:
			break;
		}

		//OnSetListTipCtrl();
	}

	
}

void Dlg_Detection::OnVideoLoopSetListTipCtrl()
{
	QTreeWidgetItem *pItem = ui.treeWidget_video->currentItem();
	if (pItem == nullptr)
	{
		return;
	}
	int nIndex = ui.treeWidget_video->indexOfTopLevelItem(pItem);

	TIPC_VidLoopCfg* ptVideoLoop = &(m_tVideoLoopInfo.tVideoLoop[nIndex]);
	wchar_t ws[129];

	ui.widget_3->SetSelType(nIndex);
	QString sEnable = ptVideoLoop->bEnable ? GBUTF8("是") : GBUTF8("否");
	pItem->setText(1, sEnable);

	wsprintf(ws, TEXT("%d"), ptVideoLoop->dwStartX);
	pItem->setText(2, QString::fromWCharArray(ws));

	wsprintf(ws, TEXT("%d"), ptVideoLoop->dwStartY);
	pItem->setText(3, QString::fromWCharArray(ws));

	wsprintf(ws, TEXT("%d"), ptVideoLoop->dwWidth);
	pItem->setText(4, QString::fromWCharArray(ws));

	wsprintf(ws, TEXT("%d"), ptVideoLoop->dwHeight);
	pItem->setText(5, QString::fromWCharArray(ws));

	wsprintf(ws, TEXT("%d"), ptVideoLoop->X1);
	pItem->setText(6, QString::fromWCharArray(ws));

	wsprintf(ws, TEXT("%d"), ptVideoLoop->Y1);
	pItem->setText(7, QString::fromWCharArray(ws));

	wsprintf(ws, TEXT("%d"), ptVideoLoop->W1);
	pItem->setText(8, QString::fromWCharArray(ws));

	wsprintf(ws, TEXT("%d"), ptVideoLoop->X2);
	pItem->setText(9, QString::fromWCharArray(ws));

	wsprintf(ws, TEXT("%d"), ptVideoLoop->Y2);
	pItem->setText(10, QString::fromWCharArray(ws));

	wsprintf(ws, TEXT("%d"), ptVideoLoop->W2);
	pItem->setText(11, QString::fromWCharArray(ws));

	int temp = videoloop::GetMethodIndex(ptVideoLoop->dwMethod);
	pItem->setText(12, m_map1[temp]);

	temp = videoloop::GetDirectionModeIndex(ptVideoLoop->dwDirection);
	pItem->setText(13, m_map2[temp]);

	temp = videoloop::GetAngleModeIndex(ptVideoLoop->wPlateAngle);
	pItem->setText(14, m_map3[temp]);

	wsprintf(ws, TEXT("%d"), ptVideoLoop->dwPresentTime);
	pItem->setText(15, QString::fromWCharArray(ws));

	wsprintf(ws, TEXT("%d"), ptVideoLoop->dwTargetWidth);
	pItem->setText(16, QString::fromWCharArray(ws));
	
	wsprintf(ws, TEXT("%d"), ptVideoLoop->dwTargetHeight);
	pItem->setText(17, QString::fromWCharArray(ws));

	wsprintf(ws, TEXT("%d"), ptVideoLoop->dwSensitive);
	pItem->setText(18, QString::fromWCharArray(ws));

	wsprintf(ws, TEXT("%d"), ptVideoLoop->dwFilterOnTimes);
	pItem->setText(19, QString::fromWCharArray(ws));

	wsprintf(ws, TEXT("%d"), ptVideoLoop->dwFilterOffTimes);
	pItem->setText(20, QString::fromWCharArray(ws));
	//m_tTipCtrl.SetNeedPost(true);
}

void Dlg_Detection::OnCoilLoopSetListTipCtrl()
{
	QTreeWidgetItem *pItem = ui.treeWidget_video->currentItem();
	if (pItem == nullptr)
	{
		return;
	}
	int nIndex = ui.treeWidget_video->indexOfTopLevelItem(pItem);

	TIPC_CoilLoopCfg& tCoilLoop = m_tCoilLoopInfo.tCoilLoop[nIndex];
	wchar_t ws[129];

	QString sEnable = tCoilLoop.bEnable ? GBUTF8("是") : GBUTF8("否");
	pItem->setText(1, sEnable);
	
	/*
	if (m_tTipCtrl2.pListItem[i] != NULL)
	{
		m_tTipCtrl2.pListItem[i]->SetCmbValue(tCoilLoop.dwSerialChnId);
	}
	i++;

	if (m_tTipCtrl2.pListItem[i] != NULL)
	{
		m_tTipCtrl2.pListItem[i]->SetCmbValue(tCoilLoop.dwCoilChnId);
	}
	i++;

	if (m_tTipCtrl2.pListItem[i] != NULL)
	{
		m_tTipCtrl2.pListItem[i]->SetCmbValue(tCoilLoop.dwDirection - 1);
	}
	i++;

	if (m_tTipCtrl2.pListItem[i] != NULL)
	{
		wsprintf(ws, TEXT("%d"), tCoilLoop.dwV_distance);
		m_tTipCtrl2.pListItem[i]->SetEditValue(ws);
	}
	i++;

	if (m_tTipCtrl2.pListItem[i] != NULL)
	{
		char buf[24];
		sprintf(buf, "%f", tCoilLoop.fCorrectRate);
		MultiByteToWideChar(CP_ACP, 0, buf, -1, ws, 128);
		m_tTipCtrl2.pListItem[i]->SetEditValue(ws);
	}
	i++;

	if (m_tTipCtrl2.pListItem[i] != NULL)
	{
		m_tTipCtrl2.pListItem[i]->SetCmbValue(tCoilLoop.dwIOLabel);
	}
	i++;

	if (m_tTipCtrl2.pListItem[i] != NULL)
	{
		m_tTipCtrl2.pListItem[i]->SetCmbValue(tCoilLoop.dwPhyLaneNo - 1);
	}
	i++;

	if (m_tTipCtrl2.pListItem[i] != NULL)
	{
		m_tTipCtrl2.pListItem[i]->SetCmbValue(tCoilLoop.dwCoilType);
	}
	i++;

	m_tTipCtrl2.SetNeedPost(true);
	*/
}



// 获取当前行参数到编辑行
void Dlg_Detection::OnSetListTipCtrl()
{
	switch (m_nTabIndex)
	{
	case 0:
		OnVideoLoopSetListTipCtrl();
		break;
	case 1:
		OnCoilLoopSetListTipCtrl();
		break;
	case 2:
		//OnRedarLoopSetListTipCtrl();
		break;
	case 3:
		//OnRedLampSigSetListTipCtrl();
		break;
	case 4:
		//OnRecogAreaSetListTipCtrl();
		break;
	case 5:
		//OnRedLampIncreaseSetListTipCtrl();
		break;
	default:
		break;
	}
}

Dlg_Detection::~Dlg_Detection()
{
	MgrData::getInstance()->GetMgrSubject()->Del(this);
	if (MgrData::getInstance()->GetMgrEvtLst())
	{
		MgrData::getInstance()->GetMgrEvtLst()->DelSender(static_cast<CObserver*>(this));
	}
	ReleaseVideoLoop();
	ReleaseCoil();
	ReleaseRadar();
	ReleaseRedLampSig();
	ReleaseRedLampIncrease();
	ReleaseRecogArea();
}

void Dlg_Detection::InitData(bool is)
{
	if (!is)
	{
		return;
	}
	// 进入当前窗口的起始位置
	m_bChange = false;
	OnSnap();
	ui.tabWidget->setProperty("id", 111);
	ui.tabWidget->setCurrentIndex(0);
	Sleep(1);
	TIPC_GetUICapsRsp *m_pUICap = MgrData::getInstance()->GetDevCap();
	if (m_pUICap->szCaps[EMUI_CAP_TYPE_ILLGPARK] == 0)
	{
		OnSwitchScene();
	}
	else
	{
		GetScenceID();
	}
}

void Dlg_Detection::GetScenceID()
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


void Dlg_Detection::OnSnap()
{
	u32 dwIP = MgrData::getInstance()->GetIP();
	ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dwIP);
	if (pCtrl != NULL)
	{
		pCtrl->IpcEvt_Snap();
	}
}

void Dlg_Detection::Slot_CoilLoopChanged()
{
	UpdateData(false);
}

void Dlg_Detection::OnChangeScenceNum()
{
	int nNum = m_tSeceneCfg.dwSceneSize;
	int nCount = ui.cmb_scence->count();
	if (nNum != nCount)
	{
		ui.cmb_scence->setProperty("id", 111);
		ui.cmb_scence->clear();
		if (nNum < m_nSceneIndex) m_nSceneIndex = 0;
		QStringList sList;
		for (int i = 1; i <= nNum; i++)
		{
			sList.append(QString("%1").arg(i));
		}
		ui.cmb_scence->setProperty("id", 111);
		ui.cmb_scence->addItems(sList);
		Sleep(1);
		ui.cmb_scence->setProperty("id", 111);
		ui.cmb_scence->setCurrentIndex(m_nSceneIndex);
		Sleep(1);
	}

}

void Dlg_Detection::RecogAreaConvert1(TIPC_SetRecogAreaReq *ptIPC_SetRecogAreaReq, recogarea::TRecogAreaParam *ptRecogAreaParam)
{
	for (int i = 0; i < ROAD_NUM_MAX; i++)
	{
		ptRecogAreaParam[i].dwLabel = i + 1;
		ptRecogAreaParam[i].bEnable = 1;
		ptRecogAreaParam[i].X1 = ptIPC_SetRecogAreaReq->tRecogArea[i].tArea.X1;
		ptRecogAreaParam[i].Y1 = ptIPC_SetRecogAreaReq->tRecogArea[i].tArea.Y1;
		ptRecogAreaParam[i].W1 = ptIPC_SetRecogAreaReq->tRecogArea[i].tArea.W1;
		ptRecogAreaParam[i].X2 = ptIPC_SetRecogAreaReq->tRecogArea[i].tArea.X2;
		ptRecogAreaParam[i].Y2 = ptIPC_SetRecogAreaReq->tRecogArea[i].tArea.Y2;
		ptRecogAreaParam[i].W2 = ptIPC_SetRecogAreaReq->tRecogArea[i].tArea.W2;
		ptRecogAreaParam[i].dwPlateWidth = ptIPC_SetRecogAreaReq->tRecogArea[i].dwPlateWidth;

		switch (ptIPC_SetRecogAreaReq->tRecogArea[i].dwShootRear)
		{
		case DETECT_HEAD_VIEW:
			ptRecogAreaParam[i].dwShootRear = 0;
			break;
		case DETECT_TAIL_VIEW:
			ptRecogAreaParam[i].dwShootRear = 1;
			break;
		default:
			ptRecogAreaParam[i].dwShootRear = 0;
			break;
		}

		switch (ptIPC_SetRecogAreaReq->tRecogArea[i].dwPlateAngle)
		{
		case PLATE_ANGLE_C:
			ptRecogAreaParam[i].dwPlateAngle = 0;
			break;
		case PLATE_ANGLE_A_R:
			ptRecogAreaParam[i].dwPlateAngle = 1;
			break;
		case PLATE_ANGLE_A_L:
			ptRecogAreaParam[i].dwPlateAngle = 2;
			break;
		case PLATE_ANGLE_UN:
			ptRecogAreaParam[i].dwPlateAngle = 3;
			break;
		case PLATE_ANGLE_A_R_R:
			ptRecogAreaParam[i].dwPlateAngle = 4;
			break;
		case PLATE_ANGLE_A_L_L:
			ptRecogAreaParam[i].dwPlateAngle = 5;
			break;
		case PLATE_ANGLE_A_UN_UN:
			ptRecogAreaParam[i].dwPlateAngle = 6;
			break;
		default:
			ptRecogAreaParam[i].dwPlateAngle = 0;
			break;
		}
	}
}

bool Dlg_Detection::GetParamScenceInfo()
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
	return true;
}

// 线圈检测器获取
bool Dlg_Detection::GetParamCoilLoopInfo()
{
	MgrItsCtrl* pMgrItsCtrl = MgrData::getInstance()->GetMgrItsCtrl();
	if (pMgrItsCtrl != NULL)
	{
		u32 dwIP = MgrData::getInstance()->GetIP();
		ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dwIP);
		if (pCtrl != NULL)
		{
			int sceneid = m_nSceneIndex + 1;
			bool bRet = pCtrl->GetIpcCoilLoop(&sceneid, (u32)(CObserver*)this);
			if (!bRet)
			{
			}
		}
	}
	return true;
}

// 雷达检测器信息获取
bool Dlg_Detection::GetParamRedarLoopInfo()
{
	MgrItsCtrl* pMgrItsCtrl = MgrData::getInstance()->GetMgrItsCtrl();
	if (pMgrItsCtrl != NULL)
	{
		u32 dwIP = MgrData::getInstance()->GetIP();
		ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dwIP);
		if (pCtrl != NULL)
		{
			int sceneid = m_nSceneIndex + 1;
			bool bRet = pCtrl->GetIpcRedarLoop(&sceneid, (u32)(CObserver*)this);
			if (!bRet)
			{
			}
		}
	}
	return true;
}

// 红灯检测器信息获取
bool Dlg_Detection::GetParamRedLampSigInfo()
{
	MgrItsCtrl* pMgrItsCtrl = MgrData::getInstance()->GetMgrItsCtrl();
	if (pMgrItsCtrl != NULL)
	{
		u32 dwIP = MgrData::getInstance()->GetIP();
		ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dwIP);
		if (pCtrl != NULL)
		{
			int sceneid = m_nSceneIndex + 1;
			bool bRet = pCtrl->GetIpcRedSigLoop(&sceneid, (u32)(CObserver*)this);
			if (!bRet)
			{
			}
		}
	}
	return true;
}

// 红灯增强信息获取
bool Dlg_Detection::GetParamRedEnhanceInfo()
{
	MgrItsCtrl* pMgrItsCtrl = MgrData::getInstance()->GetMgrItsCtrl();
	if (pMgrItsCtrl != NULL)
	{
		u32 dwIP = MgrData::getInstance()->GetIP();
		ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dwIP);
		if (pCtrl != NULL)
		{
			int sceneid = m_nSceneIndex + 1;
			bool bRet = pCtrl->GetIpcRedEnhLoop(&sceneid, (u32)(CObserver*)this);
			if (!bRet)
			{
			}
		}
	}
	return true;
}

void Dlg_Detection::OnSwitchScene()
{
	GetParamScenceInfo();
	switch (m_nTabIndex)
	{
	case 0:
		GetParamVideoLoopInfo();
		break;
	case 1:
		GetParamCoilLoopInfo();
		break;
	case 2:
		GetParamRedarLoopInfo();
		break;
	case 3:
		GetParamRedLampSigInfo();
		break;
	case 4:
		GetParamRecogAreaInfo();
		break;
	case 5:
		GetParamRedEnhanceInfo();
		break;
	default:
		break;
	}
}




void Dlg_Detection::OnObserverNotify(LPARAM lHint, LPVOID pHint)
{
	u32 dwEvtType = (u32)lHint;
	u32 dwId = (u32)pHint;

	switch (dwEvtType)
	{
	case NMC_GET_IPC_VIDEOLOOPINFO:
	{
		TIPC_GetVideoLoopRsp* p = (TIPC_GetVideoLoopRsp*)pHint;
		if (p->nRetCode == 0)
		{
			memcpy(&(m_tVideoLoopInfo.tVideoLoop), &(p->tVideoLoop), sizeof(p->tVideoLoop));
			emit signal_videoLoopChanged();
			
			//BOOL bHandled = TRUE;
			//this->PostMessage(NMC_GET_IPC_VIDEOLOOPINFO);
		}
	}
		break;
	case NMC_GET_IPC_SCENCEINFO:
	{
		TIPC_GetSceneCfgRsp* p = (TIPC_GetSceneCfgRsp*)pHint;
		if (p->nRetCode == 0)
		{
			memcpy(&m_tSeceneCfg, p, sizeof(m_tSeceneCfg));
			emit signal_scenceinfo();
		}
	}
		break;
	case NMC_GET_IPC_RECOGAREA:
	{
		TIPC_GetRecogAreaRsp* p = (TIPC_GetRecogAreaRsp*)pHint;
		if (p->nRetCode == 0)
		{
			memcpy(&(m_tRecogAreaInfo.tRecogArea), &(p->tRecogArea), sizeof(TIPC_RecogArea) * ROAD_NUM_MAX);
			emit signal_recogAreaInfo();
			
		}
	}
		break;
	case NMC_GET_IPC_REDARLOOP:
	{
		TIPC_GetRadarLoopRsp* p = (TIPC_GetRadarLoopRsp*)pHint;
		if (p->nRetCode == 0)
		{
			memcpy(&(m_tRedarLoopInfo.tRadarLoop), &(p->tRadarLoop), sizeof(TIPC_RadarLoopCfg) * RADAR_NUM_MAX);
			emit signal_redarLoop();
		}
	}
		break;
	case NMC_GET_IPC_COILLOOP:
	{
		TIPC_GetCoilLoopRsp* p = (TIPC_GetCoilLoopRsp*)pHint;
		if (p->nRetCode == 0)
		{
			memcpy(&(m_tCoilLoopInfo.tCoilLoop), &(p->tCoilLoop), sizeof(TIPC_CoilLoopCfg) * COIL_LOOP_CHANNEL_MAX);
			emit signal_coilLoop();
			
		}
	}
		break;
	case NMC_GET_IPC_REDSIGLOOP:
	{
		TIPC_GetRedLampSigRsp* p = (TIPC_GetRedLampSigRsp*)pHint;
		if (p->nRetCode == 0)
		{
			memcpy(&(m_tRedLampSigInfo.tRedLoop), &(p->tRedLoop), sizeof(TIPC_RedLampSigLoop) * MAX_REDLAMP_DET_NUM);
			emit signal_redSigLoop();
			
		}
	}
		break;
	case NMC_GET_IPC_REDENHLOOP:
	{
		TIPC_GetRedEnRsp* p = (TIPC_GetRedEnRsp*)pHint;
		if (p->nRetCode == 0)
		{
			memcpy(&(m_tSetRedEnInfo.tRedEnLoop), &(p->tRedEnLoop), sizeof(TIPC_RedLampSigLoop) * MAX_REDLAMP_DET_NUM);
			emit signal_redEnhloop();
			
		}
	}
		break;
	case NMC_SET_IPC_CREATE_LOOP:
	{
		emit signal_createLoop();
	}
		break;
	case NMC_GET_IPC_IOLOOP:
		break;
	case NMC_EVT_VIDEO_PIC:
	{
		BOOL bHandled;
		OnRefreshPic(0, (WPARAM)0, 0, bHandled);
	}
		break;
	case NMC_GET_IPC_DRAW_CHANGE:
	{
		DrawDataToList();
	}
		break;
	case NMC_EVT_LOGOUT:
	{
		//CloseChildDlg();
	}
		break;
	case NMC_GET_IPC_SCENCE_ID:
	{
		Uint32 scenceId = *(Uint32*)pHint;
		m_nSceneIndex = scenceId;
		//ui.cmb_scence->setProperty("id", 111);
		//ui.cmb_scence->setCurrentIndex(m_nSceneIndex);
		//Sleep(1);
		OnSwitchScene();
	}
		break;
	default:
		break;
	}

}

void  Dlg_Detection::VideoLoopUpdateListItem(int nIndex, int nEUpdateItem)
{
	TIPC_VidLoopCfg& tVideoLoop = m_tVideoLoopInfo.tVideoLoop[nIndex];

	QTreeWidgetItem *pItem = ui.treeWidget_video->topLevelItem(nIndex);

	if (!pItem)
	{
		return;
	}

	for (int i = 0; i < videoloop::EVIDEOLOOP_COUNT; i++)
	{
		if (i == nEUpdateItem || nEUpdateItem == videoloop::EVIDEOLOOP_COUNT)
		{
			pItem->setText(i, QString::fromStdWString(videoloop::GetListValue(&tVideoLoop, i)));
		}
	}
}



void Dlg_Detection::DrawDataToList()
{
	TRECTANGLE_S tRectOut;
	TECHELON_S tTrapezium;
	int i = ui.widget_3->GetSelType();
	int drawMode = ui.widget_3->GetSelDrawMode();

	if (drawMode == EMDRAWMODE_TRAPEZIUM)
	{
		ui.widget_3->GetTrapeziumOut(&tTrapezium, i);
	}
	else
	{
		ui.widget_3->GetRectOut(&tRectOut, i);
	}

	switch (m_nTabIndex)
	{
	case 0:
	{
		if (drawMode == EMDRAWMODE_RECT)
		{
			m_tVideoLoopInfo.tVideoLoop[i].dwStartX = tRectOut.X;
			m_tVideoLoopInfo.tVideoLoop[i].dwStartY = tRectOut.Y;
			m_tVideoLoopInfo.tVideoLoop[i].dwWidth = tRectOut.W;
			m_tVideoLoopInfo.tVideoLoop[i].dwHeight = tRectOut.H;
			OnVideoLoopSetListTipCtrl();
			VideoLoopUpdateListItem(i, 2);
			VideoLoopUpdateListItem(i, 3);
			VideoLoopUpdateListItem(i, 4);
			VideoLoopUpdateListItem(i, 5);
		}
		else
		{
			m_tVideoLoopInfo.tVideoLoop[i].X1 = tTrapezium.X1;
			m_tVideoLoopInfo.tVideoLoop[i].Y1 = tTrapezium.Y1;
			m_tVideoLoopInfo.tVideoLoop[i].W1 = tTrapezium.W1;
			m_tVideoLoopInfo.tVideoLoop[i].X2 = tTrapezium.X2;
			m_tVideoLoopInfo.tVideoLoop[i].Y2 = tTrapezium.Y2;
			m_tVideoLoopInfo.tVideoLoop[i].W2 = tTrapezium.W2;
			OnVideoLoopSetListTipCtrl();
			VideoLoopUpdateListItem(i, 6);
			VideoLoopUpdateListItem(i, 7);
			VideoLoopUpdateListItem(i, 8);
			VideoLoopUpdateListItem(i, 9);
			VideoLoopUpdateListItem(i, 10);
			VideoLoopUpdateListItem(i, 11);
		}
	}
		break;
	case 4:
	{
		m_tRecogAreaParam[i].X1 = tTrapezium.X1;
		m_tRecogAreaParam[i].Y1 = tTrapezium.Y1;
		m_tRecogAreaParam[i].W1 = tTrapezium.W1;
		m_tRecogAreaParam[i].X2 = tTrapezium.X2;
		m_tRecogAreaParam[i].Y2 = tTrapezium.Y2;
		m_tRecogAreaParam[i].W2 = tTrapezium.W2;
		
		OnRecogAreaSetListTipCtrl();
		RecogAreaUpdateListItem(i, 2);
		RecogAreaUpdateListItem(i, 3);
		RecogAreaUpdateListItem(i, 4);
		RecogAreaUpdateListItem(i, 5);
		RecogAreaUpdateListItem(i, 6);
		RecogAreaUpdateListItem(i, 7);
	}
		break;
	default:
		break;
	}
}

void Dlg_Detection::RecogAreaUpdateListItem(int nIndex, int nEUpdateItem)
{
	recogarea::TRecogAreaParam& tRecogAreaParam = m_tRecogAreaParam[nIndex];

	QTreeWidgetItem *pListItem = ui.treeWidget_recogArea->topLevelItem(nIndex);
	if (pListItem == NULL)
	{
		return;
	}
	for (int i = 0; i < 11; i++)
	{
		if (i == nEUpdateItem)
		{
			pListItem->setText(i,  QString::fromStdWString(recogarea::GetListValue(&tRecogAreaParam, i)));
		}
	}
}



void Dlg_Detection::OnRecogAreaSetListTipCtrl()
{
	int nIndex = ui.treeWidget_recogArea->currentIndex().row();
	if (nIndex < 0)
		return;

	ui.widget_3->SetSelType(nIndex);
	QTreeWidgetItem *pItem = ui.treeWidget_recogArea->currentItem();
	recogarea::TRecogAreaParam& tRecogAreaParam = m_tRecogAreaParam[nIndex];
	wchar_t ws[129];

	pItem->setText(2, QString("%1").arg(tRecogAreaParam.X1));
	pItem->setText(3, QString("%1").arg(tRecogAreaParam.Y1));
	pItem->setText(4, QString("%1").arg(tRecogAreaParam.W1));
	pItem->setText(5, QString("%1").arg(tRecogAreaParam.X2));
	pItem->setText(6, QString("%1").arg(tRecogAreaParam.Y2));
	pItem->setText(7, QString("%1").arg(tRecogAreaParam.W2));
	pItem->setText(8, QString("%1").arg(tRecogAreaParam.dwPlateWidth));
	pItem->setText(9, QString("%1").arg(m_recogArea_map1[tRecogAreaParam.dwPlateAngle]));
	pItem->setText(10, QString("%1").arg(m_recogArea_map2[tRecogAreaParam.dwShootRear]));

}



void Dlg_Detection::VideoLoopUpdateDraw(int nIndex, int nEUpdateItem)
{
	bool bUpdate = false;
	TIPC_VidLoopCfg& tVideoLoop = m_tVideoLoopInfo.tVideoLoop[nIndex];

	if (nEUpdateItem == -1 || nEUpdateItem == 1)
	{
		bUpdate = true;
	}

	bool bTrapezium = m_bTrapezium;
	if (tVideoLoop.dwMethod == 5)
	{
		bTrapezium = false;
	}

	if (bTrapezium)
	{
		if (nEUpdateItem == 6
			|| nEUpdateItem == 7
			|| nEUpdateItem == 8
			|| nEUpdateItem == 9
			|| nEUpdateItem == 10
			|| nEUpdateItem == 11)
		{
			bUpdate = true;
		}
	}
	else
	{
		if (nEUpdateItem == 2
			|| nEUpdateItem == 3
			|| nEUpdateItem == 4
			|| nEUpdateItem == 5)
		{
			bUpdate = true;
		}
	}

	if (bUpdate == false)
	{
		return;
	}

	ui.widget_3->DelDrawData(nIndex);

	if (tVideoLoop.bEnable == false)
	{
		return;
	}

	int nType = nIndex;

	if (bTrapezium)
	{
		TECHELON_S trapezium;
		trapezium.X1 = tVideoLoop.X1;
		trapezium.Y1 = tVideoLoop.Y1;
		trapezium.W1 = tVideoLoop.W1;
		trapezium.X2 = tVideoLoop.X2;
		trapezium.Y2 = tVideoLoop.Y2;
		trapezium.W2 = tVideoLoop.W2;
		ui.widget_3->AddTrapezium(&trapezium, nType, tVideoLoop.dwLoopLabel);
	}
	else
	{
		TRECTANGLE_S area;
		area.X = tVideoLoop.dwStartX;
		area.Y = tVideoLoop.dwStartY;
		area.W = tVideoLoop.dwWidth;
		area.H = tVideoLoop.dwHeight;
		ui.widget_3->AddRect(&area, nType, tVideoLoop.dwLoopLabel);
	}

	ui.widget_3->SetSelType(nType);
	switch (tVideoLoop.dwMethod)
	{
	case 0:
		ui.widget_3->SetColorByType(nType, COLOR_YELLOW);
		break;
	case 1:
		ui.widget_3->SetColorByType(nType, COLOR_BLUE);
		break;
	case 2:
		ui.widget_3->SetColorByType(nType, COLOR_GREEN);
		break;
	case 3:
		ui.widget_3->SetColorByType(nType, COLOR_PURPLE);
		break;
	case 4:
		ui.widget_3->SetColorByType(nType, COLOR_BLACK);
		break;
	case 5:
		ui.widget_3->SetColorByType(nType, COLOR_RED);
		break;
	case 6:
		ui.widget_3->SetColorByType(nType, COLOR_WHITE);
		break;
	default:
		break;
	}
}





LRESULT Dlg_Detection::OnRefreshPic(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	u32 dwId = (u32)wParam;
	// 图片浏览取缓冲区中的图片
	if (uMsg == NMC_EVT_VIDEO_PIC)
		dwId = 0;

	Dlg_PictureDraw *pDraw = ui.widget_3;
	if (pDraw && pDraw->isVisible())
	{
		pDraw->SetNewPic2(dwId, 0);
		
	}
	if (ui.widget_3)
	{
		ui.widget_3->SetNewPic2(dwId, 0);
		//CRect rcPaint = m_pScenceDrawUI->GetPos();
		//InvalidateRect(*this, rcPaint, TRUE);
	}

	//
	bHandled = TRUE;
	return 0;
}

