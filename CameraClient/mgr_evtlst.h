/*==============================================================================
ģ����      :  CMgrEvtLst
�ļ���      :  CMgrEvtLst.h
����ļ�    :  CMgrEvtLst.cpp
�ļ�ʵ�ֹ���:  ������Ϣ�����������գ���������
����        :
��Ȩ        :  <Copyright(C) 2003-2010 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------
�޸ļ�¼    :
��  ��         �汾          �޸���          �߶���          �޸ļ�¼
2015/03/02     1.0           ���ǿ�                          ����
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
		EMRUNSTATE_SEL,			// ѡ����ʾ
		EMRUNSTATE_ROLL			// ������ʾ
	};
public:
	CMgrEvtLst(void *pData);
	~CMgrEvtLst();
public:

	/*=================================================================
	��������: Init
	��    ��: ��ʼ��
	����˵��:
	�� �� ֵ: ��
	=================================================================*/
	void	Init();
	void    Release();

	/*=================================================================
	��������:
	��    ��: ��ӡ�ɾ��
	����˵��:
	�� �� ֵ: ��
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
	��������:
	��    ��: ��ӡ�ɾ��
	����˵��:
	�� �� ֵ: ��
	=================================================================*/
	void	ClearSender();
	void	AddSender(CObserver* pObserver);
	void	DelSender(CObserver* pObserver);

	/*=================================================================
	��������:
	��    ��: ֪ͨ����
	����˵��:
	�� �� ֵ: ��
	=================================================================*/
	void	Notify(u32 dwEvtType, u32 dwId);


	/*=================================================================
	��������:
	��    ��: ·������
	����˵��:
	�� �� ֵ: ��
	=================================================================*/
	std::wstring GetNewDir(u32 dwIP, SYSTEMTIME* st, wchar_t *path);
	void	MakeDir(std::wstring wsPath);
	void UpdateDir();

	/*=================================================================
	��������:
	��    ��: ͼƬ�������
	����˵��:
	�� �� ֵ: ��
	=================================================================*/
	void		RefreshImageBuf(u32 dwID, TVechileData* pData);
	void		RefreshImageBuf(u32 dwID);			// �Ӵ洢�����л�ȡ
	TImageBuf*	GetAndLockImageBuf(u32 dwID);
	u32			GetImageBufId();
	void		UnlockImageBuf();

	/*=================================================================
	��������:
	��    ��: ͼƬ�洢����
	����˵��:
	�� �� ֵ: ��
	=================================================================*/
	void		AddImageData(u32 dwID, TVechileData* pData);
	void		SavePic(int nIndex);
	int         SaveVideo(int index);


	/*=================================================================
	��������:
	��    ��: ���ݲ���
	����˵��:
	�� �� ֵ: ��
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
	u32					m_dwStartkey;	// �ڴ������е���ʼkeyλ��
	u32					m_dwMapKey;
	u32                 m_dwSumItem;
	int					m_nHourDir;		// ͼƬ�洢��·��ʱ�䣬û��һ��Сʱ���л�·��
	std::wstring				m_wsPicPath;	// ͼƬ�洢·��
	std::wstring				m_wsPicVideoPath;	// ͼƬ�洢·��
	TImageBuf			m_tImageBuf;
	TImageBuf			m_atImageSaveBuf[8];	// �洢ͼƬ������,����8���ӵ�
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


