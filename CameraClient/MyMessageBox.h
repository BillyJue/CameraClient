#pragma once

#include <QObject>
#include <QMessageBox>
class MyMessageBox : public QMessageBox
{
public:
	MyMessageBox(int w = 0, int h = 0, QWidget *parent = Q_NULLPTR);

protected:
	void showEvent(QShowEvent* event);

	void resizeEvent(QResizeEvent*event);

private:
	int m_nWidth;
	int m_nHeight;

	//{
	//	QMessageBox::showEvent(event);
	//	//setFixedSize(640, 480);
	//}
};