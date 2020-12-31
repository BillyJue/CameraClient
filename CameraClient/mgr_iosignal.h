
#pragma once
#include "kosa_socket.h"
#include "kosa.h"
#include "iosignal_sdk.h"


class CMgrIoSignal
{
public:
	CMgrIoSignal(void* pData);
	~CMgrIoSignal();

	void  Init();

	void  Release();

	void  Reset();

	void  SetIP(u32 ip);

	int   Connect(HWND hwnd);

	int   Disconnect();

	void  IOSignalMsgPro(TPiNetIOData *pData);

private:
	//CMgrData*			m_pMgrData;
	u32                 m_nIP;
	u32                 m_handle;
	HWND                m_hwnd;
};