#pragma once

#include <QWidget>
#include "ui_Dlg_otherConfig.h"
#include "Dlg_PowerSyn.h"
#include "dlg_IPCInfoSync.h"
#include "dlg_serial.h"
#include "dlg_signalConfig.h"
#include "dlg_velometer.h"
#include "dlg_hardware.h"
#include <QSignalMapper>
#include "MyWidget.h"
class Dlg_otherConfig : public MyWidget
{
	Q_OBJECT

public:
	Dlg_otherConfig(QWidget *parent = Q_NULLPTR);

	~Dlg_otherConfig();

	void AddStackedWidget();

	void InitData(bool is /* = true */);

public slots:
	void Slot_HardWareTestClicked();

	void Slot_VideoSpeedClicked();

	void Slot_PowerSyncClicked();

	void Slot_SerialSettingClicked();

	void Slot_SignalSettingClicked();

	void Slot_SignalSyncClicked();
private:
	Ui::Dlg_otherConfig ui;
	Dlg_velometer *m_dlgVelometer;
	Dlg_PowerSyn  *m_dlgPowerSync;
	Dlg_Serial	  *m_dlgSerial;
	Dlg_SignalConfig	*m_dlgSignalConfig;
	Dlg_IPCInfoSync *m_dlgIpcInfoSync;
	Dlg_HardWare	*m_dlgHardWare;
	QSignalMapper	*m_singalMap;
};
