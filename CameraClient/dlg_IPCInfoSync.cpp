#include "dlg_IPCInfoSync.h"
#include "MgrData.h"
#include <QListView>
Dlg_IPCInfoSync::Dlg_IPCInfoSync(QWidget *parent)
	: MyWidget(parent)
{
	MgrData::getInstance()->GetMgrSubject()->Add(this);
	ui.setupUi(this);
	connect(this, SIGNAL(dataChanged()), this, SLOT(slot_dataChanged()));
	connect(ui.btn_save, SIGNAL(clicked()), this, SLOT(slot_saveConfig()));
	connect(ui.check_videoRedStatus, SIGNAL(stateChanged(int)), this, SLOT(Slot_VideoRedStatusChanged(int)));
	Init();
}

void Dlg_IPCInfoSync::Slot_VideoRedStatusChanged(int nValue)
{
	ui.check_leftWaiting->setEnabled(nValue);
	ui.check_left->setEnabled(nValue);
	ui.check_straight->setEnabled(nValue);
	ui.check_right->setEnabled(nValue);
}

void Dlg_IPCInfoSync::InitData(bool is)
{
	if (is)
	{
		GetParam();
	}
}

void Dlg_IPCInfoSync::SaveConfig()
{
	m_infoSyncCfg.tInfoSyncCfg.wSnycType = 0;
	if (ui.check_NumberSync->isChecked())
	{
		m_infoSyncCfg.tInfoSyncCfg.wSnycType += 1;
	}
	if (ui.check_outerStatusSync->isChecked())
	{
		m_infoSyncCfg.tInfoSyncCfg.wSnycType += 2;
	}
	if (ui.check_videoRedStatus->isChecked())
	{
		m_infoSyncCfg.tInfoSyncCfg.wSnycType += 4;
	}
	int nCmbSync = ui.cmb_type->currentIndex();
	int nCmbCoil1 = ui.cmb_turnlocalSync->currentIndex();
	int nCmbCoil2 = ui.cmb_turnOuterSync->currentIndex();
	int nCmbCoil3 = ui.cmb_turnSyncNum->currentIndex();

	int nCmbRed1 = ui.cmb_signalLocalSync->currentIndex();
	int nCmbRed2 = ui.cmb_signalOuterSync->currentIndex();
	int nCmbRed3 = ui.cmb_signalSyncNum->currentIndex();

	int nCmbRadar1 = ui.cmb_infoLocalSync->currentIndex();
	int nCmbRadar2 = ui.cmb_infoOuterSync->currentIndex();
	int nCmbRadar3 = ui.cmb_infoSyncNum->currentIndex();

	m_infoSyncCfg.tInfoSyncCfg.byEVICameraType = nCmbSync + 1;

	m_infoSyncCfg.tInfoSyncCfg.byLocalIODetLabel = nCmbCoil1 + 1;
	m_infoSyncCfg.tInfoSyncCfg.byIODetStartLabel = nCmbCoil2 + 1;
	m_infoSyncCfg.tInfoSyncCfg.byIODetSize = nCmbCoil3;

	m_infoSyncCfg.tInfoSyncCfg.byLocalSigDetLabel = nCmbRed1 + 17;
	m_infoSyncCfg.tInfoSyncCfg.bySigDetStartLabel = nCmbRed2 + 17;
	m_infoSyncCfg.tInfoSyncCfg.bySigDetSize = nCmbRed3;

	m_infoSyncCfg.tInfoSyncCfg.byLocalRadarDetLabel = nCmbRadar1 + 33;
	m_infoSyncCfg.tInfoSyncCfg.byRadarDetStartLabel = nCmbRadar2 + 33;
	m_infoSyncCfg.tInfoSyncCfg.byRadarDetSize = nCmbRadar3;

	m_infoSyncCfg.tInfoSyncCfg.byVidRedLampType = 0;
	if (ui.check_leftWaiting->isChecked())
	{
		m_infoSyncCfg.tInfoSyncCfg.byVidRedLampType += 8;
	}

	if (ui.check_left->isChecked())
	{
		m_infoSyncCfg.tInfoSyncCfg.byVidRedLampType += 4;
	}

	if (ui.check_straight->isChecked())
	{
		m_infoSyncCfg.tInfoSyncCfg.byVidRedLampType += 2;
	}

	if (ui.check_right->isChecked())
	{
		m_infoSyncCfg.tInfoSyncCfg.byVidRedLampType += 1;
	}
}


void Dlg_IPCInfoSync::slot_dataChanged()
{

	if (m_infoSyncCfg.tInfoSyncCfg.wSnycType & 1)
	{
		ui.check_NumberSync->setChecked(true);
	}
	else
	{
		ui.check_NumberSync->setChecked(false);
	}

	if (m_infoSyncCfg.tInfoSyncCfg.wSnycType & 2)
	{
		ui.check_outerStatusSync->setChecked(true);
	}
	else
	{
		ui.check_outerStatusSync->setChecked(false);
	}
	ui.cmb_type->setCurrentIndex(m_infoSyncCfg.tInfoSyncCfg.byEVICameraType - 1);
	ui.cmb_turnlocalSync->setCurrentIndex(m_infoSyncCfg.tInfoSyncCfg.byLocalIODetLabel - 1);
	ui.cmb_turnOuterSync->setCurrentIndex(m_infoSyncCfg.tInfoSyncCfg.byIODetStartLabel - 1);
	ui.cmb_turnSyncNum->setCurrentIndex(m_infoSyncCfg.tInfoSyncCfg.byIODetSize);

	ui.cmb_signalLocalSync->setCurrentIndex(m_infoSyncCfg.tInfoSyncCfg.byLocalSigDetLabel - 17);
	ui.cmb_signalOuterSync->setCurrentIndex(m_infoSyncCfg.tInfoSyncCfg.bySigDetStartLabel - 17);
	ui.cmb_signalSyncNum->setCurrentIndex(m_infoSyncCfg.tInfoSyncCfg.bySigDetSize);

	ui.cmb_infoLocalSync->setCurrentIndex(m_infoSyncCfg.tInfoSyncCfg.byLocalRadarDetLabel - 33);
	ui.cmb_infoOuterSync->setCurrentIndex(m_infoSyncCfg.tInfoSyncCfg.byRadarDetStartLabel - 33);
	ui.cmb_infoSyncNum->setCurrentIndex(m_infoSyncCfg.tInfoSyncCfg.byRadarDetSize);

	bool isUse = false;
	if (m_infoSyncCfg.tInfoSyncCfg.wSnycType & 4)
	{
		isUse = true;
	}
	ui.check_videoRedStatus->setChecked(isUse);
	ui.check_leftWaiting->setChecked(isUse);
	ui.check_left->setChecked(isUse);
	ui.check_straight->setChecked(isUse);
	ui.check_right->setChecked(isUse);

	ui.check_leftWaiting->setEnabled(isUse);
	ui.check_left->setEnabled(isUse);
	ui.check_straight->setEnabled(isUse);
	ui.check_right->setEnabled(isUse);

	bool isLeftWaitting = false;
	if (m_infoSyncCfg.tInfoSyncCfg.byVidRedLampType & 8)
	{
		isLeftWaitting = true;
	}
	ui.check_leftWaiting->setChecked(isLeftWaitting);

	bool isLeft = false;
	if (m_infoSyncCfg.tInfoSyncCfg.byVidRedLampType & 4)
	{
		isLeft = true;
	}
	ui.check_left->setChecked(isLeft);

	bool isStraight = false;
	if (m_infoSyncCfg.tInfoSyncCfg.byVidRedLampType & 2)
	{
		isStraight = true;
	}
	ui.check_straight->setChecked(isStraight);

	bool isRigth = false;
	if (m_infoSyncCfg.tInfoSyncCfg.byVidRedLampType & 1)
	{
		isRigth = true;
	}
	ui.check_right->setChecked(isRigth);
}

//
void Dlg_IPCInfoSync::Init()
{
	ui.cmb_type->clear();
	ui.cmb_type->addItem(GBUTF8("一体机(转发)"));
	ui.cmb_type->addItem(GBUTF8("普通相机(接收)"));
	ui.cmb_type->setView(new QListView);
	ui.cmb_signalSyncNum->setView(new QListView);
	ui.cmb_infoSyncNum->setView(new QListView);
	ui.cmb_turnlocalSync->setView(new QListView);
	ui.cmb_turnOuterSync->setView(new QListView);
	ui.cmb_turnSyncNum->setView(new QListView);

	ui.cmb_signalLocalSync->setView(new QListView);
	ui.cmb_signalOuterSync->setView(new QListView);
	ui.cmb_signalSyncNum->setView(new QListView);

	ui.cmb_turnSyncNum->addItem(QString("%1").arg(0));
	ui.cmb_signalSyncNum->addItem(QString("%1").arg(0));
	ui.cmb_infoSyncNum->addItem(QString("%1").arg(0));
	for (int i = 0; i < 17; i++)
	{
		ui.cmb_turnlocalSync->addItem(QString("%1").arg(i));
		ui.cmb_turnOuterSync->addItem(QString("%1").arg(i));
		ui.cmb_turnSyncNum->addItem(QString("%1").arg(i));

		ui.cmb_signalLocalSync->addItem(QString("%1").arg(i+16));
		ui.cmb_signalOuterSync->addItem(QString("%1").arg(i+16));
		ui.cmb_signalSyncNum->addItem(QString("%1").arg(i));
	}

	for (int j = 0; j < 4; j++)
	{
		ui.cmb_infoLocalSync->addItem(QString("%1").arg(j+33));
		ui.cmb_infoOuterSync->addItem(QString("%1").arg(j+33));
		ui.cmb_infoSyncNum->addItem(QString("%1").arg(j));
	}
}

Dlg_IPCInfoSync::~Dlg_IPCInfoSync()
{
	MgrData::getInstance()->GetMgrSubject()->Del(this);
}

void Dlg_IPCInfoSync::OnObserverNotify(LPARAM lHint, LPVOID pHint)
{

	u32 dwEvtType = (u32)lHint;
	u32 dwId = (u32)pHint;

	switch (dwEvtType)
	{
	case NMC_GET_IPC_INFO_SYNC:
	{
		TIPC_GetInfoSyncCfgRsp* p = (TIPC_GetInfoSyncCfgRsp*)pHint;
		if (p->nRetCode == 0)
		{
			memcpy(&m_infoSyncCfg, p, sizeof(TIPC_GetInfoSyncCfgRsp));
			emit dataChanged();
		}
	}
		break;

	}
}


void Dlg_IPCInfoSync::slot_saveConfig()
{
	SaveConfig();
	SetParam();
}

void Dlg_IPCInfoSync::SetParam()
{
	u32 dIP = MgrData::getInstance()->GetIP();
	ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dIP);
	if (pCtrl == NULL)
	{
		return;
	}

	pCtrl->SetIpcInfoSyncCfg(&(m_infoSyncCfg.tInfoSyncCfg));
}

bool Dlg_IPCInfoSync::GetParam()
{
	MgrItsCtrl* pMgrItsCtrl = MgrData::getInstance()->GetMgrItsCtrl();
	if (pMgrItsCtrl != NULL)
	{
		u32 dIP = MgrData::getInstance()->GetIP();
		ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dIP);
		if (pCtrl != NULL)
		{
			bool bRet = pCtrl->GetIpcInfoSyncCfg(NULL, (u32)(CObserver*)this);
			if (!bRet)
			{
				return false;
			}
		}
	}
	return true;
}
