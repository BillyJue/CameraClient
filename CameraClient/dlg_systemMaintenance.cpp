#include "stdafx.h"
#include "dlg_systemMaintenance.h"
#include <QMessageBox>
#include "Dlg_MessageBox.h"
#include <QFileDialog>
#include <QFile>
#include <WinInet.h>
#include <QtConcurrent> 

TIPC_DevInfo			  m_tDevInfo2;
Dlg_systemMaintenance::Dlg_systemMaintenance(QWidget *parent)
	: MyWidget(parent), m_messageBox(nullptr)
{
	MgrData::getInstance()->GetMgrSubject()->Add(this);
	ui.setupUi(this);
	connect(ui.btn_restart, SIGNAL(clicked()), this, SLOT(slot_restart_clicked()));
	connect(ui.btn_easyRecovery, SIGNAL(clicked()), this, SLOT(slot_easyRecovery_clicked()));
	connect(ui.btn_exportConfig, SIGNAL(clicked()), this, SLOT(slot_exportConfig_clicked()));
	connect(ui.btn_loadConfig, SIGNAL(clicked()), this, SLOT(slot_loadConfig_clicked()));
	connect(ui.btn_systemUpdate, SIGNAL(clicked()), this, SLOT(slot_systemUpdate_clicked()));
}

bool Dlg_systemMaintenance::SaveCfg(TDevAllConfig* p)
{
	QFile sfile(m_wsPath);
	if (sfile.open(QIODevice::Truncate))
	{
		sfile.write(p->szData, p->dwSize);
		sfile.write(MgrData::getInstance()->GetDevInfo()->szDevType, 32);
		sfile.close();
	}

	return TRUE;
}

void Dlg_systemMaintenance::InitData(bool is)
{

}

void Dlg_systemMaintenance::OnObserverNotify(LPARAM lHint, LPVOID pHint)
{
	u32 dwEvtType = (u32)lHint;
	u32 dwId = (u32)pHint;
	switch (dwEvtType)
	{
	case NMC_GET_IPC_CONFIG:
	{
		TDevAllConfig* p = (TDevAllConfig*)pHint;
		if (p->dwSize != 0)
		{
			SaveCfg(p);
			BOOL bHandled = TRUE;
		}
	}
	break;
	case NMC_GET_IPC_DEVINFO:
	{
		TIPC_GetDevRsp* p = (TIPC_GetDevRsp*)pHint;
		if (p->nRetCode == 0)
		{
			memcpy(&m_tDevInfo2, &(p->tDev), sizeof(TIPC_DevInfo));

		}
	}
	break;
	case EV_SYSTEM_UPGRADE_ACK:
	{
		int *p = (int *)pHint;
		if (*p == -1)
		{
			MgrData::getInstance()->SetManual(false);
			if (m_messageBox)
			{
				m_messageBox->SetInfoText(GBUTF8("升级成功"));
				Sleep(500);
				m_messageBox->close();
				SAFE_DELETE(m_messageBox);
			}
		}
		else
		{
			if (m_messageBox)
			{
				m_messageBox->SetInfoText(GBUTF8("升级失败"));
				Sleep(500);
				m_messageBox->close();
				SAFE_DELETE(m_messageBox);
			}
		}
	}
	default:
		break;
	}

}

void Dlg_systemMaintenance::slot_restart_clicked()
{
	QMessageBox msgBox;
	msgBox.setText("IPC Control");
	msgBox.setInformativeText(GBUTF8("确定要重启吗"));
	//msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	msgBox.addButton(GBUTF8("确定"), QMessageBox::YesRole);
	msgBox.addButton(GBUTF8("取消"), QMessageBox::NoRole);
	int nRet = msgBox.exec();
	if (nRet == QMessageBox::Yes)
	{
		DWORD dIp = MgrData::getInstance()->GetIP();
		ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dIp);
		if (pCtrl == NULL)
		{
			return;
		}
		pCtrl->Restart();
		Sleep(1000);
		MgrData::getInstance()->GetMgrItsCtrl()->DelCtrl(dIp);
		MgrData::getInstance()->SetLogin(false);
		return;
	}

}
void Dlg_systemMaintenance::slot_easyRecovery_clicked()
{
	Dlg_MessageBox msgBox;
	msgBox.setWindowTitle("IPC Control");
	msgBox.SetInfoText(GBUTF8("设备需要重启以完成简单恢复！"));
	//msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	int nRet = msgBox.exec();
	if (nRet == 1)//0：确定 1取消
	{
		DWORD dIp = MgrData::getInstance()->GetIP();
		ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dIp);
		if (pCtrl == NULL)
		{
			return;
		}
		pCtrl->Reset();
		return;
	}

}
void Dlg_systemMaintenance::slot_exportConfig_clicked()
{
	QString sfileName = QFileDialog::getSaveFileName(this,
		GBUTF8("保存文件"),
		"",
		GBUTF8("tar.gz文件(*.tar.gz);;(*.tar.gz)"));

	m_wsPath = sfileName;
	DWORD dIp = MgrData::getInstance()->GetIP();
	ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dIp);
	if (pCtrl != NULL)
	{
		pCtrl->ExportCfg(NULL, (u32)(CObserver*)this);
	}

}
void Dlg_systemMaintenance::slot_loadConfig_clicked()
{
	QString sfileName = QFileDialog::getOpenFileName(this,
		GBUTF8("打开文件"),
		"",
		GBUTF8("tar.gz文件(*.tar.gz);;(*.tar.gz)"));

	QFile sFile(sfileName);
	if (!sFile.open(QIODevice::ReadOnly))
	{
		return;
	}
	int size = sFile.size();
	TDevAllConfig config;
	config.dwSize = size - 32;
	config.szData = new Char[size];
	memset(config.szData, 0, size);
	DWORD readSize = 0;
	sFile.read(config.szData, size);
	sFile.close();
	
	if (strcmp(config.szData + config.dwSize, MgrData::getInstance()->GetDevInfo()->szDevType) != 0)
	{
		delete config.szData;
		QMessageBox msgBox;
		msgBox.setWindowTitle(GBUTF8("错误"));
		msgBox.setStandardButtons(QMessageBox::Ok);
		msgBox.setText(GBUTF8("配置文件和相机不匹配"));
		//msgBox.addButton(GBUTF8("确定"), QMessageBox::Ok);
		msgBox.exec();
		return;
	}
	DWORD dIp = MgrData::getInstance()->GetIP();
	ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dIp);
	if (pCtrl != NULL)
	{
		pCtrl->ImportCfg(&config, (u32)this);
	}

	delete config.szData;
}

bool Dlg_systemMaintenance::OnCheckFileVersion()
{
	FILE *file = NULL;
	QByteArray arr = m_wsPath.toLatin1();
	char *fileName = arr.data();
	file = fopen(fileName, "rb");
	fseek(file, -128, SEEK_END);

	char ipcType[64];
	char softversion[64];
	fread(ipcType, 1, 64, file);
	fread(softversion, 1, 64, file);

	fseek(file, 0, SEEK_END);
	m_nFileLen = ftell(file);
	fclose(file);

	if (m_bPKG)
	{
		return true;
	}

	if (strcmp("IPC2255-Gi8N", m_tDevInfo2.szDevType) == 0
		|| strcmp("IPC2655-Gi8N", m_tDevInfo2.szDevType) == 0
		|| strcmp("ZD5920-Gi8N", m_tDevInfo2.szDevType) == 0)
	{
		memset(m_tDevInfo2.szDevType, 0, sizeof(m_tDevInfo2.szDevType));
		memcpy(m_tDevInfo2.szDevType, "IPC2X55-Gi8N", sizeof("IPC2X55-Gi8N"));
	}

	if (strncmp(ipcType, m_tDevInfo2.szDevType, strlen(ipcType)) == 0
		&& strlen(ipcType) >= 6
		&& strncmp(softversion, m_tDevInfo2.szSoftWareBaseVer, strlen(softversion)) == 0
		&& strlen(softversion) >= 8)
	{
		return true;
	}

	return false;
}

static void OnUpdate(void*pThis, void *ptr)
{
	Dlg_systemMaintenance *pSystem = (Dlg_systemMaintenance*)pThis;
	pSystem->SystemUpdate();
	if (pSystem->GetIsPkg())
	{
		Dlg_MessageBox *dlg = (Dlg_MessageBox*)ptr;
		Sleep(1000);
		if (MgrData::getInstance()->GetMgrIpcSdk()->GetSystemUpdateState() == 1)
		{
			dlg->SetInfoText(GBUTF8("升级成功"));
		}
		else
		{
			dlg->SetInfoText(GBUTF8("升级失败"));
		}
		dlg->close();
		SAFE_DELETE(ptr);
		MgrData::getInstance()->SetManual(false);
	}

	
	
}


void Dlg_systemMaintenance::slot_systemUpdate_clicked()
{
	QString sfileName = QFileDialog::getOpenFileName(this,
		GBUTF8("选择文件"),
		"",
		GBUTF8("升级文件(*.bz2;*.pkg);"));
	m_wsPath = sfileName;
	bool isPKG = false;
	isPKG = sfileName.contains(".pkg");
	m_bPKG = isPKG;
	bool nRet = OnCheckFileVersion();
	if (nRet == false)
	{
		QMessageBox msgBox;
		msgBox.setText(GBUTF8("错误"));
		msgBox.setStandardButtons(QMessageBox::Ok);
		msgBox.setInformativeText(GBUTF8("版本不匹配！"));
		msgBox.exec();
		return;
	}

	MgrData::getInstance()->SetManual(true);
	MgrData::getInstance()->GetMgrVideo()->StopPlayVideo();
	//创建个非模态对话框
	if (!m_messageBox)
	{
		m_messageBox = new Dlg_MessageBox;
	}
	m_messageBox->SetInfoText(GBUTF8("正在升级，请稍候！"));
	m_messageBox->setWindowTitle(GBUTF8("提示"));
	QFuture<void> future = QtConcurrent::run(OnUpdate, this, (void*)m_messageBox);
	m_messageBox->exec();
	//future.waitForFinished();
	//SystemUpdate();
	//MgrData::getInstance()->SetManual(false);
}



void  Dlg_systemMaintenance::SystemUpdate()
{

	if (m_bPKG)
	{
		QByteArray arr = m_wsPath.toLatin1();
		char *szFilePath = arr.data();
		BOOL32 nRet = MgrData::getInstance()->GetMgrIpcSdk()->SystemUpdate(szFilePath, m_nFileLen);
		if (nRet == RET_SUCCESS)
		{
			//SetTimer(this->GetHWND(), 1234, 240000, NULL);
			//SetTimer(this->GetHWND(), 1236, 1000, NULL);
		}
		else
		{
			//SetTimer(this->GetHWND(), 1235, 1000, NULL);
		}
	}
	else
	{
		OnFtpFile();
	}

}

bool Dlg_systemMaintenance::OnFtpFile()
{
	HINTERNET hInternet = NULL;
	HINTERNET hFtpSession = NULL;

	hInternet = InternetOpen(NULL, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);

	wchar_t ip[32];
	in_addr ipaddr;
	ipaddr.S_un.S_addr = MgrData::getInstance()->GetIP();
	char2wchar(ip, inet_ntoa(ipaddr));

	hFtpSession = InternetConnect(hInternet, ip, INTERNET_DEFAULT_FTP_PORT, MgrData::getInstance()->GetMgrUser()->GetCurUser()->szName,
		MgrData::getInstance()->GetMgrUser()->GetCurUser()->szPassword, INTERNET_SERVICE_FTP, 0, 0);

	wchar_t	wsPatha[260];
	QByteArray pathArr = m_wsPath.toLatin1();
	char *pathChar = pathArr.data();
	char2wchar(wsPatha, pathChar);
	FtpDeleteFile(hFtpSession, TEXT("/storage/sdcard0/update.bz2"));
	int nRet = FtpPutFile(hFtpSession, wsPatha, TEXT("/storage/sdcard0/update.bz2"), INTERNET_FLAG_TRANSFER_BINARY, NULL);

	InternetCloseHandle(hFtpSession);
	InternetCloseHandle(hInternet);

	Sleep(1000);

	if (nRet)
	{
		MgrItsCtrl* pMgrItsCtrl = MgrData::getInstance()->GetMgrItsCtrl();
		if (pMgrItsCtrl != NULL)
		{
			DWORD dIp = MgrData::getInstance()->GetIP();
			ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dIp);
			if (pCtrl != NULL)
			{
				pCtrl->PostTransparentMsg(&m_nFileLen, sizeof(m_nFileLen), EV_SYSTEM_UPGRADE_REQ, (u32)(CObserver*)this);
			}
		}

		//SetTimer(this->GetHWND(), 1234, 120000, NULL);

		return true;
	}

	return false;
}


Dlg_systemMaintenance::~Dlg_systemMaintenance()
{
	MgrData::getInstance()->GetMgrSubject()->Del(this);
	SAFE_DELETE(m_messageBox);
}
