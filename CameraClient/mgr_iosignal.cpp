
#include "stdafx.h"
#include "mgr_iosignal.h"

MgrData *m_mgrData7 = nullptr;
void IOSignalMsgCB(Uint32 dwHandle, Uint32 dwMsgType, char *pBuf, Uint32 dwBufSize, Uint32 dwContext)
{
	CMgrIoSignal *pCtrl;
	pCtrl = (CMgrIoSignal*)dwContext;
	if (pCtrl == NULL)
	{
		return;
	}

	switch (dwMsgType)
	{
	case NEW_IO_SIGNAL_MSG:
		pCtrl->IOSignalMsgPro((TPiNetIOData*)pBuf);
		break;
	case NOTIFY_TYPE_DISCONNECT:
		pCtrl->Disconnect();
		break;
	default:
		break;
	}

}

CMgrIoSignal::CMgrIoSignal(void* pData)
{
	m_mgrData7 = (MgrData*)pData;
	m_nIP = 0;
	m_hwnd = 0;
	m_handle = 0;

	Init();
}

CMgrIoSignal::~CMgrIoSignal()
{
	Release();
}

void  CMgrIoSignal::Init()
{
	IOSignalInit(1);
}

void  CMgrIoSignal::Release()
{
	IOSignalUnInit();
}

void  CMgrIoSignal::Reset()
{

}

void  CMgrIoSignal::SetIP(u32 ip)
{
	m_nIP = ip;
}

int   CMgrIoSignal::Connect(HWND hwnd)
{
	m_hwnd = hwnd;
	m_handle = IOSignalConnect(m_nIP, 0);
	if (m_handle != 0)
	{
		IOSignalRegisterCBFun(m_handle, IOSignalMsgCB, (Uint32)this);
	}
	return 0;
}

int   CMgrIoSignal::Disconnect()
{
	IOSignalClose(m_handle);
	m_handle = 0;

	// 向界面发送消息
	::PostMessage(m_hwnd, NMC_EVT_IO_SIGNAL_DISCONNECT, 0, 0);

	return 0;
}

void  CMgrIoSignal::IOSignalMsgPro(TPiNetIOData *pData)
{
	// 向界面发送消息
	::PostMessage(m_hwnd, NMC_EVT_IO_SIGNAL, pData->dwIOState, 0);
}