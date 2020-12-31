#pragma once
#include <windows.h>
#include "search_sdk_define.h"

#define SEARCH_API  extern "C" __declspec(dllexport)

typedef void (*MsgAckCB)( UINT wMsg, LPVOID pParam, DWORD dwContext);

SEARCH_API BOOL IPCSEARCH_InitDll (MsgAckCB mCB, DWORD dwContext);
SEARCH_API BOOL IPCSEARCH_ReleaseDll ();
SEARCH_API BOOL GetLocalIP (DWORD &dwLocalIP, DWORD dwDestIP, DWORD dwDestIPMask);
SEARCH_API BOOL SendDevDetectMsg (BOOL bRefreshAdapter);
SEARCH_API BOOL SendDevDetectUniMsg (BOOL bRefreshAdapter, DWORD dwDstIP);
SEARCH_API BOOL SendRebootMsg (u32 dwDstIP, u8* pDstMacAddr, u8* abySecureCode, char* szUser, char* szPwd);
SEARCH_API BOOL SendRebootMsgS (TDevItem* ptDevItem, char* szUser, char* szPwd);
SEARCH_API BOOL SendActiveMsg (u32 dwDstIP, u8* pDstMacAddr, u8* abySecureCode, char* szUser, char* szMail, char* szPwdNew);
SEARCH_API BOOL SendActiveMsgS (TDevItem* ptDevItem, char* szUser, char* szMail, char* szPwdNew);
SEARCH_API BOOL SendNewPwdMsg (u32 dwDstIP, u8* pDstMacAddr, u8* abySecureCode, char* szUser, char* szPwd, char* szPwdNew);
SEARCH_API BOOL SendNewPwdMsgS (TDevItem* ptDevItem, char* szUser, char* szPwd, char* szPwdNew);
SEARCH_API BOOL SendBWListMsg (u32 dwDstIP, u8* pDstMacAddr, u8* abySecureCode, char* szUser, char* szPwd);
SEARCH_API BOOL SendBWListMsgS (TDevItem* ptDevItem, char* szUser, char* szPwd);
SEARCH_API BOOL SendResetMsg (u32 dwDstIP, u8* pDstMacAddr, u8* abySecureCode, char* szUser, char* szPwd);
SEARCH_API BOOL SendResetMsgS (TDevItem* ptDevItem, char* szUser, char* szPwd);
SEARCH_API BOOL SendSSHMsg (u32 dwDstIP, u8* pDstMacAddr, u8* abySecureCode, char* szUser, char* szPwd);
SEARCH_API BOOL SendSSHMsgS (TDevItem* ptDevItem, char* szUser, char* szPwd);
SEARCH_API BOOL SendUpgradeMsg (u32 dwDstIP, u8* pDstMacAddr, u8* abySecureCode, char* szUser, char* szPwd, TDTFtpUpdateInfo* pFtpUpdateInfo);
SEARCH_API BOOL SendUpgradeMsgS (TDevItem* ptDevItem, char* szUser, char* szPwd, TDTFtpUpdateInfo* pFtpUpdateInfo);
SEARCH_API BOOL SendImporCfgMsg (u32 dwDstIP, u8* pDstMacAddr, u8* abySecureCode, char* szUser, char* szPwd, TDTFtpUpdateInfo* pFtpUpdateInfo);
SEARCH_API BOOL SendImporCfgMsgS (TDevItem* ptDevItem, char* szUser, char* szPwd, TDTFtpUpdateInfo* pFtpUpdateInfo);
SEARCH_API BOOL SendNetPrmMsg (u32 dwDstIP, u8* pDstMacAddr, u8* abySecureCode, char* szUser, char* szPwd, TDevNetParam* pDevNetPrm);
SEARCH_API BOOL SendNetPrmMsgS (TDevItem* ptDevItem, char* szUser, char* szPwd, TDevNetParam* pDevNetPrm);
SEARCH_API BOOL SendPwdRecoverMsg (u32 dwDstIP, u8* pDstMacAddr, u8* abySecureCode, char* abyCommand);
SEARCH_API BOOL SendPwdRecoverMsgS (TDevItem* ptDevItem, char* abyCommand);