#pragma once

#include <QWidget>
#include "ui_Dlg_LightConfig.h"
#include <subject.h>
#include "ivhs_struct.h"
#include "define.h"
#include <QMap>
#include <QLineEdit>
#include "MyWidget.h"
#include "Dlg_MessageBox.h"
#define GBUTF8(da) QString::fromLocal8Bit(da)
class Dlg_LightConfig : public MyWidget, public CObserver
{
	Q_OBJECT

public:
	Dlg_LightConfig(QWidget *parent = Q_NULLPTR);

	~Dlg_LightConfig();

	void InitData(bool is /* = true */);

	void Init();

	void OnSetTxtVisible();

	void OnObserverNotify(LPARAM lHint, LPVOID pHint);

	void UpdateData_Light(bool bUpdate);

	bool GetLightParam();

	void SetLightParam();
public slots:
	void Slot_DataChanged();

	void Slot_BtnSaveClicked();

signals:
	void signal_dataChanged();
private:
	Ui::Dlg_LightConfig ui;
	Dlg_MessageBox *m_messageBox;
	QWidget *m_parent;

	TIPC_LightParam          m_tLightParam;

	QMap<QString, QLineEdit*> m_map;
};
