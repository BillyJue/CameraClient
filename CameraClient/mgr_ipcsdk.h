//ipcV7 SDK EPTZ����ģ��
#pragma once

#include "ipcdefine.h"
#include "ipcsdk_c.h"
#include "mgr_video.h"

class CMgrIpcSdk :public CCriticalObject
{
public:
	CMgrIpcSdk(void*);
	~CMgrIpcSdk();

	int   login();
	int   logout();

	void  ptzMove(const char*, int);
	void  stopMove();
	void  wiper();

	bool  usePrepos(u16, char*);
	bool  savePrepos(u16, char*);
	bool  delPrepos(u16, char*);

	int   GetParam(DWORD dwIP, WORD wPort, int nIndex);

	int   SystemUpdate(char *filepath, u32 len);

	// ��ȡϵͳ����״̬ 0���ڽ��� 1������� -1����ʧ��
	int   GetSystemUpdateState();

protected:
	//CMgrData*    m_pMgrData;
	bool		 m_bWiperEnable;
	u32          m_dwIpcHandle;              //��¼�豸�ľ��;
};