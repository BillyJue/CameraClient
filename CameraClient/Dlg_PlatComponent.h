#pragma once

#include <QWidget>
#include "ui_Dlg_PlatComponent.h"
#include <QLineEdit>
#include <QMap>
#include <QSignalMapper>
#include "ivhs_struct.h"
#include "define.h"
#define DELETE_PTR(obj) if(obj){delete obj; obj = nullptr;}
#define GBUTF8(obj) QString::fromLocal8Bit(obj)
class Dlg_PlatComponent : public QWidget
{
	Q_OBJECT

public:
	Dlg_PlatComponent(QWidget *parent = Q_NULLPTR);

	~Dlg_PlatComponent();

	void Init();

	void SetReportedChecked(unsigned int ch);

	unsigned int GetReported();

	void SetCheckEnable(bool is);

	bool GetCheckEnable();

	void SetUFT8Checked(bool is){ ui.chk_utf8->setChecked(is); }

	bool GetUFT8Checked(){ return ui.chk_utf8->isChecked(); }

	void SetEditIP(QString sIP);

	QString GetEditIP();

	void SetEditPort(QString sPort);

	QString GetEditPort();

	void SetUUID(QString sId);

	QString GetUUID();

	void SetEditUsrName(QString sName);

	QString GetEditUsrName();

	void SetEditPassword(QString sPwd);

	QString GetEditPassword();

	void SetEditOverSavePath(QString sPath);

	QString GetEditOverSavePath();

	void SetProCmbIndex(int nIndex);

	int GetProCmbIndex();

	void SetEditRule(QString sRule);

	QString GetEditRule();

	QString GetEditIllegalPath();

	void SetEditIllegalPath(QString sRule);

	void SetEditSelf(QString sRule);

	QString GetEditSelf();
	
	void SetEnableCheck(bool is){ ui.chk_enable->setChecked(is); }

	bool GetEnableCheck(){ return ui.chk_enable->isChecked(); }

public slots:
	void Slot_CmbIndexChanged(int nSel);

	void Slot_BtnClearClicked();

	void Slot_AddClicked();

	void Slot_CheckClicked(QWidget *pWidget);

	void Slot_CarInfoClicked(int nStatus);
signals:
	void signal_btnClicked();

private:
	Ui::Dlg_PlatComponent ui;

	QLineEdit *m_ledt_uuid;
	QLineEdit *m_ledt_usr;
	QLineEdit *m_ledt_pwd;
	QLineEdit *m_ledt_overSavePath;//过车存储路径
	QLineEdit *m_ledt_illegalSavePath;//违法存储路径
	QLineEdit *m_ledt_nameRule;
	QLineEdit *m_ledt_self;

	QMap<QString, QCheckBox*> m_map;

	QMap<int, QString> m_spit;
	unsigned int m_dwDataType;
	QMap<QCheckBox*, int> mapTypeIdx;
	QSignalMapper *m_signalMapper;
};
