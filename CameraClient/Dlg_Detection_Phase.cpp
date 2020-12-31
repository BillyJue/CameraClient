#include "Dlg_Detection_Phase.h"
#include "ivhs_define.h"

Dlg_Detection_Phase::Dlg_Detection_Phase(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	connect(ui.btn_ok, SIGNAL(clicked()), this, SLOT(Slot_BtnClicked()));
	connect(ui.btn_cancel, SIGNAL(clicked()), this, SLOT(Slot_BtnClicked()));
	setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
	setWindowTitle(GBUTF8("相位关联"));
}


Dlg_Detection_Phase::~Dlg_Detection_Phase()
{

}

void Dlg_Detection_Phase::Slot_BtnClicked()
{
	if (this->sender() == ui.btn_ok)
	{
		GetData();
		this->done(1);
	}
	else if (this->sender() == ui.btn_cancel)
	{
		this->done(0);
	}
}

void Dlg_Detection_Phase::SetData(unsigned int *nPhase)
{
	m_pPhase = nPhase;
}

int Dlg_Detection_Phase::GetSpecifyBitValue(Uint32 dwSrc, int nIndex)
{
	Uint32 dw2 = ((int)pow(2.0, nIndex));
	dwSrc = dwSrc & dw2;
	dwSrc = dwSrc >> nIndex;

	return dwSrc;
}

u32 Dlg_Detection_Phase::SetSpecifyBitValue(Uint32& dwSrcOut, Uint32 dwValue, int nIndex)
{
	Uint32 dw2 = ((int)pow(2.0, nIndex));

	if (dwValue == 0)
	{
		dw2 = ~dw2;
		dwSrcOut = dwSrcOut & dw2;
	}
	else
	{
		dwSrcOut = dwSrcOut | dw2;
	}

	return dwSrcOut;
}



void Dlg_Detection_Phase::ShowData()
{
	if (m_pPhase == NULL)
	{
		return;
	}

	unsigned int phase = *m_pPhase;
	if (REDLAMP_PHASE_TURNRIGHT & phase)
	{
		ui.chk_3->setChecked(true);
	}
	else
	{
		ui.chk_3->setChecked(false);
	}

	if (REDLAMP_PHASE_STRA_AHEAD & phase)
	{
		ui.chk_2->setChecked(true);
	}
	else
	{
		ui.chk_2->setChecked(false);
	}

	if (REDLAMP_PHASE_TURNLEFT & phase)
	{
		ui.chk_1->setChecked(true);
	}
	else
	{
		ui.chk_1->setChecked(false);
	}
}

void Dlg_Detection_Phase::GetData()
{
	unsigned int phase = 0;

	if (ui.chk_3->isChecked())
	{
		SetSpecifyBitValue(phase, 1, 2);
	}
	else
	{
		SetSpecifyBitValue(phase, 0, 2);
	}

	if (ui.chk_2->isChecked())
	{
		SetSpecifyBitValue(phase, 1, 3);
	}
	else
	{
		SetSpecifyBitValue(phase, 0, 3);
	}

	if (ui.chk_1->isChecked())
	{
		SetSpecifyBitValue(phase, 1, 4);
	}
	else
	{
		SetSpecifyBitValue(phase, 0, 4);
	}

	*m_pPhase = phase;
}

