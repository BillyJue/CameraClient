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
		//����ͬ���¼�����-�˳��¼�
		m_hExitHandle=CreateEvent( NULL, // �¼�����
			TRUE,  // �Ƿ��ֶ�����
			FALSE, // �Ƿ����ź�,���ź�����ִ�У����źŵȴ�����
			NULL); // �¼�����
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

	// ��Ҫ�ȴ��˳������̴߳������иı�m_nState��ֵ
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

// �ж��߳��Ƿ��˳�
bool CThreadBase::IsExit(void)
{
	if (m_hTheadHandle == NULL)
	{
		return true;
	}

	return FALSE;
}




