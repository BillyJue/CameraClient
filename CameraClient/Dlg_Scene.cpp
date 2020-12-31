#include "Dlg_Scene.h"
#include "MgrData.h"
#include <QListView>
#include <QMessageBox>
#include "Dlg_MessageBox.h"
#include "Dlg_SceneDraw.h"
#include "Dlg_Scene_ManualCapture.h"
#include <QTimer>
Dlg_Scene::Dlg_Scene(QWidget *parent)
	: MyWidget(parent), m_bSupportPark(false)
	, m_nSceneIndex(0), m_nShowType(-1), m_nIndex(0), m_bSupport(false), m_messageBox(nullptr)
{
	ui.setupUi(this);
	
	Init();
	memset(&m_extDevType, 0, sizeof(m_extDevType));
	memset(&m_tDevInfo, 0, sizeof(m_tDevInfo));
	memset(&m_tParkParam, 0, sizeof(m_tParkParam));
	connect(ui.cmb_scene, SIGNAL(currentIndexChanged(int)), this, SLOT(Slot_CmbSceneIndexChanged(int)));
	connect(ui.cmb_plateNum, SIGNAL(currentIndexChanged(int)), this, SLOT(Slot_PlateNumChanged(int)));
	connect(this, SIGNAL(signal_devInfoChanged()), this, SLOT(Slot_DeviceInfoChanged()));
	connect(this, SIGNAL(signal_bayonetChanged()), this, SLOT(Slot_BayonentChanged()));
	connect(this, SIGNAL(signal_parkScenceChanged()), this, SLOT(Slot_ParkScenceChanged()));
	connect(this, SIGNAL(signal_parkParamChanged()), this, SLOT(Slot_ParkParamChanged()));
	connect(this, SIGNAL(signal_extDevTypeChanged()), this, SLOT(Slot_ExtDevTypeChanged()));
	
	connect(ui.btn_bayonet, SIGNAL(clicked()), this, SLOT(Slot_TabCurrentChanged()));
	connect(ui.btn_illegalPark, SIGNAL(clicked()), this, SLOT(Slot_TabCurrentChanged()));
	connect(ui.btn_illegalMode, SIGNAL(clicked()), this, SLOT(Slot_TabCurrentChanged()));
	connect(ui.illegal_btnsave, SIGNAL(clicked()), this, SLOT(Slot_BtnSaveClicked()));
	connect(ui.btn_sceneSave, SIGNAL(clicked()), this, SLOT(Slot_BtnSaveClicked()));
	connect(ui.btn_illModeSave, SIGNAL(clicked()), this, SLOT(Slot_BtnSaveClicked()));
	connect(ui.btn_ptzSave, SIGNAL(clicked()), this, SLOT(Slot_BtnSaveClicked()));

	connect(ui.btn_ptzFresh, SIGNAL(clicked()), this, SLOT(Slot_BtnFreshClicked()));
	connect(ui.btn_illModeFresh, SIGNAL(clicked()), this, SLOT(Slot_BtnFreshClicked()));
	connect(ui.btn_sceneFresh, SIGNAL(clicked()), this, SLOT(Slot_BtnFreshClicked()));
	connect(ui.illegal_btnfresh, SIGNAL(clicked()), this, SLOT(Slot_BtnFreshClicked()));

	connect(ui.btn_selectPos, SIGNAL(clicked()), this, SLOT(Slot_BtnSelectPosClicked()));

	connect(ui.btn_leftup, SIGNAL(pressed()), this, SLOT(Slot_PTZControlClicked()));
	connect(ui.btn_up, SIGNAL(pressed()), this, SLOT(Slot_PTZControlClicked()));
	connect(ui.btn_rightup, SIGNAL(pressed()), this, SLOT(Slot_PTZControlClicked()));
	connect(ui.btn_left, SIGNAL(pressed()), this, SLOT(Slot_PTZControlClicked()));
	connect(ui.btn_out, SIGNAL(pressed()), this, SLOT(Slot_PTZControlClicked()));
	connect(ui.btn_right, SIGNAL(pressed()), this, SLOT(Slot_PTZControlClicked()));
	connect(ui.btn_leftdown, SIGNAL(pressed()), this, SLOT(Slot_PTZControlClicked()));
	connect(ui.btn_down, SIGNAL(pressed()), this, SLOT(Slot_PTZControlClicked()));
	connect(ui.btn_rightdown, SIGNAL(pressed()), this, SLOT(Slot_PTZControlClicked()));
	connect(ui.btn_zoomIn, SIGNAL(pressed()), this, SLOT(Slot_PTZControlClicked()));
	connect(ui.btn_zoomOut, SIGNAL(pressed()), this, SLOT(Slot_PTZControlClicked()));

	connect(ui.btn_leftup, SIGNAL(released()), this, SLOT(Slot_PTZStopClicked()));
	connect(ui.btn_up, SIGNAL(released()), this, SLOT(Slot_PTZStopClicked()));
	connect(ui.btn_rightup, SIGNAL(released()), this, SLOT(Slot_PTZStopClicked()));
	connect(ui.btn_left, SIGNAL(released()), this, SLOT(Slot_PTZStopClicked()));
	connect(ui.btn_out, SIGNAL(released()), this, SLOT(Slot_PTZStopClicked()));
	connect(ui.btn_right, SIGNAL(released()), this, SLOT(Slot_PTZStopClicked()));
	connect(ui.btn_leftdown, SIGNAL(released()), this, SLOT(Slot_PTZStopClicked()));
	connect(ui.btn_down, SIGNAL(released()), this, SLOT(Slot_PTZStopClicked()));
	connect(ui.btn_rightdown, SIGNAL(released()), this, SLOT(Slot_PTZStopClicked()));
	connect(ui.btn_zoomIn, SIGNAL(released()), this, SLOT(Slot_PTZStopClicked()));
	connect(ui.btn_zoomOut, SIGNAL(released()), this, SLOT(Slot_PTZStopClicked()));

	connect(ui.cmb_pic1, SIGNAL(currentIndexChanged(int)), this, SLOT(Slot_CmbPictureIndexChanged(int)));
	connect(ui.cmb_pic2, SIGNAL(currentIndexChanged(int)), this, SLOT(Slot_CmbPictureIndexChanged(int)));
	connect(ui.cmb_pic3, SIGNAL(currentIndexChanged(int)), this, SLOT(Slot_CmbPictureIndexChanged(int)));
	connect(ui.cmb_pic4, SIGNAL(currentIndexChanged(int)), this, SLOT(Slot_CmbPictureIndexChanged(int)));

	connect(ui.cmb_areaSum, SIGNAL(currentIndexChanged(int)), this, SLOT(Slot_CmbAreaSumIndexChanged(int)));
	connect(ui.cmb_workMode, SIGNAL(currentIndexChanged(int)), this, SLOT(Slot_CmbAreaSumIndexChanged(int)));

	connect(ui.btn_areaCopy, SIGNAL(clicked()), this, SLOT(Slot_BtnCopyClicked()));
	connect(ui.btn_laneCopy, SIGNAL(clicked()), this, SLOT(Slot_BtnCopyClicked()));

	m_mapLane[0] = ui.btn_lane1;
	m_mapLane[1] = ui.btn_lane2;
	m_mapLane[2] = ui.btn_lane3;
	m_mapLane[3] = ui.btn_lane4;

	m_mapChkLane[0] = ui.chk_road1;
	m_mapChkLane[1] = ui.chk_road2;
	m_mapChkLane[2] = ui.chk_road3;
	m_mapChkLane[3] = ui.chk_road4;

	m_mapChkArea[0] = ui.chk_area1;
	m_mapChkArea[1] = ui.chk_area2;
	m_mapChkArea[2] = ui.chk_area3;
	m_mapChkArea[3] = ui.chk_area4;

	m_mapArea[0] = ui.btn_area1;
	m_mapArea[1] = ui.btn_area2;
	m_mapArea[2] = ui.btn_area3;
	m_mapArea[3] = ui.btn_area4;

	memset(&m_tSeceneCfg, 0, sizeof(TIPC_GetSceneCfgRsp));
	memset(&m_tIPC_illgParkPara, 0, sizeof(m_tIPC_illgParkPara));

	connect(ui.btn_lane1, SIGNAL(clicked()), this, SLOT(Slot_LaneAndAreaClicked()));
	connect(ui.btn_lane2, SIGNAL(clicked()), this, SLOT(Slot_LaneAndAreaClicked()));
	connect(ui.btn_lane3, SIGNAL(clicked()), this, SLOT(Slot_LaneAndAreaClicked()));
	connect(ui.btn_lane4, SIGNAL(clicked()), this, SLOT(Slot_LaneAndAreaClicked()));

	connect(ui.btn_area1, SIGNAL(clicked()), this, SLOT(Slot_LaneAndAreaClicked()));
	connect(ui.btn_area2, SIGNAL(clicked()), this, SLOT(Slot_LaneAndAreaClicked()));
	connect(ui.btn_area3, SIGNAL(clicked()), this, SLOT(Slot_LaneAndAreaClicked()));
	connect(ui.btn_area4, SIGNAL(clicked()), this, SLOT(Slot_LaneAndAreaClicked()));

	connect(ui.btn_drawArea, SIGNAL(clicked()), this, SLOT(Slot_BtnDrawAreaClicked()));
	connect(ui.btn_snap, SIGNAL(clicked()), this, SLOT(Slot_BtnManualCaptureClicked()));
	m_chkBayonetAll.push_back(ui.chk_F1);
	m_chkBayonetAll.push_back(ui.chk_F2);
	m_chkBayonetAll.push_back(ui.chk_F3);
	m_chkBayonetAll.push_back(ui.chk_F4);
	m_chkBayonetAll.push_back(ui.chk_innerLed);
	m_chkBayonetAll.push_back(ui.chk_outerLED1);
	m_chkBayonetAll.push_back(ui.chk_outerLED2);

	m_chkBayonetAll.push_back(ui.chk_turnRight);
	m_chkBayonetAll.push_back(ui.chk_straight);
	m_chkBayonetAll.push_back(ui.chk_turnLeft);
	m_chkBayonetAll.push_back(ui.chk_waitTurn);
	
	

	m_chkBayonetAll.push_back(ui.chk_bayonet);
	m_chkBayonetAll.push_back(ui.chk_overSpeed);
	m_chkBayonetAll.push_back(ui.chk_nofollow);
	m_chkBayonetAll.push_back(ui.chk_laneChange);
	m_chkBayonetAll.push_back(ui.chk_runRedLight);
	m_chkBayonetAll.push_back(ui.chk_nofollowRoad);
	m_chkBayonetAll.push_back(ui.chk_forbidLeft);
	m_chkBayonetAll.push_back(ui.chk_wrongWay);
	m_chkBayonetAll.push_back(ui.chk_line);
	m_chkBayonetAll.push_back(ui.chk_illegalTurn);
	m_chkBayonetAll.push_back(ui.chk_overstop);

	m_chkBayonetAll.push_back(ui.chk_stay);
	m_chkBayonetAll.push_back(ui.chk_yellowStop);
	m_chkBayonetAll.push_back(ui.checkBox_76);
	m_chkBayonetAll.push_back(ui.chk_quickReturn);
	m_chkBayonetAll.push_back(ui.chk_round);
	m_chkBayonetAll.push_back(ui.chk_goRed);
	m_chkBayonetAll.push_back(ui.chk_safeDistance);
	m_chkBayonetAll.push_back(ui.chk_driverIllegal);
	m_chkBayonetAll.push_back(ui.chk_limitCar);
	m_chkBayonetAll.push_back(ui.chk_illegalSound);
	m_chkBayonetAll.push_back(ui.chk_high);

	m_chkBayonetAll.push_back(ui.chk_use);
	m_chkBayonetAll.push_back(ui.chk_hurryRoad);
	m_chkBayonetAll.push_back(ui.chk_busRoad);
	m_chkBayonetAll.push_back(ui.chk_largeCar);
	m_chkBayonetAll.push_back(ui.chk_forbidGoodsCar);
	m_chkBayonetAll.push_back(ui.chk_use_noCar);
	m_chkBayonetAll.push_back(ui.chk_manned);

	m_chkBayonetAll.push_back(ui.chk_add);
	m_chkBayonetAll.push_back(ui.chk_large2small);
	m_chkBayonetAll.push_back(ui.chk_road1);
	m_chkBayonetAll.push_back(ui.chk_road2);
	m_chkBayonetAll.push_back(ui.chk_road3);
	m_chkBayonetAll.push_back(ui.chk_road4);

	ui.btn_bayonet->setCheckable(true);
	ui.btn_illegalPark->setCheckable(true);
	ui.btn_illegalMode->setCheckable(true);
	//ui.btn_bayonet->setChecked(true);
	emit ui.btn_bayonet->setChecked(true);
	// 添加电警过程数据的接收通知窗口
	if (MgrData::getInstance()->GetMgrEvtLst())
	{
		MgrData::getInstance()->GetMgrEvtLst()->AddSender(static_cast<CObserver*>(this));
	}
	MgrData::getInstance()->GetMgrSubject()->Add(this);

	ui.lab_txt->setVisible(false);
	ui.lab_txt_2->setVisible(false);
	ui.lab_txt_3->setVisible(false);
}



Dlg_Scene::~Dlg_Scene()
{
	SAFE_DELETE(m_messageBox);
	ui.widget_3->ControlPlayVideo(false);
	MgrData::getInstance()->GetMgrSubject()->Del(this);
	MgrData::getInstance()->GetMgrEvtLst()->DelSender(this);
	
}

// 解析检测模式, 视频检测、线圈检测、雷达检测、视频检测+雷达测速
int	Dlg_Scene::AnalyCheckMode2Index(u32 dwMode)
{
	switch (dwMode)
	{
	case DETECT_MODE_VIDEO:
		return 0;
	case DETECT_MODE_COIL:
		return 1;
	case DETECT_MODE_RADAR:
		return 2;
	case DETECT_MODE_VIDEO_RADAR:
		return 3;;
	default:
		return 0;
	}
}

// 
u32 Dlg_Scene::AnalyIndex2CheckMode(int nIndex)
{
	switch (nIndex)
	{
	case 0:
		return DETECT_MODE_VIDEO;
	case 1:
		return DETECT_MODE_COIL;
	case 2:
		return DETECT_MODE_RADAR;
	case 3:
		return DETECT_MODE_VIDEO_RADAR;
	default:
		return 0;
	}
}

// 解析补光模式
int	Dlg_Scene::AnalyLightMode2Index(u32 dwMode)
{
	switch (dwMode)
	{
	case 0:
		return 0;
	case 1:
		return 1;
	case 2:
		return 2;
	case 3:
		return 3;
	default:
		return 0;
	}
}
// 解析补光模式
u32 Dlg_Scene::AnalyIndex2LightMode(int nIndex)
{
	switch (nIndex)
	{
	case 0:
		return 0;
	case 1:
		return 1;
	case 2:
		return 2;
	case 3:
		return 3;
	default:
		return 0;
	}
}

//setProperty 主要为了解决（addItem会解决选中事件）
void Dlg_Scene::Init()
{
	ui.cmb_F1->setView(new QListView());
	ui.cmb_F2->setView(new QListView());
	ui.cmb_F3->setView(new QListView());
	ui.cmb_F4->setView(new QListView());
	ui.cmb_lednei->setView(new QListView());
	ui.cmb_ledout1->setView(new QListView());
	ui.cmb_ledout2->setView(new QListView());

	ui.cmb_scene->setView(new QListView());
	QStringList sList;
	for (int i = 0; i < SCENCE_NUM; i++)
	{
		sList.append(QString("%1").arg(i + 1));
	}
	ui.cmb_scene->setProperty("id", 111);
	ui.cmb_scene->addItems(sList);
	ui.cmb_plateNum->setView(new QListView());
	QStringList sList2;
	for (int i = 0; i < 4; i++)
	{
		sList2.append(QString("%1").arg(i + 1));
	}
	ui.cmb_plateNum->setProperty("id", 111);
	ui.cmb_plateNum->addItems(sList2);

	QStringList sList9;
	for (int i = 0; i <= 4; i ++)
	{
		sList9.append(QString("%1").arg(i));
	}
	ui.cmb_areaSum->setView(new QListView());
	ui.cmb_areaSum->setProperty("id", 111);
	ui.cmb_areaSum->addItems(sList9);
	
	ui.cmb_plateDirection->setView(new QListView());
	ui.cmb_plateDirection->addItem(GBUTF8("东向西"));
	ui.cmb_plateDirection->addItem(GBUTF8("西向东"));
	ui.cmb_plateDirection->addItem(GBUTF8("南向北"));
	ui.cmb_plateDirection->addItem(GBUTF8("北向南"));
	ui.cmb_plateDirection->addItem(GBUTF8("东南向西北"));
	ui.cmb_plateDirection->addItem(GBUTF8("西北向东南"));
	ui.cmb_plateDirection->addItem(GBUTF8("东北向西南"));
	ui.cmb_plateDirection->addItem(GBUTF8("西南向东北"));
	ui.cmb_plateDirection->setCurrentIndex(0);

	ui.cmb_driveDirection->setView(new QListView());
	ui.cmb_driveDirection->addItem(GBUTF8("车头模式"));
	ui.cmb_driveDirection->addItem(GBUTF8("车尾模式"));
	//ui.cmb_driveDirection->addItem(GBUTF8("双向混行"));
	ui.cmb_driveDirection->setCurrentIndex(0);

	ui.cmb_testMode->setView(new QListView());
	ui.cmb_testMode->addItem(GBUTF8("视频检测"));
	ui.cmb_testMode->addItem(GBUTF8("线圈检测"));
	ui.cmb_testMode->addItem(GBUTF8("雷达检测"));
	ui.cmb_testMode->addItem(GBUTF8("视频检测+雷达检测"));
	ui.cmb_testMode->setCurrentIndex(0);

	ui.cmb_snapDay->setView(new QListView());
	ui.cmb_snapDay->addItem(GBUTF8("视频流截图"));
	ui.cmb_snapDay->addItem(GBUTF8("闪光灯"));
	ui.cmb_snapDay->addItem(GBUTF8("LED灯"));
	ui.cmb_snapDay->setCurrentIndex(0);

	ui.cmb_snapNight->setView(new QListView());
	ui.cmb_snapNight->addItem(GBUTF8("视频流截图"));
	ui.cmb_snapNight->addItem(GBUTF8("闪光灯"));
	ui.cmb_snapNight->addItem(GBUTF8("LED灯"));
	ui.cmb_snapNight->setCurrentIndex(0);

	QStringList sList3;
	sList3.append(GBUTF8("单次确认模式"));
	sList3.append(GBUTF8("二次确认模式"));
	sList3.append(GBUTF8("三次确认模式"));
	sList3.append(GBUTF8("场景空拍"));
	ui.cmb_workMode->setView(new QListView());
	ui.cmb_workMode->addItems(sList3);
	ui.cmb_workMode->setCurrentIndex(0);

	QStringList sList4;
	sList4.append(GBUTF8("全景"));
	sList4.append(GBUTF8("中景"));
	sList4.append(GBUTF8("近景"));
	sList4.append(GBUTF8("特写"));
	ui.cmb_pic1->setView(new QListView());
	ui.cmb_pic1->addItems(sList4);
	ui.cmb_pic1->setProperty("id", 111);
	ui.cmb_pic1->setCurrentIndex(0);
	ui.cmb_pic1->setProperty("id", 0);

	ui.cmb_pic2->setView(new QListView());
	ui.cmb_pic2->setProperty("id", 111);
	ui.cmb_pic2->addItems(sList4);
	ui.cmb_pic2->setCurrentIndex(2);
	ui.cmb_pic2->setProperty("id", 0);

	ui.cmb_pic3->setView(new QListView());
	ui.cmb_pic3->setProperty("id", 111);
	ui.cmb_pic3->addItems(sList4);
	ui.cmb_pic3->setCurrentIndex(-1);
	ui.cmb_pic3->setProperty("id", 0);

	ui.cmb_pic4->setView(new QListView());
	ui.cmb_pic4->setProperty("id", 111);
	ui.cmb_pic4->addItems(sList4);
	ui.cmb_pic4->setCurrentIndex(-1);
	ui.cmb_pic4->setProperty("id", 0);

	QStringList sList5;
	sList5.append(GBUTF8("低"));
	sList5.append(GBUTF8("高"));
	ui.cmb_tetLevel->setView(new QListView());
	ui.cmb_tetLevel->addItems(sList5);
	ui.cmb_tetLevel->setCurrentIndex(0);

	QStringList sList6;
	sList6.append(GBUTF8("自动模式"));
	sList6.append(GBUTF8("手动模式"));
	ui.cmb_illegalType->setView(new QListView());
	ui.cmb_illegalType->addItems(sList6);
	ui.cmb_illegalType->setCurrentIndex(0);

	QStringList sList7;
	sList7.append(GBUTF8("单任务模式"));
	sList7.append(GBUTF8("多任务模式"));
	ui.cmb_autoType->setView(new QListView());
	ui.cmb_autoType->addItems(sList7);
	ui.cmb_autoType->setCurrentIndex(0);

	QStringList sList8;
	sList8.append(GBUTF8("通用模式"));
	sList8.append(GBUTF8("苏州违停"));
	sList8.append(GBUTF8("安康违停"));
	sList8.append(GBUTF8("东丽模式"));
	sList8.append(GBUTF8("沈阳模式"));
	ui.cmb_detailType->setView(new QListView());
	ui.cmb_detailType->addItems(sList8);
	ui.cmb_detailType->setCurrentIndex(0);
}

QPushButton* Dlg_Scene::GetClickedButton()
{
	if (m_nShowType == 0 || m_nShowType == -1)
	{
		switch (m_nIndex)
		{
		case 0:
		{
			return ui.btn_lane1;
			break;
		}
		case 1:
		{
			return ui.btn_lane2;
			break;
		}
		case 2:
		{
			return ui.btn_lane3;
			break;
		}
		case 3:
		{
			return ui.btn_lane4;
			break;
		}
		}
	
	}
	else if (m_nShowType == 1)
	{
		switch (m_nIndex)
		{
		case 0:
		{
			return ui.btn_area1;
			break;
		}
		case 1:
		{
			return ui.btn_area2;
			break;
		}
		case 2:
		{
			return ui.btn_area3;
			break;
		}
		case 3:
		{
			return ui.btn_area4;
			break;
		}
		}
	}
	
}

void Dlg_Scene::OnSwitchIndex(int index, bool isSave)
{
	OnUpdateData(isSave);
	m_nIndex = index;
	QPushButton *pBtn = GetClickedButton();
	if (pBtn)
	{
		pBtn->setChecked(true);
	}
	
	switch (m_nShowType)
	{
	case 0:
		OnChangeLaneNum();
		break;
	case 1:
		OnChangeAreaNum();
		break;
	default:
		break;
	}
	OnUpdateData(false);
}

void Dlg_Scene::OnChangeAreaNum()
{
	/*ui.cmb_areaSum->setProperty("id", 111);
	ui.cmb_areaSum->clear();
	int nArea = m_tIPC_illgParkPara.RoiAreaNum;
	QStringList sListt;
	for (int i = 0; i <= nArea; i++)
	{
		sListt.append(QString("%1").arg(i));
		
	}
	ui.cmb_areaSum->setProperty("id", 111);
	ui.cmb_areaSum->addItems(sListt);
	*/
	int nArea = m_tIPC_illgParkPara.RoiAreaNum;
	for (int i = 0; i < 4; i++)
	{
		if (i < nArea)
		{
			m_mapArea[i]->setVisible(true);
			if (i == 0)
			{
				//m_mapArea[i]->setChecked(true);
			}

			if (i == m_nIndex)
			{
				m_mapChkArea[i]->setVisible(false);
			}
			else
			{
				m_mapChkArea[i]->setVisible(true);
			}
		}
		else
		{
			m_mapArea[i]->setVisible(false);
			m_mapChkArea[i]->setVisible(false);
		}
		/*if (i > nArea - 1)
		{
		m_mapArea[i]->setVisible(false);
		}
		else
		{
		m_mapArea[i]->setVisible(true);
		}*/
		m_mapLane[i]->setVisible(false);
	}
	if (nArea == 0)
	{
		ui.widget_7->setVisible(false);
		ui.stackedWidget_2->setCurrentIndex(2);
	}
	else
	{
		ui.widget_7->setVisible(true);
		ui.stackedWidget_2->setCurrentIndex(1);
	}

}

void Dlg_Scene::OnChangeLaneNum()
{
	/*ui.cmb_plateNum->clear();
	int nLane = m_tSeceneCfg.tSceneCfg.dwLaneNum;
	QStringList sList;
	for (int i = 0; i < nLane; i++)
	{
	sList.append(QString("%1").arg(i + 1));
	}
	ui.cmb_plateNum->setProperty("id", 111);
	ui.cmb_plateNum->addItems(sList);*/
	int nLane = m_tSeceneCfg.tSceneCfg.dwLaneNum;
	for (int i = 0; i < 4; i++)
	{
		if (i < nLane)
		{
			m_mapLane[i]->setVisible(true);
			/*if (i == 0)
			{
				m_mapLane[i]->setChecked(true);
			}*/

			if (i == m_nIndex)
			{
				m_mapChkLane[i]->setVisible(false);
			}
			else
			{
				m_mapChkLane[i]->setVisible(true);
			}
		}
		else
		{
			m_mapLane[i]->setVisible(false);
			m_mapChkLane[i]->setVisible(false);
		}

		m_mapArea[i]->setVisible(false);
		/*if (i > nLane-1)
		{
			m_mapLane[i]->setVisible(false);
		}
		else
		{
			m_mapLane[i]->setVisible(true);
		}*/
	}

	/*for (int i = 0; i < 4; i++)
	{
		m_mapArea[i]->setVisible(false);
	}*/
}


bool Dlg_Scene::GetParkParam()
{
	u32 dwIP = MgrData::getInstance()->GetIP();
	MgrItsCtrl* pMgrItsCtrl = MgrData::getInstance()->GetMgrItsCtrl();
	if (pMgrItsCtrl != NULL)
	{
		ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dwIP);
		if (pCtrl != NULL)
		{
			bool bRet = pCtrl->GetIpcParkParam(NULL, (u32)(CObserver*)this);
			if (!bRet)
			{
			}
		}
	}
	return true;
}

void Dlg_Scene::GetScenceID()
{
	u32 dwIP = MgrData::getInstance()->GetIP();
	ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dwIP);
	if (pCtrl != NULL)
	{
		bool bRet = pCtrl->GetScenceID(NULL, (u32)(CObserver*)this);
		if (!bRet)
		{
		}
	}
}

bool Dlg_Scene::GetParamScence()
{
	u32 dwIP = MgrData::getInstance()->GetIP();
	ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dwIP);
	if (pCtrl != NULL)
	{
		unsigned int sceneIndex = m_nSceneIndex + 1;
		bool bRet = pCtrl->GetIpcScence(&sceneIndex, (u32)(CObserver*)this);

		if (!bRet)
		{
		}

		return bRet;
	}

	return false;
}

void Dlg_Scene::GetParamDevInfo()
{
	MgrItsCtrl* pMgrItsCtrl = MgrData::getInstance()->GetMgrItsCtrl();
	if (pMgrItsCtrl != NULL)
	{
		u32 dIP = MgrData::getInstance()->GetIP();
		ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dIP);
		if (pCtrl != NULL)
		{
			bool bRet = pCtrl->GetIpcDevInfo(NULL, (u32)(CObserver*)this);
			if (!bRet)
			{
			}
		}
	}
}

void Dlg_Scene::OnSaveExtType()
{
	m_extDevType.flashType[0] = ui.cmb_F1->currentIndex();
	m_extDevType.flashType[1] = ui.cmb_F2->currentIndex();
	m_extDevType.flashType[2] = ui.cmb_F3->currentIndex();
	m_extDevType.flashType[3] = ui.cmb_F4->currentIndex();

	m_extDevType.ledType[0] = ui.cmb_lednei->currentIndex();
	m_extDevType.ledType[1] = ui.cmb_ledout1->currentIndex();
	m_extDevType.ledType[2] = ui.cmb_ledout2->currentIndex();
	SetExtDevType();

}

void Dlg_Scene::SetExtDevVisible(bool isShow)
{
	ui.label_50->setVisible(isShow);
	ui.label_51->setVisible(isShow);
	ui.cmb_F1->setVisible(isShow);
	ui.cmb_F2->setVisible(isShow);
	ui.cmb_F3->setVisible(isShow);
	ui.cmb_F4->setVisible(isShow);
	ui.cmb_lednei->setVisible(isShow);
	ui.cmb_ledout1->setVisible(isShow);
	ui.cmb_ledout2->setVisible(isShow);
}

void Dlg_Scene::InitData(bool is)
{
	// 获取本地软件配置参数
	if (!is)
	{
		if (tLocalCfgInfo.ispic == 0)
		{
			ui.widget_3->ControlPlayVideo(is);
		}
		return;
	}

	MgrData::getInstance()->GetMgrLocalCfg()->GetLocalCfg(&tLocalCfgInfo);
	TIPC_GetUICapsRsp *m_pUICap = MgrData::getInstance()->GetDevCap();

	if (m_pUICap->szCaps[EMUI_CAP_TYPE_ILLGPARK] == 0)
	{
		m_bSupportPark = false;
		ui.btn_selectPos->setVisible(false);
		ui.btn_illegalPark->setVisible(false);
		ui.btn_illegalMode->setVisible(false);
		if (MgrData::getInstance()->GetDevVersion() > 2)
		{
			SetExtDevVisible(true);
			GetExtDevType();
		}
		else
		{
			SetExtDevVisible(false);
		}
		ui.btn_snap->setVisible(false);
	}
	else
	{
		m_bSupportPark = true;
		ui.btn_illegalPark->setVisible(true);
		ui.btn_illegalMode->setVisible(true);
		ui.btn_selectPos->setVisible(true);
		ui.btn_snap->setVisible(true);
	}
	if (is)
	{
		TIPC_GetUICapsRsp *m_pUICap = MgrData::getInstance()->GetDevCap();
		GetParamDevInfo();
	}

	if (m_bSupportPark)
	{
		m_bSupport = true;
		GetParkScence();//违停场景
		GetParkParam();
		GetScenceID();

	}
	else
	{
		m_bSupport = false;
		GetParamScence();
	}

	// 显示视频
	if (tLocalCfgInfo.ispic == 0)
	{
		ui.widget_3->ControlPlayVideo(is);
	}
	else
	{
		// 显示图片
		OnSnap();
	}

}

void Dlg_Scene::SetExtDevType()
{
	MgrItsCtrl* pMgrItsCtrl = MgrData::getInstance()->GetMgrItsCtrl();
	if (pMgrItsCtrl != NULL)
	{
		u32 dwIP = MgrData::getInstance()->GetIP();
		ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dwIP);
		if (pCtrl != NULL)
		{
			pCtrl->SetExtDevType(&m_extDevType);
		}
	}
}

bool Dlg_Scene::GetExtDevType()
{
	MgrItsCtrl* pMgrItsCtrl = MgrData::getInstance()->GetMgrItsCtrl();
	if (pMgrItsCtrl != NULL)
	{
		u32 dwIP = MgrData::getInstance()->GetIP();
		ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dwIP);
		if (pCtrl != NULL)
		{
			bool bRet = pCtrl->GetExtDevType((u32)(CObserver*)this);
			if (!bRet)
			{
			}
		}
	}
	return true;
}


void Dlg_Scene::OnSnap()
{
	u32 dwIP = MgrData::getInstance()->GetIP();
	ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dwIP);
	if (pCtrl != NULL)
	{
		pCtrl->IpcEvt_Snap();
	}
}

bool Dlg_Scene::GetParkScence()
{
	MgrItsCtrl* pMgrItsCtrl = MgrData::getInstance()->GetMgrItsCtrl();
	if (pMgrItsCtrl != NULL)
	{
		u32 dwIP = MgrData::getInstance()->GetIP();
		ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dwIP);
		if (pCtrl != NULL)
		{
			unsigned int sceneIndex = m_nSceneIndex + 1;
			bool bRet = pCtrl->GetIpcParkScence(&sceneIndex, (u32)(CObserver*)this);
			if (!bRet)
			{
			}
		}
	}
	return true;
}


void Dlg_Scene::OnObserverNotify(LPARAM lHint, LPVOID pHint)
{
	u32 dwEvtType = (u32)lHint;
	u32 dwId = (u32)pHint;


	switch (dwEvtType)
	{
	case NMC_EVT_DLG_CLOSE:
	{
		// 场景绘制窗口关闭;
		//if (pHint == m_pDlgIpc_ScenceDraw || pHint == m_pDlgIpc_ParkScenceDraw)
		//{
		//OnSaveDrawScence();
		//}
	}
		break;
	case NMC_EVT_LOGOUT:
	{
		//CloseChildDlg();
	}
		break;
	case NMC_GET_IPC_SCENCEINFO:
	{
		TIPC_GetSceneCfgRsp* p = (TIPC_GetSceneCfgRsp*)pHint;
		if (p->nRetCode == 0)
		{
			memcpy(&m_tSeceneCfg, p, sizeof(TIPC_GetSceneCfgRsp));
			emit signal_bayonetChanged();
		}
	}
		break;
	case NMC_GET_IPC_PARKSCENCE:
	{
		TIPC_illgParkPara* p = (TIPC_illgParkPara*)pHint;
		memcpy(&m_tIPC_illgParkPara, p, sizeof(TIPC_illgParkPara));
		emit signal_parkScenceChanged();
	}
		break;
	case NMC_GET_IPC_DEVINFO:
	{
		TIPC_GetDevRsp* p = (TIPC_GetDevRsp*)pHint;
		if (p->nRetCode == 0)
		{
			memcpy(&m_tDevInfo, &(p->tDev), sizeof(TIPC_DevInfo));
			emit signal_devInfoChanged();
		}
	}
		break;
	case NMC_EVT_VIDEO_PIC:
	{
		//BOOL bHandled;
		//OnRefreshPic(0, (WPARAM)0, 0, bHandled);
	}
		break;
	case NMC_GET_IPC_SCENCE_ID:
	{
		u32 scenceId = *(u32*)pHint;
		m_nSceneIndex = scenceId;
		ui.cmb_scene->setCurrentIndex(m_nSceneIndex);
		OnSwitchScene();
	}
		break;
	case NMC_GET_IPC_PARKPARAM:
	{
		TIPC_ParkParam* p = (TIPC_ParkParam*)pHint;
		memcpy(&m_tParkParam, p, sizeof(TIPC_ParkParam));
		emit signal_parkParamChanged();
	}
		break;
	case NMC_EVT_EXT_TYPE:
	{
		TIvhsExtDevType *p = (TIvhsExtDevType*)pHint;
		memcpy(&m_extDevType, p, sizeof(TIvhsExtDevType));
		emit signal_extDevTypeChanged();
	}
		break;
	default:
		break;
	}
}

void Dlg_Scene::SetParkParam()
{
	u32 dwIP = MgrData::getInstance()->GetIP();
	ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dwIP);
	if (pCtrl == NULL)
	{
		return;
	}

	UpdateParkParam(true);
	pCtrl->SetIpcParkParam(&m_tParkParam);
}

void Dlg_Scene::Slot_DeviceInfoChanged()
{
	UpdateDevInfo(false);
}

void Dlg_Scene::OnChangePreset(int nPreset)
{
	MgrItsCtrl* pMgrItsCtrl = MgrData::getInstance()->GetMgrItsCtrl();
	if (pMgrItsCtrl != NULL)
	{
		u32 dwIP = MgrData::getInstance()->GetIP();
		ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dwIP);
		if (pCtrl != NULL)
		{
			TIPC_PTZCtrl tData;
			tData.wEventType = PTZ_EVENT_TYPE::enum_ptz_event_type_preset;
			tData.wP1 = (Uint16)nPreset;
			tData.wP4 = (Uint16)m_nSceneIndex;
			bool bRet = pCtrl->IpcPtz(&tData);
		}
	}
}

void Dlg_Scene::Slot_ParkScenceChanged()
{
	// 切换预制位
	int nPreset = m_tIPC_illgParkPara.PrePosLabel;
	OnChangePreset(nPreset);

	if (m_bSupport)
	{
		ui.btn_illegalPark->setChecked(true);
		emit ui.btn_illegalPark->clicked(true);
		return ;
	}
	if (m_nShowType == 1)
	{
		OnShowPark(true);
	}
}

void Dlg_Scene::OnShowPark(bool bShow)
{
	ui.stackedWidget->setCurrentIndex(1);
	ui.stackedWidget_2->setCurrentIndex(1);
	if (bShow)
	{
		QPushButton *pBtn = GetClickedButton();
		if (pBtn)
		{
			pBtn->setChecked(true);
		}
		ui.btn_drawArea->setVisible(true);
		ui.btn_snap->setVisible(true);
		UpdatePark(false);
	}
	else
	{
		UpdatePark(true);
		//ui.btn_drawArea->setVisible(false);
		ui.btn_snap->setVisible(false);
		ui.widget_3->ClearDrawData();
	}
}


void Dlg_Scene::Slot_PlateNumChanged(int nIndex)
{
	if ( nIndex == -1)
	{
		return;
	}
	m_tSeceneCfg.tSceneCfg.dwLaneNum = nIndex + 1;
	OnChangeLaneNum();
	OnSwitchIndex(0);
}

void Dlg_Scene::Slot_ParkParamChanged()
{
	if (m_nShowType == 2)
	{
		OnShowParkMode(true);
	}
}

void Dlg_Scene::Slot_ExtDevTypeChanged()
{
	ui.cmb_F1->setCurrentIndex(m_extDevType.flashType[0]);
	ui.cmb_F2->setCurrentIndex(m_extDevType.flashType[1]);
	ui.cmb_F3->setCurrentIndex(m_extDevType.flashType[2]);
	ui.cmb_F4->setCurrentIndex(m_extDevType.flashType[3]);

	ui.cmb_lednei->setCurrentIndex(m_extDevType.ledType[0]);
	ui.cmb_ledout1->setCurrentIndex(m_extDevType.ledType[1]);
	ui.cmb_ledout2->setCurrentIndex(m_extDevType.ledType[2]);

	if (MgrData::getInstance()->GetDevVersion() > 2)
	{
		SetExtDevVisible(true);
	}
	else
	{
		SetExtDevVisible(false);
	}
}

void Dlg_Scene::Slot_CmbSceneIndexChanged(int nIndex)
{
	if (ui.cmb_scene->property("id").toInt() == 111)
	{
		ui.cmb_scene->setProperty("id", 0);
		return;
	}
	if ((nIndex != m_nSceneIndex) && (nIndex != -1))
	{
		m_nSceneIndex = ui.cmb_scene->currentIndex();
		OnSwitchScene();
	}
}

void Dlg_Scene::OnSwitchScene()
{
	GetParamScence();
	if (m_bSupportPark)
	{
		GetParkScence();
	}
	UpdateDevInfo(false);
}

void Dlg_Scene::UpdateDevInfo(bool bUpdate)
{
	if (bUpdate)
	{
		QString sceneName = ui.ledt_pos->text();
		wchar2char(m_tDevInfo.szSceneName[m_nSceneIndex], sceneName.toStdWString().c_str());
		m_tDevInfo.szSceneName[m_nSceneIndex][127] = 0;
	}
	else
	{
		wchar_t ws[129];
		ZeroMemory(ws, 129);
		m_tDevInfo.szSceneName[m_nSceneIndex][127] = 0;
		char2wchar(ws, m_tDevInfo.szSceneName[m_nSceneIndex]);
		QString strTemp = QString::fromWCharArray(ws);
		if (strTemp.isEmpty())
		{
			ZeroMemory(ws, 129);
			char2wchar(ws, m_tDevInfo.szDevName);
		}
		ui.ledt_pos->setText(QString::fromWCharArray(ws));
	}
}

void Dlg_Scene::Slot_BtnManualCaptureClicked()
{
	Dlg_Scene_ManualCapture dlg(nullptr);
	dlg.setWindowTitle(GBUTF8("手动抓拍"));
	dlg.exec();
}

void Dlg_Scene::Slot_BtnDrawAreaClicked()
{
	if (m_nShowType == 0)
	{
		Dlg_SceneDraw dlg;
		dlg.setWindowTitle(GBUTF8("场景配置"));
		dlg.SetIsParkScene(false);
		// 设置参数
		//dlg.GetCtrl_ScenceDraw()->SetNewPic(0);
		dlg.SetData(m_tSeceneCfg.tSceneCfg, m_tSeceneCfg.tMaxParam.dwMaxWidth, m_tSeceneCfg.tMaxParam.dwMaxHeight);
		if (dlg.exec() == 1)
		{
			dlg.GetData(m_tSeceneCfg.tSceneCfg);
			if (tLocalCfgInfo.ispic == 0)
			{
				ui.widget_3->SetScenceDrawData(&m_tSeceneCfg);
			}
			else
			{
				//SetScenceDrawData();
			}
		}
	}
	else if (m_nShowType == 1)
	{
		Dlg_SceneDraw dlg;
		dlg.setWindowTitle(GBUTF8("场景配置"));
		dlg.SetScenceIndex(m_nSceneIndex);
		dlg.SetIsParkScene(true);
		// 设置参数
		dlg.SetData(m_tIPC_illgParkPara, m_tSeceneCfg.tMaxParam.dwMaxWidth, m_tSeceneCfg.tMaxParam.dwMaxHeight);
		if (dlg.exec() == 1)
		{
			dlg.GetData(m_tIPC_illgParkPara);
			if (tLocalCfgInfo.ispic == 0)
			{
				ui.widget_3->SetParkScenceDrawData(&m_tIPC_illgParkPara);
			}
			else
			{
				SetParkScenceDrawData(&m_tIPC_illgParkPara);
			}
		}

	}
}

void Dlg_Scene::Slot_LaneAndAreaClicked()
{
	QPushButton *pBtn = dynamic_cast<QPushButton*>(this->sender());
	if (pBtn == ui.btn_lane1)
	{
		OnSwitchIndex(0, true);
	}
	else if (pBtn == ui.btn_lane2)
	{
		OnSwitchIndex(1, true);
	}
	else if (pBtn == ui.btn_lane3)
	{
		OnSwitchIndex(2, true);
	}
	else if (pBtn == ui.btn_lane4)
	{
		OnSwitchIndex(3);
	}
	else if (pBtn == ui.btn_area1)
	{
		OnSwitchIndex(0, true);
	}
	else if (pBtn == ui.btn_area2)
	{
		OnSwitchIndex(1, true);
	}
	else if (pBtn == ui.btn_area3)
	{
		OnSwitchIndex(2, true);
	}
	else if (pBtn == ui.btn_area4)
	{
		OnSwitchIndex(3, true);
	}
}

void Dlg_Scene::OnShowBayonet(bool bShow)
{
	ui.stackedWidget->setCurrentIndex(0);
	ui.stackedWidget_2->setCurrentIndex(0);
	if (bShow)
	{
		OnChangeLaneNum();
		/*QPushButton *pBtn = GetClickedButton();
		if (pBtn)
		{
			pBtn->setChecked(true);
		}*/
		UpdateScene(false);
	}
	else
	{
		UpdateScene(true);
		//m_pBtnDraw->SetVisible(false);

		ui.widget_3->ClearDrawData();

	}
}

void Dlg_Scene::OnShowParkMode(bool bShow)
{
	if (bShow)
	{
		ui.stackedWidget->setCurrentIndex(2);
		ui.stackedWidget_2->setCurrentIndex(2);
		ui.widget_6->setVisible(false);
		UpdateParkParam(false);
	}
	else
	{
		ui.widget_6->setVisible(true);
	}
	
}


void Dlg_Scene::OnShowPtz(bool bShow)
{
	if (bShow)
	{
		ui.stackedWidget->setCurrentIndex(3);
		ui.stackedWidget_2->setCurrentIndex(2);
		ui.widget_6->setVisible(false);
	}
	else
	{
		ui.stackedWidget_2->setCurrentIndex(m_nShowType);
		ui.widget_6->setVisible(true);
	}
}


void Dlg_Scene::OnSwitchPage(int index, bool isSave)
{
	if (isSave)
	{
		switch (m_nShowType)
		{
		case 0:
			OnShowBayonet(false);
			break;
		case 1:
			OnShowPark(false);
			break;
		case 2:
			OnShowParkMode(false);
			break;
		case 3:
			OnShowPtz(false);
			break;
		default:
			break;
		}

	}

	m_nShowType = index;

	switch (index)
	{
	case 0:
		OnShowBayonet(true);
		break;
	case 1:
		OnShowPark(true);
		break;
	case 2:
		OnShowParkMode(true);
		break;
	case 3:
		OnShowPtz(true);
		break;
	default:
		break;
	}
}

bool Dlg_Scene::CheckCarProperty()
{
	if (m_nShowType != 0)
	{
		return false;
	}
	int nCount = ui.cmb_areaSum->currentIndex() + 1;
	for (int i = 0; i < nCount; i++)
	{
		int nState1 = GetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)&m_tSeceneCfg)->tSceneCfg.tPhyLane[i].dwSnapType, 8);//闯红灯
		int nState2 = GetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)&m_tSeceneCfg)->tSceneCfg.tPhyLane[i].dwSnapType, 9);//不按导向车道行驶
		int nState3 = GetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)&m_tSeceneCfg)->tSceneCfg.tPhyLane[i].dwSnapType, 23);//行人闯红灯

		if ((nState1 == 1) || (nState2 == 1) || (nState3 == 1))
		{
			int nStateTurn = GetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)&m_tSeceneCfg)->tSceneCfg.tPhyLane[i].dwLaneAttribute, 3);
			int nStateLeft = GetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)&m_tSeceneCfg)->tSceneCfg.tPhyLane[i].dwLaneAttribute, 2);
			int nStateRight = GetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)&m_tSeceneCfg)->tSceneCfg.tPhyLane[i].dwLaneAttribute, 0);
			int nStateLine = GetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)&m_tSeceneCfg)->tSceneCfg.tPhyLane[i].dwLaneAttribute, 1);
			if ((nStateTurn != 1) && (nStateRight != 1) && (nStateLine != 1) && (nStateLeft != 1))
			{
				return true;
			}

		}
	}

	return false;
}

void Dlg_Scene::SetParamScence()
{
	MgrItsCtrl* pMgrItsCtrl = MgrData::getInstance()->GetMgrItsCtrl();
	if (pMgrItsCtrl != NULL)
	{
		TIPC_SetSceneCfgReq* pData = new TIPC_SetSceneCfgReq;
		pData->dwSceneId = m_tSeceneCfg.dwSceneId;
		memcpy(&(pData->tSceneCfg), &(m_tSeceneCfg.tSceneCfg), sizeof(m_tSeceneCfg.tSceneCfg));
		u32 dwIP = MgrData::getInstance()->GetIP();
		ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dwIP);
		if (pCtrl != NULL)
		{
			bool bRet = pCtrl->SetIpcScence(pData);

			if (!bRet)
			{
			}
		}

		delete pData;
	}
}


void Dlg_Scene::SetParkScence()
{
	u32 dwIP = MgrData::getInstance()->GetIP();
	ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dwIP);
	if (pCtrl == NULL)
	{
		return;
	}

	m_tIPC_illgParkPara.parkId = m_nSceneIndex + 1;
	pCtrl->SetIpcParkScence(&m_tIPC_illgParkPara);
}

void Dlg_Scene::OnSave()
{
	bool bSuccess = true;
	switch (m_nShowType)
	{
	case 0:
	case 1:
	{
		OnUpdateData(true);
		if (CheckCarProperty())
		{
			QMessageBox msgBox;
			msgBox.setText(GBUTF8("   请先选择车道属性！  "));
			msgBox.exec();
			return;
		}

		SetParamScence();
		if (m_bSupportPark)
		{
			SetParkScence();
		}
		OnSaveExtType();
		UpdateDevInfo(true);
		//OnSetOsdPic();
	}
		break;
	case 2:
	{
		SetParkParam();
	}
		break;
	case 3:
		OnSavePtzPos();
		break;
	}

	if (bSuccess)
	{
		
	}
}

void Dlg_Scene::OnChangePicType(int type)
{
	int i = 0;
	int j = 0;
	switch (type)
	{
	case 1:
	{
		i = ui.cmb_pic1->currentIndex();
		j = ui.cmb_pic2->currentIndex();
		if (i < 2 && j < 2)
		{
			ui.cmb_pic2->setCurrentIndex(2);
		}
	}
		break;
	case 2:
	{
		i = ui.cmb_pic1->currentIndex();
		j = ui.cmb_pic2->currentIndex();
		if (i < 2 && j < 2)
		{
			ui.cmb_pic1->setCurrentIndex(2);
		}
	}
		break;
	case 3:
	{
		i = ui.cmb_pic3->currentIndex();
		j = ui.cmb_pic4->currentIndex();
		if (i < 2 && j < 2)
		{
			ui.cmb_pic4->setCurrentIndex(2);
		}
	}
		break;
	case 4:
	{
		i = ui.cmb_pic3->currentIndex();
		j = ui.cmb_pic4->currentIndex();
		if (i < 2 && j < 2)
		{
			ui.cmb_pic3->setCurrentIndex(2);
		}
	}
		break;
	default:
		break;
	}
}



void Dlg_Scene::OnSavePtzPos()
{
	char szIndex[32] = { 0 };
	char szNum[32] = { 0 };
	int pos = m_tIPC_illgParkPara.PrePosLabel - 1;
	sprintf(szIndex, "preset-%d", m_tIPC_illgParkPara.PrePosLabel);
	MgrData::getInstance()->GetMgrIpcSdk()->savePrepos(pos, szIndex);

	ui.btn_bayonet->setChecked(true);
	emit ui.btn_bayonet->clicked();
	/*OnChangeLaneNum();
	m_nShowType = 0;
	OnSwitchPage(0);*/
}

void Dlg_Scene::Slot_BtnSelectPosClicked()
{
	OnSwitchPage(3);
}

void Dlg_Scene::Slot_BtnCopyClicked()
{
	OnUpdateData(true);
	if (m_nShowType == 0)
	{
		TIPC_PhysicLane *pCurData = &(m_tSeceneCfg.tSceneCfg.tPhyLane[m_nIndex]);
		for (int i = 0; i < 4; i++)
		{	
			if (m_mapChkLane[i]->isChecked() && m_mapChkLane[i]->isVisible() && i != m_nIndex)
			{
				// 拷贝
				TIPC_PhysicLane *pNeedCpyData = &(m_tSeceneCfg.tSceneCfg.tPhyLane[i]);
				if (pNeedCpyData != NULL && pCurData != NULL)
				{
					memcpy(pNeedCpyData, pCurData, sizeof(TIPC_PhysicLane));
				}
				pNeedCpyData->dwLaneLabel = i + 1;
			}
		}
	}
	else
	{
		for (int i = 0; i < 4; i++)
		{
			
			if (m_mapChkArea[i]->isChecked() && m_mapChkArea[i]->isVisible() && i != m_nIndex)
			{
				m_tIPC_illgParkPara.tRoiArea[i].enable = m_tIPC_illgParkPara.tRoiArea[m_nIndex].enable;
				m_tIPC_illgParkPara.tRoiArea[i].minVehWidth = m_tIPC_illgParkPara.tRoiArea[m_nIndex].minVehWidth;
				m_tIPC_illgParkPara.tRoiArea[i].maxVehWidth = m_tIPC_illgParkPara.tRoiArea[m_nIndex].maxVehWidth;
				m_tIPC_illgParkPara.tRoiArea[i].level = m_tIPC_illgParkPara.tRoiArea[m_nIndex].level;
				m_tIPC_illgParkPara.tRoiArea[i].halfScanfVal = m_tIPC_illgParkPara.tRoiArea[m_nIndex].halfScanfVal;
				m_tIPC_illgParkPara.tRoiArea[i].scanfstep = m_tIPC_illgParkPara.tRoiArea[m_nIndex].scanfstep;
				m_tIPC_illgParkPara.tRoiArea[i].trimVal = m_tIPC_illgParkPara.tRoiArea[m_nIndex].trimVal;
				m_tIPC_illgParkPara.tRoiArea[i].filterOn = m_tIPC_illgParkPara.tRoiArea[m_nIndex].filterOn;
				m_tIPC_illgParkPara.tRoiArea[i].filterOff = m_tIPC_illgParkPara.tRoiArea[m_nIndex].filterOff;
			}
		}
	}
}

void Dlg_Scene::Slot_CmbAreaSumIndexChanged(int nIndex)
{
	/*if (this->sender()->property("id") == 111)
	{
		this->sender()->setProperty("id", 0);
		return;
	}*/
	if (this->sender() == ui.cmb_areaSum)
	{
		m_tIPC_illgParkPara.RoiAreaNum = nIndex;
		OnChangeAreaNum();
		OnSwitchIndex(0);
	}
	else if (this->sender() == ui.cmb_workMode)
	{
		if (nIndex == 2)
		{
			ui.ledt_notice->setEnabled(true);
		}
		else
		{
			ui.ledt_notice->setEnabled(false);
		}
	}
	
}

void Dlg_Scene::Slot_CmbPictureIndexChanged(int nIndex)
{
	QComboBox *pCmb = dynamic_cast<QComboBox*>(this->sender());
	if (pCmb->property("id").toInt() == 111)
	{
		pCmb->setProperty("id", 0);
		return;
	}
	if (pCmb == ui.cmb_pic1)
	{
		OnChangePicType(1);
	}
	else if (pCmb == ui.cmb_pic2)
	{
		OnChangePicType(2);
	}
	else if (pCmb == ui.cmb_pic3)
	{
		OnChangePicType(3);
	}
	else if (pCmb == ui.cmb_pic4)
	{
		OnChangePicType(4);
	}
}

void Dlg_Scene::Slot_PTZStopClicked()
{
	char stop[32] = "zoomstop";
	if (this->sender() == ui.btn_zoomIn || this->sender() == ui.btn_zoomOut)
	{
		MgrData::getInstance()->GetMgrIpcSdk()->ptzMove(stop, 100);
	}
	else
	{
		MgrData::getInstance()->GetMgrIpcSdk()->stopMove();
	}
}

void Dlg_Scene::Slot_PTZControlClicked()
{
	char direction[32] = { 0 };
	int speed = 0;
	if (this->sender() == ui.btn_leftup)
	{
		strcpy(direction, "move_leftup");
		speed = ui.horizontalSlider->value();
		MgrData::getInstance()->GetMgrIpcSdk()->ptzMove(direction, speed);
	}

	if (this->sender() == ui.btn_up)
	{
		strcpy(direction, "moveup");
		speed = ui.horizontalSlider->value();
		MgrData::getInstance()->GetMgrIpcSdk()->ptzMove(direction, speed);
	}

	if (this->sender() == ui.btn_rightup)
	{
		strcpy(direction, "move_rightup");
		speed = ui.horizontalSlider->value();
		MgrData::getInstance()->GetMgrIpcSdk()->ptzMove(direction, speed);
	}

	if (this->sender() == ui.btn_left)
	{
		strcpy(direction, "moveleft");
		speed = ui.horizontalSlider->value();
		MgrData::getInstance()->GetMgrIpcSdk()->ptzMove(direction, speed);
	}

	if (this->sender() == ui.btn_right)
	{
		strcpy(direction, "moveright");
		speed = ui.horizontalSlider->value();
		MgrData::getInstance()->GetMgrIpcSdk()->ptzMove(direction, speed);
	}

	if (this->sender() == ui.btn_leftdown)
	{
		strcpy(direction, "move_leftdown");
		speed = ui.horizontalSlider->value();
		MgrData::getInstance()->GetMgrIpcSdk()->ptzMove(direction, speed);
	}

	if (this->sender() == ui.btn_down)
	{
		strcpy(direction, "movedown");
		speed = ui.horizontalSlider->value();
		MgrData::getInstance()->GetMgrIpcSdk()->ptzMove(direction, speed);
	}

	if (this->sender() == ui.btn_rightdown)
	{
		strcpy(direction, "move_rightdown");
		speed = ui.horizontalSlider->value();
		MgrData::getInstance()->GetMgrIpcSdk()->ptzMove(direction, speed);
	}

	if (this->sender() == ui.btn_zoomIn)
	{
		strcpy(direction, "zoomtele");
		speed = ui.horizontalSlider->value();
		MgrData::getInstance()->GetMgrIpcSdk()->ptzMove(direction, speed);
	}

	if (this->sender() == ui.btn_zoomOut)
	{
		strcpy(direction, "zoomwide");
		speed = ui.horizontalSlider->value();
		MgrData::getInstance()->GetMgrIpcSdk()->ptzMove(direction, speed);
	}
	
}

void Dlg_Scene::Slot_BtnFreshClicked()
{
	GetParamDevInfo();

	if (m_nShowType == 0)
	{
		GetParamScence();
	}
	else if (m_nShowType == 1)
	{
		GetParkScence();
	}
	else if (m_nShowType == 2)
	{
		GetParkParam();
	}
}

void Dlg_Scene::OnSetTxtVisible()
{
	if (m_messageBox)
	{
		m_messageBox->close();
		SAFE_DELETE(m_messageBox);
	}

}

void Dlg_Scene::Slot_BtnSaveClicked()
{	
	OnSave();
	if (!m_messageBox)
	{
		QTimer::singleShot(2000, this, &Dlg_Scene::OnSetTxtVisible);
		m_messageBox = new Dlg_MessageBox;
		m_messageBox->SetInfoText(GBUTF8("保存成功！"));
		m_messageBox->SetBtnNoVisible(false);
		m_messageBox->exec();
	}
	
}

void Dlg_Scene::Slot_TabCurrentChanged()
{
	if (m_nSceneIndex == -1)
	{
		return;
	}
	if (this->sender() == ui.btn_bayonet)
	{
		ui.btn_drawArea->setVisible(true);
		OnSwitchPage(0, true);
	}
	else if (this->sender() == ui.btn_illegalPark)
	{
		ui.btn_drawArea->setVisible(true);
		ui.btn_snap->setVisible(true);
		OnSwitchPage(1, true);
	}
	else if (this->sender() == ui.btn_illegalMode)
	{
		ui.btn_drawArea->setVisible(false);
		ui.btn_snap->setVisible(false);
		OnSwitchPage(2, true);
	}
}

void Dlg_Scene::Slot_BayonentChanged()
{
	ui.lab_scence->setText(GBUTF8("场景选择：%1").arg(m_tSeceneCfg.dwSceneId));
	int nNum = m_tSeceneCfg.dwSceneSize;
	ui.cmb_scene->clear();

	if (m_nSceneIndex == -1)
	{
		m_nSceneIndex = 0;
	}
	if (nNum < m_nSceneIndex)
	{
		m_nSceneIndex = 0;
	}
	
	QStringList slist;
	for (int i = 1; i <= nNum; i++)
	{
		slist.append(QString("%1").arg(i));
	}
	ui.cmb_scene->setProperty("id", 111);
	ui.cmb_scene->addItems(slist);
	Sleep(1);
	ui.cmb_scene->setProperty("id", 0);
	if (m_nShowType == -1
		|| m_nShowType == 0)
	{
		m_nShowType = 0;
		ui.stackedWidget->setCurrentIndex(0);
		OnShowBayonet(true);
	}
	
	ui.cmb_scene->setProperty("id", 111);
	ui.cmb_scene->setCurrentIndex(m_nSceneIndex);
	Sleep(1);
	ui.cmb_scene->setProperty("id", 0);
	
}
void Dlg_Scene::UpdateScene(bool bUpdate)
{
	if (bUpdate)
	{
		wchar_t wz[129];
		ZeroMemory(wz, 129);
		QString sRoadNumber = ui.ledt_roadNumber->text();
		ui.ledt_roadNumber->text().toWCharArray(wz);
		wchar2char(m_tSeceneCfg.tSceneCfg.szRoadCode, wz);
		m_tSeceneCfg.tSceneCfg.dwLaneWidth = ui.ledt_plateWidth->text().toInt();
		m_tSeceneCfg.tSceneCfg.tPhyLane[m_nIndex].dwLaneLabel = ui.ledt_plateNum->text().toInt();
		m_tSeceneCfg.tSceneCfg.tPhyLane[m_nIndex].dwDriveDir = ui.ledt_directionCode->text().toInt();
		m_tSeceneCfg.tSceneCfg.tPhyLane[m_nIndex].dwLandId = ui.ledt_plateID->text().toInt();

		m_tSeceneCfg.tSceneCfg.dwLaneNum = ui.cmb_plateNum->currentIndex() + 1;
		int nSel = ui.cmb_driveDirection->currentIndex();
		switch (nSel)
		{
		case 0:
			m_tSeceneCfg.tSceneCfg.tPhyLane[m_nIndex].dwDriveDir = DRIVE_DIR_TYPE_LX;
			break;
		case 1:
			m_tSeceneCfg.tSceneCfg.tPhyLane[m_nIndex].dwDriveDir = DRIVE_DIR_TYPE_QX;
			break;
		default:
			m_tSeceneCfg.tSceneCfg.tPhyLane[m_nIndex].dwDriveDir = DRIVE_DIR_TYPE_LX;
			break;
		}
		nSel = ui.cmb_testMode->currentIndex();
		m_tSeceneCfg.tSceneCfg.tPhyLane[m_nIndex].dwDetType = AnalyIndex2CheckMode(nSel);
		nSel = ui.cmb_snapDay->currentIndex();
		m_tSeceneCfg.tSceneCfg.tPhyLane[m_nIndex].dwDaySnapLight = AnalyIndex2LightMode(nSel);
		nSel = ui.cmb_snapNight->currentIndex();
		m_tSeceneCfg.tSceneCfg.tPhyLane[m_nIndex].dwNightSnapLight = AnalyIndex2LightMode(nSel);

		nSel = ui.cmb_plateDirection->currentIndex();
		m_tSeceneCfg.tSceneCfg.tPhyLane[m_nIndex].szLaneDir[0] = nSel + 1;
		// chk 填写
		for (int i = 0; i < m_chkBayonetAll.size(); i++)
		{
			GetChkValue(&m_tSeceneCfg, m_chkBayonetAll.at(i), i, m_nIndex);
		}
	}
	else
	{
		wchar_t wz[129];
		char2wchar(wz, m_tSeceneCfg.tSceneCfg.szRoadCode);
		ui.ledt_roadNumber->setText(QString::fromWCharArray(wz));
		ui.ledt_plateWidth->setText(QString("%1").arg(m_tSeceneCfg.tSceneCfg.dwLaneWidth));
		ui.ledt_plateNum->setText(QString("%1").arg(m_tSeceneCfg.tSceneCfg.tPhyLane[m_nIndex].dwLaneLabel));
		ui.ledt_directionCode->setText(QString("%1").arg(m_tSeceneCfg.tSceneCfg.tPhyLane[m_nIndex].dwDriveDir));
		ui.ledt_plateID->setText(QString("%1").arg(m_tSeceneCfg.tSceneCfg.tPhyLane[m_nIndex].dwLandId));

		int nIndex = AnalyCheckMode2Index(m_tSeceneCfg.tSceneCfg.tPhyLane[m_nIndex].dwDetType);
		ui.cmb_testMode->setCurrentIndex(nIndex);
		switch (m_tSeceneCfg.tSceneCfg.tPhyLane[m_nIndex].dwDriveDir)
		{
		case DRIVE_DIR_TYPE_LX:
			nIndex = 0;
			break;
		case DRIVE_DIR_TYPE_QX:
			nIndex = 1;
			break;
		default:
			nIndex = 0;
			break;
		}
		ui.cmb_driveDirection->setCurrentIndex(nIndex);
		nIndex = AnalyLightMode2Index(m_tSeceneCfg.tSceneCfg.tPhyLane[m_nIndex].dwDaySnapLight);
		ui.cmb_snapDay->setCurrentIndex(nIndex);
		nIndex = AnalyLightMode2Index(m_tSeceneCfg.tSceneCfg.tPhyLane[m_nIndex].dwNightSnapLight);
		ui.cmb_snapNight->setCurrentIndex(nIndex);
		nIndex = m_tSeceneCfg.tSceneCfg.tPhyLane[m_nIndex].szLaneDir[0] - 1;
		if (nIndex < 0)
		{
			nIndex = 0;
		}
		ui.cmb_plateDirection->setCurrentIndex(nIndex);

		// chk 填写
		for (int i = 0; i < m_chkBayonetAll.size(); i++)
		{
			SetChkValue(&m_tSeceneCfg, m_chkBayonetAll.at(i), i, m_nIndex);
		}
		//避免循环
		//ui.cmb_plateNum->setProperty("id", 111);
		ui.cmb_plateNum->setCurrentIndex(m_tSeceneCfg.tSceneCfg.dwLaneNum - 1);

		if (tLocalCfgInfo.ispic == 0)
		{
			ui.widget_3->SetScenceDrawData(&m_tSeceneCfg);
		}
		else
		{
			//SetScenceDrawData();
		}

	}
}

// 获取指定位的值0/1
int Dlg_Scene::GetSpecifyBitValue(Uint32 dwSrc, int nIndex)
{
	Uint32 dw2 = ((int)pow(2.0, nIndex));
	dwSrc = dwSrc & dw2;
	dwSrc = dwSrc >> nIndex;

	return dwSrc;
}

u32 Dlg_Scene::SetSpecifyBitValue(Uint32& dwSrcOut, Uint32 dwValue, int nIndex)
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

bool Dlg_Scene::SetChkValue(void* pData, QCheckBox* pChkBtn, int em, int nIndex)
{

	if (pChkBtn && pData)
	{
		int nState = -1;
		switch (em)
		{
		case EMSCENECTRLCHK_CHKBSD_F1:
		{
			nState = GetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwSnapF, 0);
		}
			break;
		case EMSCENECTRLCHK_CHKBSD_F2:
		{
			nState = GetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwSnapF, 1);
		}
			break;
		case EMSCENECTRLCHK_CHKBSD_F3:
		{
			nState = GetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwSnapF, 2);
		}
			break;
		case EMSCENECTRLCHK_CHKBSD_F4:
		{
			nState = GetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwSnapF, 3);
		}
			break;
		case EMSCENECTRLCHK_CHKLED_NEI:
		{
			nState = GetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwSnapLED, 0);
		}
			break;
		case EMSCENECTRLCHK_CHKLED_WAI1:
		{
			nState = GetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwSnapLED, 1);
		}
			break;
		case EMSCENECTRLCHK_CHKLED_WAI2:
		{
			nState = GetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwSnapLED, 2);
		}
			break;
		case EMSCENECTRLCHK_CHKTOTRUN:
		{
			nState = GetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwLaneAttribute, 3);
		}
			break;
		case EMSCENECTRLCHK_CHKTOLEFT:
		{
			nState = GetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwLaneAttribute, 2);
		}
			break;
		case EMSCENECTRLCHK_CHKTORIGHT:
		{
			nState = GetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwLaneAttribute, 0);
		}
			break;
		case EMSCENECTRLCHK_CHKTOLINE:
		{
			nState = GetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwLaneAttribute, 1);
		}
			break;
		case EMSCENECTRLCHK_CHKSANP_0:
		{
			nState = GetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwSnapType, 0);

		}
			break;
		case EMSCENECTRLCHK_CHKSANP_1:
		{
			nState = GetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwSnapType, 1);
		}
			break;
		case EMSCENECTRLCHK_CHKSANP_3:
		{
			nState = GetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwSnapType, 3);
		}
			break;
		case EMSCENECTRLCHK_CHKSANP_6:
		{
			nState = GetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwSnapType, 6);
		}
			break;
		case EMSCENECTRLCHK_CHKSANP_8:
		{
			nState = GetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwSnapType, 8);
		}
			break;
		case EMSCENECTRLCHK_CHKSANP_9:
		{
			nState = GetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwSnapType, 9);
		}
			break;
		case EMSCENECTRLCHK_CHKSANP_10:
		{
			nState = GetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwSnapType, 10);
		}
			break;
		case EMSCENECTRLCHK_CHKSANP_12:
		{
			nState = GetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwSnapType, 12);
		}
			break;
		case EMSCENECTRLCHK_CHKSANP_13:
		{
			nState = GetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwSnapType, 13);
		}
			break;
		case EMSCENECTRLCHK_CHKSANP_14:
		{
			nState = GetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwSnapType, 14);
		}
			break;
		case EMSCENECTRLCHK_CHKSANP_17:
		{
			nState = GetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwSnapType, 17);
		}
			break;
		case EMSCENECTRLCHK_CHKSANP_18:
		{
			nState = GetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwSnapType, 18);
		}
			break;
		case EMSCENECTRLCHK_CHKSANP_19:
		{
			nState = GetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwSnapType, 19);
		}
			break;
		case EMSCENECTRLCHK_CHKSANP_20:
		{
			nState = GetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwSnapType, 20);
		}
			break;
		case EMSCENECTRLCHK_CHKSANP_21:
		{
			nState = GetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwSnapType, 21);
		}
			break;
		case EMSCENECTRLCHK_CHKSANP_22:
		{
			nState = GetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwSnapType, 22);
		}
			break;
		case EMSCENECTRLCHK_CHKSANP_23:
		{
			nState = GetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwSnapType, 23);
		}
			break;
		case EMSCENECTRLCHK_CHKSANP_24:
		{
			nState = GetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwSnapType, 24);
		}
			break;
		case EMSCENECTRLCHK_CHKSANP_25:
		{
			nState = GetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwSnapType, 25);
		}
			break;
		case EMSCENECTRLCHK_CHKSANP_26:
		{
			nState = GetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwSnapType, 26);
		}
			break;
		case EMSCENECTRLCHK_CHKSANP_27:
		{
			nState = GetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwSnapType, 27);
		}
			break;
		case EMSCENECTRLCHK_CHKSANP_1_88:
		{
			nState = GetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwSnapType, 28);
		}
			break;
		case EMSCENECTRLCHK_CHKSANP_1_0:
		{
			nState = GetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwExtSnapType, 0);
		}
			break;
		case EMSCENECTRLCHK_CHKSANP_1_1:
		{
			nState = GetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwExtSnapType, 1);
		}
			break;
		case EMSCENECTRLCHK_CHKSANP_1_2:
		{
			nState = GetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwExtSnapType, 2);
		}
			break;
		case EMSCENECTRLCHK_CHKSANP_1_3:
		{
			nState = GetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwExtSnapType, 3);
		}
			break;
		case EMSCENECTRLCHK_CHKSANP_1_4:
		{
			nState = GetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwExtSnapType, 4);
		}
			break;
		case EMSCENECTRLCHK_CHKSANP_1_5:
		{
			nState = GetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwExtSnapType, 5);
		}
			break;
		case EMSCENECTRLCHK_CHKSANP_1_6:
		{
			nState = GetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwExtSnapType, 6);
		}
			break;
		case EMSCENECTRLCHK_CHKSANP_1_7:
		{
			nState = GetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwExtSnapType, 7);
		}
			break;
		case EMSCENECTRLCHK_CHKSANP_1_10:
		{
			nState = GetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwExtSnapType, 10);
		}
			break;
		case EMSCENECTRLCHK_CHKCPY_LANE1:
		{
			return true;
		}
			break;
		case EMSCENECTRLCHK_CHKCPY_LANE2:
		{
			return true;
		}
			break;
		case EMSCENECTRLCHK_CHKCPY_LANE3:
		{
			return true;
		}
			break;
		case EMSCENECTRLCHK_CHKCPY_LANE4:
		{
			return true;
		}
			break;
		}

		if (nState >= 0)
		{
			pChkBtn->setChecked(nState == 1);
			return true;
		}
	}

	return false;

}

bool Dlg_Scene::GetChkValue(void* pData, QCheckBox* pChkBtn, int em, int nIndex)
{
	if (pChkBtn)
	{
		int nSel = (int)pChkBtn->isChecked();
		switch (em)
		{
		case EMSCENECTRLCHK_CHKBSD_F1:
		{
			((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwSnapF = 0;
			SetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwSnapF, nSel, 0);
		}
			break;
		case EMSCENECTRLCHK_CHKBSD_F2:
		{
			SetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwSnapF, nSel, 1);
		}
			break;
		case EMSCENECTRLCHK_CHKBSD_F3:
		{
			SetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwSnapF, nSel, 2);
		}
			break;
		case EMSCENECTRLCHK_CHKBSD_F4:
		{
			SetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwSnapF, nSel, 3);
		}
			break;
		case EMSCENECTRLCHK_CHKLED_NEI:
		{
			((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwSnapLED = 0;
			SetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwSnapLED, nSel, 0);
		}
			break;
		case EMSCENECTRLCHK_CHKLED_WAI1:
		{
			SetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwSnapLED, nSel, 1);
		}
			break;
		case EMSCENECTRLCHK_CHKLED_WAI2:
		{
			SetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwSnapLED, nSel, 2);
		}
			break;

		case EMSCENECTRLCHK_CHKTORIGHT:
		{
			((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwLaneAttribute = 0;
			SetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwLaneAttribute, nSel, 0);
		}
			break;
		case EMSCENECTRLCHK_CHKTOLINE:
		{
			SetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwLaneAttribute, nSel, 1);
		}
			break;
		case EMSCENECTRLCHK_CHKTOLEFT:
		{
			SetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwLaneAttribute, nSel, 2);
		}
			break;
		case EMSCENECTRLCHK_CHKTOTRUN:
		{
			SetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwLaneAttribute, nSel, 3);
		}
			break;
		case EMSCENECTRLCHK_CHKSANP_0:
		{
			((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwSnapType = 0;
			SetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwSnapType, nSel, 0);
		}
			break;
		case EMSCENECTRLCHK_CHKSANP_1:
		{
			SetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwSnapType, nSel, 1);
		}
			break;
		case EMSCENECTRLCHK_CHKSANP_3:
		{
			SetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwSnapType, nSel, 3);
		}
			break;
		case EMSCENECTRLCHK_CHKSANP_6:
		{
			SetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwSnapType, nSel, 6);
		}
			break;
		case EMSCENECTRLCHK_CHKSANP_8:
		{
			SetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwSnapType, nSel, 8);
		}
			break;
		case EMSCENECTRLCHK_CHKSANP_9:
		{
			SetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwSnapType, nSel, 9);
		}
			break;
		case EMSCENECTRLCHK_CHKSANP_10:
		{
			SetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwSnapType, nSel, 10);
		}
			break;
		case EMSCENECTRLCHK_CHKSANP_12:
		{
			SetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwSnapType, nSel, 12);
		}
			break;
		case EMSCENECTRLCHK_CHKSANP_13:
		{
			SetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwSnapType, nSel, 13);
		}
			break;
		case EMSCENECTRLCHK_CHKSANP_14:
		{
			SetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwSnapType, nSel, 14);
		}
			break;
		case EMSCENECTRLCHK_CHKSANP_17:
		{
			SetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwSnapType, nSel, 17);
		}
			break;
		case EMSCENECTRLCHK_CHKSANP_18:
		{
			SetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwSnapType, nSel, 18);
		}
			break;
		case EMSCENECTRLCHK_CHKSANP_19:
		{
			SetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwSnapType, nSel, 19);
		}
			break;
		case EMSCENECTRLCHK_CHKSANP_20:
		{
			SetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwSnapType, nSel, 20);
		}
			break;
		case EMSCENECTRLCHK_CHKSANP_21:
		{
			SetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwSnapType, nSel, 21);
		}
			break;
		case EMSCENECTRLCHK_CHKSANP_22:
		{
			SetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwSnapType, nSel, 22);
		}
			break;
		case EMSCENECTRLCHK_CHKSANP_23:
		{
			SetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwSnapType, nSel, 23);
		}
			break;
		case EMSCENECTRLCHK_CHKSANP_24:
		{
			SetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwSnapType, nSel, 24);
		}
			break;
		case EMSCENECTRLCHK_CHKSANP_25:
		{
			SetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwSnapType, nSel, 25);
		}
			break;
		case EMSCENECTRLCHK_CHKSANP_26:
		{
			SetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwSnapType, nSel, 26);
		}
			break;
		case EMSCENECTRLCHK_CHKSANP_27:
		{
			SetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwSnapType, nSel, 27);
		}
			break;
		case EMSCENECTRLCHK_CHKSANP_1_88:
		{
			SetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwSnapType, nSel, 28);
		}
			break;
		case EMSCENECTRLCHK_CHKSANP_1_0:
		{
			SetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwExtSnapType, nSel, 0);
		}
			break;
		case EMSCENECTRLCHK_CHKSANP_1_1:
		{
			SetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwExtSnapType, nSel, 1);
		}
			break;
		case EMSCENECTRLCHK_CHKSANP_1_2:
		{
			SetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwExtSnapType, nSel, 2);
		}
			break;
		case EMSCENECTRLCHK_CHKSANP_1_3:
		{
			SetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwExtSnapType, nSel, 3);
		}
			break;
		case EMSCENECTRLCHK_CHKSANP_1_4:
		{
			SetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwExtSnapType, nSel, 4);
		}
			break;
		case EMSCENECTRLCHK_CHKSANP_1_5:
		{
			SetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwExtSnapType, nSel, 5);
		}
			break;
		case EMSCENECTRLCHK_CHKSANP_1_6:
		{
			SetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwExtSnapType, nSel, 6);
		}
			break;
		case EMSCENECTRLCHK_CHKSANP_1_7:
		{
			SetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwExtSnapType, nSel, 7);
		}
			break;
		case EMSCENECTRLCHK_CHKSANP_1_10:
		{
			SetSpecifyBitValue(((TIPC_GetSceneCfgRsp*)pData)->tSceneCfg.tPhyLane[nIndex].dwExtSnapType, nSel, 10);
		}
			break;

		case EMSCENECTRLCHK_CHKCPY_LANE1:
		{
			return true;
		}
			break;
		case EMSCENECTRLCHK_CHKCPY_LANE2:
		{
			return true;
		}
			break;
		case EMSCENECTRLCHK_CHKCPY_LANE3:
		{
			return true;
		}
			break;
		case EMSCENECTRLCHK_CHKCPY_LANE4:
		{
			return true;
		}
			break;
		}
	}

	return true;
}


void Dlg_Scene::UpdatePark(bool bUpdate)
{
	if (bUpdate)
	{
		m_tIPC_illgParkPara.illParkTime = ui.ledt_illpark->text().toInt();
		m_tIPC_illgParkPara.RoiAreaNum = ui.cmb_areaSum->currentIndex();
		m_tIPC_illgParkPara.workMode = ui.cmb_workMode->currentIndex();
		m_tIPC_illgParkPara.dwSecSnapTime = ui.ledt_snapTime->text().toInt();
		m_tIPC_illgParkPara.illParkNoticeTime = ui.ledt_notice->text().toInt();

		m_tIPC_illgParkPara.picMode[0] = ui.cmb_pic1->currentIndex() + 1;
		m_tIPC_illgParkPara.picMode[1] = ui.cmb_pic2->currentIndex() + 1;
		m_tIPC_illgParkPara.picMode[2] = ui.cmb_pic3->currentIndex() + 1;
		m_tIPC_illgParkPara.picMode[3] = ui.cmb_pic4->currentIndex() + 1;

		m_tIPC_illgParkPara.picTime[0] = ui.ledt_Pic12Time->text().toInt();
		m_tIPC_illgParkPara.picTime[1] = ui.ledt_Pic34Time->text().toInt();

		m_tIPC_illgParkPara.tRoiArea[m_nIndex].parkTime = ui.ledt_illegalTime->text().toInt();
		m_tIPC_illgParkPara.tRoiArea[m_nIndex].minVehWidth = ui.ledt_carMiniWidth->text().toInt();
		m_tIPC_illgParkPara.tRoiArea[m_nIndex].maxVehWidth = ui.ledt_carMaxWidth->text().toInt();
		m_tIPC_illgParkPara.tRoiArea[m_nIndex].filterOn = ui.ledt_enterFilter->text().toInt();
		m_tIPC_illgParkPara.tRoiArea[m_nIndex].filterOff = ui.ledt_outFilter->text().toInt();
		m_tIPC_illgParkPara.tRoiArea[m_nIndex].level = ui.cmb_tetLevel->currentIndex();

		m_tIPC_illgParkPara.tRoiArea[m_nIndex].trimVal = ui.ledt_factor->text().toFloat();
		m_tIPC_illgParkPara.bPlateMode = ui.chk_noPlateNumberMode->isChecked();
		m_tIPC_illgParkPara.code = ui.ledt_illegalCode->text().toInt();
	}
	else
	{
		OnChangeAreaNum();
		ui.ledt_illpark->setText(QString("%1").arg(m_tIPC_illgParkPara.illParkTime));
		ui.ledt_snapTime->setText(QString("%1").arg(m_tIPC_illgParkPara.dwSecSnapTime));
		ui.ledt_notice->setText(QString("%1").arg(m_tIPC_illgParkPara.illParkNoticeTime));

		if (m_tIPC_illgParkPara.workMode == 2)
		{
			ui.ledt_notice->setEnabled(true);
		}
		else
		{
			ui.ledt_notice->setEnabled(false);
		}
		//ui.cmb_areaSum->setProperty("id", 111);
		ui.cmb_areaSum->setCurrentIndex(m_tIPC_illgParkPara.RoiAreaNum);
		ui.cmb_workMode->setCurrentIndex(m_tIPC_illgParkPara.workMode);
		//ui.cmb_pic1->setProperty("id", 111);
		//ui.cmb_pic2->setProperty("id", 111);
		//ui.cmb_pic3->setProperty("id", 111);
		//ui.cmb_pic4->setProperty("id", 111);
		ui.cmb_pic1->setCurrentIndex(m_tIPC_illgParkPara.picMode[0] - 1);
		ui.cmb_pic2->setCurrentIndex(m_tIPC_illgParkPara.picMode[1] - 1);
		ui.cmb_pic3->setCurrentIndex(m_tIPC_illgParkPara.picMode[2] - 1);
		ui.cmb_pic4->setCurrentIndex(m_tIPC_illgParkPara.picMode[3] - 1);

		ui.ledt_Pic12Time->setText(QString("%1").arg(m_tIPC_illgParkPara.picTime[0]));
		ui.ledt_Pic34Time->setText(QString("%1").arg(m_tIPC_illgParkPara.picTime[1]));
		ui.ledt_illegalTime->setText(QString("%1").arg(m_tIPC_illgParkPara.tRoiArea[m_nIndex].parkTime));
		ui.ledt_carMiniWidth->setText(QString("%1").arg(m_tIPC_illgParkPara.tRoiArea[m_nIndex].minVehWidth));
		ui.ledt_carMaxWidth->setText(QString("%1").arg(m_tIPC_illgParkPara.tRoiArea[m_nIndex].maxVehWidth));
		ui.ledt_enterFilter->setText(QString("%1").arg(m_tIPC_illgParkPara.tRoiArea[m_nIndex].filterOn));
		ui.ledt_outFilter->setText(QString("%1").arg(m_tIPC_illgParkPara.tRoiArea[m_nIndex].filterOff));
		ui.cmb_tetLevel->setCurrentIndex(m_tIPC_illgParkPara.tRoiArea[m_nIndex].level);
		ui.ledt_illegalCode->setText(QString("%1").arg(m_tIPC_illgParkPara.code));
		ui.ledt_factor->setText(QString::number(m_tIPC_illgParkPara.tRoiArea[m_nIndex].trimVal, 'f', 6));
	
		if (tLocalCfgInfo.ispic == 0)
		{
			ui.widget_3->SetPixSize(m_tSeceneCfg.tMaxParam.dwMaxWidth, m_tSeceneCfg.tMaxParam.dwMaxHeight);
			ui.widget_3->SetParkScenceDrawData(&m_tIPC_illgParkPara);
		}
		else
		{
			SetParkScenceDrawData(&m_tIPC_illgParkPara);
		}
		if (m_tIPC_illgParkPara.bPlateMode)
		{
			ui.chk_noPlateNumberMode->setChecked(true);
		}
		else
		{
			ui.chk_noPlateNumberMode->setChecked(false);
		}
	}
}

void Dlg_Scene::SetParkScenceDrawData(TIPC_illgParkPara *ptParkScence)
{
	/*m_pScenceDrawUI->Clear();

	TRoiArea_S apArea;
	for (int i = 0; i < ptParkScence->RoiAreaNum; i++)
	{
		apArea.dwRoiLabel = i;
		apArea.wPointNum = ptParkScence->tRoiArea[i].pointNum;
		memcpy(apArea.tPoint, ptParkScence->tRoiArea[i].tPoint, sizeof(TPoint_S)*apArea.wPointNum);
		m_pScenceDrawUI->AddRgn(&apArea, i, apArea.dwRoiLabel);
		m_pScenceDrawUI->SetColorByType(i, COLOR_YELLOW);
	}*/
}

void Dlg_Scene::UpdateParkParam(bool bUpdate)
{

	if (bUpdate)
	{
		m_tParkParam.mode = ui.cmb_illegalType->currentIndex();
		m_tParkParam.autoMode = ui.cmb_autoType->currentIndex();
		m_tParkParam.snapMode = ui.cmb_detailType->currentIndex();
	}
	else
	{
		ui.cmb_illegalType->setCurrentIndex(m_tParkParam.mode);
		ui.cmb_autoType->setCurrentIndex(m_tParkParam.autoMode);
		ui.cmb_detailType->setCurrentIndex(m_tParkParam.snapMode);
	}
}


void Dlg_Scene::OnUpdateData(bool bUpdate)
{
	switch (m_nShowType)
	{
	case 0:
		UpdateScene(bUpdate);
		break;
	case 1:
		UpdatePark(bUpdate);
		break;
	case 2:
		UpdateParkParam(bUpdate);
		break;
	default:
		break;
	}
}