#pragma once

#include <QWidget>

class MyWidget : public QWidget
{
	Q_OBJECT

public:
	MyWidget(QWidget *parent);

	virtual void InitData(bool is = true);

	virtual	void ReceveVideoStream(bool is = true); //接收视频流

	~MyWidget();
};
