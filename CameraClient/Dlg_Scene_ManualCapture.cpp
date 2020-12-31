#include "Dlg_Scene_ManualCapture.h"
#include "MgrData.h"
#include <QMessageBox>
Dlg_Scene_ManualCapture::Dlg_Scene_ManualCapture(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
	connect(ui.btn_loadPicScene, SIGNAL(clicked()), this, SLOT(Slot_LoadSceneClicked()));
	connect(ui.btn_illSnap, SIGNAL(clicked()), this, SLOT(Slot_SnapClicked()));

	// 添加电警过程数据的接收通知窗口
	if (MgrData::getInstance()->GetMgrEvtLst())
	{
		MgrData::getInstance()->GetMgrEvtLst()->AddSender(static_cast<CObserver*>(this));
	}
	MgrData::getInstance()->GetMgrSubject()->Add(this);

	GetScenceParam();

	GetScenceID();

	LoadScenceImage();
}

void Dlg_Scene_ManualCapture::Slot_LoadSceneClicked()
{
	LoadScenceImage();
}

void Dlg_Scene_ManualCapture::Slot_SnapClicked()
{
	SnapIllegalParking();
	this->done(1);
}


void Dlg_Scene_ManualCapture::GetScenceID()
{
	u32 dwIP = MgrData::getInstance()->GetIP();
	ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dwIP);
	if (pCtrl != NULL)
	{
		bool bRet = pCtrl->GetScenceID(NULL, (u32)(CObserver*)this);
		if (!bRet)
		{
		}
	}
}


void Dlg_Scene_ManualCapture::GetIpcParkScence()
{
	u32 dwIP = MgrData::getInstance()->GetIP();
	ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dwIP);
	if (pCtrl != NULL)
	{
		unsigned int sceneIndex = m_scenceId + 1;
		bool bRet = pCtrl->GetIpcParkScence(&sceneIndex, (u32)(CObserver*)this);
		if (!bRet)
		{
		}
	}
}

void Dlg_Scene_ManualCapture::SnapIllegalParking()
{
	m_tSnapIllegalPark.scenceId = m_scenceId + 1;
	TRECTANGLE_S rect;
	ui.picDraw->GetRectOut(&rect, 1);
	m_tSnapIllegalPark.rect.X = rect.X;
	m_tSnapIllegalPark.rect.Y = rect.Y;
	m_tSnapIllegalPark.rect.W = rect.W;
	m_tSnapIllegalPark.rect.H = rect.H;

	if (m_tSnapIllegalPark.rect.W == 0 || m_tSnapIllegalPark.rect.H == 0)
	{
		QMessageBox box;
		box.setWindowTitle(GBUTF8("警告"));
		box.setText(GBUTF8("	请选择违停车辆区域	"));
		box.exec();
		return;
	}

	u32 dwIP = MgrData::getInstance()->GetIP();
	ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dwIP);
	if (pCtrl != NULL)
	{
		bool bRet = pCtrl->SetSnapIllegalParking(&m_tSnapIllegalPark);
		if (!bRet)
		{
		}
	}
}


void Dlg_Scene_ManualCapture::LoadScenceImage()
{
	u32 dwIP = MgrData::getInstance()->GetIP();
	ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dwIP);
	if (pCtrl != NULL)
	{
		pCtrl->IpcEvt_Snap();
	}
}

void Dlg_Scene_ManualCapture::GetScenceParam()
{
	u32 dwIP = MgrData::getInstance()->GetIP();
	ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dwIP);
	if (pCtrl != NULL)
	{
		unsigned int sceneIndex = 1;
		bool bRet = pCtrl->GetIpcScence(&sceneIndex, (u32)(CObserver*)this);
		if (!bRet)
		{
		}
	}
}


Dlg_Scene_ManualCapture::~Dlg_Scene_ManualCapture()
{
	MgrData::getInstance()->GetMgrEvtLst()->AddSender(static_cast<CObserver*>(this));
	MgrData::getInstance()->GetMgrSubject()->Del(this);
}


LRESULT Dlg_Scene_ManualCapture::OnRefreshPic(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	u32 dwId = (u32)wParam;
	// 图片浏览取缓冲区中的图片
	if (uMsg == NMC_EVT_VIDEO_PIC)
	{
		dwId = 0;
	}	
	ui.picDraw->SetNewPic2(dwId, lParam);

	//
	bHandled = TRUE;
	return 0;
}

void Dlg_Scene_ManualCapture::OnObserverNotify(LPARAM lHint, LPVOID pHint)
{
	//if (!IsRecvNotiyMsg())  return;

	u32 dwEvtType = (u32)lHint;
	u32 dwId = (u32)pHint;

	switch (dwEvtType)
	{
	case NMC_EVT_VIDEO_PIC:
	{
		BOOL bHandled;
		OnRefreshPic(0, (WPARAM)0, 0, bHandled);
		TRECTANGLE_S area;
		area.X = 0;
		area.Y = 0;
		area.W = 0;
		area.H = 0;
		ui.picDraw->Clear();
		ui.picDraw->AddRect(&area, 1);
		ui.picDraw->SetSelType(1);
		ui.picDraw->SetColorByType(1, COLOR_YELLOW);
	}
		break;
	case NMC_GET_IPC_SCENCEINFO:
	{
		TIPC_GetSceneCfgRsp* p = (TIPC_GetSceneCfgRsp*)pHint;
		if (p->nRetCode == 0)
		{
			memcpy(&m_tSeceneCfg, p, sizeof(TIPC_GetSceneCfgRsp));
			ui.picDraw->SetCoefficient(m_tSeceneCfg.tMaxParam.dwMaxWidth, m_tSeceneCfg.tMaxParam.dwMaxHeight);
		}
	}
		break;
	case NMC_GET_IPC_PARKSCENCE:
	{
		TIPC_illgParkPara* p = (TIPC_illgParkPara*)pHint;
		memcpy(&m_tIPC_illgParkPara, p, sizeof(m_tIPC_illgParkPara));
	}
		break;
	case NMC_GET_IPC_SCENCE_ID:
	{
		m_scenceId = *(u32*)pHint;
	}
		break;
	default:
		break;
	}
}

