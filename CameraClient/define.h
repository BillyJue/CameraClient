/*****************************************************************************
模块名  : define
文件名  : define.h 
相关文件：
实现功能：定义类
作者    ：刘登科
版本    ：IPC-V7.0-R1.0 Copyright (C) 2009 - 2012 All Rights Reserved
-----------------------------------------------------------------------------
修改记录:
日  期        版本        修改人        走读人    修改内容
20151111      v1.0        刘登科                  创建
******************************************************************************/
#pragma once

#include "kdvtype.h"
#include "ivhs_struct.h"
#include <map>
#include <list>


//===================设备PID===========================
#define IPC626_AU_S_PID                      0x1180
#define IPC626_AU_S_HID                      0x0210

#define IPC627_AU_C_PID                      0x119B
#define IPC627_AU_C_HID                      0x01EB

#define IPC625_BU_C_PID                      0x1204
#define IPC625_BU_C_HID                      0x01EB

#define IPC625_BU_E_PID                      0x1205
#define IPC625_BU_E_HID                      0x01EB

#define IPC655_BU_PID                        0x11F9
#define IPC655_BU_HID                        0x01EB

#define IPC123_Ei8N                          0x127A

#define IPC2255_GN                           0x128A
#define IPC425_A030_N                        0x11D3

#define IPC425_P323_N                        0x128F
#define IPC425_P323_NWH                      0x12C9

#define IPC425_P345_N                        0x1415

#define IPC625_GN                            0x12AE
#define IPC625_GN_HID                        0x026F

#define IPC655_GN                            0x12AF
#define IPC655_GN_HID                        0x0270

#define IPC675_GN                            0x145B
#define IPC675_GN_HID                        0x02D5

#define IPC695_GN                            0x1425
#define IPC695_GN_HID                        0x02B3

#define IPC2255_Gi8N_PID                     0x140F
#define IPC2255_Gi8N_HID                     0x02A1

#define IPC2655_Gi8N_PID                     0x1410
#define IPC2655_Gi8N_HID                     0x02A2

#define IPC123_HiBNM_PID                     0x12EC
#define IPC123_HiBNM_HID                     0x028D

#define ZD5920_Gi8N_PID                      0x1438
#define ZD5920_Gi8N_HID                      0x02A1

#define IPC186_PCWS_PID						 0x1679
#define IPC625_HU_PID                        0x167C
#define IPC695_HU_PID                        0x167D
#define IPC695_HUH_PID						 0x167E
#define IPC695_GNS							 0x164d
#define IPC695_GN_S_HID						 0x02B3
//====== Generic Normal notification codes =================================
//
#define  TELNETPORT						4000						// Telnet Port
#define  WEBSERVERPORT					80							// http协议端口 80
#define	 DEFTITLE						TEXT("服务器")
#define  DECODERMAX						260							// 解码器的最大数量
#define  DEFLANG						TEXT("zh")					// 默认语言

#define	 MINIINFO						TEXT("600,400")				// 窗口最小宽高
#define  DLGMAX							32				// 窗口最大数量 
#define  SCREENNUM						1							// 分屏数量
#define  DEFLAYOUT						"vh_equ_1"					// 默认布局方案,1画面
#define  FRAMEBORDER					4							// 窗口外壳宽度


//====== Generic VER notification codes =================================
//
#define PRODUCT_VER						"1.1.1"//Version

//====== Generic layout notification codes =================================
//
#define PLAN_DEF						TEXT("def")			// 默认预案
#define LAYOUT_GP_1						TEXT("gp_v1h1")		// 等比1画面
#define LAYOUT_GP_4						TEXT("gp_v2h2")		// 等比4画面
#define LAYOUT_GP_9						TEXT("gp_v3h3")		// 等比9画面
#define LAYOUT_GP_16					TEXT("gp_v4h4")		// 等比16画面

//====== Generic WM_NOTIFY notification codes =================================
//
#define NMC_FIRST                       (WM_USER+10000)		// generic to all controls
#define NMC_CHANGELANG                  (NMC_FIRST+1)		// 语言切换			
#define NMC_CHANGEUI                    (NMC_FIRST+2)		// 皮肤切换
#define NMC_DESTROY                     (NMC_FIRST+3)		// 销毁窗口
#define NMC_PLAYCTRLOVER                (NMC_FIRST+4)		// 播放请求完毕
#define NMC_RELAYOUT                    (NMC_FIRST+5)		// 重新布局
#define NMC_VIEWLOADPLAN                (NMC_FIRST+6)		// 窗口预案载入
#define NMC_ALARM                       (NMC_FIRST+7)		// 告警产生
#define NMC_PLAY_CHANGE                 (NMC_FIRST+8)		// 播放改变
#define NMC_ERROR						(NMC_FIRST+9)		// generic to all controls

//----------
#define NMC_DATE_ERROR					(NMC_FIRST+10)		//播放时间错误

#define WM_ADDLISTITEM					(NMC_FIRST+11)      //显示单行消息

#define WM_CHECKPOSITION				(NMC_FIRST+12)		//检测预置位


// UI等常规配置类
#define NMC_CFG_FIRST                   (NMC_FIRST+1000)			//
#define NMC_CFG_SETIP					(NMC_CFG_FIRST+1)			// 设置指定窗口IP
#define NMC_CFG_SETSVRIP				(NMC_CFG_FIRST+2)			// 设置服务器的IP
#define NMC_CFG_SETIPCIP				(NMC_CFG_FIRST+3)			// 设置IPCIP
#define NMC_CFG_SETSVRADNIPCIP		    (NMC_CFG_FIRST+4)			// 设置设备所有参数配置页面的IP

// 事件类
//
#define NMC_EVT_FIRST                   (NMC_CFG_FIRST+1000)		// 
#define NMC_EVT_LOGIN					(NMC_EVT_FIRST+1)			// 登入
#define NMC_EVT_LOGOUT					(NMC_EVT_FIRST+2)			// 登出
#define NMC_EVT_STARTVIDEO				(NMC_EVT_FIRST+3)			// 开始码流
#define NMC_EVT_STOPVIDEO				(NMC_EVT_FIRST+4)			// 停止码流
#define NMC_EVT_VIDEOSTATEREFRESH		(NMC_EVT_FIRST+5)			// 码流窗口刷新
#define NMC_EVT_ADD						(NMC_EVT_FIRST+11)			// 电警过车信息增加
#define NMC_EVT_DEL						(NMC_EVT_FIRST+12)			// 电警过车信息删除
#define NMC_EVT_DELALL					(NMC_EVT_FIRST+13)			// 电警过车信息删除
#define NMC_EVT_SEL						(NMC_EVT_FIRST+14)			// 电警过车信息选中
#define NMC_EVT_DLG_CLOSE				(NMC_EVT_FIRST+15)			// 窗口关闭，保存数据
#define NMC_EVT_LISTCTRL_CHANGE			(NMC_EVT_FIRST+16)			// dlglistCtrl 窗口数据发生改变；
#define NMC_EVT_BUILDLOGICLANE			(NMC_EVT_FIRST+17)			// 生成逻辑车道成功
#define NMC_EVT_VIDEO_PIC				(NMC_EVT_FIRST+18)			// 视频流刷新
#define NMC_EVT_VIDEO_PIC_DISCONNECT    (NMC_EVT_FIRST+19)          // 视频流断链
#define NMC_EVT_FLOWDETECTION           (NMC_EVT_FIRST+20)          // 车流量信息
#define NMC_EVT_SYSTEM_UPGRADE			(NMC_EVT_FIRST+21)			// ipc系统升级
#define NMC_EVT_STAT_FLOWDETECTION      (NMC_EVT_FIRST+22)          // 统计车流量信息
#define NMC_EVT_IO_SIGNAL               (NMC_EVT_FIRST+23)          // IO信号信息
#define NMC_EVT_IO_SIGNAL_DISCONNECT    (NMC_EVT_FIRST+24)          // IO信号断链信息
#define NMC_EVT_HARDWARE_TEST           (NMC_EVT_FIRST+25)          // 硬件测试信息
#define NMC_EVT_SAVE_PARAM              (NMC_EVT_FIRST+26)          // 保存参数
#define NMC_EVT_EXT_TYPE				(NMC_EVT_FIRST+27)          // led闪光灯


// 参数公共获取类
//
#define NMC_GET_FIRST                   (NMC_EVT_FIRST+1000)		// 
#define NMC_GET_OSDINFO					(NMC_GET_FIRST+1)			// 获取OSD信息
#define NMC_GET_PICINFO					(NMC_GET_FIRST+2)			// 获取pic信息
#define NMC_GET_OSDFONTINFO				(NMC_GET_FIRST+3)			// 获取OSD字体信息
#define NMC_GET_UICAP					(NMC_GET_FIRST+4)			// 获取UI能力集

// 参数公共获取服务器类
//
#define NMC_GET_SVR_FIRST               (NMC_GET_FIRST+1000)		// 
#define NMC_GET_SVR_DEVLIST				(NMC_GET_SVR_FIRST+1)		// 获取设备列表信息
#define NMC_GET_SVR_DEVINFO				(NMC_GET_SVR_FIRST+2)		// 获取设备信息
#define NMC_GET_SVR_ASSDEVINFO			(NMC_GET_SVR_FIRST+3)		// 获取辅助设备信息
#define NMC_GET_SVR_PLATEINFO      		(NMC_GET_SVR_FIRST+4)		// 获取plate信息
#define NMC_GET_SVR_RECINFO				(NMC_GET_SVR_FIRST+5)		// 获取rec信息
#define NMC_GET_SVR_STATISTICSINFO		(NMC_GET_SVR_FIRST+6)		// 获取统计信息

// 参数公共获取IPC类
//
#define NMC_GET_IPC_FIRST               (NMC_GET_SVR_FIRST+1000)	
#define NMC_GET_IPC_DEVINFO				(NMC_GET_IPC_FIRST+1)		// 获取设备信息
#define NMC_GET_IPC_SCENCEINFO			(NMC_GET_IPC_FIRST+2)		// 获取场景信息
#define NMC_GET_IPC_VIDEOLOOPINFO		(NMC_GET_IPC_FIRST+3)		// 获取检测器信息
#define NMC_GET_IPC_LOGICLANEINFO		(NMC_GET_IPC_FIRST+4)		// 获取逻辑车道信息
#define NMC_GET_IPC_RECOGAREA			(NMC_GET_IPC_FIRST+5)		// 获取识别区域信息
#define NMC_GET_IPC_IMAGEQUALITY		(NMC_GET_IPC_FIRST+6)		// 获取图像质量信息
#define NMC_GET_IPC_IDENTPARAM			(NMC_GET_IPC_FIRST+7)		// 获取识别参数信息
#define NMC_GET_IPC_POWERSYN			(NMC_GET_IPC_FIRST+8)		// 获取电源同步信息
#define NMC_GET_IPC_REDLIGHT			(NMC_GET_IPC_FIRST+9)		// 获取红灯测量信息
#define NMC_GET_IPC_VELOMETER			(NMC_GET_IPC_FIRST+10)		// 获取视频测速信息
#define NMC_GET_IPC_REDARLOOP			(NMC_GET_IPC_FIRST+11)		// 获取雷达检测器信息
#define NMC_GET_IPC_COILLOOP			(NMC_GET_IPC_FIRST+12)		// 获取线圈检测器信息
#define NMC_GET_IPC_REDSIGLOOP			(NMC_GET_IPC_FIRST+13)		// 获红灯信号检测器信息
#define NMC_GET_IPC_REDENHLOOP			(NMC_GET_IPC_FIRST+14)		// 获红灯增强检测器信息
#define NMC_GET_IPC_IOLOOP				(NMC_GET_IPC_FIRST+15)		// 获io检测器信息
#define NMC_GET_IPC_DRAW_CHANGE			(NMC_GET_IPC_FIRST+16)		// 绘制变化通知信息
#define NMC_SET_IPC_CREATE_LOOP			(NMC_GET_IPC_FIRST+17)		// 生成检测器信息
#define NMC_GET_IPC_SERIALCFG           (NMC_GET_IPC_FIRST+18)      // 获取串口配置
#define NMC_GET_IPC_DEBUG_PARAM			(NMC_GET_IPC_FIRST+19)		// 获取调试信息
#define NMC_GET_IPC_ITS_VERSION	        (NMC_GET_IPC_FIRST+20)		// 获取软件版本信息
#define NMC_IPC_SHOW_ORIGINAL_PIC       (NMC_GET_IPC_FIRST+21)		// 原图显示
#define NMC_GET_IPC_CONFIG              (NMC_GET_IPC_FIRST+22)      // 获取配置文件;
#define NMC_GET_DEV_INFO                (NMC_GET_IPC_FIRST+23)      // 添加搜索设备;
#define NMC_GET_DEV_SEARCH              (NMC_GET_IPC_FIRST+24)      // 搜索设备;
#define NMC_GET_IPC_PARKSCENCE          (NMC_GET_IPC_FIRST+25)      // 违停配置;
#define NMC_IPC_LOGOUT                  (NMC_GET_IPC_FIRST+26)      // 退出登入;
#define NMC_IPC_LIGHT_BRIGHTNESS        (NMC_GET_IPC_FIRST+27)      // 灯亮度配置;
#define NMC_QUERY_SD_RECORD             (NMC_GET_IPC_FIRST+28)      // 查询过车记录
#define NMC_GET_SD_RECORD				(NMC_GET_IPC_FIRST+29)		// 获取过车记录
#define NMC_GET_IPC_SCENEPOLL			(NMC_GET_IPC_FIRST+29)		// 获取场景轮巡参数
#define NMC_GET_IPC_FLOWDETECTION		(NMC_GET_IPC_FIRST+30)		// 获取流量检测参数
#define NMC_GET_IPC_ILLEGALDICT 		(NMC_GET_IPC_FIRST+31)		// 获取违法字典参数
#define NMC_GET_IPC_MERGE_PIC           (NMC_GET_IPC_FIRST+32)		// 获取合成图片参数
#define NMC_IPC_SERIAL_TEST        	    (NMC_GET_IPC_FIRST+33)		// 串口测试结果
#define NMC_GET_IPC_SIGNAL_PROTOCOL     (NMC_GET_IPC_FIRST+34)		// 获取信号协议配置
#define NMC_GET_IPC_SCENCE_ID           (NMC_GET_IPC_FIRST+35)		// 获取当前场景ID
#define NMC_GET_IPC_INFO_SYNC           (NMC_GET_IPC_FIRST+36)		// 获取信息同步配置
#define NMC_GET_IPC_PARKPARAM           (NMC_GET_IPC_FIRST+37)      // 违停配置;



#define NMC_LAST						(NMC_FIRST+20000)	// generic to all controls

#define WM_FIRST						(NMC_LAST+1)		// send/post
#define WM_PREEVT						(WM_FIRST+2)		// 事件处理
#define WM_UPDATEPARAM					(WM_FIRST+3)		// 参数更新

//====== Generic WebEvent notification codes =================================

#define EVS_VIEW_TOOL					TEXT("viewtool")// 播放工具窗口
#define EVS_DEVTREE						TEXT("devtree")	// 播放设备树窗口
#define EVS_VIEW_PLAYLAYOUT				TEXT("viewplaylayout")	// 播放设备树窗口
#define EVS_VIEW_PLAY					TEXT("viewplay")	// 播放窗口
#define EVS_VIEW_TOPTOOLBAR				TEXT("viewplaytoptoolbar")	// 播放工具栏
#define EVS_VIEW_BOTTOMTOOLBAR			TEXT("viewplaybottomtoolbar")	// 播放工具栏
#define EVS_VIEW_ALARM					TEXT("viewalarm")	// 浏览告警区域
#define EVS_VIEW_ALARM_SHOW				TEXT("viewalarmshow")	// 浏览告警显示
#define EVS_VIEW_PLAYINFO				TEXT("viewplayinfo")	// 浏览信息
#define EVS_SPEED						TEXT("speed")		// 浏览告警区域
#define EVS_ALARM						TEXT("alarm")	// 告警
#define EVS_STATISTICS					TEXT("statistics")	// 统计
#define EVS_CONFIG						TEXT("config")	// 配置
#define EVS_TITLE						TEXT("title")	// 标题窗口
#define EVS_GETTITLE					TEXT("gettitle")// 获取标题头
#define EVS_GETMINIINFO					TEXT("getminiinfo")// 获取最小宽高信息
#define EVS_BASECONFIG					TEXT("baseconfig") //基础配置页面
#define EVS_VIEWSRC						TEXT("viewsrc")   //视频源配置
#define EVS_ALGCONFIG					TEXT("algconfig")  //烟火检测
#define EVS_DLG_VIEW_LAYOUT				TEXT("view")	// 预览窗口

// 以上为早期的

// 窗口类型
// 公共窗口类
#define EVS_CFG_OSD					               TEXT("its_cfg_osd")	           // OSD
#define EVS_CFG_PICMERGE			               TEXT("its_cfg_picmerge")  // 图片合成配置
#define EVS_DLG_ITS_SYSCFG_PLATE        TEXT("its_cfg_sysplate")     // 平台配置

// 服务器窗口类型
#define EVS_DLG_BASE				TEXT("base")			// 基础窗口
#define EVS_DLG_MAIN				TEXT("main")			// 主窗口
#define EVS_DLG_LOGIN				TEXT("login")			// 登录窗口
#define EVS_DLG_TST					TEXT("dlg_tst")			// 创建测试窗口
#define EVS_DLG_PICSHOW				TEXT("dlg_pichow")		// 图片显示
#define EVS_DLG_EVTLST				TEXT("dlg_evtlst")		// 创建事件显示窗口
#define EVS_DLG_ITS_CFG_IPCMGR		TEXT("its_cfg_dev")		// 挂载设备配置
#define EVS_DLG_ITS_IPCCFG_DEVINFO	TEXT("its_cfg_normal")	// 普通信息
#define EVS_DLG_ITS_IPCCFG_ASSDEV	TEXT("its_cfg_assdev")	// 辅助相机
#define EVS_DLG_ITS_CFG_SYSMGR		TEXT("its_cfg_sys")		// 系统配置界面
#define EVS_DLG_ITS_SYSCFG_INFO		TEXT("its_cfg_sysinfo")	// 系统信息
#define EVS_DLG_ITS_SYSCFG_REC		TEXT("its_cfg_sysrec")	// 录像配置
#define EVS_DLG_ITS_STATISTICS		TEXT("its_statistics")	// 统计
#define EVS_DLG_SERVER_IPCMGR       TEXT("server_ipcmgr")	// 设备管理界面

// IPC 窗口类
#define EVS_DLG_IPC_MGR				TEXT("ipc_mgr")			// ipc图片管理窗口
#define EVS_DLG_IPC_VIEW			TEXT("ipc_picview")		// 创建预览和事件显示窗口
#define EVS_DLG_IPC_PICINFO			TEXT("ipc_picinfo")		// ipc图片信息
#define EVS_DLG_IPC_PICVIEW_TOOL	TEXT("ipc_picview_tool")// ipc图片浏览工具栏
#define EVS_DLG_IPC_SCENE			TEXT("ipc_scene")		// ipc场景配置
#define EVS_DLG_IPC_DETECTION		TEXT("ipc_detection")	// ipc检测配置
#define EVS_DLG_IPC_LOGICLANE		TEXT("ipc_logiclane")	// ipc逻辑车道
#define EVS_DLG_IPC_IDENTPARAM		TEXT("ipc_identparam")	// ipc识别参数
#define EVS_DLG_IPC_VELOMETER 		TEXT("ipc_velometer")	// ipc视频测速
#define EVS_DLG_IPC_IMAGEQUALITY 	TEXT("ipc_imagequality")// ipc图片质量
#define EVS_DLG_IPC_REDLIGHT 		TEXT("ipc_redlight")	// ipc红灯测量
#define EVS_DLG_IPC_POWERSYN 		TEXT("ipc_powersyn")	// ipc电源同步 
#define EVS_DLG_IPC_LISTCTRL 		TEXT("ipc_listctrl")	// ipcList 控件 
#define EVS_DLG_IPC_SERIAL 		    TEXT("ipc_serial")		// ipc串口配置 
#define EVS_DLG_IPC_LOCALCFG 		TEXT("ipc_localcfg")	// ipc本地配置 
#define EVS_DLG_IPC_SYSMAINTENANCE  TEXT("ipc_sysmaintenance") //ipc系统维护窗口
#define EVS_DLG_IPC_PARKSCENE       TEXT("ipc_parkscene")	//ipc违停场景配置
#define EVS_DLG_IPC_VIDEO           TEXT("ipc_video")		//ipc图片浏览界面视频播放窗口
#define EVS_DLG_IPC_SDPIC           TEXT("ipc_sdpic")       //ipc抓拍管理窗口(sd卡存储图片查询)
#define EVS_DLG_IPC_BROWSE          TEXT("ipc_browse")      //ipc视频浏览
#define EVS_DLG_IPC_LIGHT           TEXT("ipc_light")       //ipc灯光亮度配置
#define EVS_DLG_IPC_DEVINFO 		TEXT("ipc_devinfo")     // ipc设备信息
#define EVS_DLG_IPC_SCENEPOLL 		TEXT("ipc_scenepoll")   // ipc场景轮巡
#define EVS_DLG_IPC_FLOWDETECTION	TEXT("ipc_flowdetection")// ipc流量检测
#define EVS_DLG_IPC_ILLEGALDICT 	TEXT("ipc_illegaldict") // ipc违法字典
#define EVS_DLG_IPC_MANUALCAP 	TEXT("ipc_manualcapture") // ipc违法抓拍
#define EVS_DLG_IPC_OTHER 	TEXT("ipc_other") // ipc其它配置
#define EVS_DLG_IPC_HARDWARE 	TEXT("ipc_hardware") // ipc硬件测试
#define EVS_DLG_IPC_SIGNAL_PROTOCOL TEXT("ipc_signal_protocol")		// ipc信号协议配置
#define EVS_DLG_IPC_INFO_SYNC TEXT("ipc_info_sync")		// ipc信息同步配置 


//----------------------
#define EVS_DLG_IPC_EPTZ			TEXT("ipc_eptz")		//ipcEPTZ窗口
#define EVS_DLG_IPC_EPTZCTRL		TEXT("ipc_eptzctrl")    //ipcEPTZ控制
 

// 事件类型
#define EVS_EVT_SET_UITYPE			TEXT("evt_uitype")		// 设置窗口类型
#define EVS_EVT_IS_LOGIN			TEXT("evt_islogin")		// 是否登录
#define EVS_EVT_ADDLINK				TEXT("evt_addlink")		// 建立连接
#define EVS_EVT_DELLINK				TEXT("evt_dellink")		// 删除连接
#define EVS_EVT_REGISTER			TEXT("evt_register")	// 注册
#define EVS_EVT_STARTRCV			TEXT("evt_startrecv")	// 开始接收
#define EVS_EVT_STOPRCV				TEXT("evt_startrecv")	// 停止接收
#define EVS_EVT_ADDDEVICE			TEXT("evt_add_device")  // 添加设备
#define EVS_EVT_DELDVICE			TEXT("evt_del_device")  // 删除设备
#define EVS_EVT_LOGIN				TEXT("evt_login")		// 登录: IP, PORT ,用户名， 密码；
#define EVS_EVT_SETIP				TEXT("evt_set_ip")		// 设置指定窗口IP
#define EVS_EVT_SET_IP_SVR			TEXT("evt_set_ip_svr")	// 系统参数设置，设置对应的nvr
#define EVS_EVT_SETIPCIP			TEXT("evt_set_ip_ipc")	// 设置ipc ip
#define EVS_EVT_SET_IP_SVRANDIPC	TEXT("evt_set_ip_svrandipc") // 参数设置,指定窗口
#define EVS_EVT_IS_EXIT				TEXT("evt_isexit")		// 是否退出

// 应答
#define ACK_ERR						TEXT("err")				// 
#define ACK_OK						TEXT("ok")				// 


//====== Generic normal notification codes =================================

#define USERMAXLEN					32				// 用户名的最大长度
#define PASSWORDMAXLEN				32				// 密码的最大长度
#define MAXPICBUF					6144000			// 最大图片缓冲区大小
#define MAX_PATH_LEN                256             // 最大路径长度

//====== Generic color notification codes =================================

#define COLORSEL					    0xFFFF0000		// 选中的对象颜色
#define COLOROVER					0xFFFF0000		// 选中的对象颜色
#define COLORLINENOR				0x0003a7fd		// 普通线条颜色
#define COLORLINEDRAWING		0xFFFFFF00		// 正在绘制线条颜色
#define COLOR_YELLOW             0xFFFFFF00
#define COLOR_WHITE                0xFFFFFFFF
#define COLOR_BLUE                  0xFF0000FF
#define COLOR_PURPLE              0xFF9400D3
#define COLOR_RED                    0xFFFF0000
#define COLOR_GREEN               0xFF00FF00
#define COLOR_BLACK               0xFF000000

//====== Generic size boder =================================

#define SIZBOX_BORDER				6				// 缩放拖动边框长度
#define POINT_R						3				// 单点绘制的半径



//====== Generic enum notification codes =================================
enum EMDATATYPE
{
	EMDATATYPE_NVR = 0,
	EMDATATYPE_IPC,
	EMDATATYPE_SERVER,
	EMDATATYPE_COUNT
};

// 字体配置类型
enum EFONTTYPE
{
	FONT_UI_SML = 0,
	FONT_UI_MID,
	FONT_UI_BIG,
	FONT_COUNT
};

// 界面配置类型
enum EUICONFIG
{
	CFG_UI_CLR = 0,
	CFG_UI_CLR_TXT,				// 文本颜色
	CFG_UI_CLR_TXT_SEL,			// 选中文本颜色
	CFG_UI_CLR_DLGBK,			// 普通窗口的颜色
	CFG_UI_CLR_LAYOUT_BK,		// 布局框的背景色
	CFG_UI_CLR_LAYOUT_BORDER,	// 布局框边框颜色
	CFG_UI_CLR_LAYOUT_SEL,		// 选中
	CFG_UI_CLR_LAYOUT_FLICKER,	// 闪烁
	CFG_UI_COUNT
};

// 视频浏览类型
enum EVIEWPLAYTYPE
{
	E_VPT_NULL = 0,
	E_VPT_VIDEO,
	E_VPT_AUDIO1,
	E_VPT_AUDIO2,
	E_VPT_AUDIO3,
	E_VPT_META,
	E_VPT_COUNT
};

// 码流请求协议类型类型
enum ESTREAMTRANSPORTTYPE
{
	E_STT_NULL = 0,
	E_STT_RTSP,
	E_STT_RTP,
	E_STT_COUNT
};

//====== Generic typedef notification codes =================================

typedef std::map<std::wstring , std::wstring> TMAPWS;
//typedef std::map<std::wstring , Image*> TMAPIMAGE;  先注释，暂未用到
typedef std::map<u32, std::wstring> TMAPDWWS;


//====== Generic struct notification codes =================================

#pragma pack(push, 4)

enum EMPLAYFLOW
{
	EPF_IDEL = 0,
	EPF_START,
	EPF_RUNNING,
	EPF_STOP
};
// 播放参数
typedef struct tagPlayInfo
{
	void*	pWnd;					// 窗口句柄
	int		nScreenIndex;			// 屏幕索引
	int		nLayoutIndex;			// 窗口位置索引
	u8		byState1;				// EMPLAYFLOW: 0.空闲， 1.请求浏览，3.浏览中， 3.停止浏览
	u8		byState2;				// 1，告警，2，下线
	u32		dwPuIP;					// 前端IP
	u16		wPuPort;				// 前端通信端口
	u8		byTransportType;		// 协议类型 ， ESTREAMTRANSPORTTYPE
	s8		szURL[129];				// rtsp请求URL	
	u32		dwPlayID;				// 播放句柄,前端回传的，用于控制播放
	void*   pDecMedia;				// 解码器指针

	//		
	u32		dwLocIP;				// 本地IP
	bool	abyStreamType[E_VPT_COUNT];	// 码流类型, true表示请求
	u16		awRtpPort[E_VPT_COUNT];	// 本地码流rtp端口
	u16		awRtcpPort[E_VPT_COUNT];	// 本地码流rtcp端口

	u32		dwReplayNum;			// 重复请求播放的次数

	tagPlayInfo()
	{
		empty();
	}

	void empty()
	{
		memset(this, 0, sizeof(tagPlayInfo));
	}
	void clear()
	{
		byState1	= 0;			// EMPLAYFLOW: 0.空闲， 1.请求浏览，3.浏览中， 3.停止浏览
		byState2	= 0;			// 1，告警，2，下线
		dwPuIP		= 0;			// 前端IP
		wPuPort		= 0;			// 前端通信端口
		byTransportType	= 0;		// 协议类型 ， ESTREAMTRANSPORTTYPE
		strcpy(szURL, "");			// rtsp请求URL	
		dwPlayID	= 0;			// 播放句柄,前端回传的，用于控制播放
		pDecMedia	= 0;			// 解码器指针	
		dwLocIP		= 0;			// 本地IP
		dwReplayNum	= 0;
		for (int i =0; i< E_VPT_COUNT; i++)
		{
			abyStreamType[i]= 0;	// 码流类型, true表示请求
			awRtpPort[i]	= 0;	// 本地码流rtp端口
			awRtcpPort[i]	= 0;	// 本地码流rtcp端口
		}
		
	}
	bool Compare(tagPlayInfo* p1)
	{
		if (dwPuIP		!= p1->dwPuIP	||
			wPuPort		!= p1->wPuPort	||
			byTransportType != p1->byTransportType ||
			strcmp(szURL, p1->szURL)	!= 0
			)
		{
			return false;
		}

		for(int i=0; i < E_VPT_COUNT;i++)
		{
			if (abyStreamType[i]	!= p1->abyStreamType[i])
				return false;
		}

		return true;
	}
}TPLAYINFO, *PTPLAYINFO;


// 布局信息，从配置文件中获取
typedef struct tagLayoutInfo
{
	int		nLayoutNum;			// 窗口数量
	int		xOrgin,yOrgin;		// 起始位置
	int		nWidth, nHeight;	// 区域划分
	u32		x[DLGMAX],y[DLGMAX],w[DLGMAX],h[DLGMAX];

	tagLayoutInfo()
	{
		empty();
	}

	void empty()
	{
		memset(this, 0, sizeof(tagLayoutInfo));
	}
}TLAYOUTINFO, *PTLAYOUTINFO;

// 预案信息
typedef struct tagPlanInfo
{
	wchar_t		wsLayoutType[33];		// 布局方案名称
	u16			wFullScreen;			// 全屏窗口索引，0，不全屏
	int			wFullWindow;			// 充满窗口的索引，0，不充满
	TPLAYINFO	tPlayInfo[DLGMAX];		// 布局监控点信息

	tagPlanInfo()
	{
		empty();
	}

	void empty()
	{
		memset(this, 0, sizeof(tagPlanInfo));
	}
}TPLANINFO, *PTPLANINFO;

// 设备信息
typedef struct tagDevInfo
{
	wchar_t		wsName[33];		// 布局方案名称
	u32			dwPuIP;			// 前端IP
	u16			wPuPort;		// 前端通信端口

	tagDevInfo()
	{
		empty();
	}

	void empty()
	{
		memset(this, 0, sizeof(tagDevInfo));
	}
}TDEVINFO, *PTDEVINFO;

// 告警类型
enum EALARMTYPE
{
	E_AT_NULL = 0,
	E_AT_HYJC,			// 火焰检测
	E_AT_MOREPEOPLE,	// 人数过多
	E_AT_COUNT
};

// 告警信息
typedef struct tagAlarmInfo_Its
{
	u32			dwType;					// 告警类型，EALARMTYPE
	u32			dwIP;					// 告警IP
	SYSTEMTIME	tDateTime;				// 告警时间
	wchar_t		szAddr[33];				// 地点
	wchar_t		szPicAddr[65];			// 图片地址
	wchar_t		szRecAddr[65];			// 录像地址
	SYSTEMTIME	tRecDateTimeStart;		// 录像开始时间
	SYSTEMTIME	tRecDateTimeEnd;		// 录像结束时间

	tagAlarmInfo_Its()
	{
		empty();
	}

	void empty()
	{
		memset(this, 0, sizeof(tagAlarmInfo_Its));
	}
}TALARMINFO_ITS, *PTALARMINFO_ITS;
typedef std::map<u32, TALARMINFO_ITS*> TMAPALARMINFO;
typedef std::map<u32, TALARMINFO_ITS*>::iterator TMAPALARMINFOITE;

#define  MAXNAMELEN	65			    // 名称的最大长度
#define  MAXPATHLEN	129				// 存储路径字符串长度
// 电子警察过车信息
typedef struct tagEvtVehicleInfo
{
	u32			dwId;
	u32         dwIP;
	KOSA_HANDLE	dwHandle;
	TVechileData tVechileData;
	wchar_t		szDir[MAXPATHLEN];	// 存储路径
	wchar_t		szImageName[MAX_NUM_IMAGE][MAXNAMELEN];

	tagEvtVehicleInfo()
	{
		empty();
	}

	void empty()
	{
		memset(this, 0, sizeof(tagEvtVehicleInfo));
	}
}TEVTVEHICLEINFO, *PTEVTVEHICLEINFO;
typedef std::map<u32, PTEVTVEHICLEINFO> TMAPEPEVTINFO;
typedef std::map<u32, PTEVTVEHICLEINFO>::iterator TMAPEPEVTINFOITE;

// 过车图片存储空间
struct TImageBuf
{
	u32		dwID;		// dwID == 0表示空闲
	u32		dwBufNum;	//
	u32		anBufType[MAX_NUM_IMAGE];
	u32		anBufLen[MAX_NUM_IMAGE];
	u32     anBufMaxLen[MAX_NUM_IMAGE];
	void*	apBuf[MAX_NUM_IMAGE];

	TImageBuf()
	{
		empty();
	}

	void empty()
	{
		memset(this, 0, sizeof(TImageBuf));
	}
};

// 用户类型
enum EUSERTYPE
{
	E_AT_NOR = 0

};
// 用户信息
typedef struct tagUserInfo_Its
{
	u32			dwType;
	wchar_t		szName[33];		// 名称
	wchar_t		szPassword[33];	// 用户名

	tagUserInfo_Its()
	{
		empty();
	}

	void empty()
	{
		memset(this, 0, sizeof(tagUserInfo_Its));
	}
}TUSERINFO_ITS, *PTUSERINFO_ITS;
typedef std::list<PTUSERINFO_ITS> TLSTUSERINFO, *PTLSTUSERINFO;
typedef std::list<PTUSERINFO_ITS>::iterator TLSTUSERINFOITE;

// 本地配置信息
typedef struct tagLocalCfgInfo
{
	wchar_t		picturepath[MAX_PATH_LEN];			// 本地图片存储路径
	wchar_t		videopath[MAX_PATH_LEN];	        // 本地视频存储路径
	u32         isrtsp;
	u32         istcp;
	u32         ispic;
	u32			isip;
	u32         isyear;
	u32         ismonth;
	u32			isday;
	u32			ishour;
	u32         isBayonet;
	u32         isBayonetMontage;
	u32         isillegal;
	u32         isillegalMontage;
	u32         isVideo;
	u32         isFace;
	u32         isVehicle;
	u32         isPlate;
	u32         isHardware;
	u32         serialNum;
	u32         serialRate;


	tagLocalCfgInfo()
	{
		empty();
	}

	void empty()
	{
		memset(this, 0, sizeof(tagLocalCfgInfo));
	}
}TLocalCfgInfo, *PTLocalCfgInfo;


typedef std::map<u32, u32> TMAPEVTINFO;
typedef std::map<u32, u32>::iterator TMAPEVTINFOITE;


//图片合成
typedef struct tagPhotoMergeInfo
{
	//图片合成参数

	//合成图片OSD参数
}TPhotoMergeInfo, *PTPhotoMergeInfo;

// 绘制类型
enum EMDRAWMODE
{
	EMDRAWMODE_NONE  = 0,
	// 直线
	EMDRAWMODE_LINE,
	// 矩形
	EMDRAWMODE_RECT,
	// 多边形
	EMDRAWMODE_RGN,
	// 四边形
	EMDRAWMODE_RGN_4,
	// 梯形
	EMDRAWMODE_TRAPEZIUM,
	EMDRAWMODE_COUNT
};
#define  MAXDRAWPOINTS 8   // 最多绘制8个点
typedef struct tagDrawData
{
	int nType;
	int nDrawMode;
	int nId;
	int color;
	//bool isHide;
	int nPtNum;
	int x[MAXDRAWPOINTS];
	int y[MAXDRAWPOINTS];
	bool isHide;
	Uint32 attribute;


	void Empty()
	{
		memset(this, 0, sizeof(tagDrawData));
	}

	tagDrawData()
	{
		Empty();
	}

	void addPT(int x1, int y1)
	{
		x[nPtNum] = x1;
		y[nPtNum] = y1;
		nPtNum++;
	}

	void updatePT(int x1, int y1, int nIndex = -1)
	{
		if(nIndex == -1)
			nIndex = nPtNum;
		x[nIndex] = x1;
		y[nIndex] = y1;
	}

	void delPT()
	{
		x[nPtNum] = 0;
		y[nPtNum] = 0;
		if (nPtNum > 0) 
			nPtNum--;
	}

	void clearPT()
	{
		nPtNum = 0;
		memset(x, 0, sizeof(x));
		memset(y, 0, sizeof(y));
	}

}TDrawData, *PTDrawData;

// 视频浏览信息
enum EMVIDEOSTATE
{
	EMVIDEOSTATE_STOP = 1,		// 停止
	EMVIDEOSTATE_START,			// 开始
	EMVIDEOSTATE_RELEASE,		// 释放
};
typedef struct tagVideoInfo
{
	bool	bSet;			// 是否设置
	bool	bReqing;		// 是否请求中
	u32		dwReqResult;	// 请求结果, 0, 无， 1，请求失败,其它错误码
	u32		dwReqTime;		// 请求的时间
	u8		byFlow;			// EMVIDEOSTATE
	u8		byNextFlow;    // 回传后比对一样不处理，不一样处理
	u8		byVideoType;	// 
	u32		dwContext;		// 上下文,暂不使用，给回调模式使用
	u32		dwErr;			// 1:参数未设置
	u32		dwLocalIP;		
	u32		dwIP;
	u16		wPort;
	u32		dwVideoHandle;
	char	szUserName[65];
	char	szPassword[65];

	void Empty()
	{
		memset(this, 0, sizeof(tagVideoInfo));
	}

	tagVideoInfo()
	{
		Empty();
	}

}TVideoInfo, *PTVideoInfo;

// 恢复系统默认字节对齐方式
#pragma pack(pop)

