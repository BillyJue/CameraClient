#pragma once

#include <QWidget>
#include "ui_Dlg_OsdAddConfig.h"
#include <subject.h>
#include "ivhs_struct.h"
#include "define.h"
#include <QMap>
#include <QLineEdit>
#include <QListWidget>
#include <QMap>
#include <QCheckBox>
#include <QSignalMapper>
#include "MyWidget.h"
#include "Dlg_MessageBox.h"
#define MAX_OSD_OPTION 52
#define GBUTF8(da) QString::fromLocal8Bit(da)
class Dlg_OsdAddConfig : public MyWidget, public CObserver
{
	Q_OBJECT

public:
	Dlg_OsdAddConfig(QWidget *parent = Q_NULLPTR);

	~Dlg_OsdAddConfig();

	void Init();

	void InitData(bool is /* = true */);

	void OnObserverNotify(LPARAM lHint, LPVOID pHint);

	void OnSetTxtVisible();

	bool GetParam();

	bool SetParam();

	bool SetOsdFontParam();

	void UpdateData(bool bUpdate = true);

	void ParserStr(std::wstring &str, bool updateChk);

public slots:
	void Slot_TabCurrentIndexChanged();

	void Slot_FontSettingClicked();

	void Slot_OsdInfo();

	void Slot_OsdFontInfo();

	void Slot_DefaultOsdClicked();

	void Slot_SaveClicked();

	void Slot_FreshClicked();

	void Slot_WordWrapClicked();

	void Slot_ClearFormatClicked();

	void Slot_CheckClicked(QWidget *pWidget);
signals:
	void signal_osdInfo();

	void signal_osdFontInfo();
private:
	Ui::Dlg_OsdAddConfig ui;
	Dlg_MessageBox	*m_messageBox;
	TOsdCfgInfo		m_tData;
	TOsdFontCfg		m_tFont;
	u32				m_nOsdType;
	QMap<QString, QCheckBox*> m_map;
	QSignalMapper *m_signalMapper;
};
