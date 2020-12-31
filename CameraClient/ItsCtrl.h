#pragma once
#include "stdafx.h"
#include "ivhs_struct.h"
#include "ivhs_sdk.h"
#include "ivhs_struct.h"
#include "kdvtype.h"
#include <QString>
#include <QSemaphore>
#include <QObject>
class ItsCtrl
{
public:
	ItsCtrl(void* pData);

	~ItsCtrl();

	void RefreshReceive();

	bool StartReceive();

	bool StopReceive();

	bool Connect();

	void setParam(u32 sIp, Uint16 sport, QString sUsr, QString sPwd);

	bool Disconnect();

	bool Login();

	bool isLogin(){ return m_bLogin; }

	bool IsConnect(){ return m_dwHandle == nullptr ? false : true; }
	
	void Release();

	void LoginAck(TItsMessage *ptMsg);

	static void IvhsCallBack(TItsMessage *ptMsg);

	bool PostMsg(u32 dwEvtType, void* szBodyIn, u32 dwBodyLen, u32 dwContex = 0);

	KOSA_HANDLE getCurHandle(){ return m_dwHandle; }

	void ProData(TItsMessage *ptMsg);

	u32	getIP(){ return m_sIp; }

	static ItsCtrl* GetItsCtrlFromHandle(KOSA_HANDLE dwHandle);
	//IPC�豸��Ϣ
	bool SetIpcDevInfo(TIPC_DevInfo* pData);

	bool GetIpcDevInfo(void* pData, u32 dwContex);

	void OnGetIpcDevInfoAck(TItsMessage *ptMsg);

	//����OSDͼƬ
	bool  SetOsdPic(TIPC_OsdImgData *pData);

	//�����豸
	void Restart();

	//�򵥻ָ�
	void Reset();

	//���������ļ�;
	void ExportCfg(void* pData, u32 dwContex);
	void OnGetExportCfgAck(TItsMessage *ptMsg);

	//���������ļ�;
	void ImportCfg(TDevAllConfig* pData, u32 dwContex);
	void OnImportCfgAck(TItsMessage *ptMsg);

	bool AddDevice(TITS_DevList *pDevList);
	bool DelDevice(TITS_DevList *pDevList);
	bool GetDevInfo(u32 dwContex = 0);
	void OnGetDevListAck(TItsMessage *ptMsg);

	bool PostTransparentMsg(void *addr, u32 len, u32 msgType, u32 dwContex);

	// ��Ƶ����
	bool SetIpcVelometer(TIPC_VidSpdDet* pData);
	bool GetIpcVelometer(void* pData, u32 dwContex = 0);
	void OnGetIpcVelometerAck(TItsMessage *ptMsg);

	// ��Դͬ��
	bool SetIpcPowerSyn(TIPC_PowerPhaseSync* pData);
	bool GetIpcPowerSyn(void* pData, u32 dwContex = 0);
	void OnGetIpcPowerSynAck(TItsMessage *ptMsg);

	// ipc ��������
	bool SetIpcSerialCfg(TIPC_SerialCfg *pData);
	bool GetIpcSerialCfg(void* pData, u32 dwContex = 0);
	void OnGetIpcSerialCfgAck(TItsMessage *ptMsg);

	// ipc �ź�����
	bool SetIpcSignalProtocolCfg(TIPC_Signal *pData);
	bool GetIpcSignalProtocolCfg(void* pData, u32 dwContex = 0);
	void OnGetIpcSignalProtocolCfgAck(TItsMessage *ptMsg);

	// ipc ��Ϣͬ������
	bool SetIpcInfoSyncCfg(TIPC_InfoSyncCfg *pData);
	bool GetIpcInfoSyncCfg(void* pData, u32 dwContex = 0);
	void OnGetIpcInfoSyncCfgAck(TItsMessage *ptMsg);
	// Υ���ֵ�
	bool SetIpcIllegalDict(TIPC_IllegalDict* pData);
	bool GetIpcIllegalDict(void* pData, u32 dwContex = 0);
	void OnGetIpcIllegalDictAck(TItsMessage *ptMsg);

	// ƽ̨����
	bool SetSysCfgPlate(TITS_SetServerReq* pData);
	bool GetSysCfgPlate(TITS_GetServerRsp* pData, u32 dwContex = 0);
	void OnGetSysCfgPlateAck(TItsMessage *ptMsg);

	//����LED ���������
	bool GetExtDevType(u32 dwContex);
	bool SetExtDevType(TIvhsExtDevType* pData);
	void OnExtDevTypeAck(TItsMessage *ptMsg);


	//��ѯ������¼
	bool QuerySdRecord(void * pData, u32 dwContex);
	void OnQuerySdRecordAck(TItsMessage *ptMsg);

	//��ȡ������¼
	bool GetSdRecord(void* pData, u32 dwContex);
	void OnGetSdRecordAck(TItsMessage *ptMsg);

	// ipc ����������
	bool SetIpcLightBrightness(TIPC_LightParam* pData);
	bool GetIpcLightBrightness(void* pData, u32 dwContex);
	void OnGetIpcLightBrightnessAck(TItsMessage *ptMsg);

	// ƽ̨ͼƬOSD���Ӳ���
	bool SetSysCfgOsd(TOsdCfgInfo* pData);
	bool GetSysCfgOsd(u32* pData, u32 dwContex = 0);
	void OnGetSysCfgOsdAck(TItsMessage *ptMsg);

	// ƽ̨OSD�������
	bool SetSysCfgOsdFont(TOsdFontCfg* pData);
	bool GetSysCfgOsdFont(u32* pData, u32 dwContex = 0);
	void OnGetSysCfgOsdFontAck(TItsMessage *ptMsg);

	// ipc ͼƬ���
	bool DebugMode();
	void OnDebugModeAck(TItsMessage *ptMsg);

	// ipc evt
	bool IpcEvt_Snap(int mode = 0);

	// ipc ������Ϣ
	bool SetIpcDebugParam(TIPC_DebugParam* pData);
	bool GetIpcDebugParam(void* pData, u32 dwContex);
	void  OnGetIpcDebugParamAck(TItsMessage *ptMsg);

	// ipc ��������
	bool SetIpcScence(TIPC_SetSceneCfgReq* pData);
	bool GetIpcScence(void* pData, u32 dwContex = 0);
	void OnGetIpcScenceAck(TItsMessage *ptMsg);

	// ipc Υͣ��������
	bool SetIpcParkScence(TIPC_illgParkPara* pData);
	bool GetIpcParkScence(void* pData, u32 dwContex = 0);
	void OnGetIpcParkScenceAck(TItsMessage *ptMsg);

	// ipc Υͣ����
	bool SetIpcParkParam(TIPC_ParkParam* pData);
	bool GetIpcParkParam(void* pData, u32 dwContex = 0);
	void OnGetIpcParkParamAck(TItsMessage *ptMsg);

	//��ȡ����ID
	bool GetScenceID(void* pData, u32 dwContex);
	void OnGetScenceIDAck(TItsMessage *ptMsg);

	bool IpcPtz(TIPC_PTZCtrl* pData);

	// ʶ��������Ϣ��ȡ
	bool SetIpcRecogArea(TIPC_SetRecogAreaReq* pData);
	bool GetIpcRecogArea(void* pData, u32 dwContex = 0);
	void OnGetIpcRecogAreaAck(TItsMessage *ptMsg);

	// ipc ��Ƶ�����
	bool SetIpcVideoLoop(TIPC_SetVideoLoopReq* pData);
	bool GetIpcVideoLoop(void* pData, u32 dwContex = 0);
	void OnGetIpcVideoLoopAck(TItsMessage *ptMsg);

	// ��Ȧ�������Ϣ
	bool SetIpcCoilLoop(TIPC_SetCoilLoopReq* pData);
	bool GetIpcCoilLoop(void* pData, u32 dwContex);
	void OnGetIpcCoilLoopAck(TItsMessage *ptMsg);

	// �״�������Ϣ
	bool SetIpcRedarLoop(TIPC_SetRadarLoopReq* pData);
	bool GetIpcRedarLoop(void* pData, u32 dwContex);
	void OnGetIpcRedarLoopAck(TItsMessage *ptMsg);

	// ����źż������Ϣ
	bool SetIpcRedSigLoop(TIPC_SetRedLampSigReq* pData);
	bool GetIpcRedSigLoop(void* pData, u32 dwContex);
	void OnGetIpcRedSigLoopAck(TItsMessage *ptMsg);

	// �����ǿ�������Ϣ
	bool SetIpcRedEnhLoop(TIPC_SetRedEnReq* pData);
	bool GetIpcRedEnhLoop(void* pData, u32 dwContex);
	void OnGetIpcRedEnhLoopAck(TItsMessage *ptMsg);

	// ���ɼ����
	bool SetIpcCreateLoop(void* pData, u32 dwContex);
	void  OnSetIpcCreateLoopAck(TItsMessage *ptMsg);

	// �����߼�����
	bool BuildLogicLane(void* pData, u32 dwContex);
	void OnBuildLogicLaneAck(TItsMessage *ptMsg);

	bool SetSnapIllegalParking(TIPC_SnapIllegalPark* pData);

	// ͼ������
	bool SetIpcImageQuality(TIPC_KdISPCfg* pData);
	bool GetIpcImageQuality(void* pData, u32 dwContex = 0);
	void OnGetIpcImageQualityAck(TItsMessage *ptMsg);

	// ipc ������Ѳ����
	bool SetIpcScencePoll(TIPC_ScenePoll* pData, Uint32 number);
	bool GetIpcScencePoll(void* pData, u32 dwContex = 0);
	void OnGetIpcScencePollAck(TItsMessage *ptMsg);

	// ipc �߼�����
	bool SetIpcLogicLane(TIPC_SetLogicLaneReq* pData);
	bool GetIpcLogicLane(void* pData, u32 dwContex = 0);
	void OnGetIpcLogicLaneAck(TItsMessage *ptMsg);

	// �ϳ�ͼƬ����
	bool SetIpcMergeParam(TIPC_TImgMergePara* pData);
	bool GetIpcMergeParam(void* pData, u32 dwContex = 0);
	void OnGetIpcMergeParamAck(TItsMessage *ptMsg);

	// ʶ�����
	bool SetIpcIdent(TIPC_RecogParam* pData);
	bool GetIpcIdent(void* pData, u32 dwContex = 0);
	void OnGetIpcIdentAck(TItsMessage *ptMsg);

	//  UI ��������ȡ
	bool GetUICap(u32 dwContex);
	void OnGetUICapAck(TItsMessage *ptMsg);

private:
	u32 m_sIp;
	QString m_sUsr;
	QString m_sPwd;
	Uint16 m_port;
	bool m_bLogin;
	KOSA_HANDLE m_dwHandle; //��ǰ��¼��
	HANDLE		m_sem;
	QSemaphore  *m_semaphore;

	bool m_bRecv;

};

namespace ITSCTRL
{
	void SetIsIvhsInit(bool bInit);
	bool IsIvhsInit();
	void MakeMsgParam(TItsMessage* pMsgOut, Uint32 dwMsgType, void* szBody, Uint32 nBodyLen, Uint32 dwContex, ItsCtrl* pCtrl);
}

