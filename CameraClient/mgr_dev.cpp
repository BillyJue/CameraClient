/*==============================================================================
ģ����      :  CMgrDev
�ļ���      :  CMgrDev.h
����ļ�    :  CMgrDev.cpp
�ļ�ʵ�ֹ���:  Ԥ������Ԥ���洢��ȡ��ת��
����        :
��Ȩ        :  <Copyright(C) 2003-2010 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------
�޸ļ�¼    :
��  ��         �汾          �޸���          �߶���          �޸ļ�¼
2015/10/28     1.0           ���ǿ�                          ����
==============================================================================*/
#include "stdafx.h"
#include "mgr_dev.h"

////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
u32 g_dwDevID = 1;
MgrData *m_pMgrData8 = nullptr;
CMgrDev::CMgrDev(void* pData)
{
	m_pMgrData8 = (MgrData*)pData;
	Init();
}

CMgrDev::~CMgrDev()
{
	Release();
}

void CMgrDev::Init()
{

}

void CMgrDev::Release()
{
	CCriticalLock lock(_pcs);

	std::map<u32, PTDEVINFO>::iterator ite;
	ite = m_mapDevInfo.begin();
	while (ite != m_mapDevInfo.end())
	{
		if (ite->second != NULL)
		{
			delete ite->second;;
			ite->second = NULL;
		}
		ite++;
	}

	m_mapDevInfo.clear();
}



void CMgrDev::Load()
{

}

void	CMgrDev::Add(PTDEVINFO ptInfo)
{
	CCriticalLock lock(_pcs);

	PTDEVINFO ptInfo2 = new TDEVINFO;
	memcpy(ptInfo2, ptInfo, sizeof(TDEVINFO));
	m_mapDevInfo[g_dwDevID++] = ptInfo2;
}
void	CMgrDev::Del(u32 dwDevID)
{
	CCriticalLock lock(_pcs);

	std::map<u32, PTDEVINFO>::iterator ite;
	ite = m_mapDevInfo.find(dwDevID);
	if (ite != m_mapDevInfo.end())
	{
		delete ite->second;;
		ite->second = NULL;
		m_mapDevInfo.erase(ite);
	}
}
void	CMgrDev::Update(u32 dwDevID, PTDEVINFO ptInfo)
{
	CCriticalLock lock(_pcs);

	std::map<u32, PTDEVINFO>::iterator ite;
	ite = m_mapDevInfo.find(dwDevID);
	if (ite != m_mapDevInfo.end())
	{
		memcpy(ite->second, ptInfo, sizeof(TDEVINFO));
	}
}
PTDEVINFO 	CMgrDev::Get(u32 dwDevID)
{
	std::map<u32, PTDEVINFO>::iterator ite;
	ite = m_mapDevInfo.find(dwDevID);
	if (ite != m_mapDevInfo.end())
	{
		return ite->second;
	}

	return NULL;
}

