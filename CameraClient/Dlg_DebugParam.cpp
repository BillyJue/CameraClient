#include "Dlg_DebugParam.h"
#include "MgrData.h"
#include <QIntValidator>
Dlg_DebugParam::Dlg_DebugParam(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	MgrData::getInstance()->GetMgrSubject()->Add(this);
	memset(&m_tIPC_DebugParam, 0, sizeof(m_tIPC_DebugParam));
	connect(this, SIGNAL(data_changed()), this, SLOT(Slot_DataChanged()));
	connect(ui.btn_save, SIGNAL(clicked()), this, SLOT(Slot_BtnSave()));
	connect(ui.btn_cancel, SIGNAL(clicked()), this, SLOT(Slot_BtnCancel()));
	setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
	setWindowTitle(GBUTF8("调试参数"));
	Init();
	//GetDebugParam();
}

Dlg_DebugParam::~Dlg_DebugParam()
{
	MgrData::getInstance()->GetMgrSubject()->Del(this);
}

void Dlg_DebugParam::Init()
{
	ui.ledt_shutter->setValidator(new QIntValidator(0, 1000, this));
	ui.ledt_eq->setValidator(new QIntValidator(0, 1000, this));

	ui.cmb_picSize->addItem(GBUTF8("1/4"));
	ui.cmb_picSize->addItem(GBUTF8("1/2"));
	ui.cmb_picSize->addItem(GBUTF8("原图"));
	GetDebugParam();
}

void Dlg_DebugParam::Slot_BtnSave()
{
	m_tIPC_DebugParam.bVideoLoop = ui.chk_videoCheck->isChecked();
	m_tIPC_DebugParam.bVideotrack = ui.chk_videoLook->isChecked();
	m_tIPC_DebugParam.bAuxFocus = ui.chk_focus->isChecked();
	m_tIPC_DebugParam.b2DRadar = ui.chk_2d->isChecked();
	m_tIPC_DebugParam.bRedLampTest = ui.chk_readTest->isChecked();
	m_tIPC_DebugParam.bExDevice = ui.chk_outerstauts->isChecked();
	m_tIPC_DebugParam.dwPicSize = ui.cmb_picSize->currentIndex();
	m_tIPC_DebugParam.dwQpValue = ui.ledt_eq->text().toInt();
	m_tIPC_DebugParam.dwEquShutter = ui.ledt_shutter->text().toInt();
	m_tIPC_DebugParam.bExposure = ui.chk_expParam->isChecked();
	m_tIPC_DebugParam.bIllMode = ui.chk_illegalType->isChecked();

	SetDebugParam();
	this->done(1);
}

void Dlg_DebugParam::Slot_BtnCancel()
{
	this->done(0);
}

void Dlg_DebugParam::Slot_DataChanged()
{
	ui.chk_videoCheck->setChecked(m_tIPC_DebugParam.bVideoLoop);
	ui.chk_videoLook->setChecked(m_tIPC_DebugParam.bVideotrack);
	ui.chk_illegalType->setChecked(m_tIPC_DebugParam.bIllMode);
	ui.chk_focus->setChecked(m_tIPC_DebugParam.bAuxFocus);
	ui.chk_outerstauts->setChecked(m_tIPC_DebugParam.bExDevice);
	ui.chk_expParam->setChecked(m_tIPC_DebugParam.bExposure);
	ui.chk_2d->setChecked(m_tIPC_DebugParam.b2DRadar);
	ui.chk_readTest->setChecked(m_tIPC_DebugParam.bRedLampTest);
	ui.ledt_shutter->setText(QString("%1").arg(m_tIPC_DebugParam.dwEquShutter));
	ui.ledt_eq->setText(QString("%1").arg(m_tIPC_DebugParam.dwQpValue));

}

void Dlg_DebugParam::SetDebugParam()
{
	//UpdateData(FALSE);
	u32 dIP = MgrData::getInstance()->GetIP();
	ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dIP);
	if (pCtrl != NULL)
	{
		pCtrl->SetIpcDebugParam(&m_tIPC_DebugParam);
	}
}

void Dlg_DebugParam::GetDebugParam()
{
	u32 dIP = MgrData::getInstance()->GetIP();
	ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dIP);
	if (pCtrl != NULL)
	{
		pCtrl->GetIpcDebugParam(NULL, (u32)(CObserver*)this);
	}
}

void Dlg_DebugParam::OnObserverNotify(LPARAM lHint, LPVOID pHint)
{
	u32 dwEvtType = (u32)lHint;
	u32 dwId = (u32)pHint;

	switch (dwEvtType)
	{
	case NMC_GET_IPC_DEBUG_PARAM:
	{
		TIPC_GetDebugParamRsp* p = (TIPC_GetDebugParamRsp*)pHint;
		if (p->nRetCode == 0)
		{
			memcpy(&m_tIPC_DebugParam, &(p->tDebugParam), sizeof(TIPC_DebugParam));
			emit data_changed();
		}
	}
		break;
	default:
		break;
	}
}