#pragma once

#include <QDialog>
#include <subject.h>
#include "ivhs_struct.h"
#include "define.h"
#include <QMap>
#define GBUTF8(da) QString::fromLocal8Bit(da)
#include "ui_dlg_main.h"
#include "dlg_deviceInfo.h"
#include "dlg_systemMaintenance.h"
#include "dlg_otherConfig.h"
#include "dlg_localConfig.h"
#include "dlg_illegaldict.h"
#include "Dlg_RegisterPlat.h"
#include "Dlg_SnapManager.h"
#include "Dlg_LightConfig.h"
#include "Dlg_OsdAddConfig.h"
#include "Dlg_PicView.h"
#include "Dlg_Detection.h"
#include "Dlg_Scene.h"
#include "Dlg_ImageQuality.h"
#include "Dlg_LogicLane.h"
#include "Dlg_PicMerge.h"
#include "Dlg_IdentParam.h"
#include "Dlg_FlowDetection.h"
#include "dlg_hardware.h"
#include "Dlg_ScenePoll.h"
//#include "drop_shadow_widget.h"
#include "customdialog.h"
class Dlg_main : public CustomDialog, public CObserver
{
	Q_OBJECT

public:
	Dlg_main(QWidget *parent = Q_NULLPTR);

	~Dlg_main();

	MyWidget* GetWidget(QPushButton *pBtn);

	void OnObserverNotify(LPARAM lHint, LPVOID pHint);

	void GetDevInfo();

	bool GetUICap();

	void Init();

	void InitConfigBtn();

	void InitData();

	void SelectPage(int index);

	bool event(QEvent *event);

public slots:
	void Slot_OpenBrowserClicked();

	void Slot_ItemClicked(QListWidgetItem *pItem);

	void Slot_UiCapChanged();

	void Slot_SelectPageClicked();

	void Slot_TabCurrentIndex();

	void SLot_LogoutClicked();

	void Slot_PTZStopClicked();

	void Slot_PTZControlClicked();

	void Slot_SizeChanged();
signals:
	void signal_uiCapChanged();

	void signal_loginOut();
private:
	Ui::dlg_main ui;
	int m_nIndexPage;
	int m_nCount;
	int m_nTabIndex;
	TIPC_GetUICapsRsp m_tUICap;
	Dlg_DeviceInfo *m_dlgDeviceInfo;
	Dlg_systemMaintenance *m_dlgSystemMaintenance;
	Dlg_otherConfig *m_dlgOtherConfig;
	Dlg_LocalConfig	*m_dlgLocalConfig;
	Dlg_IllegalDict *m_dlgIllegalDict;
	Dlg_RegisterPlat *m_dlgRegisterPlat;
	Dlg_SnapManager	 *m_snapManager;
	Dlg_LightConfig *m_dlgLightConfig;
	Dlg_OsdAddConfig *m_dlgOsdConfig;
	Dlg_PicView		 *m_dlgPictureView;
	Dlg_Detection	 *m_dlgDetection;
	Dlg_Scene *m_dlgScene;
	Dlg_ImageQuality *m_dlgImgQuality;
	Dlg_LogicLane	*m_dlgLogicLane;
	Dlg_PicMerge    *m_dlgPicMerge;
	Dlg_IdentParam  *m_dlgIdentParam;
	Dlg_FlowDetection *m_dlgFlowDetection;
	Dlg_HardWare	  *m_hardWare;
	Dlg_ScenePoll     *m_dlgScenePoll;
	QMap<int, QPushButton*> m_mapBtnInfo;
	QMap<QPushButton*, MyWidget*> m_mapWidget;

	MyWidget			*m_currentWidget;
};
