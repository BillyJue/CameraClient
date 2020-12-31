#include "stdafx.h"
#include "MgrData.h"
#include "define_err.h"
#include <QDir>
MgrData *MgrData::m_mgr = nullptr;

MgrData::MgrData():m_pMgrSearch(nullptr)
{
	init();
}



MgrData::~MgrData()
{
	SAFE_DELETE(m_pMgrSearch);
	SAFE_DELETE(m_pMgrAlarm);
	SAFE_DELETE(m_pMgrDev);
	SAFE_DELETE(m_pMgrEvtLst);
	SAFE_DELETE(m_pMgrIoSignal);
	SAFE_DELETE(m_pMgrIpcSdk);
	SAFE_DELETE(m_pMgrLocalCfg);
	SAFE_DELETE(m_pMgrMjpegCtrl);
	SAFE_DELETE(m_pMgrPort);
	SAFE_DELETE(m_pMgrSearch);
	SAFE_DELETE(m_pMgrVersion);
	SAFE_DELETE(m_pMgrVideo);

}

u16	MgrData::Login(DWORD dwDestIP, WORD wPort, QString strName, QString strPassword, u16& wErr)
{
	m_dwDestIP = dwDestIP;
	m_wPort = wPort;

	return Login(strName, strPassword, wErr);
}

Uint32 MgrData::IPstrToInt(QString ipStr)
{
	QStringList ipList = ipStr.split(".");
	Uint32 ipA = ipList[3].toUInt();
	Uint32 ipB = ipList[2].toUInt();
	Uint32 ipC = ipList[1].toUInt();
	Uint32 ipD = ipList[0].toUInt();

	Uint32 ipInt = (ipA << 24) + (ipB << 16) + (ipC << 8) + ipD;

	return ipInt;
}

QString MgrData::Int2IP(unsigned int ipInt)
{
	int tokenInt = 0;
	unsigned int leftValue = ipInt;
	QString str;
	for (int i = 0; i < 4; i++){
		int temp = pow(256, 3 - i);
		tokenInt = leftValue / temp;
		leftValue %= temp;
		str.append(QString("%1").arg(tokenInt));
		if (i != 3){
			str.append(".");
		}
	}
	QString sTemp;
	QStringList lis = str.split(".");
	if (lis.size() == 4)
	{
		sTemp.append(lis.at(3));
		sTemp.append(".");
		sTemp.append(lis.at(2));
		sTemp.append(".");
		sTemp.append(lis.at(1));
		sTemp.append(".");
		sTemp.append(lis.at(0));
	}

	return sTemp;
}


u16	MgrData::Login(QString strName, QString strPassword, u16& wErr)
{
	u16 nRet = m_mgrItsCtrl->AddCtrl(m_dwDestIP, 20000, strName, strPassword);
	if (nRet == ERR_NONE)
	{
		SetLogin(true);
	}
	else
	{
		m_mgrItsCtrl->DelCtrl(m_dwDestIP);
	}
	return nRet;
}

QString MgrData::GetUserPath()
{
	QDir tempDir;
	QString sPath = QDir::currentPath() + "/itsclient/";
	if (!tempDir.exists(sPath))
	{
		tempDir.mkpath(sPath);
	}
	return sPath;
}

void MgrData::SetLogin(bool b)
{
	if (m_bLogin == b)
	{
		return;
	}

	if (b)
	{
		m_bLogin = b;
		GetMgrMjpegCtrl()->SetIP(m_dwDestIP);
		GetMgrIoSignal()->SetIP(m_dwDestIP);
		GetMgrIpcSdk()->login();
		Notify(NULL, NMC_EVT_LOGIN, 0);
	}
	else
	{
		GetMgrVideo()->StopPlayVideo();
		GetMgrIpcSdk()->logout();
		GetMgrMjpegCtrl()->Reset();
		GetMgrEvtLst()->Clear();
		GetMgrEvtLst()->ClearSender();

		if (m_bManual == false)
		{
			m_bLogin = b;
			Notify(NMC_EVT_LOGOUT, NULL);
		}
	}
}

void MgrData::init()
{
	m_bManual = false;
	m_bLogin = false;
	m_pMgrSearch = new CMgrSearch(this);
	m_pMgrVersion = new CMgrVersion(u32(this));
	m_mgrUser = new CMgrUser(this);
	m_mgrItsCtrl = new MgrItsCtrl(this); 
	m_pMgrPort = new CMgrPort();
	m_pMgrLocalCfg = new CMgrLocalCfg(this);
	m_pMgrVideo = new CMgrVideo(this);
	m_pMgrEvtLst = new CMgrEvtLst(this);
	m_pMgrIoSignal = new CMgrIoSignal(this);
	m_pMgrIpcSdk = new CMgrIpcSdk(this);
	m_pMgrAlarm = new CMgrAlarm(this);
	m_pMgrDev = new CMgrDev(this);
	m_pMgrMjpegCtrl = new CMgrMjpegCtrl(this);
	memset(&m_tDevInfo, 0, sizeof(m_tDevInfo));
	m_tDevInfo.dwVender = 1;
}

void MgrData::SetDevInfo(TIPC_DevInfo *pDevInfo)
{
	memcpy(&m_tDevInfo, pDevInfo, sizeof(TIPC_DevInfo));

	if (m_tDevInfo.dwVender == 2)
	{
		TLocalCfgInfo tLocalCfgInfo;
		GetMgrLocalCfg()->GetLocalCfg(&tLocalCfgInfo);
		tLocalCfgInfo.ispic = 1;
		GetMgrLocalCfg()->SetLocalCfg(&tLocalCfgInfo, false);
	}

	if (strcmp(m_tDevInfo.szDevType, "KIPACS") == 0)
	{
		m_nType = EMDATATYPE_SERVER;
	}
}

void MgrData::SetDevCap(TIPC_GetUICapsRsp *pUICap)
{
	memcpy(&m_tUICap, pUICap, sizeof(TIPC_GetUICapsRsp));
}

MgrData *MgrData::getInstance()
{
	if (m_mgr == nullptr)
	{
		m_mgr = new MgrData();
	}

	return m_mgr;
}