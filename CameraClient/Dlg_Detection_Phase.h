#pragma once

#include <QDialog>
#include "ui_Dlg_Detection_Phase.h"
#include "ivhs_struct.h"
#include "define.h"
#define  GBUTF8(obj) QString::fromLocal8Bit(obj)
class Dlg_Detection_Phase : public QDialog
{
	Q_OBJECT

public:
	Dlg_Detection_Phase(QWidget *parent = Q_NULLPTR);

	~Dlg_Detection_Phase();

	void ShowData();

	void GetData();

	void SetData(unsigned int *nPhase);

	int GetSpecifyBitValue(Uint32 dwSrc, int nIndex);

	u32 SetSpecifyBitValue(Uint32& dwSrcOut, Uint32 dwValue, int nIndex);

public slots:
 void Slot_BtnClicked();

private:
	Ui::Dlg_Detection_Phase ui;

	unsigned int *m_pPhase;
};
