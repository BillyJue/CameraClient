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
