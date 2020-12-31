#pragma once
//#include "stdafx.h"
#include "subject.h"
#include "mgr_search.h"
#include "mgr_usr.h"
#include "MgrItsCtrl.h"
#include "mgr_version.h"
#include "mgr_port.h"
#include "mgr_localcfg.h"
#include "mgr_video.h"
#include "mgr_evtlst.h"
#include "mgr_iosignal.h"
#include "mgr_ipcsdk.h"
#include "mgr_alarm.h"
#include "mgr_dev.h"
#include "mgr_mjpegctrl.h"
class MgrData
{
public:
	~MgrData();
	MgrData();

	static MgrData *getInstance();

	static QString GetUserPath();

	QString Int2IP(unsigned int ipInt);

	Uint32 IPstrToInt(QString ipStr);

	void init();

	void Notify(LPARAM lHint, LPVOID pHint) { m_cSubject.Notify(NULL, lHint, pHint); }
	void Notify(CObserver* p, LPARAM lHint, LPVOID pHint) { m_cSubject.Notify(p, lHint, pHint); }

	CSubject* GetMgrSubject(){ return &m_cSubject; }

	CMgrSearch* GetMgrSearch(){ return m_pMgrSearch; }

	CMgrUser* GetMgrUser(){ return m_mgrUser; }

	MgrItsCtrl*GetMgrItsCtrl(){ return m_mgrItsCtrl; }

	CMgrLocalCfg *GetMgrLocalCfg(){ return m_pMgrLocalCfg; }

	CMgrVersion*GetMgrVersion(){ return m_pMgrVersion; }

	CMgrPort *GetMgrPort(){ return m_pMgrPort; }

	CMgrVideo *GetMgrVideo(){ return m_pMgrVideo; }

	CMgrEvtLst *GetMgrEvtLst(){ return m_pMgrEvtLst; }

	CMgrIoSignal *GetMgrIoSignal(){ return m_pMgrIoSignal; }

	CMgrIpcSdk	*GetMgrIpcSdk(){ return m_pMgrIpcSdk; }

	CMgrDev		*GetMgrDev(){ return m_pMgrDev; }

	CMgrAlarm	*GetMgrAlarm(){ return m_pMgrAlarm; }

	CMgrMjpegCtrl *GetMgrMjpegCtrl(){ return m_pMgrMjpegCtrl; }

	TIPC_DevInfo*	GetDevInfo(){ return &m_tDevInfo; }

	Uint32			GetDevVersion(){ return m_tDevInfo.dwProtocol; }

	void SetDevInfo(TIPC_DevInfo *pDevInfo);

	void SetDataType(int nType){ m_nType = nType; }

	int	 GetDataType(){ return m_nType; }

	u16	Login(DWORD dwDestIP, WORD wPort, QString strName, QString strPassword, u16& wErr);

	u16	Login(QString strName, QString strPassword, u16& wErr);

	u32	GetIP(){ return m_dwDestIP; }

	void SetManual(bool b) { m_bManual = b; }
	//暂时还不全，等待后续再加进来
	void SetLogin(bool b);

	bool IsLogin(){ return m_bLogin; }

	void SetDevCap(TIPC_GetUICapsRsp *pUICap);
	TIPC_GetUICapsRsp*	GetDevCap(){ return &m_tUICap; }



private:
	CSubject		m_cSubject;
	CMgrSearch*     m_pMgrSearch;
	CMgrUser		*m_mgrUser;
	MgrItsCtrl		*m_mgrItsCtrl;
	CMgrVersion		*m_pMgrVersion;
	CMgrPort		*m_pMgrPort;
	CMgrLocalCfg	*m_pMgrLocalCfg;
	CMgrVideo		*m_pMgrVideo;
	CMgrEvtLst		*m_pMgrEvtLst;
	CMgrIoSignal	*m_pMgrIoSignal;
	CMgrIpcSdk		*m_pMgrIpcSdk;
	CMgrDev			*m_pMgrDev;
	CMgrAlarm		*m_pMgrAlarm;
	CMgrMjpegCtrl	*m_pMgrMjpegCtrl;
	static MgrData* m_mgr;

	TIPC_DevInfo	m_tDevInfo;
	DWORD			m_dwDestIP;
	DWORD			m_wPort;
	bool			m_bLogin;
	bool			m_bManual;
	int				m_nType;				// 数据类型
	
	TIPC_GetUICapsRsp m_tUICap;

};

