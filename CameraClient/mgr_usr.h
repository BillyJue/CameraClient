#pragma once
#include "stdafx.h"
//#include "lock.h"
#include "define.h"
#include <QString>

class CMgrUser
{
public:
	CMgrUser(void *pData);
	~CMgrUser();

	void	Init();
	void    Release();

	void	Load(PTLSTUSERINFO plstUserInfo);
	void	Save(PTLSTUSERINFO plstUserInfo);
	void	ReaseLstInfo(PTLSTUSERINFO plstUserInfo);
	void	Add(PTUSERINFO_ITS ptInfo);
	void	Del(PTUSERINFO_ITS ptInfo);
	void	Update(PTUSERINFO_ITS ptInfo);

	PTUSERINFO_ITS	GetCurUser(){ return &m_tUserInfo; };
	void	SetCurUser(PTUSERINFO_ITS p);
	void	SetCurUser(QString strName, QString strPassword);
	bool	isSaveCfg(){ return m_bSaveCfg; }
	void	SetSaveCfg(bool	bSaveCfg);
	bool	isAutoLogin(){ return m_bAutoLogin; }
	void	SetAutoLogin(bool	bAutoLogin);
	QString	GetIpAddr(){ return m_strIpAddr; }
	void	SetIpAddr(QString strIpAddr);


protected:
	//CMgrData*				m_pMgrData;
	TUSERINFO_ITS			m_tUserInfo;
	QString				    m_strIpAddr;
	bool				    m_bSaveCfg;
	bool				    m_bAutoLogin;
	QString                 m_tCfgFilePath;
};

