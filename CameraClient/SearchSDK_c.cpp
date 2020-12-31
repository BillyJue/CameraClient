#include "SearchSDK_c.h"
#include <WinBase.h>
HINSTANCE m_hSDKDllSearch	= NULL;

#define PARAM 
#define PARAM1(p1)  p1
#define PARAM2(p1, p2) p1, p2
#define PARAM3(p1, p2, p3)  p1, p2, p3
#define PARAM4(p1, p2, p3, p4) p1, p2, p3, p4
#define PARAM5(p1, p2, p3, p4, p5) p1, p2, p3, p4, p5
#define PARAM6(p1, p2, p3, p4, p5, p6) p1, p2, p3, p4, p5, p6
#define PARAM7(p1, p2, p3, p4, p5, p6, p7) p1, p2, p3, p4, p5, p6, p7
#define PARAM8(p1, p2, p3, p4, p5, p6, p7, p8) p1, p2, p3, p4, p5, p6, p7, p8
#define PARAM9(p1, p2, p3, p4, p5, p6, p7, p8, p9) p1, p2, p3, p4, p5, p6, p7, p8, p9
#define PARAM10(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) p1, p2, p3, p4, p5, p6, p7, p8, p9, p10
#define PARAM11(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11) p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11
#define PARAM12(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12) p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12
#define PARAM13(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13)  p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13

#define LOADDLLFUN(fun, funname, prm)  						       \
{										 							\
	fun pFun = NULL;					 							\
	if (m_hSDKDllSearch != NULL)				 							\
	{																\
		pFun = (fun)::GetProcAddress(m_hSDKDllSearch, funname);			\
	}																\
	if (pFun == NULL)												\
		return FALSE;												\
																	\
	if(!pFun(prm))													\
		return FALSE;												\
}																	\

SEARCH_API BOOL IPCSEARCH_InitDll (MsgAckCB mCB,  DWORD dwContext)
{
	char szFilePath[MAX_PATH];
	GetModuleFileNameA(NULL, szFilePath, MAX_PATH);
	char* lpszEnd = strrchr(szFilePath, '\\');
	*(lpszEnd+1) = 0;
	strcat_s(szFilePath, "SearchSDK.dll");
	
	m_hSDKDllSearch = ::LoadLibraryA(szFilePath);
	if (NULL == m_hSDKDllSearch)
	{
		return FALSE;
	}

	typedef BOOL (*_pFun1) ();
	LOADDLLFUN(_pFun1, "InitSearchSDK", PARAM);	

	typedef BOOL (*_pFun2)(MsgAckCB, DWORD);
	LOADDLLFUN(_pFun2, "SetMsgAckCB", PARAM2(mCB, dwContext));

	return TRUE;
}

SEARCH_API BOOL IPCSEARCH_ReleaseDll ()
{
// 	typedef BOOL (*_pFun1)(MsgAckCB, DWORD);
// 	LOADDLLFUN(_pFun1, "SetMsgAckCB", PARAM2(NULL, 0));

	if (m_hSDKDllSearch != NULL)
	{
		typedef BOOL (*_pFun2)();
		LOADDLLFUN(_pFun2, "ExitSearchSDK", PARAM);

		if (m_hSDKDllSearch != NULL)
		{
			if(::FreeLibrary(m_hSDKDllSearch) != 0)
			{	
				m_hSDKDllSearch = NULL;
			}
		}	
	}

	return TRUE;
}

SEARCH_API BOOL GetLocalIP (DWORD &dwLocalIP, DWORD dwDestIP, DWORD dwDestIPMask)
{
	typedef BOOL (*_pFun)(DWORD &, DWORD, DWORD);
	LOADDLLFUN(_pFun, "GetLocalIP", PARAM3(dwLocalIP, dwDestIP, dwDestIPMask));

	return TRUE;
}

SEARCH_API BOOL SendDevDetectMsg (BOOL bRefreshAdapter)
{
	typedef BOOL (*_pFun)(DWORD);
	LOADDLLFUN(_pFun, "SendDevDetectMsg", PARAM(bRefreshAdapter));

	return TRUE;
}

SEARCH_API BOOL SendDevDetectUniMsg (BOOL bRefreshAdapter, DWORD dwDstIP)
{
	typedef BOOL (*_pFun)(BOOL, DWORD);
	LOADDLLFUN(_pFun, "SendDevDetectUniMsg", PARAM2(bRefreshAdapter, dwDstIP));

	return TRUE;
}

SEARCH_API BOOL SendRebootMsg (u32 dwDstIP, u8* pDstMacAddr, u8* abySecureCode, char* szUser, char* szPwd)
{
	typedef BOOL (*_pFun)(u32, u8*, u8*, char*, char*);
	LOADDLLFUN(_pFun, "SendRebootMsg", PARAM5(dwDstIP, pDstMacAddr, abySecureCode, szUser, szPwd));

	return TRUE;
}

SEARCH_API BOOL SendRebootMsgS (TDevItem* ptDevItem, char* szUser, char* szPwd)
{
	typedef BOOL (*_pFun)(TDevItem*, char*, char*);
	LOADDLLFUN(_pFun, "SendRebootMsgS", PARAM3(ptDevItem, szUser, szPwd));

	return TRUE;
}

SEARCH_API BOOL SendActiveMsg (u32 dwDstIP, u8* pDstMacAddr, u8* abySecureCode, char* szUser, char* szMail, char* szPwdNew)
{
	typedef BOOL (*_pFun)(u32, u8*, u8*, char*, char*, char* );
	LOADDLLFUN(_pFun, "SendActiveMsg", PARAM6(dwDstIP, pDstMacAddr, abySecureCode, szUser, szMail, szPwdNew));

	return TRUE;
}

SEARCH_API BOOL SendActiveMsgS (TDevItem* ptDevItem, char* szUser, char* szMail, char* szPwdNew)
{
	typedef BOOL (*_pFun)(TDevItem*, char*, char*, char*);
	LOADDLLFUN(_pFun, "SendActiveMsgS", PARAM4(ptDevItem, szUser, szMail, szPwdNew));

	return TRUE;
}

SEARCH_API BOOL SendNewPwdMsg (u32 dwDstIP, u8* pDstMacAddr, u8* abySecureCode, char* szUser, char* szPwd, char* szPwdNew)
{
	typedef BOOL (*_pFun)(u32, u8*, u8*, char*, char*, char*);
	LOADDLLFUN(_pFun, "SendNewPwdMsg", PARAM6(dwDstIP, pDstMacAddr, abySecureCode, szUser, szPwd, szPwdNew));

	return TRUE;
}

SEARCH_API BOOL SendNewPwdMsgS (TDevItem* ptDevItem, char* szUser, char* szPwd, char* szPwdNew)
{
	typedef BOOL (*_pFun)(TDevItem*, char*, char*, char*);
	LOADDLLFUN(_pFun, "SendNewPwdMsgS", PARAM4(ptDevItem, szUser, szPwd, szPwdNew));

	return TRUE;
}

SEARCH_API BOOL SendBWListMsg (u32 dwDstIP, u8* pDstMacAddr, u8* abySecureCode, char* szUser, char* szPwd)
{
	typedef BOOL (*_pFun)(u32, u8*, u8*, char*, char*);
	LOADDLLFUN(_pFun, "SendBWListMsg", PARAM5(dwDstIP, pDstMacAddr, abySecureCode, szUser, szPwd));

	return TRUE;
}

SEARCH_API BOOL SendBWListMsgS (TDevItem* ptDevItem, char* szUser, char* szPwd)
{
	typedef BOOL (*_pFun)(TDevItem*, char*, char*);
	LOADDLLFUN(_pFun, "SendBWListMsgS", PARAM3(ptDevItem, szUser, szPwd));

	return TRUE;
}

SEARCH_API BOOL SendResetMsg (u32 dwDstIP, u8* pDstMacAddr, u8* abySecureCode, char* szUser, char* szPwd)
{
	typedef BOOL (*_pFun)(u32, u8*, u8*, char*, char*);
	LOADDLLFUN(_pFun, "SendResetMsg", PARAM5(dwDstIP, pDstMacAddr, abySecureCode, szUser, szPwd));

	return TRUE;
}

SEARCH_API BOOL SendResetMsgS (TDevItem* ptDevItem, char* szUser, char* szPwd)
{
	typedef BOOL (*_pFun)(TDevItem*, char*, char*);
	LOADDLLFUN(_pFun, "SendResetMsgS", PARAM3(ptDevItem, szUser, szPwd));

	return TRUE;
}

SEARCH_API BOOL SendUpgradeMsg (u32 dwDstIP, u8* pDstMacAddr, u8* abySecureCode, char* szUser, char* szPwd, TDTFtpUpdateInfo* pFtpUpdateInfo)
{
	typedef BOOL (*_pFun)(u32, u8*, u8*, char*, char*, TDTFtpUpdateInfo*);
	LOADDLLFUN(_pFun, "SendUpgradeMsg", PARAM6(dwDstIP, pDstMacAddr, abySecureCode, szUser, szPwd, pFtpUpdateInfo));

	return TRUE;
}

SEARCH_API BOOL SendUpgradeMsgS (TDevItem* ptDevItem, char* szUser, char* szPwd, TDTFtpUpdateInfo* pFtpUpdateInfo)
{
	typedef BOOL (*_pFun)(TDevItem*, char*, char*, TDTFtpUpdateInfo*);
	LOADDLLFUN(_pFun, "SendUpgradeMsgS", PARAM4(ptDevItem, szUser, szPwd, pFtpUpdateInfo));

	return TRUE;
}

SEARCH_API BOOL SendImporCfgMsg (u32 dwDstIP, u8* pDstMacAddr, u8* abySecureCode, char* szUser, char* szPwd, TDTFtpUpdateInfo* pFtpUpdateInfo)
{
	typedef BOOL (*_pFun)(u32, u8*, u8*, char*, char*, TDTFtpUpdateInfo*);
	LOADDLLFUN(_pFun, "SendImporCfgMsg", PARAM6(dwDstIP, pDstMacAddr, abySecureCode, szUser, szPwd, pFtpUpdateInfo));

	return TRUE;
}

SEARCH_API BOOL SendImporCfgMsgS (TDevItem* ptDevItem, char* szUser, char* szPwd, TDTFtpUpdateInfo* pFtpUpdateInfo)
{
	typedef BOOL (*_pFun)(TDevItem*, char*, char*, TDTFtpUpdateInfo*);
	LOADDLLFUN(_pFun, "SendImporCfgMsgS", PARAM4(ptDevItem, szUser, szPwd, pFtpUpdateInfo));

	return TRUE;
}

SEARCH_API BOOL SendSSHMsg (u32 dwDstIP, u8* pDstMacAddr, u8* abySecureCode, char* szUser, char* szPwd)
{
	typedef BOOL (*_pFun)(u32, u8*, u8*, char*, char*);
	LOADDLLFUN(_pFun, "SendSSHMsg", PARAM5(dwDstIP, pDstMacAddr, abySecureCode, szUser, szPwd));

	return TRUE;
}

SEARCH_API BOOL SendSSHMsgS (TDevItem* ptDevItem, char* szUser, char* szPwd)
{
	typedef BOOL (*_pFun)(TDevItem*, char*, char*);
	LOADDLLFUN(_pFun, "SendSSHMsgS", PARAM3(ptDevItem, szUser, szPwd));

	return TRUE;
}

SEARCH_API BOOL SendNetPrmMsg (u32 dwDstIP, u8* pDstMacAddr, u8* abySecureCode, char* szUser, char* szPwd, TDevNetParam* pDevNetPrm)
{
	typedef BOOL (*_pFun)(u32, u8*, u8*, char*, char*, TDevNetParam*);
	LOADDLLFUN(_pFun, "SendNetPrmMsg", PARAM6(dwDstIP, pDstMacAddr, abySecureCode, szUser, szPwd, pDevNetPrm));

	return TRUE;
}

SEARCH_API BOOL SendNetPrmMsgS (TDevItem* ptDevItem, char* szUser, char* szPwd, TDevNetParam* pDevNetPrm)
{
	typedef BOOL (*_pFun)(TDevItem*, char*, char*, TDevNetParam*);
	LOADDLLFUN(_pFun, "SendNetPrmMsgS", PARAM4(ptDevItem, szUser, szPwd, pDevNetPrm));

	return TRUE;
}

SEARCH_API BOOL SendPwdRecoverMsg (u32 dwDstIP, u8* pDstMacAddr, u8* abySecureCode, char* abyCommand)
{
	typedef BOOL (*_pFun)(u32, u8*, u8*, char*);
	LOADDLLFUN(_pFun, "SendPwdRecoverMsg", PARAM4(dwDstIP, pDstMacAddr, abySecureCode, abyCommand));

	return TRUE;
}

SEARCH_API BOOL SendPwdRecoverMsgS (TDevItem* ptDevItem, char* abyCommand)
{
	typedef BOOL (*_pFun)(TDevItem*, char*);
	LOADDLLFUN(_pFun, "SendPwdRecoverMsgS", PARAM2(ptDevItem, abyCommand));

	return TRUE;
}