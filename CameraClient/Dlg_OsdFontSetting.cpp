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
	ui.cmb_font->addItem(GBUTF8("ԭ�ִ�С"));
	ui.cmb_font->addItem(GBUTF8("ԭ��2��"));
	ui.cmb_font->addItem(GBUTF8("ԭ��3��"));
	ui.cmb_font->addItem(GBUTF8("ԭ��4��"));
	ui.cmb_font->addItem(GBUTF8("ԭ��5��"));
	ui.cmb_font->addItem(GBUTF8("ԭ��6��"));
	ui.cmb_font->addItem(GBUTF8("ԭ��7��"));
	ui.cmb_font->addItem(GBUTF8("ԭ��8��"));
	ui.cmb_font->addItem(GBUTF8("ԭ��9��"));
	ui.cmb_font->addItem(GBUTF8("ԭ��10��"));

	ui.cmb_fontColor->addItem(GBUTF8("��ɫ"));
	ui.cmb_fontColor->addItem(GBUTF8("��ɫ"));
	ui.cmb_fontColor->addItem(GBUTF8("��ɫ"));
	ui.cmb_fontColor->addItem(GBUTF8("��ɫ"));
	ui.cmb_fontColor->addItem(GBUTF8("��ɫ"));
	ui.cmb_fontColor->addItem(GBUTF8("��ɫ"));
	ui.cmb_fontColor->addItem(GBUTF8("��ɫ"));
	ui.cmb_fontColor->addItem(GBUTF8("�ڰ�ɫ"));
	ui.cmb_fontColor->setView(new  QListView());
	

}


void Dlg_OsdFontSetting::Slot_SaveClicked()
{
	UpdateData(false);
	this->done(1);
}

void Dlg_OsdFontSetting::UpdateData(bool bUpdate /*= true*/)
{
	// �����ݸ��µ�����
	if (bUpdate)
	{
		ui.cmb_font->setCurrentIndex(m_pFontCfg->atFont[0].dwFontSize - 1);
		ui.cmb_fontColor->setCurrentIndex(m_pFontCfg->atFont[0].dwFontColor);

	}
	// �ӽ�����µ�����
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
