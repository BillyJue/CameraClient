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
2015/08/24     1.0           ���ǿ�                          ����
==============================================================================*/
#pragma once
#include "stdafx.h"
#include <map>
#include "define.h"
class CMgrVersion
{
	// ����
public:
	CMgrVersion(u32 dwKey);
	~CMgrVersion();

	// ʵ��
public:
	void SetVersion(char* pszVersion);
	char* GetVersion();
	int  Compare(char* pszVersion); // ����汾�󷵻� 1 ����� ���� 0 ��С�ڷ��� -1��

	// ����֧��ʶ��
public:
	// �Ƿ�֧�ֶ�·��Ƶ,����
	BOOL IsMulAuto();
	static BOOL IsMulAuto(void* pHandle);

public:
	u32			m_dwKey;
	char		m_pszVersion[MAX_PATH];
};

