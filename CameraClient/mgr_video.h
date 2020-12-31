//ipcV7 SDK 视频播放的封装;

#pragma once
#include "stdafx.h"
#include "ipcdefine.h"
#include "ipcsdk_c.h"


typedef void(__stdcall *VIDEO_MSG_CB)(unsigned long context, unsigned int msg, unsigned int param);

class CMgrVideo :public CCriticalObject
{
public:
	CMgrVideo(void* pData);
	~CMgrVideo();

	void IpcInit();
	void IpcRelease();

	void SetIpcHandle(u32 handle);
	void PlayVideo(unsigned long hPlayWnd = 0);
	void StopPlayVideo(unsigned long hPlayWnd = 0);
	void RequestIpcVideo();
	void StopReqIpcVideo();
	void StartLocalPlayVideo(unsigned long hPlayWnd = 0);
	void StopLocalPlayVideo(unsigned long hPlayWnd = 0);
	void PauseLocalPlayVideo();
	void ResumeLocalPlayVideo();
	void SwitchPlayWnd(unsigned long hPlayWnd);
	void SetPlayWnd(unsigned long hPlayWnd);

	void RegisterDrawCB(MEDIA_DRAW_CB fDrawCB, unsigned long dwContext);
	static void __stdcall DetectMultiConnect(u32 dwIP, u16 wPort, u32 dwHandle, u32 dwCBconnectType, u32 dwDataLen, u32 dwData, void* pContext);

	static void __stdcall MediaConnectLostCB(PLAYERHANDLE hPlayHandle, void *pContext);
	int RegisterMsgCB(VIDEO_MSG_CB cb, unsigned long context);

	void StartRecVideo(const char* pchFilePath);
	void StopRecVideo();
	static void __stdcall RecordStateCallBack(PLAYERHANDLE hPlayHandle, u32 dwRecorderID, u32 dwCBType, void* pData, int nDataLen, void* pContext);


protected:
	//MgrData*				m_pMgrData;                 //这个里面存放了ip等数据;
	BOOL				    m_bIsInit;					//是否初始化;
	TLocalCfgInfo           m_tLocalCfgInfo;
	char					m_userName[64];
	char					m_password[64];

	u32					    dwPlayId;				    //(playId) ipcsdk
	u32					    m_dwPlayerID;               // Player Handle(ipcmedia)
	WORD				    wStreamChn;					// first way: mainstream  second way: secondary flow主流辅流
	WORD                    wVideoPort;
	WORD					wAudioPort;
	WORD					wVideoBackRtcp;
	WORD					wAudioBackRtcp;

	VIDEO_MSG_CB			m_cb;
	unsigned long			m_context;

public:
	BOOL                    m_bVideoRecord;
	BOOL				    m_bVideoRequest;			//当前是否请求码流
	int                     m_nVideoPlayStatus;
	unsigned long			m_hWndPlayed;				//播放视频的窗口句柄;
	u32					    m_dwIpcHandle;              //登录设备的句柄;
	CCriticalObject		    m_lock;
};