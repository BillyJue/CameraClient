/*==============================================================================
模块名      :  CMgrPort
文件名      :
相关文件    :
文件实现功能:  端口管理

作者        :
版权        :  <Copyright(C) 2003-2010 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------
修改记录    :
日  期         版本          修改人          走读人          修改记录
2015/0303/28   1.0           刘登科                          创建
==============================================================================*/

#pragma once
#include "define.h"
class CMgrPort
{
public:
	CMgrPort();
	virtual ~CMgrPort();


public:
	void Init();
	void Release();
	//
	u16  AllocPort(u16 wPortNumber = 1, int nOffset = 0);	// 取得一段空闲端口,立即占用
	void FreePort(u16 wStartPort, u16 wPortNumber = 1);		// 释放端口
	BOOL CheckPortIsUsed(u16 wPort);
	u16	 GetXmlpPort(){ return m_wXmlpPort; }

protected:
	BOOL AllocSharePort(WORD& wFirstIdlePort, WORD wStartPort, WORD wPortNumber);
	void FreeSharePort(WORD wStartPort, WORD wPortNumber);

private:
	BYTE m_abySharedPortState[MAXWORD];						// 1:表示该端口已被占用,0:空闲

	HANDLE	m_hSharedFile[3];
	u16		m_wFirstPort;
	u16		m_wXmlpPort;
};
