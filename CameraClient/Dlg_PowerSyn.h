#pragma once

#include <QWidget>
#include "ui_Dlg_PowerSyn.h"
#include <windows.h>
#include <subject.h>
#include "ivhs_struct.h"
#include "Dlg_MessageBox.h"
#include "MyWidget.h"
#define GBUTF8(da) QString::fromLocal8Bit(da)
class Dlg_PowerSyn : public MyWidget, public CObserver
{
	Q_OBJECT

public:
	Dlg_PowerSyn(QWidget *parent = Q_NULLPTR);

	~Dlg_PowerSyn();

	void InitData(bool is /* = true */);

	void OnSetTxtVisible();

	bool GetParam();

	void SetParam();

	void SaveConfig();

	void OnObserverNotify(LPARAM lHint, LPVOID pHint);

public slots:
	void slot_dataChanged();

	void slot_saveClicked();

	void slot_save2Clicked();

	void slot_comboIndexChanged(int nIndex);
	
signals:
	void dataChanged();
private:
	Ui::Dlg_PowerSyn ui;
	Dlg_MessageBox				*m_messageBox;
	TIPC_PowerPhaseSync			m_tCfg;

};
