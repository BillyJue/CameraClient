//ipcV7 SDK EPTZ功能模块
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

	// 获取系统升级状态 0正在进行 1升级完成 -1升级失败
	int   GetSystemUpdateState();

protected:
	//CMgrData*    m_pMgrData;
	bool		 m_bWiperEnable;
	u32          m_dwIpcHandle;              //登录设备的句柄;
};