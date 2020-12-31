#pragma once

#include <QWidget>
#include "ui_Dlg_MessageBox.h"
#include <QDialog>
class Dlg_MessageBox : public QDialog
{
	Q_OBJECT

public:
	Dlg_MessageBox(QWidget *parent = Q_NULLPTR);
	~Dlg_MessageBox();

	void SetTitleInfo(QString sTitle);

	void SetInfoText(QString sInfo);

	void SetBtnNoVisible(bool is);

	void SetBtnYesVisible(bool is);

public slots:
	void Slot_BtnYesClicked();

	void Slot_BtnNoClicked();

private:
	Ui::Dlg_MessageBox ui;
	
};
