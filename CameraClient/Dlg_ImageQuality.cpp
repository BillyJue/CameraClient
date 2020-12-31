#include "Dlg_ImageQuality.h"
#include <MgrData.h>
#include <QIntValidator>
#include <QTimer>
#include <QListView>
Dlg_ImageQuality::Dlg_ImageQuality(QWidget *parent)
	: MyWidget(parent), m_messageBox(nullptr)
{
	ui.setupUi(this);
	ui.cmb_led_mode_3->setView(new QListView);
	ui.cmb_led_outer2_pl_3->setView(new QListView);
	ui.cmb_led_mode_5->setView(new QListView);
	ui.cmb_led_outer2_pl_5->setView(new QListView);
	ui.cmb_led_mode_6->setView(new QListView);
	ui.cmb_led_outer2_pl_6->setView(new QListView);
	ui.cmb_pgj_mode->setView(new QListView);
	ui.cmb_exposure_mode->setView(new QListView);
	ui.cmb_exposure_kmsx->setView(new QListView);
	ui.cmb_exposure_km->setView(new QListView);
	ui.cmb_mode_day->setView(new QListView);
	ui.cmb_mode_night->setView(new QListView);
	ui.cmb_bsd_mode_night->setView(new QListView);
	ui.cmb_bsd_mode_3->setView(new QListView);
	

	//setAttribute(Qt::WA_ShowWithoutActivating, true);
	connect(ui.slider_light_day, SIGNAL(valueChanged(int)), this, SLOT(Slot_SetEditData(int)));
	connect(ui.slider_dbd_day, SIGNAL(valueChanged(int)), this, SLOT(Slot_SetEditData(int)));
	connect(ui.slider_bhd_day, SIGNAL(valueChanged(int)), this, SLOT(Slot_SetEditData(int)));
	connect(ui.slider_rd_day, SIGNAL(valueChanged(int)), this, SLOT(Slot_SetEditData(int)));
	connect(ui.slider_gama_day, SIGNAL(valueChanged(int)), this, SLOT(Slot_SetEditData(int)));
	connect(ui.slider_2d_day, SIGNAL(valueChanged(int)), this, SLOT(Slot_SetEditData(int)));

	connect(ui.slider_light_night, SIGNAL(valueChanged(int)), this, SLOT(Slot_SetEditData(int)));
	connect(ui.slider_dbd_night, SIGNAL(valueChanged(int)), this, SLOT(Slot_SetEditData(int)));
	connect(ui.slider_bhd_night, SIGNAL(valueChanged(int)), this, SLOT(Slot_SetEditData(int)));
	connect(ui.slider_rd_night, SIGNAL(valueChanged(int)), this, SLOT(Slot_SetEditData(int)));
	connect(ui.slider_gama_night, SIGNAL(valueChanged(int)), this, SLOT(Slot_SetEditData(int)));
	connect(ui.slider_2d_night, SIGNAL(valueChanged(int)), this, SLOT(Slot_SetEditData(int)));

	connect(ui.ledt_light_day, SIGNAL(textChanged(const QString &)), this, SLOT(Slot_SetSliderChanged(const QString &)));
	connect(ui.ledt_dbd_day, SIGNAL(textChanged(const QString &)), this, SLOT(Slot_SetSliderChanged(const QString &)));
	connect(ui.ledt_bhd_day, SIGNAL(textChanged(const QString &)), this, SLOT(Slot_SetSliderChanged(const QString &)));
	connect(ui.ledt_rd_day, SIGNAL(textChanged(const QString &)), this, SLOT(Slot_SetSliderChanged(const QString &)));
	connect(ui.ledt_gama_day, SIGNAL(textChanged(const QString &)), this, SLOT(Slot_SetSliderChanged(const QString &)));
	connect(ui.ledt_2d_day, SIGNAL(textChanged(const QString &)), this, SLOT(Slot_SetSliderChanged(const QString &)));
	connect(ui.ledt_light_night, SIGNAL(textChanged(const QString &)), this, SLOT(Slot_SetSliderChanged(const QString &)));
	connect(ui.ledt_dbd_night, SIGNAL(textChanged(const QString &)), this, SLOT(Slot_SetSliderChanged(const QString &)));
	connect(ui.ledt_bhd_night, SIGNAL(textChanged(const QString &)), this, SLOT(Slot_SetSliderChanged(const QString &)));
	connect(ui.ledt_rd_night, SIGNAL(textChanged(const QString &)), this, SLOT(Slot_SetSliderChanged(const QString &)));
	connect(ui.ledt_gama_night, SIGNAL(textChanged(const QString &)), this, SLOT(Slot_SetSliderChanged(const QString &)));
	connect(ui.ledt_2d_night, SIGNAL(textChanged(const QString &)), this, SLOT(Slot_SetSliderChanged(const QString &)));

	connect(this, SIGNAL(signal_imageQuatityChanged()), this, SLOT(Slot_ImageQuatityChanged()));
	connect(ui.tab1, SIGNAL(clicked()), this, SLOT(Slot_TabIndexChanged()));
	connect(ui.tab2, SIGNAL(clicked()), this, SLOT(Slot_TabIndexChanged()));
	connect(ui.tab3, SIGNAL(clicked()), this, SLOT(Slot_TabIndexChanged()));
	connect(ui.tab4, SIGNAL(clicked()), this, SLOT(Slot_TabIndexChanged()));
	connect(ui.tab5, SIGNAL(clicked()), this, SLOT(Slot_TabIndexChanged()));
	connect(ui.tab6, SIGNAL(clicked()), this, SLOT(Slot_TabIndexChanged()));
	connect(ui.tab7, SIGNAL(clicked()), this, SLOT(Slot_TabIndexChanged()));

	m_mapTemp1.insert(ui.slider_light_day, ui.ledt_light_day);
	m_mapTemp1.insert(ui.slider_dbd_day, ui.ledt_dbd_day);
	m_mapTemp1.insert(ui.slider_bhd_day, ui.ledt_bhd_day);
	m_mapTemp1.insert(ui.slider_rd_day, ui.ledt_rd_day);
	m_mapTemp1.insert(ui.slider_gama_day, ui.ledt_gama_day);
	m_mapTemp1.insert(ui.slider_2d_day, ui.ledt_2d_day);
	m_mapTemp1.insert(ui.slider_light_night, ui.ledt_light_night);
	m_mapTemp1.insert(ui.slider_dbd_night, ui.ledt_dbd_night);
	m_mapTemp1.insert(ui.slider_bhd_night, ui.ledt_bhd_night);
	m_mapTemp1.insert(ui.slider_rd_night, ui.ledt_rd_night);
	m_mapTemp1.insert(ui.slider_gama_night, ui.ledt_gama_night);
	m_mapTemp1.insert(ui.slider_2d_night, ui.ledt_2d_night);

	m_mapTemp2.insert(ui.ledt_light_day, ui.slider_light_day);
	m_mapTemp2.insert(ui.ledt_dbd_day, ui.slider_dbd_day);
	m_mapTemp2.insert(ui.ledt_bhd_day, ui.slider_bhd_day);
	m_mapTemp2.insert(ui.ledt_rd_day, ui.slider_rd_day);
	m_mapTemp2.insert(ui.ledt_gama_day, ui.slider_gama_day);
	m_mapTemp2.insert(ui.ledt_2d_day, ui.slider_2d_day);
	m_mapTemp2.insert(ui.ledt_light_night, ui.slider_light_night);
	m_mapTemp2.insert(ui.ledt_dbd_night, ui.slider_dbd_night);
	m_mapTemp2.insert(ui.ledt_bhd_night, ui.slider_bhd_night);
	m_mapTemp2.insert(ui.ledt_rd_night, ui.slider_rd_night);
	m_mapTemp2.insert(ui.ledt_gama_night, ui.slider_gama_night);
	m_mapTemp2.insert(ui.ledt_2d_night, ui.slider_2d_night);

	
	//ui.page_3->setAttribute(Qt::WA_ShowWithoutActivating, true);
	QMap<QLineEdit*, QSlider*>::Iterator itor;
	for (itor = m_mapTemp2.begin(); itor != m_mapTemp2.end(); itor++)
	{
		QLineEdit *pEdit = itor.key();
		pEdit->setValidator(new QIntValidator(0, 100, this));
	}
	//曝光初始化
	connect(ui.ledt_exposure_zysx, SIGNAL(textChanged(const QString &)), this, SLOT(Slot_SetExposureChanged(const QString &)));
	connect(ui.ledt_exposure_qwld, SIGNAL(textChanged(const QString &)), this, SLOT(Slot_SetExposureChanged(const QString &)));
	connect(ui.ledt_exposure_zy, SIGNAL(textChanged(const QString &)), this, SLOT(Slot_SetExposureChanged(const QString &)));
	connect(ui.ledt_exposure_qwhpld, SIGNAL(textChanged(const QString &)), this, SLOT(Slot_SetExposureChanged(const QString &)));
	connect(ui.ledt_exposure_qwhpldsx, SIGNAL(textChanged(const QString &)), this, SLOT(Slot_SetExposureChanged(const QString &)));
	connect(ui.ledt_exposure_qwhpldxx, SIGNAL(textChanged(const QString &)), this, SLOT(Slot_SetExposureChanged(const QString &)));
	connect(ui.cmb_exposure_mode, SIGNAL(currentIndexChanged(int)), this, SLOT(Slot_SetCmbExposureIndexChanged(int)));
	connect(ui.cmb_exposure_kmsx, SIGNAL(currentIndexChanged(int)), this, SLOT(Slot_SetCmbExposureIndexChanged(int)));
	connect(ui.cmb_exposure_km, SIGNAL(currentIndexChanged(int)), this, SLOT(Slot_SetCmbExposureIndexChanged(int)));

	ui.ledt_exposure_zysx->setValidator(new QIntValidator(0, 100, this));
	ui.ledt_exposure_qwld->setValidator(new QIntValidator(0, 100, this));
	ui.ledt_exposure_zy->setValidator(new QIntValidator(0, 100, this));
	ui.ledt_exposure_qwhpld->setValidator(new QIntValidator(0, 100, this));
	ui.ledt_exposure_qwhpldsx->setValidator(new QIntValidator(0, 100, this));
	ui.ledt_exposure_qwhpldxx->setValidator(new QIntValidator(0, 100, this));

	//白平衡初始化
	connect(ui.cmb_mode_day, SIGNAL(currentIndexChanged(int)), this, SLOT(Slot_SetCmbWhiteBalanceIndexChanged(int)));
	connect(ui.cmb_mode_night, SIGNAL(currentIndexChanged(int)), this, SLOT(Slot_SetWhiteBalanceIndexChanged(int)));
	connect(ui.ledt_QDR_day, SIGNAL(textChanged(const QString &)), this, SLOT(Slot_SetWhiteBalanceChanged(const QString &)));
	connect(ui.ledt_QDB_day, SIGNAL(textChanged(const QString &)), this, SLOT(Slot_SetWhiteBalanceChanged(const QString &)));
	connect(ui.ledt_R_day, SIGNAL(textChanged(const QString &)), this, SLOT(Slot_SetWhiteBalanceChanged(const QString &)));
	connect(ui.ledt_B_day, SIGNAL(textChanged(const QString &)), this, SLOT(Slot_SetWhiteBalanceChanged(const QString &)));
	connect(ui.ledt_QDR_night, SIGNAL(textChanged(const QString &)), this, SLOT(Slot_SetWhiteBalanceChanged(const QString &)));
	connect(ui.ledt_QDB_night, SIGNAL(textChanged(const QString &)), this, SLOT(Slot_SetWhiteBalanceChanged(const QString &)));
	connect(ui.ledt_R_night, SIGNAL(textChanged(const QString &)), this, SLOT(Slot_SetWhiteBalanceChanged(const QString &)));
	connect(ui.ledt_B_night, SIGNAL(textChanged(const QString &)), this, SLOT(Slot_SetWhiteBalanceChanged(const QString &)));
	ui.ledt_QDR_day->setValidator(new QIntValidator(0, 100, this));
	ui.ledt_QDB_day->setValidator(new QIntValidator(0, 100, this));
	ui.ledt_R_day->setValidator(new QIntValidator(0, 100, this));
	ui.ledt_B_day->setValidator(new QIntValidator(0, 100, this));
	ui.ledt_QDR_night->setValidator(new QIntValidator(0, 100, this));
	ui.ledt_QDB_night->setValidator(new QIntValidator(0, 100, this));
	ui.ledt_R_night->setValidator(new QIntValidator(0, 100, this));
	ui.ledt_B_night->setValidator(new QIntValidator(0, 100, this));
	//日夜切换
	connect(ui.ledt_exchange_val, SIGNAL(textChanged(const QString &)), this, SLOT(Slot_SetWhiteBalanceChanged(const QString &)));
	ui.ledt_exchange_val->setValidator(new QIntValidator(0, 100000, this));
	//抓拍
	connect(ui.chk_picSize_day, SIGNAL(stateChanged(int)), this, SLOT(Slot_Snap_SetChkChanged(int)));
	connect(ui.chk_huidu_enable_day_3, SIGNAL(stateChanged(int)), this, SLOT(Slot_Snap_SetChkChanged(int)));
	connect(ui.chk_picSize_night, SIGNAL(stateChanged(int)), this, SLOT(Slot_Snap_SetChkChanged(int)));
	connect(ui.chk_huidu_enable_night, SIGNAL(stateChanged(int)), this, SLOT(Slot_Snap_SetChkChanged(int)));
	connect(ui.cmb_bsd_mode_3, SIGNAL(currentIndexChanged(int)), this, SLOT(Slot_SetCmbSnapIndexChanged(int)));
	connect(ui.cmb_bsd_mode_night, SIGNAL(currentIndexChanged(int)), this, SLOT(Slot_SetCmbSnapIndexChanged(int)));
	connect(ui.ledt_picSize_day_3, SIGNAL(textChanged(const QString &)), this, SLOT(Slot_SetSnapInputChanged(const QString &)));
	connect(ui.ledt_picYz_day_3, SIGNAL(textChanged(const QString &)), this, SLOT(Slot_SetSnapInputChanged(const QString &)));
	connect(ui.ledt_huidu_day_3, SIGNAL(textChanged(const QString &)), this, SLOT(Slot_SetSnapInputChanged(const QString &)));
	connect(ui.cmb_snap_day_bsd_time_3, SIGNAL(textChanged(const QString &)), this, SLOT(Slot_SetSnapInputChanged(const QString &)));
	connect(ui.ledt_snap_day_led_time_3, SIGNAL(textChanged(const QString &)), this, SLOT(Slot_SetSnapInputChanged(const QString &)));
	connect(ui.ledt_snap_day_kmsx_3, SIGNAL(textChanged(const QString &)), this, SLOT(Slot_SetSnapInputChanged(const QString &)));
	connect(ui.ledt_snap_day_zysx_3, SIGNAL(textChanged(const QString &)), this, SLOT(Slot_SetSnapInputChanged(const QString &)));
	
	connect(ui.ledt_picSize_night, SIGNAL(textChanged(const QString &)), this, SLOT(Slot_SetSnapInputChanged(const QString &)));
	connect(ui.ledt_picYz_night, SIGNAL(textChanged(const QString &)), this, SLOT(Slot_SetSnapInputChanged(const QString &)));
	connect(ui.ledt_huidu_night, SIGNAL(textChanged(const QString &)), this, SLOT(Slot_SetSnapInputChanged(const QString &)));
	connect(ui.cmb_snap_night_bsd_time, SIGNAL(textChanged(const QString &)), this, SLOT(Slot_SetSnapInputChanged(const QString &)));
	connect(ui.ledt_snap_night_led_time, SIGNAL(textChanged(const QString &)), this, SLOT(Slot_SetSnapInputChanged(const QString &)));
	connect(ui.ledt_snap_night_kmsx, SIGNAL(textChanged(const QString &)), this, SLOT(Slot_SetSnapInputChanged(const QString &)));
	connect(ui.ledt_snap_night_zysx, SIGNAL(textChanged(const QString &)), this, SLOT(Slot_SetSnapInputChanged(const QString &)));
	connect(ui.ledt_snap_night_R, SIGNAL(textChanged(const QString &)), this, SLOT(Slot_SetSnapInputChanged(const QString &)));
	connect(ui.ledt_snap_night_B, SIGNAL(textChanged(const QString &)), this, SLOT(Slot_SetSnapInputChanged(const QString &)));

	ui.ledt_picSize_day_3->setValidator(new QIntValidator(100, 3000, this));
	ui.ledt_picYz_day_3->setValidator(new QIntValidator(0, 100, this));
	ui.ledt_huidu_day_3->setValidator(new QIntValidator(0, 100, this));
	ui.cmb_snap_day_bsd_time_3->setValidator(new QIntValidator(0, 1000, this));
	ui.ledt_snap_day_led_time_3->setValidator(new QIntValidator(0, 10000, this));
	ui.ledt_snap_day_zysx_3->setValidator(new QIntValidator(0, 1000, this));
	ui.ledt_snap_day_kmsx_3->setValidator(new QIntValidator(0, 10000, this));
	
	ui.ledt_picSize_night->setValidator(new QIntValidator(100, 3000, this));
	ui.ledt_picYz_night->setValidator(new QIntValidator(0, 100, this));
	ui.ledt_huidu_night->setValidator(new QIntValidator(0, 100, this));
	ui.cmb_snap_night_bsd_time->setValidator(new QIntValidator(0, 1000, this));
	ui.ledt_snap_night_led_time->setValidator(new QIntValidator(0, 10000, this));
	ui.ledt_snap_night_zysx->setValidator(new QIntValidator(0, 1000, this));
	ui.ledt_snap_night_kmsx->setValidator(new QIntValidator(0, 10000, this));
	ui.ledt_snap_night_R->setValidator(new QIntValidator(0, 100, this));
	ui.ledt_snap_night_B->setValidator(new QIntValidator(0, 100, this));
	//LED
	connect(ui.radio_open_3, SIGNAL(toggled(bool)), this, SLOT(Slot_LedRadioChanged(bool)));
	connect(ui.radio_open_5, SIGNAL(toggled(bool)), this, SLOT(Slot_LedRadioChanged(bool)));
	connect(ui.radio_open_6, SIGNAL(toggled(bool)), this, SLOT(Slot_LedRadioChanged(bool)));
	connect(ui.radio_close_3, SIGNAL(toggled(bool)), this, SLOT(Slot_LedRadioChanged(bool)));
	connect(ui.radio_close_5, SIGNAL(toggled(bool)), this, SLOT(Slot_LedRadioChanged(bool)));
	connect(ui.radio_close_6, SIGNAL(toggled(bool)), this, SLOT(Slot_LedRadioChanged(bool)));
	connect(ui.cmb_led_mode_3, SIGNAL(currentIndexChanged(int)), this, SLOT(Slot_SetCmbLEDIndexChanged(int)));
	connect(ui.cmb_led_mode_5, SIGNAL(currentIndexChanged(int)), this, SLOT(Slot_SetCmbLEDIndexChanged(int)));
	connect(ui.cmb_led_mode_6, SIGNAL(currentIndexChanged(int)), this, SLOT(Slot_SetCmbLEDIndexChanged(int)));
	connect(ui.cmb_led_outer2_pl_3, SIGNAL(currentIndexChanged(int)), this, SLOT(Slot_SetCmbLEDIndexChanged(int)));
	connect(ui.cmb_led_outer2_pl_5, SIGNAL(currentIndexChanged(int)), this, SLOT(Slot_SetCmbLEDIndexChanged(int)));
	connect(ui.cmb_led_outer2_pl_6, SIGNAL(currentIndexChanged(int)), this, SLOT(Slot_SetCmbLEDIndexChanged(int)));
	connect(ui.ledt_led_outer1_mk_3, SIGNAL(textChanged(const QString &)), this, SLOT(Slot_SetLEDInputChanged(const QString &)));
	connect(ui.ledt_led_outer1_mk_5, SIGNAL(textChanged(const QString &)), this, SLOT(Slot_SetLEDInputChanged(const QString &)));
	connect(ui.ledt_led_outer1_mk_6, SIGNAL(textChanged(const QString &)), this, SLOT(Slot_SetLEDInputChanged(const QString &)));
	ui.ledt_led_outer1_mk_3->setValidator(new QIntValidator(0, 6000, this));
	ui.ledt_led_outer1_mk_5->setValidator(new QIntValidator(0, 6000, this));
	ui.ledt_led_outer1_mk_6->setValidator(new QIntValidator(0, 6000, this));
	//偏光镜
	connect(ui.cmb_pgj_mode, SIGNAL(currentIndexChanged(int)), this, SLOT(Slot_SetCmbLEDIndexChanged(int)));
	connect(ui.chk_pgj_open, SIGNAL(toggled(bool)), this, SLOT(Slot_LedRadioChanged(bool)));
	connect(ui.chk_pgj_close, SIGNAL(toggled(bool)), this, SLOT(Slot_LedRadioChanged(bool)));
	connect(ui.ledt_pgj_start_es, SIGNAL(textChanged(const QString &)), this, SLOT(Slot_SetLEDInputChanged(const QString &)));
	connect(ui.ledt_pgj_end_es, SIGNAL(textChanged(const QString &)), this, SLOT(Slot_SetLEDInputChanged(const QString &)));
	ui.ledt_pgj_start_es->setValidator(new QIntValidator(0, 6000, this));
	ui.ledt_pgj_end_es->setValidator(new QIntValidator(0, 6000, this));

	connect(ui.btn_save_tab1, SIGNAL(clicked()), this, SLOT(Slot_BtnSaveClicked()));
	connect(ui.btn_save_tab2, SIGNAL(clicked()), this, SLOT(Slot_BtnSaveClicked()));
	connect(ui.btn_save_tab3, SIGNAL(clicked()), this, SLOT(Slot_BtnSaveClicked()));
	connect(ui.btn_save_tab4, SIGNAL(clicked()), this, SLOT(Slot_BtnSaveClicked()));
	connect(ui.btn_save_tab5, SIGNAL(clicked()), this, SLOT(Slot_BtnSaveClicked()));
	connect(ui.btn_save_tab5_2, SIGNAL(clicked()), this, SLOT(Slot_BtnSaveClicked()));
	connect(ui.btn_save_tab7, SIGNAL(clicked()), this, SLOT(Slot_BtnSaveClicked()));
	if (MgrData::getInstance()->GetMgrEvtLst())
	{
		MgrData::getInstance()->GetMgrEvtLst()->AddSender(static_cast<CObserver*>(this));
	}
	MgrData::getInstance()->GetMgrSubject()->Add(this);

	//test
	//InitData(true);
}

void Dlg_ImageQuality::Slot_BtnSaveClicked()
{
	SetParam();
	if (!m_messageBox)
	{
		QTimer::singleShot(1500, this, &Dlg_ImageQuality::OnSetTxtVisible);
		m_messageBox = new Dlg_MessageBox;
		m_messageBox->SetInfoText(GBUTF8("保存成功！"));
		m_messageBox->SetBtnNoVisible(false);
		m_messageBox->exec();
	}
}

void Dlg_ImageQuality::OnSetTxtVisible()
{
	if (m_messageBox)
	{
		m_messageBox->close();
		SAFE_DELETE(m_messageBox);
	}
}

void Dlg_ImageQuality::SetParam()
{
	m_tCfg.atExposure->dwMaxShuter = OnShutterValueToIpc(m_nMaxShutter);
	m_tCfg.atExposure->dwManuShutter = OnShutterValueToIpc(m_nManuShutter);

	MgrItsCtrl* pMgrItsCtrl = MgrData::getInstance()->GetMgrItsCtrl();
	if (pMgrItsCtrl != NULL)
	{
		u32 dwIP = MgrData::getInstance()->GetIP();
		ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dwIP);
		if (pCtrl != NULL)
		{
			bool bRet = pCtrl->SetIpcImageQuality(&m_tCfg);

			if (!bRet)
			{
			}
		}
	}
	
}

// 将界面快门参数转换成相机参数
Uint32 Dlg_ImageQuality::OnShutterValueToIpc(Uint32 index)
{
	Uint32 shutter = 0;
	Uint32 value = 1000000;

	switch (index)
	{
	case 0: // 1/1
		shutter = value / 1;
		break;
	case 1: // 1/2
		shutter = value / 2;
		break;
	case 2: // 1/7.5
		shutter = value / 7.5;
		break;
	case 3: // 1/10
		shutter = value / 10;
		break;
	case 4: // 1/15
		shutter = value / 15;
		break;
	case 5: // 1/25
		shutter = value / 25;
		break;
	case 6: // 1/50
		shutter = value / 50;
		break;
	case 7: // 1/100
		shutter = value / 100;
		break;
	case 8: // 1/200
		shutter = value / 200;
		break;
	case 9: // 1/240
		shutter = value / 240;
		break;
	case 10: //1/300
		shutter = value / 300;
		break;
	case 11: // 1/400
		shutter = value / 400;
		break;
	case 12: // 1/480
		shutter = value / 480;
		break;
	case 13: // 1/960
		shutter = value / 960;
		break;
	case 14: // 1/1024
		shutter = value / 1024;
		break;
	case 15: // 1/2000
		shutter = value / 2000;
		break;
	case 16: // 1/4000
		shutter = value / 4000;
		break;
	case 17: // 1/8000
		shutter = value / 8000;
		break;
	case 18: // 1/16000
		shutter = value / 16000;
		break;
	case 19: // 1/30000
		shutter = value / 30000;
		break;
	case 20: // 1/100000
		shutter = value / 100000;
		break;
	case 21: // 1/124000
		shutter = value / 124000;
		break;
	default:
		shutter = value / 200;
		break;
	}

	return shutter;
}

void Dlg_ImageQuality::Slot_SetLEDInputChanged(const QString &)
{
	QLineEdit *pEdit = dynamic_cast<QLineEdit*>(this->sender());
	if (pEdit == ui.ledt_led_outer1_mk_3)
	{
		m_tCfg.atLedLight[0].dwLedLightTime = pEdit->text().toInt();
	}
	else if (pEdit == ui.ledt_led_outer1_mk_5)
	{
		m_tCfg.atLedLight[1].dwLedLightTime = pEdit->text().toInt();
	}
	else if (pEdit == ui.ledt_led_outer1_mk_6)
	{
		m_tCfg.atLedLight[2].dwLedLightTime = pEdit->text().toInt();
	}
	else if (pEdit == ui.ledt_pgj_start_es)
	{
		m_tCfg.tPLCtrl.dwPLenableEs = pEdit->text().toInt();
	}
	else if (pEdit == ui.ledt_pgj_end_es)
	{
		m_tCfg.tPLCtrl.dwPLdisableEs = pEdit->text().toInt();
	}
}

void Dlg_ImageQuality::Slot_SetCmbLEDIndexChanged(int nIndex)
{
	QComboBox *pCmb = dynamic_cast<QComboBox*>(this->sender());
	if (pCmb == ui.cmb_led_mode_3)
	{
		if (nIndex == 1)
		{
			m_tCfg.atLedLight[0].dwLedCtrlMode = 0x4;
			ui.widget_6->setVisible(true);
		}
		else
		{
			if (nIndex == 0)
			{
				m_tCfg.atLedLight[0].dwLedCtrlMode = 0x1;
			}
			if (nIndex == 2)
			{
				m_tCfg.atLedLight[0].dwLedCtrlMode = 0x16;
			}
			ui.widget_6->setVisible(false);
		}
	}
	else if (pCmb == ui.cmb_led_mode_5)
	{
		if (nIndex == 1)
		{
			m_tCfg.atLedLight[1].dwLedCtrlMode = 0x4;
			ui.widget_9->setVisible(true);
		}
		else
		{
			m_tCfg.atLedLight[1].dwLedCtrlMode = 0x1;
			ui.widget_9->setVisible(false);
		}
	}
	else if (pCmb == ui.cmb_led_mode_6)
	{
		if (nIndex == 1)
		{
			m_tCfg.atLedLight[2].dwLedCtrlMode = 0x4;
			ui.widget_13->setVisible(true);
		}
		else
		{
			m_tCfg.atLedLight[2].dwLedCtrlMode = 0x1;
			ui.widget_13->setVisible(false);
		}
	}
	else if (pCmb == ui.cmb_led_outer2_pl_3)
	{
		int nSel = -1;
		if (nIndex == 0)
		{
			nSel = 50;
		}
		else if (nIndex == 1)
		{
			nSel = 75;
		}
		else if (nIndex == 2)
		{
			nSel = 100;
		}
		m_tCfg.atLedLight[0].dwLedLightHz = nSel;
	}
	else if (pCmb == ui.cmb_led_outer2_pl_5)
	{
		int nSel = -1;
		if (nIndex == 0)
		{
			nSel = 50;
		}
		else if (nIndex == 1)
		{
			nSel = 75;
		}
		else if (nIndex == 2)
		{
			nSel = 100;
		}
		m_tCfg.atLedLight[1].dwLedLightHz = nSel;
	}
	else if (pCmb == ui.cmb_led_outer2_pl_6)
	{
		int nSel = -1;
		if (nIndex == 0)
		{
			nSel = 50;
		}
		else if (nIndex == 1)
		{
			nSel = 75;
		}
		else if (nIndex == 2)
		{
			nSel = 100;
		}
		m_tCfg.atLedLight[2].dwLedLightHz = nSel;
	}
	else if (pCmb == ui.cmb_pgj_mode)
	{
		if (nIndex == 1)
		{
			m_tCfg.tPLCtrl.dwPLCtrlMode = 0x4;
			ui.widget_11->setVisible(true);
			ui.label_97->setVisible(false);
			ui.label_96->setVisible(false);
			ui.ledt_pgj_start_es->setVisible(false);
			ui.ledt_pgj_end_es->setVisible(false);
		}
		else
		{
			m_tCfg.tPLCtrl.dwPLCtrlMode = 0x1;
			ui.widget_11->setVisible(false);
			ui.label_97->setVisible(true);
			ui.label_96->setVisible(true);
			ui.ledt_pgj_start_es->setVisible(true);
			ui.ledt_pgj_end_es->setVisible(true);
		}
	}
}

void Dlg_ImageQuality::Slot_SetCmbSnapIndexChanged(int nIndex)
{
	if (this->sender() == ui.cmb_bsd_mode_3)
	{
		m_tCfg.atSnap[0].dwFLightMode = ui.cmb_bsd_mode_3->currentIndex();
	}
	else if (this->sender() == ui.cmb_bsd_mode_night)
	{
		m_tCfg.atSnap[1].dwFLightMode = ui.cmb_bsd_mode_night->currentIndex();
	}
}

void Dlg_ImageQuality::Slot_SetCmbWhiteBalanceIndexChanged(int nIndex)
{
	if (this->sender()->property("id") == 111)
	{
		this->sender()->setProperty("id", 0);
		return;
	}
	if (this->sender() == ui.cmb_mode_day)
	{
		m_tCfg.atWhiteBal[0].dwAWBMode = ui.cmb_mode_day->currentIndex() + 1;
	}
	else if (this->sender() == ui.cmb_mode_night)
	{
		m_tCfg.atWhiteBal[1].dwAWBMode = ui.cmb_mode_day->currentIndex() + 1;
	}
}

void Dlg_ImageQuality::Slot_SetCmbExposureIndexChanged(int nIndex)
{
	if (this->sender() == ui.cmb_exposure_mode)
	{
		m_tCfg.atExposure[0].dwAEmode = ui.cmb_exposure_mode->currentIndex() + 1;
	}
	else if (this->sender() == ui.cmb_exposure_kmsx)
	{
		m_tCfg.atExposure->dwMaxShuter = OnShutterValueToIpc(m_nMaxShutter);
	}
	else if (this->sender() == ui.cmb_exposure_km)
	{
		m_tCfg.atExposure->dwManuShutter = OnShutterValueToIpc(m_nManuShutter);
	}
}

void Dlg_ImageQuality::Slot_LedRadioChanged(bool is)
{
	QRadioButton *pRadio = dynamic_cast<QRadioButton*>(this->sender());
	if (pRadio == ui.radio_open_3 || pRadio == ui.radio_close_3)
	{
		Uint32 dwMode = 0x1;
		if (ui.radio_open_3->isChecked())
		{
			dwMode = 0x2;
		}
		else if (ui.radio_close_3->isChecked())
		{
			dwMode = 0x4;
		}
		m_tCfg.atLedLight[0].dwLedCtrlMode = dwMode;
	}
	else if (pRadio == ui.radio_open_5 || pRadio == ui.radio_close_5)
	{
		Uint32 dwMode = 0x1;
		if (ui.radio_open_5->isChecked())
		{
			dwMode = 0x2;
		}
		else if (ui.radio_close_5->isChecked())
		{
			dwMode = 0x4;
		}
		m_tCfg.atLedLight[1].dwLedCtrlMode = dwMode;
	}
	else if (pRadio == ui.radio_open_6 || pRadio == ui.radio_close_6)
	{
		Uint32 dwMode = 0x1;
		if (ui.radio_open_6->isChecked())
		{
			dwMode = 0x2;
		}
		else if (ui.radio_close_6->isChecked())
		{
			dwMode = 0x4;
		}
		m_tCfg.atLedLight[2].dwLedCtrlMode = dwMode;
	}
	else if (pRadio == ui.chk_pgj_open || pRadio == ui.chk_pgj_close)
	{
		Uint32 dwMode = 0x1;
		if (ui.chk_pgj_open->isChecked())
		{
			dwMode = 0x2;
		}
		else if (ui.chk_pgj_close->isChecked())
		{
			dwMode = 0x4;
		}
		m_tCfg.tPLCtrl.dwPLCtrlMode = dwMode;
	}
	
}

void Dlg_ImageQuality::Slot_SetSnapInputChanged(const QString & sValue)
{
	QLineEdit *pEdit = dynamic_cast<QLineEdit*>(this->sender());
	if (pEdit == ui.ledt_picSize_day_3)
	{
		m_tCfg.atSnap[0].dwPicSize = pEdit->text().toInt();
	}
	else if (pEdit == ui.ledt_picYz_day_3)
	{
		m_tCfg.atSnap[0].dwJpegQp = pEdit->text().toInt();
	}
	else if (pEdit == ui.ledt_huidu_day_3)
	{
		m_tCfg.atSnap[0].byGStretchCutOff = pEdit->text().toInt();
	}
	else if (pEdit == ui.cmb_snap_day_bsd_time_3)
	{
		m_tCfg.atSnap[0].dwFLightTime = pEdit->text().toInt();
	}
	else if (pEdit == ui.ledt_snap_day_led_time_3)
	{
		m_tCfg.atSnap[0].dwLedTime = pEdit->text().toInt();
	}
	else if (pEdit == ui.ledt_snap_day_kmsx_3)
	{
		m_tCfg.atSnap[0].dwSnapMaxShuter = pEdit->text().toInt();
	}
	else if (pEdit == ui.ledt_snap_day_zysx_3)
	{
		m_tCfg.atSnap[0].dwSnapMaxAgain = pEdit->text().toInt();
	}
	else if (pEdit == ui.ledt_picSize_night)
	{
		m_tCfg.atSnap[1].dwPicSize = pEdit->text().toInt();
	}
	else if (pEdit == ui.ledt_picYz_night)
	{
		m_tCfg.atSnap[1].dwJpegQp = pEdit->text().toInt();
	}
	else if (pEdit == ui.ledt_huidu_night)
	{
		m_tCfg.atSnap[1].byGStretchCutOff = pEdit->text().toInt();
	}
	else if (pEdit == ui.cmb_snap_night_bsd_time)
	{
		m_tCfg.atSnap[1].dwFLightTime = pEdit->text().toInt();
	}
	else if (pEdit == ui.ledt_snap_night_led_time)
	{
		m_tCfg.atSnap[1].dwLedTime = pEdit->text().toInt();
	}
	else if (pEdit == ui.ledt_snap_night_kmsx)
	{
		m_tCfg.atSnap[1].dwSnapMaxShuter = pEdit->text().toInt();
	}
	else if (pEdit == ui.ledt_snap_night_zysx)
	{
		m_tCfg.atSnap[1].dwSnapMaxAgain = pEdit->text().toInt();
	}
	else if (pEdit == ui.ledt_snap_night_R)
	{
		m_tCfg.atSnap[1].dwSnapRgain = pEdit->text().toInt();
	}
	else if (pEdit == ui.ledt_snap_night_R)
	{
		m_tCfg.atSnap[1].dwSnapBgain = pEdit->text().toInt();
	}

}

void Dlg_ImageQuality::Slot_Snap_SetChkChanged(int nIndex)
{
	QCheckBox *pChk = dynamic_cast<QCheckBox*>(this->sender());

	if (pChk == ui.chk_picSize_day)
	{
		m_tCfg.atSnap[0].dwEncCtrl = nIndex;
		if (nIndex > 0)
		{
			ui.ledt_picSize_day_3->setEnabled(true);
		}
		else
		{
			ui.ledt_picSize_day_3->setEnabled(false);
		}
	}
	else if (pChk == ui.chk_huidu_enable_day_3)
	{
		m_tCfg.atSnap[0].bGStretch = nIndex;
	}
	else if (pChk == ui.chk_picSize_night)
	{
		m_tCfg.atSnap[1].dwEncCtrl = nIndex;
		if (nIndex > 1)
		{
			ui.ledt_picSize_night->setEnabled(true);
		}
		else
		{
			ui.ledt_picSize_night->setEnabled(false);
		}
	}
	else if (pChk == ui.chk_huidu_enable_night)
	{
		m_tCfg.atSnap[1].bGStretch = nIndex;
	}
}

void Dlg_ImageQuality::Slot_SetWhiteBalanceChanged(const QString & sValue)
{
	QLineEdit *pEdit = dynamic_cast<QLineEdit*>(this->sender());
	if (pEdit == ui.ledt_QDR_day)
	{
		m_tCfg.atWhiteBal[0].dwExpRgain = pEdit->text().toInt();
	}
	else if (pEdit == ui.ledt_QDB_day)
	{
		m_tCfg.atWhiteBal[0].dwExpBgain = pEdit->text().toInt();
	}
	else if (pEdit == ui.ledt_R_day)
	{
		m_tCfg.atWhiteBal[0].dwWbRgain = pEdit->text().toInt();
	}
	else if (pEdit == ui.ledt_B_day)
	{
		m_tCfg.atWhiteBal[0].dwWbBgain = pEdit->text().toInt();
	}
	else if (pEdit == ui.ledt_QDR_night)
	{
		m_tCfg.atWhiteBal[1].dwExpRgain = pEdit->text().toInt();
	}
	else if (pEdit == ui.ledt_QDB_night)
	{
		m_tCfg.atWhiteBal[1].dwExpBgain = pEdit->text().toInt();
	}
	else if (this->sender() == ui.ledt_R_night)
	{
		m_tCfg.atWhiteBal[1].dwWbRgain = pEdit->text().toInt();
	}
	else if (this->sender() == ui.ledt_B_night)
	{
		m_tCfg.atWhiteBal[1].dwWbBgain = pEdit->text().toInt();
	}
	else if (this->sender() == ui.ledt_exchange_val)
	{
		m_tCfg.dwDayNightSwitchEs = ui.ledt_exchange_val->text().toInt();
	}
}

void Dlg_ImageQuality::Slot_SetExposureChanged(const QString &)
{
	if (this->sender() == ui.ledt_exposure_zysx)
	{
		m_tCfg.atExposure[0].dwMaxAgain = ui.ledt_exposure_zysx->text().toInt();
	}
	else if (this->sender() == ui.ledt_exposure_qwld)
	{
		m_tCfg.atExposure[0].dwTargetY = ui.ledt_exposure_qwld->text().toInt();
	}
	else if (this->sender() == ui.ledt_exposure_zy)
	{
		m_tCfg.atExposure[0].dwManuAgain = ui.ledt_exposure_zy->text().toInt();
	}
	else if (this->sender() == ui.ledt_exposure_qwhpld)
	{
		m_tCfg.atExposure[0].dwEptLPRBright = ui.ledt_exposure_qwhpld->text().toInt();
	}
	else if (this->sender() == ui.ledt_exposure_qwhpldsx)
	{
		m_tCfg.atExposure[0].dwMaxTargetY = ui.ledt_exposure_qwhpldsx->text().toInt();
	}
	else if (this->sender() == ui.ledt_exposure_qwhpldxx)
	{
		m_tCfg.atExposure[0].dwMinTargetY = ui.ledt_exposure_qwhpldxx->text().toInt();
	}
}

void Dlg_ImageQuality::Slot_SetEditData(int nValue)
{
	QSlider *pSlider = (QSlider *)this->sender();
	m_mapTemp1[pSlider]->setText(QString("%1").arg(nValue));
}

void Dlg_ImageQuality::Slot_SetSliderChanged(const QString &sText)
{
	QLineEdit *pEdit = (QLineEdit *)this->sender();
	int nValue = sText.toInt();
	m_mapTemp2[pEdit]->setValue(nValue);
	if (pEdit == ui.ledt_light_day)
	{
		m_tCfg.atImageCtrl[0].byBright = ui.ledt_light_day->text().toInt();
	}
	else if (pEdit == ui.ledt_dbd_day)
	{
		m_tCfg.atImageCtrl[0].byContrast = ui.ledt_dbd_day->text().toInt();
	}
	else if (pEdit == ui.ledt_bhd_day)
	{
		m_tCfg.atImageCtrl[0].bySaturation = ui.ledt_bhd_day->text().toInt();
	}
	else if (pEdit == ui.ledt_rd_day)
	{
		m_tCfg.atImageCtrl[0].bySharpness = ui.ledt_rd_day->text().toInt();
	}
	else if (pEdit == ui.ledt_gama_day)
	{
		m_tCfg.atImageCtrl[0].byGama = ui.ledt_gama_day->text().toInt();
	}
	else if (pEdit == ui.ledt_2d_day)
	{
		m_tCfg.atImageCtrl[0].bydeNoise2D = ui.ledt_2d_day->text().toInt();
	}
	if (pEdit == ui.ledt_light_night)
	{
		m_tCfg.atImageCtrl[1].byBright = ui.ledt_light_night->text().toInt();
	}
	else if (pEdit == ui.ledt_dbd_night)
	{
		m_tCfg.atImageCtrl[1].byContrast = ui.ledt_dbd_night->text().toInt();
	}
	else if (pEdit == ui.ledt_bhd_night)
	{
		m_tCfg.atImageCtrl[1].bySaturation = ui.ledt_bhd_night->text().toInt();
	}
	else if (pEdit == ui.ledt_rd_night)
	{
		m_tCfg.atImageCtrl[1].bySharpness = ui.ledt_rd_night->text().toInt();
	}
	else if (pEdit == ui.ledt_gama_night)
	{
		m_tCfg.atImageCtrl[1].byGama = ui.ledt_gama_night->text().toInt();
	}
	else if (pEdit == ui.ledt_2d_night)
	{
		m_tCfg.atImageCtrl[1].bydeNoise2D = ui.ledt_2d_night->text().toInt();
	}

}

Dlg_ImageQuality::~Dlg_ImageQuality()
{
	SAFE_DELETE(m_messageBox);
	ui.video->ControlPlayVideo(false);
	MgrData::getInstance()->GetMgrEvtLst()->DelSender(static_cast<CObserver*>(this));
	MgrData::getInstance()->GetMgrSubject()->Del(this);
}

bool Dlg_ImageQuality::GetParam()
{
	u32 dwIP = MgrData::getInstance()->GetIP();
	ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dwIP);
	if (pCtrl != NULL)
	{
		bool bRet = pCtrl->GetIpcImageQuality(NULL, (u32)(CObserver*)this);
		if (!bRet)
		{

		}

		return bRet;
	}

	return false;
}



void Dlg_ImageQuality::InitData(bool is)
{
	if (is)
	{
		// 进入当前窗口的起始位置
		memset(&m_tCfg, 0, sizeof(m_tCfg));
		GetParam();
		emit ui.tab1->clicked(true);
		
	}
	ui.video->ControlPlayVideo(is);
}


// 将相机快门参数转换成界面参数
Uint32 Dlg_ImageQuality::OnShutterValueToClient(Uint32 shutter)
{
	Uint32 index = 0;
	Uint32 value = 1000000;
	if (shutter >= value / 1)
	{
		index = 0;
	}
	else if (shutter >= value / 2)
	{
		index = 1;
	}
	else if (shutter >= value / 7.5)
	{
		index = 2;
	}
	else if (shutter >= value / 10)
	{
		index = 3;
	}
	else if (shutter >= value / 15)
	{
		index = 4;
	}
	else if (shutter >= value / 25)
	{
		index = 5;
	}
	else if (shutter >= value / 50)
	{
		index = 6;
	}
	else if (shutter >= value / 100)
	{
		index = 7;
	}
	else if (shutter >= value / 200)
	{
		index = 8;
	}
	else if (shutter >= value / 240)
	{
		index = 9;
	}
	else if (shutter >= value / 300)
	{
		index = 10;
	}
	else if (shutter >= value / 400)
	{
		index = 11;
	}
	else if (shutter >= value / 480)
	{
		index = 12;
	}
	else if (shutter >= value / 960)
	{
		index = 13;
	}
	else if (shutter >= value / 1024)
	{
		index = 14;
	}
	else if (shutter >= value / 2000)
	{
		index = 15;
	}
	else if (shutter >= value / 4000)
	{
		index = 16;
	}
	else if (shutter >= value / 8000)
	{
		index = 17;
	}
	else if (shutter >= value / 16000)
	{
		index = 18;
	}
	else if (shutter >= value / 30000)
	{
		index = 19;
	}
	else if (shutter >= value / 100000)
	{
		index = 20;
	}
	else if (shutter >= value / 124000)
	{
		index = 21;
	}

	return index;
}

bool Dlg_ImageQuality::eventFilter(QObject *object, QEvent *event)
{
	return QWidget::eventFilter(object, event);
}

// 获取指定位的值0/1
int Dlg_ImageQuality::GetSpecifyBitValue(Uint32 dwSrc, int nIndex)
{
	Uint32 dw2 = ((int)pow(2.0, nIndex));
	dwSrc = dwSrc & dw2;
	dwSrc = dwSrc >> nIndex;

	return dwSrc;
}

u32 Dlg_ImageQuality::SetSpecifyBitValue(Uint32& dwSrcOut, Uint32 dwValue, int nIndex)
{
	Uint32 dw2 = ((int)pow(2.0, nIndex));

	if (dwValue == 0)
	{
		dw2 = ~dw2;
		dwSrcOut = dwSrcOut & dw2;
	}
	else
	{
		dwSrcOut = dwSrcOut | dw2;
	}

	return dwSrcOut;
}

void Dlg_ImageQuality::SetData()
{
	
	//图像调节
	ui.ledt_light_day->setText(QString("%1").arg(m_tCfg.atImageCtrl[0].byBright));
	ui.ledt_dbd_day->setText(QString("%1").arg(m_tCfg.atImageCtrl[0].byContrast));
	ui.ledt_bhd_day->setText(QString("%1").arg(m_tCfg.atImageCtrl[0].bySaturation));
	ui.ledt_rd_day->setText(QString("%1").arg(m_tCfg.atImageCtrl[0].bySharpness));
	ui.ledt_gama_day->setText(QString("%1").arg(m_tCfg.atImageCtrl[0].byGama));
	ui.ledt_2d_day->setText(QString("%1").arg(m_tCfg.atImageCtrl[0].bydeNoise2D));

	ui.ledt_light_night->setText(QString("%1").arg(m_tCfg.atImageCtrl[1].byBright));
	ui.ledt_dbd_night->setText(QString("%1").arg(m_tCfg.atImageCtrl[1].byContrast));
	ui.ledt_bhd_night->setText(QString("%1").arg(m_tCfg.atImageCtrl[1].bySaturation));
	ui.ledt_rd_night->setText(QString("%1").arg(m_tCfg.atImageCtrl[1].bySharpness));
	ui.ledt_gama_night->setText(QString("%1").arg(m_tCfg.atImageCtrl[1].byGama));
	ui.ledt_2d_night->setText(QString("%1").arg(m_tCfg.atImageCtrl[1].bydeNoise2D));
	//曝光
	if (m_tCfg.atExposure[0].dwSptAEModeList != 0)
	{
		while (ui.cmb_exposure_mode->count() > 0)
		{
			ui.cmb_exposure_mode->removeItem(0);
		}

		u32 dwValue = m_tCfg.atExposure[0].dwSptAEModeList;
		if (GetSpecifyBitValue(dwValue, 0))
		{
			ui.cmb_exposure_mode->addItem(GBUTF8("手动"));
		}

		if (GetSpecifyBitValue(dwValue, 1))
		{
			ui.cmb_exposure_mode->addItem(GBUTF8("自动"));
		}

		if (GetSpecifyBitValue(dwValue, 2))
		{
			ui.cmb_exposure_mode->addItem(GBUTF8("ISP内置AE"));
		}

		if (GetSpecifyBitValue(dwValue, 3))
		{
			ui.cmb_exposure_mode->addItem(GBUTF8("锁定"));
		}

		if (GetSpecifyBitValue(dwValue, 4))
		{
			ui.cmb_exposure_mode->addItem(GBUTF8("无效"));
		}

		if (ui.cmb_exposure_mode->count() > 0)
		{
			//ui.cmb_exposure_mode->setCurrentIndex(0);
		}
	}
		ui.cmb_exposure_mode->setCurrentIndex(m_tCfg.atExposure[0].dwAEmode - 1);
		ui.cmb_exposure_kmsx->setCurrentIndex(m_nMaxShutter);
		ui.ledt_exposure_zysx->setText(QString("%1").arg(m_tCfg.atExposure[0].dwMaxAgain));
		ui.cmb_exposure_km->setCurrentIndex(m_nManuShutter);
		ui.ledt_exposure_zy->setText(QString("%1").arg(m_tCfg.atExposure[0].dwManuAgain));
		ui.ledt_exposure_qwld->setText(QString("%1").arg(m_tCfg.atExposure[0].dwTargetY));
		ui.chk_exposure_qwhpenable->setChecked(m_tCfg.atExposure[0].dwLPRAEenable);
		ui.ledt_exposure_qwhpld->setText(QString("%1").arg(m_tCfg.atExposure[0].dwEptLPRBright));
		ui.ledt_exposure_qwhpldsx->setText(QString("%1").arg(m_tCfg.atExposure[0].dwMaxTargetY));
		ui.ledt_exposure_qwhpldxx->setText(QString("%1").arg(m_tCfg.atExposure[0].dwMinTargetY));
		
	//白平衡
		for (int i = 0; i < 2; i++)
		{
			QComboBox *pCmb;
			if (i == 0)
			{
				pCmb = ui.cmb_mode_day;
			}
			else
			{
				pCmb = ui.cmb_mode_night;
			}
			if (m_tCfg.atWhiteBal[i].dwSptAWBModeList != 0)
			{
				while (pCmb->count() > 0)
				{
					pCmb->setProperty("id", 111);
					pCmb->removeItem(0);
				}
			
				u32 dwValue = m_tCfg.atWhiteBal[i].dwSptAWBModeList;
				if (GetSpecifyBitValue(dwValue, 0))
				{
					pCmb->setProperty("id", 111);
					pCmb->addItem(GBUTF8("手动"));
				}

				if (GetSpecifyBitValue(dwValue, 1))
				{
					pCmb->setProperty("id", 111);
					pCmb->addItem(GBUTF8("自动白平衡"));
				}

				if (GetSpecifyBitValue(dwValue, 2))
				{
					pCmb->setProperty("id", 111);
					pCmb->addItem(GBUTF8("ISP内置AWB"));
				}

				if (GetSpecifyBitValue(dwValue, 3))
				{
					pCmb->setProperty("id", 111);
					pCmb->addItem(GBUTF8("锁定"));
				}

				if (GetSpecifyBitValue(dwValue, 4))
				{
					pCmb->setProperty("id", 111);
					pCmb->addItem(GBUTF8("无效"));
				}
				pCmb->setProperty("id", 0);

				if (pCmb->count() > 0)
				{
					//pCmb->setCurrentIndex(0);
				}
			}
		}
		
		ui.cmb_mode_day->setCurrentIndex(m_tCfg.atWhiteBal[0].dwAWBMode - 1);//- 1
		ui.cmb_mode_night->setCurrentIndex(m_tCfg.atWhiteBal[1].dwAWBMode - 1);
		ui.ledt_QDR_day->setText(QString("%1").arg(m_tCfg.atWhiteBal[0].dwExpRgain));
		ui.ledt_QDB_day->setText(QString("%1").arg(m_tCfg.atWhiteBal[0].dwExpBgain));
		ui.ledt_R_day->setText(QString("%1").arg(m_tCfg.atWhiteBal[0].dwWbRgain));
		ui.ledt_B_day->setText(QString("%1").arg(m_tCfg.atWhiteBal[0].dwWbBgain));

		ui.cmb_mode_night->setCurrentIndex(m_tCfg.atWhiteBal[1].dwAWBMode - 1);
		ui.ledt_QDR_night->setText(QString("%1").arg(m_tCfg.atWhiteBal[1].dwExpRgain));
		ui.ledt_QDB_night->setText(QString("%1").arg(m_tCfg.atWhiteBal[1].dwExpBgain));
		ui.ledt_R_night->setText(QString("%1").arg(m_tCfg.atWhiteBal[1].dwWbRgain));
		ui.ledt_B_night->setText(QString("%1").arg(m_tCfg.atWhiteBal[1].dwWbBgain));
		//日夜切换
		ui.ledt_exchange_val->setText(QString("%1").arg(m_tCfg.dwDayNightSwitchEs));
		//抓拍
	
		ui.chk_picSize_day->setChecked(m_tCfg.atSnap[0].dwEncCtrl);
		ui.ledt_picSize_day_3->setText(QString("%1").arg(m_tCfg.atSnap[0].dwPicSize));
		ui.ledt_picYz_day_3->setText(QString("%1").arg(m_tCfg.atSnap[0].dwJpegQp));
		ui.chk_huidu_enable_day_3->setChecked(m_tCfg.atSnap[0].bGStretch);
		ui.ledt_huidu_day_3->setText(QString("%1").arg(m_tCfg.atSnap[0].byGStretchCutOff));
		ui.cmb_bsd_mode_3->setCurrentIndex(m_tCfg.atSnap[0].dwFLightMode);
		ui.cmb_snap_day_bsd_time_3->setText(QString("%1").arg(m_tCfg.atSnap[0].dwFLightTime));
		ui.ledt_snap_day_led_time_3->setText(QString("%1").arg(m_tCfg.atSnap[0].dwLedTime));
		ui.ledt_snap_day_kmsx_3->setText(QString("%1").arg(m_tCfg.atSnap[0].dwSnapMaxShuter));
		ui.ledt_snap_day_zysx_3->setText(QString("%1").arg(m_tCfg.atSnap[0].dwSnapMaxAgain));

		ui.chk_picSize_night->setChecked(m_tCfg.atSnap[1].dwEncCtrl);
		ui.ledt_picSize_night->setText(QString("%1").arg(m_tCfg.atSnap[1].dwPicSize));
		ui.ledt_picYz_night->setText(QString("%1").arg(m_tCfg.atSnap[1].dwJpegQp));
		ui.chk_huidu_enable_night->setChecked(m_tCfg.atSnap[1].bGStretch);
		ui.ledt_huidu_night->setText(QString("%1").arg(m_tCfg.atSnap[1].byGStretchCutOff));
		ui.cmb_bsd_mode_night->setCurrentIndex(m_tCfg.atSnap[1].dwFLightMode);
		ui.cmb_snap_night_bsd_time->setText(QString("%1").arg(m_tCfg.atSnap[1].dwFLightTime));
		ui.ledt_snap_night_led_time->setText(QString("%1").arg(m_tCfg.atSnap[1].dwLedTime));
		ui.ledt_snap_night_kmsx->setText(QString("%1").arg(m_tCfg.atSnap[1].dwSnapMaxShuter));
		ui.ledt_snap_night_zysx->setText(QString("%1").arg(m_tCfg.atSnap[1].dwSnapMaxAgain));
		ui.ledt_snap_night_R->setText(QString("%1").arg(m_tCfg.atSnap[1].dwSnapRgain));
		ui.ledt_snap_night_B->setText(QString("%1").arg(m_tCfg.atSnap[1].dwSnapBgain));

		//LED设值
		int nSel = -1;
		if (m_tCfg.atLedLight[0].dwLedCtrlMode == 0x1)
		{
			nSel = 0;
		}
		else if (m_tCfg.atLedLight[0].dwLedCtrlMode == 0x16)
		{
			nSel = 2;
		}
		else 
		{
			nSel = 1;
		}

		if (nSel == 0 || nSel == 2)
		{
			ui.widget_6->setVisible(false);
		}
		else 
		{
			ui.widget_6->setVisible(true);
		}
		ui.cmb_led_mode_3->setCurrentIndex(nSel);
		QComboBox *pCmbLight;
		for (int i = 0; i < 3; i++)
		{
			if (i == 0)
			{
				pCmbLight = ui.cmb_led_outer2_pl_3;
			}
			else if (i == 1)
			{
				pCmbLight = ui.cmb_led_mode_5;
			}
			else
			{
				pCmbLight = ui.cmb_led_mode_6;
			}
			int nSelHz = -1;
			switch (m_tCfg.atLedLight[i].dwLedLightHz)
			{
			case 50:
				nSelHz = 0;
				break;
			case 75:
				nSelHz = 1;
				break;
			case 100:
				nSelHz = 2;
				break;
			default:
				break;
			}
			pCmbLight->setCurrentIndex(nSelHz);
		}
		
		
		ui.ledt_led_outer1_mk_3->setText(QString("%1").arg(m_tCfg.atLedLight[0].dwLedLightTime));
		if (m_tCfg.atLedLight[0].dwLedCtrlMode == 2)
		{
			ui.radio_open_3->setChecked(true);
		}
		else if (m_tCfg.atLedLight[0].dwLedCtrlMode == 4)
		{
			ui.radio_close_3->setChecked(true);
		}


		if (m_tCfg.atLedLight[1].dwLedCtrlMode == 0x1)
		{
			nSel = 0;
		}
		else
		{
			nSel = 1;
		}

		if (nSel == 0 )
		{
			ui.widget_9->setVisible(false);
		}
		else
		{
			ui.widget_9->setVisible(true);
		}
		ui.cmb_led_mode_5->setCurrentIndex(nSel);
		//ui.cmb_led_outer2_pl_5->setCurrentIndex(m_tCfg.atLedLight[1].dwLedLightHz);
		ui.ledt_led_outer1_mk_5->setText(QString("%1").arg(m_tCfg.atLedLight[1].dwLedLightTime));
		if (m_tCfg.atLedLight[1].dwLedCtrlMode == 2)
		{
			ui.radio_open_5->setChecked(true);
		}
		else if (m_tCfg.atLedLight[1].dwLedCtrlMode == 4)
		{
			ui.radio_close_5->setChecked(true);
		}

		if (m_tCfg.atLedLight[2].dwLedCtrlMode == 0x1)
		{
			nSel = 0;
		}
		else
		{
			nSel = 1;
		}

		if (nSel == 0 )
		{
			ui.widget_13->setVisible(false);
		}
		else
		{
			ui.widget_13->setVisible(true);
		}
		ui.cmb_led_mode_6->setCurrentIndex(nSel);
		//ui.cmb_led_outer2_pl_6->setCurrentIndex(m_tCfg.atLedLight[2].dwLedLightHz);
		ui.ledt_led_outer1_mk_6->setText(QString("%1").arg(m_tCfg.atLedLight[2].dwLedLightTime));
		if (m_tCfg.atLedLight[0].dwLedCtrlMode == 2)
		{
			ui.radio_open_6->setChecked(true);
		}
		else if (m_tCfg.atLedLight[0].dwLedCtrlMode == 4)
		{
			ui.radio_close_6->setChecked(true);
		}
		//控制内置灯的显示与隐藏
		if (MgrData::getInstance()->GetDevInfo()->dwVender == 2)
		{
			ui.widget_7->setVisible(false);
			ui.widget_12->setVisible(false);	
		}
		else
		{
			if (strcmp(MgrData::getInstance()->GetDevInfo()->szDevType, "IPC425-P323-NWH") == 0
				|| strcmp(MgrData::getInstance()->GetDevInfo()->szDevType, "IPC425-P323-N") == 0
				|| strcmp(MgrData::getInstance()->GetDevInfo()->szDevType, "IPC425-P345-N") == 0
				|| strcmp(MgrData::getInstance()->GetDevInfo()->szDevType, "IPC425-P345-NWH") == 0)
			{
				ui.widget_5->setVisible(false);
				ui.widget_7->setVisible(false);
				ui.label_99->setText(GBUTF8("外置LED灯"));
			}
			else if (strcmp(MgrData::getInstance()->GetDevInfo()->szDevType, "IPC123-Ei8NT") == 0
				|| strcmp(MgrData::getInstance()->GetDevInfo()->szDevType, "IPC2255-Gi8N") == 0
				|| strcmp(MgrData::getInstance()->GetDevInfo()->szDevType, "IPC2655-Gi8N") == 0)
			{
				ui.label_83->setText(GBUTF8("内置LED灯1"));
				ui.label_91->setText(GBUTF8("内置LED灯2"));
				ui.label_99->setText(GBUTF8("外置LED灯"));
				
			}
		}

		//偏光镜
		int nSelect = -1;
		if (m_tCfg.tPLCtrl.dwPLCtrlMode == 0x1)
		{
			nSelect = 0;
			ui.widget_11->setVisible(false);
			ui.label_97->setVisible(true);
			ui.label_96->setVisible(true);
			ui.ledt_pgj_start_es->setVisible(true);
			ui.ledt_pgj_end_es->setVisible(true);

		}
		else
		{
			nSelect = 1;
			ui.widget_11->setVisible(true);
			ui.label_97->setVisible(false);
			ui.label_96->setVisible(false);
			ui.ledt_pgj_start_es->setVisible(false);
			ui.ledt_pgj_end_es->setVisible(false);
		}
		ui.ledt_pgj_start_es->setText(QString("%1").arg(m_tCfg.tPLCtrl.dwPLenableEs));
		ui.ledt_pgj_end_es->setText(QString("%1").arg(m_tCfg.tPLCtrl.dwPLdisableEs));
		ui.cmb_pgj_mode->setCurrentIndex(nSelect);
}

void Dlg_ImageQuality::Slot_ImageQuatityChanged()
{
	SetData();
	if (MgrData::getInstance()->GetDevVersion() > 3)
	{
		ui.tab1->setVisible(false);
		ui.tab2->setChecked(true);
		emit ui.tab2->clicked(true);
	}
	//OnSwitchTab(NULL);
}

void Dlg_ImageQuality::Slot_TabIndexChanged()
{
	if (this->sender() == ui.tab1)
	{
		ui.stackedWidget->setCurrentIndex(1);
	}
	else if (this->sender() == ui.tab2)
	{
		ui.stackedWidget->setCurrentIndex(6);
	}
	else if (this->sender() == ui.tab3)
	{
		ui.stackedWidget->setCurrentIndex(0);
	}
	else if (this->sender() == ui.tab4)
	{
		ui.stackedWidget->setCurrentIndex(2);
	}
	else if (this->sender() == ui.tab5)
	{
		ui.stackedWidget->setCurrentIndex(3);
	}
	else if (this->sender() == ui.tab6)
	{
		ui.stackedWidget->setCurrentIndex(4);
	}
	else if (this->sender() == ui.tab7)
	{
		ui.stackedWidget->setCurrentIndex(5);
	}
	
}

void Dlg_ImageQuality::OnObserverNotify(LPARAM lHint, LPVOID pHint)
{

	u32 dwEvtType = (u32)lHint;
	u32 dwId = (u32)pHint;

	switch (dwEvtType)
	{
	case NMC_GET_IPC_IMAGEQUALITY:
	{
		TIPC_GetKISPCfgRsp* p = (TIPC_GetKISPCfgRsp*)pHint;
		if (p->nRetCode == 0)
		{
			memcpy(&m_tCfg, &(p->tKdISPCfg), sizeof(TIPC_KdISPCfg));
			m_nMaxShutter = OnShutterValueToClient(m_tCfg.atExposure->dwMaxShuter);
			m_nManuShutter = OnShutterValueToClient(m_tCfg.atExposure->dwManuShutter);
			emit signal_imageQuatityChanged();
			
		}
	}
		break;
	case NMC_EVT_VIDEO_PIC:
	{
		BOOL bHandled;
		//OnRefreshPic(0, (WPARAM)0, 0, bHandled);
	}
		break;
	default:
		break;
	}
}

