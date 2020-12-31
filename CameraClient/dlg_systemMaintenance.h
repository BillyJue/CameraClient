#pragma once

#include "stdafx.h"
#include <QWidget>
#include "ui_dlg_systemMaintenance.h"
#include "Dlg_MessageBox.h"
#include "MyWidget.h"
#define GBUTF8(da) QString::fromLocal8Bit(da)
class Dlg_systemMaintenance : public MyWidget, public CObserver
{
	Q_OBJECT

public:
	Dlg_systemMaintenance(QWidget *parent = Q_NULLPTR);

	~Dlg_systemMaintenance();

	void InitData(bool is /* = true */);

	void OnObserverNotify(LPARAM lHint, LPVOID pHint);

	bool SaveCfg(TDevAllConfig* p);

	bool OnCheckFileVersion();

	void SystemUpdate();

	bool OnFtpFile();

	bool GetIsPkg(){ return m_bPKG; }

public slots:
	void slot_restart_clicked();
	void slot_easyRecovery_clicked();
	void slot_exportConfig_clicked();
	void slot_loadConfig_clicked();
	void slot_systemUpdate_clicked();

private:
	Ui::dlg_systemMaintenance ui;
	QString m_wsPath;
	LONG m_nFileLen;
	bool m_bPKG;
	Dlg_MessageBox *m_messageBox;
};
