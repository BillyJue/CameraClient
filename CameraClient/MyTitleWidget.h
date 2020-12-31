#pragma once

#include <QWidget>
#include "ui_MyTitleWidget.h"
#include <QMouseEvent>
#include <QDialog>
class MyTitleWidget : public QWidget
{
	Q_OBJECT

public:
	MyTitleWidget(QWidget *parent = Q_NULLPTR);

	~MyTitleWidget();

	void SetParentWidget(QDialog *pDlg){ m_parent = pDlg; }

	void mouseDoubleClickEvent(QMouseEvent *e);

	void mouseMoveEvent(QMouseEvent *event);

	void mouseReleaseEvent(QMouseEvent *);

	void mousePressEvent(QMouseEvent *event);


	public slots:
	void Slot_TitleButtonClicked();

signals:
	void singal_sizeChanged();

private:
	Ui::MyTitleWidget ui;
	bool	maxNormal;
	QDialog  *m_parent;
	bool        m_bDrag;
	QPoint      mouseStartPoint;
	QPoint      windowTopLeftPoint;
};
