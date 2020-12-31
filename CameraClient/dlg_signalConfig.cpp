#include "dlg_signalConfig.h"
#include "MgrData.h"

Dlg_SignalConfig::Dlg_SignalConfig(QWidget *parent)
	: MyWidget(parent)
{
	ui.setupUi(this);
	MgrData::getInstance()->GetMgrSubject()->Add(this);
	connect(this, SIGNAL(dataChanged()), this, SLOT(slot_dataChanged()));
	connect(ui.btn_save, SIGNAL(clicked()), this, SLOT(slot_saveConfig()));
	Init();
}

void Dlg_SignalConfig::InitData(bool is)
{
	if (is)
	{
		GetParam();
	}
}

void Dlg_SignalConfig::Init()
{
	ui.com_protocol->clear();
	ui.com_protocol->addItem(GBUTF8("IO盒子"));
	ui.com_protocol->addItem(GBUTF8("骏马信号机"));
	ui.com_protocol->addItem(GBUTF8("声呐雷达"));

}

Dlg_SignalConfig::~Dlg_SignalConfig()
{
	MgrData::getInstance()->GetMgrSubject()->Del(this);
}

void Dlg_SignalConfig::OnObserverNotify(LPARAM lHint, LPVOID pHint)
{
	u32 dwEvtType = (u32)lHint;
	u32 dwId = (u32)pHint;

	switch (dwEvtType)
	{
	case NMC_GET_IPC_SIGNAL_PROTOCOL:
	{
		TIPC_Signal* p = (TIPC_Signal*)pHint;
		memcpy(&m_tData, p, sizeof(TIPC_Signal));
		emit dataChanged();
	}
		break;

	}
}

void Dlg_SignalConfig::SetParam()
{
	u32 dIp = MgrData::getInstance()->GetIP();
	ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dIp);
	if (pCtrl == NULL)
	{
		return;
	}

	pCtrl->SetIpcSignalProtocolCfg(&m_tData);
}

bool Dlg_SignalConfig::GetParam()
{
	MgrItsCtrl* pMgrItsCtrl = MgrData::getInstance()->GetMgrItsCtrl();
	if (pMgrItsCtrl != NULL)
	{
		u32 dIp = MgrData::getInstance()->GetIP();
		ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dIp);
		if (pCtrl != NULL)
		{
			bool bRet = pCtrl->GetIpcSignalProtocolCfg(NULL, (u32)(CObserver*)this);
			if (!bRet)
			{
				return false;
			}
		}
	}
	return true;
}

void Dlg_SignalConfig::SaveConfig()
{
	bool isEnable = ui.check_enable->isChecked();
	QString sIP = ui.ledt_ip->text();
	unsigned int dwIpAddr = inet_addr((char*)sIP.toStdString().c_str());
	QString sPort = ui.ledt_port->text();
	int nPort = sPort.toInt();
	int nProtocol = ui.com_protocol->currentIndex()+1;
	m_tData.enable = isEnable;
	m_tData.dwIp = dwIpAddr;
	m_tData.wPort = nPort;
	m_tData.protocol = nProtocol;

}

//QString int2ip(unsigned int ipInt){
//	int tokenInt = 0;
//	unsigned int leftValue = ipInt;
//	QString str;
//	for (int i = 0; i < 4; i++){
//		int temp = pow(256, 3 - i);
//		tokenInt = leftValue / temp;
//		leftValue %= temp;
//		str.append(QString("%1").arg(tokenInt));
//		if (i != 3){
//			str.append(".");
//		}
//
//	}
//	return str;
//}

void Dlg_SignalConfig::slot_dataChanged()
{
	QString sIP = MgrData::getInstance()->Int2IP(m_tData.dwIp);
	ui.check_enable->setChecked(m_tData.enable);
	ui.ledt_ip->setText(sIP);
	ui.ledt_port->setText(QString("%1").arg(m_tData.wPort));
	ui.com_protocol->setCurrentIndex(m_tData.protocol - 1);
}

void Dlg_SignalConfig::slot_saveConfig()
{
	SaveConfig();
	SetParam();
}