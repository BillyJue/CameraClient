#pragma once

#include <QWidget>
#include "ui_Dlg_FlowDetection.h"
#include "MyWidget.h"
class Dlg_FlowDetection : public MyWidget
{
	Q_OBJECT

public:
	Dlg_FlowDetection(QWidget *parent = Q_NULLPTR);

	~Dlg_FlowDetection();

	void InitData(bool is);

private:
	Ui::Dlg_FlowDetection ui;
};
