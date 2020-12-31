/*==============================================================================
模块名      :  CMgrEvtLst
文件名      :  CMgrEvtLst.h
相关文件    :  CMgrEvtLst.cpp
文件实现功能:  过车信息管理，包含接收，界面推送
作者        :
版权        :  <Copyright(C) 2003-2010 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------
修改记录    :
日  期         版本          修改人          走读人          修改记录
2015/03/02     1.0           刘登科                          创建
==============================================================================*/
#include "stdafx.h"
#include "winsock2.h"
#include "mgr_evtlst.h"
#include "ivhs_define.h"
#include <QDebug>
#include <wininet.h>

#define IMAGESAVEBUFLEN 4

////////////////////////////////////////////////////////////////////////

MgrData *m_pMgrData5 = nullptr;

std::wstring string2wstringTemp(const std::string& s)
{
	int len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), -1, NULL, 0);
	wchar_t *szGBK = new wchar_t[len + 1];
	if (!szGBK)
	{
		return std::wstring(TEXT(""));
	}
	memset(szGBK, 0, len + 1);
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), -1, szGBK, len);

	std::wstring strTmp(szGBK);
	delete[] szGBK;
	szGBK = NULL;

	return strTmp;
}

// 
UINT __stdcall ProSavePic(void* pParam)
{
	CThreadBase* pThread = (CThreadBase*)pParam;
	CMgrEvtLst*	pCtrl = (CMgrEvtLst*)pThread->m_pContent;
	//ASSERT(pThread);
	//ASSERT(pCtrl);

	void *szMsgBuf = NULL;
	u32 dwBufLen = 0;

	pThread->m_nRunState = 1;
	HANDLE hExitHandle = pThread->m_hExitHandle;
	while (::WaitForSingleObject(hExitHandle, 40) != WAIT_OBJECT_0)
	{
		for (int i = 0; i < IMAGESAVEBUFLEN; i++)
		{
			if (::WaitForSingleObject(hExitHandle, 0) == WAIT_OBJECT_0)
			{
				pThread->SetExit();
				pThread->m_nRunState = 0;
				return true;
			}
			if (pCtrl->m_atImageSaveBuf[i].dwID != 0)
			{
				
				// 存储图片
				pCtrl->SavePic(i);
				break;
			}

		}
	}

	pThread->SetExit();
	pThread->m_nRunState = 0;

	return true;
}

UINT __stdcall ProSaveVideo(void* pParam)
{
	CThreadBase* pThread = (CThreadBase*)pParam;
	CMgrEvtLst*	pCtrl = (CMgrEvtLst*)pThread->m_pContent;
	//ASSERT(pThread);
	//ASSERT(pCtrl);

	void *szMsgBuf = NULL;
	u32 dwBufLen = 0;
	u32 id = 1;
	u32 count = 3;
	int nRet = 0;

	pThread->m_nRunState = 1;
	HANDLE hExitHandle = pThread->m_hExitHandle;
	while (::WaitForSingleObject(hExitHandle, 1000) != WAIT_OBJECT_0)
	{
		if (pCtrl->m_dwMapKey > id && pCtrl->m_dwMapKey > 1)
		{
			for (int i = 0; i<count; i++)
			{
				nRet = pCtrl->SaveVideo(id);
				if (nRet == 1)
				{
					break;
				}
			}
			id++;
		}
		else if (id > pCtrl->m_dwMapKey)
		{
			id = 0;
		}
	}

	pThread->SetExit();
	pThread->m_nRunState = 0;

	return true;
}

//////////////////////////////////////////////////////////////////////////
CMgrEvtLst::CMgrEvtLst(void *pData)
{
	m_dwRunState = EMRUNSTATE_ROLL;
	m_pMgrData5 = (MgrData*)pData;
	m_nHourDir = 0;
	m_bSaveVideoPic = false;
	m_bUpdateDir = false;
	memset(&m_tImageBuf, 0, sizeof(m_tImageBuf));
	memset(&m_atImageSaveBuf, 0, sizeof(m_atImageSaveBuf));
	m_wsPicVideoPath.clear();

	Init();
}

CMgrEvtLst::~CMgrEvtLst()
{
	Release();
}

void CMgrEvtLst::Init()
{
	for (int i = 0; i < MAX_NUM_IMAGE; i++)
	{
		if (m_tImageBuf.apBuf[i] == NULL)
		{
			m_tImageBuf.apBuf[i] = new BYTE[MAXPICBUF];
			m_tImageBuf.anBufMaxLen[i] = MAXPICBUF;
		}
	}

	for (int j = 0; j < IMAGESAVEBUFLEN; j++)
	{
		for (int i = 0; i < MAX_NUM_IMAGE; i++)
			if (m_atImageSaveBuf[j].apBuf[i] == NULL)
			{
				m_atImageSaveBuf[j].apBuf[i] = new BYTE[MAXPICBUF];
				m_atImageSaveBuf[j].anBufMaxLen[i] = MAXPICBUF;
			}
	}

	Clear();

	m_bUpdateDir = true;

	//
	m_threadSavePic.Run(&m_threadSavePic, (PVOID)this, ProSavePic);
	m_threadSaveVideo.Run(&m_threadSaveVideo, (PVOID)this, ProSaveVideo);
}

void CMgrEvtLst::Release()
{
	Clear();

	m_dwRunState = EMRUNSTATE_STOP;
	m_threadSavePic.Exit(TRUE);
	m_threadSaveVideo.Exit(FALSE);

	for (int i = 0; i < MAX_NUM_IMAGE; i++)
	{
		delete m_tImageBuf.apBuf[i];
		m_tImageBuf.apBuf[i] = NULL;
	}

	for (int j = 0; j < IMAGESAVEBUFLEN; j++)
	{
		for (int i = 0; i < MAX_NUM_IMAGE; i++)
		{
			if (m_atImageSaveBuf[j].apBuf[i] != NULL)
			{
				delete m_atImageSaveBuf[j].apBuf[i];
				m_atImageSaveBuf[j].apBuf[i] = NULL;
			}
		}
	}
}

void CMgrEvtLst::Clear()
{
	CCriticalLock lock(_pcs);

	m_dwSumItem = 1;
	m_dwMapKey = 1;
	m_dwStartkey = m_dwMapKey;
	TMAPEPEVTINFOITE ite = m_mapInfo.begin();
	while (ite != m_mapInfo.end())
	{
		if (ite->second != NULL)
		{
			delete  ite->second;
			ite->second = NULL;
		}

		ite++;
	}

	m_mapInfo.clear();
}

cJSON* CMgrEvtLst::StorageVideoInfoToJson(TVideInfo *ptInfo)
{
	cJSON *root;
	root = cJSON_CreateObject();

	double num;

	//违法视频存储类型 0: HTTP, 1: FTP
	num = ptInfo->dwIllvideotype;
	cJSON_AddNumberToObject(root, "dwIllvideotype", num);
	//违法视频开始时间（20140905100010）
	cJSON_AddStringToObject(root, "achIllvideostarttime", ptInfo->achIllvideostarttime);
	//违法视频结束时间（20140905100030）
	cJSON_AddStringToObject(root, "achIllvideoendtime", ptInfo->achIllvideoendtime);
	//违法视频全路径
	cJSON_AddStringToObject(root, "achIllvideofullurl", ptInfo->achIllvideofullurl);

	return root;
}


cJSON* CMgrEvtLst::StoragePersonInfoToJson(TIPCPersonInfo *ptInfo)
{
	cJSON *root;
	root = cJSON_CreateObject();

	double num;

	/*可选  人脸坐标左上角X坐标*/
	num = ptInfo->dwFacePointLeftX;
	cJSON_AddNumberToObject(root, "dwFacePointLeftX", num);
	/*可选  人脸坐标左上角Y坐标*/
	num = ptInfo->dwFacePointLeftY;
	cJSON_AddNumberToObject(root, "dwFacePointLeftY", num);
	/*可选  人脸坐标右上角X坐标*/
	num = ptInfo->dwFacePointRightX;
	cJSON_AddNumberToObject(root, "dwFacePointRightX", num);
	/*可选  人脸坐标右上角Y坐标*/
	num = ptInfo->dwFacePointRightY;
	cJSON_AddNumberToObject(root, "dwFacePointRightY", num);
	/*是否主驾驶员*/
	num = ptInfo->bMainDriver;
	cJSON_AddNumberToObject(root, "bMainDriver", num);
	/*是否正在打电话*/
	num = ptInfo->bPhoneCalling;
	cJSON_AddNumberToObject(root, "bPhoneCalling", num);
	/*是否未系安全带*/
	num = ptInfo->bWithOutBelt;
	cJSON_AddNumberToObject(root, "bWithOutBelt", num);
	/*姓名*/
	cJSON_AddStringToObject(root, "chName", ptInfo->chName);
	/*证件类型*/
	num = ptInfo->idType;
	cJSON_AddNumberToObject(root, "idType", num);
	/*证件号码*/
	cJSON_AddStringToObject(root, "chIdNumber", ptInfo->chIdNumber);

	return root;
}


void CMgrEvtLst::StorageVehicleInfoToJson(TVechileData *pData, QString sPath)
{
	TVechileInfo*ptInfo = &(pData->tInfo);
	cJSON *root;
	root = cJSON_CreateObject();

	double num;

	/*必要  过车记录编号*/
	num = ptInfo->dwSeqNumber;
	cJSON_AddNumberToObject(root, "dwSeqNumber", num);
	/*必要  过车时间，默认为第一张图片的时间*/
	num = ptInfo->qwThroughTime;
	cJSON_AddNumberToObject(root, "qwThroughTime", num);
	/*可选  设备名称，可以是中文字符*/
	cJSON_AddStringToObject(root, "achPointNum", ptInfo->achPointNum);
	/*必要  设备编号，数字字符串*/
	cJSON_AddStringToObject(root, "achDevId", ptInfo->achDevId);
	/*可选 地点*/
	cJSON_AddStringToObject(root, "szSceneName", ptInfo->szSceneName);
	/*可选 道路编码*/
	cJSON_AddStringToObject(root, "szRoadCode", ptInfo->szRoadCode);
	/*必要  0-图片数据；1-图片URL*/
	num = ptInfo->nPhotoType;
	cJSON_AddNumberToObject(root, "nPhotoType", num);

	/*必要  车道号(如1、2、3…)*/
	num = ptInfo->dwLaneNum;
	cJSON_AddNumberToObject(root, "dwLaneNum", num);
	/*可选  车辆行驶方向*/
	cJSON_AddStringToObject(root, "achDirection", ptInfo->achDirection);
	/*必要  限速值*/
	num = ptInfo->dwSpeedLimit;
	cJSON_AddNumberToObject(root, "dwSpeedLimit", num);
	/*必要  车辆行驶速度*/
	num = ptInfo->dwVehicleSpeed;
	cJSON_AddNumberToObject(root, "dwVehicleSpeed", num);
	/*必要  车牌号码（如京A12345）*/
	cJSON_AddStringToObject(root, "achFrontPlateNum", ptInfo->achFrontPlateNum);
	/*可选  车牌颜色（见车牌颜色定义 EPlateColor）*/
	num = ptInfo->dwFrontPlateColor;
	cJSON_AddNumberToObject(root, "dwFrontPlateColor", num);
	/*可选  车牌种类 参考枚举EPlateType*/
	num = ptInfo->dwFrontPlateType;
	cJSON_AddNumberToObject(root, "dwFrontPlateType", num);
	/*可选  车牌置信度（0-100）*/
	num = ptInfo->frontplateconf;
	cJSON_AddNumberToObject(root, "frontplateconf", num);
	/*可选  车牌单字符置信度*/
	cJSON_AddStringToObject(root, "achFrontPlatesubcconf", ptInfo->achFrontPlatesubcconf);
	/*对象大类 参考枚举ESnapType*/
	num = ptInfo->dwVehicleType;
	cJSON_AddNumberToObject(root, "dwVehicleType", num);
	/*可选 车辆种类（参见车辆种类EVehicleClass）*/
	num = ptInfo->dwVehicleclass;
	cJSON_AddNumberToObject(root, "dwVehicleclass", num);
	/*可选 细分车型（参见车辆子类型定义EVehicleSubClass）*/
	num = ptInfo->dwVehiclesubclass;
	cJSON_AddNumberToObject(root, "dwVehiclesubclass", num);
	/*可选  车身颜色 参考枚举EVehicleColor*/
	num = ptInfo->nVehiclecColor;
	cJSON_AddNumberToObject(root, "nVehiclecColor", num);
	/*车辆标识（如大众）*/
	cJSON_AddStringToObject(root, "achVehiclelogo", ptInfo->achVehiclelogo);
	/*细分品牌及年款（如桑塔纳3000）*/
	cJSON_AddStringToObject(root, "achDetailedtype", ptInfo->achDetailedtype);

	/*必要  0为正常，1为违法，2为事件*/
	num = ptInfo->dwIllegalState;
	cJSON_AddNumberToObject(root, "dwIllegalState", num);
	/*必要  违法或事件类型 参考枚举EIllegalType*/
	cJSON_AddStringToObject(root, "achIllegalType", ptInfo->achIllegalType);
	/*违章代码*/
	num = ptInfo->dwIllegalCode;
	cJSON_AddNumberToObject(root, "dwIllegalCode", num);
	//违法视频个数
	num = ptInfo->dwIllvideonum;
	cJSON_AddNumberToObject(root, "dwIllvideonum", num);
	//违法视频信息
	if (ptInfo->dwIllvideonum > 0)
	{
		cJSON *ptVideoArray = cJSON_CreateArray();
		for (Uint32 i = 0; i < ptInfo->dwIllvideonum; i++)
		{
			cJSON *ptVideoInfo = StorageVideoInfoToJson(&(ptInfo->atVideoInfo[i]));
			cJSON_AddItemToArray(ptVideoArray, ptVideoInfo);
		}
		cJSON_AddItemToObject(root, "VideoInfo", ptVideoArray);
	}

	/*可选  红灯开始时间*/
	num = ptInfo->qwRLampStartTime;
	cJSON_AddNumberToObject(root, "qwRLampStartTime", num);
	/*可选  红灯结束时间*/
	num = ptInfo->qwRLampStopTime;
	cJSON_AddNumberToObject(root, "qwRLampStopTime", num);

	/*车辆、行人或非机动车坐标      左上角X坐标*/
	num = ptInfo->dwVehcPointLeftX;
	cJSON_AddNumberToObject(root, "dwVehcPointLeftX", num);
	/*车辆、行人或非机动车坐标      左上角Y坐标*/
	num = ptInfo->dwVehcPointLeftY;
	cJSON_AddNumberToObject(root, "dwVehcPointLeftY", num);
	/*车辆、行人或非机动车坐标      右上角X坐标*/
	num = ptInfo->dwVehcPointRightX;
	cJSON_AddNumberToObject(root, "dwVehcPointRightX", num);
	/*车辆、行人或非机动车坐标      右上角Y坐标*/
	num = ptInfo->dwVehcPointRightY;
	cJSON_AddNumberToObject(root, "dwVehcPointRightY", num);
	/*可选  车牌坐标左上角X坐标*/
	num = ptInfo->dwPlatePointLeftX;
	cJSON_AddNumberToObject(root, "dwPlatePointLeftX", num);
	/*可选  车牌坐标左上角Y坐标*/
	num = ptInfo->dwPlatePointLeftY;
	cJSON_AddNumberToObject(root, "dwPlatePointLeftY", num);
	/*可选  车牌坐标右上角X坐标*/
	num = ptInfo->dwPlatePointRightX;
	cJSON_AddNumberToObject(root, "dwPlatePointRightX", num);
	/*可选  车牌坐标右上角Y坐标*/
	num = ptInfo->dwPlatePointRightY;
	cJSON_AddNumberToObject(root, "dwPlatePointRightY", num);
	/*司乘人员数量*/
	num = ptInfo->dwPersonNum;
	cJSON_AddNumberToObject(root, "dwPersonNum", num);
	/*司乘人员属性*/
	TIPCPersonInfo atPerson[MAX_NUM_PERSON];
	if (ptInfo->dwPersonNum > 0)
	{
		cJSON *ptPersonArray = cJSON_CreateArray();
		for (Uint32 i = 0; i < ptInfo->dwPersonNum; i++)
		{
			cJSON *ptPersonInfo = StoragePersonInfoToJson(&(ptInfo->atPerson[i]));
			cJSON_AddItemToArray(ptPersonArray, ptPersonInfo);
		}
		cJSON_AddItemToObject(root, "PersonInfo", ptPersonArray);
	}

	/*以下为公安视图库信息必选项*/
	/*图片采集方式,参考enum EInfoKind*/
	num = ptInfo->dwInfoKind;
	cJSON_AddNumberToObject(root, "dwInfoKind", num);
	/*图片的格式,参考enum EFileFormat*/
	num = ptInfo->dwFileFormat;
	cJSON_AddNumberToObject(root, "dwFileFormat", num);
	/*图片信息来源,参考enum EDataSourceType*/
	num = ptInfo->dwDataSourceType;
	cJSON_AddNumberToObject(root, "dwDataSourceType", num);
	/*图片的宽带*/
	num = ptInfo->dwImgWidth;
	cJSON_AddNumberToObject(root, "dwImgWidth", num);
	/*图片的高度*/
	num = ptInfo->dwImgHeight;
	cJSON_AddNumberToObject(root, "dwImgHeight", num);
	/*图像资料名称的汉语描述*/
	cJSON_AddStringToObject(root, "achTitle", ptInfo->achTitle);
	/*对图像内容的简要描述*/
	cJSON_AddStringToObject(root, "achContentDescription", ptInfo->achContentDescription);
	/*自动分析事件类型,参考enum EEventSort*/
	num = ptInfo->dwEventSort;
	cJSON_AddNumberToObject(root, "dwEventSort", num);
	/*是否单独发送人脸对象*/
	num = ptInfo->bSendFace;
	cJSON_AddNumberToObject(root, "bSendFace", num);

	cJSON* rootParent = cJSON_CreateObject();
	cJSON_AddNumberToObject(rootParent, "dwImageNum", pData->dwImageNum);

	cJSON_AddItemToObject(rootParent, "TVechileInfo", root);

	cJSON* imgInfoArr = cJSON_CreateArray();
	for (int i = 0; i < pData->dwImageNum; i++)
	{
		cJSON*temp = cJSON_CreateObject();
		cJSON_AddNumberToObject(temp, "dwSize", pData->atImage[i].dwSize);
		cJSON_AddNumberToObject(temp, "qwTime", pData->atImage[i].qwTime);
		cJSON_AddNumberToObject(temp, "dwType", pData->atImage[i].dwType);
		cJSON_AddStringToObject(temp, "szAntifake", pData->atImage[i].szAntifake);
		cJSON_AddStringToObject(temp, "achUrl", pData->atImage[i].achUrl);
		cJSON_AddNumberToObject(temp, "dwWidth", pData->atImage[i].dwWidth);
		cJSON_AddNumberToObject(temp, "dwHeight", pData->atImage[i].dwHeight);
		//cJSON_AddBoolToObject(temp, "bDrop", pData->atImage[i].bDrop);
		//cJSON_AddBoolToObject(temp, "fromRemote", pData->atImage[i].fromRemote);
		cJSON_AddItemToArray(imgInfoArr, temp);

	}
	cJSON_AddItemToObject(rootParent, "ImageInfoArr", imgInfoArr);
	if (!rootParent)
	{
		return;
	}
	char *pStr = cJSON_Print(rootParent);
	FILE* fp = _wfopen(sPath.toStdWString().c_str(), TEXT("wb"));
	if (NULL != fp)
	{
		fwrite(pStr, strlen(pStr), 1, fp);
		fclose(fp);
	}
	cJSON_Delete(rootParent);
}



void CMgrEvtLst::AddPic(void* pData, u32 dwLen, bool isNotify)
{
	CCriticalLock lock2(m_lockPicBuf.GetSection());

	// 最新的数据保存一份用于图片显示
	m_tImageBuf.dwID = 0;
	m_tImageBuf.dwBufNum = 1;

	// 图片存储队列
	m_tImageBuf.anBufType[0] = 1;
	m_tImageBuf.anBufLen[0] = dwLen;
	if (m_tImageBuf.anBufMaxLen[0] < dwLen)
	{
		m_tImageBuf.anBufMaxLen[0] = dwLen;
		delete m_tImageBuf.apBuf[0];
		m_tImageBuf.apBuf[0] = new BYTE[dwLen];
	}
	if (m_tImageBuf.apBuf[0] != NULL)
	{
		memcpy(m_tImageBuf.apBuf[0], pData, dwLen);
	}

	if (m_wsPicVideoPath.empty())
	{
		m_pMgrData5->GetMgrLocalCfg()->GetLocalCfg(&m_tLocalCfgInfo);
		m_wsPicVideoPath = m_tLocalCfgInfo.picturepath;
		m_wsPicVideoPath += TEXT("\\视频流图片\\");
		MakeDir(m_wsPicVideoPath);
	}



	//
	if (m_bSaveVideoPic)
	{

		SYSTEMTIME st;
		GetLocalTime(&st);
		wchar_t wsNameFirst[MAX_PATH]; // 时间 + 车牌
		wsprintf(wsNameFirst, TEXT("%s%02d%02d%02d%02d%02d%02d%03d.jpg"), m_wsPicVideoPath.c_str(), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
		FILE* fp = _wfopen(wsNameFirst, TEXT("wb"));
		if (NULL != fp)
		{
			fwrite(m_tImageBuf.apBuf[0], dwLen, 1, fp);
			fclose(fp);
		}
	}

	if (isNotify)
	{
		Notify(NMC_EVT_VIDEO_PIC, 0);
	}
}

void CMgrEvtLst::AddItem(KOSA_HANDLE dwHandle, TVechileData* p, wchar_t *path)
{
	CCriticalLock lock(_pcs);

	if (p == NULL)
		return;

	if (m_dwRunState == EMRUNSTATE_STOP)
		return;

	// 保存过车信息
	PTEVTVEHICLEINFO pData = new TEVTVEHICLEINFO;
	memcpy(&pData->tVechileData, p, sizeof(TVechileData));

	// 设置图片存储路径
	// ldk tst
	u32 dwIP = inet_addr("10.61.1.24");
	ItsCtrl* pCtrl = ItsCtrl::GetItsCtrlFromHandle(dwHandle);
	if (pCtrl != NULL)
	{
		dwIP = pCtrl->getIP();
	}

	SYSTEMTIME st;
	GetLocalTime(&st);
	if (st.wHour != m_nHourDir)
	{
		m_nHourDir = st.wHour;
		m_bUpdateDir = true;
	}

	if (m_bUpdateDir)
	{
		m_pMgrData5->GetMgrLocalCfg()->GetLocalCfg(&m_tLocalCfgInfo);
		m_wsPicPath = GetNewDir(dwIP, &st, m_tLocalCfgInfo.picturepath);
		MakeDir(m_wsPicPath);
	}

	//
	pData->dwHandle = dwHandle;
	pData->dwIP = dwIP;

	// 设置图片路径
	wsprintf(pData->szDir, TEXT("%s\\"), m_wsPicPath.c_str());

	// 保存图片名称
	time_t timer = pData->tVechileData.tInfo.qwThroughTime / 1000;
	int milliseconds = pData->tVechileData.tInfo.qwThroughTime % 1000;
	struct tm *local_time = NULL;
	local_time = localtime(&timer);
	wchar_t wsNameFirst[MAX_PATH]; // 时间 + 车牌
	if (local_time != NULL)
	{
		wsprintf(wsNameFirst, TEXT("%02d%02d%02d%02d%02d%02d%03d_%d_%s_%s_"), local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday, local_time->tm_hour, local_time->tm_min, local_time->tm_sec, milliseconds,
			pData->tVechileData.tInfo.dwSeqNumber, string2wstringTemp(std::string(pData->tVechileData.tInfo.achFrontPlateNum)).c_str(), Vechile::GetIllegalType(&(pData->tVechileData)).c_str());
	}
	else
	{
		wsprintf(wsNameFirst, TEXT("%02d%02d%02d%02d%02d%02d%03d_%d_%s_%s_"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds,
			pData->tVechileData.tInfo.dwSeqNumber, string2wstringTemp(std::string(pData->tVechileData.tInfo.achFrontPlateNum)).c_str(), Vechile::GetIllegalType(&(pData->tVechileData)).c_str());
	}

	qCritical() << "pData->tVechileData.dwImageNum:" << pData->tVechileData.dwImageNum;
	int i;
	for (i = 0; i < (int)pData->tVechileData.dwImageNum; i++)
	{
		wsprintf(pData->szImageName[i], TEXT("%s%d%s"), wsNameFirst, i + 1, TEXT(".jpg"));
	}

	// 数据满删除考前的一部分
	if (m_mapInfo.size() > MAX_EVTLIST_NUM)
	{
		DelAllItem();
	}

	// 数据入库
	m_mapInfo[m_dwMapKey] = pData;

	qCritical() << "最新的数据保存一份用于图片显示pppp" << p->dwImageNum;
	// 最新的数据保存一份用于图片显示
	
	// 
	AddImageData(m_dwMapKey, p);
	RefreshImageBuf(m_dwMapKey, p);

	Notify(NMC_EVT_ADD, m_dwMapKey);

	m_dwMapKey++;
	m_dwSumItem++;
}

void   CMgrEvtLst::DelAllItem()
{
	m_dwSumItem = 1;
	m_dwMapKey = 1;
	m_dwStartkey = m_dwMapKey;

	TMAPEPEVTINFOITE ite = m_mapInfo.begin();
	while (ite != m_mapInfo.end())
	{
		if (ite->second != NULL)
		{
			delete  ite->second;
			ite->second = NULL;
		}

		ite++;
	}
	m_mapInfo.clear();

	Notify(NMC_EVT_DELALL, 0);
}

void   CMgrEvtLst::DelFirstItem(int nLen)
{
	int nDel = 0;
	TMAPEPEVTINFOITE ite = m_mapInfo.begin();
	while (ite != m_mapInfo.end())
	{
		if (ite->second != NULL)
		{
			delete  ite->second;
			ite->second = NULL;
		}

		nDel++;
		m_mapInfo.erase(ite);
		ite = m_mapInfo.begin();
		if (nDel >= nLen)
		{
			break;;
		}
	}
	m_dwSumItem -= nDel;
}

void	CMgrEvtLst::DelItem(u32 dwId)
{
	CCriticalLock lock(_pcs);

	TMAPEPEVTINFOITE ite = m_mapInfo.find(dwId);
	if (ite != m_mapInfo.end())
	{
		if (ite->second != NULL)
		{
			Notify(NMC_EVT_DEL, ite->first);

			delete  ite->second;
			ite->second = NULL;
		}

		m_mapInfo.erase(ite);
	}
}

void	CMgrEvtLst::SelItem(u32 dwId)
{
	CCriticalLock lock(_pcs);

	TMAPEPEVTINFOITE ite = m_mapInfo.find(dwId);
	if (ite != m_mapInfo.end())
	{
		if (ite->second != NULL)
		{
			Notify(NMC_EVT_SEL, ite->first);
		}
	}
}

PTEVTVEHICLEINFO	CMgrEvtLst::GetItem(u32 dwId)
{
	CCriticalLock lock(_pcs);

	TMAPEPEVTINFOITE ite = m_mapInfo.find(dwId);
	if (ite != m_mapInfo.end())
	{
		if (ite->second != NULL)
		{
			return  ite->second;
		}
	}

	return NULL;
}

u32		CMgrEvtLst::GetItemIndex(u32 dwId)
{
	CCriticalLock lock(_pcs);

	TMAPEPEVTINFOITE ite = m_mapInfo.find(dwId);
	if (ite != m_mapInfo.end())
	{
		if (ite->second != NULL)
		{
			return  ite->first - m_dwStartkey + 1;
		}
	}

	return 0;
}

void	CMgrEvtLst::ClearSender()
{
	m_lstSender.clear();
}

void	CMgrEvtLst::AddSender(CObserver* pObserver)
{
	m_lstSender.push_back(pObserver);
}

void	CMgrEvtLst::DelSender(CObserver* pObserver)
{
	std::list<CObserver*>::iterator ite = m_lstSender.begin();
	while (ite != m_lstSender.end())
	{
		if (pObserver == *(ite))
		{
			m_lstSender.erase(ite);
			return;
		}
		ite++;
	}
}

void	CMgrEvtLst::Notify(u32 dwEvtType, u32 dwId)
{
	std::list<CObserver*>::iterator ite = m_lstSender.begin();
	while (ite != m_lstSender.end())
	{
		CObserver* po = *(ite);
		m_pMgrData5->Notify(po, dwEvtType, (LPVOID)dwId);

		ite++;
	}

}

std::wstring CMgrEvtLst::GetNewDir(u32 dwIP, SYSTEMTIME* st, wchar_t *path)
{
	// 路径为IP\\年\\月\\日\\小时\\PIC\\ITS， ip样式：172-16-255-2
	std::wstring ws = path;
	wchar_t tmp[64];

	int i = ws.find_last_of(TEXT("\\"));
	if (i + 1 == ws.length())
	{
		ws.erase(ws.end() - 1);
	}

	if (m_tLocalCfgInfo.isip)
	{
		memset(tmp, 0, sizeof(tmp));
		swprintf(tmp, L"\\%d-%d-%d-%d",
			dwIP & 0xff, (dwIP & 0xff00) >> 8, (dwIP & 0xff0000) >> 16, dwIP >> 24);
		ws += tmp;
	}

	if (m_tLocalCfgInfo.isyear)
	{
		memset(tmp, 0, sizeof(tmp));
		swprintf(tmp, L"\\%04d", st->wYear);
		ws += tmp;
	}

	if (m_tLocalCfgInfo.ismonth)
	{
		memset(tmp, 0, sizeof(tmp));
		swprintf(tmp, L"\\%02d", st->wMonth);
		ws += tmp;
	}

	if (m_tLocalCfgInfo.isday)
	{
		memset(tmp, 0, sizeof(tmp));
		swprintf(tmp, L"\\%02d", st->wDay);
		ws += tmp;
	}

	if (m_tLocalCfgInfo.ishour)
	{
		memset(tmp, 0, sizeof(tmp));
		swprintf(tmp, L"\\%02d", st->wHour);
		ws += tmp;
	}


	return ws;
}

void	CMgrEvtLst::MakeDir(std::wstring wsPath)
{
	if (wsPath.empty())
		return;

	if (wsPath.at(wsPath.length() - 1) != '\\')
	{
		wsPath.append(1, '\\');
	}

	int end = wsPath.rfind('\\');
	int pt = wsPath.find('\\');

	//如果是网络路径，如 \\172.16.0.2
	if (0 == pt)
		return;

	if (wsPath.at(pt - 1) == ':')
		pt = wsPath.find('\\', pt + 1);

	//如果是CD-ROM
	TCHAR szDrive[20] = { 0 };
	for (int i = 0; i < pt; i++)
	{
		szDrive[i] = wsPath.at(i);
	}
	if (DRIVE_CDROM == GetDriveType(szDrive))
		return;

	wchar_t wszTmp[MAX_PATH];
	while (pt != -1 && pt <= end)
	{
		wcscpy(wszTmp, const_cast<wchar_t*>(wsPath.c_str())); wszTmp[pt] = '\0';
		if (_waccess(wszTmp, 0) == -1)
		{
			if (_wmkdir(wszTmp) != 0)
			{
				return;
			}
		}
		pt = wsPath.find('\\', pt + 1);
	}
}

void CMgrEvtLst::UpdateDir()
{
	m_bUpdateDir = true;
}

void CMgrEvtLst::RefreshImageBuf(u32 dwID, TVechileData* pData)
{
	qCritical() << "RefreshImageBuf first-------";
	// 不是滚动状态数据不用更新
	if (!IsRoll())
	{
		return;
	}
	qCritical() << "RefreshImageBuf seconde-------"<<pData->dwImageNum;

	CCriticalLock lock(m_lockPicBuf.GetSection());

	// 最新的数据保存一份用于图片显示
	m_tImageBuf.dwID = dwID;
	m_tImageBuf.dwBufNum = pData->dwImageNum;

	// 图片存储队列
	for (int i = 0; i < (int)pData->dwImageNum; i++)
	{
		m_tImageBuf.anBufType[i] = pData->atImage[i].dwType;
		m_tImageBuf.anBufLen[i] = pData->atImage[i].dwSize;
		if (m_tImageBuf.anBufLen[i] > m_tImageBuf.anBufMaxLen[i])
		{
			m_tImageBuf.anBufMaxLen[i] = m_tImageBuf.anBufLen[i];
			delete m_tImageBuf.apBuf[i];
			m_tImageBuf.apBuf[i] = new BYTE[m_tImageBuf.anBufMaxLen[i]];
		}
		memcpy(m_tImageBuf.apBuf[i], pData->atImage[i].szBuf, pData->atImage[i].dwSize);
	}
}

void CMgrEvtLst::RefreshImageBuf(u32 dwID)
{
	CCriticalLock lock(m_lockPicBuf.GetSection());

	//
	PTEVTVEHICLEINFO pInfo = GetItem(dwID);
	if (pInfo == NULL)
	{
		return;
	}

	// 最新的数据保存一份用于图片显示
	m_tImageBuf.dwID = dwID;
	m_tImageBuf.dwBufNum = pInfo->tVechileData.dwImageNum;

	// 图片存储队列
	for (int i = 0; i < (int)pInfo->tVechileData.dwImageNum; i++)
	{
		m_tImageBuf.anBufType[i] = pInfo->tVechileData.atImage[i].dwType;
		m_tImageBuf.anBufLen[i] = pInfo->tVechileData.atImage[i].dwSize;

		// 从存储图片中读取数据
		std::wstring wsPath;
		wsPath = pInfo->szDir;
		wsPath += pInfo->szImageName[i];

		if (_waccess(wsPath.c_str(), 0) != -1)
		{
			FILE* fp = _wfopen(wsPath.c_str(), TEXT("rb"));
			if (NULL != fp)
			{
				fseek(fp, 0, SEEK_END);
				long iFileLen = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				m_tImageBuf.anBufLen[i] = iFileLen;
				fread(m_tImageBuf.apBuf[i], iFileLen, 1, fp);
				fclose(fp);
			}
		}
		else
		{
			m_tImageBuf.anBufLen[i] = 0;
		}
	}
}

TImageBuf*	CMgrEvtLst::GetAndLockImageBuf(u32 dwID)
{
	::EnterCriticalSection(m_lockPicBuf.GetSection());

	if (dwID == m_tImageBuf.dwID || dwID == 0)
	{
		return &m_tImageBuf;
	}

	// 重新获取
	RefreshImageBuf(dwID);

	if (dwID == m_tImageBuf.dwID)
	{
		return &m_tImageBuf;
	}

	::LeaveCriticalSection(m_lockPicBuf.GetSection());
	return NULL;
}


void	CMgrEvtLst::UnlockImageBuf()
{
	::LeaveCriticalSection(m_lockPicBuf.GetSection());
}

u32	CMgrEvtLst::GetImageBufId()
{
	CCriticalLock lock(m_lockPicBuf.GetSection());

	// 最新的数据保存一份用于图片显示
	return m_tImageBuf.dwID;
}


void CMgrEvtLst::AddImageData(u32 dwID, TVechileData* pData)
{
	int nIndexIdle = -1;
	for (int i = 0; i < IMAGESAVEBUFLEN; i++)
		if (m_atImageSaveBuf[i].dwID == 0)
		{
			nIndexIdle = i;
			break;
		}

	if (nIndexIdle == -1)
	{
		qCritical()<< ("AddImageData 无空闲的位置");
		return;
	}

	memset(m_atImageSaveBuf[nIndexIdle].anBufLen, 0, sizeof(m_atImageSaveBuf[nIndexIdle].anBufLen));
	memset(m_atImageSaveBuf[nIndexIdle].anBufType, 0, sizeof(m_atImageSaveBuf[nIndexIdle].anBufType));

	// 图片存储队列
	for (int i = 0; i < (int)pData->dwImageNum; i++)
	{
		qCritical() << "ddaaaaaaaaaaaaaaaa" << pData->dwImageNum;
		m_atImageSaveBuf[nIndexIdle].anBufType[i] = pData->atImage[i].dwType;
		m_atImageSaveBuf[nIndexIdle].anBufLen[i] = pData->atImage[i].dwSize;
		if (m_atImageSaveBuf[nIndexIdle].anBufLen[i] > m_atImageSaveBuf[nIndexIdle].anBufMaxLen[i])
		{
			m_atImageSaveBuf[nIndexIdle].anBufMaxLen[i] = m_atImageSaveBuf[nIndexIdle].anBufLen[i];
			delete m_atImageSaveBuf[nIndexIdle].apBuf[i];
			m_atImageSaveBuf[nIndexIdle].apBuf[i] = new BYTE[m_atImageSaveBuf[nIndexIdle].anBufMaxLen[i]];
		}
		memcpy(m_atImageSaveBuf[nIndexIdle].apBuf[i], pData->atImage[i].szBuf, pData->atImage[i].dwSize);
	}

	// 
	m_atImageSaveBuf[nIndexIdle].dwID = dwID;
}

void	CMgrEvtLst::SavePic(int nIndex)
{
	if (m_atImageSaveBuf[nIndex].dwID == 0)
		return;

	PTEVTVEHICLEINFO pData = GetItem(m_atImageSaveBuf[nIndex].dwID);
	if (pData == NULL)
	{
		m_atImageSaveBuf[nIndex].dwID = 0;
		return;
	}
	{
		QString wsPath = QString::fromWCharArray(pData->szDir);
		QString str = QString::fromWCharArray(pData->szImageName[0]);
		str = str.left(str.length() - 6);
		str += ".ini";
		wsPath += str;
		//保存JSon数据
		StorageVehicleInfoToJson(&pData->tVechileData, wsPath);

	}

	for (int i = 0; i < (int)pData->tVechileData.dwImageNum; i++)
	{
		std::wstring wsPath;
		wsPath = pData->szDir;
		wsPath += pData->szImageName[i];

		bool bSave = true;
		switch (pData->tVechileData.atImage[i].dwType)
		{
		case VEH_IMAGE_TYPE_FRONT_PANORAMA:
		case VEH_IMAGE_TYPE_BACK_PANORAMA:
		{
			if (pData->tVechileData.tInfo.dwIllegalState == 0)
			{
				if (m_tLocalCfgInfo.isBayonet == false)
				{
					bSave = false;
				}
			}
			else if (pData->tVechileData.tInfo.dwIllegalState == 1)
			{
				if (m_tLocalCfgInfo.isillegal == false)
				{
					bSave = false;
				}
			}
		}
		break;
		case VEH_IMAGE_TYPE_FRONT_PLANTE:
		case VEH_IMAGE_TYPE_BACK_PLANTE:
		{
			if (m_tLocalCfgInfo.isPlate == false)
			{
				bSave = false;
			}
		}
		break;
		case 7://VEH_IMAGE_TYPE_MASTER_FACE  //为什么不一致ivhs_define。h定义的宏
		case 8://VEH_IMAGE_TYPE_COPILOT_FACE
		{
			if (m_tLocalCfgInfo.isFace == false)
			{
				bSave = false;
			}
		}
		break;
		case VEH_IMAGE_TYPE_ILLEGAL:
		{
			if (m_tLocalCfgInfo.isillegal == false)
			{
				bSave = false;
			}
		}
		break;
		case VEH_IMAGE_TYPE_MONTAGE:
		{
			if (pData->tVechileData.tInfo.dwIllegalState == 0)
			{
				if (m_tLocalCfgInfo.isBayonetMontage == false)
				{
					bSave = false;
				}
			}
			else  if (pData->tVechileData.tInfo.dwIllegalState == 1)
			{
				if (m_tLocalCfgInfo.isillegalMontage == false)
				{
					bSave = false;
				}
			}
		}
		break;
		case VEH_IMAGE_TYPE_VEHICLE:
		{
			if (m_tLocalCfgInfo.isVehicle == false)
			{
				bSave = false;
			}
		}
		break;
		default:
			break;
		}

		if (_waccess(wsPath.c_str(), 0) == -1 && bSave)
		{
			FILE* fp = _wfopen(wsPath.c_str(), TEXT("wb"));
			if (NULL != fp)
			{
				fwrite(m_atImageSaveBuf[nIndex].apBuf[i], m_atImageSaveBuf[nIndex].anBufLen[i], 1, fp);
				fclose(fp);
			}
		}
	}

	m_atImageSaveBuf[nIndex].dwID = 0;
	memset(m_atImageSaveBuf[nIndex].anBufLen, 0, sizeof(m_atImageSaveBuf[nIndex].anBufLen));
	memset(m_atImageSaveBuf[nIndex].anBufType, 0, sizeof(m_atImageSaveBuf[nIndex].anBufType));
	for (int i = 0; i < MAX_NUM_IMAGE; i++)
		memset(m_atImageSaveBuf[nIndex].apBuf[i], 0, sizeof(m_atImageSaveBuf[nIndex].apBuf[i]));

}

int  CMgrEvtLst::SaveVideo(int index)
{
	static HINTERNET hInternet = NULL;
	HINTERNET hFtpSession = NULL;
	int nRet = 0;

	PTEVTVEHICLEINFO pData = GetItem(index);
	if (pData == NULL)
	{
		return 0;
	}

	if (pData->tVechileData.tInfo.dwIllvideonum == 0)
	{
		return 0;
	}

	if (m_tLocalCfgInfo.isVideo == false)
	{
		return 0;
	}

	if (hInternet == NULL)
	{
		hInternet = InternetOpen(NULL, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	}

	wchar_t ip[32];
	in_addr ipaddr;
	ipaddr.S_un.S_addr = m_pMgrData5->GetIP();
	char2wchar(ip, inet_ntoa(ipaddr));

	if (hFtpSession == NULL && hInternet != NULL)
	{
		hFtpSession = InternetConnect(hInternet, ip, INTERNET_DEFAULT_FTP_PORT, m_pMgrData5->GetMgrUser()->GetCurUser()->szName,
			m_pMgrData5->GetMgrUser()->GetCurUser()->szPassword, INTERNET_SERVICE_FTP, 0, 0);
	}

	wchar_t remotefile[1024];
	std::wstring localfile = pData->szDir;

	std::string fullurl = pData->tVechileData.tInfo.atVideoInfo[0].achIllvideofullurl;
	int n = fullurl.find('@');
	if (n == -1)
	{
		InternetCloseHandle(hFtpSession);
		return 0;
	}
	n = fullurl.find('/', n);
	if (n == -1)
	{
		InternetCloseHandle(hFtpSession);
		return 0;
	}
	int m = fullurl.size();
	fullurl = fullurl.substr(n, m - n);

	char2wchar(remotefile, fullurl.c_str());
	//char2wchar(remotefile, pData->tVechileData.tInfo.atVideoInfo[0].achIllvideofullurl);

	std::wstring file = pData->szImageName[0];
	int i = file.find_last_of(TEXT("_"));
	int count = file.length();
	localfile += file.substr(0, i);
	localfile += TEXT("_video.avi");

	if (hFtpSession != NULL)
	{
		nRet = FtpGetFile(hFtpSession, remotefile, localfile.c_str(), 0, 0, FTP_TRANSFER_TYPE_BINARY, 0);
	}

	//CStdString strText;
	//strText.Format(_T("ftp:%d,%d\n"), hFtpSession, nRet);
	//TRACE(strText.GetData());

	InternetCloseHandle(hFtpSession);
	//InternetCloseHandle(hInternet);
	return nRet;
}