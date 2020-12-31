#pragma once

#include <QWidget>
#include "ui_Dlg_velometer.h"
#include <windows.h>
#include <subject.h>
#include "ivhs_struct.h"
#include "MyWidget.h"
#include <QPixmap>
#define GBUTF8(da) QString::fromLocal8Bit(da)
class Dlg_velometer : public MyWidget, public CObserver
{
	Q_OBJECT

public:
	Dlg_velometer(QWidget *parent = Q_NULLPTR);

	~Dlg_velometer();

	void InitData(bool is);

	void OnObserverNotify(LPARAM lHint, LPVOID pHint);

	void SetParam();

	bool GetParam();

	void SaveConfig();

public slots:
	void slot_dataChanged();

	void slot_saveConfig();

signals:
	void dataChanged();//回调有数据，更新界面数据
private:
	Ui::Dlg_velometer ui;

	TIPC_VidSpdDet			m_tCfg;

};
