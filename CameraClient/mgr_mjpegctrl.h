/*==============================================================================
模块名      :  CMgrVideoCtrl
文件名      :  CMgrVideoCtrl.h
相关文件    :  CMgrVideoCtrl.cpp
文件实现功能:  视频控制管理
作者        :
版权        :  <Copyright(C) 2003-2010 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------
修改记录    :
日  期         版本          修改人          走读人          修改记录
2016/06/28     1.0           刘登科                          创建
==============================================================================*/

#pragma once
#include "stdafx.h"
#include "define.h"
#include "MyWidget.h"

class CMgrMjpegCtrl :public CCriticalObject
{
public:
	CMgrMjpegCtrl(void* pData);
	~CMgrMjpegCtrl();
public:
	/*=================================================================
	函数名称: Init
	功    能: 初始化
	参数说明:
	返 回 值: 无
	=================================================================*/
	void  Init();

	void  Release();

	void  Reset();

	void  SetIP(u32 ip);

	void  StartRecvMjpeg(HWND h);

	void  StopRecvMjpeg(HWND h);

	void  AddHandle(MyWidget* pWidget);

	void  DelHandle(MyWidget *pWidget);

	void  StopRecvMjpeg(CObserver* pObserver);

	void  MJpegMsgPro(char *pBuf, Uint32 dwBufSize);

	void  SetMJpegSave(bool flg);

	void  MakeDir(std::wstring wsPath);

	TImageBuf*	GetAndLockImageBuf(u32 dwID);

	void  Disconnect();

public:
	//MgrData*			m_pMgrData;
	u32                      m_nIP;
	u32                      m_handle;
	std::map<u32, u32>	m_mapCtrlHandle;
	std::map<MyWidget*, MyWidget*>	m_mapMsgHandle;
	TImageBuf            m_tImageBuf;
	bool                      m_bUsed;
	bool                      m_bSave;
	std::wstring				     m_wsPicVideoPath;
	CCriticalObject		 m_lockPicBuf;
};


