/*==============================================================================
模块名      :  threadbase
文件名      :  threadbase.h
相关文件    :  threadbase.cpp
文件实现功能:  基础线程，兼容
作者        :  
版权        :  <Copyright(C) 2003-2010 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------
修改记录    :  
日  期         版本          修改人          走读人          修改记录
2015/10/28     1.0           刘登科                          创建
==============================================================================*/
#pragma once
#include "define.h"

typedef UINT (__stdcall *pFunRun)(void* ptr); 

class CThreadBase
{
public:
	CThreadBase(void);
	~CThreadBase(void);

public:
	HANDLE m_hExitHandle;
	HANDLE m_hTheadHandle;

	
	unsigned m_uTheadId;
	void*	m_pContent;

	u32		m_dwTimeOut;
	u32		m_nRunState;

public:
	bool Run(void* pParam, void* pContent, pFunRun pfun);
	bool Suspend();
	bool Resume();
	bool Exit(bool bWaiting = true);
	bool IsExit(void);
	void SetExit();
};


