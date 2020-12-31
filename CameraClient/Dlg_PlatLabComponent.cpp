#include "Dlg_PlatLabComponent.h"

Dlg_PlatLabComponent::Dlg_PlatLabComponent(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	connect(ui.btn_expand, SIGNAL(clicked()), this, SLOT(Slot_ExpandClicked()));
	ui.chk_enable->setEnabled(false);
	m_map.insert(0, GBUTF8("�ƴ�ƽ̨"));
	m_map.insert(1, GBUTF8("����ƽ̨"));
	m_map.insert(2, GBUTF8("������Ծƽ̨"));
	m_map.insert(3, GBUTF8("����ƽ̨"));
	m_map.insert(4, GBUTF8("���ƽ̨"));
	m_map.insert(5, GBUTF8("������ͼ��"));
	m_map.insert(6, GBUTF8("��������"));
	m_map.insert(7, GBUTF8("����ƽ̨"));
	m_map.insert(8, GBUTF8("��������"));
	m_map.insert(9, GBUTF8("������ԣ"));
	m_map.insert(10, GBUTF8("����ƽ̨"));
	m_map.insert(11, GBUTF8("����ƽ̨"));
	m_map.insert(12, GBUTF8("�п�ƽ̨"));
	m_map.insert(13, GBUTF8("��򽻹�ƽ̨"));
}

Dlg_PlatLabComponent::~Dlg_PlatLabComponent()
{
}

void Dlg_PlatLabComponent::Slot_ExpandClicked()
{
	emit signal_expandClicked();
}

void Dlg_PlatLabComponent::SetProtocolName(int nIndex)
{
	QString sName;
	nIndex -= 1;
	if (nIndex < 0)
	{
		sName = "";
	}
	else
	{
		sName = m_map[nIndex];
	}
	ui.lab_pro->setText(sName);
}