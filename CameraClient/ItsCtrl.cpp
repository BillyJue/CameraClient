#include "stdafx.h"
#include "ItsCtrl.h"
#include <map>
#include <QDebug>
static std::map<KOSA_HANDLE, ItsCtrl*>* m_pmapHandleAndData = NULL;
bool g_bIvhsInit = 0;
Uint32 g_nMsgId = 1;
MgrData *m_pMgrData = nullptr;
ItsCtrl::ItsCtrl(void* pData) : m_bRecv(false)
{
	//m_semaphore = new QSemaphore(1);
	//m_semaphore->acquire(1); //先占用
	m_sem = CreateSemaphore(NULL, 0, 1, NULL);
	m_pMgrData = (MgrData *)pData;
	if (m_pmapHandleAndData == NULL)
	{
		m_pmapHandleAndData = new std::map<KOSA_HANDLE, ItsCtrl*> ;
	}
	m_pmapHandleAndData->clear();
}


ItsCtrl::~ItsCtrl()
{
	m_pmapHandleAndData->clear();
	Release();
	CloseHandle(m_sem);
	//SAFE_DELETE(m_semaphore);
	/*if (m_semaphore)
	{
		delete m_semaphore;
		m_semaphore = nullptr;
	}*/
}

void ItsCtrl::setParam(u32 sIp, Uint16 port, QString sUsr, QString sPwd)
{
	m_sIp = sIp;
	m_port = port;
	m_sUsr = sUsr;
	m_sPwd = sPwd;
}


bool ItsCtrl::Connect()
{
	KOSA_HANDLE dwHandle = IvhsFwdConnect(m_sIp, m_port);
	if (!dwHandle)
	{
		return false;
	}
	m_dwHandle = dwHandle;
	m_pmapHandleAndData->insert(std::make_pair(m_dwHandle, this));
	return true;
	/*QByteArray ipArr = m_sIp.toLatin1();
	char *cIp = ipArr.data();

	QByteArray usrArr = m_sUsr.toLatin1();
	char *cUsr = usrArr.data();

	QByteArray pwdArr = m_sPwd.toLatin1();
	char *cPwd = pwdArr.data();
	UINT32 error;

	KOSA_HANDLE dwHandle = IvhsConnectIPC(cIp, cUsr, cPwd, &error);

	if (dwHandle == 0)
	{
	   dwHandle = IvhsConnectIPC(cIp, cUsr, cPwd, &error);
	}

	if (dwHandle)
	{
		m_pmapHandleAndData->insert(std::make_pair(dwHandle, this));
		return true;
	}

	return false;
	*/
}

bool ItsCtrl::Disconnect()
{
	// 断开连接
	if (m_dwHandle != 0)
	{
		std::map<KOSA_HANDLE, ItsCtrl*>::iterator ite = m_pmapHandleAndData->find(m_dwHandle);
		if (ite != m_pmapHandleAndData->end())
		{
			m_pmapHandleAndData->erase(ite);
		}
		u32 nRet = IvhsDisconnect(m_dwHandle);
		m_dwHandle = 0;
	}

	return true;
}

namespace ITSCTRL
{
	void SetIsIvhsInit(bool bInit){ g_bIvhsInit = bInit; }
	bool IsIvhsInit(){ return g_bIvhsInit; }

	void MakeMsgParam(TItsMessage* pMsgOut, Uint32 dwMsgType, void* szBody, Uint32 dwBufLen, Uint32 dwContex, ItsCtrl* pCtrl)
	{
		memset(pMsgOut, 0, sizeof(pMsgOut));
		pMsgOut->dwHandle = pCtrl->getCurHandle();
		pMsgOut->dwContex = dwContex;
		pMsgOut->wMsgType = dwMsgType;
		pMsgOut->dwSrcSeqId = g_nMsgId++;
		pMsgOut->dwSrcIP = pCtrl->getIP();
		pMsgOut->szMsgBody = (char*)szBody;
		pMsgOut->dwMsgLen = dwBufLen;
	}
}

// ipc 设备信息
bool ItsCtrl::SetIpcDevInfo(TIPC_DevInfo* pData)
{
	return PostMsg(EV_SET_IPC_DEV_INFO_REQ, pData, sizeof(TIPC_DevInfo));
}

bool ItsCtrl::GetIpcDevInfo(void* pData, u32 dwContex)
{
	bool bRet = PostMsg(EV_GET_IPC_DEV_INFO_REQ, NULL, 0, dwContex);

	return bRet;
}

void  ItsCtrl::OnGetIpcDevInfoAck(TItsMessage *ptMsg)
{
	CObserver* pObserver = (CObserver*)ptMsg->dwContex;
	m_pMgrData->Notify(pObserver, (LPARAM)NMC_GET_IPC_DEVINFO, ptMsg->szMsgBody);
}

//重启
void ItsCtrl::Restart()
{
	PostMsg(EV_COMM_DEV_REBOOT_CMD, NULL, 0);
}

//简单恢复
void ItsCtrl::Reset()
{
	PostMsg(EV_RESTORE_SETTINE_REQ, NULL, 0);
}

//获取配置文件
void ItsCtrl::ExportCfg(void* pData, u32 dwContex)
{
	PostMsg(EV_READ_CONFIG_REQ, NULL, 0, dwContex);
}
void  ItsCtrl::OnGetExportCfgAck(TItsMessage *ptMsg)
{
	CObserver* pObserver = (CObserver*)ptMsg->dwContex;
	m_pMgrData->Notify(pObserver, (LPARAM)NMC_GET_IPC_CONFIG, (ptMsg->szMsgBody));
}

//导入配置文件;
void ItsCtrl::ImportCfg(TDevAllConfig* pData, u32 dwContex)
{
	PostMsg(EV_IMPORT_CONFIG_REQ, pData, sizeof(TDevAllConfig), dwContex);
}

void ItsCtrl::OnImportCfgAck(TItsMessage *ptMsg)
{
}

bool ItsCtrl::AddDevice(TITS_DevList *pDevList)
{
	PostMsg(EV_ADD_DEVICE_REQ, pDevList, sizeof(TITS_DevList), 0);

	return true;
}

bool ItsCtrl::DelDevice(TITS_DevList *pDevList)
{
	return PostMsg(EV_DEL_DEVICE_REQ, pDevList, sizeof(TITS_DevList), 0);
}

bool ItsCtrl::GetDevInfo(u32 dwContex)
{
	return PostMsg(EV_GET_DEVLIST_CONFIG_REQ, NULL, 0, dwContex);
}

void ItsCtrl::OnGetDevListAck(TItsMessage *ptMsg)
{

	CObserver* pObserver = NULL;
	pObserver = (CObserver*)ptMsg->dwContex;
	m_pMgrData->Notify(pObserver, (LPARAM)EV_GET_DEVLIST_CONFIG_ACK, ptMsg->szMsgBody);
}


bool ItsCtrl::PostTransparentMsg(void *addr, u32 len, u32 msgType, u32 dwContex)
{
	TCommMsgData tCommMsg;
	Uint16 temp;

	memset(&tCommMsg, 0, sizeof(tCommMsg));

	temp = msgType;
	tCommMsg.wMsgType = htons(temp);
	temp = len;
	tCommMsg.wMsgLen = htons(temp);
	memcpy(tCommMsg.xmlTransDataBuf, addr, len);

	return PostMsg(EV_SEND_COMMON_REQ, &tCommMsg, sizeof(TCommMsgData), dwContex);
}



void ItsCtrl::ProData(TItsMessage *ptMsg)
{
	
	qCritical()<<(TEXT("CallBackCarInfo: %d\n"), ptMsg->wMsgType);
	switch (ptMsg->wMsgType)
	{
	case EV_VEHICLE_INFO_DATA_CMD:
	{
		
		if (m_dwHandle == 0)
		{
			return;
		}
		// 存储过车信息，设置保存图片的路径，
		TVechileData *pData = (TVechileData *)ptMsg->szMsgBody;
		if (pData->atImage[0].dwType == VEH_IMAGE_TYPE_MJPEG_DEBUG)
		{
			qCritical() << "pData->atImage[0].dwType" << VEH_IMAGE_TYPE_MJPEG_DEBUG;
			MgrData::getInstance()->GetMgrEvtLst()->AddPic(pData->atImage[0].szBuf, pData->atImage[0].dwSize, true);
		}
		else
		{
			MgrData::getInstance()->GetMgrEvtLst()->StorageVehicleInfoToJson(pData, "e:\\1111.ini");
			//qCritical() << "pData->atImage[0].dwType" << "Other" << pData->dwImageNum;
			MgrData::getInstance()->GetMgrEvtLst()->AddItem(ptMsg->dwHandle, pData, NULL);
		}
		
	}
	break;
	case EV_PEER_DISCONNECT_NTY:
	case EV_COMM_PEER_DISCONNECT_NTY:
	{
		m_pMgrData->SetLogin(false);
	}
	break;
	case EV_REGISTER_ACK:
	{
		LoginAck(ptMsg);
	}
	case EV_GET_DEV_CFG_REQ:
	{
		//OnGetDevListAck(ptMsg);
	}
	break;
	case  EV_GET_DEVINFO_CONFIG_ACK:
	{
		//OnGetDevCfgNormalAck(ptMsg);
	}
	break;
	case  EV_GET_AUX_DEVINFO_ACK:
	{
		//OnGetDevCfgAssDevAck(ptMsg);
	}
	break;
	case EV_GET_DEVLIST_CONFIG_ACK:
	{
		//OnGetDevListAck(ptMsg);
	}
	break;
	case EV_GET_SERVER_CONFIG_ACK:
	{
		OnGetSysCfgPlateAck(ptMsg);
	}
	break;
	case EV_GET_IMAGE_MERGE_CFG_ACK:
	{
		//OnGetSysCfgPicMergeAck(ptMsg);
	}
	break;
	case EV_GET_SCENE_CFG_ACK:
	{
		OnGetIpcScenceAck(ptMsg);
	}
	break;
	case EV_GET_ILLG_PARK_ACK:
	{
		OnGetIpcParkScenceAck(ptMsg);
	}
	break;
	case EV_IPC_GET_PARK_PARAM_ACK:
	{
		OnGetIpcParkParamAck(ptMsg);
	}
	break;
	case EV_GET_SCENEPOLL_ACK:
	{
		OnGetIpcScencePollAck(ptMsg);
	}
	break;
	case EV_GET_IPC_DEV_INFO_ACK:
	{
		OnGetIpcDevInfoAck(ptMsg);
	}
	break;
	case EV_GET_OSD_CONFIG_ACK:
	{
		OnGetSysCfgOsdAck(ptMsg);
	}
	break;
	case EV_GET_OSD_FONT_CFG_ACK:
		OnGetSysCfgOsdFontAck(ptMsg);
		break;
	case EV_GET_VIDEO_CONFIG_ACK:
		//OnGetSysCfgRecAck(ptMsg);
		break;
	case EV_GET_LOGIC_LANE_CFG_ACK:
	{
		OnGetIpcLogicLaneAck(ptMsg);
	}
	break;
	case EV_GET_VIDEO_LOOP_CFG_ACK:
	{
		OnGetIpcVideoLoopAck(ptMsg);
	}
	break;
	case EV_GET_RECOG_AREA_CFG_ACK:
	{
		OnGetIpcRecogAreaAck(ptMsg);
	}
	break;
	case EV_GET_RECOG_PARAM_CFG_ACK:
	{
		OnGetIpcIdentAck(ptMsg);
	}
	break;
	case EV_IPC_GET_IMAGMERGEPARA_ACK:
	{
		OnGetIpcMergeParamAck(ptMsg);
	}
	break;
	case EV_GET_COIL_LOOP_CFG_ACK:
	{
		OnGetIpcCoilLoopAck(ptMsg);
	}
	break;
	case EV_GET_RADAR_LOOP_CFG_ACK:
	{
		OnGetIpcRedarLoopAck(ptMsg);
	}
	break;
	case EV_GET_RED_LAMP_CFG_ACK:
	{
		OnGetIpcRedSigLoopAck(ptMsg);
	}
	break;
	case EV_GET_ENHANCE_RED_CFG_ACK:
	{
		OnGetIpcRedEnhLoopAck(ptMsg);
	}
	break;
	case EV_GET_IO_LOOP_CFG_ACK:
	{
		OnGetIpcPowerSynAck(ptMsg);
	}
	break;
	case EV_GET_VIDEO_SPD_DET_CFG_ACK:
	{
		OnGetIpcVelometerAck(ptMsg);
	}
	break;
	case EV_GET_IPC_ISP_CFG_ACK:
	{
		OnGetIpcImageQualityAck(ptMsg);
	}
	break;
	case EV_GET_IPC_POWER_SYNC_ACK:
	{
		OnGetIpcPowerSynAck(ptMsg);
	}
	break;
	case EV_AUTO_CREATE_LOGIC_LANE_ACK:
	{
		OnBuildLogicLaneAck(ptMsg);
	}
	break;
	case EV_GET_IPC_UI_CAP_ACK:
	{
		OnGetUICapAck(ptMsg);
	}
	break;
	case EV_GET_EXTDEVTYPE_ACK:
	{
		OnExtDevTypeAck(ptMsg);
	}
		break;
	case EV_AUTO_CREATE_VIDEO_LOOP_ACK:
	{
		OnSetIpcCreateLoopAck(ptMsg);
	}
	break;
	case EV_IPC_BIT_STREAM_START_ACK:
	{
		//OnStartVideoStreamAck(ptMsg);
	}
	break;
	case EV_IPC_BIT_STREAM_STOP_ACK:
	{
		//OnStopVideoStreamAck(ptMsg);
	}
	break;
	case EV_GET_IPC_SERIAL_CFG_ACK:
	{
		OnGetIpcSerialCfgAck(ptMsg);
	}
	break;
	case EV_IPC_GET_SIGNAL_RSP:
	{
		OnGetIpcSignalProtocolCfgAck(ptMsg);
	}
	break;
	case EV_IPC_GET_INFOSYNC_CFG_ACK:
	{
		OnGetIpcInfoSyncCfgAck(ptMsg);
	}
	break;
	case EV_GET_IPC_DEBUG_PARAM_ACK:
	{
		OnGetIpcDebugParamAck(ptMsg);
	}
	break;
	case EV_GET_IPC_VERSION_ACK:
	{
		//OnGetSoftwareVersionAck(ptMsg);
	}
	break;
	case EV_IPC_GET_CUR_SCENEID_ACK:
	{
		OnGetScenceIDAck(ptMsg);
	}
	break;
	case EV_READ_CONFIG_ACK:
	{
		OnGetExportCfgAck(ptMsg);
	}
	break;
	case  EV_QUERY_RECORD_ACK:
	{
		OnQuerySdRecordAck(ptMsg);
	}
	break;
	case EV_GET_RECORD_ACK:
	{
		OnGetSdRecordAck(ptMsg);
	}
	break;
	case EV_GET_LIGHT_PARAM_ACK:
	{
		OnGetIpcLightBrightnessAck(ptMsg);
	}
	break;
	case EV_GET_IPC_FLOW_CFG_ACK:
	{
		//OnGetIpcFlowDetectionAck(ptMsg);
	}
	break;
	case EV_GET_ILLEGAL_CODE_LIST_ACK:
	{
		OnGetIpcIllegalDictAck(ptMsg);
	}
	break;
	case EV_IPC_FLOWINFO_NTY:
	{
		//OnIpcFlowDetectionNotify(ptMsg);
	}
	break;
	case EV_IPC_FLOWSTAT_NTY:
	{
		//OnIpcStatFlowDetectionNotify(ptMsg);
	}
	break;

	case EV_SEND_COMMON_ACK:
	{
		//OnTransparentMsgAck(ptMsg);
	}
	break;
	case EV_EMPTY_SNAP_ACK:
	{
		//OnIpcEmptySnapAck(ptMsg);
	}
	break;
	default:
		break;
	}
}



bool ItsCtrl::PostMsg(u32 dwEvtType, void* szBodyIn, u32 dwBodyLen, u32 dwContex)
{
	if (m_dwHandle == 0 )
	{
		return false;
	}

	TItsMessage tItsMessage;
	ITSCTRL::MakeMsgParam(&tItsMessage, dwEvtType, szBodyIn, dwBodyLen, dwContex, this);

	int nRet = false;

	qCritical()<<(TEXT("********PostMsg start type = %d \n"), dwEvtType);
	DWORD dw = GetTickCount();
	nRet = IvhsPostMsg(&tItsMessage);
	qCritical() << (TEXT("********PostMsg over type = %d  time = %d\n"), dwEvtType, GetTickCount() - dw);

	return nRet == 0;
}

void ItsCtrl::IvhsCallBack(TItsMessage *ptMsg)
{
	if (m_pmapHandleAndData != NULL)
	{
		std::map<KOSA_HANDLE, ItsCtrl*>::iterator ite = m_pmapHandleAndData->find(ptMsg->dwHandle);
		if (ite != m_pmapHandleAndData->end())
		{
			ItsCtrl* pCtrl =  ite->second;
			pCtrl->ProData(ptMsg);
		}
	}
}

ItsCtrl* ItsCtrl::GetItsCtrlFromHandle(KOSA_HANDLE dwHandle)
{
	if (m_pmapHandleAndData != NULL)
	{
		std::map<KOSA_HANDLE, ItsCtrl*>::iterator ite = m_pmapHandleAndData->find(dwHandle);
		if (ite != m_pmapHandleAndData->end())
		{
			return  ite->second;
		}
	}

	return NULL;
}


void ItsCtrl::Release()
{
	Disconnect();
}

//传输OSD图片
bool  ItsCtrl::SetOsdPic(TIPC_OsdImgData *pData)
{
	return PostMsg(EV_UPLOAD_IPC_OSD_BACK_PIC, pData, sizeof(TIPC_OsdImgData));
}

//端口先注释，后面用到再写
bool ItsCtrl::Login()
{
	
	if (m_dwHandle == 0)
	{
		return false;
	}
	m_bLogin = false;
	// 发送入网请求
	TLoginReqInfo  tLoginReq;
	memset(&tLoginReq, 0, sizeof(TLoginReqInfo));
	sprintf(tLoginReq.achDevId, "%d", GetTickCount());
	tLoginReq.nDevType = 2;
	strcpy(tLoginReq.achUsername, m_sUsr.toStdString().c_str());
	strcpy(tLoginReq.achPassword, m_sPwd.toStdString().c_str());
	
	tLoginReq.dwImgoverlay = 1;
	strcpy(tLoginReq.achProductType, "ITS200");
	strcpy(tLoginReq.achVender, "keda");

	PostMsg(EV_REGISTER_REQ, &tLoginReq, sizeof(tLoginReq), 0);

	
	WaitForSingleObject(m_sem, 4000);
	return m_bLogin;
}

void ItsCtrl::LoginAck(TItsMessage *ptMsg)
{
	TLoginRspInfo *ptRsp;
	ptRsp = (TLoginRspInfo*)ptMsg->szMsgBody;
	if (ptRsp->nRetCode == 0)
	{
		m_bLogin = true;
	}
	else
	{
		m_bLogin = false;
	}
	ReleaseSemaphore(m_sem, 1, NULL);
}

// 视频测速
bool ItsCtrl::SetIpcVelometer(TIPC_VidSpdDet* pData)
{
	bool bRet = true;
	bRet = PostMsg(EV_SET_VIDEO_SPD_DET_CFG_REQ, pData, sizeof(TIPC_VidSpdDet));

	return bRet;
}

bool ItsCtrl::GetIpcVelometer(void* pData, u32 dwContex)
{
	bool bRet = true;
	bRet = PostMsg(EV_GET_VIDEO_SPD_DET_CFG_REQ, NULL, 0, dwContex);

	return bRet;
}

void  ItsCtrl::OnGetIpcVelometerAck(TItsMessage *ptMsg)
{
	CObserver* pObserver = (CObserver*)ptMsg->dwContex;
	m_pMgrData->Notify(pObserver, (LPARAM)NMC_GET_IPC_VELOMETER, (ptMsg->szMsgBody));
}

// 电源同步
bool ItsCtrl::SetIpcPowerSyn(TIPC_PowerPhaseSync* pData)
{
	return  PostMsg(EV_SET_IPC_POWER_SYNC_REQ, pData, sizeof(TIPC_PowerPhaseSync));
}

bool ItsCtrl::GetIpcPowerSyn(void* pData, u32 dwContex)
{
	bool bRet = PostMsg(EV_GET_IPC_POWER_SYNC_REQ, NULL, 0, dwContex);

	return bRet;
}

void  ItsCtrl::OnGetIpcPowerSynAck(TItsMessage *ptMsg)
{
	CObserver* pObserver = (CObserver*)ptMsg->dwContex;
	m_pMgrData->Notify(pObserver, (LPARAM)NMC_GET_IPC_POWERSYN, (ptMsg->szMsgBody));
}

// ipc 串口配置
bool ItsCtrl::SetIpcSerialCfg(TIPC_SerialCfg *pData)
{
	return PostMsg(EV_SET_IPC_SERIAL_CFG_REQ, pData, sizeof(TIPC_SerialCfg));
}

bool ItsCtrl::GetIpcSerialCfg(void* pData, u32 dwContex)
{
	bool bRet = PostMsg(EV_GET_IPC_SERIAL_CFG_REQ, pData, sizeof(pData), dwContex);

	return bRet;
}

void ItsCtrl::OnGetIpcSerialCfgAck(TItsMessage *ptMsg)
{
	CObserver* pObserver = (CObserver*)ptMsg->dwContex;
	m_pMgrData->Notify(pObserver, (LPARAM)NMC_GET_IPC_SERIALCFG, (ptMsg->szMsgBody));
}


// ipc 信号配置
bool ItsCtrl::SetIpcSignalProtocolCfg(TIPC_Signal *pData)
{
	return PostMsg(EV_IPC_SET_SIGNAL_REQ, pData, sizeof(TIPC_Signal));
}

bool ItsCtrl::GetIpcSignalProtocolCfg(void* pData, u32 dwContex)
{
	bool bRet = PostMsg(EV_IPC_GET_SIGNAL_REQ, pData, sizeof(pData), dwContex);

	return bRet;
}

void ItsCtrl::OnGetIpcSignalProtocolCfgAck(TItsMessage *ptMsg)
{
	CObserver* pObserver = (CObserver*)ptMsg->dwContex;
	m_pMgrData->Notify(pObserver, (LPARAM)NMC_GET_IPC_SIGNAL_PROTOCOL, (ptMsg->szMsgBody));
}

// ipc 信息同步配置
bool ItsCtrl::SetIpcInfoSyncCfg(TIPC_InfoSyncCfg *pData)
{
	return PostMsg(EV_IPC_SET_INFOSYNC_CFG_REQ, pData, sizeof(TIPC_Signal));
}

bool ItsCtrl::GetIpcInfoSyncCfg(void* pData, u32 dwContex)
{
	bool bRet = PostMsg(EV_IPC_GET_INFOSYNC_CFG_REQ, pData, sizeof(pData), dwContex);

	return bRet;
}

void ItsCtrl::OnGetIpcInfoSyncCfgAck(TItsMessage *ptMsg)
{
	CObserver* pObserver = (CObserver*)ptMsg->dwContex;
	m_pMgrData->Notify(pObserver, (LPARAM)NMC_GET_IPC_INFO_SYNC, (ptMsg->szMsgBody));
}


// 违法字典
bool ItsCtrl::SetIpcIllegalDict(TIPC_IllegalDict* pData)
{
	bool bRet = true;
	bRet = PostMsg(EV_SET_ILLEGAL_CODE_LIST_REQ, pData, sizeof(TIPC_IllegalDict));

	return bRet;
}

bool ItsCtrl::GetIpcIllegalDict(void* pData, u32 dwContex)
{
	bool bRet = true;
	bRet = PostMsg(EV_GET_ILLEGAL_CODE_LIST_REQ, pData, sizeof(pData), dwContex);

	return bRet;
}

void  ItsCtrl::OnGetIpcIllegalDictAck(TItsMessage *ptMsg)
{
	CObserver* pObserver = (CObserver*)ptMsg->dwContex;
	m_pMgrData->Notify(pObserver, (LPARAM)NMC_GET_IPC_ILLEGALDICT, (ptMsg->szMsgBody));
}

bool ItsCtrl::SetSysCfgPlate(TITS_SetServerReq* pData)
{
	return PostMsg(EV_SET_SERVER_CONFIG_REQ, pData, sizeof(TITS_SetServerReq));
}

bool ItsCtrl::GetSysCfgPlate(TITS_GetServerRsp* pData, u32 dwContex)
{
	return PostMsg(EV_GET_SERVER_CONFIG_REQ, pData, sizeof(TITS_GetServerRsp), dwContex);
}

void ItsCtrl::OnGetSysCfgPlateAck(TItsMessage *ptMsg)
{
	CObserver* pObserver = (CObserver*)ptMsg->dwContex;
	m_pMgrData->Notify(pObserver, (LPARAM)NMC_GET_SVR_PLATEINFO, ptMsg->szMsgBody);
}

//查询过车记录
bool ItsCtrl::QuerySdRecord(void * pData, u32 dwContex)
{
	return PostMsg(EV_QUERY_RECORD_REQ, pData, sizeof(pData), dwContex);
}
void ItsCtrl::OnQuerySdRecordAck(TItsMessage *ptMsg)
{
	CObserver* pObserver = (CObserver*)ptMsg->dwContex;
	m_pMgrData->Notify(NULL, (LPARAM)NMC_QUERY_SD_RECORD, (ptMsg->szMsgBody));
}

//获取过车记录 
bool ItsCtrl::GetSdRecord(void* pData, u32 dwContex)
{
	return PostMsg(EV_GET_RECORD_REQ, pData, sizeof(pData), dwContex);
}

void ItsCtrl::OnGetSdRecordAck(TItsMessage *ptMsg)
{
	CObserver* pObserver = (CObserver*)ptMsg->dwContex;
	m_pMgrData->Notify(pObserver, (LPARAM)NMC_GET_SD_RECORD, (ptMsg->szMsgBody));
}

// 灯光亮度
bool ItsCtrl::SetIpcLightBrightness(TIPC_LightParam* pData)
{
	bool bRet = true;
	bRet = PostMsg(EV_SET_LIGHT_PARAM_REQ, pData, sizeof(TIPC_LightParam));
	return bRet;
}

bool ItsCtrl::GetIpcLightBrightness(void* pData, u32 dwContex)
{
	bool bRet = true;
	bRet = PostMsg(EV_GET_LIGHT_PARAM_REQ, NULL, 0, dwContex);
	return bRet;
}

void ItsCtrl::OnGetIpcLightBrightnessAck(TItsMessage *ptMsg)
{
	CObserver* pObserver = (CObserver*)ptMsg->dwContex;
	m_pMgrData->Notify(pObserver, (LPARAM)NMC_IPC_LIGHT_BRIGHTNESS, (ptMsg->szMsgBody));
}


// 
bool ItsCtrl::SetSysCfgOsd(TOsdCfgInfo* pData)
{
	return PostMsg(EV_SET_OSD_CONFIG_REQ, pData, sizeof(TOsdCfgInfo));
}

bool ItsCtrl::GetSysCfgOsd(u32* pData, u32 dwContex)
{
	return PostMsg(EV_GET_OSD_CONFIG_REQ, pData, sizeof(u32), dwContex);
}

void ItsCtrl::OnGetSysCfgOsdAck(TItsMessage *ptMsg)
{
	CObserver* pObserver = (CObserver*)ptMsg->dwContex;
	m_pMgrData->Notify(pObserver, (LPARAM)NMC_GET_OSDINFO, ptMsg->szMsgBody);
}

// 平台OSD字体相关函数
bool ItsCtrl::SetSysCfgOsdFont(TOsdFontCfg* pData)
{
	return PostMsg(EV_SET_OSD_FONT_CFG_REQ, pData, sizeof(TOsdFontCfg));
}

bool ItsCtrl::GetSysCfgOsdFont(u32* pData, u32 dwContex /*= 0*/)
{
	return PostMsg(EV_GET_OSD_FONT_CFG_REQ, pData, sizeof(u32), dwContex);
}

void ItsCtrl::OnGetSysCfgOsdFontAck(TItsMessage *ptMsg)
{
	CObserver* pObserver = (CObserver*)ptMsg->dwContex;
	m_pMgrData->Notify(pObserver, (LPARAM)NMC_GET_OSDFONTINFO, ptMsg->szMsgBody);
}

// ipc 图片浏览
bool ItsCtrl::DebugMode()
{
	return true;//PostMsg(EV_GET_AUX_DEVINFO_REQ, pData, sizeof(void*), dwContex);
}

void  ItsCtrl::OnDebugModeAck(TItsMessage *ptMsg)
{

}


void ItsCtrl::RefreshReceive()
{
	if (m_bRecv)
	{
		StartReceive();
	}
	else
	{
		StopReceive();
	}
}

bool ItsCtrl::StartReceive()
{
	if (PostMsg(EV_POST_PIC_UNSUSPEND, NULL, 0))
	{
		m_bRecv = true;
		return true;
	}

	return false;
}

bool ItsCtrl::StopReceive()
{
	if (PostMsg(EV_POST_PIC_SUSPEND, NULL, 0))
	{
		m_bRecv = false;
		return true;
	}

	return false;
}


bool ItsCtrl::IpcEvt_Snap(int mode)
{
	bool bRet = PostMsg(EV_CAPTURE_PIC_REQ, (void*)&mode, sizeof(mode));
	return bRet;
}

// ipc 调试信息
bool ItsCtrl::SetIpcDebugParam(TIPC_DebugParam* pData)
{
	bool bRet = true;

	bRet = PostMsg(EV_SET_IPC_DEBUG_PARAM_REQ, pData, sizeof(TIPC_DebugParam));

	return bRet;
}

bool ItsCtrl::GetIpcDebugParam(void* pData, u32 dwContex)
{
	bool bRet = true;

	bRet = PostMsg(EV_GET_IPC_DEBUG_PARAM_REQ, NULL, 0, dwContex);

	return bRet;
}

void  ItsCtrl::OnGetIpcDebugParamAck(TItsMessage *ptMsg)
{
	CObserver* pObserver = (CObserver*)ptMsg->dwContex;
	m_pMgrData->Notify(pObserver, (LPARAM)NMC_GET_IPC_DEBUG_PARAM, (ptMsg->szMsgBody));
}
// // ipc 场景配置
bool ItsCtrl::SetIpcScence(TIPC_SetSceneCfgReq* pData)
{
	bool bRet = true;

	bRet = PostMsg(EV_SET_SCENE_CFG_REQ, pData, sizeof(TIPC_SetSceneCfgReq));

	return bRet;
}

bool ItsCtrl::GetIpcScence(void* pData, u32 dwContex)
{
	bool bRet = true;

	bRet = PostMsg(EV_GET_SCENE_CFG_REQ, pData, sizeof(pData), dwContex);

	return bRet;
}

void  ItsCtrl::OnGetIpcScenceAck(TItsMessage *ptMsg)
{
	CObserver* pObserver = (CObserver*)ptMsg->dwContex;
	m_pMgrData->Notify(pObserver, (LPARAM)NMC_GET_IPC_SCENCEINFO, (ptMsg->szMsgBody));
}

bool ItsCtrl::SetIpcParkScence(TIPC_illgParkPara* pData)
{
	bool bRet = true;
	bRet = PostMsg(EV_SET_ILLG_PARK_REQ, pData, sizeof(TIPC_illgParkPara));
	return bRet;
}

bool ItsCtrl::GetIpcParkScence(void* pData, u32 dwContex)
{
	bool bRet = true;
	bRet = PostMsg(EV_GET_ILLG_PARK_REQ, pData, sizeof(pData), dwContex);
	return bRet;
}

void ItsCtrl::OnGetIpcParkScenceAck(TItsMessage *ptMsg)
{
	CObserver* pObserver = (CObserver*)ptMsg->dwContex;
	m_pMgrData->Notify(pObserver, (LPARAM)NMC_GET_IPC_PARKSCENCE, (ptMsg->szMsgBody));
}

// ipc 违停配置
bool ItsCtrl::SetIpcParkParam(TIPC_ParkParam* pData)
{
	bool bRet = true;
	bRet = PostMsg(EV_IPC_SET_PARK_PARAM_REQ, pData, sizeof(TIPC_illgParkPara));
	return bRet;
}

bool ItsCtrl::GetIpcParkParam(void* pData, u32 dwContex)
{
	bool bRet = true;
	bRet = PostMsg(EV_IPC_GET_PARK_PARAM_REQ, pData, sizeof(pData), dwContex);
	return bRet;
}

void ItsCtrl::OnGetIpcParkParamAck(TItsMessage *ptMsg)
{
	CObserver* pObserver = (CObserver*)ptMsg->dwContex;
	m_pMgrData->Notify(pObserver, (LPARAM)NMC_GET_IPC_PARKPARAM, (ptMsg->szMsgBody));
}

//获取场景ID
bool ItsCtrl::GetScenceID(void* pData, u32 dwContex)
{
	return PostMsg(EV_IPC_GET_CUR_SCENEID_REQ, NULL, 0, dwContex);
}
#include <kosa_debug.h>
void ItsCtrl::OnGetScenceIDAck(TItsMessage *ptMsg)
{
	CObserver* pObserver = (CObserver*)ptMsg->dwContex;
	m_pMgrData->Notify(pObserver, (LPARAM)NMC_GET_IPC_SCENCE_ID, (ptMsg->szMsgBody));
}

bool ItsCtrl::IpcPtz(TIPC_PTZCtrl* pData)
{
	return PostMsg(EV_PTZ_CTRL_REQ, pData, sizeof(TIPC_PTZCtrl));
}

// 识别区域信息获取
bool ItsCtrl::SetIpcRecogArea(TIPC_SetRecogAreaReq* pData)
{
	return PostMsg(EV_SET_RECOG_AREA_CFG_REQ, pData, sizeof(TIPC_SetRecogAreaReq));
}

bool ItsCtrl::GetIpcRecogArea(void* pData, u32 dwContex)
{
	bool bRet = PostMsg(EV_GET_RECOG_AREA_CFG_REQ, pData, sizeof(pData), dwContex);

	return bRet;
}

void  ItsCtrl::OnGetIpcRecogAreaAck(TItsMessage *ptMsg)
{
	CObserver* pObserver = (CObserver*)ptMsg->dwContex;
	m_pMgrData->Notify(pObserver, (LPARAM)NMC_GET_IPC_RECOGAREA, (ptMsg->szMsgBody));
}


// ipc 视频检测器
bool ItsCtrl::SetIpcVideoLoop(TIPC_SetVideoLoopReq* pData)
{
	return PostMsg(EV_SET_VIDEO_LOOP_CFG_REQ, pData, sizeof(TIPC_SetVideoLoopReq));
}

bool ItsCtrl::GetIpcVideoLoop(void* pData, u32 dwContex)
{
	bool bRet = PostMsg(EV_GET_VIDEO_LOOP_CFG_REQ, pData, sizeof(pData), dwContex);

	return bRet;
}

void  ItsCtrl::OnGetIpcVideoLoopAck(TItsMessage *ptMsg)
{
	CObserver* pObserver = (CObserver*)ptMsg->dwContex;
	m_pMgrData->Notify(pObserver, (LPARAM)NMC_GET_IPC_VIDEOLOOPINFO, (ptMsg->szMsgBody));
}

// 线圈检测器信息
bool ItsCtrl::SetIpcCoilLoop(TIPC_SetCoilLoopReq* pData)
{
	return PostMsg(EV_SET_COIL_LOOP_CFG_REQ, pData, sizeof(TIPC_SetCoilLoopReq));
}

bool ItsCtrl::GetIpcCoilLoop(void* pData, u32 dwContex)
{
	return PostMsg(EV_GET_COIL_LOOP_CFG_REQ, pData, sizeof(pData), dwContex);
}

void ItsCtrl::OnGetIpcCoilLoopAck(TItsMessage *ptMsg)
{
	CObserver* pObserver = (CObserver*)ptMsg->dwContex;
	m_pMgrData->Notify(pObserver, (LPARAM)NMC_GET_IPC_COILLOOP, (ptMsg->szMsgBody));
}

bool ItsCtrl::SetIpcRedarLoop(TIPC_SetRadarLoopReq* pData)
{
	return PostMsg(EV_SET_RADAR_LOOP_CFG_REQ, pData, sizeof(TIPC_SetRecogAreaReq));
}

bool ItsCtrl::GetIpcRedarLoop(void* pData, u32 dwContex)
{
	bool bRet = PostMsg(EV_GET_RADAR_LOOP_CFG_REQ, pData, sizeof(pData), dwContex);

	return bRet;
}

void  ItsCtrl::OnGetIpcRedarLoopAck(TItsMessage *ptMsg)
{
	CObserver* pObserver = (CObserver*)ptMsg->dwContex;
	m_pMgrData->Notify(pObserver, (LPARAM)NMC_GET_IPC_REDARLOOP, (ptMsg->szMsgBody));
}

// 红灯信号检测器信息
bool ItsCtrl::SetIpcRedSigLoop(TIPC_SetRedLampSigReq* pData)
{
	return PostMsg(EV_SET_RED_LAMP_CFG_REQ, pData, sizeof(TIPC_SetRedLampSigReq));
}

bool ItsCtrl::GetIpcRedSigLoop(void* pData, u32 dwContex)
{
	return PostMsg(EV_GET_RED_LAMP_CFG_REQ, pData, sizeof(pData), dwContex);
}

void ItsCtrl::OnGetIpcRedSigLoopAck(TItsMessage *ptMsg)
{
	CObserver* pObserver = (CObserver*)ptMsg->dwContex;
	m_pMgrData->Notify(pObserver, (LPARAM)NMC_GET_IPC_REDSIGLOOP, (ptMsg->szMsgBody));
}

// 红灯增强检测器信息
bool ItsCtrl::SetIpcRedEnhLoop(TIPC_SetRedEnReq* pData)
{
	return PostMsg(EV_SET_ENHANCE_RED_CFG_REQ, pData, sizeof(TIPC_SetRedLampSigReq));
}

bool ItsCtrl::GetIpcRedEnhLoop(void* pData, u32 dwContex)
{
	return PostMsg(EV_GET_ENHANCE_RED_CFG_REQ, pData, sizeof(pData), dwContex);
}

void ItsCtrl::OnGetIpcRedEnhLoopAck(TItsMessage *ptMsg)
{
	CObserver* pObserver = (CObserver*)ptMsg->dwContex;
	m_pMgrData->Notify(pObserver, (LPARAM)NMC_GET_IPC_REDENHLOOP, (ptMsg->szMsgBody));
}

// ipc 生成检测器
bool ItsCtrl::SetIpcCreateLoop(void* pData, u32 dwContex)
{
	return PostMsg(EV_AUTO_CREATE_VIDEO_LOOP_REQ, pData, sizeof(pData), dwContex);
}

void  ItsCtrl::OnSetIpcCreateLoopAck(TItsMessage *ptMsg)
{
	CObserver* pObserver = (CObserver*)ptMsg->dwContex;
	m_pMgrData->Notify(pObserver, (LPARAM)NMC_SET_IPC_CREATE_LOOP, (ptMsg->szMsgBody));
}

bool ItsCtrl::BuildLogicLane(void* pData, u32 dwContex)
{
	bool bRet = PostMsg(EV_AUTO_CREATE_LOGIC_LANE_REQ, pData, sizeof(u32), dwContex);

	return bRet;
}

void  ItsCtrl::OnBuildLogicLaneAck(TItsMessage *ptMsg)
{
	CObserver* pObserver = (CObserver*)ptMsg->dwContex;
	m_pMgrData->Notify(pObserver, (LPARAM)NMC_EVT_BUILDLOGICLANE, (ptMsg->szMsgBody));
}

// 违停手动抓拍
bool ItsCtrl::SetSnapIllegalParking(TIPC_SnapIllegalPark* pData)
{
	bool bRet = true;

	bRet = PostMsg(EV_IPC_SNAP_ILLEGALPARK_REQ, pData, sizeof(TIPC_SnapIllegalPark));

	return bRet;
}

// 图像质量
bool ItsCtrl::SetIpcImageQuality(TIPC_KdISPCfg* pData)
{
	bool bRet = true;

	bRet = PostMsg(EV_SET_IPC_ISP_CFG_REQ, pData, sizeof(TIPC_KdISPCfg));

	return bRet;
}

bool ItsCtrl::GetIpcImageQuality(void* pData, u32 dwContex)
{
	bool bRet = true;

	bRet = PostMsg(EV_GET_IPC_ISP_CFG_REQ, NULL, 0, dwContex);

	return bRet;
}

void  ItsCtrl::OnGetIpcImageQualityAck(TItsMessage *ptMsg)
{
	CObserver* pObserver = (CObserver*)ptMsg->dwContex;
	m_pMgrData->Notify(pObserver, (LPARAM)NMC_GET_IPC_IMAGEQUALITY, (ptMsg->szMsgBody));
}


bool ItsCtrl::SetIpcScencePoll(TIPC_ScenePoll* pData, Uint32 number)
{
	bool bRet = true;
	bRet = PostMsg(EV_SET_SCENEPOLL_REQ, pData, sizeof(TIPC_ScenePoll)*number);
	return bRet;
}

bool ItsCtrl::GetIpcScencePoll(void* pData, u32 dwContex)
{
	bool bRet = true;
	bRet = PostMsg(EV_GET_SCENEPOLL_REQ, pData, sizeof(pData), dwContex);
	return bRet;
}

void ItsCtrl::OnGetIpcScencePollAck(TItsMessage *ptMsg)
{
	CObserver* pObserver = (CObserver*)ptMsg->dwContex;
	m_pMgrData->Notify(pObserver, (LPARAM)NMC_GET_IPC_SCENEPOLL, (ptMsg->szMsgBody));
}

// ipc 逻辑车道
bool ItsCtrl::SetIpcLogicLane(TIPC_SetLogicLaneReq* pData)
{
	bool bRet = true;

	bRet = PostMsg(EV_SET_LOGIC_LANE_CFG_REQ, pData, sizeof(TIPC_SetLogicLaneReq));

	return bRet;
}

bool ItsCtrl::GetIpcLogicLane(void* pData, u32 dwContex)
{
	bool bRet = true;

	bRet = PostMsg(EV_GET_LOGIC_LANE_CFG_REQ, pData, sizeof(pData), dwContex);

	return bRet;
}

void  ItsCtrl::OnGetIpcLogicLaneAck(TItsMessage *ptMsg)
{
	CObserver* pObserver = (CObserver*)ptMsg->dwContex;
	m_pMgrData->Notify(pObserver, (LPARAM)NMC_GET_IPC_LOGICLANEINFO, (ptMsg->szMsgBody));
}

// 合成图片参数
bool ItsCtrl::SetIpcMergeParam(TIPC_TImgMergePara* pData)
{
	bool bRet = true;

	bRet = PostMsg(EV_IPC_SET_IMAGMERGEPARA_REQ, pData, sizeof(TIPC_RecogParam));

	return bRet;
}

bool ItsCtrl::GetIpcMergeParam(void* pData, u32 dwContex)
{
	bool bRet = true;

	bRet = PostMsg(EV_IPC_GET_IMAGMERGEPARA_REQ, NULL, 0, dwContex);

	return bRet;
}

void ItsCtrl::OnGetIpcMergeParamAck(TItsMessage *ptMsg)
{
	CObserver* pObserver = (CObserver*)ptMsg->dwContex;
	m_pMgrData->Notify(pObserver, (LPARAM)NMC_GET_IPC_MERGE_PIC, (ptMsg->szMsgBody));
}


// 识别参数
bool ItsCtrl::SetIpcIdent(TIPC_RecogParam* pData)
{
	bool bRet = true;

	bRet = PostMsg(EV_SET_RECOG_PARAM_CFG_REQ, pData, sizeof(TIPC_RecogParam));

	return bRet;
}

bool ItsCtrl::GetIpcIdent(void* pData, u32 dwContex)
{
	bool bRet = true;

	bRet = PostMsg(EV_GET_RECOG_PARAM_CFG_REQ, NULL, 0, dwContex);

	return bRet;
}

void  ItsCtrl::OnGetIpcIdentAck(TItsMessage *ptMsg)
{
	CObserver* pObserver = (CObserver*)ptMsg->dwContex;
	m_pMgrData->Notify(pObserver, (LPARAM)NMC_GET_IPC_IDENTPARAM, (ptMsg->szMsgBody));
}

bool ItsCtrl::GetUICap(u32 dwContex)
{
	bool bRet = PostMsg(EV_GET_IPC_UI_CAP_REQ, NULL, 0, dwContex);

	return bRet;
}

void  ItsCtrl::OnGetUICapAck(TItsMessage *ptMsg)
{
	CObserver* pObserver = (CObserver*)ptMsg->dwContex;
	m_pMgrData->Notify(pObserver, (LPARAM)NMC_GET_UICAP, (ptMsg->szMsgBody));
}

bool ItsCtrl::GetExtDevType(u32 dwContex)
{
	return PostMsg(EV_GET_EXTDEVTYPE_REQ, NULL, 0, dwContex);
}

bool ItsCtrl::SetExtDevType(TIvhsExtDevType* pData)
{
	return PostMsg(EV_SET_EXTDEVTYPE_REQ, pData, sizeof(TIvhsExtDevType));
}


void  ItsCtrl::OnExtDevTypeAck(TItsMessage *ptMsg)
{
	CObserver* pObserver = (CObserver*)ptMsg->dwContex;
	m_pMgrData->Notify(pObserver, (LPARAM)NMC_EVT_EXT_TYPE, (ptMsg->szMsgBody));
}
