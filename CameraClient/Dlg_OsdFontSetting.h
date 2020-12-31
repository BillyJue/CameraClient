#pragma once

#include <QWidget>
#include "ui_Dlg_OsdFontSetting.h"
#include <subject.h>
#include "ivhs_struct.h"
#include "define.h"
#include <QDialog>
#define GBUTF8(da) QString::fromLocal8Bit(da)
class Dlg_OsdFontSetting : public QDialog
{
	Q_OBJECT

public:
	Dlg_OsdFontSetting(TOsdFontCfg *osdfont, QWidget *parent = Q_NULLPTR);
	~Dlg_OsdFontSetting();
	void Init();
	void UpdateData(bool bUpdate = true);
public slots:
	void Slot_SaveClicked();
	void Slot_ExitClicked();
private:
	Ui::Dlg_OsdFontSetting ui;
	TOsdFontCfg * m_pFontCfg;
};
