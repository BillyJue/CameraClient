#pragma once

#include <QWidget>
#include "ui_Dlg_ScenePoll.h"
#include <subject.h>
#include "ivhs_struct.h"
#include "define.h"
#include <QButtonGroup>
#include <QPushButton>
#include <QLineEdit>
#include <QCheckBox>
#include "MyWidget.h"
#define GBUTF8(da) QString::fromLocal8Bit(da)
class Dlg_ScenePoll : public MyWidget, public CObserver
{
	Q_OBJECT

public:
	Dlg_ScenePoll(QWidget *parent = Q_NULLPTR);

	~Dlg_ScenePoll();

	//初始化数据
	void InitData(bool is);

	void OnObserverNotify(LPARAM lHint, LPVOID pHint);

	bool GetScencePoll();

	void UpdateData(bool bUpdate);

	QPushButton *GetScenceButton();

	QPushButton *GetDayButton();

	void SetWeekData(int index);

	void GetWeekData(int index);

	void CopySceneData();

	void CopyWeekData();
public slots:

	void Slot_ScenceButtonClicked(QAbstractButton*);

	void Slot_DayButtonClicked(QAbstractButton* btn);

	void Slot_DataChanged();

	void Slot_ScenceCopyClicked();

	void Slot_DayCopyClicked();
signals:
	void signal_dataChanged();
private:
	Ui::Dlg_ScenePoll ui;

	QButtonGroup *m_scenceGroup;
	QButtonGroup *m_dayGroup;
	QButtonGroup *m_checkdayGroup;
	QButtonGroup *m_checkscenceGroup;
	int	 m_nScenceIndex;
	int  m_nDayIndex;
	TIPC_ScenePoll    m_tScenePoll[8];
	QLineEdit*		  m_apEdtWeek[20];
	QCheckBox         *m_apChkScene[8];
	QCheckBox		  *m_apChkDay[7];

};
