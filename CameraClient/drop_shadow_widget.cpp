#include "drop_shadow_widget.h"

DropShadowWidget::DropShadowWidget(QWidget *parent)
	: QDialog(parent)
{
	setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
	//setAttribute(Qt::WA_TranslucentBackground);

	//��ʼ��Ϊδ����������
	mouse_press = false;
}

DropShadowWidget::~DropShadowWidget()
{

}

void DropShadowWidget::paintEvent(QPaintEvent *)
{
  
}

void DropShadowWidget::mousePressEvent(QMouseEvent *event)
{
	//ֻ�����������ƶ��͸ı��С
	if(event->button() == Qt::LeftButton) 
	{
		mouse_press = true;
	}

	//�����ƶ�����
	move_point = event->globalPos() - pos(); 
}

void DropShadowWidget::mouseReleaseEvent(QMouseEvent *)
{
	mouse_press = false;
}

void DropShadowWidget::mouseMoveEvent(QMouseEvent *event)
{
	//�ƶ�����
	if(mouse_press)   
	{
		QPoint move_pos = event->globalPos();
		move(move_pos - move_point);
	}
}