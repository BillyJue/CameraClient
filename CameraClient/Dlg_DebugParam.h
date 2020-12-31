#pragma once

#include <QWidget>
#include "ui_Dlg_DebugParam.h"
#include <QDialog>
#include <subject.h>
#include "ivhs_struct.h"
#include "define.h"
#define GBUTF8(da) QString::fromLocal8Bit(da)
class Dlg_DebugParam : public QDialog,public CObserver
{
	Q_OBJECT

public:
	Dlg_DebugParam(QWidget *parent = Q_NULLPTR);

	~Dlg_DebugParam();

	void Init();

	void GetDebugParam();

	void SetDebugParam();

	void OnObserverNotify(LPARAM lHint, LPVOID pHint);

public slots:
	void Slot_DataChanged();

	void Slot_BtnSave();

	void Slot_BtnCancel();
signals:
	void data_changed();

private:
	Ui::Dlg_DebugParam ui;

	TIPC_DebugParam m_tIPC_DebugParam;
};
