#pragma once

#include <QWidget>
#include "ui_Dlg_Detection.h"
#include <subject.h>
#include "ivhs_struct.h"
#include "define.h"
#include "delegate.h"
#include <QMap>
#include "global.h"
#include "MyWidget.h"
#include "Dlg_MessageBox.h"
Q_DECLARE_METATYPE(TIPC_VidLoopCfg)
Q_DECLARE_METATYPE(TIPC_CoilLoopCfg)
Q_DECLARE_METATYPE(TIPC_RadarLoopCfg)
Q_DECLARE_METATYPE(TIPC_RedLampSigLoop)
Q_DECLARE_METATYPE(TIPC_RedSigEnhance)
Q_DECLARE_METATYPE(recogarea::TRecogAreaParam)
#define GBUTF8(da) QString::fromLocal8Bit(da)


class Dlg_Detection : public MyWidget, public CObserver
{
	Q_OBJECT

public:
	Dlg_Detection(QWidget *parent = Q_NULLPTR);

	~Dlg_Detection();

	void OnSetTxtVisible();

	QWidget *CreateWidget(int nIndex);

	void OnReset();

	void OnCreateVideoLoop();

	void ClearAllTreeWidgetItem(QTreeWidget *pTree);

	void GetScenceID();

	//初始化视频检测数据
	void InitVideoLoop();

	void ReleaseVideoLoop();
	//初始化线圈
	void InitCoil();

	void ReleaseCoil();

	void InitRadar();

	void ReleaseRadar();

	void InitRedLampSig();

	void ReleaseRedLampSig();

	void InitRecogArea();

	void ReleaseRecogArea();

	void InitRedLampIncrease();

	void ReleaseRedLampIncrease();
	//初始化数据
	void InitData(bool is);

	void OnSnap();

	void OnSave();

	void OnRecogAreaSetListTipCtrl();

	void RecogAreaUpdateListItem(int nIndex, int nEUpdateItem);

	void RedLampIncreaseUpdateListItem(int nIndex, int nEUpdateItem);

	void RecogAreaUpdateDraw(int nIndex, int nEUpdateItem);

	void RedLampIncreaseUpdateDraw(int nIndex, int nEUpdateItem);

	void ChangeMethod(TIPC_VidLoopCfg *ptLoop, int nIndex);

	void OnObserverNotify(LPARAM lHint, LPVOID pHint);

	void UpdateData(bool bUpdate);

	void VideoLoopUpdateListItem(int nIndex, int nEUpdateItem);
	
	void DrawDataToList();

	void VideoLoopUpdate();

	void CoilLoopUpdate();

	void RedarLoopUpdate();

	void RedLampSigUpdate();
	
	void RecogAreaUpdate();

	void RedLampIncreaseUpdate();

	void OnSetListTipCtrl();

	void OnChangeScenceNum();

	void OnChangePreset(int nPreset);

	void RecogAreaConvert1(TIPC_SetRecogAreaReq *ptIPC_SetRecogAreaReq, recogarea::TRecogAreaParam *ptRecogAreaParam);

	void RecogAreaConvert2(recogarea::TRecogAreaParam *ptRecogAreaParam, TIPC_SetRecogAreaReq *ptIPC_SetRecogAreaReq);

	void OnVideoLoopSetListTipCtrl();

	void OnCoilLoopSetListTipCtrl();

	bool GetParamVideoLoopInfo();

	void SetParamVideoLoopInfo();

	bool GetParamRecogAreaInfo();

	void SetParamRecogAreaInfo();

	bool GetParamScenceInfo();

	bool GetParamCoilLoopInfo();

	void SetParamCoilLoopInfo();

	bool GetParamRedarLoopInfo();

	void SetParamRedarLoopInfo();

	bool GetParamRedLampSigInfo();

	void SetParamRedLampSigInfo();

	bool GetParamRedEnhanceInfo();

	void SetParamRedEnhanceInfo();

	LRESULT OnRefreshPic(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void VideoLoopUpdateDraw(int nIndex, int nEUpdateItem);

	void RedarLoopUpdateDraw(int nIndex);

	void OnSwitchScene();
public slots:
	void Slot_Radar_AreaSelectClicked();
	
	void Slot_VideoLoopDataChanged();

	void Slot_TabWidgetCurrentChanged(int nTabIndex);

	void Slot_ScenceInfoChanged();

	void Slot_RecogAreaInfoChanged();

	void Slot_RedarLoopChanged();

	void Slot_CoilLoopChanged();
	
	void Slot_RedSigLoopChanged();

	void Slot_ReadEnhloopDataChanged();

	void Slot_CreateLoopChanged();

	void Slot_TreeVideoWidgetItemClicked(QTreeWidgetItem *pItem, int column);

	void Slot_TreeCoilWidgetItemClicked(QTreeWidgetItem*pItem, int column);

	void Slot_TreeRadarWidgetItemClicked(QTreeWidgetItem*pItem, int column);

	void Slot_TreeRedLampSigWidgetItemClicked(QTreeWidgetItem *pItem, int column);

	void Slot_TreeRedLampIncreaseWidgetItemClicked(QTreeWidgetItem *pItem, int column);

	void Slot_TreeRecogAreaWidgetItemClicked(QTreeWidgetItem *pItem, int column);

	void Slot_DataDrawChanged();

	void Slot_VideoDataChanged(int nRow, int nColumn);

	void Slot_BtnGenerator();

	void Slot_CoilDataChanged(int nRow, int nColumn);

	void Slot_RadarDataChanged(int nRow, int nColumn);

	void Slot_RedLampSigDataChanged(int nRow, int nColumn);

	void Slot_RecogAreaDataChanged(int nRow, int nColumn);
	
	void Slot_RedLampIncreaseDataChanged(int nRow, int nColumn);//红灯增强

	void Slot_ScenceIndexChanged(int nIndex);

	void Slot_LoadSceneClicked();

	void Slot_BtnSaveClicked();

	void Slot_BtnFreshClicked();

	void Slot_VideoLoopSelectArea();

	void Slot_RedLampSigItemButtonClicked();

	void Slot_RedLampItemButtonClicked(); //红灯增强

signals:
	void signal_videoLoopChanged();

	void signal_scenceinfo();

	void signal_recogAreaInfo();

	void signal_redarLoop();

	void signal_coilLoop();

	void signal_redSigLoop();

	void signal_redEnhloop();

	void signal_createLoop();

private:
	Ui::Dlg_Detection ui;
	Dlg_MessageBox					*m_messageBox;
	TIPC_SetVideoLoopReq		    m_tVideoLoopInfo;
	TIPC_GetSceneCfgRsp			    m_tSeceneCfg;
	TIPC_SetRecogAreaReq		    m_tRecogAreaInfo;
	TIPC_SetCoilLoopReq             m_tCoilLoopInfo;
	TIPC_SetRadarLoopReq            m_tRedarLoopInfo;
	TIPC_SetRedLampSigReq           m_tRedLampSigInfo;
	recogarea::TRecogAreaParam      m_tRecogAreaParam[ROAD_NUM_MAX];
	TIPC_SetRedEnReq                m_tSetRedEnInfo;

	bool							m_bTrapezium;
	int								m_nTabIndex;
	int								m_nSceneIndex;
	bool							m_bChange;

	//视频检测
	LabelDelegate					*m_labDelegate;
	CheckBoxDelegate				*m_cheDelegate;
	EditDelegate					*m_editDelegate2;
	EditDelegate					*m_editDelegate3;
	EditDelegate					*m_editDelegate4;
	EditDelegate					*m_editDelegate5;
	EditDelegate					*m_editDelegate6;
	EditDelegate					*m_editDelegate7;
	EditDelegate					*m_editDelegate8;
	EditDelegate					*m_editDelegate9;
	EditDelegate					*m_editDelegate10;
	EditDelegate					*m_editDelegate11;

	EditDelegate					*m_editDelegate15;
	EditDelegate					*m_editDelegate16;
	EditDelegate					*m_editDelegate17;
	EditDelegate					*m_editDelegate18;
	EditDelegate					*m_editDelegate19;
	EditDelegate					*m_editDelegate20;

	ComboboxDelegate				*m_comboDelegate12;
	ComboboxDelegate				*m_comboDelegate13;
	ComboboxDelegate				*m_comboDelegate14;
	QMap<int, QString>				m_map1;
	QMap<int, QString>				m_map2;
	QMap<int, QString>				m_map3;

	//线圈检测器
	LabelDelegate					*m_coil_labDelegate;
	CheckBoxDelegate				*m_coil_chkDelegate;
	ComboboxDelegate				*m_coil_cmbDelegate2;
	ComboboxDelegate				*m_coil_cmbDelegate3;
	ComboboxDelegate				*m_coil_cmbDelegate4;
	EditDelegate					*m_coil_editDelegate5;
	EditDelegate					*m_coil_editDelegate6;
	ComboboxDelegate				*m_coil_cmbDelegate7;
	ComboboxDelegate				*m_coil_cmbDelegate8;
	ComboboxDelegate				*m_coil_cmbDelegate9;
	QMap<int, QString>				m_coil_serial_map;
	QMap<int, QString>				m_coil_way_map;//线圈通道号
	QMap<int, QString>				m_coil_direction_map;//方向
	QMap<int, QString>				m_coil_io_map;
	QMap<int, QString>				m_coil_lane_map;
	QMap<int, QString>				m_coil_coilType_map;

	//雷达检测器
	LabelDelegate					*m_radar_labDelegate;
	CheckBoxDelegate				*m_radar_chkDelegate;
	ComboboxDelegate				*m_radar_cmbDelegate2;
	ComboboxDelegate				*m_radar_cmbDelegate3;
	EditDelegate					*m_radar_editDelegate4;
	EditDelegate					*m_radar_editDelegate5;
	ComboboxDelegate				*m_radar_cmbDelegate6;

	//红灯信号检测
	LabelDelegate					*m_redLampSig_labDelegate;
	CheckBoxDelegate				*m_redLampSig_chkDelegate;
	ComboboxDelegate				*m_redLampSig_cmbDelegate2;
	ComboboxDelegate				*m_redLampSig_cmbDelegate3;

	//识别区域
	LabelDelegate					*m_recogArea_labDelegate;
	CheckBoxDelegate				*m_recogArea_chkDelegate;
	EditDelegate					*m_recogArea_editDelegate2;
	EditDelegate					*m_recogArea_editDelegate3;
	EditDelegate					*m_recogArea_editDelegate4;
	EditDelegate					*m_recogArea_editDelegate5;
	EditDelegate					*m_recogArea_editDelegate6;
	EditDelegate					*m_recogArea_editDelegate7;
	EditDelegate					*m_recogArea_editDelegate8;
	ComboboxDelegate				*m_recogArea_cmbDelegat9;
	ComboboxDelegate				*m_recogArea_cmbDelegat10;
	QMap<int, QString>				m_recogArea_map1;
	QMap<int, QString>				m_recogArea_map2;


	//红灯增强
	
	CheckBoxDelegate				*m_redLampIncrease_chkDelegate;
	EditDelegate					*m_redLampIncrease_editDelegate;
	EditDelegate					*m_redLampIncrease_editDelegate2;
	EditDelegate					*m_redLampIncrease_editDelegate3;
	EditDelegate					*m_redLampIncrease_editDelegate4;
	EditDelegate					*m_redLampIncrease_editDelegate5;
	EditDelegate					*m_redLampIncrease_editDelegate6;
	EditDelegate					*m_redLampIncrease_editDelegate7;
};
