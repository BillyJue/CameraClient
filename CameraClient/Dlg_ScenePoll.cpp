#include "Dlg_ScenePoll.h"
#include "MgrData.h"
#include <QPushButton>
#include <QVariant>
Dlg_ScenePoll::Dlg_ScenePoll(QWidget *parent)
	: MyWidget(parent), m_scenceGroup(nullptr)
	, m_dayGroup(nullptr), m_checkdayGroup(nullptr)
	, m_checkscenceGroup(nullptr), m_nScenceIndex(0), m_nDayIndex(0)
{
	ui.setupUi(this);
	MgrData::getInstance()->GetMgrSubject()->Add(this);
	connect(this, SIGNAL(signal_dataChanged()), this, SLOT(Slot_DataChanged()));
	memset(&m_tScenePoll, 0, sizeof(m_tScenePoll));
	m_scenceGroup = new QButtonGroup(this);
	m_scenceGroup->setExclusive(true);
	ui.btn_sence1->setCheckable(true);
	ui.btn_sence1->setProperty("nIndex", 0);
	ui.btn_sence2->setCheckable(true);
	ui.btn_sence2->setProperty("nIndex", 1);
	ui.btn_sence3->setCheckable(true);
	ui.btn_sence3->setProperty("nIndex", 2);
	ui.btn_sence4->setCheckable(true);
	ui.btn_sence4->setProperty("nIndex", 3);
	m_scenceGroup->addButton(ui.btn_sence1);
	m_scenceGroup->addButton(ui.btn_sence2);
	m_scenceGroup->addButton(ui.btn_sence3);
	m_scenceGroup->addButton(ui.btn_sence4);
	
	m_dayGroup = new QButtonGroup(this);
	m_dayGroup->setExclusive(true);
	ui.btn_day1->setCheckable(true);
	ui.btn_day1->setProperty("nIndex", 1);
	ui.btn_day2->setCheckable(true);
	ui.btn_day2->setProperty("nIndex", 2);
	ui.btn_day3->setCheckable(true);
	ui.btn_day3->setProperty("nIndex", 3);
	ui.btn_day4->setCheckable(true);
	ui.btn_day4->setProperty("nIndex", 4);
	ui.btn_day5->setCheckable(true);
	ui.btn_day5->setProperty("nIndex", 5);
	ui.btn_day6->setCheckable(true);
	ui.btn_day6->setProperty("nIndex", 6);
	ui.btn_day7->setCheckable(true);
	ui.btn_day7->setProperty("nIndex", 0);
	m_dayGroup->addButton(ui.btn_day1);
	m_dayGroup->addButton(ui.btn_day2);
	m_dayGroup->addButton(ui.btn_day3);
	m_dayGroup->addButton(ui.btn_day4);
	m_dayGroup->addButton(ui.btn_day5);
	m_dayGroup->addButton(ui.btn_day6);
	m_dayGroup->addButton(ui.btn_day7);

	m_checkdayGroup = new QButtonGroup(this);
	m_checkdayGroup->setExclusive(true);
	m_checkdayGroup->addButton(ui.chk_day1);
	m_checkdayGroup->addButton(ui.chk_day2);
	m_checkdayGroup->addButton(ui.chk_day3);
	m_checkdayGroup->addButton(ui.chk_day4);
	m_checkdayGroup->addButton(ui.chk_day5);
	m_checkdayGroup->addButton(ui.chk_day6);
	m_checkdayGroup->addButton(ui.chk_day7);

	m_checkscenceGroup = new QButtonGroup(this);
	m_checkscenceGroup->setExclusive(true);
	m_checkscenceGroup->addButton(ui.chk_scence1);
	m_checkscenceGroup->addButton(ui.chk_scence2);
	m_checkscenceGroup->addButton(ui.chk_scence3);
	m_checkscenceGroup->addButton(ui.chk_scence4);


	m_apEdtWeek[0] = ui.ledt_1_hour_from;
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
	m_apEdtWeek[15] = ui.ledt_4_min_to;
	m_apEdtWeek[16] = ui.ledt_time1;
	m_apEdtWeek[17] = ui.ledt_time2;
	m_apEdtWeek[18] = ui.ledt_time3;
	m_apEdtWeek[19] = ui.ledt_time4;

	m_apChkScene[0] = ui.chk_scence1;
	m_apChkScene[1] = ui.chk_scence2;
	m_apChkScene[2] = ui.chk_scence3;
	m_apChkScene[3] = ui.chk_scence4;
	m_apChkScene[4] = ui.chk_scence5;
	m_apChkScene[5] = ui.chk_scence6;
	m_apChkScene[6] = ui.chk_scence7;
	m_apChkScene[7] = ui.chk_scence8;

	m_apChkDay[0] = ui.chk_day7;
	m_apChkDay[1] = ui.chk_day1;
	m_apChkDay[2] = ui.chk_day2;
	m_apChkDay[3] = ui.chk_day3;
	m_apChkDay[4] = ui.chk_day4;
	m_apChkDay[5] = ui.chk_day5;
	m_apChkDay[6] = ui.chk_day6;
	//只显示1-4场景其它隐藏
	ui.btn_sence5->setVisible(false);
	ui.btn_sence6->setVisible(false);
	ui.btn_sence7->setVisible(false);
	ui.btn_sence8->setVisible(false);
	ui.chk_scence5->setVisible(false);
	ui.chk_scence6->setVisible(false);
	ui.chk_scence7->setVisible(false);
	ui.chk_scence8->setVisible(false);


	connect(m_scenceGroup, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(Slot_ScenceButtonClicked(QAbstractButton*)));
	connect(m_dayGroup, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(Slot_DayButtonClicked(QAbstractButton*)));
	connect(ui.btn_sceneCopy, SIGNAL(clicked()), this, SLOT(Slot_ScenceCopyClicked()));
	connect(ui.btn_dayCopy, SIGNAL(clicked()), this, SLOT(Slot_DayCopyClicked()));

	emit ui.btn_sence1->click();
	emit ui.btn_day7->click();

	//InitData();
}

void Dlg_ScenePoll::Slot_ScenceCopyClicked()
{
	CopySceneData();
}

void Dlg_ScenePoll::Slot_DayCopyClicked()
{
	CopyWeekData();
}

void Dlg_ScenePoll::CopyWeekData()
{
	GetWeekData(m_nDayIndex);

	for (int i = 0; i < 7; i++)
	{
		if (i != m_nDayIndex && m_apChkDay[i]->isChecked())
		{
			memcpy(&m_tScenePoll[m_nScenceIndex].tSnapTiming[i],
				&m_tScenePoll[m_nScenceIndex].tSnapTiming[m_nDayIndex], sizeof(TIPC_SnapTiming));
			memcpy(&m_tScenePoll[m_nScenceIndex].stayTime[i][0], &m_tScenePoll[m_nScenceIndex].stayTime[m_nDayIndex][0], sizeof(Uint32) * 4);
		}
	}
}

QPushButton *Dlg_ScenePoll::GetScenceButton()
{
	switch (m_nScenceIndex)
	{
	case 0:
		return ui.btn_sence1;
		break;
	case 1:
		return ui.btn_sence2;
		break;
	case 2:
		return ui.btn_sence3;
		break;
	case 3:
		return ui.btn_sence4;
		break;
	}
	
}

QPushButton *Dlg_ScenePoll::GetDayButton()
{
	switch (m_nDayIndex)
	{
	case 0:
		return ui.btn_day1;
		break;
	case 1:
		return ui.btn_day2;
		break;
	case 2:
		return ui.btn_day3;
		break;
	case 3:
		return ui.btn_day4;
		break;
	case 4:
		return ui.btn_day5;
		break;
	case 5:
		return ui.btn_day6;
		break;
	case 6:
		return ui.btn_day7;
		break;
	}

}

void Dlg_ScenePoll::UpdateData(bool bUpdate)
{
	if (bUpdate)
	{
		m_tScenePoll[m_nScenceIndex].bEnable = ui.chk_enable->isChecked();
		m_tScenePoll[m_nScenceIndex].maxShutter = ui.ledt_quickLimit->text().toInt();
		m_tScenePoll[m_nScenceIndex].maxAgain = ui.ledt_moreLimit->text().toInt();
		m_tScenePoll[m_nScenceIndex].internalLight1 = ui.ledt_inner1->text().toInt();
		m_tScenePoll[m_nScenceIndex].internalLight2 = ui.ledt_inner2->text().toInt();
		m_tScenePoll[m_nScenceIndex].internalLight3 = ui.ledt_inner3->text().toInt();
		m_tScenePoll[m_nScenceIndex].externalLight1 = ui.ledt_outerHeight->text().toInt();
		GetWeekData(m_nDayIndex);
	}
	else
	{
		ui.lab_scenceId->setText(GBUTF8("场景ID: %1").arg(m_tScenePoll[m_nScenceIndex].posId));
		ui.chk_enable->setChecked(m_tScenePoll[m_nScenceIndex].bEnable);
		ui.ledt_quickLimit->setText(QString("%1").arg(m_tScenePoll[m_nScenceIndex].maxShutter));
		ui.ledt_moreLimit->setText(QString("%1").arg(m_tScenePoll[m_nScenceIndex].maxAgain));
		ui.ledt_inner1->setText(QString("%1").arg(m_tScenePoll[m_nScenceIndex].internalLight1));
		ui.ledt_inner2->setText(QString("%1").arg(m_tScenePoll[m_nScenceIndex].internalLight2));
		ui.ledt_inner3->setText(QString("%1").arg(m_tScenePoll[m_nScenceIndex].internalLight3));
		ui.ledt_outerHeight->setText(QString("%1").arg(m_tScenePoll[m_nScenceIndex].externalLight1));

		SetWeekData(m_nDayIndex);

	}
}

void Dlg_ScenePoll::GetWeekData(int index)
{
	int begin_hour, begin_min, end_hour, end_min;
	int n = 0;

	for (int i = 0; i < 4; i++)
	{
		begin_hour = m_apEdtWeek[i * 4 + 0]->text().toInt();
		begin_min = m_apEdtWeek[i * 4 + 1]->text().toInt();
		end_hour = m_apEdtWeek[i * 4 + 2]->text().toInt();
		end_min = m_apEdtWeek[i * 4 + 3]->text().toInt();

		if ((end_hour * 60 + end_min) > (begin_hour * 60 + begin_min))
		{
			m_tScenePoll[m_nScenceIndex].tSnapTiming[index].startTimeHour[n] = begin_hour;
			m_tScenePoll[m_nScenceIndex].tSnapTiming[index].startTimeMin[n] = begin_min;
			m_tScenePoll[m_nScenceIndex].tSnapTiming[index].stopTimeHour[n] = end_hour;
			m_tScenePoll[m_nScenceIndex].tSnapTiming[index].stopTimeMin[n] = end_min;
			m_tScenePoll[m_nScenceIndex].stayTime[index][n] = m_apEdtWeek[i + 16]->text().toInt();
			n++;
		}
	}

	m_tScenePoll[m_nScenceIndex].tSnapTiming[index].num = n;
}

void Dlg_ScenePoll::SetWeekData(int index)
{
	wchar_t ws[129];
	for (int i = 0; i < 4; i++)
	{
		m_apEdtWeek[i * 4 + 0]->setText(QString("%1").arg(m_tScenePoll[m_nScenceIndex].tSnapTiming[index].startTimeHour[i]));
		m_apEdtWeek[i * 4 + 1]->setText(QString("%1").arg(m_tScenePoll[m_nScenceIndex].tSnapTiming[index].startTimeMin[i]));
		m_apEdtWeek[i * 4 + 2]->setText(QString("%1").arg(m_tScenePoll[m_nScenceIndex].tSnapTiming[index].stopTimeHour[i]));
		m_apEdtWeek[i * 4 + 3]->setText(QString("%1").arg(m_tScenePoll[m_nScenceIndex].tSnapTiming[index].stopTimeHour[i]));
		m_apEdtWeek[i + 16]->setText(QString("%1").arg(m_tScenePoll[m_nScenceIndex].stayTime[index][i]));

	}
	 //GetDayButton()->clicked();
}



void Dlg_ScenePoll::Slot_DataChanged()
{
	UpdateData(false);
}

void Dlg_ScenePoll::Slot_DayButtonClicked(QAbstractButton* btn)
{
	QPushButton *pBtn = (QPushButton*)btn;
	m_nDayIndex = pBtn->property("nIndex").toInt();
	SetWeekData(m_nDayIndex);
	/*ui.chk_day1->setEnabled(false);
	ui.chk_day2->setEnabled(false);
	ui.chk_day3->setEnabled(false);
	ui.chk_day4->setEnabled(false);
	ui.chk_day5->setEnabled(false);
	ui.chk_day6->setEnabled(false);
	ui.chk_day7->setEnabled(false);*/
	switch (m_nDayIndex)
	{
	case 0:
		//ui.chk_day7->setEnabled(true);
		ui.chk_day7->setChecked(true);
		break;
	case 1:
		//ui.chk_day1->setEnabled(true);
		ui.chk_day1->setChecked(true);
		break;
	case 2:
		//ui.chk_day2->setEnabled(true);
		ui.chk_day2->setChecked(true);
		break;
	case 3:
		//ui.chk_day3->setEnabled(true);
		ui.chk_day3->setChecked(true);
		break;
	case 4:
		//ui.chk_day4->setEnabled(true);
		ui.chk_day4->setChecked(true);
		break;
	case 5:
		//ui.chk_day5->setEnabled(true);
		ui.chk_day5->setChecked(true);
		break;
	case 6:
		//ui.chk_day6->setEnabled(true);
		ui.chk_day6->setChecked(true);
		break;
	}
}

void Dlg_ScenePoll::CopySceneData()
{
	UpdateData(TRUE);

	for (int i = 0; i < 8; i++)
	{
		if (i != m_nScenceIndex && m_apChkScene[i]->isChecked())
		{
			memcpy(&m_tScenePoll[i],
				&m_tScenePoll[m_nScenceIndex], sizeof(TIPC_ScenePoll));
		}
	}
}

void Dlg_ScenePoll::Slot_ScenceButtonClicked(QAbstractButton* btn)
{
	QPushButton *pBtn = (QPushButton*)btn;
	m_nScenceIndex = pBtn->property("nIndex").toInt();
	UpdateData(false);

	/*ui.chk_scence1->setEnabled(false);
	ui.chk_scence2->setEnabled(false);
	ui.chk_scence3->setEnabled(false);
	ui.chk_scence4->setEnabled(false);*/
	switch (m_nScenceIndex)
	{
	case 0:
		//ui.chk_scence1->setEnabled(true);
		ui.chk_scence1->setChecked(true);
		break;
	case 1:
		//ui.chk_scence2->setEnabled(true);
		ui.chk_scence2->setChecked(true);
		break;
	case 2:
		//ui.chk_scence3->setEnabled(true);
		ui.chk_scence3->setChecked(true);
		break;
	case 3:
		//ui.chk_scence4->setEnabled(true);
		ui.chk_scence4->setChecked(true);
		break;
	case 4:
		//ui.chk_scence5->setEnabled(true);
		ui.chk_scence5->setChecked(true);
		break;
	case 5:
		//ui.chk_scence6->setEnabled(true);
		ui.chk_scence6->setChecked(true);
		break;
	case 6:
		//ui.chk_scence7->setEnabled(true);
		ui.chk_scence7->setChecked(true);
		break;
	case 7:
		//ui.chk_scence8->setEnabled(true);
		ui.chk_scence8->setChecked(true);
		break;
	}
}

Dlg_ScenePoll::~Dlg_ScenePoll()
{
	MgrData::getInstance()->GetMgrSubject()->Del(this);
	SAFE_DELETE(m_scenceGroup);
	SAFE_DELETE(m_dayGroup);
	SAFE_DELETE(m_checkdayGroup);
	SAFE_DELETE(m_checkscenceGroup);
}

bool Dlg_ScenePoll::GetScencePoll()
{
	MgrItsCtrl* pMgrItsCtrl = MgrData::getInstance()->GetMgrItsCtrl();
	if (pMgrItsCtrl != NULL)
	{
		u32 dwIP = MgrData::getInstance()->GetIP();
		ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dwIP);
		if (pCtrl != NULL)
		{
			bool bRet = pCtrl->GetIpcScencePoll(NULL, (u32)(CObserver*)this);
			if (!bRet)
			{
			}
		}
	}
	return true;
}

void Dlg_ScenePoll::InitData(bool is)
{
	if (is)
	{
		GetScencePoll();
	}
	
}

void Dlg_ScenePoll::OnObserverNotify(LPARAM lHint, LPVOID pHint)
{

	u32 dwEvtType = (u32)lHint;
	u32 dwId = (u32)pHint;

	switch (dwEvtType)
	{
	case NMC_GET_IPC_SCENEPOLL:
	{
		TIPC_ScenePoll *p = (TIPC_ScenePoll*)pHint;
		memcpy(m_tScenePoll, p, sizeof(TIPC_ScenePoll) * 8);
		emit signal_dataChanged();
	}
		break;
	default:
		break;
	}
}



