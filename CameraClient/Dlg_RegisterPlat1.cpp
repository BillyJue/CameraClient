#include "Dlg_RegisterPlat.h"
#include "MgrData.h"
Dlg_RegisterPlat::Dlg_RegisterPlat(QWidget *parent)
	: QWidget(parent), m_platComponent(nullptr)
{
	ui.setupUi(this);
	MgrData::getInstance()->GetMgrSubject()->Add(this);
	GetParam();
	connect(this, SIGNAL(data_changed()), this, SLOT(Slot_DataChanged()));
	//ReLayout(-1);
}

Dlg_RegisterPlat::~Dlg_RegisterPlat()
{
	MgrData::getInstance()->GetMgrSubject()->Del(this);
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
	ReLayout(-1);
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
	m_LabVector.clear();
	for (int i = 0; i < 4; i++)//4
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
				//设置数据
			}
		}
		else
		{
			Dlg_PlatLabComponent *lab1 = new Dlg_PlatLabComponent(this);
			ui.verticalLayout->addWidget(lab1);
			lab1->setProperty("index", i);
			connect(lab1, SIGNAL(signal_expandClicked()), this, SLOT(Slot_BtnClicked()));
			m_LabVector.append(lab1);
			if (nIndex > 0)
			{
				//设置数据
			}

		}
	}
	QSpacerItem *teverticalSpacer = new QSpacerItem(20, 143, QSizePolicy::Minimum, QSizePolicy::Expanding);
	ui.verticalLayout->addItem(teverticalSpacer);
}

void Dlg_RegisterPlat::SetCurrentPlatConfig()
{
	if (m_nIndex == -1)
	{
		return;
	}
	
}

void Dlg_RegisterPlat::Slot_BtnHidenClicked()
{
	SetCurrentPlatConfig();
	Dlg_PlatComponent *lab = dynamic_cast<Dlg_PlatComponent*>(this->sender());
	int nIndex = lab->property("index").toInt();
	m_nIndex = nIndex;
	Dlg_PlatLabComponent *dlg_Lab = new Dlg_PlatLabComponent;
	dlg_Lab->setProperty("index", nIndex);
	connect(dlg_Lab, SIGNAL(signal_expandClicked()), this, SLOT(Slot_BtnClicked()));
	ui.verticalLayout->replaceWidget(lab, dlg_Lab);
	lab->deleteLater();
	//ReLayout(-1);
	/*{
		m_LabVector[nIndex]->SetLabIP(lab->GetEditIP());
		m_LabVector[nIndex]->SetLabPort(lab->GetEditPort());
		m_LabVector[nIndex]->SetCheckedStatus(lab->GetCheckEnable());
	}*/
}


void Dlg_RegisterPlat::Slot_BtnClicked()
{
	SetCurrentPlatConfig();
	Dlg_PlatLabComponent *lab = dynamic_cast<Dlg_PlatLabComponent*>(this->sender());
	int nIndex = lab->property("index").toInt();
	if (m_platComponent)
	{
		m_platComponent->setParent(NULL);
		delete m_platComponent;
		m_platComponent = nullptr;
	}
	m_platComponent = new Dlg_PlatComponent;
	m_platComponent->setProperty("index", nIndex);
	connect(m_platComponent, SIGNAL(signal_btnClicked()), this, SLOT(Slot_BtnHidenClicked()));
	ui.verticalLayout->replaceWidget(lab, m_platComponent);
	lab->deleteLater();
	/*m_platComponent->SetCheckEnable(m_getServerRsp->tServer[nIndex].bEnable);
	m_platComponent->SetUUID(QString("%1").arg(m_getServerRsp->tServer[nIndex].szPuId));
	m_platComponent->SetEditUsrName(QString("%1").arg(m_getServerRsp->tServer[nIndex].szUsername));
	m_platComponent->SetEditPassword(QString("%1").arg(m_getServerRsp->tServer[nIndex].szPassword));
	m_platComponent->SetEditIP(Mgr_Data::getInstance()->IPIntToStr(m_getServerRsp->tServer[nIndex].dwIpAddr));
	m_platComponent->SetEditPort(QString("%1").arg(m_getServerRsp->tServer[nIndex].wPort));
	m_platComponent->SetEditSavePath(QString("%1").arg(m_getServerRsp->tServer[nIndex].szSavePath));
	m_platComponent->SetEditRule(QString("%1").arg(m_getServerRsp->tServer[nIndex].szRegulation));*/

}
