#pragma once

#include <QWidget>
#include "ui_Scene_IllegalParkComponent.h"

class Scene_IllegalParkComponent : public QWidget
{
	Q_OBJECT

public:
	Scene_IllegalParkComponent(QWidget *parent = Q_NULLPTR);
	~Scene_IllegalParkComponent();

private:
	Ui::Scene_IllegalParkComponent ui;
};
