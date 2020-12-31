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
#pragma once
#include "stdafx.h"
#include "threadbase.h"
#include "cJSON.h"
#define MAX_EVTLIST_NUM 3600
////////////////////////////////////////////////////////////////////////

class CMgrEvtLst :public CCriticalObject
{
public:
	enum EMRUNSTATE
	{
		EMRUNSTATE_STOP = 0,
		EMRUNSTATE_SEL,			// 选中显示
		EMRUNSTATE_ROLL			// 滚动显示
	};
public:
	CMgrEvtLst(void *pData);
	~CMgrEvtLst();
public:

	/*=================================================================
	函数名称: Init
	功    能: 初始化
	参数说明:
	返 回 值: 无
	=================================================================*/
	void	Init();
	void    Release();

	/*=================================================================
	函数名称:
	功    能: 添加、删除
	参数说明:
	返 回 值: 无
	=================================================================*/
	void	Clear();
	void	AddItem(KOSA_HANDLE dwHandle, TVechileData* p, wchar_t *path);
	void	DelItem(u32 dwId);
	void    DelAllItem();
	void    DelFirstItem(int nLen);
	void	SelItem(u32 dwId);
	int		GetItemCount(){ return m_dwSumItem; }
	PTEVTVEHICLEINFO	GetItem(u32 dwId);
	u32		GetItemIndex(u32 dwId);
	void	AddPic(void* pData, u32 dwLen, bool isNotify);

	void		StorageVehicleInfoToJson(TVechileData *pData, QString sPath);
	cJSON*		StorageVideoInfoToJson(TVideInfo *ptInfo);
	cJSON*		StoragePersonInfoToJson(TIPCPersonInfo *ptInfo);
	/*=================================================================
	函数名称:
	功    能: 添加、删除
	参数说明:
	返 回 值: 无
	=================================================================*/
	void	ClearSender();
	void	AddSender(CObserver* pObserver);
	void	DelSender(CObserver* pObserver);

	/*=================================================================
	函数名称:
	功    能: 通知界面
	参数说明:
	返 回 值: 无
	=================================================================*/
	void	Notify(u32 dwEvtType, u32 dwId);


	/*=================================================================
	函数名称:
	功    能: 路径设置
	参数说明:
	返 回 值: 无
	=================================================================*/
	std::wstring GetNewDir(u32 dwIP, SYSTEMTIME* st, wchar_t *path);
	void	MakeDir(std::wstring wsPath);
	void UpdateDir();

	/*=================================================================
	函数名称:
	功    能: 图片浏览数据
	参数说明:
	返 回 值: 无
	=================================================================*/
	void		RefreshImageBuf(u32 dwID, TVechileData* pData);
	void		RefreshImageBuf(u32 dwID);			// 从存储数据中获取
	TImageBuf*	GetAndLockImageBuf(u32 dwID);
	u32			GetImageBufId();
	void		UnlockImageBuf();

	/*=================================================================
	函数名称:
	功    能: 图片存储数据
	参数说明:
	返 回 值: 无
	=================================================================*/
	void		AddImageData(u32 dwID, TVechileData* pData);
	void		SavePic(int nIndex);
	int         SaveVideo(int index);


	/*=================================================================
	函数名称:
	功    能: 数据操作
	参数说明:
	返 回 值: 无
	=================================================================*/
	void		RunToStop(){ m_dwRunState = EMRUNSTATE_STOP; }
	void		RunToRoll(){ m_dwRunState = EMRUNSTATE_ROLL; }
	void		RunToSel(){ m_dwRunState = EMRUNSTATE_SEL; }
	bool		IsStop(){ return m_dwRunState == EMRUNSTATE_STOP; }
	bool		IsRoll(){ return m_dwRunState == EMRUNSTATE_ROLL; }
	bool		IsSel(){ return m_dwRunState == EMRUNSTATE_SEL; }

public:
	//MgrData*			m_pMgrData;
	TMAPEPEVTINFO		m_mapInfo;
	u32					m_dwStartkey;	// 内存数据中的起始key位置
	u32					m_dwMapKey;
	u32                 m_dwSumItem;
	int					m_nHourDir;		// 图片存储的路径时间，没过一个小时，切换路径
	std::wstring				m_wsPicPath;	// 图片存储路径
	std::wstring				m_wsPicVideoPath;	// 图片存储路径
	TImageBuf			m_tImageBuf;
	TImageBuf			m_atImageSaveBuf[8];	// 存储图片的序列,超过8个扔掉
	TLocalCfgInfo       m_tLocalCfgInfo;

	std::list<CObserver*>	m_lstSender;
	bool				m_bSaveVideoPic;
	bool				m_bUpdateDir;

private:
	CCriticalObject		m_lockPicBuf;
	CThreadBase			m_threadSavePic;
	CThreadBase			m_threadSaveVideo;
	u32					m_dwRunState;
};


