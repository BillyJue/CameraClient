#pragma once
#include <string.h>
#include <windows.h>
//#include "kdvtype.h"
// 常量定义
typedef int            s32, BOOL32;
typedef unsigned long   u32;
typedef unsigned char   u8;
typedef unsigned short  u16;
typedef short          s16;
typedef char           s8;


//
#define DT_MAGIC	"NDT1"							// 标识头1
#define DT_MAGIC2	"NDT2"							// 标识头2
#define DT_PROTO_VER "1.3.0.0"						// 协议版本号
#define DT_AUTHENTICATION_ID		0x3210			// 鉴权
#define DT_B2_EXT_TAG				0xF7F7			// 如果携带该字段，则表示会携带DEV VERSION
#define DT_V7_EXT_TAG				0xF7F8			// 如果携带该字段，则表示会携带DEV VERSION、DEVICE STATE以及AUTH NONCE

// 设备重置请求动作
#define ACTION_ID_ADMIN				0x0001			// 重置管理员密码
#define ACTION_ID_BW_LIST			0x0002			// 黑白名单
#define ACTION_ID_RECOVER			0x0004			// 恢复出厂

// 激活状态
#define ACTIVE_STATE_NOTFINISHED	0x0000         // 未激活
#define ACTIVE_STATE_FINISHED		0x0001         // 已激活
#define ACTIVE_STATE_EXCEPTION		0xFFFF         // 设备异常

#define DT_LEN_MAGIC			4                 // 标识头长度
#define DT_LEN_VERSION			4                 // 版本号长度
#define DT_LEN_MACADDR			6					// MAC地址长度
#define DT_LEN_USER_PWD_MD5		16					// MD5加密后长度
#define DT_LEN_DNS				64					// DNS
#define DT_LEN_NAME				32					// 用户名
#define DT_LEN_USER_PWD			32					// 密码长度
#define DT_LEN_FTP_USER			64
#define DT_LEN_FTP_PASSWD		64
#define DT_LEN_FILE_PATH		260					//windows: MAX_PATH
#define DT_LEN_PROTO_VER		16					// 协议版本号长度
#define DT_LEN_SOFT_VER			64					// 软件版本号长度
#define DT_LEN_SECURE_CODE		32					// 安全码长度
#define DT_LEN_SECURE_MAIL		64					// 安全邮箱长度
#define DT_LEN_SERIAL			10					// 设备序列号
#define DT_LEN_UPDATE_INFO		14					// 设备升级信息

//
const u16 DT_PORT = 5513;							//
const u16 MULTICAST_PORT = 5512;					// 多播地址
const u16 MAXLEN_BCMSG = 1024;						//

// 错误码
const u32 DT_ERR_OK = 0;							// 操作成功
const u32 DT_ERR_SET_IP_FAILED = 1;					// 设置IP失败
const u32 DT_ERR_CONNECT_CHECK_FAILED = 2;			// FTP连接失败
const u32 DT_ERR_FILE_PATH_WRONG = 3;			// 文件路径错误
const u32 DT_ERR_UPDATE_FILE_CREATE_FAILED = 4;		// 升级文件创建失败
const u32 DT_ERR_DATABASE_OP_FAILED = 5;
const u32 DT_ERR_UPDATE_FILE_CHECK_FAILED = 6;		// 升级文件检查失败
const u32 DT_ERR_SYSUPDATE_FAILED = 7;				// 操作失败
const u32 DT_ERR_APPLOAD_MISS = 8;				// 缺少apploader
const u32 DT_ERR_AUTHEN_FAILED = 9;					// 鉴权失败
const u32 DT_ERR_ACTIVATE_WRONG = 10;				// 设备未激活
const u32 DT_ERR_AUTHEN_TIMES_FAILED = 11;			// 鉴权失败次数过多
const u32 DT_ERR_OLD_VERSION_WRONG = 12;			// 不支持的旧版指令
const u32 DT_ERR_UPDATE_NOACK = 13;					// 升级请求无回应
const u32 DT_ERR_SET_GATEWAY_FAILED = 14;			// 设置IP失败(网关失败)


const u32 DT_NETMASK_MODADDR = 0x01;
const u32 DT_NETMASK_MODREGPLAT = 0x02;
// 
//typedef void (*MsgAckCB)( UINT wMsg, LPVOID pParam, DWORD dwContext); 没用到吧


// 消息ID定义
#define DECLARE_MESSAGE(m, id) const u16 m = id;

DECLARE_MESSAGE(NC_DEV_TRACK_REQ_CAST, 0x0001)		// 设备探测广播消息：TDTHeader
DECLARE_MESSAGE(DEV_NC_TRACK_ACK_CAST, 0x0002)		// 设备信息通知消息：TDTHeader+TDTDevInfo

DECLARE_MESSAGE(NC_DEV_MODADDR_REQ_CAST, 0x0003)		// 修改设备地址消息: TDTHeader+TDTDevNetParam
DECLARE_MESSAGE(DEV_NC_MODADDR_ACK_CAST, 0x0004)		// 修改设备地址消息: TDTHeader+u32(ErrorCode) (待定)

DECLARE_MESSAGE(NC_DEV_REBOOT_REQ_CAST, 0x0005)		// 重启设备消息，不需要回馈: TDTHeader

DECLARE_MESSAGE(NC_DEV_VERSION_REQ_CAST, 0x0006)		//获取版本信息: TDTHeader
DECLARE_MESSAGE(DEV_NC_VERSION_ACK_CAST, 0x0007)		//获取版本信息应答: TDTHeader+achVersion[32]

DECLARE_MESSAGE(NC_DEV_FTPUPDATE_REQ, 0x0008)		// FTP升级 TDTHeader+TDTFtpUpdateInfo
DECLARE_MESSAGE(DEV_NC_FTPUPDATE_ACK, 0x0009)		// FTP升级 TDTHeader+u32(ErrorCode)

DECLARE_MESSAGE(NC_DEV_FTPCFG_REQ, 0x0010)		// FTP升级配置文件 TDTHeader+TDTFtpUpdateInfo
DECLARE_MESSAGE(DEV_NC_FTPCFG_ACK, 0x0011)		// FTP升级配置文件 TDTHeader+u32(ErrorCode)

DECLARE_MESSAGE(NC_DEV_RESTORESETTING_REQ, 0x0012)		//恢复出厂设置 TDTHeader+TDTRestoreInfo
DECLARE_MESSAGE(DEV_NC_RESTORESETTING_ACK, 0x0013)		//恢复出厂设置 TDTHeader+u32(ErrorCode)

DECLARE_MESSAGE(NC_DEV_ACTIVATE_REQ, 0x0014)		//设备激活请求 TDTHeader+TDTRestoreInfo
DECLARE_MESSAGE(DEV_NC_ACTIVATE_ACK, 0x0015)		//设备激活请求 TDTHeader+u32(ErrorCode)

DECLARE_MESSAGE(NC_DEV_PWD_CHANGE_REQ, 0x0016)		//用户密码修改请求 TDTHeader+TDTRestoreInfo
DECLARE_MESSAGE(DEV_NC_PWD_CHANGE_ACK, 0x0017)		//用户密码修改请求 TDTHeader+u32(ErrorCode)

DECLARE_MESSAGE(DEV_NC_REBOOT_ACK, 0x0018)		//重启应答 TDTHeader+u32(ErrorCode)

DECLARE_MESSAGE(NC_DEV_SSH_REQ, 0x0019)		//SSH配置请求 TDTHeader+u16
DECLARE_MESSAGE(DEV_NC_SSH_ACK, 0x001A)		//SSH配置应答 TDTHeader+u32(ErrorCode)

DECLARE_MESSAGE(NC_DEV_UPDATE_INFO_REQ_CAST, 0x001B)		//获取升级信息: TDTHeader
DECLARE_MESSAGE(DEV_NC_UPDATE_INFO_ACK_CAST, 0x001C)		//获取升级信息应答: TDTHeader+TDTFtpUpdateInfo

// 结构体定义

// Windows OS下字节紧缩对齐
#ifdef WIN32
#pragma pack( push )
#pragma pack( 1 )
#define window( x )	x
#else
#include <netinet/in.h>
#define window( x )
#endif

struct TDMacAddr
{
	u8  abyMac[DT_LEN_MACADDR];						// MAC地址
};

struct TDTHeader
{
	TDMacAddr  tMacSrc;					// 发送者MAC地址(CHAR[12])  
	TDMacAddr  tMacDst;					// 接收者MAC地址(CHAR[12]) 	
	u16 wMsgID;											// 消息ID
}
#ifndef WIN32
__attribute__((packed))
#endif
;

struct TDAuthen											// 鉴权信息
{
	s8 abyUser[DT_LEN_NAME];							// 鉴权用户名
	u8 abyPwd[DT_LEN_USER_PWD_MD5];						// 鉴权密码
	u8 bAuthType;										// 鉴权类型0x00表示用户名密码方式，0x01表示安全码方式
	u8 bReserved;										// 新密码长度，仅适用于修改密码和激活

	TDAuthen()
	{
		memset(this, 0, sizeof(TDAuthen));
	}
}
#ifndef WIN32
__attribute__((packed))
#endif
;

struct TDTHeaderAuthen									// 需要加鉴权的消息头
{
	TDMacAddr  tMacSrc;									// 发送者MAC地址(CHAR[12])  
	TDMacAddr  tMacDst;									// 接收者MAC地址(CHAR[12]) 	
	u16	wAuthen;										// 鉴权ID
	u16 wMsgID;											// 消息ID
	TDAuthen tAuthen;									// 鉴权信息
};
#ifndef WIN32
__attribute__((packed))
#endif
;

struct TDTDevAddr
{
	// DHCP服务器类型，第一字节: 0-没有DHCP服务 1-服务器，2-客户端 
	// 第二字节: 0-关闭 1-开启；例如：0x0000表示没有服务器，0x0201表示客户端(开启)
	u16 wDHCPOpen;										// 0-关闭 1-开启DHCP
	u32 dwIpaddr;										// IP地址，网络序
	u32 dwNetmask;										// 子网掩码，网络序
	u32 dwGateWay;										// 默认网关，网络序
}
#ifndef WIN32
__attribute__((packed))
#endif
;

struct TDTDvsRegPlat
{
	u8 byRegIPType;										// 注册地址类型, 0-ip地址、 1-dns
	u8 byIsCMS;											// 是否启用负载均衡， 0-不启用，1-启用
	u32 dwRegIP;										// 网络序，负载均衡的IP以及平台的ip地址公用
	s8  achRegDNS[DT_LEN_DNS];							// 注册平台DNS
	u16 wRegPort;										// 注册平台端口，只有在启用负责均衡时才需修改，其他默认5510
}
#ifndef WIN32
__attribute__((packed))
#endif
;

// 平台支持功能
typedef struct tagPlatForm
{
	u8 bR : 1;											// 重启 Manual Reboot Mark
	u8 bF : 1;											// 升级 FTP Upgrate Mark
	u8 bReserved1 : 1;
	u8 bReserved2 : 1;
	u8 bReserved3 : 1;
	u8 bReserved4 : 1;
	u8 bReserved5 : 1;
	u8 bReserved6 : 1;
}TPlatForm
;

struct TDTDevInfo
{
	TDTDevAddr tAddr;									// IP、MASK、GW
	u32 dwTickCount;									// 运行时间
	u32 dwBrdID;										// 设备类型ID
	TDTDvsRegPlat tRegPlat;						// 前端注册平台参数
	u8 byDevIdMask;										// 标识后续是否有DevId信息(1:有;0:无 2:DEVID-NAME使用UTF-8编码)
	u8 byDevTypeMask;									// 是否有设备类型字符串(1:有;0:无)，为1时，设备信息在TDTDevID后还有TDTDevType信息
	u8 bySrvTypeMask;									// 服务提供类型（0:IPC(ipdt)，1:VS前端(KDMSearch))
	u8 byMask3;											//判断升级系列modify by xuqinglai at 20101213
	TPlatForm tPlatForm;								// 平台支持功能
	u8 bReserved1;
	u8 bReserved2;
	u8 bReserved3;
}
#ifndef WIN32
__attribute__((packed))
#endif
;

struct TDTDevID
{
	s8 achDevName[99];						// 设备名称
	s8 achDevSerial[DT_LEN_SERIAL + 1];					// 设备序列号
	s8 achDevType[DT_LEN_NAME + 1];						// 设备类型名称
	s8 achDevUpdate[DT_LEN_UPDATE_INFO + 1];				// 设备升级信息
}
#ifndef WIN32
__attribute__((packed))
#endif
;

struct TDTDevEx											// 扩展信息
{
	s8 achDevSoftVersion[DT_LEN_SOFT_VER];				// 软件版本号
	u16	wActiveState;									// 设备激活状态 0xFFFF-设备异常，0x0000-设备未激活，0x0001-设备已激活
	u8 achSecureCode[DT_LEN_SECURE_CODE];				// 安全码
	s8 achProTocolVer[DT_LEN_PROTO_VER];				// 协议版本号
	s8 achSecureMail[DT_LEN_SECURE_MAIL];				// 安全邮箱
}
#ifndef WIN32
__attribute__((packed))
#endif
;

struct TDevItem
{
	TDMacAddr tDevMacAddr;								// 设备MAC地址
	TDTDevInfo tDevInfo;								// 基本信息
	TDTDevID tDevID;									// ID
	u16 wTag;											// 扩展标识位
	TDTDevEx tDevEx;									// 扩展位
	BOOL bNDT2;											// 协议头是否为NDT2

	TDevItem()
	{
		memset(this, 0, sizeof(TDevItem));
	}
};

struct TDevRetInfo
{
	TDMacAddr tDevMacAddr;									// 设备MAC地址
	u32 dwErrorCode;										// 错误码

	TDevRetInfo()
	{
		memset(this, 0, sizeof(TDevRetInfo));
	}
};

struct TDevUpProcess
{
	TDMacAddr tDevMacAddr;									// 设备MAC地址
	u32 dwErrorCode;										// 0—查询请求回复。
	//1—升级进度，RESERVE携带的就是升级进度值（1~100,100代表业务升级操作完成）。
	//2—升级状态，RESERVE携带的就是升级最终的状态（0-失败，1-成功，2-状态未知）。
	u16 wMark;
	u32 dwReserve;

	TDevUpProcess()
	{
		memset(this, 0, sizeof(TDevUpProcess));
	}
};

//////////////////////////////////////////////////////////////////////////////////////
struct TMACAddr
{
	u8 abyFields[DT_LEN_MACADDR];

	TMACAddr()
	{
		memset(this, 0, sizeof(TMACAddr));
	}
};

struct TIPInfo
{
	u8 abyIpFields[4 * 4];
	u8 abyIPMaskFields[4 * 4];
	TIPInfo()
	{
		memset(this, 0, sizeof(TIPInfo));
	}
};

struct TDTFtpUpdateInfo
{
	u16 wFileType;
	u32 dwFtpServerIp;
	u16 wFtpServerPort;
	s8 achFtpUser[DT_LEN_FTP_USER];
	s8 achFtpPasswd[DT_LEN_FTP_PASSWD];
	s8 achFilePath[DT_LEN_FILE_PATH];
};

typedef struct tagDevNetParam
{
	u32 dwNetMask;					// DT_NETMASK_MODADDR -表示地址有效 DT_NETMASK_MODREGPLAT-表示注册地址有效，需要修改
	TDTDevAddr tAddr;		// IP、MASK、GW
	TDTDvsRegPlat tRegPlat;	// 前端注册平台参数
	u32 dwReserved1;
	u32 dwReserved2;

	tagDevNetParam()
	{
		memset(this, 0, sizeof(tagDevNetParam));
	}
}TDevNetParam, *PTDevNetParam;

typedef struct tagTDragDeviceInfo
{
	char m_sDeviceName[256];
	char m_sDeviceIp[64];
	char m_sDeviceType[32];
	char m_sDeviceKind[32];	// 大类., DEVICE_KIND_2800, DEVICE_KIND_PU
	char m_sReserve[256];

	tagTDragDeviceInfo()
	{
		memset(this, 0, sizeof(tagTDragDeviceInfo));
	}
}TDragDeviceInfo, *PTDragDeviceInfo;