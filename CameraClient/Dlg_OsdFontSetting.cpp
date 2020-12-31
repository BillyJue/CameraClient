#include "Dlg_OsdFontSetting.h"
#include <QListView>
Dlg_OsdFontSetting::Dlg_OsdFontSetting(TOsdFontCfg *osdfont,QWidget *parent)
	: QDialog(parent), m_pFontCfg(osdfont)
{
	setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
	ui.setupUi(this);
	connect(ui.btn_save, SIGNAL(clicked()), this, SLOT(Slot_SaveClicked()));
	connect(ui.btn_exit, SIGNAL(clicked()), this, SLOT(Slot_ExitClicked()));
	Init();
	UpdateData();
}

void Dlg_OsdFontSetting::Init()
{
	ui.cmb_font->setView(new  QListView());
	ui.cmb_font->addItem(GBUTF8("原字大小"));
	ui.cmb_font->addItem(GBUTF8("原字2倍"));
	ui.cmb_font->addItem(GBUTF8("原字3倍"));
	ui.cmb_font->addItem(GBUTF8("原字4倍"));
	ui.cmb_font->addItem(GBUTF8("原字5倍"));
	ui.cmb_font->addItem(GBUTF8("原字6倍"));
	ui.cmb_font->addItem(GBUTF8("原字7倍"));
	ui.cmb_font->addItem(GBUTF8("原字8倍"));
	ui.cmb_font->addItem(GBUTF8("原字9倍"));
	ui.cmb_font->addItem(GBUTF8("原字10倍"));

	ui.cmb_fontColor->addItem(GBUTF8("白色"));
	ui.cmb_fontColor->addItem(GBUTF8("红色"));
	ui.cmb_fontColor->addItem(GBUTF8("黄色"));
	ui.cmb_fontColor->addItem(GBUTF8("绿色"));
	ui.cmb_fontColor->addItem(GBUTF8("蓝色"));
	ui.cmb_fontColor->addItem(GBUTF8("紫色"));
	ui.cmb_fontColor->addItem(GBUTF8("黑色"));
	ui.cmb_fontColor->addItem(GBUTF8("黑白色"));
	ui.cmb_fontColor->setView(new  QListView());
	

}


void Dlg_OsdFontSetting::Slot_SaveClicked()
{
	UpdateData(false);
	this->done(1);
}

void Dlg_OsdFontSetting::UpdateData(bool bUpdate /*= true*/)
{
	// 从数据更新到界面
	if (bUpdate)
	{
		ui.cmb_font->setCurrentIndex(m_pFontCfg->atFont[0].dwFontSize - 1);
		ui.cmb_fontColor->setCurrentIndex(m_pFontCfg->atFont[0].dwFontColor);

	}
	// 从界面更新到数据
	else
	{
		for (int i = 0; i < MAX_OSD_TYPE_COUNT; i++)
		{
			m_pFontCfg->atFont[i].dwFontSize = ui.cmb_font->currentIndex() + 1;

			m_pFontCfg->atFont[i].dwFontColor = ui.cmb_fontColor->currentIndex();
			if (m_pFontCfg->atFont[i].dwFontColor == 7)
			{
				m_pFontCfg->atFont[i].dwFontColor = 8;
			}
		}
	}
}



void Dlg_OsdFontSetting::Slot_ExitClicked()
{
	this->done(0);
}

Dlg_OsdFontSetting::~Dlg_OsdFontSetting()
{
}
