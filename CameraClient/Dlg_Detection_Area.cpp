#include "Dlg_Detection_Area.h"
#include "MgrData.h"
Dlg_Detection_Area::Dlg_Detection_Area(QWidget *parent, int nWidth, int nHeight)
	: QDialog(parent), m_nWidth(nWidth), m_nHeight(nHeight), m_pLine(nullptr)
	, m_pX(nullptr), m_pY(nullptr), m_pW(nullptr), m_pH(nullptr)
{
	ui.setupUi(this);
	connect(ui.btn_ok, SIGNAL(clicked()), this, SLOT(Slot_BtnOKClicked()));
	connect(ui.btn_cancel, SIGNAL(clicked()), this, SLOT(Slot_BtnCancelClicked()));
	
	showFullScreen();
	Init();
	// 添加电警过程数据的接收通知窗口
	if (MgrData::getInstance()->GetMgrEvtLst())
	{
		MgrData::getInstance()->GetMgrEvtLst()->AddSender(static_cast<CObserver*>(this));
	}

	InitData();
	
}

void Dlg_Detection_Area::InitData()
{
	ui.widget_2->SetNewPic2(0, 0);
	ui.widget_2->update();
}

void Dlg_Detection_Area::Init()
{
	if (m_nWidth == 0 || m_nHeight == 0)
	{
		ui.widget_2->setFixedSize(size());
	}
	else
	{
		ui.widget_2->setFixedSize(m_nWidth, m_nHeight);
		ui.widget_2->SetCoefficient(m_nWidth, m_nWidth);
	}

}

void Dlg_Detection_Area::OnObserverNotify(LPARAM lHint, LPVOID pHint)
{
	u32 dwEvtType = (u32)lHint;
	u32 dwId = (u32)pHint;

	switch (dwEvtType)
	{
	case NMC_EVT_VIDEO_PIC:
	{
		PTEVTVEHICLEINFO pData = MgrData::getInstance()->GetMgrEvtLst()->GetItem(dwId);
		u32 dwIP = MgrData::getInstance()->GetIP();
		// 来源相同
		if (pData != NULL && pData->dwIP == dwIP)
		{
			ui.widget_2->SetNewPic2(dwId, 0);
			ui.widget_2->update();
		}
	}
		break;
	default:
		break;
	}
}



Dlg_Detection_Area::~Dlg_Detection_Area()
{
	MgrData::getInstance()->GetMgrEvtLst()->DelSender(static_cast<CObserver*>(this));
}

void Dlg_Detection_Area::Slot_BtnOKClicked()
{
	GetData();
	this->done(1);
}

void Dlg_Detection_Area::Slot_BtnCancelClicked()
{
	this->done(0);
}

void Dlg_Detection_Area::SetData(unsigned int *x, unsigned int *y, unsigned int *w, unsigned int *h)
{
	m_pX = x;
	m_pY = y;
	m_pW = w;
	m_pH = h;

	ShowData();
}

void Dlg_Detection_Area::ShowData()
{
	if (m_pX != NULL)
	{
		TRECTANGLE_S rect;
		memset(&rect, 0, sizeof(rect));
		rect.X = *m_pX;
		rect.Y = *m_pY;
		rect.W = *m_pW;
		rect.H = *m_pH;
		ui.widget_2->AddRect(&rect, 0);
		ui.widget_2->SetSelType(0);
	}

	if (m_pLine != NULL)
	{
		ui.widget_2->AddLine(&(m_pLine->tLine), 1, m_pLine->LineLabel);
		ui.widget_2->SetSelType(1);
	}
}


void Dlg_Detection_Area::SetData(TRuleLine_S *line)
{
	ui.widget_2->CalcPosData(0);
	m_pLine = line;
	ShowData();
}

void Dlg_Detection_Area::GetData()
{
	if (m_pX != NULL)
	{
		TRECTANGLE_S rect;
		ZeroMemory(&rect, sizeof(TRECTANGLE_S));
		ui.widget_2->GetRectOut(&rect, 0);

		*m_pX = rect.X;
		*m_pY = rect.Y;
		*m_pW = rect.W;
		*m_pH = rect.H;
	}

	if (m_pLine != NULL)
	{
		ui.widget_2->GetLineOut(&(m_pLine->tLine), 1);
	}
}
