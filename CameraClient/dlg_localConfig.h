#pragma once

#include <QWidget>
#include "ui_Dlg_LocalConfig.h"
#include <windows.h>
#include <subject.h>
#include "ivhs_struct.h"
#include "define.h"
#include <QButtonGroup>
#include "MyWidget.h"
#include "Dlg_MessageBox.h"
#define GBUTF8(da) QString::fromLocal8Bit(da)
class Dlg_LocalConfig : public MyWidget, public CObserver
{
	Q_OBJECT

public:
	Dlg_LocalConfig(QWidget *parent = Q_NULLPTR);

	~Dlg_LocalConfig();

	void InitData(bool is /* = true */);

	void OnSetTxtVisible();

	bool GetParam();

	void SetParam();

	void OnObserverNotify(LPARAM lHint, LPVOID pHint);

public slots:
	void Slot_BtnSaveClicked();

	void Slot_BtnVideoScanClicked();

	void Slot_BtnPictureScanClicked();

private:
	Ui::Dlg_LocalConfig ui;

	TLocalCfgInfo m_tLocalCfgInfo;

	QButtonGroup	*m_buttonGroup;
	Dlg_MessageBox	*m_messageBox;
};
