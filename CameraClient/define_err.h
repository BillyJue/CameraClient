/*****************************************************************************
ģ����  : define_err
�ļ���  : define_err.h 
����ļ���
ʵ�ֹ��ܣ������������
����    �����ǿ�
�汾    ��IPC-V7.0-R1.0 Copyright (C) 2009 - 2012 All Rights Reserved
-----------------------------------------------------------------------------
�޸ļ�¼:
��  ��        �汾        �޸���        �߶���    �޸�����
20141111      v1.0        ���ǿ�                  ����
******************************************************************************/
#pragma once


//====== Generic Normal notification codes =================================
// ��xml��langstringcn�Ķ��屣��һ��
#define	 ERR_NONE			0 		
#define	 ERR_DESTROYLOGINERR	1	// �ͷŵ�¼����ʧ��
#define	 ERR_CREATETITLEERR		2	// ����title����ʧ��
#define	 ERR_CREAVIEWERR		3	// �����������ʧ��

// <!--error:10000~12999��-->
#define	 ERR_IPADDRERR		10000	// <String num="10000" text="IP��ַ����!" />
#define	 ERR_USERNULL		10001	// 
#define	 ERR_USEROVERLANGTH	10002	// 
#define	 ERR_PASSWORDNULL		10003	// 
#define	 ERR_PASSWORDOVERLANGTH	10004	// 
#define	 ERR_USERORPASSWORD	10005	// �û������������
