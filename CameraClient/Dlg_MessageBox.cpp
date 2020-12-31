#include "Dlg_MessageBox.h"

Dlg_MessageBox::Dlg_MessageBox(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	setWindowFlags( Qt::CustomizeWindowHint);
	//setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint& ~Qt::WindowMinimizeButtonHint);
	setFixedSize(382, 256);
	setWindowTitle("");
	connect(ui.btn_yes, SIGNAL(clicked()), this, SLOT(Slot_BtnYesClicked()));
	connect(ui.btn_no, SIGNAL(clicked()), this, SLOT(Slot_BtnNoClicked()));

}

Dlg_MessageBox::~Dlg_MessageBox()
{

}

void Dlg_MessageBox::SetBtnNoVisible(bool is)
{
	ui.btn_no->setVisible(is);
}

void Dlg_MessageBox::SetBtnYesVisible(bool is)
{
	ui.btn_yes->setVisible(is);
}

void Dlg_MessageBox::Slot_BtnNoClicked()
{
	this->done(0);
}

void Dlg_MessageBox::Slot_BtnYesClicked()
{
	this->done(1);
}
void Dlg_MessageBox::SetTitleInfo(QString sTitle)
{
	ui.lab_title->setText(sTitle);
}

void Dlg_MessageBox::SetInfoText(QString sInfo)
{
	ui.lab_info->setText(sInfo);
}