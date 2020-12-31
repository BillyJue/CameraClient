#include "Dlg_SelectAttr.h"

Dlg_SelectAttr::Dlg_SelectAttr(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
}

Dlg_SelectAttr::~Dlg_SelectAttr()
{
}

void Dlg_SelectAttr::SetData(Uint32 nAttr)
{
	int nSel = 0;
	switch (nAttr)
	{
	case RULE_LINE_ATTRIBUTE_ShiXian:
	{
		nSel = 0;
		break;
	}
	case RULE_LINE_ATTRIBUTE_XuXian:
	{
		nSel = 1;
		break;
	}
	case RULE_LINE_ATTRIBUTE_ZuoShiYouXu:
	{
		nSel = 2;
		break;
	}
	case RULE_LINE_ATTRIBUTE_ZuoXuYouShi:
	{
		nSel = 3;
		break;
	}
	case RULE_LINE_ATTRIBUTE_GeLiDai:
	{
		nSel = 4;
		break;
	}

	}
	ui.cmb_attr->setCurrentIndex(nSel);
}

Uint32 Dlg_SelectAttr::GetData()
{
	Uint32 nAttr = -1;
	int nSel = ui.cmb_attr->currentIndex();
	switch (nSel)
	{
	case 0:
	{
		nAttr = RULE_LINE_ATTRIBUTE_ShiXian;
		break;
	}
	case 1:
	{
		nAttr = RULE_LINE_ATTRIBUTE_XuXian;
		break;
	}
	case 2:
	{
		nAttr = RULE_LINE_ATTRIBUTE_ZuoShiYouXu;
		break;
	}
	case 3:
	{
		nAttr = RULE_LINE_ATTRIBUTE_ZuoXuYouShi;
		break;
	}
	case 4:
	{
		nAttr = RULE_LINE_ATTRIBUTE_GeLiDai;
		break;
	}
	}
	return nAttr;
}

