#include "dlg_localConfig.h"
#include "MgrData.h"
#include <QFileDialog>
#include <string.h>
#include <QTimer>
Dlg_LocalConfig::Dlg_LocalConfig(QWidget *parent)
	: MyWidget(parent), m_buttonGroup(nullptr), m_messageBox(nullptr)
{
	ui.setupUi(this);
	connect(ui.btn_save, SIGNAL(clicked()), this, SLOT(Slot_BtnSaveClicked()));
	connect(ui.btn_picScan, SIGNAL(clicked()), this, SLOT(Slot_BtnPictureScanClicked()));
	connect(ui.btn_videoScan, SIGNAL(clicked()), this, SLOT(Slot_BtnVideoScanClicked()));
	MgrData::getInstance()->GetMgrSubject()->Add(this);
	
	wchar_t ws[64];
	char2wchar(ws, MgrData::getInstance()->GetMgrVersion()->GetVersion());
	ui.lb_softversion->setText(QString::fromWCharArray(ws));
	ui.ledt_picPath->setEnabled(false);
	ui.ledt_videoPath->setEnabled(false);
	m_buttonGroup = new QButtonGroup;
	m_buttonGroup->setExclusive(true);
	m_buttonGroup->addButton(ui.check_pic);
	m_buttonGroup->addButton(ui.check_video);
}

void Dlg_LocalConfig::OnSetTxtVisible()
{
	if (m_messageBox)
	{
		m_messageBox->close();
		SAFE_DELETE(m_messageBox);
	}
}

void Dlg_LocalConfig::Slot_BtnSaveClicked()
{
	SetParam();
	if (!m_messageBox)
	{
		QTimer::singleShot(1500, this, &Dlg_LocalConfig::OnSetTxtVisible);
		m_messageBox = new Dlg_MessageBox;
		m_messageBox->SetInfoText(GBUTF8("保存成功！"));
		m_messageBox->SetBtnNoVisible(false);
		m_messageBox->exec();
	}
}

void Dlg_LocalConfig::InitData(bool is)
{
	if (is)
	{
		GetParam();
	}
}


void Dlg_LocalConfig::Slot_BtnPictureScanClicked()
{
	QString sPicDirPath = QFileDialog::getExistingDirectory(
		this, "choose Picture Directory", "/");

	if (sPicDirPath.isEmpty())
	{
		return;
	}
	ui.ledt_picPath->setText(sPicDirPath);
}

void Dlg_LocalConfig::Slot_BtnVideoScanClicked()
{
	QString sVideoDirPath = QFileDialog::getExistingDirectory(
		this, "choose Video Directory", "/");

	if (sVideoDirPath.isEmpty())
	{
		return;
	}
	ui.ledt_videoPath->setText(sVideoDirPath);
}

Dlg_LocalConfig::~Dlg_LocalConfig()
{
	MgrData::getInstance()->GetMgrSubject()->Del(this);
	if (m_buttonGroup)
	{
		delete m_buttonGroup;
		m_buttonGroup = nullptr;
	}
	SAFE_DELETE(m_messageBox);
}

//本地获取不需要
void Dlg_LocalConfig::OnObserverNotify(LPARAM lHint, LPVOID pHint)
{

}



void Dlg_LocalConfig::SetParam()
{
	QString sPicPath = ui.ledt_picPath->text();
	memset(m_tLocalCfgInfo.picturepath, 0, MAX_PATH_LEN);
	int num = sPicPath.toStdWString().length();
	std::wstring wsPic = sPicPath.toStdWString();
	memcpy(m_tLocalCfgInfo.picturepath, wsPic.c_str(), num * 2);

	QString sVideoPath = ui.ledt_videoPath->text();
	num = sVideoPath.toStdWString().length();
	std::wstring wsVideo = sVideoPath.toStdWString();
	memset(m_tLocalCfgInfo.videopath, 0, MAX_PATH_LEN);
	memcpy(m_tLocalCfgInfo.videopath, wsVideo.c_str(), num * 2);

	m_tLocalCfgInfo.isrtsp = ui.check_rstp->isChecked();
	m_tLocalCfgInfo.isip = ui.check_ip->isChecked();
	m_tLocalCfgInfo.isyear = ui.check_year->isChecked();
	m_tLocalCfgInfo.ismonth = ui.check_month->isChecked();
	m_tLocalCfgInfo.isday = ui.check_day->isChecked();
	m_tLocalCfgInfo.ishour = ui.check_hour->isChecked();

	m_tLocalCfgInfo.isBayonet = ui.check_BayonetOriginpic->isChecked();
	m_tLocalCfgInfo.isBayonetMontage = ui.check_BayonetComposePic->isChecked();
	m_tLocalCfgInfo.isillegal = ui.check_illegalOriginPic->isChecked();
	m_tLocalCfgInfo.isillegalMontage = ui.check_illegalComposePic->isChecked();
	m_tLocalCfgInfo.isVideo = ui.check_illegalVideo->isChecked();
	m_tLocalCfgInfo.isFace = ui.check_specialFace->isChecked();
	m_tLocalCfgInfo.isVehicle = ui.check_specialCar->isChecked();
	m_tLocalCfgInfo.isPlate = ui.check_CarNumber->isChecked();
	//m_tLocalCfgInfo.isHardware = ui.check_hardTest->isChecked();
	MgrData::getInstance()->GetMgrLocalCfg()->SetLocalCfg(&m_tLocalCfgInfo, true);
	MgrData::getInstance()->GetMgrEvtLst()->UpdateDir();
}

bool Dlg_LocalConfig::GetParam()
{
	MgrData::getInstance()->GetMgrLocalCfg()->GetLocalCfg(&m_tLocalCfgInfo);
	QString sPicPath = QString::fromWCharArray(m_tLocalCfgInfo.picturepath);
	QString sVideoPath = QString::fromWCharArray(m_tLocalCfgInfo.videopath);

	ui.ledt_picPath->setText(sPicPath);
	ui.ledt_videoPath->setText(sVideoPath);
	if (m_tLocalCfgInfo.isrtsp)
	{
		ui.check_pic->setEnabled(false);
		ui.check_video->setEnabled(false);
	}
	else
	{
		ui.check_pic->setEnabled(true);
		ui.check_video->setEnabled(true);
	}
	if (m_tLocalCfgInfo.istcp)
	{
		ui.check_tcp->setChecked(true);
		ui.check_udp->setChecked(false);
	}
	else
	{
		ui.check_tcp->setChecked(false);
		ui.check_udp->setChecked(true);
	}
	if (m_tLocalCfgInfo.ispic)
	{
		ui.check_pic->setChecked(true);
		ui.check_video->setChecked(false);
	}
	else
	{
		ui.check_pic->setChecked(false);
		ui.check_video->setChecked(true);
	}
	ui.check_ip->setChecked(m_tLocalCfgInfo.isip);
	ui.check_year->setChecked(m_tLocalCfgInfo.isyear);
	ui.check_month->setChecked(m_tLocalCfgInfo.ismonth);
	ui.check_day->setChecked(m_tLocalCfgInfo.isday);
	ui.check_hour->setChecked(m_tLocalCfgInfo.ishour);
	if (m_tLocalCfgInfo.isyear == 0)
	{
		ui.check_month->setEnabled(false);
	}
	if (m_tLocalCfgInfo.ismonth == 0)
	{
		ui.check_day->setEnabled(false);
	}
	if (m_tLocalCfgInfo.isday == 0)
	{
		ui.check_hour->setEnabled(false);
	}
	ui.check_BayonetOriginpic->setChecked(m_tLocalCfgInfo.isBayonet);
	ui.check_BayonetComposePic->setChecked(m_tLocalCfgInfo.isBayonetMontage);
	ui.check_illegalOriginPic->setChecked(m_tLocalCfgInfo.isillegal);
	ui.check_illegalComposePic->setChecked(m_tLocalCfgInfo.isillegalMontage);
	ui.check_illegalVideo->setChecked(m_tLocalCfgInfo.isVideo);
	ui.check_specialFace->setChecked(m_tLocalCfgInfo.isFace);
	ui.check_specialCar->setChecked(m_tLocalCfgInfo.isVehicle);
	ui.check_CarNumber->setChecked(m_tLocalCfgInfo.isPlate);
	//ui.check_hardTest->setChecked(m_tLocalCfgInfo.isHardware);

	return true;
}