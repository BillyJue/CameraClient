#pragma once

#include <QWidget>
#include "ui_Dlg_LogicLane.h"
#include <subject.h>
#include "ivhs_struct.h"
#include "define.h"
#include "Dlg_PictureDraw.h"
#include "delegate.h"
#include "MyWidget.h"
#include "Dlg_MessageBox.h"
#include <QMap>
#define GBUTF8(da) QString::fromLocal8Bit(da)
class Dlg_LogicLane : public MyWidget, public CObserver
{
	Q_OBJECT

public:
	Dlg_LogicLane(QWidget *parent = Q_NULLPTR);

	~Dlg_LogicLane();

	void OnSetTxtVisible();

	QWidget* CreateWidget(int nIndex);

	void SetScenceDrawData();

	bool GetParamLogicLane();

	void SetParamLogiclane();

	void InitData(bool is);

	void OnSnap();
	//ÔÝÊ±Ã»Ð´
	LRESULT OnRefreshPic(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void OnChangeScenceNum();

	void OnChangePreset(int nPreset);

	void GetParamRecogAreaInfo();

	void GetParamScenceInfo();

	void GetParamVideoLoopInfo();

	void GetScenceID();

	void UpdateData(bool bUpdate);

	void OnBuildLoginLane();

	void OnObserverNotify(LPARAM lHint, LPVOID pHint);

public slots:
	void Slot_LogicLaneInfoChanged();

	void Slot_SceneInfoChanged();
	
	void Slot_VideoLoopInfoChanged();

	void Slot_RecogAreaChanged();

	void Slot_CmbSenceIndexChanged(int nIndex);

	void Slot_CmbIndexChanged(int nRow, int nColmn);

	void Slot_TreeWidgetItemClicked(QTreeWidgetItem*pItem, int column);

	void Slot_BtnLoadSceneClicked();

	void Slot_BtnSaveClicked();

	void Slot_BtnFreshClicked();

	void Slot_BtnCreateLogicLaneClicked();

	void Slot_ShowLogicLaneDetailClicked();

signals:
	void signal_logicLaneInfoChanged();

	void signal_sceneInfoChanged();

	void signal_videoLoopInfoChanged();

	void signal_recogAreaChanged();

	
private:
	Ui::Dlg_LogicLane ui;
	Dlg_MessageBox				*m_messageBox;
	TIPC_SetLogicLaneReq		m_tLogicLaneInfo;
	TIPC_SetVideoLoopReq		m_tVideoLoopInfo;
	TIPC_GetSceneCfgRsp			m_tSeceneCfg;
	TIPC_SetRecogAreaReq		m_tRecogAreaInfo;
	TLocalCfgInfo				m_tLocalCfgInfo;
	Dlg_PictureDraw *			m_pixDraw;
	int						m_nSceneIndex;

	LabelDelegate				*m_labSeq;
	CheckBoxDelegate			*m_chkEnable;
	ComboboxDelegate			*m_cmbTestType;
	ComboboxDelegate			*m_cmbSensitive;
	ComboboxDelegate			*m_cmbRecogArea;
	ComboboxDelegate			*m_cmbLaneID;
	ComboboxDelegate			*m_cmbWhiteLight;
	ComboboxDelegate			*m_cmbNightLight;
	CheckBoxDelegate			*m_chkFaceExposure;
	QMap<int, QString>			 m_mapType;
};
