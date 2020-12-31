#pragma once

#include <QWidget>
#include "ui_Dlg_LogicLane_Detail.h"
#include <QDialog>
#include "ivhs_struct.h"
#include "define.h"
#include "delegate.h"
#define GBUTF8(da) QString::fromLocal8Bit(da)
#include <QPushButton>

class Dlg_LogicLane_Detail : public QDialog
{
	Q_OBJECT

public:
	Dlg_LogicLane_Detail(QWidget *parent = Q_NULLPTR);

	~Dlg_LogicLane_Detail();

	bool eventFilter(QObject *watched, QEvent *event);

	void Init();

	void SetOrderCmbData(bool isAddSpecial = false);

	void GetWeekData(int index);

	void CopyWeekData();

	int GetSpecifyBitValue(Uint32 dwSrc, int nIndex);

	u32 SetSpecifyBitValue(Uint32& dwSrcOut, Uint32 dwValue, int nIndex);

	void VehicleImgChange();

	void PeopleImgChange();

	void UpdateAddFace(bool  isEnable, bool is = false);

	void OnSwitchSnapCtrlMode(int nShowIndex);

	void SetMultiFormatCmbVisible();

	void PlateImgChange();

	void SetWeekData(int index);

	void SetData(TIPC_LogicLane* pData);

	void GetData();

	void ShowData(TIPC_LogicLane* pData);

public slots:

	void Slot_SwitchDayClicked();

	void Slot_BtnSaveClicked();

	void Slot_CheckBoxClicked(int nValue);

	void Slot_SwitchTabClicked();

	void Slot_CmbIndexChanged(int nIndex);

	void Slot_EditTextChanged(const QString &str);

	void Slot_DayCopyClicked();

private:
	Ui::Dlg_LogicLane_Detail ui;
	TIPC_LogicLane*		m_pLogicLaneSrc;
	TIPC_LogicLane*		m_pLogicLane;
	bool			    m_bSetData;
	QComboBox*		    m_apCmb[80];//logiclane::EMLOGICLANEDETAILS_COUNT
	QLineEdit*		    m_apEdt[80];//logiclane::EMLOGICLANEDETAILS_COUNT
	QCheckBox*			m_apChk[80];//logiclane::EMLOGICLANEDETAILS_COUNT
	QLineEdit*		    m_apEdtWeek[16];
	QCheckBox*			m_apChkDay[7];
	QPushButton*        m_apWeek[7];
	QCheckBox*			m_apChkTailLpr[10];
	int                 m_nWeek;
	bool				m_bSetlight;
};
