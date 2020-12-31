/*==============================================================================
ģ����      :  CMgrDev
�ļ���      :  MgrDev.h
����ļ�    :  MgrDev.cpp
�ļ�ʵ�ֹ���:  �豸����

����        :
��Ȩ        :  <Copyright(C) 2003-2010 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------
�޸ļ�¼    :
��  ��         �汾          �޸���          �߶���          �޸ļ�¼
2016/1/7       1.0           ���ǿ�                          ����
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
	��������: Init
	��    ��: ��ʼ��
	����˵��:
	�� �� ֵ: ��
	=================================================================*/
	void	Init();
	void    Release();

	/*=================================================================
	��������:
	��    ��: ��ӡ�ɾ�����޸�
	����˵��:
	�� �� ֵ: ��
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


