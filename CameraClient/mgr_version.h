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
2015/08/24     1.0           刘登科                          创建
==============================================================================*/
#pragma once
#include "stdafx.h"
#include <map>
#include "define.h"
class CMgrVersion
{
	// 构造
public:
	CMgrVersion(u32 dwKey);
	~CMgrVersion();

	// 实现
public:
	void SetVersion(char* pszVersion);
	char* GetVersion();
	int  Compare(char* pszVersion); // 输入版本大返回 1 ，相等 返回 0 ，小于返回 -1；

	// 功能支持识别
public:
	// 是否支持多路音频,样例
	BOOL IsMulAuto();
	static BOOL IsMulAuto(void* pHandle);

public:
	u32			m_dwKey;
	char		m_pszVersion[MAX_PATH];
};

