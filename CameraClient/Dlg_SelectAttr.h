#pragma once

#include <QDialog>
#include "ui_Dlg_SelectAttr.h"
#include "ivhs_struct.h"
class Dlg_SelectAttr : public QDialog
{
	Q_OBJECT

public:
	Dlg_SelectAttr(QWidget *parent = Q_NULLPTR);

	~Dlg_SelectAttr();

	Uint32 GetData();
	
	void SetData(Uint32 nAttr);

private:
	Ui::Dlg_SelectAttr ui;
};
