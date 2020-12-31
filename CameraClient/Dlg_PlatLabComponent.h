#pragma once

#include <QWidget>
#include "ui_Dlg_PlatLabComponent.h"
#include <QMap>
#define GBUTF8(obj) QString::fromLocal8Bit(obj)
class Dlg_PlatLabComponent : public QWidget
{
	Q_OBJECT

public:
	Dlg_PlatLabComponent(QWidget *parent = Q_NULLPTR);
	~Dlg_PlatLabComponent();

	bool GetCheckedStatus() { return ui.chk_enable->isChecked(); }

	void SetCheckedStatus(bool is){ ui.chk_enable->setChecked(is); }

	void SetLabIP(QString sIP){ ui.lab_ip->setText(sIP); }

	QString GetLabIP(){ return ui.lab_ip->text(); }

	void SetLabPort(QString sPort){ ui.lab_port->setText(sPort); }

	QString GetLabPort(){ return ui.lab_port->text(); }

	void SetProtocolName(int nIndex);

	public slots:
	void Slot_ExpandClicked();
signals:

	void signal_expandClicked();

private:
	Ui::Dlg_PlatLabComponent ui;

	QMap<int, QString> m_map;

};
