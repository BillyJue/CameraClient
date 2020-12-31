/*==============================================================================
模块名      :  CMgrAlarm
文件名      :  CMgrAlarm.h
相关文件    :  CMgrAlarm.cpp
文件实现功能:  告警数据管理
作者        :
版权        :  <Copyright(C) 2003-2010 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------
修改记录    :
日  期         版本          修改人          走读人          修改记录
2015/12/28     1.0           刘登科                          创建
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