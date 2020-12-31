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
	//IPC设备信息
	bool SetIpcDevInfo(TIPC_DevInfo* pData);

	bool GetIpcDevInfo(void* pData, u32 dwContex);

	void OnGetIpcDevInfoAck(TItsMessage *ptMsg);

	//传输OSD图片
	bool  SetOsdPic(TIPC_OsdImgData *pData);

	//重启设备
	void Restart();

	//简单恢复
	void Reset();

	//导出配置文件;
	void ExportCfg(void* pData, u32 dwContex);
	void OnGetExportCfgAck(TItsMessage *ptMsg);

	//导入配置文件;
	void ImportCfg(TDevAllConfig* pData, u32 dwContex);
	void OnImportCfgAck(TItsMessage *ptMsg);

	bool AddDevice(TITS_DevList *pDevList);
	bool DelDevice(TITS_DevList *pDevList);
	bool GetDevInfo(u32 dwContex = 0);
	void OnGetDevListAck(TItsMessage *ptMsg);

	bool PostTransparentMsg(void *addr, u32 len, u32 msgType, u32 dwContex);

	// 视频测速
	bool SetIpcVelometer(TIPC_VidSpdDet* pData);
	bool GetIpcVelometer(void* pData, u32 dwContex = 0);
	void OnGetIpcVelometerAck(TItsMessage *ptMsg);

	// 电源同步
	bool SetIpcPowerSyn(TIPC_PowerPhaseSync* pData);
	bool GetIpcPowerSyn(void* pData, u32 dwContex = 0);
	void OnGetIpcPowerSynAck(TItsMessage *ptMsg);

	// ipc 串口配置
	bool SetIpcSerialCfg(TIPC_SerialCfg *pData);
	bool GetIpcSerialCfg(void* pData, u32 dwContex = 0);
	void OnGetIpcSerialCfgAck(TItsMessage *ptMsg);

	// ipc 信号配置
	bool SetIpcSignalProtocolCfg(TIPC_Signal *pData);
	bool GetIpcSignalProtocolCfg(void* pData, u32 dwContex = 0);
	void OnGetIpcSignalProtocolCfgAck(TItsMessage *ptMsg);

	// ipc 信息同步配置
	bool SetIpcInfoSyncCfg(TIPC_InfoSyncCfg *pData);
	bool GetIpcInfoSyncCfg(void* pData, u32 dwContex = 0);
	void OnGetIpcInfoSyncCfgAck(TItsMessage *ptMsg);
	// 违法字典
	bool SetIpcIllegalDict(TIPC_IllegalDict* pData);
	bool GetIpcIllegalDict(void* pData, u32 dwContex = 0);
	void OnGetIpcIllegalDictAck(TItsMessage *ptMsg);

	// 平台参数
	bool SetSysCfgPlate(TITS_SetServerReq* pData);
	bool GetSysCfgPlate(TITS_GetServerRsp* pData, u32 dwContex = 0);
	void OnGetSysCfgPlateAck(TItsMessage *ptMsg);

	//设置LED 闪光灯属性
	bool GetExtDevType(u32 dwContex);
	bool SetExtDevType(TIvhsExtDevType* pData);
	void OnExtDevTypeAck(TItsMessage *ptMsg);


	//查询过车记录
	bool QuerySdRecord(void * pData, u32 dwContex);
	void OnQuerySdRecordAck(TItsMessage *ptMsg);

	//获取过车记录
	bool GetSdRecord(void* pData, u32 dwContex);
	void OnGetSdRecordAck(TItsMessage *ptMsg);

	// ipc 灯亮度配置
	bool SetIpcLightBrightness(TIPC_LightParam* pData);
	bool GetIpcLightBrightness(void* pData, u32 dwContex);
	void OnGetIpcLightBrightnessAck(TItsMessage *ptMsg);

	// 平台图片OSD叠加参数
	bool SetSysCfgOsd(TOsdCfgInfo* pData);
	bool GetSysCfgOsd(u32* pData, u32 dwContex = 0);
	void OnGetSysCfgOsdAck(TItsMessage *ptMsg);

	// 平台OSD字体参数
	bool SetSysCfgOsdFont(TOsdFontCfg* pData);
	bool GetSysCfgOsdFont(u32* pData, u32 dwContex = 0);
	void OnGetSysCfgOsdFontAck(TItsMessage *ptMsg);

	// ipc 图片浏览
	bool DebugMode();
	void OnDebugModeAck(TItsMessage *ptMsg);

	// ipc evt
	bool IpcEvt_Snap(int mode = 0);

	// ipc 调试信息
	bool SetIpcDebugParam(TIPC_DebugParam* pData);
	bool GetIpcDebugParam(void* pData, u32 dwContex);
	void  OnGetIpcDebugParamAck(TItsMessage *ptMsg);

	// ipc 场景配置
	bool SetIpcScence(TIPC_SetSceneCfgReq* pData);
	bool GetIpcScence(void* pData, u32 dwContex = 0);
	void OnGetIpcScenceAck(TItsMessage *ptMsg);

	// ipc 违停场景配置
	bool SetIpcParkScence(TIPC_illgParkPara* pData);
	bool GetIpcParkScence(void* pData, u32 dwContex = 0);
	void OnGetIpcParkScenceAck(TItsMessage *ptMsg);

	// ipc 违停配置
	bool SetIpcParkParam(TIPC_ParkParam* pData);
	bool GetIpcParkParam(void* pData, u32 dwContex = 0);
	void OnGetIpcParkParamAck(TItsMessage *ptMsg);

	//获取场景ID
	bool GetScenceID(void* pData, u32 dwContex);
	void OnGetScenceIDAck(TItsMessage *ptMsg);

	bool IpcPtz(TIPC_PTZCtrl* pData);

	// 识别区域信息获取
	bool SetIpcRecogArea(TIPC_SetRecogAreaReq* pData);
	bool GetIpcRecogArea(void* pData, u32 dwContex = 0);
	void OnGetIpcRecogAreaAck(TItsMessage *ptMsg);

	// ipc 视频检测器
	bool SetIpcVideoLoop(TIPC_SetVideoLoopReq* pData);
	bool GetIpcVideoLoop(void* pData, u32 dwContex = 0);
	void OnGetIpcVideoLoopAck(TItsMessage *ptMsg);

	// 线圈检测器信息
	bool SetIpcCoilLoop(TIPC_SetCoilLoopReq* pData);
	bool GetIpcCoilLoop(void* pData, u32 dwContex);
	void OnGetIpcCoilLoopAck(TItsMessage *ptMsg);

	// 雷达检测器信息
	bool SetIpcRedarLoop(TIPC_SetRadarLoopReq* pData);
	bool GetIpcRedarLoop(void* pData, u32 dwContex);
	void OnGetIpcRedarLoopAck(TItsMessage *ptMsg);

	// 红灯信号检测器信息
	bool SetIpcRedSigLoop(TIPC_SetRedLampSigReq* pData);
	bool GetIpcRedSigLoop(void* pData, u32 dwContex);
	void OnGetIpcRedSigLoopAck(TItsMessage *ptMsg);

	// 红灯增强检测器信息
	bool SetIpcRedEnhLoop(TIPC_SetRedEnReq* pData);
	bool GetIpcRedEnhLoop(void* pData, u32 dwContex);
	void OnGetIpcRedEnhLoopAck(TItsMessage *ptMsg);

	// 生成检测器
	bool SetIpcCreateLoop(void* pData, u32 dwContex);
	void  OnSetIpcCreateLoopAck(TItsMessage *ptMsg);

	// 生成逻辑车道
	bool BuildLogicLane(void* pData, u32 dwContex);
	void OnBuildLogicLaneAck(TItsMessage *ptMsg);

	bool SetSnapIllegalParking(TIPC_SnapIllegalPark* pData);

	// 图像质量
	bool SetIpcImageQuality(TIPC_KdISPCfg* pData);
	bool GetIpcImageQuality(void* pData, u32 dwContex = 0);
	void OnGetIpcImageQualityAck(TItsMessage *ptMsg);

	// ipc 场景轮巡配置
	bool SetIpcScencePoll(TIPC_ScenePoll* pData, Uint32 number);
	bool GetIpcScencePoll(void* pData, u32 dwContex = 0);
	void OnGetIpcScencePollAck(TItsMessage *ptMsg);

	// ipc 逻辑车道
	bool SetIpcLogicLane(TIPC_SetLogicLaneReq* pData);
	bool GetIpcLogicLane(void* pData, u32 dwContex = 0);
	void OnGetIpcLogicLaneAck(TItsMessage *ptMsg);

	// 合成图片参数
	bool SetIpcMergeParam(TIPC_TImgMergePara* pData);
	bool GetIpcMergeParam(void* pData, u32 dwContex = 0);
	void OnGetIpcMergeParamAck(TItsMessage *ptMsg);

	// 识别参数
	bool SetIpcIdent(TIPC_RecogParam* pData);
	bool GetIpcIdent(void* pData, u32 dwContex = 0);
	void OnGetIpcIdentAck(TItsMessage *ptMsg);

	//  UI 能力集获取
	bool GetUICap(u32 dwContex);
	void OnGetUICapAck(TItsMessage *ptMsg);

private:
	u32 m_sIp;
	QString m_sUsr;
	QString m_sPwd;
	Uint16 m_port;
	bool m_bLogin;
	KOSA_HANDLE m_dwHandle; //当前登录的
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

