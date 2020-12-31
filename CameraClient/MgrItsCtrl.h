#pragma once
#include "stdafx.h"
#include "define.h"
#include "ItsCtrl.h"
#include "lock.h"
#include "MgrData.h"

class MgrItsCtrl :public CCriticalObject
{
public:
	MgrItsCtrl(void *pData);

	~MgrItsCtrl();

	void  Init();
	void  Release();
	ItsCtrl* GetCtrl(u32 dwIP);

	//
	u32 AddCtrl(u32 dwIP, u16 wPort, QString strName, QString strPassword);
	bool DelCtrl(u32 dwIP);
	void DelAll();

	void StopAll();
	void StopReceive();
	void StartReceive();
	void DebugMode();

private:
	std::map<u32, ItsCtrl*> m_mapItsCtrl;
	bool                      m_bInit;
};


