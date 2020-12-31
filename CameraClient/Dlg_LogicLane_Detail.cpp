#include "Dlg_LogicLane_Detail.h"
#include "global.h"
#include "globalFunction.h"
#include <QMessageBox>
#include <QListView>
Dlg_LogicLane_Detail::Dlg_LogicLane_Detail(QWidget *parent)
	: QDialog(parent), m_pLogicLaneSrc(nullptr), m_bSetData(false)
	, m_nWeek(0)
{
	ui.setupUi(this);
	Init();
	m_pLogicLane = new TIPC_LogicLane;

	connect(ui.tab_lane1, SIGNAL(clicked()), this, SLOT(Slot_SwitchTabClicked()));
	connect(ui.tab_lane2, SIGNAL(clicked()), this, SLOT(Slot_SwitchTabClicked()));
	connect(ui.tab_lane3, SIGNAL(clicked()), this, SLOT(Slot_SwitchTabClicked()));
	connect(ui.tab_lane4, SIGNAL(clicked()), this, SLOT(Slot_SwitchTabClicked()));
	connect(ui.tab_lane5, SIGNAL(clicked()), this, SLOT(Slot_SwitchTabClicked()));
	connect(ui.tab_lane6, SIGNAL(clicked()), this, SLOT(Slot_SwitchTabClicked()));

	connect(ui.btn_day7, SIGNAL(clicked()), this, SLOT(Slot_SwitchDayClicked()));
	connect(ui.btn_day1, SIGNAL(clicked()), this, SLOT(Slot_SwitchDayClicked()));
	connect(ui.btn_day2, SIGNAL(clicked()), this, SLOT(Slot_SwitchDayClicked()));
	connect(ui.btn_day3, SIGNAL(clicked()), this, SLOT(Slot_SwitchDayClicked()));
	connect(ui.btn_day4, SIGNAL(clicked()), this, SLOT(Slot_SwitchDayClicked()));
	connect(ui.btn_day5, SIGNAL(clicked()), this, SLOT(Slot_SwitchDayClicked()));
	connect(ui.btn_day6, SIGNAL(clicked()), this, SLOT(Slot_SwitchDayClicked()));

	connect(ui.cmb_snapctrl, SIGNAL(currentIndexChanged(int)), this, SLOT(Slot_CmbIndexChanged(int)));
	connect(ui.cmb_snapNum, SIGNAL(currentIndexChanged(int)), this, SLOT(Slot_CmbIndexChanged(int)));
	connect(ui.cmb_face1, SIGNAL(currentIndexChanged(int)), this, SLOT(Slot_CmbIndexChanged(int)));
	connect(ui.cmb_face2, SIGNAL(currentIndexChanged(int)), this, SLOT(Slot_CmbIndexChanged(int)));

	connect(m_apEdt[logiclane::EMLOGICLANEDETAILS_EDT_RATELIMIT_LITTLE], SIGNAL(textChanged(const QString &)), this, SLOT(Slot_EditTextChanged(const QString &)));
	connect(m_apEdt[logiclane::EMLOGICLANEDETAILS_EDT_RATELIMIT_BIG], SIGNAL(textChanged(const QString &)), this, SLOT(Slot_EditTextChanged(const QString &)));
	connect(ui.btn_dayCopy, SIGNAL(clicked()), this, SLOT(Slot_DayCopyClicked()));

	connect(ui.btn_ok, SIGNAL(clicked()), this, SLOT(Slot_BtnSaveClicked()));
	connect(ui.btn_cancel, SIGNAL(clicked()), this, SLOT(Slot_BtnSaveClicked()));
	connect(m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_MERGE_LEFT_TOP], SIGNAL(stateChanged(int)), this, SLOT(Slot_CheckBoxClicked(int)));
	connect(m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_MERGE_RIGHT_TOP], SIGNAL(stateChanged(int)), this, SLOT(Slot_CheckBoxClicked(int)));
	connect(m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_MERGE_RIGHT_BOM], SIGNAL(stateChanged(int)), this, SLOT(Slot_CheckBoxClicked(int)));
	connect(m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_MERGE_LEFT_BOM], SIGNAL(stateChanged(int)), this, SLOT(Slot_CheckBoxClicked(int)));
	connect(m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_CHILD_PLATE], SIGNAL(stateChanged(int)), this, SLOT(Slot_CheckBoxClicked(int)));
	connect(m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_CHILD_VEHICLE], SIGNAL(stateChanged(int)), this, SLOT(Slot_CheckBoxClicked(int)));
	connect(m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_CHILD_POEPLE], SIGNAL(stateChanged(int)), this, SLOT(Slot_CheckBoxClicked(int)));
	connect(m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_ADD_FACE], SIGNAL(stateChanged(int)), this, SLOT(Slot_CheckBoxClicked(int)));
	
	m_apEdt[logiclane::EMLOGICLANEDETAILS_EDT_SNAPLIMIT_LITTLE]->installEventFilter(this);  //在窗体上为lineEdit1安装过滤器
	m_apEdt[logiclane::EMLOGICLANEDETAILS_EDT_SNAPLIMIT_BIG]->installEventFilter(this);  //在窗体上为lineEdit2安装过滤器
}

bool Dlg_LogicLane_Detail::eventFilter(QObject *watched, QEvent *event)
{
	if (watched == m_apEdt[logiclane::EMLOGICLANEDETAILS_EDT_SNAPLIMIT_LITTLE] && event->type() == QEvent::FocusOut)         //首先判断控件(这里指 lineEdit1)
	{
		logiclane::GetEdtValue(m_pLogicLane, m_apEdt[logiclane::EMLOGICLANEDETAILS_EDT_RATELIMIT_LITTLE], logiclane::EMLOGICLANEDETAILS_EDT_RATELIMIT_LITTLE);
		logiclane::GetEdtValue(m_pLogicLane, m_apEdt[logiclane::EMLOGICLANEDETAILS_EDT_SNAPLIMIT_LITTLE], logiclane::EMLOGICLANEDETAILS_EDT_SNAPLIMIT_LITTLE);
		if (m_pLogicLane->dwCarLmSpd > m_pLogicLane->dwCarSnapSpd)
		{
			m_pLogicLane->dwCarSnapSpd = m_pLogicLane->dwCarLmSpd;
			logiclane::SetEdtValue(m_pLogicLane, m_apEdt[logiclane::EMLOGICLANEDETAILS_EDT_SNAPLIMIT_LITTLE], logiclane::EMLOGICLANEDETAILS_EDT_SNAPLIMIT_LITTLE);
			QMessageBox box;
			box.setText(GBUTF8("		小车起拍速度必须大于等于小车限速		"));
			box.setWindowTitle(GBUTF8("告警"));
			box.exec();

		}
	}
	if (watched == m_apEdt[logiclane::EMLOGICLANEDETAILS_EDT_SNAPLIMIT_BIG] && event->type() == QEvent::FocusOut)
	{
		logiclane::GetEdtValue(m_pLogicLane, m_apEdt[logiclane::EMLOGICLANEDETAILS_EDT_RATELIMIT_BIG], logiclane::EMLOGICLANEDETAILS_EDT_RATELIMIT_BIG);
		logiclane::GetEdtValue(m_pLogicLane, m_apEdt[logiclane::EMLOGICLANEDETAILS_EDT_SNAPLIMIT_BIG], logiclane::EMLOGICLANEDETAILS_EDT_SNAPLIMIT_BIG);
		if (m_pLogicLane->dwTruckLmSpd > m_pLogicLane->dwTruckSnapSpd)
		{
			m_pLogicLane->dwTruckSnapSpd = m_pLogicLane->dwTruckLmSpd;
			logiclane::SetEdtValue(m_pLogicLane, m_apEdt[logiclane::EMLOGICLANEDETAILS_EDT_SNAPLIMIT_BIG], logiclane::EMLOGICLANEDETAILS_EDT_SNAPLIMIT_BIG);
			QMessageBox box;
			box.setText(GBUTF8("		大车起拍速度必须大于等于大车限速		"));
			box.setWindowTitle(GBUTF8("告警"));
			box.exec();
		}
	}
	return QDialog::eventFilter(watched, event);     // 最后将事件交给上层对话框
}

void Dlg_LogicLane_Detail::Init()
{
	memset(m_apCmb, 0, sizeof(m_apCmb));
	m_apCmb[logiclane::EMLOGICLANEDETAILS_CMB_SNAPNUM] = ui.cmb_snapNum;
	m_apCmb[logiclane::EMLOGICLANEDETAILS_CMB_SNAPCTRL] = ui.cmb_snapctrl;
	m_apCmb[logiclane::EMLOGICLANEDETAILS_CMB_ZFPP] = ui.cmb_zfpp;
	m_apCmb[logiclane::EMLOGICLANEDETAILS_CMB_JX] = ui.cmbredlight_jx;
	m_apCmb[logiclane::EMLOGICLANEDETAILS_CMB_FILTERDIR] = ui.cmb_filterdir;
	m_apCmb[logiclane::EMLOGICLANEDETAILS_CMB_PIC2DAY] = ui.cmb_day2;
	m_apCmb[logiclane::EMLOGICLANEDETAILS_CMB_PIC2NIGHT] = ui.cmb_night2;
	m_apCmb[logiclane::EMLOGICLANEDETAILS_CMB_PIC3DAY] = ui.cmb_day3;
	m_apCmb[logiclane::EMLOGICLANEDETAILS_CMB_PIC3NIGHT] = ui.cmb_night3;
	m_apCmb[logiclane::EMLOGICLANEDETAILS_CMB_PIC4DAY] = ui.cmb_day4;
	m_apCmb[logiclane::EMLOGICLANEDETAILS_CMB_PIC4NIGHT] = ui.cmb_night4;

	ui.cmb_snapNum->setView(new QListView);
	ui.cmb_snapctrl->setView(new QListView);
	ui.cmb_face1->setView(new QListView);
	ui.cmb_face2->setView(new QListView);
	// edit 值获取
	memset(m_apEdt, 0, sizeof(m_apEdt));
	m_apEdt[logiclane::EMLOGICLANEDETAILS_EDT_DECTOR1] = ui.ledt_1;
	m_apEdt[logiclane::EMLOGICLANEDETAILS_EDT_DECTOR2] = ui.ledt_2;
	m_apEdt[logiclane::EMLOGICLANEDETAILS_EDT_DECTOR3] = ui.ledt_3;
	m_apEdt[logiclane::EMLOGICLANEDETAILS_EDT_DECTOR4] = ui.ledt_4;
	ui.ledt_1->setValidator(new QIntValidator(0, 255, this));
	ui.ledt_2->setValidator(new QIntValidator(0, 255, this));
	ui.ledt_3->setValidator(new QIntValidator(0, 255, this));
	ui.ledt_4->setValidator(new QIntValidator(0, 255, this));
	m_apEdt[logiclane::EMLOGICLANEDETAILS_EDT_MINSNAPTIME] = ui.ledt_minsnaptime;
	m_apEdt[logiclane::EMLOGICLANEDETAILS_EDT_SPEEDTIME12] = ui.ledt_speedtime12;
	m_apEdt[logiclane::EMLOGICLANEDETAILS_EDT_SPEEDTIME23] = ui.ledt_speedtime23;
	m_apEdt[logiclane::EMLOGICLANEDETAILS_EDT_SPEEDRANG12] = ui.ledt_speeddistance12;
	m_apEdt[logiclane::EMLOGICLANEDETAILS_EDT_SPEEDRANG23] = ui.ledt_speeddistance23;
	ui.ledt_speedtime23->setValidator(new QIntValidator(1, 100000, this));
	ui.ledt_speedtime12->setValidator(new QIntValidator(1, 100000, this));
	ui.ledt_speeddistance12->setValidator(new QIntValidator(1, 100000, this));
	ui.ledt_speeddistance23->setValidator(new QIntValidator(1, 100000, this));
	m_apEdt[logiclane::EMLOGICLANEDETAILS_EDT_CHILDPLATE_WIDHT] = ui.ledt_childplate_widht;
	m_apEdt[logiclane::EMLOGICLANEDETAILS_EDT_CHILDPLATE_HEIGHT] = ui.ledt_childplate_height;
	ui.ledt_childplate_widht->setValidator(new QIntValidator(0, 1000, this));
	ui.ledt_childplate_height->setValidator(new QIntValidator(0, 1000, this));
	m_apEdt[logiclane::EMLOGICLANEDETAILS_EDT_CHILDVEHICLE_WIDHT] = ui.ledt_childvehicle_widht;
	m_apEdt[logiclane::EMLOGICLANEDETAILS_EDT_CHILDVEHICLE_HEIGHT] = ui.ledt_chilvehicle_height;
	ui.ledt_childvehicle_widht->setValidator(new QIntValidator(0, 3000, this));
	ui.ledt_chilvehicle_height->setValidator(new QIntValidator(0, 3000, this));
	m_apEdt[logiclane::EMLOGICLANEDETAILS_EDT_CHILDPOEPLE_WIDHT] = ui.ledt_childpoeple_widht;
	m_apEdt[logiclane::EMLOGICLANEDETAILS_EDT_CHILDPOEPLE_HEIGHT] = ui.ledt_childpoeple_height;
	m_apEdt[logiclane::EMLOGICLANEDETAILS_EDT_CHILDPOEPLE_WIDHT_2] = ui.ledt_childpoeple_widht_2;
	m_apEdt[logiclane::EMLOGICLANEDETAILS_EDT_CHILDPOEPLE_HEIGHT_2] = ui.ledt_childpoeple_height_2;
	//ui.ledt_childpoeple_widht->setValidator(new QIntValidator(64, 512, this));
	ui.ledt_childpoeple_height->setValidator(new QIntValidator(0, 1000, this));
	//ui.ledt_childpoeple_widht_2->setValidator(new QIntValidator(64, 1024, this));
	ui.ledt_childpoeple_height_2->setValidator(new QIntValidator(0, 1000, this));
	m_apEdt[logiclane::EMLOGICLANEDETAILS_EDT_RATELIMIT_LITTLE] = ui.edtratelimit_little;
	m_apEdt[logiclane::EMLOGICLANEDETAILS_EDT_RATELIMIT_BIG] = ui.edtratelimit_big;
	m_apEdt[logiclane::EMLOGICLANEDETAILS_EDT_SNAPLIMIT_LITTLE] = ui.edtsnaplimit_little;
	m_apEdt[logiclane::EMLOGICLANEDETAILS_EDT_SNAPLIMIT_BIG] = ui.edtsnaplimit_big;
	m_apEdt[logiclane::EMLOGICLANEDETAILS_EDT_RATEOFFSET] = ui.edtrateoffset;
	m_apEdt[logiclane::EMLOGICLANEDETAILS_EDT_MINREDLIGHTTIME] = ui.edtminredlighttime;
	m_apEdt[logiclane::EMLOGICLANEDETAILS_EDT_MAXREDLIGHTTIME] = ui.edtmaxredlighttime;
	m_apEdt[logiclane::EMLOGICLANEDETAILS_EDT_GZZXD] = ui.edt_gzzxd;
	m_apEdt[logiclane::EMLOGICLANEDETAILS_EDT_SNAPNUM2] = ui.ledt_snapNum2;

	/*m_apEdtWeek[0] = ui.ledt_1_hour_from;
	m_apEdtWeek[1] = ui.ledt_1_min_from;
	m_apEdtWeek[2] = ui.ledt_1_hour_to;
	m_apEdtWeek[3] = ui.ledt_1_min_to;
	m_apEdtWeek[4] = ui.ledt_2_hour_from;
	m_apEdtWeek[5] = ui.ledt_2_min_from;
	m_apEdtWeek[6] = ui.ledt_2_hour_to;
	m_apEdtWeek[7] = ui.ledt_2_min_to;
	m_apEdtWeek[8] = ui.ledt_3_hour_from;
	m_apEdtWeek[9] = ui.ledt_3_min_from;
	m_apEdtWeek[10] = ui.ledt_3_hour_to;
	m_apEdtWeek[11] = ui.ledt_3_min_to;
	m_apEdtWeek[12] = ui.ledt_4_hour_from;
	m_apEdtWeek[13] = ui.ledt_4_min_from;
	m_apEdtWeek[14] = ui.ledt_4_hour_to;
	m_apEdtWeek[15] = ui.ledt_4_min_to;*/
	m_apWeek[0] = ui.btn_day7;
	m_apWeek[1] = ui.btn_day1;
	m_apWeek[2] = ui.btn_day2;
	m_apWeek[3] = ui.btn_day3;
	m_apWeek[4] = ui.btn_day4;
	m_apWeek[5] = ui.btn_day5;
	m_apWeek[6] = ui.btn_day6;

	m_apChkDay[0] = ui.chk_day7;
	m_apChkDay[1] = ui.chk_day1;
	m_apChkDay[2] = ui.chk_day2;
	m_apChkDay[3] = ui.chk_day3;
	m_apChkDay[4] = ui.chk_day4;
	m_apChkDay[5] = ui.chk_day5;
	m_apChkDay[6] = ui.chk_day6;

	m_apChkTailLpr[0] = ui.chk_0;
	m_apChkTailLpr[1] = ui.chk_1;
	m_apChkTailLpr[2] = ui.chk_2;
	m_apChkTailLpr[3] = ui.chk_3;
	m_apChkTailLpr[4] = ui.chk_4;
	m_apChkTailLpr[5] = ui.chk_5;
	m_apChkTailLpr[6] = ui.chk_6;
	m_apChkTailLpr[7] = ui.chk_7;
	m_apChkTailLpr[8] = ui.chk_8;
	m_apChkTailLpr[9] = ui.chk_9;

	//m_apChkLocalLpr = static_cast<CCheckButtonUI*>(m_pm.FindControl(TEXT("chklocallpr")));

	memset(m_apChk, 0, sizeof(m_apChk));
	m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_DECTOR1] = ui.chk_test1;
	m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_DECTOR2] = ui.chk_test2;
	m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_DECTOR3] = ui.chk_test3;
	m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_DECTOR4] = ui.chk_test4;
	m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_CHILD_PLATE] = ui.chk_bsd_child_plate;
	m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_CHILD_VEHICLE] = ui.chk_bsd_child_vehicle;
	m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_CHILD_POEPLE] = ui.chk_bsd_child_poeple;
	m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_ADD_FACE] = ui.chkadd_face;
	m_apChk[logiclane::EMLOGICLANEDETAILS_CHKREDLIGHT_LEFT] = ui.chkredlight_left;
	m_apChk[logiclane::EMLOGICLANEDETAILS_CHKREDLIGHT_LINE] = ui.chkredlight_line;
	m_apChk[logiclane::EMLOGICLANEDETAILS_CHKREDLIGHT_RIGHT] = ui.chkredlight_right;
	m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_HPGL_YELLOW] = ui.chkplate_yellow;
	m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_HPGL_WHITE] = ui.chkplate_white;
	m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_HPGL_BLUE] = ui.chkplate_blue;
	m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_HPGL_BLACK] = ui.chkplate_black;
	m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_HPGL_GREEN] = ui.chkplate_green;
	m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_HPGL_YELLOW_GREEN] = ui.chkplate_yellow_green;
	m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_HPGL_NO] = ui.chkplate_no;
	m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_SPEED_CHECK] = ui.chkspeedcheck;
	m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_VEH_0] = ui.chkveh_0;
	m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_VEH_1] = ui.chkveh_1;
	m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_VEH_2] = ui.chkveh_2;
	m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_VEH_3] = ui.chkveh_3;
	m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_VEH_4] = ui.chkveh_4;
	m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_VEH_5] = ui.chkveh_5;
	m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_VEH_6] = ui.chkveh_6;
	m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_VEH_7] = ui.chkveh_7;
	m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_VEH_8] = ui.chkveh_8;

	m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_MERGE_LEFT_TOP] = ui.chk_leftTop;
	m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_MERGE_RIGHT_TOP] = ui.chk_rightTop;
	m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_MERGE_LEFT_BOM] = ui.chk_leftBom;
	m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_MERGE_RIGHT_BOM] = ui.chk_rightBom;
	m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_MERGE_SELECT_DEST] = ui.chk_selectDest;
}

Dlg_LogicLane_Detail::~Dlg_LogicLane_Detail()
{
	SAFE_DELETE(m_pLogicLane);
}

void Dlg_LogicLane_Detail::ShowData(TIPC_LogicLane* pData)
{
	if (!m_bSetData) return;

	if (MgrData::getInstance()->GetDevVersion() > 0)
	{
		m_bSetlight = true;
	}
	else
	{
		m_bSetlight = false;
	}

	if (m_bSetlight == false)
	{
		ui.widget_10->setVisible(false);
	}


	emit ui.tab_lane1->clicked(true);
	ui.tab_lane3->setVisible(false);
	ui.tab_lane4->setVisible(false);

	switch (pData->dwLgRdType)
	{
	case LOGIC_ROAD_TYPE_YX_2:
	{
		// 压实线
		ui.label_8->setText(GBUTF8("(规则线：不可压的车道实线编号)"));
	}
		break;
	case LOGIC_ROAD_TYPE_VEH_DISTANCE:
	{
		// 不按规定保持行车安全距离
		ui.label_6->setText(GBUTF8("(规则线：前车离开测距截止线)"));
		ui.label_7->setText(GBUTF8("(规则线：前车继续行驶到指定位置)"));
		ui.label_8->setText(GBUTF8("(规则区域：测距区域)"));
	}
		break;
	case LOGIC_ROAD_TYPE_LINE_STOP:
	{
		// 越线停车
		ui.label_6->setText(GBUTF8("(规则区域：检测停车的区域)"));
		ui.label_7->setText(GBUTF8("(规则区域：检测停车的区域)"));

		ui.ledt_4->setEnabled(false);
		ui.chk_test4->setEnabled(false);
		ui.tab_lane4->setVisible(true);
	}
		break;
	case LOGIC_ROAD_TYPE_pedestrian_crossing_stop:
	{
		// 人行横道、黄网格停车
		ui.label_6->setText(GBUTF8("(规则区域：检测停车的区域)"));
		ui.label_7->setText(GBUTF8("(规则区域：检测停车的区域)"));

		ui.ledt_4->setEnabled(false);
		ui.chk_test4->setEnabled(false);
	}
		break;
	case LOGIC_ROAD_TYPE_pedestrian_run_red_light:
	{
		// 行人或非机动车闯红灯
		ui.label_8->setText(GBUTF8("(检测器：行人、非机动车检测器)"));
		ui.tab_lane4->setVisible(true);
	}
		break;
	case LOGIC_ROAD_TYPE_KK:
	{
		// 卡口
		ui.label_8->setText(GBUTF8("(检测器：行人、非机动车检测器)"));
	}
		break;
	case LOGIC_ROAD_TYPE_CS:
	{
		// 超速
		ui.tab_lane3->setVisible(true);
		ui.ledt_4->setEnabled(false);
		ui.chk_test4->setEnabled(false);
	}
		break;
	case LOGIC_ROAD_TYPE_HD:
	{
		// 机动车闯红灯
		ui.tab_lane4->setVisible(true);
		ui.ledt_4->setEnabled(false);
		ui.chk_test4->setEnabled(false);
	}
		break;
	case LOGIC_ROAD_TYPE_ZD:
	{
		// 不按规定车道行驶
		ui.chkplate_yellow->setText(GBUTF8("黄牌(客车)"));
		ui.chkplate_blue->setText(GBUTF8("蓝牌(货车)"));
		ui.ledt_4->setEnabled(false);
		ui.chk_test4->setEnabled(false);
	}
		break;
	default:
		break;
	}

	switch (pData->dwExtLgRdType)
	{
	case EXT_LOGIC_ROAD_TYPE_YXYJ:
	{
		// 越线预警
		ui.tab_lane4->setVisible(true);

		ui.label_5->setText(GBUTF8("(行人检测器)"));
		ui.label_6->setText(GBUTF8("(预警区域)"));
		ui.label_7->setText(GBUTF8(""));
		ui.label_8->setText(GBUTF8(""));
		ui.ledt_3->setEnabled(false);
		ui.chk_test3->setEnabled(false);
		ui.ledt_4->setEnabled(false);
		ui.chk_test4->setEnabled(false);
	}
		break;
	default:
		break;
	}

	//设置是否隐藏
	Uint32 nTempW = m_pLogicLane->dwFacePicW & 0xffff;
	Uint32 nTempH = m_pLogicLane->dwFacePicH & 0xffff;
	
	if (nTempH == 0 && nTempW == 0)
	{
		ui.cmb_face1->setCurrentIndex(0);
		ui.widget_8->setVisible(false);

	}
	else
	{
		ui.cmb_face1->setCurrentIndex(1);
		ui.widget_8->setVisible(true);
	}

	Uint32 nTempW2 = m_pLogicLane->dwFacePicW & 0xffff0000;
	Uint32 nTempH2 = m_pLogicLane->dwFacePicH & 0xffff0000;
	if (nTempW2 == 0 && nTempW2 == 0)
	{
		ui.cmb_face2->setCurrentIndex(0);
		ui.widget_9->setVisible(false);

	}
	else
	{
		ui.cmb_face2->setCurrentIndex(1);
		ui.widget_9->setVisible(true);

	}


	// cmb 获取
	for (int i = 0; i < logiclane::EMLOGICLANEDETAILS_COUNT; i++)
	{
		if (m_apCmb[i] != NULL)
		{
			logiclane::SetCmbValue(m_pLogicLane, m_apCmb[i], i);
		}
	}
		

	// chk 获取
	for (int i = 0; i < logiclane::EMLOGICLANEDETAILS_COUNT; i++)
	{
		if (m_apChk[i] != NULL)
		{
			logiclane::SetChkValue(m_pLogicLane, m_apChk[i], i);
		}
	}

	// edit 值获取
	for (int i = 0; i < logiclane::EMLOGICLANEDETAILS_COUNT; i++)
	{
		if (m_apEdt[i] != NULL)
		{
			logiclane::SetEdtValue(m_pLogicLane, m_apEdt[i], i);
		}
	}
		
	// 抓拍间隔类型-需要隐藏其他选项
	OnSwitchSnapCtrlMode(ui.cmb_snapctrl->currentIndex());

	PlateImgChange();
	VehicleImgChange();
	PeopleImgChange();
	SetWeekData(0);
	UpdateAddFace(m_pLogicLane->bAddFaceSubPic, true);

	if (pData->dwLgRdType == 1 && pData->dwExtLgRdType == 0)
	{
		ui.label_63->setVisible(true);
		ui.chk_kkVideo->setVisible(true);
	}
	else
	{
		ui.label_63->setVisible(false);
		ui.chk_kkVideo->setVisible(false);
	}

}

void Dlg_LogicLane_Detail::SetWeekData(int index)
{
	int i = 0;
	ui.ledt_1_hour_from->setText(QString("%1").arg(m_pLogicLane->tSnapTiming[index].startTimeHour[i]));
	ui.ledt_1_min_from->setText(QString("%1").arg(m_pLogicLane->tSnapTiming[index].startTimeMin[i]));
	ui.ledt_1_hour_to->setText(QString("%1").arg(m_pLogicLane->tSnapTiming[index].stopTimeHour[i]));
	ui.ledt_1_min_to->setText(QString("%1").arg(m_pLogicLane->tSnapTiming[index].stopTimeMin[i]));
	i++;
	ui.ledt_2_hour_from->setText(QString("%1").arg(m_pLogicLane->tSnapTiming[index].startTimeHour[i]));
	ui.ledt_2_min_from->setText(QString("%1").arg(m_pLogicLane->tSnapTiming[index].startTimeMin[i]));
	ui.ledt_2_hour_to->setText(QString("%1").arg(m_pLogicLane->tSnapTiming[index].stopTimeHour[i]));
	ui.ledt_2_min_to->setText(QString("%1").arg(m_pLogicLane->tSnapTiming[index].stopTimeMin[i]));
	i++;
	ui.ledt_3_hour_from->setText(QString("%1").arg(m_pLogicLane->tSnapTiming[index].startTimeHour[i]));
	ui.ledt_3_min_from->setText(QString("%1").arg(m_pLogicLane->tSnapTiming[index].startTimeMin[i]));
	ui.ledt_3_hour_to->setText(QString("%1").arg(m_pLogicLane->tSnapTiming[index].stopTimeHour[i]));
	ui.ledt_3_min_to->setText(QString("%1").arg(m_pLogicLane->tSnapTiming[index].stopTimeMin[i]));
	i++;
	ui.ledt_4_hour_from->setText(QString("%1").arg(m_pLogicLane->tSnapTiming[index].startTimeHour[i]));
	ui.ledt_4_min_from->setText(QString("%1").arg(m_pLogicLane->tSnapTiming[index].startTimeMin[i]));
	ui.ledt_4_hour_to->setText(QString("%1").arg(m_pLogicLane->tSnapTiming[index].stopTimeHour[i]));
	ui.ledt_4_min_to->setText(QString("%1").arg(m_pLogicLane->tSnapTiming[index].stopTimeMin[i]));

	/*for (int i = 0; i < 4; i++)
	{
		ui.ledt_1_hour_from->setText("0");
		m_apEdtWeek[i * 4 + 0]->setText(QString("%1").arg(m_pLogicLane->tSnapTiming[index].startTimeHour[i]));
		m_apEdtWeek[i * 4 + 1]->setText(QString("%1").arg(m_pLogicLane->tSnapTiming[index].startTimeMin[i]));
		m_apEdtWeek[i * 4 + 2]->setText(QString("%1").arg(m_pLogicLane->tSnapTiming[index].stopTimeHour[i]));
		m_apEdtWeek[i * 4 + 3]->setText(QString("%1").arg(m_pLogicLane->tSnapTiming[index].stopTimeMin[i]));
	}
*/
	for (int i = 0; i < 7; i++)
	{
		if (i == index)
		{
			m_apChkDay[i]->setChecked(true);
			m_apChkDay[i]->setEnabled(false);
		}
		else
		{
			m_apChkDay[i]->setChecked(false);
			m_apChkDay[i]->setEnabled(true);
		}
	}

	ui.chk_limitOuterPlate->setChecked(m_pLogicLane->bLocalLpr[index]);

	for (int i = 0; i < 10; i++)
	{
		int status = GetSpecifyBitValue(m_pLogicLane->dwLprTailNum[index], i);
		m_apChkTailLpr[i]->setChecked(status);
	}
}

// 获取指定位的值0/1
int Dlg_LogicLane_Detail::GetSpecifyBitValue(Uint32 dwSrc, int nIndex)
{
	Uint32 dw2 = ((int)pow(2.0, nIndex));
	dwSrc = dwSrc & dw2;
	dwSrc = dwSrc >> nIndex;

	return dwSrc;
}

u32 Dlg_LogicLane_Detail::SetSpecifyBitValue(Uint32& dwSrcOut, Uint32 dwValue, int nIndex)
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

void Dlg_LogicLane_Detail::PeopleImgChange()
{
	if (m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_CHILD_POEPLE]->isChecked())
	{
		m_apEdt[logiclane::EMLOGICLANEDETAILS_EDT_CHILDPOEPLE_WIDHT]->setEnabled(true);
		m_apEdt[logiclane::EMLOGICLANEDETAILS_EDT_CHILDPOEPLE_HEIGHT]->setEnabled(true);
	}
	else
	{
		m_apEdt[logiclane::EMLOGICLANEDETAILS_EDT_CHILDPOEPLE_WIDHT]->setEnabled(false);
		m_apEdt[logiclane::EMLOGICLANEDETAILS_EDT_CHILDPOEPLE_HEIGHT]->setEnabled(false);
	}
}

void Dlg_LogicLane_Detail::PlateImgChange()
{
	if (m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_CHILD_PLATE]->isChecked())
	{
		m_apEdt[logiclane::EMLOGICLANEDETAILS_EDT_CHILDPLATE_WIDHT]->setEnabled(true);
		m_apEdt[logiclane::EMLOGICLANEDETAILS_EDT_CHILDPLATE_HEIGHT]->setEnabled(true);
	}
	else
	{
		m_apEdt[logiclane::EMLOGICLANEDETAILS_EDT_CHILDPLATE_WIDHT]->setEnabled(false);
		m_apEdt[logiclane::EMLOGICLANEDETAILS_EDT_CHILDPLATE_HEIGHT]->setEnabled(false);
	}
}


void Dlg_LogicLane_Detail::VehicleImgChange()
{
	if (m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_CHILD_VEHICLE]->isChecked())
	{
		m_apEdt[logiclane::EMLOGICLANEDETAILS_EDT_CHILDVEHICLE_WIDHT]->setEnabled(true);
		m_apEdt[logiclane::EMLOGICLANEDETAILS_EDT_CHILDVEHICLE_HEIGHT]->setEnabled(true);
	}
	else
	{
		m_apEdt[logiclane::EMLOGICLANEDETAILS_EDT_CHILDVEHICLE_WIDHT]->setEnabled(false);
		m_apEdt[logiclane::EMLOGICLANEDETAILS_EDT_CHILDVEHICLE_HEIGHT]->setEnabled(false);
	}
}


void Dlg_LogicLane_Detail::OnSwitchSnapCtrlMode(int nShowIndex)
{
	if (nShowIndex == 0)
	{
		ui.label_11->setVisible(true);
		ui.ledt_speedtime12->setVisible(true);
		ui.label_10->setVisible(true);
		ui.ledt_speedtime23->setVisible(true);
	}
	else
	{
		ui.label_11->setVisible(false);
		ui.ledt_speedtime12->setVisible(false);
		ui.label_10->setVisible(false);
		ui.ledt_speedtime23->setVisible(false);
	}

	if (nShowIndex == 1)
	{
		ui.label_21->setVisible(true);
		ui.label_22->setVisible(true);
		ui.ledt_speeddistance12->setVisible(true);
		ui.ledt_speeddistance23->setVisible(true);
	}
	else
	{
		ui.label_21->setVisible(false);
		ui.label_22->setVisible(false);
		ui.ledt_speeddistance12->setVisible(false);
		ui.ledt_speeddistance23->setVisible(false);
	}
}

void Dlg_LogicLane_Detail::SetData(TIPC_LogicLane* pData)
{
	m_bSetData = true;
	memcpy(m_pLogicLane, pData, sizeof(TIPC_LogicLane));
	m_pLogicLaneSrc = pData;

	ShowData(m_pLogicLane);
}

void Dlg_LogicLane_Detail::GetData()
{
	// cmb 获取
	for (int i = 0; i < logiclane::EMLOGICLANEDETAILS_COUNT; i++)
	{
		if (m_bSetlight == false && i >= logiclane::EMLOGICLANEDETAILS_CMB_PIC2DAY
			&& i <= logiclane::EMLOGICLANEDETAILS_CMB_PIC4NIGHT)
		{
			continue;
		}

		if (m_apCmb[i] != NULL)
		{
			logiclane::GetCmbValue(m_pLogicLane, m_apCmb[i], i);
		}
	}
		
	// chk 获取
	for (int i = 0; i < logiclane::EMLOGICLANEDETAILS_COUNT; i++)
	{
		if (m_apChk[i] != NULL)
		{
			logiclane::GetChkValue(m_pLogicLane, m_apChk[i], i);
		}
	}
	//人脸isAddFace字段 拼接
	UpdateAddFace(m_pLogicLane->bAddFaceSubPic, false);

	// edit 值获取
	for (int i = 0; i < logiclane::EMLOGICLANEDETAILS_COUNT; i++)
	{
		if (m_apEdt[i] != NULL)
		{
			logiclane::GetEdtValue(m_pLogicLane, m_apEdt[i], i);
		}
	}
		
	GetWeekData(m_nWeek);

	// 拷贝到保存位置
	memcpy(m_pLogicLaneSrc, m_pLogicLane, sizeof(TIPC_LogicLane));
}


void Dlg_LogicLane_Detail::SetOrderCmbData(bool isAddSpecial)
{
	ui.com_order->setProperty("id", 111);
	ui.com_order->clear();
	int nPage = ui.cmb_snapNum->currentIndex() + 1;//几张

	QStringList sList;
	for (int i = 0; i < nPage; i++)
	{
		sList.append(GBUTF8("图%1").arg(i + 1));
	}

	if (isAddSpecial)
	{
		sList.append(GBUTF8("特写"));
	}
	ui.com_order->setProperty("id", 111);
	ui.com_order->addItems(sList);
}



void Dlg_LogicLane_Detail::UpdateAddFace(bool  isEnable, bool is)
{
	if (is)
	{
		Uint8 byAddFace = m_pLogicLane->bAddFaceSubPic;
		Uint8 byPos = byAddFace & 0x0f;
		bool isLeftTop = byPos & 0x1;
		bool isRightTop = byPos & 0x2;
		bool isLeftBom = byPos & 0x4;
		bool isRightBom = byPos & 0x8;

		ui.chk_leftTop->setChecked(isLeftTop);
		ui.chk_rightTop->setChecked(isRightTop);
		ui.chk_leftBom->setChecked(isLeftBom);
		ui.chk_rightBom->setChecked(isRightBom);

		if (isEnable)
		{
			ui.widget_4->setVisible(true);
			ui.widget_4->setEnabled(true);
			if (ui.chk_leftTop->isChecked() || ui.chk_rightTop->isChecked())
			{
				ui.chk_leftBom->setEnabled(false);
				ui.chk_rightBom->setEnabled(false);

			}
			if (ui.chk_rightBom->isChecked() || ui.chk_leftBom->isChecked())
			{
				ui.chk_leftTop->setEnabled(false);
				ui.chk_rightTop->setEnabled(false);
			}

			ui.com_pos->setEnabled(true);
			ui.chkadd_face->setChecked(true);
		}
		else
		{
			ui.widget_4->setVisible(false);
			ui.widget_4->setEnabled(false);
		}
		if (ui.chk_bsd_child_vehicle->isChecked())
		{
			SetOrderCmbData(true);
		}
		else
		{
			SetOrderCmbData();
		}

		//人脸位置index
		Uint8 byIndex = (byAddFace >> 4) & 0x7;
		int nSel = byIndex - 1;
		ui.com_order->setProperty("id", 111);
		ui.com_order->setCurrentIndex(nSel);

		/*byPos 0 1*/
		Uint8 bPos = (byAddFace & 0xf0) >> 7;
		ui.com_pos->setCurrentIndex(bPos);

		SetMultiFormatCmbVisible();
	}
	else
	{
		Uint8 AddFaceValue;
		Uint8 byPos = ui.com_pos->currentIndex();/////
		Uint8 byIndex = ui.com_order->currentIndex() + 1;

		AddFaceValue = (byIndex << 4);
		if (ui.chk_leftTop->isChecked())
		{
			AddFaceValue = AddFaceValue | 0x1;
		}
		if (ui.chk_rightTop->isChecked())
		{
			AddFaceValue = AddFaceValue | 0x2;
		}
		if (ui.chk_leftBom->isChecked())
		{
			AddFaceValue = AddFaceValue | 0x4;
		}
		if (ui.chk_rightBom->isChecked())
		{
			AddFaceValue = AddFaceValue | 0x8;
		}
		AddFaceValue = AddFaceValue | (byPos << 7);

		m_pLogicLane->bAddFaceSubPic = AddFaceValue;
	}
}


void Dlg_LogicLane_Detail::SetMultiFormatCmbVisible()
{
	int nCount = 0;
	nCount = m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_MERGE_LEFT_TOP]->isChecked() ? ++nCount : nCount;
	nCount = m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_MERGE_LEFT_BOM]->isChecked() ? ++nCount : nCount;
	nCount = m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_MERGE_RIGHT_TOP]->isChecked() ? ++nCount : nCount;
	nCount = m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_MERGE_RIGHT_BOM]->isChecked() ? ++nCount : nCount;
	if (nCount > 1)
	{
		ui.com_pos->setVisible(true);
		ui.label_29->setVisible(true);
	}
	else
	{
		ui.com_pos->setVisible(false);
		ui.label_29->setVisible(false);
	}

}

void Dlg_LogicLane_Detail::GetWeekData(int index)
{
	int begin_hour, begin_min, end_hour, end_min;
	int n = 0;
	m_pLogicLane->tSnapTiming[index].startTimeHour[n] = ui.ledt_1_hour_from->text().toInt();
	m_pLogicLane->tSnapTiming[index].startTimeMin[n] = ui.ledt_1_min_from->text().toInt();
	m_pLogicLane->tSnapTiming[index].stopTimeHour[n] = ui.ledt_1_hour_to->text().toInt();
	m_pLogicLane->tSnapTiming[index].stopTimeMin[n] = ui.ledt_1_min_to->text().toInt();
	n++;
	m_pLogicLane->tSnapTiming[index].startTimeHour[n] = ui.ledt_2_hour_from->text().toInt();
	m_pLogicLane->tSnapTiming[index].startTimeMin[n] = ui.ledt_2_min_from->text().toInt();
	m_pLogicLane->tSnapTiming[index].stopTimeHour[n] = ui.ledt_2_hour_to->text().toInt();
	m_pLogicLane->tSnapTiming[index].stopTimeMin[n] = ui.ledt_2_min_to->text().toInt();
	n++;
	m_pLogicLane->tSnapTiming[index].startTimeHour[n] = ui.ledt_3_hour_from->text().toInt();
	m_pLogicLane->tSnapTiming[index].startTimeMin[n] = ui.ledt_3_min_from->text().toInt();
	m_pLogicLane->tSnapTiming[index].stopTimeHour[n] = ui.ledt_3_hour_to->text().toInt();
	m_pLogicLane->tSnapTiming[index].stopTimeMin[n] = ui.ledt_3_min_to->text().toInt();
	n++;
	m_pLogicLane->tSnapTiming[index].startTimeHour[n] = ui.ledt_4_hour_from->text().toInt();
	m_pLogicLane->tSnapTiming[index].startTimeMin[n] = ui.ledt_4_min_from->text().toInt();
	m_pLogicLane->tSnapTiming[index].stopTimeHour[n] = ui.ledt_4_hour_to->text().toInt();
	m_pLogicLane->tSnapTiming[index].stopTimeMin[n] = ui.ledt_4_min_to->text().toInt();
	n++;
	/*
	for (int i = 0; i < 4; i++)
	{
		begin_hour = m_apEdtWeek[i * 4 + 0]->text().toInt();
		begin_min = m_apEdtWeek[i * 4 + 1]->text().toInt();
		end_hour = m_apEdtWeek[i * 4 + 2]->text().toInt();
		end_min = m_apEdtWeek[i * 4 + 3]->text().toInt();

		if ((end_hour * 60 + end_min) > (begin_hour * 60 + begin_min))
		{
			m_pLogicLane->tSnapTiming[index].startTimeHour[n] = begin_hour;
			m_pLogicLane->tSnapTiming[index].startTimeMin[n] = begin_min;
			m_pLogicLane->tSnapTiming[index].stopTimeHour[n] = end_hour;
			m_pLogicLane->tSnapTiming[index].stopTimeMin[n] = end_min;
			n++;
		}
	}
	*/

	m_pLogicLane->tSnapTiming[index].num = n;

	m_pLogicLane->bLocalLpr[index] = ui.chk_limitOuterPlate->isChecked();

	for (int i = 0; i < 10; i++)
	{
		Uint32 status = m_apChkTailLpr[i]->isChecked();
		SetSpecifyBitValue(m_pLogicLane->dwLprTailNum[index], status, i);
	}
}

void Dlg_LogicLane_Detail::CopyWeekData()
{
	GetWeekData(m_nWeek);

	for (int i = 0; i < 7; i++)
	{
		if (i != m_nWeek && m_apChkDay[i]->isChecked())
		{
			memcpy(&m_pLogicLane->tSnapTiming[i], &m_pLogicLane->tSnapTiming[m_nWeek], sizeof(TIPC_SnapTiming));
		}
	}
}

void Dlg_LogicLane_Detail::Slot_DayCopyClicked()
{
	if (this->sender() == ui.btn_dayCopy)
	{
		CopyWeekData();
		return;
	}
}

void Dlg_LogicLane_Detail::Slot_BtnSaveClicked()
{
	if (this->sender() == ui.btn_cancel)
	{
		this->done(0);
		return;
	}
	GetData();
	this->done(1);
}

void Dlg_LogicLane_Detail::Slot_EditTextChanged(const QString &str)
{
	if (this->sender() == m_apEdt[logiclane::EMLOGICLANEDETAILS_EDT_RATELIMIT_LITTLE])
	{
		logiclane::GetEdtValue(m_pLogicLane, m_apEdt[logiclane::EMLOGICLANEDETAILS_EDT_RATELIMIT_LITTLE], logiclane::EMLOGICLANEDETAILS_EDT_RATELIMIT_LITTLE);
		logiclane::GetEdtValue(m_pLogicLane, m_apEdt[logiclane::EMLOGICLANEDETAILS_EDT_SNAPLIMIT_LITTLE], logiclane::EMLOGICLANEDETAILS_EDT_SNAPLIMIT_LITTLE);
		if (m_pLogicLane->dwCarLmSpd > m_pLogicLane->dwCarSnapSpd)
		{
			m_pLogicLane->dwCarSnapSpd = m_pLogicLane->dwCarLmSpd;
			logiclane::SetEdtValue(m_pLogicLane, m_apEdt[logiclane::EMLOGICLANEDETAILS_EDT_SNAPLIMIT_LITTLE], logiclane::EMLOGICLANEDETAILS_EDT_SNAPLIMIT_LITTLE);
		}
	}
	else if (this->sender() == m_apEdt[logiclane::EMLOGICLANEDETAILS_EDT_RATELIMIT_BIG])
	{
		logiclane::GetEdtValue(m_pLogicLane, m_apEdt[logiclane::EMLOGICLANEDETAILS_EDT_RATELIMIT_BIG], logiclane::EMLOGICLANEDETAILS_EDT_RATELIMIT_BIG);
		logiclane::GetEdtValue(m_pLogicLane, m_apEdt[logiclane::EMLOGICLANEDETAILS_EDT_SNAPLIMIT_BIG], logiclane::EMLOGICLANEDETAILS_EDT_SNAPLIMIT_BIG);
		if (m_pLogicLane->dwTruckLmSpd > m_pLogicLane->dwTruckSnapSpd)
		{
			m_pLogicLane->dwTruckSnapSpd = m_pLogicLane->dwTruckLmSpd;
			logiclane::SetEdtValue(m_pLogicLane, m_apEdt[logiclane::EMLOGICLANEDETAILS_EDT_SNAPLIMIT_BIG], logiclane::EMLOGICLANEDETAILS_EDT_SNAPLIMIT_BIG);
		}
	}
}

void Dlg_LogicLane_Detail::Slot_CheckBoxClicked(int nValue)
{
	 if (this->sender() == m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_MERGE_LEFT_TOP]
		 || this->sender() == m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_MERGE_RIGHT_TOP])
	{
		UpdateAddFace(true, false);//先设值
		SetMultiFormatCmbVisible();
		if (m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_MERGE_LEFT_TOP]->isChecked()
			|| m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_MERGE_RIGHT_TOP]->isChecked())
		{
			m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_MERGE_LEFT_BOM]->setEnabled(false);
			m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_MERGE_RIGHT_BOM]->setEnabled(false);
		}
		else
		{
			m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_MERGE_LEFT_BOM]->setEnabled(true);
			m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_MERGE_RIGHT_BOM]->setEnabled(true);
		}
	}
	else if (this->sender() == m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_MERGE_RIGHT_BOM]
		|| this->sender() == m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_MERGE_LEFT_BOM])
	{
		UpdateAddFace(true, false);//先设值
		SetMultiFormatCmbVisible();
		if (m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_MERGE_RIGHT_BOM]->isChecked()
			|| m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_MERGE_LEFT_BOM]->isChecked())
		{
			m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_MERGE_LEFT_TOP]->setEnabled(false);
			m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_MERGE_RIGHT_TOP]->setEnabled(false);
		}
		else
		{
			m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_MERGE_LEFT_TOP]->setEnabled(true);
			m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_MERGE_RIGHT_TOP]->setEnabled(true);
		}
	}
	else if (this->sender() == m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_CHILD_PLATE])
	{
		PlateImgChange();
	}
	else if (this->sender() == m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_CHILD_VEHICLE])
	{
		bool isChecked = m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_ADD_FACE]->isChecked();
		UpdateAddFace(isChecked, true);
		VehicleImgChange();
	}
	else if (this->sender() == m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_CHILD_POEPLE])
	{
		PeopleImgChange();
	}
	else if (this->sender() == m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_ADD_FACE])
	{
		bool isClicked = m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_ADD_FACE]->isChecked();
		//不勾选清空
		if (!isClicked)
		{
			m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_MERGE_LEFT_TOP]->setChecked(false);
			m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_MERGE_LEFT_BOM]->setChecked(false);
			m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_MERGE_RIGHT_BOM]->setChecked(false);
			m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_MERGE_RIGHT_TOP]->setChecked(false);

			m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_MERGE_LEFT_TOP]->setEnabled(true);
			m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_MERGE_LEFT_BOM]->setEnabled(true);
			m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_MERGE_RIGHT_BOM]->setEnabled(true);
			m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_MERGE_RIGHT_TOP]->setEnabled(true);
			ui.com_pos->setCurrentIndex(0);
			ui.com_order->setCurrentIndex(-1);
		}

		UpdateAddFace(isClicked, false);//先设值
		UpdateAddFace(isClicked, true);//显示

	}
	
}

void Dlg_LogicLane_Detail::Slot_SwitchTabClicked()
{
	if (this->sender() == ui.tab_lane1)
	{
		ui.stackedWidget->setCurrentIndex(0);
	}
	else if (this->sender() == ui.tab_lane2)
	{
		ui.stackedWidget->setCurrentIndex(1);
	}
	else if (this->sender() == ui.tab_lane3)
	{
		ui.stackedWidget->setCurrentIndex(2);
	}
	else if (this->sender() == ui.tab_lane4)
	{
		ui.stackedWidget->setCurrentIndex(3);
	}
	else if (this->sender() == ui.tab_lane5)
	{
		ui.stackedWidget->setCurrentIndex(4);
	}
	else if (this->sender() == ui.tab_lane6)
	{
		ui.stackedWidget->setCurrentIndex(5);
	}

}

void Dlg_LogicLane_Detail::Slot_CmbIndexChanged(int nIndex)
{
	if (this->sender() == ui.cmb_snapctrl)
	{
		OnSwitchSnapCtrlMode(nIndex);
	}
	else if (this->sender() == ui.cmb_snapNum)
	{
		bool isChecked = m_apChk[logiclane::EMLOGICLANEDETAILS_CHK_ADD_FACE]->isChecked();
		UpdateAddFace(isChecked, true);
		//SetOrderCmbData();
	}
	else if (this->sender() == ui.cmb_face1)
	{
		if (ui.cmb_face1->currentIndex() == 0)
		{
			m_apEdt[logiclane::EMLOGICLANEDETAILS_EDT_CHILDPOEPLE_WIDHT]->setText("0");
			m_apEdt[logiclane::EMLOGICLANEDETAILS_EDT_CHILDPOEPLE_HEIGHT]->setText("0");
			ui.widget_8->setVisible(false);
		}
		else
		{
			ui.widget_8->setVisible(true);
		}
	}
	else if (this->sender() == ui.cmb_face2)
	{
		if (ui.cmb_face2->currentIndex() == 0)
		{
			m_apEdt[logiclane::EMLOGICLANEDETAILS_EDT_CHILDPOEPLE_WIDHT_2]->setText("0");
			m_apEdt[logiclane::EMLOGICLANEDETAILS_EDT_CHILDPOEPLE_HEIGHT_2]->setText("0");
			ui.widget_9->setVisible(false);
		}
		else
		{
			ui.widget_9->setVisible(true);
		}
	}
}

void Dlg_LogicLane_Detail::Slot_SwitchDayClicked()
{
	GetWeekData(m_nWeek);
	if (this->sender() == ui.btn_day7)
	{
		m_nWeek = 0;
	}
	else if (this->sender() == ui.btn_day1)
	{
		m_nWeek = 1;
	}
	else if (this->sender() == ui.btn_day2)
	{
		m_nWeek = 2;
	}
	else if (this->sender() == ui.btn_day3)
	{
		m_nWeek = 3;
	}
	else if (this->sender() == ui.btn_day4)
	{
		m_nWeek = 4;
	}
	else if (this->sender() == ui.btn_day5)
	{
		m_nWeek = 5;
	}
	else if (this->sender() == ui.btn_day6)
	{
		m_nWeek = 6;
	}
	
	SetWeekData(m_nWeek);
}