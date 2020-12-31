/*==============================================================================
ģ����      :  CMgrLocalCfg
�ļ���      :  CMgrLocalCfg.h
����ļ�    :  CMgrLocalCfg.cpp
�ļ�ʵ�ֹ���:  �û�����

����        :
��Ȩ        :  <Copyright(C) 2003-2010 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------
�޸ļ�¼    :
��  ��         �汾          �޸���          �߶���          �޸ļ�¼
2016/6/28       1.0           ����                          ����
==============================================================================*/

#pragma once
#include "stdafx.h"


class CMgrLocalCfg :public CCriticalObject
{
public:
	CMgrLocalCfg(void* pData);
	~CMgrLocalCfg();

public:
	void	Init();
	void	Release();
	void	GetLocalCfg(TLocalCfgInfo *ptLocalCfgInfo);
	void	SetLocalCfg(TLocalCfgInfo *ptLocalCfgInfo, bool bSave);
	wchar_t*  GetVideoPath();
	BOOL WritePrivateProfileInt(LPCTSTR lpAppName, LPCTSTR lpKeyName, INT Value, LPCTSTR lpFileName);
protected:
	

	TLocalCfgInfo       m_tLocalCfgInfo;
	std::wstring             m_tCfgFilePath;

};
