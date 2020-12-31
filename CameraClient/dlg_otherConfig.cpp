#include "dlg_otherConfig.h"
#include "MgrData.h"
Dlg_otherConfig::Dlg_otherConfig(QWidget *parent)
	: MyWidget(parent)
{
	ui.setupUi(this);
	//connect(ui.btn_hardWareTest, SIGNAL(clicked()), this, SLOT(Slot_HardWareTestClicked()));
	connect(ui.btn_videoSpeed, SIGNAL(clicked()), this, SLOT(Slot_VideoSpeedClicked()));
	connect(ui.btn_powerTB, SIGNAL(clicked()), this, SLOT(Slot_PowerSyncClicked()));
	connect(ui.btn_serialSetting, SIGNAL(clicked()), this, SLOT(Slot_SerialSettingClicked()));
	connect(ui.btn_signalSetting, SIGNAL(clicked()), this, SLOT(Slot_SignalSettingClicked()));
	connect(ui.btn_signalTB, SIGNAL(clicked()), this, SLOT(Slot_SignalSyncClicked()));
	
	AddStackedWidget();
	
}

void Dlg_otherConfig::InitData(bool is)
{
	if (is)
	{
		emit ui.btn_videoSpeed->clicked(true);
	}
}

void Dlg_otherConfig::Slot_HardWareTestClicked()
{
	MyWidget *pSender = dynamic_cast<MyWidget*>(this->sender());
	pSender->InitData(true);
	ui.stackedWidget->setCurrentIndex(0);
	
}
void Dlg_otherConfig::Slot_VideoSpeedClicked()
{
	MyWidget *pSender = dynamic_cast<MyWidget*>(m_dlgVelometer);
	pSender->InitData(true);
	ui.stackedWidget->setCurrentIndex(0);
}

void Dlg_otherConfig::Slot_PowerSyncClicked()
{
	MyWidget *pSender = dynamic_cast<MyWidget*>(m_dlgPowerSync);
	pSender->InitData(true);
	ui.stackedWidget->setCurrentIndex(1);
}

void Dlg_otherConfig::Slot_SerialSettingClicked()
{
	MyWidget *pSender = dynamic_cast<MyWidget*>(m_dlgSerial);
	pSender->InitData(true);
	ui.stackedWidget->setCurrentIndex(2);
}

void Dlg_otherConfig::Slot_SignalSettingClicked()
{
	MyWidget *pSender = dynamic_cast<MyWidget*>(m_dlgSignalConfig);
	pSender->InitData(true);
	ui.stackedWidget->setCurrentIndex(3);
}

void Dlg_otherConfig::Slot_SignalSyncClicked()
{
	MyWidget *pSender = dynamic_cast<MyWidget*>(m_dlgIpcInfoSync);
	pSender->InitData(true);
	ui.stackedWidget->setCurrentIndex(4);
}


Dlg_otherConfig::~Dlg_otherConfig()
{
	//SAFE_DELETE(m_dlgHardWare);
	SAFE_DELETE(m_dlgVelometer);
	SAFE_DELETE(m_dlgPowerSync);
	SAFE_DELETE(m_dlgSerial);
	SAFE_DELETE(m_dlgSignalConfig);
	SAFE_DELETE(m_dlgIpcInfoSync);
}

void Dlg_otherConfig::AddStackedWidget()
{
	while (ui.stackedWidget->count() > 0)
	{
		QWidget *tempWidget = ui.stackedWidget->widget(0);
		ui.stackedWidget->removeWidget(tempWidget);
		delete tempWidget;
		tempWidget = nullptr;
	}
	//m_dlgHardWare = new Dlg_HardWare;
	m_dlgVelometer = new Dlg_velometer;
	m_dlgPowerSync = new Dlg_PowerSyn;
	m_dlgSerial = new Dlg_Serial;
	m_dlgSignalConfig = new Dlg_SignalConfig;
	m_dlgIpcInfoSync = new Dlg_IPCInfoSync;

	//ui.stackedWidget->addWidget(m_dlgHardWare);
	ui.stackedWidget->addWidget(m_dlgVelometer);
	ui.stackedWidget->addWidget(m_dlgPowerSync);
	ui.stackedWidget->addWidget(m_dlgSerial);
	ui.stackedWidget->addWidget(m_dlgSignalConfig);
	ui.stackedWidget->addWidget(m_dlgIpcInfoSync);

	TLocalCfgInfo tLocalCfgInfo;
	MgrData::getInstance()->GetMgrLocalCfg()->GetLocalCfg(&tLocalCfgInfo);
	if (false)//(tLocalCfgInfo.isHardware)
	{
		//m_dlgHardWare->InitData(true);
		ui.stackedWidget->setCurrentIndex(0);
	}
	else
	{
		//ui.btn_hardWareTest->setVisible(false);
		ui.stackedWidget->setCurrentIndex(0);
	}
	
}