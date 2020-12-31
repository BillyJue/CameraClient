#include "stdafx.h"
#include "mgr_search.h"
#include "SearchSDK_c.h"
#include "define.h"
CMgrSearch::CMgrSearch(void* pData)
{
	//m_pMgrData = (CMgrData*)pData;
	m_nDevNum = 0;
	m_pObserver = NULL;
	m_bRun = true;
	Init();
}

CMgrSearch::~CMgrSearch()
{
	SetRun(false);
	ClearItem();
	Release();
}

void CMgrSearch::Init()
{
	IPCSEARCH_InitDll (MsgProCallBack, (DWORD)this);
}

void CMgrSearch::Release()
{
	IPCSEARCH_ReleaseDll();
}

void CMgrSearch::MsgProCallBack (UINT wMsg, LPVOID pParam, DWORD dwContext)
{
	CMgrSearch* pDlg = (CMgrSearch*)dwContext;

	if (pDlg->IsRun() == false)
	{
		return;
	}

	switch(wMsg)
	{
	case DEV_NC_TRACK_ACK_CAST:
		{
			TDevItem *ptDevItem = (TDevItem *)pParam;
			pDlg->AddItem(ptDevItem);
		}
		break;
	default:
		break;
	}
}

void CMgrSearch::AddItem(TDevItem *ptDevItem)
{
	bool isSupport;
	switch(ptDevItem->tDevInfo.dwBrdID)
	{
	case IPC626_AU_S_PID:
		isSupport = true;
		break;
	case IPC627_AU_C_PID:
		isSupport = true;
		break;
	case IPC625_BU_C_PID:
		isSupport = true;
		break;
	case IPC625_BU_E_PID:
		isSupport = true;
		break;
	case IPC655_BU_PID:
		isSupport = true;
		break;
	case IPC123_Ei8N:
		isSupport = true;
		break;
	case IPC2255_GN:
		isSupport = true;
		break;
	case IPC425_A030_N:
		isSupport = true;
		break;
	case IPC425_P323_N:
		isSupport = true;
		break;
	case IPC425_P323_NWH:
		isSupport = true;
		break;
	case IPC625_GN:
		isSupport = true;
		break;
	case IPC655_GN:
		isSupport = true;
		break;
	case IPC2255_Gi8N_PID:
		isSupport = true;
		break;
	case IPC123_HiBNM_PID:
		isSupport = true;
		break;
	case IPC675_GN:
		isSupport = true;
		break;
	case IPC695_GN:
		isSupport = true;
		break;
	case ZD5920_Gi8N_PID:
		isSupport = true;
		break;
	case IPC186_PCWS_PID:
		isSupport = true;
		break;
	case IPC625_HU_PID:
		isSupport = true;
		break;
	case IPC695_HU_PID:
		isSupport = true;
		break;
	case IPC695_HUH_PID:
		isSupport = true;
		break;
	case IPC695_GNS:
		isSupport = true;
		break;
	case IPC695_GN_S_HID:
		isSupport = true;
		break;
	default:
		isSupport = false;
		break;
	}

	if (IsInMap(ptDevItem) || isSupport == false)
	{
		return;
	}

	CCriticalLock lock(_pcs);

	TDevItem *ptItem = new TDevItem;
	memcpy(ptItem, ptDevItem, sizeof(TDevItem));
	m_mapDevInfo[m_nDevNum++] = ptItem;

	if (m_pObserver)
	{
		m_pObserver->OnObserverNotify(NMC_GET_DEV_INFO, (LPVOID)ptItem);
	}
}

bool CMgrSearch::IsInMap(TDevItem *ptDevItem)
{
	TDevItem *pItem;
	std::map<u32, TDevItem*>::iterator ite;

	CCriticalLock lock(_pcs);

	for(ite=m_mapDevInfo.begin(); ite!=m_mapDevInfo.end(); ++ite)
	{
		pItem = ite->second;
		if (ptDevItem->tDevInfo.tAddr.dwIpaddr == pItem->tDevInfo.tAddr.dwIpaddr)
		{
			return true;
		}
	}

	return false;
}

void CMgrSearch::ClearItem()
{
	CCriticalLock lock(_pcs);

	for (int i=0; i<m_nDevNum; i++)
	{
		delete (m_mapDevInfo[i]);
	}

	m_mapDevInfo.erase(m_mapDevInfo.begin(), m_mapDevInfo.end());
	m_nDevNum = 0;
}

TDevItem* CMgrSearch::GetItemById(u32 id)
{
	return m_mapDevInfo[id];
}

void CMgrSearch::SendDetectMsg(BOOL bSend)
{
	SendDevDetectMsg(bSend);
}

void CMgrSearch::RegisterCallback(CObserver* obj)
{
	m_pObserver = obj;
}

void CMgrSearch::SetRun(BOOL bRun)
{
	m_bRun = bRun;
	SendDetectMsg(false);
}

bool CMgrSearch::IsRun()
{
	return m_bRun;
}