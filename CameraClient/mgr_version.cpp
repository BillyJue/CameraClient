/*==============================================================================
模块名      :  CMgrVersion
文件名      :  versionmgr.h
相关文件    :  versionmgr.cpp
文件实现功能:  版本管理模块，保存制定ip的业务版本，获取时间
作者        :  刘登科
版权        :  <Copyright(C) 2003-2010 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------
修改记录    :
日  期         版本          修改人          走读人          修改记录
2015/12/2     1.0           刘登科                          创建
==============================================================================*/
#include "stdafx.h"
#include "mgr_version.h"

CMgrVersion::CMgrVersion(u32 dwKey)
{
	m_dwKey = dwKey;
	memset(m_pszVersion, 0, 260);
	sprintf(m_pszVersion, "%s %s %s", PRODUCT_VER, __DATE__, __TIME__);
}

CMgrVersion::~CMgrVersion()
{

}

void CMgrVersion::SetVersion(char* pszVersion)
{
	strcpy(m_pszVersion, pszVersion);
}

char* CMgrVersion::GetVersion()
{
	return m_pszVersion;
}

int CMgrVersion::Compare(char* pszVersion)
{
	return strcmp(m_pszVersion, pszVersion);
}

BOOL CMgrVersion::IsMulAuto()
{
	if (Compare("1.1.3.1") >= 0)
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CMgrVersion::IsMulAuto(void* pHandle)
{


	return FALSE;
}
