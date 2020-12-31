#pragma once

#include <QWidget>
#include <QDialog>
#include "ui_Dlg_Detection_Area.h"
#include <subject.h>
#include "ivhs_struct.h"
#include "define.h"
#define GBUTF8(da) QString::fromLocal8Bit(da)
class Dlg_Detection_Area : public QDialog, public CObserver
{
	Q_OBJECT

public:
	Dlg_Detection_Area(QWidget *parent, int width, int height);

	~Dlg_Detection_Area();

	void Init();

	void InitData();

	void OnObserverNotify(LPARAM lHint, LPVOID pHint);

	void SetData(unsigned int *x, unsigned int *y, unsigned int *w, unsigned int *h);

	void SetData(TRuleLine_S *line);

	void GetData();

	void ShowData();

public slots:

	void Slot_BtnOKClicked();

	void Slot_BtnCancelClicked();


private:
	Ui::Dlg_Detection_Area ui;
	int m_nWidth;
	int m_nHeight;
	unsigned int        *m_pX;
	unsigned int        *m_pY;
	unsigned int        *m_pW;
	unsigned int        *m_pH;

	TRuleLine_S       *m_pLine;
};
