/*==============================================================================
模块名      :  lock
文件名      :  lock.h
相关文件    :  
文件实现功能:  锁
作者        :  刘登科
版权        :  <Copyright(C) 2003-2010 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------
修改记录    :  
日  期         版本          修改人          走读人          修改记录
2015/12/2      1.0           刘登科                          创建
==============================================================================*/
#pragma  once
#include <WinBase.h>

// 临界区锁
class CCriticalLock
{
public:
	CCriticalLock(PCRITICAL_SECTION pcs):_pcs(pcs)
	{
		//在构造函数内进入临界区
		EnterCriticalSection(_pcs);
	}
	virtual ~CCriticalLock(void)
	{
		//在析构函数内退出临界区
		::LeaveCriticalSection(_pcs);
	}

private:
	//指向CRITICAL_SECTION 结构体的指针
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
		//在析构函数内释放临界区
		::DeleteCriticalSection(_pcs);
		delete _pcs;
		_pcs = NULL;
	}

	PCRITICAL_SECTION GetSection(){return _pcs;}

protected:
	//指向CRITICAL_SECTION 结构体的指针
	PCRITICAL_SECTION _pcs;
};
