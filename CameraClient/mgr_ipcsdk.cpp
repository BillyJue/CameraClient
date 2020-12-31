#include "stdafx.h"
#include "mgr_ipcsdk.h"
#include <QDebug>
MgrData *m_mgrData8 = nullptr;
CMgrIpcSdk::CMgrIpcSdk(void* pData)
{
	m_mgrData8 = (MgrData*)pData;
	m_bWiperEnable = false;
	m_dwIpcHandle = 0;

	IPC_InitDll();
}

CMgrIpcSdk::~CMgrIpcSdk()
{
	u32 errorCode;
	IPC_ReleaseDll(&errorCode);
}

int CMgrIpcSdk::login()
{
	if (m_dwIpcHandle != 0)
	{
		return 0;
	}

	wchar_t* tempUser = m_mgrData8->GetMgrUser()->GetCurUser()->szName;
	wchar_t* tempPassword = m_mgrData8->GetMgrUser()->GetCurUser()->szPassword;
	unsigned short wPort = 80;

	char userName[64];
	char password[64];
	wchar2char(userName, tempUser);
	wchar2char(password, tempPassword);

	m_dwIpcHandle = IPC_CreateHandle(ntohl(m_mgrData8->GetIP()), wPort, userName, password);

	u32 nError = RET_SUCCESS;
	BOOL32 bRet = IPC_Login(&m_dwIpcHandle, userName, password, &nError);
	if (bRet != 1)
	{
		qCritical() << ("ipc %d, %s, %s,login fail!\n", m_mgrData8->GetIP(), userName, password);
		IPC_DestroyHandle();
		return 0;
	}

	m_mgrData8->GetMgrVideo()->SetIpcHandle(m_dwIpcHandle);
}

int CMgrIpcSdk::logout()
{
	u32	pError = RET_SUCCESS;

	if (m_dwIpcHandle == 0)
	{
		return 0;
	}

	BOOL32 bRet = IPC_Logout(&m_dwIpcHandle, &pError);
	if (bRet != 1)
	{
	}

	IPC_DestroyHandle();

	m_dwIpcHandle = 0;

	return 0;
}

void CMgrIpcSdk::ptzMove(const char* type, int speed)
{
	TPTZMOVE* pPtz = new TPTZMOVE;
	memset(pPtz, 0, sizeof(TPTZMOVE));
	strcpy(pPtz->szType, type);
	pPtz->nSpeed = speed;
	u32 nError = RET_SUCCESS;
	BOOL32 bRet = IPC_PtzMove(&m_dwIpcHandle, pPtz, sizeof(TPTZMOVE), &nError);
	if (bRet != RET_SUCCESS)
	{

	}

	delete pPtz;
}

void CMgrIpcSdk::stopMove()
{
	TPTZMOVE* pPtz = new TPTZMOVE;
	memset(pPtz, 0, sizeof(TPTZMOVE));
	strcpy(pPtz->szType, "movestop");
	u32 nError = RET_SUCCESS;
	BOOL32 bRet = IPC_PtzMove(&m_dwIpcHandle, pPtz, sizeof(TPTZMOVE), &nError);
	if (bRet != RET_SUCCESS)
	{

	}
	delete pPtz;
}

void CMgrIpcSdk::wiper()
{
	TPTZWIPER* pPtz = new TPTZWIPER;
	memset(pPtz, 0, sizeof(TPTZWIPER));

	if (m_bWiperEnable)
	{
		pPtz->bEnable = false;
		m_bWiperEnable = false;
	}
	else
	{
		pPtz->bEnable = true;
		m_bWiperEnable = true;
	}

	u32 nError = RET_SUCCESS;
	BOOL32 bRet = IPC_SetPtzWiper(&m_dwIpcHandle, pPtz, sizeof(TPTZWIPER), &nError);
	if (bRet != RET_SUCCESS)
	{

	}
	delete pPtz;
}

bool CMgrIpcSdk::savePrepos(u16 nIndex, char* szPreName)
{
	TPTZPRESET *pParam = new TPTZPRESET;
	memset(pParam, 0, sizeof(TPTZPRESET));

	strcpy(pParam->szEvtType, "preset_set");
	pParam->wSn = nIndex + 1;

	strcpy(pParam->szName, szPreName);

	u32 nError = RET_SUCCESS;
	BOOL32 bRet = IPC_SetPtzPreset(&m_dwIpcHandle, pParam, sizeof(TPTZPRESET), &nError);
	if (bRet != RET_SUCCESS)
	{
		return false;
	}
	else
	{
		delete pParam;
		return true;
	}
}

bool CMgrIpcSdk::usePrepos(u16 nIndex, char* szPreName)
{
	TPTZPRESET *pParam = new TPTZPRESET;
	memset(pParam, 0, sizeof(TPTZPRESET));

	strcpy(pParam->szEvtType, "preset_load");
	pParam->wSn = nIndex + 1;
	strcpy(pParam->szName, szPreName);

	u32 nError = RET_SUCCESS;
	BOOL32 bRet = IPC_SetPtzPreset(&m_dwIpcHandle, pParam, sizeof(TPTZPRESET), &nError);
	if (bRet != RET_SUCCESS)
	{
		return false;
	}
	else
	{
		delete pParam;
		return true;
	}
}

bool CMgrIpcSdk::delPrepos(u16 nIndex, char* szPreName)
{
	TPTZPRESET *pParam = new TPTZPRESET;
	memset(pParam, 0, sizeof(TPTZPRESET));

	strcpy(pParam->szEvtType, "preset_remove");
	pParam->wSn = nIndex + 1;
	strcpy(pParam->szName, szPreName);

	u32 nError = RET_SUCCESS;
	BOOL32 bRet = IPC_SetPtzPreset(&m_dwIpcHandle, pParam, sizeof(TPTZPRESET), &nError);
	if (bRet != RET_SUCCESS)
	{
		return false;
	}
	else
	{
		delete pParam;
		return true;
	}
}

int  CMgrIpcSdk::GetParam(DWORD dwIP, WORD wPort, int nIndex)
{
	TPTZPRESET	 *pParam = new TPTZPRESET;
	memset(pParam, 0, sizeof(TPTZPRESET));

	u32 nError = RET_SUCCESS;
	BOOL32 bRet = IPC_GetPtzPreset(&m_dwIpcHandle, nIndex + 1, pParam, sizeof(TPTZPRESET), &nError);
	if (bRet != RET_SUCCESS)
	{
		return -1;
	}
	else
	{
		if (pParam->bEnable == 0)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}

}

int CMgrIpcSdk::SystemUpdate(char *filepath, u32 len)
{
	u32 errorCode = 0;
	TUPLOADINFO  info;
	memset(info.szFilePath, 0, 260);
	memcpy(info.szFilePath, filepath, strlen(filepath) + 1);
	info.nLenFile = len;
	info.nMaxLen_Send = 17408;
	BOOL32 nRet = IPC_SysUpload(&m_dwIpcHandle, (void *)&info, 0, &errorCode);
	return nRet;
}

int CMgrIpcSdk::GetSystemUpdateState()
{
	u32 errorCode = 0;
	TUPLOADSTATE state;
	memset(&state, 0, sizeof(state));
	BOOL32 nRet = IPC_SysUploadState(&m_dwIpcHandle, (void *)&state, 0, &errorCode);
	if (strcmp(state.szState, "idel") == 0)
	{
		return -1;
	}
	else if (state.nProgress == 100)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}