#include "stdafx.h"
#include "mgr_usr.h"
#include <QDir>

MgrData *m_pMgrData2 = nullptr;
/*
static QString GetUserPath()
{
	QDir tempDir;
	QString sPath = QDir::currentPath() + "/itsclient/";
	if (!tempDir.exists(sPath))
	{
		tempDir.mkpath(sPath);
	}
	return sPath;
}
*/
CMgrUser::CMgrUser(void *pData)
{
	m_pMgrData2 = (MgrData *)pData;
	m_tCfgFilePath.clear();
	Init();
}


CMgrUser::~CMgrUser()
{
	Release();
}


void CMgrUser::Init()
{
	m_bSaveCfg = false;
	m_bAutoLogin = false;

	m_tCfgFilePath = m_pMgrData2->GetUserPath();
	m_tCfgFilePath += "cfg.ini";
	GetPrivateProfileStringW(TEXT("user"), TEXT("name"), TEXT(""), m_tUserInfo.szName, USERMAXLEN, m_tCfgFilePath.toStdWString().c_str());
	GetPrivateProfileStringW(TEXT("user"), TEXT("password"), TEXT(""), m_tUserInfo.szPassword, PASSWORDMAXLEN, m_tCfgFilePath.toStdWString().c_str());
	wchar_t		szIpAddr[33];	// ÓÃ»§Ãû
	GetPrivateProfileStringW(TEXT("user"), TEXT("ipaddr"), TEXT(""), szIpAddr, PASSWORDMAXLEN, m_tCfgFilePath.toStdWString().c_str());
	m_strIpAddr = QString::fromStdWString(szIpAddr);
	m_bSaveCfg = (bool)GetPrivateProfileIntW(TEXT("user"), TEXT("issave"), 0, m_tCfgFilePath.toStdWString().c_str());
	m_bAutoLogin = (bool)GetPrivateProfileIntW(TEXT("user"), TEXT("isautologin"), 0, m_tCfgFilePath.toStdWString().c_str());
}

void CMgrUser::Release()
{

}

void	CMgrUser::Load(PTLSTUSERINFO plstUserInfo)
{
	plstUserInfo = NULL;
}
void	CMgrUser::Save(PTLSTUSERINFO plstUserInfo)
{

}
void	CMgrUser::ReaseLstInfo(PTLSTUSERINFO plstUserInfo)
{
	if (plstUserInfo == NULL)
		return;

	TLSTUSERINFOITE ite = plstUserInfo->begin();
	while (ite != plstUserInfo->end())
	{
		if (*ite != NULL)
		{
			delete *ite;
			*ite = NULL;
		}
	}

	plstUserInfo->clear();
}

void	CMgrUser::Add(PTUSERINFO_ITS ptInfo)
{
	//CCriticalLock lock(_pcs);


}
void	CMgrUser::Del(PTUSERINFO_ITS ptInfo)
{
	//CCriticalLock lock(_pcs);


}
void	CMgrUser::Update(PTUSERINFO_ITS ptInfo)
{
	//CCriticalLock lock(_pcs);


}

void	CMgrUser::SetCurUser(QString strName, QString strPassword)
{
	//CCriticalLock lock(_pcs);

	m_tUserInfo.empty();
	wcscpy(m_tUserInfo.szName, strName.toStdWString().c_str());
	wcscpy(m_tUserInfo.szPassword, strPassword.toStdWString().c_str());

	if (m_bSaveCfg)
	{
		WritePrivateProfileStringW(TEXT("user"), TEXT("name"), m_tUserInfo.szName, m_tCfgFilePath.toStdWString().c_str());
		WritePrivateProfileStringW(TEXT("user"), TEXT("password"), m_tUserInfo.szPassword, m_tCfgFilePath.toStdWString().c_str());
	}
}

void	CMgrUser::SetCurUser(PTUSERINFO_ITS p)
{
	//CCriticalLock lock(_pcs);

	memcpy(&m_tUserInfo, p, sizeof(TUSERINFO_ITS));

	if (m_bSaveCfg)
	{
		WritePrivateProfileStringW(TEXT("user"), TEXT("name"), m_tUserInfo.szName, m_tCfgFilePath.toStdWString().c_str());
		WritePrivateProfileStringW(TEXT("user"), TEXT("password"), m_tUserInfo.szPassword, m_tCfgFilePath.toStdWString().c_str());
	}
}

void	CMgrUser::SetSaveCfg(bool	bSaveCfg)
{
	//CCriticalLock lock(_pcs);

	m_bSaveCfg = bSaveCfg;
	wchar_t	szTmp[33];
	wsprintf(szTmp, TEXT("%d"), bSaveCfg);

	WritePrivateProfileStringW(TEXT("user"), TEXT("issave"), szTmp, m_tCfgFilePath.toStdWString().c_str());

}

void	CMgrUser::SetAutoLogin(bool	bAutoLogin)
{
	//CCriticalLock lock(_pcs);

	m_bAutoLogin = bAutoLogin;
	wchar_t	szTmp[33];
	wsprintf(szTmp, TEXT("%d"), bAutoLogin);

	WritePrivateProfileStringW(TEXT("user"), TEXT("isautologin"), szTmp, m_tCfgFilePath.toStdWString().c_str());
}

void	CMgrUser::SetIpAddr(QString strIpAddr)
{
	//CCriticalLock lock(_pcs);

	m_strIpAddr = strIpAddr;

	if (m_bSaveCfg)
	{
		WritePrivateProfileStringW(TEXT("user"), TEXT("ipaddr"), m_strIpAddr.toStdWString().c_str(), m_tCfgFilePath.toStdWString().c_str());
	}
}
