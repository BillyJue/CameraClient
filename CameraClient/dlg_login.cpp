#include "dlg_login.h"
#include <QRegExp>
#include <QRegExpValidator>
#include <QHeaderView>
#include "MgrData.h"
#include "define.h"
#include <QTreeWidgetItem>
#include <string.h>
#include "dlg_main.h"
#include <shellapi.h>
Dlg_Login::Dlg_Login(QWidget *parent)
	: DropShadowWidget(parent), CObserver()
{

	ui.setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint);
	ui.treeWidget->clear();
	/*QHeaderView *view = ui.treeWidget->header();
	view->setStyleSheet("QHeaderView{font-size: 14px;} \
						QHeaderView::section{ \
						color:white; padding: 4px; \
						height: 24px; border: 0px solid #567dbc; \
						border - left: 0px; border - right: 0px; \
						background: #2c3e50}");
*/
	
	Init();
	MgrData::getInstance()->GetMgrSubject()->Add(this);
	connect(ui.btn_research, SIGNAL(clicked()), this, SLOT(On_btnSearch_clicked()));
	connect(ui.minimizeButton, SIGNAL(clicked()), this, SLOT(Slot_TitleButtonClicked()));
	connect(ui.closeButton, SIGNAL(clicked()), this, SLOT(Slot_TitleButtonClicked()));
	connect(ui.btn_research, SIGNAL(clicked()), this, SLOT(On_btnSearch_clicked()));
	connect(ui.btn_login, SIGNAL(clicked()), this, SLOT(slot_login_clicked()));
	connect(ui.treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(slot_item_clicked(QTreeWidgetItem*, int)));
	connect(ui.treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(slot_item_doubleClicked(QTreeWidgetItem*, int)));
	//读取配置文件的用户信息
	QString sIp = MgrData::getInstance()->GetMgrUser()->GetIpAddr();
	wchar_t *sUsrName = MgrData::getInstance()->GetMgrUser()->GetCurUser()->szName;
	wchar_t *sUsrPwd = MgrData::getInstance()->GetMgrUser()->GetCurUser()->szPassword;
	QString sUsr = QString::fromWCharArray(sUsrName);
	QString sPwd = QString::fromWCharArray(sUsrPwd);
	ui.ledt_usr->setText(sUsr);
	
	ui.ledt_ip->setText(sIp);
	if (MgrData::getInstance()->GetMgrUser()->isSaveCfg())
	{
		ui.ledt_pwd->setText(sPwd);
		ui.checkBtn_remember->setChecked(true);
	}
	setWindowTitle(GBUTF8("ItsClient"));
}

void Dlg_Login::Init()
{
	MgrData::getInstance()->GetMgrSearch()->SetRun(TRUE);
	MgrData::getInstance()->GetMgrSearch()->RegisterCallback((CObserver*)this);
	MgrData::getInstance()->GetMgrSearch()->SendDetectMsg(true);
	
}

static u32 getIpAddress(QString strIpAddr)
{
#ifdef UNICODE 
	char psz[65];
	const wchar_t * strIp = strIpAddr.toStdWString().c_str();
	int len = WideCharToMultiByte(CP_ACP, 0, strIp, -1, NULL, 0, NULL, NULL);
	memset(psz, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, strIp, -1, psz, len, NULL, NULL);

	DWORD dwIpAddr = inet_addr(psz);
	dwIpAddr = htonl(dwIpAddr);
	return dwIpAddr;
#else
	DWORD dwIpAddr = inet_addr((char*)strIpAddr.toStdString().c_str());
	dwIpAddr = htonl(dwIpAddr);
	return dwIpAddr;
#endif

}

unsigned int IPtoINT(const std::string & strIP)
{
	unsigned int nRet = 0;

	char chBuf[16] = "";
	memcpy_s(chBuf, 15, strIP.c_str(), 15);

	char* szBufTemp = NULL;
	char* szBuf = strtok_s(chBuf, ".", &szBufTemp);

	int i = 0;//计数
	while (NULL != szBuf)//取一个
	{
		nRet += atoi(szBuf) << ((3 - i) * 8);
		szBuf = strtok_s(NULL, ".", &szBufTemp);
		i++;
	}

	return nRet;
}

void Dlg_Login::slot_item_clicked(QTreeWidgetItem *pItem, int column)
{
	QString sIp = pItem->data(1, Qt::DisplayRole).value<QString>();
	ui.ledt_ip->setText(sIp);
}

void Dlg_Login::slot_item_doubleClicked(QTreeWidgetItem *pItem, int column)
{
	QString sIp = pItem->data(1, Qt::DisplayRole).value<QString>();
	sIp = QString("http://%1").arg(sIp);
	ShellExecute(NULL, L"open", L"iexplore.exe", sIp.toStdWString().c_str(), NULL, SW_SHOW);
}

void Dlg_Login::Slot_TitleButtonClicked()
{
	if (this->sender() == ui.minimizeButton)
	{
		showMinimized();
	}
	else if (this->sender() == ui.closeButton)
	{
		qApp->quit();
	}
}

void Dlg_Login::slot_login_clicked()
{
	QString sIp = ui.ledt_ip->text();
	QString sUserName = ui.ledt_usr->text();
	QString sPwd = ui.ledt_pwd->text();
	if (ui.checkBtn_remember->isChecked())
	{
		MgrData::getInstance()->GetMgrUser()->SetSaveCfg(true);
	}
	else
	{
		MgrData::getInstance()->GetMgrUser()->SetSaveCfg(false);
	}
	MgrData::getInstance()->GetMgrUser()->SetCurUser(sUserName, sPwd);
	MgrData::getInstance()->GetMgrUser()->SetIpAddr(sIp);

	u16 wErr = 0;
	if (true)//m_pLayout_IP->IsVisible()
	{
		DWORD dwIpAddr = inet_addr((char*)sIp.toStdString().c_str());
		//dwIpAddr = htonl(dwIpAddr);
		wErr = MgrData::getInstance()->Login(dwIpAddr, WEBSERVERPORT, sUserName, sPwd, wErr);
	}
	else
	{
		wErr = MgrData::getInstance()->Login(sUserName, sPwd, wErr);
	}
	bool is = MgrData::getInstance()->IsLogin();
	if (is)
	{
		this->hide();
		Dlg_main dlg;
		if (dlg.exec() == 1)
		{
			this->show();
		}
		else
		{
			qApp->quit();
		}
		
	}
	else
	{
		Dlg_MessageBox dlg;
		dlg.SetInfoText(GBUTF8("用户名或密码错误!"));
		dlg.SetBtnNoVisible(false);
		dlg.exec();
	}
}

void Dlg_Login::On_btnSearch_clicked()
{
	ui.treeWidget->clear();
	MgrData::getInstance()->GetMgrSearch()->ClearItem();
	MgrData::getInstance()->GetMgrSearch()->SendDetectMsg(true);
	/*
	KillTimer((HWND)this->winId(), 3);
	SetTimer((HWND)this->winId(), 3, 2000, NULL);*/
}

Dlg_Login::~Dlg_Login()
{
	MgrData::getInstance()->GetMgrSearch()->SetRun(FALSE);
	MgrData::getInstance()->GetMgrSearch()->RegisterCallback(NULL);
	MgrData::getInstance()->GetMgrSubject()->Del(this);
}

void Dlg_Login::OnObserverNotify(LPARAM lHint, LPVOID pHint)
{
	//if (!IsRecvNotiyMsg())  return;

	u32 dwEvtType = (u32)lHint;
	u32 dwId = (u32)pHint;

	switch (dwEvtType)
	{
	case NMC_GET_DEV_INFO:
	{
		TDevItem* p = (TDevItem*)pHint;
		AddItem(p);
	}
	break;
	default:
		break;
	}

}
static void Ip2SubFields(DWORD dwIp,
	BYTE &byField0,
	BYTE &byField1,
	BYTE &byField2,
	BYTE &byField3)
{
	// ip格式: nField0:nField1:nField2:nField3
	byField0 = HIBYTE(HIWORD(dwIp));
	byField1 = LOBYTE(HIWORD(dwIp));
	byField2 = HIBYTE(LOWORD(dwIp));
	byField3 = LOBYTE(LOWORD(dwIp));
}

static bool char2wcharTemp(wchar_t* pwsz, const char* psz)
{
	int len = MultiByteToWideChar(CP_ACP, 0, psz, -1, NULL, 0);
	if (!pwsz)
	{
		return false;
	}
	memset(pwsz, 0, len + 1);
	MultiByteToWideChar(CP_ACP, 0, psz, -1, pwsz, len);

	return true;
}


void Dlg_Login::AddItem(TDevItem *ptDevItem)
{
	QTreeWidgetItem *item = new QTreeWidgetItem;
	item->setTextAlignment(0, Qt::AlignCenter);
	item->setText(0, QString(ptDevItem->tDevID.achDevName));
	ui.treeWidget->setMinimumHeight(50);
	BYTE nField0, nField1, nField2, nField3;
	Ip2SubFields(ptDevItem->tDevInfo.tAddr.dwIpaddr, nField0, nField1, nField2, nField3);
	char temp[32];
	sprintf(temp, "%d.%d.%d.%d", nField0, nField1, nField2, nField3);
	item->setTextAlignment(1, Qt::AlignCenter);
	item->setText(1, QString(temp));

	item->setTextAlignment(2, Qt::AlignCenter);
	item->setText(2, QString(ptDevItem->tDevID.achDevType));
	ui.treeWidget->addTopLevelItem(item);
}