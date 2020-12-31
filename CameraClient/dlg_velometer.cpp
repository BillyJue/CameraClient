#include "dlg_velometer.h"
#include <MgrData.h>
Dlg_velometer::Dlg_velometer(QWidget *parent)
	: MyWidget(parent)
{
	ui.setupUi(this);
	MgrData::getInstance()->GetMgrSubject()->Add(this);
	connect(this, SIGNAL(dataChanged()), this, SLOT(slot_dataChanged()));
	connect(this, SIGNAL(clicked()), this, SLOT(slot_saveConfig()));
	//GetParam();
}

void Dlg_velometer::InitData(bool is)
{
	if (is)
	{
		GetParam();
	}
}

void Dlg_velometer::slot_dataChanged()
{
	QString sCameraHeight = QString("%1").arg(QString::number(m_tCfg.fCameraHeight, 'f', 2));
	QString sTop = QString("%1").arg(QString::number(m_tCfg.fTopLineDist, 'f', 2));
	QString sMid = QString("%1").arg(QString::number(m_tCfg.fMidLineDist, 'f', 2));
	QString sBottom = QString("%1").arg(QString::number(m_tCfg.fBottomLineDist, 'f', 2));
	QString sB2G = QString("%1").arg(QString::number(m_tCfg.fPlateB2G, 'f', 2));
	QString sCorrectRate = QString("%1").arg(QString::number(m_tCfg.fCorrectRate, 'f', 2));
	ui.ledt_cameraHeigth->setText(sCameraHeight);
	ui.ledt_top->setText(sTop);
	ui.ledt_mid->setText(sMid);
	ui.ledt_bottom->setText(sBottom);
	ui.ledt_plateB2G->setText(sB2G);
	ui.ledt_fCorrectRate->setText(sCorrectRate);
	
}

void Dlg_velometer::slot_saveConfig()
{
	SaveConfig();
	SetParam();
}

Dlg_velometer::~Dlg_velometer()
{
	MgrData::getInstance()->GetMgrSubject()->Del(this);
}

void Dlg_velometer::SaveConfig()
{
	QString sCameraHeight = ui.ledt_cameraHeigth->text();
	double dScameraHeight = sCameraHeight.toDouble();
	QString sTop = ui.ledt_top->text();
	double dsTop = sTop.toDouble();
	QString sMid = ui.ledt_mid->text();
	double dMid = sMid.toDouble();
	QString sBottom = ui.ledt_bottom->text();
	double dBottom = sBottom.toDouble();
	QString sPlateB2G = ui.ledt_plateB2G->text();
	double dPlateB2G = sPlateB2G.toDouble();
	QString sCorrectRate = ui.ledt_fCorrectRate->text();
	double dCorrectRate = sCorrectRate.toDouble();

	m_tCfg.fCameraHeight = dScameraHeight;
	m_tCfg.fTopLineDist = dsTop;
	m_tCfg.fMidLineDist = dMid;
	m_tCfg.fBottomLineDist = dBottom;
	m_tCfg.fPlateB2G = dPlateB2G;
	m_tCfg.fCorrectRate = dCorrectRate;
}

void Dlg_velometer::SetParam()
{
	MgrItsCtrl* pMgrItsCtrl = MgrData::getInstance()->GetMgrItsCtrl();
	if (pMgrItsCtrl != NULL)
	{
		u32 dIp = MgrData::getInstance()->GetIP();
		ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dIp);
		if (pCtrl != NULL)
		{
			bool bRet = pCtrl->SetIpcVelometer(&m_tCfg);

			if (!bRet)
			{
			}
		}
	}

}
bool Dlg_velometer::GetParam()
{
	u32 dIp = MgrData::getInstance()->GetIP();
	ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dIp);
	if (pCtrl != NULL)
	{
		bool bRet = pCtrl->GetIpcVelometer(NULL, (u32)(CObserver*)this);
		if (!bRet)
		{

		}

		return bRet;
	}

	return false;
}



void Dlg_velometer::OnObserverNotify(LPARAM lHint, LPVOID pHint)
{

	u32 dwEvtType = (u32)lHint;
	u32 dwId = (u32)pHint;

	switch (dwEvtType)
	{
	case NMC_GET_IPC_VELOMETER:
	{
		TIPC_GetVidSpdDetRsp* p = (TIPC_GetVidSpdDetRsp*)pHint;
		if (p->nRetCode == 0)
		{
			memcpy(&m_tCfg, &(p->tVidSpdDet), sizeof(p->tVidSpdDet));
			emit dataChanged();
		}
	}
	break;

	}
}
