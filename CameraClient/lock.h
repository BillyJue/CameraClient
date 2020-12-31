/*==============================================================================
ģ����      :  lock
�ļ���      :  lock.h
����ļ�    :  
�ļ�ʵ�ֹ���:  ��
����        :  ���ǿ�
��Ȩ        :  <Copyright(C) 2003-2010 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------
�޸ļ�¼    :  
��  ��         �汾          �޸���          �߶���          �޸ļ�¼
2015/12/2      1.0           ���ǿ�                          ����
==============================================================================*/
#pragma  once
#include <WinBase.h>

// �ٽ�����
class CCriticalLock
{
public:
	CCriticalLock(PCRITICAL_SECTION pcs):_pcs(pcs)
	{
		//�ڹ��캯���ڽ����ٽ���
		EnterCriticalSection(_pcs);
	}
	virtual ~CCriticalLock(void)
	{
		//�������������˳��ٽ���
		::LeaveCriticalSection(_pcs);
	}

private:
	//ָ��CRITICAL_SECTION �ṹ���ָ��
	PCRITICAL_SECTION _pcs;
};

class CCriticalObject
{
public:
	CCriticalObject()
	{
		_pcs = new CRITICAL_SECTION;
		::InitializeCriticalSection(_pcs);
	}
	virtual ~CCriticalObject(void)
	{
		//�������������ͷ��ٽ���
		::DeleteCriticalSection(_pcs);
		delete _pcs;
		_pcs = NULL;
	}

	PCRITICAL_SECTION GetSection(){return _pcs;}

protected:
	//ָ��CRITICAL_SECTION �ṹ���ָ��
	PCRITICAL_SECTION _pcs;
};
