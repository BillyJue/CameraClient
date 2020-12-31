#include "stdafx.h"
#include "MgrItsCtrl.h"
#include "define_err.h"
#include <QDebug>
static MgrData* m_mgrData = nullptr;
std::string GetLocalIpAddress()
{
	WORD wVersionRequested = MAKEWORD(2, 2);

	WSADATA wsaData;
	if (WSAStartup(wVersionRequested, &wsaData) != 0)
		return "";

	char local[255] = { 0 };
	gethostname(local, sizeof(local));
	hostent* ph = gethostbyname(local);
	if (ph == NULL)
		return "";

	in_addr addr;
	memcpy(&addr, ph->h_addr_list[0], sizeof(in_addr)); // 这里仅获取第一个ip  

	std::string localIP;
	localIP.assign(inet_ntoa(addr));

	WSACleanup();
	return localIP;
}


MgrItsCtrl::MgrItsCtrl(void *pData)
{
	m_mgrData = (MgrData*)pData;
	Init();
}


MgrItsCtrl::~MgrItsCtrl()
{
	Release();
}


void MgrItsCtrl::Init()
{
	if (!ITSCTRL::IsIvhsInit())
	{
		IvhsSetMsgCBFun(ItsCtrl::IvhsCallBack);
		Int32 nRet = IvhsStart(8);
		ITSCTRL::SetIsIvhsInit(nRet == 0);

		//u32 dwIp = inet_addr(GetLocalIpAddress().c_str());
		int n = 0;
		n++;
	}
}
void MgrItsCtrl::Release()
{
	CCriticalLock lock(_pcs);

	std::map<u32, ItsCtrl*>::iterator ite = m_mapItsCtrl.begin();
	while (ite != m_mapItsCtrl.end())
	{
		if (ite->second != NULL)
		{
			ite->second->Release();
			delete ite->second;
			ite->second = NULL;
		}
		ite++;
	}

	//IvhsStopListen();
	int nRet = IvhsStop();
	if (nRet == 0)
	{
		ITSCTRL::SetIsIvhsInit(false);
		qCritical()<<("XmlIsInitd ok");
	}
	else
	{
		ITSCTRL::SetIsIvhsInit(true);
		qCritical()<<("XmlIsInitd err");
	}
}
ItsCtrl* MgrItsCtrl::GetCtrl(u32 dwIP)
{
	if (dwIP == 0)
	{
		dwIP = inet_addr(GetLocalIpAddress().c_str());
	}

	CCriticalLock lock(_pcs);
	ItsCtrl* pItsCtrl = NULL;
	std::map<u32, ItsCtrl*>::iterator ite = m_mapItsCtrl.find(dwIP);
	if (ite != m_mapItsCtrl.end())
	{
		if (ite->second->IsConnect())
		{
			return ite->second;
		}
	}

	return NULL;
}



u32 MgrItsCtrl::AddCtrl(u32 dwIP, u16 wPort, QString strName, QString strPassword)
{
	CCriticalLock lock(_pcs);

	ItsCtrl* pItsCtrl = NULL;
	std::map<u32, ItsCtrl*>::iterator ite = m_mapItsCtrl.find(dwIP);
	if (ite != m_mapItsCtrl.end())
	{
		pItsCtrl = ite->second;
	}

	if (dwIP == 0)
	{
		dwIP = inet_addr(GetLocalIpAddress().c_str());
	}

	if (pItsCtrl == NULL)
	{
		pItsCtrl = new ItsCtrl(m_mgrData);
		m_mapItsCtrl[dwIP] = pItsCtrl;

		pItsCtrl->setParam(dwIP, wPort, strName, strPassword);
	}
	else
	{
		pItsCtrl->Disconnect();
		pItsCtrl->setParam(dwIP, wPort, strName, strPassword);

	}

	bool bRet = pItsCtrl->Connect();
	if (bRet == false)
	{
		return ERR_IPADDRERR;
	}

	bRet = pItsCtrl->Login();
	bRet = pItsCtrl->isLogin();
	if (bRet == false)
	{
		return ERR_USERORPASSWORD;
	}

	return ERR_NONE;
}
bool MgrItsCtrl::DelCtrl(u32 dwIP)
{
	CCriticalLock lock(_pcs);
	std::map<u32, ItsCtrl*>::iterator ite = m_mapItsCtrl.find(dwIP);
	if (ite != m_mapItsCtrl.end())
	{
		ItsCtrl* pItsCtrl = ite->second;
		m_mapItsCtrl.erase(ite);

		if (pItsCtrl->Disconnect())
		{
			pItsCtrl->Release();
			delete pItsCtrl;
			pItsCtrl = NULL;

			return true;
		}
	}

	return false;
}
void MgrItsCtrl::DelAll()
{

}

void MgrItsCtrl::StopAll()
{
	std::map<u32, ItsCtrl*>::iterator ite = m_mapItsCtrl.begin();
	while (ite != m_mapItsCtrl.end())
	{
		if (ite->second != NULL)
		{
			ite->second->Release();
			delete ite->second;
			ite->second = NULL;
		}
		ite++;
	}
}
void MgrItsCtrl::StopReceive()
{
	std::map<u32, ItsCtrl*>::iterator ite = m_mapItsCtrl.begin();
	while (ite != m_mapItsCtrl.end())
	{
		if (ite->second != NULL)
		{
			ite->second->StopReceive();
		}
		ite++;
	}
}
void MgrItsCtrl::StartReceive()
{
	std::map<u32, ItsCtrl*>::iterator ite = m_mapItsCtrl.begin();
	while (ite != m_mapItsCtrl.end())
	{
		if (ite->second != NULL)
		{
			ite->second->StartReceive();
		}
		ite++;
	}
}
void MgrItsCtrl::DebugMode()
{
	std::map<u32, ItsCtrl*>::iterator ite = m_mapItsCtrl.begin();
	while (ite != m_mapItsCtrl.end())
	{
		if (ite->second != NULL)
		{
			ite->second->DebugMode();
		}
		ite++;
	}
}