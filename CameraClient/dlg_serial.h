#pragma once

#include <QWidget>
#include "ui_Dlg_Serial.h"
#include <windows.h>
#include <subject.h>
#include "ivhs_struct.h"
#include "Dlg_MessageBox.h"
#include <MyWidget.h>
#define GBUTF8(da) QString::fromLocal8Bit(da)
class Dlg_Serial : public MyWidget, public CObserver
{
	Q_OBJECT

public:
	Dlg_Serial(QWidget *parent = Q_NULLPTR);

	~Dlg_Serial();

	void InitData(bool is);

	void SetData();

	void OnSetTxtVisible();

	void OnObserverNotify(LPARAM lHint, LPVOID pHint);

	bool GetParam();

	void SetParam();

	void GetData();

	void init();

	void SelectProtocol(int nIndex);
public slots:
	void Slot_DataChanged();

	void Slot_BtnSaveClicked();

	void Slot_CmbCurrentIndexChanged(int nSel);

signals:
	void dataChanged();

private:
	int m_nSelect;
	Ui::Dlg_Serial ui;
	Dlg_MessageBox *m_messageBox;
	TIPC_SerialCfg m_tIPC_SerialCfg;
};
