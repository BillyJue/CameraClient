#include "Dlg_PowerSyn.h"
#include "MgrData.h"
#include <QTimer>
#include <QListView>
Dlg_PowerSyn::Dlg_PowerSyn(QWidget *parent)
	: MyWidget(parent), m_messageBox(nullptr)
{
	ui.setupUi(this);
	MgrData::getInstance()->GetMgrSubject()->Add(this);
	connect(this, SIGNAL(dataChanged()), this, SLOT(slot_dataChanged()));
	connect(ui.btn_save, SIGNAL(clicked()), this, SLOT(slot_saveClicked()));
	connect(ui.bnt_save2, SIGNAL(clicked()), this, SLOT(slot_save2Clicked()));
	connect(ui.com_type, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_comboIndexChanged(int)));
	ui.com_type->addItem(GBUTF8("自动相位"));
	ui.com_type->addItem(GBUTF8("固定相位"));
	ui.com_type->setView(new QListView);
	//GetParam();
	
}

void Dlg_PowerSyn::InitData(bool is)
{
	if (is)
	{
		GetParam();
	}
}


void Dlg_PowerSyn::slot_comboIndexChanged(int nIndex)
{
	if (nIndex == 0)
	{
		ui.stackedWidget->setCurrentIndex(0);
	}
	else
	{
		ui.stackedWidget->setCurrentIndex(1);
	}
}

void Dlg_PowerSyn::OnSetTxtVisible()
{
	if (m_messageBox)
	{
		m_messageBox->close();
		SAFE_DELETE(m_messageBox);
	}
}

void Dlg_PowerSyn::slot_saveClicked()
{
	SaveConfig();
	SetParam();
	if (!m_messageBox)
	{
		QTimer::singleShot(1500, this, &Dlg_PowerSyn::OnSetTxtVisible);
		m_messageBox = new Dlg_MessageBox;
		m_messageBox->SetInfoText(GBUTF8("保存成功！"));
		m_messageBox->SetBtnNoVisible(false);
		m_messageBox->exec();
	}
}

void Dlg_PowerSyn::slot_save2Clicked()
{
	SaveConfig();
	SetParam();
	if (!m_messageBox)
	{
		QTimer::singleShot(1500, this, &Dlg_PowerSyn::OnSetTxtVisible);
		m_messageBox = new Dlg_MessageBox;
		m_messageBox->SetInfoText(GBUTF8("保存成功！"));
		m_messageBox->SetBtnNoVisible(false);
		m_messageBox->exec();
	}
}

void Dlg_PowerSyn::slot_dataChanged()
{
	ui.checkBox->setChecked(m_tCfg.bEnablePwrSync);
	if (m_tCfg.bAutoPhaseSync)
	{
		ui.com_type->setCurrentIndex(0);
		ui.stackedWidget->setCurrentIndex(0);
	}
	else
	{
		ui.com_type->setCurrentIndex(1);
		ui.stackedWidget->setCurrentIndex(1);
	}
	ui.ledt_xw->setText(QString("%1").arg(m_tCfg.dwFixPhase));
	//TIPC_PhaseTableCfg atPhaseTable[MAX_PHASE_NUM];
	ui.ledt_100->setText(QString("%1").arg(m_tCfg.atPhaseTable[0].dwPhase));
	ui.ledt_200->setText(QString("%1").arg(m_tCfg.atPhaseTable[1].dwPhase));
	ui.ledt_400->setText(QString("%1").arg(m_tCfg.atPhaseTable[2].dwPhase));
	ui.ledt_800->setText(QString("%1").arg(m_tCfg.atPhaseTable[3].dwPhase));
	ui.ledt_1600->setText(QString("%1").arg(m_tCfg.atPhaseTable[4].dwPhase));
	ui.ledt_3200->setText(QString("%1").arg(m_tCfg.atPhaseTable[5].dwPhase));
	ui.ledt_6400->setText(QString("%1").arg(m_tCfg.atPhaseTable[6].dwPhase));
	ui.ledt_12800->setText(QString("%1").arg(m_tCfg.atPhaseTable[7].dwPhase));
}

void Dlg_PowerSyn::SaveConfig()
{
	QString sPhase100 = ui.ledt_100->text();
	int dPahse100 = sPhase100.toInt();
	QString sPhase200 = ui.ledt_200->text();
	int dPahse200 = sPhase200.toInt();
	QString sPhase400 = ui.ledt_400->text();
	int dPahse400 = sPhase400.toInt();
	QString sPhase800 = ui.ledt_800->text();
	int dPahse800 = sPhase800.toInt();
	QString sPhase1600 = ui.ledt_1600->text();
	int dPahse1600 = sPhase1600.toInt();
	QString sPhase3200 = ui.ledt_3200->text();
	int dPahse3200 = sPhase3200.toInt();
	QString sPhase6400 = ui.ledt_6400->text();
	int dPahse6400 = sPhase6400.toInt();
	QString sPhase12800 = ui.ledt_12800->text();
	int dPahse12800 = sPhase12800.toInt();
	m_tCfg.atPhaseTable[0].dwPhase = dPahse100;
	m_tCfg.atPhaseTable[1].dwPhase = dPahse200;
	m_tCfg.atPhaseTable[2].dwPhase = dPahse400;
	m_tCfg.atPhaseTable[3].dwPhase = dPahse800;
	m_tCfg.atPhaseTable[4].dwPhase = dPahse1600;
	m_tCfg.atPhaseTable[5].dwPhase = dPahse3200;
	m_tCfg.atPhaseTable[6].dwPhase = dPahse6400;
	m_tCfg.atPhaseTable[7].dwPhase = dPahse12800;
	bool isCheck = ui.checkBox->isChecked();
	m_tCfg.bEnablePwrSync = isCheck;
	bool isAuto = ui.com_type->currentIndex() == 0 ? true : false;
	m_tCfg.bAutoPhaseSync = isAuto;
	QString sFixPhase = ui.ledt_xw->text();
	m_tCfg.dwFixPhase = sFixPhase.toInt();

}


Dlg_PowerSyn::~Dlg_PowerSyn()
{
	SAFE_DELETE(m_messageBox);
	MgrData::getInstance()->GetMgrSubject()->Del(this);
}


void Dlg_PowerSyn::OnObserverNotify(LPARAM lHint, LPVOID pHint)
{

	u32 dwEvtType = (u32)lHint;
	u32 dwId = (u32)pHint;

	switch (dwEvtType)
	{
	case NMC_GET_IPC_POWERSYN:
	{
		TIPC_GetPowerSnycRsp* p = (TIPC_GetPowerSnycRsp*)pHint;
		if (p->nRetCode == 0)
		{
			memcpy(&m_tCfg, &(p->tPowerSync), sizeof(TIPC_PowerPhaseSync));
			emit dataChanged();
		}
	}
	break;

	}
}


void Dlg_PowerSyn::SetParam()
{
	MgrItsCtrl* pMgrItsCtrl = MgrData::getInstance()->GetMgrItsCtrl();
	if (pMgrItsCtrl != NULL)
	{
		u32 dIp = MgrData::getInstance()->GetIP();
		ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dIp);
		if (pCtrl != NULL)
		{
			bool bRet = pCtrl->SetIpcPowerSyn(&m_tCfg);

			if (!bRet)
			{
			}
		}
	}
}

bool Dlg_PowerSyn::GetParam()
{
	u32 dIp = MgrData::getInstance()->GetIP();
	ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dIp);
	if (pCtrl != NULL)
	{
		bool bRet = pCtrl->GetIpcPowerSyn(NULL, (u32)(CObserver*)this);
		if (!bRet)
		{

		}

		return bRet;
	}

	return false;
}