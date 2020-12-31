#include "MyMessageBox.h"

MyMessageBox::MyMessageBox(int w, int h, QWidget *parent) :QMessageBox(parent), m_nWidth(w), m_nHeight(h)
{
	
}

void MyMessageBox::showEvent(QShowEvent* event)
{
	if (m_nWidth > 0 && m_nHeight > 0)
	{
		setFixedSize(m_nWidth, m_nHeight);
	}
}

void MyMessageBox::resizeEvent(QResizeEvent*event)
{
	setFixedSize(m_nWidth, m_nHeight);
}