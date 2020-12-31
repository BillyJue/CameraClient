#include "Dlg_IdentParam.h"
#include "MgrData.h"
#include <QValidator>
#include <QListView>
#include <QTimer>
Dlg_IdentParam::Dlg_IdentParam(QWidget *parent)
	: MyWidget(parent), m_messageBox(nullptr)
{
	ui.setupUi(this);
	MgrData::getInstance()->GetMgrSubject()->Add(this);
	connect(this, SIGNAL(signal_identParamChanged()), this, SLOT(Slot_IdentParamChanged()));
	connect(ui.btn_save, SIGNAL(clicked()), this, SLOT(Slot_BtnSaveClicked()));

	connect(ui.chkall, SIGNAL(stateChanged(int)), this, SLOT(Slot_BtnCheckClicked(int)));
	connect(ui.chkfacelocate, SIGNAL(stateChanged(int)), this, SLOT(Slot_BtnCheckClicked(int)));
	connect(ui.chkvehlogorecog, SIGNAL(stateChanged(int)), this, SLOT(Slot_BtnCheckClicked(int)));
	Init();
	//test
	//InitData();
}

Dlg_IdentParam::~Dlg_IdentParam()
{
	MgrData::getInstance()->GetMgrSubject()->Del(this);
}

void Dlg_IdentParam::InitData(bool is)
{
	if (!is)
	{
		return;
	}
	ui.chkall->setChecked(false);
	memset(&m_tCfg, 0, sizeof(m_tCfg));
	GetParam();
}

bool Dlg_IdentParam::GetParam()
{
	u32 dwIP = MgrData::getInstance()->GetIP();
	ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dwIP);
	if (pCtrl != NULL)
	{
		bool bRet = pCtrl->GetIpcIdent(NULL, (u32)(CObserver*)this);
		if (!bRet)
		{

		}

		return bRet;
	}

	return false;
}

void Dlg_IdentParam::Slot_IdentParamChanged()
{
	SetData();
}

void Dlg_IdentParam::Init()
{
	ui.edttimecontrol->setValidator(new QIntValidator(40, 1000, this));
	ui.cmbdefaultprovince->setView(new QListView);
	ui.cmbrecstd->setView(new QListView);

}

void Dlg_IdentParam::OnSetTxtVisible()
{
	if (m_messageBox)
	{
		m_messageBox->close();
		SAFE_DELETE(m_messageBox);
	}
}


void Dlg_IdentParam::SetData()
{
	ui.edttimecontrol->setText(QString("%1").arg(m_tCfg.dwTimeControl));
	ui.chklocalplate->setChecked(m_tCfg.bLocalPlate);
	wchar_t wz[10];
	ZeroMemory(wz, 10);
	char2wchar(wz, m_tCfg.szDefaultProvince);
	int nPos = ui.cmbdefaultprovince->findText(QString::fromWCharArray(wz));
	ui.cmbdefaultprovince->setCurrentIndex(nPos);

	nPos = ui.cmbchdefaultarea->findText(QString("%1").arg(m_tCfg.chDefaultArea));
	ui.cmbchdefaultarea->setCurrentIndex(nPos);
	ui.chkareaexcept->setChecked(m_tCfg.bAreaExcept);
	ui.chkrecogmt->setChecked(m_tCfg.bRecogMT);
	ui.chkshadowremove->setChecked(m_tCfg.bShadowRemove);
	ui.chkvehcolorrecog->setChecked(m_tCfg.bVehColorRecog);
	ui.chkfacelocate->setChecked(m_tCfg.bFaceLocate);
	ui.chkphonecheck->setChecked(m_tCfg.bPhoneCheck);
	ui.chkseatbeltcheck->setChecked(m_tCfg.bSeatbeltCheck);
	ui.chkcopilotcheck->setChecked(m_tCfg.bCopilotCheck);
	ui.chksunvisorcheck->setChecked(m_tCfg.bSunvisorCheck);
	ui.chkvehdanger->setChecked(m_tCfg.bVehDangerRecog);
	ui.chkvehlogorecog1->setChecked(m_tCfg.bVehSubModelRecog);
	ui.chkvehlogorecog->setChecked(m_tCfg.bVehLogoRecog);
	ui.chkvehlogorecog2->setChecked(m_tCfg.bVehYearModelRecog);
	ui.cmbrecstd->setCurrentIndex(m_tCfg.byRecStandard);
	
	OnCheckVisible();
}

void Dlg_IdentParam::OnCheckVisible()
{
	if (ui.chkfacelocate->isChecked())
	{
		ui.chkphonecheck->setChecked(true);
		ui.chkseatbeltcheck->setChecked(true);
		ui.chkcopilotcheck->setChecked(true);
		ui.chksunvisorcheck->setChecked(true);

		ui.chkphonecheck->setVisible(true);
		ui.chkseatbeltcheck->setVisible(true);
		ui.chkcopilotcheck->setVisible(true);
		ui.chksunvisorcheck->setVisible(true);
	}
	else
	{
		ui.chkphonecheck->setChecked(false);
		ui.chkseatbeltcheck->setChecked(false);
		ui.chkcopilotcheck->setChecked(false);
		ui.chksunvisorcheck->setChecked(false);
		
		ui.chkphonecheck->setVisible(false);
		ui.chkseatbeltcheck->setVisible(false);
		ui.chkcopilotcheck->setVisible(false);
		ui.chksunvisorcheck->setVisible(false);
	}

	if (ui.chkvehlogorecog->isChecked())
	{
		ui.chkvehlogorecog1->setChecked(true);
		ui.chkvehlogorecog2->setChecked(true);
		ui.chkvehlogorecog1->setVisible(true);
		ui.chkvehlogorecog2->setVisible(true);
	}
	else
	{
		ui.chkvehlogorecog1->setChecked(false);
		ui.chkvehlogorecog2->setChecked(false);
		ui.chkvehlogorecog1->setVisible(false);
		ui.chkvehlogorecog2->setVisible(false);

	}
}

void Dlg_IdentParam::GetData()
{
	m_tCfg.dwTimeControl = ui.edttimecontrol->text().toInt();
	m_tCfg.bLocalPlate = ui.chklocalplate->isChecked();
	QString sDefault = ui.cmbdefaultprovince->currentText();
	wchar2char(m_tCfg.szDefaultProvince, sDefault.toStdWString().c_str());
	
	sDefault = ui.cmbchdefaultarea->currentText();
	m_tCfg.chDefaultArea = sDefault.toStdString().at(0);
	
	m_tCfg.bAreaExcept = ui.chkareaexcept->isChecked();
	m_tCfg.bRecogMT = ui.chkrecogmt->isChecked();
	m_tCfg.bShadowRemove = ui.chkshadowremove->isChecked();
	m_tCfg.bVehColorRecog = ui.chkvehcolorrecog->isChecked();
	m_tCfg.bFaceLocate = ui.chkfacelocate->isChecked();
	m_tCfg.bPhoneCheck = ui.chkphonecheck->isChecked();
	m_tCfg.bSeatbeltCheck = ui.chkseatbeltcheck->isChecked();
	m_tCfg.bCopilotCheck = ui.chkcopilotcheck->isChecked();
	m_tCfg.bSunvisorCheck = ui.chksunvisorcheck->isChecked();
	m_tCfg.bVehDangerRecog = ui.chkvehdanger->isChecked();
	m_tCfg.bVehSubModelRecog = ui.chkvehlogorecog1->isChecked();
	m_tCfg.bVehLogoRecog = ui.chkvehlogorecog->isChecked();
	m_tCfg.bVehYearModelRecog = ui.chkvehlogorecog2->isChecked();

	m_tCfg.byRecStandard = ui.cmbrecstd->currentIndex();
}


void Dlg_IdentParam::OnObserverNotify(LPARAM lHint, LPVOID pHint)
{

	u32 dwEvtType = (u32)lHint;
	u32 dwId = (u32)pHint;

	switch (dwEvtType)
	{
	case NMC_GET_IPC_IDENTPARAM:
	{
		TIPC_GetRecogParamRsp* p = (TIPC_GetRecogParamRsp*)pHint;
		if (p->nRetCode == 0)
		{
			memcpy(&m_tCfg, &(p->tParam), sizeof(p->tParam));
			emit signal_identParamChanged();
		}
	}
		break;

	}
}

void Dlg_IdentParam::Slot_BtnCheckClicked(int nIndex)
{
	if (this->sender() == ui.chkall)
	{
		ui.chklocalplate->setChecked(nIndex);
		ui.chkareaexcept->setChecked(nIndex);
		ui.chkrecogmt->setChecked(nIndex);
		ui.chkshadowremove->setChecked(nIndex);
		ui.chkvehcolorrecog->setChecked(nIndex);
		ui.chkfacelocate->setChecked(nIndex);
		ui.chkphonecheck->setChecked(nIndex);
		ui.chkseatbeltcheck->setChecked(nIndex);
		ui.chkcopilotcheck->setChecked(nIndex);
		ui.chksunvisorcheck->setChecked(nIndex);
		ui.chkvehdanger->setChecked(nIndex);
		ui.chkvehlogorecog1->setChecked(nIndex);
		ui.chkvehlogorecog->setChecked(nIndex);
		ui.chkvehlogorecog2->setChecked(nIndex);
	}
	else if (this->sender() == ui.chkfacelocate || this->sender() == ui.chkvehlogorecog)
	{
		OnCheckVisible();
	}
	
}

void Dlg_IdentParam::Slot_BtnSaveClicked()
{
	GetData();
	SetParam();
	if (!m_messageBox)
	{
		QTimer::singleShot(1500, this, &Dlg_IdentParam::OnSetTxtVisible);
		m_messageBox = new Dlg_MessageBox;
		m_messageBox->SetInfoText(GBUTF8("±£´æ³É¹¦£¡"));
		m_messageBox->SetBtnNoVisible(false);
		m_messageBox->exec();
	}

}

void Dlg_IdentParam::SetParam()
{
	MgrItsCtrl* pMgrItsCtrl = MgrData::getInstance()->GetMgrItsCtrl();
	if (pMgrItsCtrl != NULL)
	{
		u32 dwIP = MgrData::getInstance()->GetIP();
		ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dwIP);
		if (pCtrl != NULL)
		{
			bool bRet = pCtrl->SetIpcIdent(&m_tCfg);

			if (!bRet)
			{
			}
		}
	}

}