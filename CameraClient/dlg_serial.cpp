#include "dlg_serial.h"
#include "MgrData.h"
#include <QListView>
#include <QTimer>
Dlg_Serial::Dlg_Serial(QWidget *parent)
	: MyWidget(parent), m_messageBox(nullptr), m_nSelect(0)
{
	ui.setupUi(this);
	MgrData::getInstance()->GetMgrSubject()->Add(this);
	connect(this, SIGNAL(dataChanged()), this, SLOT(Slot_DataChanged()));
	connect(ui.btn_save, SIGNAL(clicked()), this, SLOT(Slot_BtnSaveClicked()));
	connect(ui.com_serial, SIGNAL(currentIndexChanged(int)), this, SLOT(Slot_CmbCurrentIndexChanged(int)));
	connect(ui.com_control, SIGNAL(currentIndexChanged(int)), this, SLOT(Slot_CmbCurrentIndexChanged(int)));
	init();
	//GetParam();
}

void Dlg_Serial::InitData(bool is)
{
	if (is)
	{
		GetParam();
	}
}

void Dlg_Serial::GetData()
{
	m_tIPC_SerialCfg.atSerialParams[m_nSelect].dwBaudRate = ui.com_BaudRate->currentText().toInt();
	m_tIPC_SerialCfg.atSerialParams[m_nSelect].dwDataBit = ui.com_dataBit->currentText().toInt();
	m_tIPC_SerialCfg.atSerialParams[m_nSelect].dwStopBit = ui.com_stop->currentText().toInt();
	m_tIPC_SerialCfg.atSerialParams[m_nSelect].dwParity = ui.com_checkout->currentIndex();
	int nSel = ui.com_control->currentIndex();
	switch (nSel)
	{
	case 0:
		m_tIPC_SerialCfg.atSerialParams[m_nSelect].dwProtocolType = 255;
		break;
	case 1:
		m_tIPC_SerialCfg.atSerialParams[m_nSelect].dwProtocolType = E_RADAR_CSR_IN;
		break;
	case 2:
		m_tIPC_SerialCfg.atSerialParams[m_nSelect].dwProtocolType = E_HL_DET_MODULE_V2;
		break;
	case 3:
		m_tIPC_SerialCfg.atSerialParams[m_nSelect].dwProtocolType = E_RADAR_802_2D;
		break;
	case 4:
		m_tIPC_SerialCfg.atSerialParams[m_nSelect].dwProtocolType = E_HL_DET_MODULE_V1;
		break;
	case 5:
		m_tIPC_SerialCfg.atSerialParams[m_nSelect].dwProtocolType = E_RADAR_HC;
		break;
	case 6:
		m_tIPC_SerialCfg.atSerialParams[m_nSelect].dwProtocolType = E_RADAR_WK_MEP_II;
		break;
	default:
		m_tIPC_SerialCfg.atSerialParams[m_nSelect].dwProtocolType = nSel;
		break;
	}
}

void Dlg_Serial::Slot_BtnSaveClicked()
{
	GetData();
	SetParam();
	if (!m_messageBox)
	{
		QTimer::singleShot(1500, this, &Dlg_Serial::OnSetTxtVisible);
		m_messageBox = new Dlg_MessageBox;
		m_messageBox->SetInfoText(GBUTF8("保存成功！"));
		m_messageBox->SetBtnNoVisible(false);
		m_messageBox->exec();
	}
}


void Dlg_Serial::Slot_CmbCurrentIndexChanged(int nSel)
{
	if (this->sender() == ui.com_control)
	{
		SelectProtocol(nSel);
	}
	else if (this->sender() == ui.com_serial)
	{
		GetData();
		m_nSelect = nSel;
		SetData();
	}
	
}

void Dlg_Serial::SelectProtocol(int nIndex)
{
	if (nIndex != 0)
	{
		ui.com_BaudRate->setEnabled(false);
		ui.com_stop->setEnabled(false);
		ui.com_dataBit->setEnabled(false);
		ui.com_checkout->setEnabled(false);

	}
	else
	{
		ui.com_BaudRate->setEnabled(true);
		ui.com_stop->setEnabled(true);
		ui.com_dataBit->setEnabled(true);
		ui.com_checkout->setEnabled(true);
	}

	switch (nIndex)
	{
	case 1:
	{
		ui.com_BaudRate->setCurrentIndex(2);
	}
		break;
	case 2:
	{
		ui.com_BaudRate->setCurrentIndex(6);
	}
		break;
	case 3:
	{
		ui.com_BaudRate->setCurrentIndex(6);
	}
		break;
	case 4:
	{
		ui.com_BaudRate->setCurrentIndex(2);
	}
		break;
	case 5:
	{
		ui.com_BaudRate->setCurrentIndex(6);
	}
		break;
	case 6:
	{
		ui.com_BaudRate->setCurrentIndex(2);
	}
		break;
	default:
		break;
	}
}


void Dlg_Serial::OnSetTxtVisible()
{
	if (m_messageBox)
	{
		m_messageBox->close();
		SAFE_DELETE(m_messageBox);
	}
}

void Dlg_Serial::init()
{
	ui.com_serial->setView(new QListView);
	ui.com_BaudRate->setView(new QListView);
	ui.com_dataBit->setView(new QListView);
	ui.com_checkout->setView(new QListView);
	ui.com_stop->setView(new QListView);
	ui.com_control->setView(new QListView);
	
}

Dlg_Serial::~Dlg_Serial()
{
	SAFE_DELETE(m_messageBox);
	MgrData::getInstance()->GetMgrSubject()->Del(this);
}

void Dlg_Serial::SetParam()
{
	u32 dIp = MgrData::getInstance()->GetIP();
	ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dIp);
	if (pCtrl == NULL)
	{
		return;
	}

	pCtrl->SetIpcSerialCfg(&m_tIPC_SerialCfg);

}

void Dlg_Serial::SetData()
{
	ui.com_BaudRate->setCurrentText(QString("%1").arg(m_tIPC_SerialCfg.atSerialParams[m_nSelect].dwBaudRate));
	ui.com_dataBit->setCurrentText(QString("%1").arg(m_tIPC_SerialCfg.atSerialParams[m_nSelect].dwDataBit));
	ui.com_checkout->setCurrentIndex(m_tIPC_SerialCfg.atSerialParams[m_nSelect].dwParity);
	ui.com_stop->setCurrentText(QString("%1").arg(m_tIPC_SerialCfg.atSerialParams[m_nSelect].dwStopBit));
	int nSel = ui.com_control->currentIndex();
	switch (m_tIPC_SerialCfg.atSerialParams[m_nSelect].dwProtocolType)
	{
	case 255:
		ui.com_control->setCurrentIndex(0);
		break;
	case E_RADAR_CSR_IN:
		ui.com_control->setCurrentIndex(1);
		break;
	case E_HL_DET_MODULE_V2:
		ui.com_control->setCurrentIndex(2);
		break;
	case E_RADAR_802_2D:
		ui.com_control->setCurrentIndex(3);
		break;
	case E_HL_DET_MODULE_V1:
		ui.com_control->setCurrentIndex(4);
		break;
	case E_RADAR_HC:
		ui.com_control->setCurrentIndex(5);
		break;
	case E_RADAR_WK_MEP_II:
		ui.com_control->setCurrentIndex(6);
		break;
	}
}

void Dlg_Serial::Slot_DataChanged()
{
	if (m_tIPC_SerialCfg.dwSerialNum > 0 && m_tIPC_SerialCfg.dwSerialNum <= MAX_SERIAL_NUM)
	{
		ui.com_serial->clear();
		for (int i = 0; i < m_tIPC_SerialCfg.dwSerialNum; ++i)
		{
			ui.com_serial->addItem(GBUTF8("串口%1").arg(i+1));
		}
		ui.com_serial->setCurrentIndex(m_nSelect);
		m_nSelect = 0;
		SetData();
	}
}

void Dlg_Serial::OnObserverNotify(LPARAM lHint, LPVOID pHint)
{

	u32 dwEvtType = (u32)lHint;
	u32 dwId = (u32)pHint;

	switch (dwEvtType)
	{
	case NMC_GET_IPC_SERIALCFG:
	{
		TIPC_GetSerialCfgRsp* p = (TIPC_GetSerialCfgRsp*)pHint;
		if (p->nRetCode == 0)
		{
			memcpy(&m_tIPC_SerialCfg, &(p->tSerialCfg), sizeof(m_tIPC_SerialCfg));
			emit dataChanged();
		}
	}
	break;

	}
}


bool Dlg_Serial::GetParam()
{
	MgrItsCtrl* pMgrItsCtrl = MgrData::getInstance()->GetMgrItsCtrl();
	if (pMgrItsCtrl != NULL)
	{
		u32 dIp = MgrData::getInstance()->GetIP();
		ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dIp);
		if (pCtrl != NULL)
		{
			bool bRet = pCtrl->GetIpcSerialCfg(NULL, (u32)(CObserver*)this);
			if (!bRet)
			{
				return false;
			}
		}
	}
	return true;
}

