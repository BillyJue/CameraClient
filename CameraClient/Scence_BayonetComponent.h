#pragma once

#include <QWidget>
#include "ui_Scence_BayonetComponent.h"

class Scence_BayonetComponent : public QWidget
{
	Q_OBJECT

public:
	Scence_BayonetComponent(QWidget *parent = Q_NULLPTR);
	~Scence_BayonetComponent();

private:
	Ui::Scence_BayonetComponent ui;
};
