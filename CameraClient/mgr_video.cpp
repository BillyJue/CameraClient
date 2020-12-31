
#include "stdafx.h"
#include "mgr_video.h"
#include <QDebug>
MgrData *m_pMgrData4 = nullptr;
enum{
	VIDEO_STOP = 0,
	VIDEO_PLAY,
	VIDEO_PAUSE,
}EN_VIDEO_STATUS;

bool wchar2charTemp(char* psz, const wchar_t* pwsz)
{
	int len = WideCharToMultiByte(CP_ACP, 0, pwsz, -1, NULL, 0, NULL, NULL);
	if (!psz)
	{
		return false;
	}
	memset(psz, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, pwsz, -1, psz, len, NULL, NULL);

	return true;
}

CMgrVideo::CMgrVideo(void* pData)
{

	m_hWndPlayed = NULL;
	m_pMgrData4 = (MgrData*)pData;
	m_dwIpcHandle = 0;
	m_bIsInit = FALSE;
	m_bVideoRecord = FALSE;
	m_bVideoRequest = FALSE;
	m_nVideoPlayStatus = VIDEO_STOP;
	dwPlayId = 0;
	m_dwPlayerID = 0;
	wStreamChn = 1;
	wVideoPort = 0;
	wAudioPort = 0;
	wVideoBackRtcp = 0;
	wAudioBackRtcp = 0;

	IpcInit();
}

CMgrVideo::~CMgrVideo()
{
	IpcRelease();
}


void CMgrVideo::IpcInit()
{
	u32 pError = RET_SUCCESS;
	BOOL bOpenTelnet = 0;
#ifdef _DEBUG
	//bOpenTelnet = 1;
#endif
	BOOL32 bRet;

	bRet = IPC_InitDll("ipcmedia.dll", 3000, bOpenTelnet, &pError);
	if (bRet != 1)
	{
		return;
	}

	m_bIsInit = TRUE;   // 是否初始化;
}

void CMgrVideo::IpcRelease()
{
	if (m_bIsInit == FALSE)
	{
		return;
	}

	m_bIsInit = FALSE;

	StopPlayVideo();
}

void CMgrVideo::SetIpcHandle(u32 handle)
{
	m_dwIpcHandle = handle;
}

//断链检测;
void CMgrVideo::DetectMultiConnect(u32 dwIP, u16 wPort, u32 dwHandle, u32 dwCBconnectType, u32 dwDataLen, u32 dwData, void* pContext)
{
	CMgrVideo *pMgrVideo = (CMgrVideo *)pContext;

	if (pMgrVideo->m_dwIpcHandle != dwHandle)
	{
		return;
	}

	//if (dwCBconnectType == em_connect_no)
	//{
	//	pMgrVideo->StopPlayVideo();
	//}
	//else if (dwCBconnectType == em_connect_ok)
	//{
	//	pMgrVideo->PlayVideo();
	//}
}

void CMgrVideo::MediaConnectLostCB(PLAYERHANDLE hPlayHandle, void *pContext)
{
	CMgrVideo *pMgrVideo = (CMgrVideo *)pContext;

	if (pMgrVideo->m_cb)
	{
		pMgrVideo->m_cb(pMgrVideo->m_context, 0, 0);
	}
}

void CMgrVideo::PlayVideo(unsigned long hPlayWnd)
{
	if (hPlayWnd == 0 && m_hWndPlayed == 0)
	{
		return;
	}

	wchar_t* tempUser = m_pMgrData4->GetMgrUser()->GetCurUser()->szName;
	wchar_t* tempPassword = m_pMgrData4->GetMgrUser()->GetCurUser()->szPassword;
	wchar2charTemp(m_userName, tempUser);
	wchar2charTemp(m_password, tempPassword);

	RequestIpcVideo();

	StartLocalPlayVideo(hPlayWnd);
}

void CMgrVideo::StopPlayVideo(unsigned long hPlayWnd)
{
	if (hPlayWnd != 0 && hPlayWnd != m_hWndPlayed)
	{
		return;
	}

	StopRecVideo();
	StopLocalPlayVideo(hPlayWnd);
	StopReqIpcVideo();
}

void CMgrVideo::RequestIpcVideo()
{
	if (m_bVideoRequest)
	{
		return;
	}
	m_bVideoRequest = TRUE;

	m_pMgrData4->GetMgrLocalCfg()->GetLocalCfg(&m_tLocalCfgInfo);

	TPLAYVIDEOPARAM tPlayVideoParam;
	memset(&tPlayVideoParam, 0, sizeof(TPLAYVIDEOPARAM));

	TPLAYVIDEOINFO tPlayVideoInfo;
	memset(&tPlayVideoInfo, 0, sizeof(TPLAYVIDEOINFO));

	// Calculate to generate the local free port
	DWORD dwLocalIp = 0;
	WORD wAudioBackRtcp2 = 0;
	bool bDoubleAudio = FALSE;
	u32 tempIP = m_pMgrData4->GetIP();

	MED_GetVideoPlayPort(wVideoPort, wAudioPort, dwLocalIp, ntohl(m_pMgrData4->GetIP()), 80, wStreamChn - 1);

	int nRet = 0;
	u32 pError = RET_SUCCESS;
	int nLenInfo = 0;
	int bMetaAlarm = FALSE;

	if (m_tLocalCfgInfo.istcp == 0)
	{
		//  申请UDP码流;
		tPlayVideoParam.byVideoSouce = 1;
		tPlayVideoParam.byStreamChn = wStreamChn;
		tPlayVideoParam.dwSrcIP = dwLocalIp;
		strcpy(tPlayVideoParam.szStreamType, "both");
		strcpy(tPlayVideoParam.szTransType, "rtp_udp");
		tPlayVideoParam.wRtpVideoPort = wVideoPort;
		tPlayVideoParam.wRtpAudioPort = wAudioPort;
		tPlayVideoParam.wRtpAudioPort2 = 0;
		tPlayVideoParam.wRtcpAudioPort2 = 0;
		tPlayVideoParam.wRtcpVideoPort = wVideoPort + 1;
		tPlayVideoParam.wRtcpAudioPort = wAudioPort + 1;

		tPlayVideoParam.hPlayWnd = m_hWndPlayed;
		dwPlayId = IPC_StartRealPlay(&m_dwIpcHandle, type_udp, &tPlayVideoParam, sizeof(TPLAYVIDEOPARAM), &tPlayVideoInfo, nLenInfo, &pError);
		if (pError != RET_SUCCESS)
		{
			qCritical()<<("IPC_StartRealPlay udp %d fail!\n", pError);
			return;
		}

		// UDP stream to use insurance
		wVideoBackRtcp = tPlayVideoInfo.wRtcpVideoPort;
		wAudioBackRtcp = tPlayVideoInfo.wRtcpAudioPort;
	}
	else
	{
		TRTSPPARAM tRtspParam;
		memset(&tRtspParam, 0, sizeof(TRTSPPARAM));
		tRtspParam.byVideoSource = 1;
		tRtspParam.wVideoChanID = wStreamChn;

		TRTSPINFO tRtspInfo;
		memset(&tRtspInfo, 0, sizeof(TRTSPINFO));
		int nLen = 0;

		//********* 申请tcp码流，不申请告警************************/
		BOOL32 bRet = IPC_GetVideoInfo(&m_dwIpcHandle, type_tcp, &tRtspParam, sizeof(TRTSPPARAM), &tRtspInfo, nLen, &pError);
		if (bRet != 1)
		{
			qCritical()<<("IPC_GetVideoInfo %d fail!\n", pError);
			return;
		}
		bDoubleAudio = tRtspInfo.bDoubleAudio;

		tPlayVideoParam.byStreamChn = wStreamChn - 1;
		tPlayVideoParam.dwSrcIP = dwLocalIp;
		strcpy(tPlayVideoParam.szAdmin, m_userName);
		strcpy(tPlayVideoParam.szPassword, m_password);
		tPlayVideoParam.wRtspPort = tRtspInfo.wRtspPort;
		tPlayVideoParam.wRtpAudioPort = wAudioPort;
		tPlayVideoParam.wRtcpAudioPort = wAudioPort + 1;
		tPlayVideoParam.wRtpAudioPort2 = 0;
		tPlayVideoParam.wRtcpAudioPort2 = 0;
		tPlayVideoParam.wRtpVideoPort = wVideoPort;
		tPlayVideoParam.wRtcpVideoPort = wVideoPort + 1;
		strcpy(tPlayVideoParam.szMediaURL, tRtspInfo.szurl);

		dwPlayId = IPC_StartRealPlay(&m_dwIpcHandle, type_tcp, &tPlayVideoParam, sizeof(TPLAYVIDEOPARAM), &tPlayVideoInfo, nLenInfo, &pError);
		if (pError != RET_SUCCESS)
		{
			qCritical() << ("IPC_StartRealPlay tcp %d fail!\n", pError);
			return;
		}

		Sleep(1000);
	}
}

void CMgrVideo::StopReqIpcVideo()
{
	if (m_bVideoRequest == FALSE)
	{
		return;
	}
	m_bVideoRequest = FALSE;

	u32 pError = RET_SUCCESS;
	BOOL32 bRet;
	if (m_tLocalCfgInfo.istcp == 0)
	{
		bRet = IPC_StopRealPlay(&m_dwIpcHandle, type_udp, dwPlayId, &pError);
	}
	else
	{
		bRet = IPC_StopRealPlay(&m_dwIpcHandle, type_tcp, dwPlayId, &pError);
	}

	if (bRet != true)
	{
	}
}

void CMgrVideo::StartLocalPlayVideo(unsigned long hPlayWnd)
{
	if (m_bVideoRequest == FALSE)
	{
		return;
	}

	if (m_nVideoPlayStatus == VIDEO_PLAY)
	{
		if (hPlayWnd == m_hWndPlayed)
		{
			return;
		}
		else
		{
			SwitchPlayWnd(hPlayWnd);
			return;
		}
	}
	else if (m_nVideoPlayStatus == VIDEO_PAUSE)
	{
		if (hPlayWnd == m_hWndPlayed)
		{
			ResumeLocalPlayVideo();
			return;
		}
		else
		{
			SwitchPlayWnd(hPlayWnd);
			return;
		}
	}

	if (hPlayWnd != 0)
	{
		m_hWndPlayed = hPlayWnd;
	}

	if (m_tLocalCfgInfo.istcp == 0)
	{
		m_dwPlayerID = MED_LocalPlay((unsigned long)m_hWndPlayed, ntohl(m_pMgrData4->GetIP()),
			wStreamChn - 1, wVideoPort, wAudioPort, wVideoBackRtcp, wAudioBackRtcp, 0,
			FALSE, FALSE, type_udp, TRUE);
		if (0 == m_dwPlayerID)
		{
			qCritical() << ("MED_LocalPlay fail!\n");
			return;
		}

		MED_SetConnectLostCallBack(m_dwPlayerID, 40, MediaConnectLostCB, (void*)this);
		u32 pError = RET_SUCCESS;
		IPC_SendKeyFrame(&m_dwIpcHandle, dwPlayId, &pError);
	}
	else
	{
		m_dwPlayerID = MED_LocalPlay((unsigned long)m_hWndPlayed, ntohl(m_pMgrData4->GetIP()),
			wStreamChn - 1, wVideoPort, wAudioPort, wVideoBackRtcp, wAudioBackRtcp, 0,
			FALSE, FALSE, type_tcp, FALSE);
		if (0 == m_dwPlayerID)
		{
			qCritical() << ("MED_LocalPlay fail!\n");
			return;
		}
	}

	m_nVideoPlayStatus = VIDEO_PLAY;
}

void CMgrVideo::StopLocalPlayVideo(unsigned long hPlayWnd)
{
	if (m_nVideoPlayStatus == VIDEO_STOP)
	{
		return;
	}
	else if (hPlayWnd != 0 && m_hWndPlayed != hPlayWnd)
	{
		return;
	}

	m_nVideoPlayStatus = VIDEO_STOP;

	MEDIA_FRAME_CB fFrameCB = NULL;
	MED_SetFrameCallBack(m_dwPlayerID, NULL, NULL);

	MEDIA_ALARM_CB fAlarmCB = NULL;
	MED_SetAlarmCallBack(m_dwPlayerID, NULL, NULL);

	MEDIA_FRAMEINFO_CB fFrameInfoCB = NULL;
	MED_SetFrameInfoCallBack(m_dwPlayerID, NULL, NULL);

	MED_StopLocalPlay(m_dwPlayerID, 1);
	m_dwPlayerID = 0;
}

void CMgrVideo::PauseLocalPlayVideo()
{
	if (m_nVideoPlayStatus == VIDEO_STOP)
	{
		return;
	}

	m_nVideoPlayStatus = VIDEO_PAUSE;
	MED_PauseLocalPlay(m_dwPlayerID);
}

void CMgrVideo::ResumeLocalPlayVideo()
{
	if (m_nVideoPlayStatus == VIDEO_STOP)
	{
		return;
	}

	m_nVideoPlayStatus = VIDEO_PLAY;
	MED_ResumeLocalPlay(m_dwPlayerID);
}

void CMgrVideo::SwitchPlayWnd(unsigned long hPlayWnd)
{
	if (m_nVideoPlayStatus == VIDEO_STOP)
	{
		return;
	}

	if (hPlayWnd == 0)
	{
		return;
	}

	if (m_nVideoPlayStatus == VIDEO_PAUSE)
	{
		ResumeLocalPlayVideo();
	}

	m_hWndPlayed = hPlayWnd;

	MED_ChangePlayWnd(m_dwPlayerID, m_hWndPlayed);
}

void CMgrVideo::SetPlayWnd(unsigned long hPlayWnd)
{
	m_hWndPlayed = hPlayWnd;
}


void CMgrVideo::StartRecVideo(const char* pchFilePath)   //开始本地录制;
{
	if (m_bVideoRecord == TRUE)
	{
		return;
	}
	m_bVideoRecord = TRUE;

	if (MED_IsLocalRecording(m_dwPlayerID))
	{
		return;
	}

	SYSTEMTIME st;
	memset(&st, 0, sizeof(st));
	GetLocalTime(&st);
	DWORD dwCurTime = ::GetTickCount();
	char szFileName[MAX_PATH] = { 0 };
	in_addr inaddr;
	inaddr.s_addr = m_pMgrData4->GetIP();
	sprintf(szFileName, "%s_%.4d%.2d%.2d_%.2d%.2d%.2d_%.2d.asf", inet_ntoa(inaddr),
		st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, dwCurTime % 100);

	DWORD dwCutRecFileSize = 0;

	int nRet = MED_StartLocalRec(m_dwPlayerID, szFileName, pchFilePath, dwCutRecFileSize, (MEDIA_REC_STATE_CB)RecordStateCallBack, this);
	if (nRet != ERR_SUCCESS)
	{
		return;
	}
}

void CMgrVideo::StopRecVideo()
{
	if (m_bVideoRecord == FALSE)
	{
		return;
	}
	m_bVideoRecord = FALSE;

	if (MED_IsLocalRecording(m_dwPlayerID))
	{
		MED_StopLocalRec(m_dwPlayerID);
	}
}

void CMgrVideo::RecordStateCallBack(PLAYERHANDLE hPlayHandle, u32 dwRecorderID,
	u32 dwCBType, void* pData, int nDataLen, void* pContext)
{
	CMgrVideo* pThis = (CMgrVideo*)pContext;
	if (MEDREC_STOP == dwCBType)
	{
		TIPC_REC_STOP_INFO* ptRecStopInfo = (TIPC_REC_STOP_INFO*)pData;
		switch (ptRecStopInfo->m_wStopReason)
		{
		case RECSTOPREASON_FORMAT_CHG:
		{
		}
		break;
		case RECSTOPREASON_FILE_FAIL:
		case RECSTOPREASON_DISK_WRITE_FULL:
			pThis->StopRecVideo();
			break;
		default:
			break;
		}
	}
}

void CMgrVideo::RegisterDrawCB(MEDIA_DRAW_CB fDrawCB, unsigned long dwContext)
{
	MED_SetDrawCallBack(m_dwPlayerID, fDrawCB, dwContext);
}

int CMgrVideo::RegisterMsgCB(VIDEO_MSG_CB cb, unsigned long context)
{
	m_cb = cb;
	m_context = context;
	return 0;
}
