#include "MyTitleWidget.h"

MyTitleWidget::MyTitleWidget(QWidget *parent)
	: QWidget(parent), maxNormal(false), m_parent(nullptr)
{
	ui.setupUi(this);
	connect(ui.minimizeButton, SIGNAL(clicked()), this, SLOT(Slot_TitleButtonClicked()));
	connect(ui.maximizeButton, SIGNAL(clicked()), this, SLOT(Slot_TitleButtonClicked()));
	connect(ui.closeButton, SIGNAL(clicked()), this, SLOT(Slot_TitleButtonClicked()));
	ui.maximizeButton->setProperty("maximizeProperty", "restore");
}

MyTitleWidget::~MyTitleWidget()
{

}

void MyTitleWidget::Slot_TitleButtonClicked()
{
	if (!m_parent)
	{
		return;
	}
	if (this->sender() == ui.minimizeButton)
	{
		m_parent->showMinimized();
	}
	else if (this->sender() == ui.closeButton)
	{
		m_parent->close();
	}
	else if (this->sender() == ui.maximizeButton)
	{
		if (!maxNormal)
		{
			this->sender()->setProperty("maximizeProperty", "maximize");
			m_parent->showMaximized();
		}
		else
		{
			this->sender()->setProperty("maximizeProperty", "restore");
			m_parent->showNormal();
		}
		maxNormal = !maxNormal;
		emit singal_sizeChanged();

	}
}

void MyTitleWidget::mouseDoubleClickEvent(QMouseEvent *e)
{
	if (e->button() == Qt::LeftButton)
	{
		if (maxNormal) 
		{
			if (m_parent)
			{
				m_parent->showNormal();
			}
			maxNormal = !maxNormal;
		}
		else 
		{
			if (m_parent)
			{
				m_parent->showMaximized();
			}
			maxNormal = !maxNormal;
		}

	}

}

void MyTitleWidget::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_bDrag = true;
		//������ĳ�ʼλ��
		mouseStartPoint = event->pos();
		//��ô��ڵĳ�ʼλ��
		windowTopLeftPoint = this->geometry().topLeft();
	}
}

void MyTitleWidget::mouseMoveEvent(QMouseEvent *event)
{
	if (m_bDrag)
	{
		//�������ƶ��ľ���
		QPoint distance = event->globalPos() - mouseStartPoint;
		//�ı䴰�ڵ�λ��
		m_parent->move(windowTopLeftPoint + distance);
	}
}

void MyTitleWidget::mouseReleaseEvent(QMouseEvent *event)
{
	m_bDrag = false;
}