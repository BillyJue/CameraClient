
#include "dlg_main.h"
#include <QStackedWidget>
#include <QDebug>
#include <QListWidget>
#include  "shellapi.h" 
Dlg_main::Dlg_main(QWidget *parent)
	: CustomDialog(parent), m_dlgDeviceInfo(nullptr), m_dlgLocalConfig(nullptr)
	, m_dlgOtherConfig(nullptr), m_dlgSystemMaintenance(nullptr)
	, m_dlgIllegalDict(nullptr), m_dlgRegisterPlat(nullptr)
	, m_snapManager(nullptr), m_dlgLightConfig(nullptr)
	, m_dlgOsdConfig(nullptr), m_dlgPictureView(nullptr)
	, m_dlgDetection(nullptr)
	, m_dlgScene(nullptr), m_dlgImgQuality(nullptr)
	, m_dlgLogicLane(nullptr), m_dlgPicMerge(nullptr)
	, m_dlgIdentParam(nullptr), m_nIndexPage(-1), m_nCount(0)
	, m_dlgFlowDetection(nullptr), m_hardWare(nullptr)
	, m_dlgScenePoll(nullptr), m_currentWidget(nullptr)
{
	ui.setupUi(this);
	setWindowTitle(GBUTF8("ItsClient"));
	setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);
	//setWindowFlags(Qt::FramelessWindowHint);
	m_tUICap.bCap = false;
	connect(this, SIGNAL(signal_uiCapChanged()), this, SLOT(Slot_UiCapChanged()));
	connect(ui.btn_picView, SIGNAL(clicked()), this, SLOT(Slot_TabCurrentIndex()));
	connect(ui.btn_hardTest, SIGNAL(clicked()), this, SLOT(Slot_TabCurrentIndex()));
	connect(ui.btn_devInfo, SIGNAL(clicked()), this, SLOT(Slot_TabCurrentIndex()));
	connect(ui.btn_sence, SIGNAL(clicked()), this, SLOT(Slot_TabCurrentIndex()));
	connect(ui.btn_sencePoll, SIGNAL(clicked()), this, SLOT(Slot_TabCurrentIndex()));
	connect(ui.btn_test, SIGNAL(clicked()), this, SLOT(Slot_TabCurrentIndex()));
	connect(ui.btn_dection, SIGNAL(clicked()), this, SLOT(Slot_TabCurrentIndex()));
	connect(ui.btn_logicLane, SIGNAL(clicked()), this, SLOT(Slot_TabCurrentIndex()));
	connect(ui.btn_identParam, SIGNAL(clicked()), this, SLOT(Slot_TabCurrentIndex()));
	connect(ui.btn_imageQuatity, SIGNAL(clicked()), this, SLOT(Slot_TabCurrentIndex()));
	connect(ui.btn_imgMerge, SIGNAL(clicked()), this, SLOT(Slot_TabCurrentIndex()));
	connect(ui.btn_osdAdd, SIGNAL(clicked()), this, SLOT(Slot_TabCurrentIndex()));
	connect(ui.btn_illegalDict, SIGNAL(clicked()), this, SLOT(Slot_TabCurrentIndex()));
	connect(ui.btn_lightSetting, SIGNAL(clicked()), this, SLOT(Slot_TabCurrentIndex()));
	connect(ui.btn_regesiterPlat, SIGNAL(clicked()), this, SLOT(Slot_TabCurrentIndex()));
	connect(ui.btn_snap, SIGNAL(clicked()), this, SLOT(Slot_TabCurrentIndex()));
	connect(ui.btn_localCfg, SIGNAL(clicked()), this, SLOT(Slot_TabCurrentIndex()));
	connect(ui.btn_system, SIGNAL(clicked()), this, SLOT(Slot_TabCurrentIndex()));
	connect(ui.btn_otherConfig, SIGNAL(clicked()), this, SLOT(Slot_TabCurrentIndex()));

	connect(ui.btn_videoScan, SIGNAL(clicked()), this, SLOT(Slot_SelectPageClicked()));
	connect(ui.btn_paramSetting, SIGNAL(clicked()), this, SLOT(Slot_SelectPageClicked()));

	connect(ui.btn_ip, SIGNAL(clicked()), this, SLOT(Slot_OpenBrowserClicked()));
	connect(ui.btn_logout, SIGNAL(clicked()), this, SLOT(SLot_LogoutClicked()));
	connect(this, SIGNAL(signal_loginOut()), this, SLOT(SLot_LogoutClicked()));
	connect(ui.titleBar, SIGNAL(singal_sizeChanged()), this, SLOT(Slot_SizeChanged()));


	connect(ui.btn_leftup, SIGNAL(pressed()), this, SLOT(Slot_PTZControlClicked()));
	connect(ui.btn_up, SIGNAL(pressed()), this, SLOT(Slot_PTZControlClicked()));
	connect(ui.btn_rightup, SIGNAL(pressed()), this, SLOT(Slot_PTZControlClicked()));
	connect(ui.btn_left, SIGNAL(pressed()), this, SLOT(Slot_PTZControlClicked()));
	connect(ui.btn_out, SIGNAL(pressed()), this, SLOT(Slot_PTZControlClicked()));
	connect(ui.btn_right, SIGNAL(pressed()), this, SLOT(Slot_PTZControlClicked()));
	connect(ui.btn_leftdown, SIGNAL(pressed()), this, SLOT(Slot_PTZControlClicked()));
	connect(ui.btn_down, SIGNAL(pressed()), this, SLOT(Slot_PTZControlClicked()));
	connect(ui.btn_rightdown, SIGNAL(pressed()), this, SLOT(Slot_PTZControlClicked()));
	connect(ui.btn_zoomIn, SIGNAL(pressed()), this, SLOT(Slot_PTZControlClicked()));
	connect(ui.btn_zoomOut, SIGNAL(pressed()), this, SLOT(Slot_PTZControlClicked()));

	connect(ui.btn_leftup, SIGNAL(released()), this, SLOT(Slot_PTZStopClicked()));
	connect(ui.btn_up, SIGNAL(released()), this, SLOT(Slot_PTZStopClicked()));
	connect(ui.btn_rightup, SIGNAL(released()), this, SLOT(Slot_PTZStopClicked()));
	connect(ui.btn_left, SIGNAL(released()), this, SLOT(Slot_PTZStopClicked()));
	connect(ui.btn_out, SIGNAL(released()), this, SLOT(Slot_PTZStopClicked()));
	connect(ui.btn_right, SIGNAL(released()), this, SLOT(Slot_PTZStopClicked()));
	connect(ui.btn_leftdown, SIGNAL(released()), this, SLOT(Slot_PTZStopClicked()));
	connect(ui.btn_down, SIGNAL(released()), this, SLOT(Slot_PTZStopClicked()));
	connect(ui.btn_rightdown, SIGNAL(released()), this, SLOT(Slot_PTZStopClicked()));
	connect(ui.btn_zoomIn, SIGNAL(released()), this, SLOT(Slot_PTZStopClicked()));
	connect(ui.btn_zoomOut, SIGNAL(released()), this, SLOT(Slot_PTZStopClicked()));
	MgrData::getInstance()->GetMgrSubject()->Add(this);
	ui.titleBar->SetParentWidget(this);
	Init();
	InitData();
}

bool Dlg_main::event(QEvent *event)
{
	//&& event->type() == QEvent::MouseButtonDblClick
	if (this->sender() == ui.titleBar )
	{
		if (event->type() != QEvent::MouseButtonDblClick)
		{
			return QWidget::event(event);
		}
		if (this->sender()->property("maximizeProperty").toString() == "restore")
		{
			this->sender()->setProperty("maximizeProperty", "maximize");
			showMaximized();
			

		}
		else
		{
			this->sender()->setProperty("maximizeProperty", "restore");
			showNormal();
		}
	}
	return QWidget::event(event);
}


void Dlg_main::Slot_PTZStopClicked()
{
	char stop[32] = "zoomstop";
	if (this->sender() == ui.btn_zoomIn || this->sender() == ui.btn_zoomOut)
	{
		MgrData::getInstance()->GetMgrIpcSdk()->ptzMove(stop, 100);
	}
	else
	{
		MgrData::getInstance()->GetMgrIpcSdk()->stopMove();
	}
}

void Dlg_main::Slot_PTZControlClicked()
{
	char direction[32] = { 0 };
	int speed = 0;
	if (this->sender() == ui.btn_leftup)
	{
		strcpy(direction, "move_leftup");
		speed = ui.horizontalSlider->value();
		MgrData::getInstance()->GetMgrIpcSdk()->ptzMove(direction, speed);
	}

	if (this->sender() == ui.btn_up)
	{
		strcpy(direction, "moveup");
		speed = ui.horizontalSlider->value();
		MgrData::getInstance()->GetMgrIpcSdk()->ptzMove(direction, speed);
	}

	if (this->sender() == ui.btn_rightup)
	{
		strcpy(direction, "move_rightup");
		speed = ui.horizontalSlider->value();
		MgrData::getInstance()->GetMgrIpcSdk()->ptzMove(direction, speed);
	}

	if (this->sender() == ui.btn_left)
	{
		strcpy(direction, "moveleft");
		speed = ui.horizontalSlider->value();
		MgrData::getInstance()->GetMgrIpcSdk()->ptzMove(direction, speed);
	}

	if (this->sender() == ui.btn_right)
	{
		strcpy(direction, "moveright");
		speed = ui.horizontalSlider->value();
		MgrData::getInstance()->GetMgrIpcSdk()->ptzMove(direction, speed);
	}

	if (this->sender() == ui.btn_leftdown)
	{
		strcpy(direction, "move_leftdown");
		speed = ui.horizontalSlider->value();
		MgrData::getInstance()->GetMgrIpcSdk()->ptzMove(direction, speed);
	}

	if (this->sender() == ui.btn_down)
	{
		strcpy(direction, "movedown");
		speed = ui.horizontalSlider->value();
		MgrData::getInstance()->GetMgrIpcSdk()->ptzMove(direction, speed);
	}

	if (this->sender() == ui.btn_rightdown)
	{
		strcpy(direction, "move_rightdown");
		speed = ui.horizontalSlider->value();
		MgrData::getInstance()->GetMgrIpcSdk()->ptzMove(direction, speed);
	}

	if (this->sender() == ui.btn_zoomIn)
	{
		strcpy(direction, "zoomtele");
		speed = ui.horizontalSlider->value();
		MgrData::getInstance()->GetMgrIpcSdk()->ptzMove(direction, speed);
	}

	if (this->sender() == ui.btn_zoomOut)
	{
		strcpy(direction, "zoomwide");
		speed = ui.horizontalSlider->value();
		MgrData::getInstance()->GetMgrIpcSdk()->ptzMove(direction, speed);
	}

}

void Dlg_main::Slot_SizeChanged()
{
	this->update();
}

void Dlg_main::SLot_LogoutClicked()
{
	u32 dwIP = MgrData::getInstance()->GetIP();
	MgrData::getInstance()->GetMgrItsCtrl()->DelCtrl(dwIP);
	MgrData::getInstance()->SetLogin(false);
	this->done(1);
}

void Dlg_main::Slot_OpenBrowserClicked()
{
	QString sIP = ui.btn_ip->text();
	sIP = QString("http://%1").arg(sIP);
	ShellExecute(NULL, L"open", L"iexplore.exe", sIP.toStdWString().c_str(), NULL, SW_SHOW);
}

void Dlg_main::Slot_SelectPageClicked()
{
	int nIndex = -1;
	if (this->sender() == ui.btn_videoScan)
	{
		nIndex = 0;
		ui.widget_5->setVisible(false);
	}
	else if (this->sender() == ui.btn_paramSetting)
	{
		nIndex = 1;
		ui.widget_5->setVisible(true);
	}
	SelectPage(nIndex);
}

MyWidget* Dlg_main::GetWidget(QPushButton *pBtn)
{
	if (pBtn == ui.btn_picView )
	{
		if (!m_dlgPictureView)
		{
			m_dlgPictureView = new Dlg_PicView;
		}
		return m_dlgPictureView;
	}
	else if (pBtn == ui.btn_hardTest)
	{
		if (!m_hardWare)
		{
			m_hardWare = new Dlg_HardWare;
		}
		return m_hardWare;
	}
	else if (pBtn == ui.btn_devInfo)
	{
		if (!m_dlgDeviceInfo)
		{
			m_dlgDeviceInfo = new Dlg_DeviceInfo;
		}
		return m_dlgDeviceInfo;
	}
	else if (pBtn == ui.btn_sence)
	{
		if (!m_dlgScene)
		{
			m_dlgScene = new Dlg_Scene;
		}
		return m_dlgScene;
	}
	else if (pBtn == ui.btn_sencePoll)
	{
		if (!m_dlgScenePoll)
		{
			m_dlgScenePoll = new Dlg_ScenePoll;
		}
		return m_dlgScenePoll;
	}
	else if (pBtn == ui.btn_test)
	{
		if (!m_dlgFlowDetection)
		{
			m_dlgFlowDetection = new Dlg_FlowDetection;
		}
		return m_dlgFlowDetection;
	}
	else if (pBtn == ui.btn_dection)
	{
		if (!m_dlgDetection)
		{
			m_dlgDetection = new Dlg_Detection;
		}
		return m_dlgDetection;
	}
	else if (pBtn == ui.btn_logicLane)
	{
		if (!m_dlgLogicLane)
		{
			m_dlgLogicLane = new Dlg_LogicLane;
		}
		return m_dlgLogicLane;
	}
	else if (pBtn == ui.btn_identParam)
	{
		if (!m_dlgIdentParam)
		{
			m_dlgIdentParam = new Dlg_IdentParam;
		}
		return m_dlgIdentParam;
	}
	else if (pBtn == ui.btn_imageQuatity)
	{
		if (!m_dlgImgQuality)
		{
			m_dlgImgQuality = new Dlg_ImageQuality;
		}
		return m_dlgImgQuality;
	}
	else if (pBtn == ui.btn_imgMerge)
	{
		if (!m_dlgPicMerge)
		{
			m_dlgPicMerge = new Dlg_PicMerge;
		}
		return m_dlgPicMerge;
	}
	else if (pBtn == ui.btn_osdAdd)
	{
		if (!m_dlgOsdConfig)
		{
			m_dlgOsdConfig = new Dlg_OsdAddConfig;
		}
		return m_dlgOsdConfig;
	}
	else if (pBtn == ui.btn_illegalDict)
	{
		if (!m_dlgIllegalDict)
		{
			m_dlgIllegalDict = new Dlg_IllegalDict;
		}
		return m_dlgIllegalDict;
	}
	else if (pBtn == ui.btn_lightSetting)
	{
		if (!m_dlgLightConfig)
		{
			m_dlgLightConfig = new Dlg_LightConfig;
		}
		return m_dlgLightConfig;
	}
	else if (pBtn == ui.btn_regesiterPlat)
	{
		if (!m_dlgRegisterPlat)
		{
			m_dlgRegisterPlat = new Dlg_RegisterPlat;
		}
		return m_dlgRegisterPlat;
	}
	else if (pBtn == ui.btn_snap)
	{
		if (!m_snapManager)
		{
			m_snapManager = new Dlg_SnapManager;
		}
		return m_snapManager;
	}
	else if (pBtn == ui.btn_localCfg)
	{
		if (!m_dlgLocalConfig)
		{
			m_dlgLocalConfig = new Dlg_LocalConfig;
		}
		return m_dlgLocalConfig;
	}
	else if (pBtn == ui.btn_system)
	{
		if (!m_dlgSystemMaintenance)
		{
			m_dlgSystemMaintenance = new Dlg_systemMaintenance;
		}
		return m_dlgSystemMaintenance;
	}
	else if (pBtn == ui.btn_otherConfig)
	{
		if (!m_dlgOtherConfig)
		{
			m_dlgOtherConfig = new Dlg_otherConfig;
		}
		return m_dlgOtherConfig;
	}
}

void Dlg_main::Init()
{
	ui.btn_picView->setProperty("nCreateType", -1);
	m_mapBtnInfo.insert(0, ui.btn_picView);

	
	ui.btn_hardTest->setProperty("nCreateType", -1);
	m_mapBtnInfo.insert(1, ui.btn_hardTest);

	ui.btn_devInfo->setProperty("nCreateType", -1);
	m_mapBtnInfo.insert(2, ui.btn_devInfo);

	ui.btn_sence->setProperty("nCreateType", EMUI_CAP_TYPE_SCENE);
	m_mapBtnInfo.insert(3, ui.btn_sence);

	ui.btn_sencePoll->setProperty("nCreateType", EMUI_CAP_TYPE_ILLGPARK);
	m_mapBtnInfo.insert(4, ui.btn_sencePoll);

	ui.btn_test->setProperty("nCreateType", EMUI_CAP_TYPE_FLOWDET);
	m_mapBtnInfo.insert(5, ui.btn_test);

	ui.btn_dection->setProperty("nCreateType", EMUI_CAP_TYPE_DETECTION);
	m_mapBtnInfo.insert(6, ui.btn_dection);

	ui.btn_logicLane->setProperty("nCreateType", EMUI_CAP_TYPE_LOGICLANE);
	m_mapBtnInfo.insert(7, ui.btn_logicLane);

	ui.btn_identParam->setProperty("nCreateType", EMUI_CAP_TYPE_IDENTPARAM);
	m_mapBtnInfo.insert(8, ui.btn_identParam);

	ui.btn_imageQuatity->setProperty("nCreateType", EMUI_CAP_TYPE_IMAGEQUALITY);
	m_mapBtnInfo.insert(9, ui.btn_imageQuatity);

	ui.btn_imgMerge->setProperty("nCreateType", EMUI_CAP_TYPE_SYSPICMERGE);
	m_mapBtnInfo.insert(10, ui.btn_imgMerge);

	ui.btn_osdAdd->setProperty("nCreateType", EMUI_CAP_TYPE_OSD);
	m_mapBtnInfo.insert(11, ui.btn_osdAdd);

	ui.btn_illegalDict->setProperty("nCreateType", -1);
	m_mapBtnInfo.insert(12, ui.btn_illegalDict);

	ui.btn_lightSetting->setProperty("nCreateType", EMUI_CAP_TYPE_ILLGPARK);
	m_mapBtnInfo.insert(13, ui.btn_lightSetting);

	ui.btn_regesiterPlat->setProperty("nCreateType", EMUI_CAP_TYPE_REGISTERPLAT);
	m_mapBtnInfo.insert(14, ui.btn_regesiterPlat);


	ui.btn_snap->setProperty("nCreateType", -1);
	m_mapBtnInfo.insert(15, ui.btn_snap);

	ui.btn_localCfg->setProperty("nCreateType", -1);
	m_mapBtnInfo.insert(16, ui.btn_localCfg);

	ui.btn_system->setProperty("nCreateType", -1);
	m_mapBtnInfo.insert(17, ui.btn_system);

	ui.btn_otherConfig->setProperty("nCreateType", -1);
	m_mapBtnInfo.insert(18, ui.btn_otherConfig);
}

void Dlg_main::InitData()
{
	u32 dwIP = MgrData::getInstance()->GetIP();
	ItsCtrl *ptCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dwIP);
	if (ptCtrl != NULL)
	{
		ptCtrl->GetIpcDevInfo(NULL, (u32)(CObserver*)this);
	}
	SelectPage(1);
	TLocalCfgInfo localCfg;
	wchar_t* pName = MgrData::getInstance()->GetMgrUser()->GetCurUser()->szName;
	ui.lab_usr->setText(QString::fromWCharArray(pName));

	unsigned char *pch = (unsigned char*)&dwIP;
	char szIPAddr[64] = { 0 };
	sprintf(szIPAddr, "%d.%d.%d.%d", *pch, *(pch + 1), *(pch + 2), *(pch + 3));
	
	ui.btn_ip->setText(QString("%1").arg(szIPAddr));

}

void Dlg_main::Slot_UiCapChanged()
{
	MgrData::getInstance()->SetDevCap(&m_tUICap);
	InitConfigBtn();
}

void Dlg_main::Slot_TabCurrentIndex()
{
	QPushButton *pSender = dynamic_cast<QPushButton*>(this->sender());
	if (GetWidget(pSender) == m_currentWidget)
	{
		return;
	}

	if (m_currentWidget)
	{
		m_currentWidget->InitData(false);
	}
	
	m_currentWidget = GetWidget(pSender);
	int nIndex = m_currentWidget->property("nIndex").toInt();
	ui.stackedWidget_2->setCurrentIndex(nIndex);
	m_currentWidget->InitData(true);
}

void Dlg_main::InitConfigBtn()
{
	while (ui.stackedWidget_2->count() > 0)
	{
		QWidget *tempWidget = ui.stackedWidget_2->widget(0);
		ui.stackedWidget_2->removeWidget(tempWidget);
	}
	// 依据配置显示
	int nIndex = 0;
	QMap<int, QPushButton*>::iterator ite;
	ite = m_mapBtnInfo.begin();
	while (ite != m_mapBtnInfo.end())
	{
		QPushButton *pBtn = ite.value();
		int nCreateType =  pBtn->property("nCreateType").toInt();
		if (pBtn != NULL && ((!m_tUICap.bCap || nCreateType == -1) || (m_tUICap.bCap && m_tUICap.szCaps[nCreateType] > 0)))
		{
			MyWidget*pWidget = GetWidget(pBtn);
			pWidget->setProperty("nIndex", nIndex);
			ui.stackedWidget_2->addWidget(pWidget);
			pBtn->setVisible(true);
			nIndex++;
		}
		else
		{
			pBtn->setVisible(false);
		}
		
		ite++;
	}

	// 序列中的第一个窗口显示
	ite = m_mapBtnInfo.begin();
	while (ite != m_mapBtnInfo.end())
	{
		QPushButton *pBtn = ite.value();
		int nCreateType = pBtn->property("nCreateType").toInt();
		if (pBtn != NULL && ((!m_tUICap.bCap || nCreateType == -1) || (m_tUICap.bCap && m_tUICap.szCaps[nCreateType] > 0)))
		{
			pBtn->setChecked(true);
			MyWidget *pWidget = dynamic_cast<MyWidget*>(GetWidget(pBtn));
			pWidget->InitData(true);
			m_currentWidget = pWidget;
			/*SelSwitch(pBtn);
			pBtn->Selected(true);*/
			break;
		}
		ite++;
	}
}



void Dlg_main::Slot_ItemClicked(QListWidgetItem *pItem)
{
	/*QString sText = pItem->text();
	if (sText == GBUTF8("注册平台"))
	{
		ui.stackedWidget_2->setCurrentIndex(2);
		m_dlgRegisterPlat->GetParam();
	}
	else
	{
		ui.stackedWidget_2->setCurrentIndex(0);
	}*/
}

void Dlg_main::OnObserverNotify(LPARAM lHint, LPVOID pHint)
{
	if (lHint == NMC_CFG_SETIP)
	{
		/*u32 dwIP = *(u32*)pHint;
		SetIP(dwIP);*/
		
	}
	if (lHint == NMC_EVT_LOGIN)
	{
		
	}
	else if (lHint == NMC_EVT_LOGOUT)
	{
		emit signal_loginOut();
	}
	else if (lHint == NMC_GET_UICAP)
	{
		TIPC_GetUICapsRsp* p = (TIPC_GetUICapsRsp*)pHint;
		if (p->nRetCode == 0)
		{
			memcpy(&m_tUICap, p, sizeof(TIPC_GetUICapsRsp));
			emit signal_uiCapChanged();
		}
	}
	else if (lHint == NMC_GET_IPC_DEVINFO)
	{
		TIPC_GetDevRsp* p = (TIPC_GetDevRsp*)pHint;
		if (p->nRetCode == 0)
		{
			MgrData::getInstance()->SetDevInfo(&(p->tDev));
			wchar_t ipcType[64];
			char2wchar(ipcType, p->tDev.szDevType);
			ui.devtype->setText(QString::fromWCharArray(ipcType));
			
		}
	}
}



void Dlg_main::SelectPage(int index)
{
	if (m_nIndexPage == index)
	{
		return;
	}

	switch (m_nIndexPage)
	{
	case 0:
	{
		ui.video->ControlPlayVideo(false);
	}
		break;
	case 1:
	{
		m_currentWidget->InitData(false);
	}
		break;
	default:
		break;
	}

	m_nIndexPage = index;
	switch (index)
	{
	case 0:
	{
		ui.video->ControlPlayVideo(true);
		ui.stackedWidget->setCurrentIndex(1);
	}
		break;
	case 1:
	{
		ui.stackedWidget->setCurrentIndex(0);
		if (m_nCount == 0)
		{
			m_nCount++;
			GetUICap();
		}
		else
		{
			m_currentWidget->InitData(true);
		}
	}
		break;
	default:
		break;
	}

}

bool Dlg_main::GetUICap()
{
	u32 dwIP = MgrData::getInstance()->GetIP();
	ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dwIP);
	if (pCtrl != NULL)
	{
		bool bRet = pCtrl->GetUICap((u32)(CObserver*)this);
		if (!bRet)
		{
		}

		return bRet;
	}

	return false;
}

void Dlg_main::GetDevInfo()
{
	TIPC_DevInfo *pDevInfo;

	pDevInfo = MgrData::getInstance()->GetDevInfo();
	wchar_t ipcType[64];
	char2wchar(ipcType, pDevInfo->szDevType);
	ui.devtype->setText(QString::fromWCharArray(ipcType));
}


Dlg_main::~Dlg_main()
{
	MgrData::getInstance()->GetMgrSubject()->Del(this);
	SAFE_DELETE(m_dlgDeviceInfo);
	SAFE_DELETE(m_hardWare);
	SAFE_DELETE(m_dlgPictureView);
	SAFE_DELETE(m_dlgLocalConfig);
	SAFE_DELETE(m_dlgOtherConfig);
	SAFE_DELETE(m_dlgSystemMaintenance);
	SAFE_DELETE(m_dlgIllegalDict);
	SAFE_DELETE(m_dlgRegisterPlat);
	SAFE_DELETE(m_snapManager);
	SAFE_DELETE(m_dlgLightConfig);
	SAFE_DELETE(m_dlgOsdConfig);
	SAFE_DELETE(m_dlgDetection);
	SAFE_DELETE(m_dlgScene);
	SAFE_DELETE(m_dlgImgQuality);
	SAFE_DELETE(m_dlgLogicLane);
	SAFE_DELETE(m_dlgPicMerge);
	SAFE_DELETE(m_dlgIdentParam);
	SAFE_DELETE(m_dlgFlowDetection);

}
