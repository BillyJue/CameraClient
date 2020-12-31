/*==============================================================================
模块名      :  CMgrDev
文件名      :  CMgrDev.h
相关文件    :  CMgrDev.cpp
文件实现功能:  预案管理，预案存储读取，转换
作者        :
版权        :  <Copyright(C) 2003-2010 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------
修改记录    :
日  期         版本          修改人          走读人          修改记录
2015/10/28     1.0           刘登科                          创建
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

