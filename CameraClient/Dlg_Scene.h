#pragma once

#include <QWidget>
#include "ui_Dlg_Scene.h"
#include <subject.h>
#include "ivhs_struct.h"
#include "define.h"
#include <QMap>
#include <QPushButton>
#include <QSignalMapper>
#include <QButtonGroup>
#include "MyWidget.h"
#include "Dlg_MessageBox.h"
#define GBUTF8(da) QString::fromLocal8Bit(da)
#define  SCENCE_NUM  10

// ��������Checkbuttonö��
enum EMSCENECTRLCHK
{
	EMSCENECTRLCHK_CHKBSD_F1 = 0,	// ����������
	EMSCENECTRLCHK_CHKBSD_F2,		// 
	EMSCENECTRLCHK_CHKBSD_F3,		// 
	EMSCENECTRLCHK_CHKBSD_F4,		// 

	EMSCENECTRLCHK_CHKLED_NEI,		// ����LED��
	EMSCENECTRLCHK_CHKLED_WAI1,		// 
	EMSCENECTRLCHK_CHKLED_WAI2,		// 

	EMSCENECTRLCHK_CHKTORIGHT,		// ��������
	EMSCENECTRLCHK_CHKTOLINE,		//
	EMSCENECTRLCHK_CHKTOLEFT,		// 
	EMSCENECTRLCHK_CHKTOTRUN,		// 

	EMSCENECTRLCHK_CHKSANP_0,		// ����
	EMSCENECTRLCHK_CHKSANP_1,		// ����
	EMSCENECTRLCHK_CHKSANP_3,	    // �����涨������ʻ
	EMSCENECTRLCHK_CHKSANP_6,		// ʵ�߱��
	EMSCENECTRLCHK_CHKSANP_8,		// �����
	EMSCENECTRLCHK_CHKSANP_9,		// �������򳵵���ʻ
	EMSCENECTRLCHK_CHKSANP_10,		// ��ֹ��ת
	EMSCENECTRLCHK_CHKSANP_12,		// ����
	EMSCENECTRLCHK_CHKSANP_13,		// ѹʵ��
	EMSCENECTRLCHK_CHKSANP_14,		// Υ����ͷ
	EMSCENECTRLCHK_CHKSANP_17,		// Խ��ͣ��
	EMSCENECTRLCHK_CHKSANP_18,		// ·������
	EMSCENECTRLCHK_CHKSANP_19,		// ������ͣ��
	EMSCENECTRLCHK_CHKSANP_20,		// ������δ��������
	EMSCENECTRLCHK_CHKSANP_21,		// ����·Υ������
	EMSCENECTRLCHK_CHKSANP_22,		// ת��δ��ֱ��
	EMSCENECTRLCHK_CHKSANP_23,		// ���˻�ǻ����������
	EMSCENECTRLCHK_CHKSANP_24,		// δ���涨�����г���ȫ����
	EMSCENECTRLCHK_CHKSANP_25,		// ˾��Υ��
	EMSCENECTRLCHK_CHKSANP_26,		// ��������
	EMSCENECTRLCHK_CHKSANP_27,		// Υ������
	EMSCENECTRLCHK_CHKSANP_1_88,	// ����	

	EMSCENECTRLCHK_CHKSANP_1_0,		// ������ռ�÷ǻ�������
	EMSCENECTRLCHK_CHKSANP_1_1,		// ռ��Ӧ������
	EMSCENECTRLCHK_CHKSANP_1_2,		// ռ�ù�������
	EMSCENECTRLCHK_CHKSANP_1_3,		// ���ͳ�ռ��С�ͳ�����
	EMSCENECTRLCHK_CHKSANP_1_4,		// ��ֹ��������ͨ��
	EMSCENECTRLCHK_CHKSANP_1_5,		// ռ�ù�������
	EMSCENECTRLCHK_CHKSANP_1_6,		// �ǻ�����ռ�û�������
	EMSCENECTRLCHK_CHKSANP_1_7,		// �ǻ���������
	EMSCENECTRLCHK_CHKSANP_1_10,	// �ǻ�������װɡ��

	EMSCENECTRLCHK_CHKCPY_LANE1,			//
	EMSCENECTRLCHK_CHKCPY_LANE2,			// 
	EMSCENECTRLCHK_CHKCPY_LANE3,			// 
	EMSCENECTRLCHK_CHKCPY_LANE4,			// 

	EMSCENECTRLCHK_COUNT
};

class Dlg_Scene : public MyWidget, public CObserver
{
	Q_OBJECT

public:
	Dlg_Scene(QWidget *parent = Q_NULLPTR);

	~Dlg_Scene(); 

	void OnSetTxtVisible();

	void OnChangeLaneNum();

	void OnChangeAreaNum();

	void Init();

	void InitData(bool is);

	bool CheckCarProperty();

	void OnSnap();

	void OnChangePicType(int type);

	void OnShowBayonet(bool bShow);

	void OnShowParkMode(bool bShow);

	void OnSwitchPage(int index, bool isSave = false);

	void OnSwitchIndex(int index, bool isSave = false);

	void OnChangePreset(int nPreset);

	void OnObserverNotify(LPARAM lHint, LPVOID pHint);

	void GetParamDevInfo();

	bool GetParamScence();

	bool GetParkParam();

	void GetScenceID();

	bool GetParkScence();

	void SetParkParam();

	void SetParamScence();

	void SetParkScence();

	void OnUpdateData(bool bUpdate);

	void OnSave();

	void OnSavePtzPos();

	//bUpdateΪfalse������µ�����
	void UpdateScene(bool bUpdate);

	void UpdatePark(bool bUpdate);

	void UpdateParkParam(bool bUpdate);

	void UpdateDevInfo(bool bUpdate);

	void OnSwitchScene();

	void OnShowPark(bool bShow);

	void OnShowPtz(bool bShow);

	void SetParkScenceDrawData(TIPC_illgParkPara *ptParkScence);

	bool	GetExtDevType();

	void	SetExtDevType();

	void	SetExtDevVisible(bool isShow);

	void	OnSaveExtType();

	QPushButton* GetClickedButton();
public slots:
	void Slot_BayonentChanged();

	void Slot_CmbSceneIndexChanged(int nIndex);

	void Slot_PlateNumChanged(int nIndex);

	void Slot_LaneAndAreaClicked();

	void Slot_TabCurrentChanged();

	void Slot_DeviceInfoChanged();

	void Slot_ParkScenceChanged();

	void Slot_ParkParamChanged();

	void Slot_BtnSaveClicked();

	void Slot_BtnFreshClicked();

	void Slot_BtnSelectPosClicked();

	void Slot_PTZControlClicked();

	void Slot_PTZStopClicked();

	void Slot_CmbPictureIndexChanged(int nIndex);

	void Slot_CmbAreaSumIndexChanged(int nIndex);

	void Slot_BtnCopyClicked();

	void Slot_BtnDrawAreaClicked();

	void Slot_BtnManualCaptureClicked();

	void Slot_ExtDevTypeChanged();
signals:
		void signal_bayonetChanged();

		void signal_devInfoChanged();

		void signal_parkScenceChanged();

		void signal_parkParamChanged();

		void signal_extDevTypeChanged();
private:
	u32 AnalyIndex2LightMode(int nIndex);

	int	AnalyLightMode2Index(u32 dwMode);

	u32 AnalyIndex2CheckMode(int nIndex);

	int	AnalyCheckMode2Index(u32 dwMode);

	u32 SetSpecifyBitValue(Uint32& dwSrcOut, Uint32 dwValue, int nIndex);

	int GetSpecifyBitValue(Uint32 dwSrc, int nIndex);

	bool SetChkValue(void* pData, QCheckBox* pChkBtn, int em, int nIndex);

	bool GetChkValue(void* pData, QCheckBox* pChkBtn, int em, int nIndex);
private:
	Ui::Dlg_Scene ui;
	TIPC_GetSceneCfgRsp  m_tSeceneCfg;
	TIPC_illgParkPara        m_tIPC_illgParkPara;
	TIPC_DevInfo	m_tDevInfo;
	TLocalCfgInfo			 tLocalCfgInfo;
	TIPC_ParkParam           m_tParkParam;
	TIvhsExtDevType			 m_extDevType;
	bool m_bSupportPark;
	int  m_nSceneIndex;
	int	 m_nShowType;
	int  m_nIndex;
	bool m_bSupport;//��Υͣģʽ����һ�ε�¼��������λΥ��ģʽTabҳ�Ŀ���
	QSignalMapper *m_signalButton;
	QMap<int, QPushButton*> m_mapLane; //����
	QMap<int, QCheckBox*> m_mapChkLane; //����


	QMap<int, QPushButton*> m_mapArea;
	QMap<int, QCheckBox*> m_mapChkArea;

	QVector<QCheckBox*> m_chkBayonetAll;
	QButtonGroup *m_buttonGroup;
	QButtonGroup *m_buttonGroupTab;

	Dlg_MessageBox *m_messageBox;
};
