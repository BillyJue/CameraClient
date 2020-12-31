#pragma once

#include <QWidget>
#include "ui_Dlg_SignalConfig.h"
#include <windows.h>
#include <subject.h>
#include "ivhs_struct.h"
#include "MyWidget.h"
const int IP_STR_LEN = 15;
const int TOKEN_LEN = 4;
#define GBUTF8(da) QString::fromLocal8Bit(da)
class Dlg_SignalConfig : public MyWidget, public CObserver
{
	Q_OBJECT

public:
	Dlg_SignalConfig(QWidget *parent = Q_NULLPTR);

	~Dlg_SignalConfig();

	void Init();

	void InitData(bool is /* = true */);

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
	Ui::Dlg_SignalConfig ui;

	TIPC_Signal m_tData;
};
