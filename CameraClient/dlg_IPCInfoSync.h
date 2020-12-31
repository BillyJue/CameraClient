#pragma once

#include <QWidget>
#include "ui_Dlg_IPCInfoSync.h"
#include <windows.h>
#include <subject.h>
#include "ivhs_struct.h"
#include "MyWidget.h"
#define GBUTF8(da) QString::fromLocal8Bit(da)
class Dlg_IPCInfoSync : public MyWidget, public CObserver
{
	Q_OBJECT

public:
	Dlg_IPCInfoSync(QWidget *parent = Q_NULLPTR);

	~Dlg_IPCInfoSync();

	void Init();

	void InitData(bool is /* = true */);

	void OnObserverNotify(LPARAM lHint, LPVOID pHint);

	void SetParam();

	bool GetParam();

	void SaveConfig();

	public slots:

	void slot_dataChanged();

	void slot_saveConfig();

	void Slot_VideoRedStatusChanged(int nValue);

signals:
	void dataChanged();//回调有数据，更新界面数据
private:
	Ui::Dlg_IPCInfoSync ui;

	TIPC_GetInfoSyncCfgRsp m_infoSyncCfg;
};
