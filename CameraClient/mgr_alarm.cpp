/*==============================================================================
ģ����      :  CMgrAlarm
�ļ���      :  CMgrAlarm.h
����ļ�    :  CMgrAlarm.cpp
�ļ�ʵ�ֹ���:  �澯���ݹ���
����        :
��Ȩ        :  <Copyright(C) 2003-2010 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------
�޸ļ�¼    :
��  ��         �汾          �޸���          �߶���          �޸ļ�¼
2015/12/28     1.0           ���ǿ�                          ����
==============================================================================*/
#include "stdafx.h"
#include "mgr_alarm.h"

////////////////////////////////////////////////////////////////////////

MgrData *m_pMgrData9 = nullptr;
//////////////////////////////////////////////////////////////////////////
CMgrAlarm::CMgrAlarm(void *pData)
{
	m_pMgrData9 = (MgrData*)pData;

	Init();
}

CMgrAlarm::~CMgrAlarm()
{
	Release();
}

void CMgrAlarm::Init()
{
	Clear();
}

void CMgrAlarm::Release()
{
	Clear();
}

void CMgrAlarm::Clear()
{
	CCriticalLock lock(_pcs);
	m_dwMapKey = 1;
	TMAPALARMINFOITE ite = m_mapAlarmInfo.begin();
	while (ite != m_mapAlarmInfo.end())
	{
		if (ite->second != NULL)
		{
			delete  ite->second;
			ite->second = NULL;
		}

		ite++;
	}

	m_mapAlarmInfo.clear();
}

void	CMgrAlarm::AddAlarm(PTALARMINFO_ITS p)
{
	CCriticalLock lock(_pcs);

	PTALARMINFO_ITS p2 = new TALARMINFO_ITS;
	memcpy(p2, p, sizeof(TALARMINFO_ITS));
	m_mapAlarmInfo[m_dwMapKey++] = p2;

	Notify(m_dwMapKey - 1);
}

void	CMgrAlarm::DelAlarm(PTALARMINFO_ITS p)
{
	// 	CCriticalLock(_pcs);
	// 
	// 	TMAPALARMINFOITE ite = m_mapAlarmInfo.begin();
	// 	while(ite != m_mapAlarmInfo.end())
	// 	{
	// 		if (ite->second != NULL)
	// 		{
	// 			delete  ite->second;
	// 			ite->second = NULL;
	// 		}
	// 
	// 		ite++
	// 	}
}

void	CMgrAlarm::DelAlarm(u32 dwAlarmId)
{
	CCriticalLock lock(_pcs);

	TMAPALARMINFOITE ite = m_mapAlarmInfo.find(dwAlarmId);
	if (ite != m_mapAlarmInfo.end())
	{
		if (ite->second != NULL)
		{
			delete  ite->second;
			ite->second = NULL;

			Notify(ite->first);
		}

		m_mapAlarmInfo.erase(ite);
	}
}

PTALARMINFO_ITS	CMgrAlarm::GetAlarm(u32 dwAlarmId)
{
	CCriticalLock lock(_pcs);

	TMAPALARMINFOITE ite = m_mapAlarmInfo.find(dwAlarmId);
	if (ite != m_mapAlarmInfo.end())
	{
		if (ite->second != NULL)
		{
			return  ite->second;
		}
	}

	return NULL;
}

void	CMgrAlarm::Notify(u32 dwAlarmId)
{
	m_pMgrData9->Notify(NMC_ALARM, (LPVOID)dwAlarmId);
}