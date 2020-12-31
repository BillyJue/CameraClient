/*****************************************************************************
模块名  : NOTIFY消息处理
文件名  : subject.h
相关文件: 
实现功能: 改写NOTIFY类
作者    : 刘登科
版本    : KDM-V5.0-R1.0 Copyright (C) 2008 - All Rights Reserved
-----------------------------------------------------------------------------
修改记录:
日  期            版本      修改人        走读人      修改内容
20150930          v1.0      刘登科                    创建
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

