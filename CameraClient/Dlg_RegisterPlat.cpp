#include "Dlg_RegisterPlat.h"
#include "MgrData.h"
#include <QTimer>
Dlg_RegisterPlat::Dlg_RegisterPlat(QWidget *parent)
	: MyWidget(parent), m_platComponent(nullptr), 
	m_nExpandIndex(-1), m_nIndex(-1), m_messageBox(nullptr)
{
	ui.setupUi(this);
	MgrData::getInstance()->GetMgrSubject()->Add(this);
	//GetParam();
	connect(this, SIGNAL(data_changed()), this, SLOT(Slot_DataChanged()));
	connect(ui.btn_save, SIGNAL(clicked()), this, SLOT(Slot_BtnSaveClicked()));
	connect(ui.btn_fresh, SIGNAL(clicked()), this, SLOT(Slot_BtnFreshClicked()));
}

void Dlg_RegisterPlat::Slot_BtnSaveClicked()
{
	bool bRet = false;
	u32 dwIp = MgrData::getInstance()->GetIP();
	ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dwIp);
	if (pCtrl != NULL)
	{
		bRet = pCtrl->SetSysCfgPlate(&m_tData);
	}
	if (!m_messageBox)
	{
		QTimer::singleShot(1500, this, &Dlg_RegisterPlat::OnSetTxtVisible);
		m_messageBox = new Dlg_MessageBox;
		m_messageBox->SetInfoText(GBUTF8("保存成功！"));
		m_messageBox->SetBtnNoVisible(false);
		m_messageBox->exec();
	}

}

void Dlg_RegisterPlat::InitData(bool is)
{
	if (is)
	{
		GetParam();
	}
}

void Dlg_RegisterPlat::Slot_BtnFreshClicked()
{
	GetParam();
}

Dlg_RegisterPlat::~Dlg_RegisterPlat()
{
	SAFE_DELETE(m_messageBox);
	MgrData::getInstance()->GetMgrSubject()->Del(this);
	if (m_platComponent)
	{
		delete m_platComponent;
		m_platComponent = nullptr;
	}
}

void Dlg_RegisterPlat::GetParam()
{
	bool bRet = false;
	ItsCtrl* pCtrl = NULL;
	Uint32 dwIP = MgrData::getInstance()->GetIP();
	pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dwIP);
	if (pCtrl != NULL)
	{
		bRet = pCtrl->GetSysCfgPlate(NULL, (u32)(CObserver*)this);
	}

}

void Dlg_RegisterPlat::Slot_DataChanged()
{
	while (ui.verticalLayout->count() > 0)
	{
		QLayoutItem*pItem = ui.verticalLayout->takeAt(0);
		SAFE_DELETE(pItem);
	}
	SAFE_DELETE(m_platComponent);
	for (int i = 0; i < m_LabMap.size(); i++)
	{
		SAFE_DELETE(m_LabMap[i]);
	}
	m_nExpandIndex = -1;
	m_nIndex = -1;
	//if (m_nExpandIndex != m_nIndex)
	//{
	//	SetPlatData(m_nExpandIndex);
	//
	//	for (int i = 0; i < m_LabMap.size(); i++)
	//	{
	//		if (!m_LabMap[i])
	//		{
	//			continue;
	//		}
	//		int nIdx = m_LabMap[i]->property("index").toInt();
	//		QString sIP = MgrData::getInstance()->Int2IP(m_tData.tServer[nIdx].dwIpAddr);
	//		int nPort = m_tData.tServer[nIdx].wPort;
	//		m_LabMap[i]->SetCheckedStatus(m_tData.tServer[nIdx].bEnable);
	//		m_LabMap[i]->SetLabIP(sIP);
	//		m_LabMap[i]->SetLabPort(QString("%1").arg(nPort));
	//		m_LabMap[i]->SetProtocolName(m_tData.tServer[nIdx].byServerType);
	//	}
	//}
	//else
	{
		ReLayout(-1);
	}
	
}

void Dlg_RegisterPlat::OnObserverNotify(LPARAM lHint, LPVOID pHint)
{
	if (lHint == NMC_GET_SVR_PLATEINFO)
	{
		TITS_GetServerRsp* p = (TITS_GetServerRsp*)pHint;
		if (p->nRetCode)//nRetCode非0表示发生错误
		{
			return;
		}
		m_tData.wServerSize = p->wServerSize;
		memcpy(m_tData.tServer, p->tServer, sizeof(TITS_CenterServer)*MAX_NUM_SERVER);
		emit data_changed();
	}
}

void Dlg_RegisterPlat::ReLayout(int nIndex)
{
	for (int i = 0; i < m_tData.wServerSize; i++)//4
	{
		if (nIndex == i)
		{
			if (m_platComponent == nullptr)
			{
				m_platComponent = new Dlg_PlatComponent;
			}
			ui.verticalLayout->addWidget(m_platComponent);
			m_platComponent->setProperty("index", i);
			connect(m_platComponent, SIGNAL(signal_btnClicked()), this, SLOT(Slot_BtnHidenClicked()));
			if (nIndex != -1)
			{
				SetPlatData(i);
				//设置数据
			}
		}
		else
		{
			Dlg_PlatLabComponent *lab1 = new Dlg_PlatLabComponent;
			ui.verticalLayout->addWidget(lab1);
			lab1->setProperty("index", i);
			connect(lab1, SIGNAL(signal_expandClicked()), this, SLOT(Slot_BtnClicked()));
			m_LabMap.insert(i, lab1);
			//if (nIndex > 0)
			{
				QString sIP = MgrData::getInstance()->Int2IP(m_tData.tServer[i].dwIpAddr);
				int nPort = m_tData.tServer[i].wPort;
				lab1->SetCheckedStatus(m_tData.tServer[i].bEnable);
				lab1->SetLabIP(sIP);
				lab1->SetLabPort(QString("%1").arg(nPort));
				lab1->SetProtocolName(m_tData.tServer[i].byServerType);
				//设置数据
			}

		}
	}
	QSpacerItem *teverticalSpacer = new QSpacerItem(20, 143, QSizePolicy::Minimum, QSizePolicy::Expanding);
	ui.verticalLayout->addItem(teverticalSpacer);
}

void Dlg_RegisterPlat::SetPlatData(int i, bool isFromPage)
{
	if (isFromPage)
	{
		if (i == -1 || (!m_platComponent))
		{
			return;
		}
		bool isEnable = m_platComponent->GetCheckEnable();
		Uint32 dwIp = MgrData::getInstance()->IPstrToInt(m_platComponent->GetEditIP());
		Uint16 dwPort = m_platComponent->GetEditPort().toInt();
		Uint8 dwServerType = m_platComponent->GetProCmbIndex() + 1;
		m_tData.tServer[i].bEnable = isEnable;
		m_tData.tServer[i].dwIpAddr = dwIp;
		m_tData.tServer[i].wPort = dwPort;
		m_tData.tServer[i].byServerType = dwServerType;
		QByteArray uuidArr = m_platComponent->GetUUID().toLatin1();
		char *pUuid = uuidArr.data();
		ZeroMemory(m_tData.tServer[i].szPuId, 64);
		memcpy(m_tData.tServer[i].szPuId, pUuid, strlen(pUuid));

		QByteArray usrArr = m_platComponent->GetEditUsrName().toLatin1();
		char *pUsr = usrArr.data();
		ZeroMemory(m_tData.tServer[i].szUsername, 32);
		memcpy(m_tData.tServer[i].szUsername, pUsr, strlen(pUsr));

		QByteArray pwdArr = m_platComponent->GetEditPassword().toLatin1();
		char *pwd = pwdArr.data();
		ZeroMemory(m_tData.tServer[i].szPassword, 32);
		memcpy(m_tData.tServer[i].szPassword, pwd, strlen(pwd));

		QByteArray ruleArr = m_platComponent->GetEditRule().toLatin1();
		char *pRule = ruleArr.data();
		ZeroMemory(m_tData.tServer[i].szRegulation, 512);
		memcpy(m_tData.tServer[i].szRegulation, pRule, strlen(pRule));

		QByteArray saveArr = m_platComponent->GetEditOverSavePath().toLatin1();
		char *pSave = saveArr.data();
		ZeroMemory(m_tData.tServer[i].szSavePath, 256);
		memcpy(m_tData.tServer[i].szSavePath, pSave, strlen(pSave));

		QByteArray illgalArr = m_platComponent->GetEditIllegalPath().toLatin1();
		char *pIllegal = illgalArr.data();
		ZeroMemory(m_tData.tServer[i].szIllegalDir, 256);
		memcpy(m_tData.tServer[i].szIllegalDir, pIllegal, strlen(pIllegal));

	}
	else
	{
		m_platComponent->SetEnableCheck(m_tData.tServer[i].bEnable);
		m_platComponent->SetEditIP(MgrData::getInstance()->Int2IP(m_tData.tServer[i].dwIpAddr));
		m_platComponent->SetEditPort(QString("%1").arg(m_tData.tServer[i].wPort));
		m_platComponent->SetProCmbIndex(m_tData.tServer[i].byServerType - 1);
		m_platComponent->SetReportedChecked(m_tData.tServer[i].byServerType);
		wchar_t wchars[512];
		ZeroMemory(wchars, 512);
		char2wchar(wchars, m_tData.tServer[i].szPuId);
		m_platComponent->SetUUID(QString::fromWCharArray(wchars));

		ZeroMemory(wchars, 512);
		char2wchar(wchars, m_tData.tServer[i].szUsername);
		m_platComponent->SetEditUsrName(QString::fromWCharArray(wchars));

		ZeroMemory(wchars, 512);
		char2wchar(wchars, m_tData.tServer[i].szPassword);
		m_platComponent->SetEditUsrName(QString::fromWCharArray(wchars));

		ZeroMemory(wchars, 512);
		char2wchar(wchars, m_tData.tServer[i].szRegulation);
		m_platComponent->SetEditRule(QString::fromWCharArray(wchars));

		ZeroMemory(wchars, 512);
		char2wchar(wchars, m_tData.tServer[i].szSavePath);
		m_platComponent->SetEditOverSavePath(QString::fromWCharArray(wchars));

		ZeroMemory(wchars, 512);
		char2wchar(wchars, m_tData.tServer[i].szIllegalDir);
		m_platComponent->SetEditIllegalPath(QString::fromWCharArray(wchars));

		m_platComponent->SetUFT8Checked(m_tData.tServer[i].bUtf8Encode);
	}
	
}


void Dlg_RegisterPlat::Slot_BtnHidenClicked()
{
	//保存数据
	SetPlatData(m_nExpandIndex, true);
	Dlg_PlatComponent *lab = dynamic_cast<Dlg_PlatComponent*>(this->sender());
	int nIndex = lab->property("index").toInt();
	m_nIndex = nIndex;

	Dlg_PlatLabComponent *com = new Dlg_PlatLabComponent;
	QString sIP = MgrData::getInstance()->Int2IP(m_tData.tServer[nIndex].dwIpAddr);
	int nPort = m_tData.tServer[nIndex].wPort;
	com->SetCheckedStatus(m_tData.tServer[nIndex].bEnable);
	com->SetLabIP(sIP);
	com->SetLabPort(QString("%1").arg(nPort));
	com->SetProtocolName(m_tData.tServer[nIndex].byServerType);

	ui.verticalLayout->replaceWidget(lab, com);
	delete lab;
	lab = nullptr;
	m_platComponent = lab;
	com->setProperty("index", nIndex);
	connect(com, SIGNAL(signal_expandClicked()), this, SLOT(Slot_BtnClicked()));
	m_LabMap[nIndex] = com;
}

void Dlg_RegisterPlat::OnSetTxtVisible()
{
	if (m_messageBox)
	{
		m_messageBox->close();
		SAFE_DELETE(m_messageBox);
	}
}


void Dlg_RegisterPlat::Slot_BtnClicked()
{
	//保存数据
	SetPlatData(m_nExpandIndex, true);
	Dlg_PlatLabComponent *lab = dynamic_cast<Dlg_PlatLabComponent*>(this->sender());
	int nIndex = lab->property("index").toInt();
	//点的另外一个扩展
	if ((m_nExpandIndex != nIndex) && (m_nExpandIndex != -1))
	{
		if (m_platComponent)
		{
			int nIdx = m_platComponent->property("index").toInt();
			Dlg_PlatLabComponent *com = new Dlg_PlatLabComponent;
			com->setProperty("index", nIdx);
			m_LabMap[nIdx] = com;
			connect(com, SIGNAL(signal_expandClicked()), this, SLOT(Slot_BtnClicked()));
			ui.verticalLayout->replaceWidget(m_platComponent, com);
			delete m_platComponent;
			m_platComponent = nullptr;
		}
	}
	m_nExpandIndex = nIndex;

	m_LabMap[m_nExpandIndex] = nullptr;
	m_platComponent = new Dlg_PlatComponent;
	for (int i = 0; i < ui.verticalLayout->count(); i++)
	{
		QLayoutItem*pItem = ui.verticalLayout->takeAt(i);
	}
	for (int i = 0; i < m_LabMap.size(); i++)
	{
		if (i == nIndex)
		{
			ui.verticalLayout->addWidget(m_platComponent);
		}
		else
		{
			ui.verticalLayout->addWidget(m_LabMap[i]);
		}
	}
	SAFE_DELETE(lab);
	QSpacerItem *teverticalSpacer = new QSpacerItem(20, 143, QSizePolicy::Minimum, QSizePolicy::Expanding);
	ui.verticalLayout->addItem(teverticalSpacer);
	/*ui.verticalLayout->replaceWidget(lab, m_platComponent);
	ui.verticalLayout->removeWidget(lab);
	lab->deleteLater();*/

	m_platComponent->setProperty("index", nIndex);
	connect(m_platComponent, SIGNAL(signal_btnClicked()), this, SLOT(Slot_BtnHidenClicked()));
	SetPlatData(nIndex);
}
