/*==============================================================================
ģ����      :  CMgrVideoCtrl
�ļ���      :  CMgrVideoCtrl.h
����ļ�    :  CMgrVideoCtrl.cpp
�ļ�ʵ�ֹ���:  ��Ƶ���ƹ���
����        :
��Ȩ        :  <Copyright(C) 2003-2010 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------
�޸ļ�¼    :
��  ��         �汾          �޸���          �߶���          �޸ļ�¼
2016/06/28     1.0           ���ǿ�                          ����
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
	��������: Init
	��    ��: ��ʼ��
	����˵��:
	�� �� ֵ: ��
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


