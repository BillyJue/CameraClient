/*****************************************************************************
ģ����  : NOTIFY��Ϣ����
�ļ���  : subject.h
����ļ�: 
ʵ�ֹ���: ��дNOTIFY��
����    : ���ǿ�
�汾    : KDM-V5.0-R1.0 Copyright (C) 2008 - All Rights Reserved
-----------------------------------------------------------------------------
�޸ļ�¼:
��  ��            �汾      �޸���        �߶���      �޸�����
20150930          v1.0      ���ǿ�                    ����
******************************************************************************/
#pragma once
//#include <windows.h>
#include <windows.h>
#include "kdvtype.h"
#include <list>
//#include <algorithm>
#include <stdio.h>

class CObserver
{
public:
	CObserver(void){}
	virtual ~CObserver(void){}
	virtual void OnObserverNotify(LPARAM lHint, LPVOID pHint){}
};


class CSubject
{
public:
	CSubject(std::list<CObserver*>* pObservers = NULL)
	{
		m_pObservers = pObservers;
		if (pObservers == NULL)
		{
			m_pObservers = new std::list<CObserver*>;
		}
	}
	virtual ~CSubject(void)
	{
		if (m_pObservers == NULL)
			return;

		m_pObservers->clear(); 
		delete m_pObservers;
		m_pObservers = NULL;
	}

	virtual void Add(CObserver* po)
	{
		if (m_pObservers == NULL)
			return;

		if (std::find(m_pObservers->begin(), m_pObservers->end(), po) == m_pObservers->end())
		{
			m_pObservers->push_back(po);
		}
	}
	virtual void Del(CObserver* po)
	{
		if (m_pObservers == NULL)
			return;

		if (std::find(m_pObservers->begin(), m_pObservers->end(), po) != m_pObservers->end())
		{
			m_pObservers->remove(po);
		}
	}
	void Notify(CObserver* pSender, LPARAM lHint = 0L, LPVOID pHint = NULL)  
	{
		if (m_pObservers == NULL)
			return;

		std::list<CObserver*>::iterator ite;
		ite = m_pObservers->begin();
		while(ite != m_pObservers->end())
		{
			CObserver* po = *(ite);
			if ((pSender == NULL || po == pSender )&&
				(po != NULL))
			{
				po->OnObserverNotify(lHint, pHint);
			}
			ite++;
		}
	}
	void ClearObservers()
	{
		if (m_pObservers == NULL)
			return;
		m_pObservers->clear();
	}

	std::list<CObserver*>* GetObservers()
	{
		return m_pObservers;
	}


protected:
	std::list<CObserver*>* m_pObservers;
};

