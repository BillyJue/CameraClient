/*****************************************************************************
模块名  : define_err
文件名  : define_err.h 
相关文件：
实现功能：定义错误类型
作者    ：刘登科
版本    ：IPC-V7.0-R1.0 Copyright (C) 2009 - 2012 All Rights Reserved
-----------------------------------------------------------------------------
修改记录:
日  期        版本        修改人        走读人    修改内容
20141111      v1.0        刘登科                  创建
******************************************************************************/
#pragma once


//====== Generic Normal notification codes =================================
// 和xml：langstringcn的定义保持一致
#define	 ERR_NONE			0 		
#define	 ERR_DESTROYLOGINERR	1	// 释放登录窗口失败
#define	 ERR_CREATETITLEERR		2	// 创建title窗口失败
#define	 ERR_CREAVIEWERR		3	// 创建浏览窗口失败

// <!--error:10000~12999；-->
#define	 ERR_IPADDRERR		10000	// <String num="10000" text="IP地址错误!" />
#define	 ERR_USERNULL		10001	// 
#define	 ERR_USEROVERLANGTH	10002	// 
#define	 ERR_PASSWORDNULL		10003	// 
#define	 ERR_PASSWORDOVERLANGTH	10004	// 
#define	 ERR_USERORPASSWORD	10005	// 用户名或密码错误
