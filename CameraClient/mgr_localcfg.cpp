/*==============================================================================
模块名      :  CMgrLocalCfg
文件名      :  CMgrLocalCfg.h
相关文件    :  CMgrLocalCfg.cpp
文件实现功能:  用户管理

作者        :
版权        :  <Copyright(C) 2003-2010 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------
修改记录    :
日  期         版本          修改人          走读人          修改记录
2016/6/28       1.0           廖晖                          创建
==============================================================================*/

#include "stdafx.h"
#include "mgr_localcfg.h"
#include <ShlObj.h>
#include <QDir>
MgrData *m_pMgrData3 = nullptr;
//获取用户路径

/*
std::wstring GetUserPath2()
{
	wchar_t m_lpszDefaultDir[MAX_PATH];
	wchar_t szDocument[MAX_PATH] = { 0 };
	memset(m_lpszDefaultDir, 0, _MAX_PATH);

	LPITEMIDLIST pidl = NULL;
	SHGetSpecialFolderLocation(NULL, CSIDL_LOCAL_APPDATA, &pidl);
	if (pidl && SHGetPathFromIDList(pidl, szDocument))
	{
		GetShortPathName(szDocument, m_lpszDefaultDir, _MAX_PATH);
	}

	std::wstring wsR = m_lpszDefaultDir;
	wsR += L"/itsclient/";
	CreateDirectory(wsR.c_str(), NULL);

	return wsR;
}
*/




CMgrLocalCfg::CMgrLocalCfg(void* pData)
{
	m_pMgrData3 = (MgrData*)pData;
	m_tCfgFilePath.clear();
	Init();
}

CMgrLocalCfg::~CMgrLocalCfg()
{
	Release();
}

void CMgrLocalCfg::Init()
{

	m_tCfgFilePath = m_pMgrData3->GetUserPath().toStdWString();
	m_tCfgFilePath += L"localcfg.ini";

	GetPrivateProfileStringW(TEXT("localcfg"), TEXT("pictruepath"), TEXT("C:\\IPC Control\\PuSnapShot"), m_tLocalCfgInfo.picturepath, MAX_PATH_LEN, m_tCfgFilePath.c_str());

	GetPrivateProfileStringW(TEXT("localcfg"), TEXT("videopath"), TEXT("C:\\IPC Control\\Video"), m_tLocalCfgInfo.videopath, MAX_PATH_LEN, m_tCfgFilePath.c_str());

	m_tLocalCfgInfo.isrtsp = GetPrivateProfileIntW(TEXT("localcfg"), TEXT("isrtsp"), 0, m_tCfgFilePath.c_str());

	m_tLocalCfgInfo.istcp = GetPrivateProfileIntW(TEXT("localcfg"), TEXT("istcp"), 0, m_tCfgFilePath.c_str());

	m_tLocalCfgInfo.ispic = GetPrivateProfileIntW(TEXT("localcfg"), TEXT("ispic"), 0, m_tCfgFilePath.c_str());

	m_tLocalCfgInfo.isip = GetPrivateProfileIntW(TEXT("localcfg"), TEXT("isip"), 1, m_tCfgFilePath.c_str());

	m_tLocalCfgInfo.isyear = GetPrivateProfileIntW(TEXT("localcfg"), TEXT("isyear"), 1, m_tCfgFilePath.c_str());

	m_tLocalCfgInfo.ismonth = GetPrivateProfileIntW(TEXT("localcfg"), TEXT("ismonth"), 1, m_tCfgFilePath.c_str());

	m_tLocalCfgInfo.isday = GetPrivateProfileIntW(TEXT("localcfg"), TEXT("isday"), 1, m_tCfgFilePath.c_str());

	m_tLocalCfgInfo.ishour = GetPrivateProfileIntW(TEXT("localcfg"), TEXT("ishour"), 1, m_tCfgFilePath.c_str());

	m_tLocalCfgInfo.isBayonet = GetPrivateProfileIntW(TEXT("localcfg"), TEXT("isBayonet"), 1, m_tCfgFilePath.c_str());

	m_tLocalCfgInfo.isBayonetMontage = GetPrivateProfileIntW(TEXT("localcfg"), TEXT("isBayonetMontage"), 1, m_tCfgFilePath.c_str());

	m_tLocalCfgInfo.isillegal = GetPrivateProfileIntW(TEXT("localcfg"), TEXT("isillegal"), 1, m_tCfgFilePath.c_str());

	m_tLocalCfgInfo.isillegalMontage = GetPrivateProfileIntW(TEXT("localcfg"), TEXT("isillegalMontage"), 1, m_tCfgFilePath.c_str());

	m_tLocalCfgInfo.isVideo = GetPrivateProfileIntW(TEXT("localcfg"), TEXT("isVideo"), 1, m_tCfgFilePath.c_str());

	m_tLocalCfgInfo.isFace = GetPrivateProfileIntW(TEXT("localcfg"), TEXT("isFace"), 1, m_tCfgFilePath.c_str());

	m_tLocalCfgInfo.isVehicle = GetPrivateProfileIntW(TEXT("localcfg"), TEXT("isVehicle"), 1, m_tCfgFilePath.c_str());

	m_tLocalCfgInfo.isPlate = GetPrivateProfileIntW(TEXT("localcfg"), TEXT("isPlate"), 1, m_tCfgFilePath.c_str());

	m_tLocalCfgInfo.isHardware = GetPrivateProfileIntW(TEXT("localcfg"), TEXT("isHardware"), 0, m_tCfgFilePath.c_str());

	m_tLocalCfgInfo.serialNum = GetPrivateProfileIntW(TEXT("localcfg"), TEXT("SerialNum"), 0, m_tCfgFilePath.c_str());

	m_tLocalCfgInfo.serialRate = GetPrivateProfileIntW(TEXT("localcfg"), TEXT("SerialRate"), 6, m_tCfgFilePath.c_str());
}

void CMgrLocalCfg::Release()
{
}

void  CMgrLocalCfg::GetLocalCfg(TLocalCfgInfo *ptLocalCfgInfo)
{
	CCriticalLock lock(_pcs);

	memcpy(ptLocalCfgInfo, &m_tLocalCfgInfo, sizeof(TLocalCfgInfo));
}

void  CMgrLocalCfg::SetLocalCfg(TLocalCfgInfo *ptLocalCfgInfo, bool bSave)
{
	CCriticalLock lock(_pcs);

	memcpy(&m_tLocalCfgInfo, ptLocalCfgInfo, sizeof(TLocalCfgInfo));

	if (bSave)
	{
		WritePrivateProfileStringW(TEXT("localcfg"), TEXT("pictruepath"), m_tLocalCfgInfo.picturepath, m_tCfgFilePath.c_str());
		WritePrivateProfileStringW(TEXT("localcfg"), TEXT("videopath"), m_tLocalCfgInfo.videopath, m_tCfgFilePath.c_str());

		WritePrivateProfileInt(TEXT("localcfg"), TEXT("isrtsp"), m_tLocalCfgInfo.isrtsp, m_tCfgFilePath.c_str());
		WritePrivateProfileInt(TEXT("localcfg"), TEXT("istcp"), m_tLocalCfgInfo.istcp, m_tCfgFilePath.c_str());
		WritePrivateProfileInt(TEXT("localcfg"), TEXT("ispic"), m_tLocalCfgInfo.ispic, m_tCfgFilePath.c_str());
		WritePrivateProfileInt(TEXT("localcfg"), TEXT("isip"), m_tLocalCfgInfo.isip, m_tCfgFilePath.c_str());
		WritePrivateProfileInt(TEXT("localcfg"), TEXT("isyear"), m_tLocalCfgInfo.isyear, m_tCfgFilePath.c_str());
		WritePrivateProfileInt(TEXT("localcfg"), TEXT("ismonth"), m_tLocalCfgInfo.ismonth, m_tCfgFilePath.c_str());
		WritePrivateProfileInt(TEXT("localcfg"), TEXT("isday"), m_tLocalCfgInfo.isday, m_tCfgFilePath.c_str());
		WritePrivateProfileInt(TEXT("localcfg"), TEXT("ishour"), m_tLocalCfgInfo.ishour, m_tCfgFilePath.c_str());

		WritePrivateProfileInt(TEXT("localcfg"), TEXT("isBayonet"), m_tLocalCfgInfo.isBayonet, m_tCfgFilePath.c_str());
		WritePrivateProfileInt(TEXT("localcfg"), TEXT("isBayonetMontage"), m_tLocalCfgInfo.isBayonetMontage, m_tCfgFilePath.c_str());
		WritePrivateProfileInt(TEXT("localcfg"), TEXT("isillegal"), m_tLocalCfgInfo.isillegal, m_tCfgFilePath.c_str());
		WritePrivateProfileInt(TEXT("localcfg"), TEXT("isillegalMontage"), m_tLocalCfgInfo.isillegalMontage, m_tCfgFilePath.c_str());
		WritePrivateProfileInt(TEXT("localcfg"), TEXT("isVideo"), m_tLocalCfgInfo.isVideo, m_tCfgFilePath.c_str());
		WritePrivateProfileInt(TEXT("localcfg"), TEXT("isFace"), m_tLocalCfgInfo.isFace, m_tCfgFilePath.c_str());
		WritePrivateProfileInt(TEXT("localcfg"), TEXT("isVehicle"), m_tLocalCfgInfo.isVehicle, m_tCfgFilePath.c_str());
		WritePrivateProfileInt(TEXT("localcfg"), TEXT("isPlate"), m_tLocalCfgInfo.isPlate, m_tCfgFilePath.c_str());
		WritePrivateProfileInt(TEXT("localcfg"), TEXT("isHardware"), m_tLocalCfgInfo.isHardware, m_tCfgFilePath.c_str());
		WritePrivateProfileInt(TEXT("localcfg"), TEXT("SerialNum"), m_tLocalCfgInfo.serialNum, m_tCfgFilePath.c_str());
		WritePrivateProfileInt(TEXT("localcfg"), TEXT("SerialRate"), m_tLocalCfgInfo.serialRate, m_tCfgFilePath.c_str());
	}
}

wchar_t*  CMgrLocalCfg::GetVideoPath()
{
	return m_tLocalCfgInfo.videopath;
}

BOOL CMgrLocalCfg::WritePrivateProfileInt(LPCTSTR lpAppName, LPCTSTR lpKeyName, INT Value, LPCTSTR lpFileName)
{
	TCHAR ValBuf[16];

	wsprintf(ValBuf, TEXT("%i"), Value);

	return(WritePrivateProfileString(lpAppName, lpKeyName, ValBuf, lpFileName));
}