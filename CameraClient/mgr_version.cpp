/*==============================================================================
ģ����      :  CMgrVersion
�ļ���      :  versionmgr.h
����ļ�    :  versionmgr.cpp
�ļ�ʵ�ֹ���:  �汾����ģ�飬�����ƶ�ip��ҵ��汾����ȡʱ��
����        :  ���ǿ�
��Ȩ        :  <Copyright(C) 2003-2010 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------
�޸ļ�¼    :
��  ��         �汾          �޸���          �߶���          �޸ļ�¼
2015/12/2     1.0           ���ǿ�                          ����
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
