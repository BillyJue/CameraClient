
#pragma once
#include "stdafx.h"
#include "search_sdk_define.h"
#include "lock.h"
#include "subject.h"
#include <map>
//#include "MgrData.h"
class CMgrSearch:public CCriticalObject
{
public:
	CMgrSearch(void* pData);
	~CMgrSearch();

	void Init();
	void Release();
	static void MsgProCallBack (UINT wMsg, LPVOID pParam, DWORD dwContext);
	void AddItem(TDevItem *ptDevItem);
	bool IsInMap(TDevItem *ptDevItem);
	void ClearItem();
	TDevItem* GetItemById(u32 id);

	void SendDetectMsg(BOOL bSend);
	void RegisterCallback(CObserver* obj);
	void SetRun(BOOL bRun);
	bool IsRun();

protected:
	//MgrData*			            m_pMgrData;
	std::map<u32, TDevItem*> 	m_mapDevInfo;
	u32                                  m_nDevNum;
	CObserver*                      m_pObserver;
	bool                                 m_bRun;
};
