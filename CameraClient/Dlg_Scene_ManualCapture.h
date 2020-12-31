#pragma once

#include <QWidget>
#include <QDialog>
#include "ui_Dlg_Scene_ManualCapture.h"
#include <subject.h>
#include "ivhs_struct.h"
#include "define.h"
#define GBUTF8(da) QString::fromLocal8Bit(da)
class Dlg_Scene_ManualCapture : public QDialog, public CObserver
{
	Q_OBJECT

public:
	Dlg_Scene_ManualCapture(QWidget *parent = Q_NULLPTR);

	~Dlg_Scene_ManualCapture();

	void OnObserverNotify(LPARAM lHint, LPVOID pHint);

	void GetScenceParam();

	void GetIpcParkScence();

	void LoadScenceImage();

	void GetScenceID();

	void SnapIllegalParking();

	LRESULT OnRefreshPic(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

public slots:
	void Slot_LoadSceneClicked();

	void Slot_SnapClicked();

private:
	Ui::Dlg_Scene_ManualCapture ui;

	u32	m_scenceId;
	TIPC_GetSceneCfgRsp					  m_tSeceneCfg;
	TIPC_illgParkPara                     m_tIPC_illgParkPara;
	TIPC_SnapIllegalPark                  m_tSnapIllegalPark;
};
