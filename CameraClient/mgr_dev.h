/*==============================================================================
模块名      :  CMgrDev
文件名      :  MgrDev.h
相关文件    :  MgrDev.cpp
文件实现功能:  设备管理

作者        :
版权        :  <Copyright(C) 2003-2010 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------
修改记录    :
日  期         版本          修改人          走读人          修改记录
2016/1/7       1.0           刘登科                          创建
==============================================================================*/

#pragma once
#include "stdafx.h"


////////////////////////////////////////////////////////////////////////

class CMgrDev :public CCriticalObject
{
public:
	CMgrDev(void* pData);
	~CMgrDev();
public:

	/*=================================================================
	函数名称: Init
	功    能: 初始化
	参数说明:
	返 回 值: 无
	=================================================================*/
	void	Init();
	void    Release();

	/*=================================================================
	函数名称:
	功    能: 添加、删除、修改
	参数说明:
	返 回 值: 无
	=================================================================*/
	void	Load();
	void	Add(PTDEVINFO ptInfo);
	void	Del(u32 dwDevID);
	void	Update(u32 dwDevID, PTDEVINFO ptPlan);
	PTDEVINFO	Get(u32 dwDevID);
	std::map<u32, PTDEVINFO>&	GetAll(){ return m_mapDevInfo; }

protected:
	//CMgrData*			m_pMgrData;
	std::map<u32, PTDEVINFO>	m_mapDevInfo;
};


