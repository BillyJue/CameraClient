#pragma once

#include <QWidget>
#include "ui_Dlg_IdentParam.h"
#include <subject.h>
#include "ivhs_struct.h"
#include "define.h"
#include "MyWidget.h"
#include "Dlg_MessageBox.h"
#define GBUTF8(da) QString::fromLocal8Bit(da)
class Dlg_IdentParam : public MyWidget, public CObserver
{
	Q_OBJECT

public:

	Dlg_IdentParam(QWidget *parent = Q_NULLPTR);

	~Dlg_IdentParam();

	void OnSetTxtVisible();

	void OnCheckVisible();

	void Init();

	void InitData(bool is);

	bool GetParam();

	void SetParam();

	void SetData();

	void GetData();

	void OnObserverNotify(LPARAM lHint, LPVOID pHint);

public slots:
	void Slot_IdentParamChanged();

	void Slot_BtnCheckClicked(int);

	void Slot_BtnSaveClicked();
signals:
	void signal_identParamChanged();

private:
	Ui::Dlg_IdentParam ui;
	TIPC_RecogParam	 m_tCfg;
	Dlg_MessageBox  *m_messageBox;
};
