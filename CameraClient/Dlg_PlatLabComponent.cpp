#include "Dlg_PlatLabComponent.h"

Dlg_PlatLabComponent::Dlg_PlatLabComponent(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	connect(ui.btn_expand, SIGNAL(clicked()), this, SLOT(Slot_ExpandClicked()));
	ui.chk_enable->setEnabled(false);
	m_map.insert(0, GBUTF8("科达平台"));
	m_map.insert(1, GBUTF8("宇视平台"));
	m_map.insert(2, GBUTF8("苏州世跃平台"));
	m_map.insert(3, GBUTF8("海康平台"));
	m_map.insert(4, GBUTF8("峰火平台"));
	m_map.insert(5, GBUTF8("公安视图库"));
	m_map.insert(6, GBUTF8("重庆银江"));
	m_map.insert(7, GBUTF8("本能平台"));
	m_map.insert(8, GBUTF8("重庆天网"));
	m_map.insert(9, GBUTF8("重庆企裕"));
	m_map.insert(10, GBUTF8("朗新平台"));
	m_map.insert(11, GBUTF8("扬州平台"));
	m_map.insert(12, GBUTF8("中科平台"));
	m_map.insert(13, GBUTF8("天津交管平台"));
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