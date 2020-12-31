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

#include "stdafx.h"
#include "threadbase.h"
#include "process.h"
#include <QDebug>
CThreadBase::CThreadBase(void)
{
	m_hExitHandle = NULL;
	m_hTheadHandle = NULL;
	m_uTheadId = 0;
	m_pContent = NULL;
	m_nRunState = 0;
	m_dwTimeOut = 3000;
}

CThreadBase::~CThreadBase(void)
{
	if (m_hTheadHandle)
	{
		CloseHandle(m_hTheadHandle);
		m_hTheadHandle = NULL;
	}
	
	if (m_hExitHandle)
	{
		CloseHandle(m_hExitHandle);
		m_hExitHandle = NULL;
	}	
}

bool CThreadBase::Run( void* pParam, void* pContent, pFunRun pfun )
{
	if(m_hTheadHandle != NULL)
		return TRUE;

	m_pContent		= pContent;
	if (m_hExitHandle == NULL)
	{
		//创建同步事件对象-退出事件
		m_hExitHandle=CreateEvent( NULL, // 事件属性
			TRUE,  // 是否手动设置
			FALSE, // 是否有信号,有信号立刻执行，无信号等待激活
			NULL); // 事件名称
	}

	m_hTheadHandle	= (HANDLE)_beginthreadex(NULL, 0, *pfun, pParam, 0, &m_uTheadId);
	if (m_hTheadHandle ==  NULL)
	{
		qCritical()<<("begingthread faild");
		return FALSE;
	}
	
	return TRUE;
}

bool CThreadBase::Suspend()
{
	if(m_hTheadHandle)
	{
		SuspendThread(m_hTheadHandle);
		qCritical() << ("CThreadBase::Pause() Handle == %d", m_hTheadHandle);
		return TRUE;
	}

	return FALSE;
}

bool CThreadBase::Resume()
{
	if(m_hTheadHandle)
	{
		ResumeThread(m_hTheadHandle);
		qCritical() << ("CThreadBase::Resume() Handle == %d", m_hTheadHandle);
		return TRUE;
	}

	return FALSE;
}

bool CThreadBase::Exit(bool bWaiting)
{
	if (IsExit())
	{
		return true;
	}

	DWORD dw = GetTickCount();
	SetEvent(m_hExitHandle);

	// 需要等待退出，在线程处理函数中改变m_nState的值
	if (bWaiting)
	{
		while(!IsExit() && GetTickCount() - dw < m_dwTimeOut)
		{
			SetEvent(m_hExitHandle);
			Sleep(100);
		}
	}
	else
	{
		SuspendThread(m_hTheadHandle);
		TerminateThread(m_hTheadHandle, 0);
	}

	qCritical() << (TEXT("Exit1   time = %d \n"), GetTickCount() - dw);

	return TRUE;
}

void CThreadBase::SetExit()
{
	m_hTheadHandle = NULL;
	m_nRunState = 0;
}

// 判断线程是否退出
bool CThreadBase::IsExit(void)
{
	if (m_hTheadHandle == NULL)
	{
		return true;
	}

	return FALSE;
}




