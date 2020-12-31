/*==============================================================================
ģ����      :  CMgrPort
�ļ���      :
����ļ�    :
�ļ�ʵ�ֹ���:  �˿ڹ���

����        :
��Ȩ        :  <Copyright(C) 2003-2010 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------
�޸ļ�¼    :
��  ��         �汾          �޸���          �߶���          �޸ļ�¼
2015/0303/28   1.0           ���ǿ�                          ����
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
	u16  AllocPort(u16 wPortNumber = 1, int nOffset = 0);	// ȡ��һ�ο��ж˿�,����ռ��
	void FreePort(u16 wStartPort, u16 wPortNumber = 1);		// �ͷŶ˿�
	BOOL CheckPortIsUsed(u16 wPort);
	u16	 GetXmlpPort(){ return m_wXmlpPort; }

protected:
	BOOL AllocSharePort(WORD& wFirstIdlePort, WORD wStartPort, WORD wPortNumber);
	void FreeSharePort(WORD wStartPort, WORD wPortNumber);

private:
	BYTE m_abySharedPortState[MAXWORD];						// 1:��ʾ�ö˿��ѱ�ռ��,0:����

	HANDLE	m_hSharedFile[3];
	u16		m_wFirstPort;
	u16		m_wXmlpPort;
};
