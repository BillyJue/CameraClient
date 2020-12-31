/*==============================================================================
ģ����      :  threadbase
�ļ���      :  threadbase.h
����ļ�    :  threadbase.cpp
�ļ�ʵ�ֹ���:  �����̣߳�����
����        :  
��Ȩ        :  <Copyright(C) 2003-2010 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------
�޸ļ�¼    :  
��  ��         �汾          �޸���          �߶���          �޸ļ�¼
2015/10/28     1.0           ���ǿ�                          ����
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


