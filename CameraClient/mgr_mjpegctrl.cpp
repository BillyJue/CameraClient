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
#include "stdafx.h"
#include <io.h>
#include "MgrItsCtrl.h"
#include "mjpeg_sdk.h"
#include "mgr_mjpegctrl.h"
#include <QDir>
MgrData *m_mgrData6 = nullptr;

//CMgrMjpegCtrl *g_pMgrMjpegCtrl = NULL;
////////////////////////////////////////////////////////////////////////
void MJpegMsgCB(Uint32 dwHandle, Uint32 dwMsgType, char *pBuf, Uint32 dwBufSize, Uint32 dwContext)
{
	CMgrMjpegCtrl *pCtrl;
	pCtrl = (CMgrMjpegCtrl*)dwContext;
	if (pCtrl == NULL || pCtrl->m_handle != dwHandle)
	{
		return;
	}

	switch (dwMsgType)
	{
	case NEW_MJPEG_DATA:
		pCtrl->MJpegMsgPro(pBuf, dwBufSize);
		break;
	case NOTIFY_TYPE_DISCONNECT:
		pCtrl->Disconnect();
		break;
	default:
		break;
	}

}

//////////////////////////////////////////////////////////////////////////
CMgrMjpegCtrl::CMgrMjpegCtrl(void* pData)
{
	m_mgrData6 = (MgrData*)pData;
	m_nIP = 0;
	//g_pMgrMjpegCtrl = this;
	m_bUsed = false;
	m_handle = 0;
	m_wsPicVideoPath.clear();

	m_tImageBuf.empty();
	m_tImageBuf.apBuf[0] = new BYTE[MAXPICBUF];

	Init();
}

CMgrMjpegCtrl::~CMgrMjpegCtrl()
{
	Release();
}

void CMgrMjpegCtrl::Init()
{
	MJpegInit(1);
	//MJpegRegisterCBFun(MJpegMsgCB, (Uint32)this);
}

void CMgrMjpegCtrl::Release()
{
	MJpegUnInit();
}

void  CMgrMjpegCtrl::Reset()
{
	m_nIP = 0;
	m_mapCtrlHandle.clear();
	m_mapMsgHandle.clear();
	if (m_handle != 0)
	{
		MJpegClose(m_handle);
		m_handle = 0;
	}
}

void  CMgrMjpegCtrl::SetIP(u32 ip)
{
	m_nIP = ip;
}

void  CMgrMjpegCtrl::StartRecvMjpeg(HWND h)
{
	if (m_handle == 0)
	{
		m_handle = MJpegConnect(m_nIP, 21000);
		MJpegRegisterCBFun(m_handle, MJpegMsgCB, (Uint32)this);
	}

	m_mapCtrlHandle[(u32)h] = (u32)h;
}

void  CMgrMjpegCtrl::StopRecvMjpeg(HWND h)
{
	std::map<u32, u32>::iterator ite = m_mapCtrlHandle.find((u32)h);
	if (ite != m_mapCtrlHandle.end())
	{
		m_mapCtrlHandle.erase(ite);
	}

	if (m_mapCtrlHandle.empty())
	{
		MJpegClose(m_handle);
		m_handle = 0;
	}
}

void  CMgrMjpegCtrl::AddHandle(MyWidget* h)
{
	m_mapMsgHandle[h] = h;
}

void  CMgrMjpegCtrl::DelHandle(MyWidget* h)
{
	std::map<MyWidget*, MyWidget*>::iterator ite = m_mapMsgHandle.find(h);
	if (ite != m_mapMsgHandle.end())
	{
		m_mapMsgHandle.erase(ite);
	}
}


static bool isDirExist(QString fullPath)
{
	QDir dir(fullPath);
	if (dir.exists())
	{
		return true;
	}
	else
	{
		bool ok = dir.mkdir(fullPath);//只创建一级子目录，即必须保证上级目录存在
		return ok;
	}
}

void CMgrMjpegCtrl::MJpegMsgPro(char *pBuf, Uint32 dwBufSize)
{
	CCriticalLock lock2(m_lockPicBuf.GetSection());

	// 最新的数据保存一份用于图片显示
	m_tImageBuf.dwID = 0;
	m_tImageBuf.dwBufNum = 1;

	// 图片存储队列
	m_tImageBuf.anBufType[0] = 1;
	m_tImageBuf.anBufLen[0] = dwBufSize;
	if (m_tImageBuf.apBuf[0] != NULL)
	{
		memcpy(m_tImageBuf.apBuf[0], pBuf, dwBufSize);
	}

	if (m_bSave)
	{
		if (m_wsPicVideoPath.empty())
		{
			isDirExist(QString::fromStdWString(m_wsPicVideoPath.c_str()));
			u32 dwAddr = MgrData::getInstance()->GetIP();
			unsigned char *pch = (unsigned char*)&dwAddr;
			char szIPAddr[64] = { 0 };
			sprintf(szIPAddr, "%d-%d-%d-%d", *(pch), *(pch + 1), *(pch + 2), *(pch + 3));
			wchar_t wIP[64] = { 0 };
			char2wchar(wIP, szIPAddr);
			m_wsPicVideoPath += TEXT("\\");
			m_wsPicVideoPath += std::wstring(wIP);
			isDirExist(QString::fromStdWString(m_wsPicVideoPath.c_str()));

			m_wsPicVideoPath += TEXT("\\视频流图片\\");
			isDirExist(QString::fromStdWString(m_wsPicVideoPath.c_str()));
			//MakeDir(m_wsPicVideoPath);
		}

		SYSTEMTIME st;
		GetLocalTime(&st);
		wchar_t wsNameFirst[MAX_PATH]; // 时间 + 车牌
		wsprintf(wsNameFirst, TEXT("%s%02d%02d%02d%02d%02d%02d%03d.jpg"), m_wsPicVideoPath.c_str(), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
		FILE* fp = _wfopen(wsNameFirst, TEXT("wb"));
		if (NULL != fp)
		{
			fwrite(m_tImageBuf.apBuf[0], dwBufSize, 1, fp);
			fclose(fp);
		}
	}

	// 通知窗口刷新，异步
	if (m_mapMsgHandle.size() == 0)
	{
		return;
	}
	std::map<MyWidget*, MyWidget*>::iterator ite;
	ite = m_mapMsgHandle.begin();
	if(ite != m_mapMsgHandle.end())
	{
		ite->second->ReceveVideoStream();
	}
}

void CMgrMjpegCtrl::SetMJpegSave(bool flg)
{
	m_bSave = flg;
}

void	CMgrMjpegCtrl::MakeDir(std::wstring wsPath)
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

TImageBuf*	CMgrMjpegCtrl::GetAndLockImageBuf(u32 dwID)
{
	CCriticalLock lock2(m_lockPicBuf.GetSection());

	return &m_tImageBuf;
}

void  CMgrMjpegCtrl::Disconnect()
{
	m_handle = 0;

	// 通知窗口刷新，异步
	std::map<MyWidget*, MyWidget*>::iterator ite;
	ite = m_mapMsgHandle.begin();
	while (ite != m_mapMsgHandle.end())
	{
		/*HWND hHandle = (HWND)ite->second;
		::PostMessage(hHandle, NMC_EVT_VIDEO_PIC_DISCONNECT, (WPARAM)this, 0);
		ite++;*/
	}
}
