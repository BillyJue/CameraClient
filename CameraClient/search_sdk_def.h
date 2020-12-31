#pragma once
#include <string.h>
#include <windows.h>
//#include "kdvtype.h"
// ��������
typedef int            s32, BOOL32;
typedef unsigned long   u32;
typedef unsigned char   u8;
typedef unsigned short  u16;
typedef short          s16;
typedef char           s8;


//
#define DT_MAGIC	"NDT1"							// ��ʶͷ1
#define DT_MAGIC2	"NDT2"							// ��ʶͷ2
#define DT_PROTO_VER "1.3.0.0"						// Э��汾��
#define DT_AUTHENTICATION_ID		0x3210			// ��Ȩ
#define DT_B2_EXT_TAG				0xF7F7			// ���Я�����ֶΣ����ʾ��Я��DEV VERSION
#define DT_V7_EXT_TAG				0xF7F8			// ���Я�����ֶΣ����ʾ��Я��DEV VERSION��DEVICE STATE�Լ�AUTH NONCE

// �豸����������
#define ACTION_ID_ADMIN				0x0001			// ���ù���Ա����
#define ACTION_ID_BW_LIST			0x0002			// �ڰ�����
#define ACTION_ID_RECOVER			0x0004			// �ָ�����

// ����״̬
#define ACTIVE_STATE_NOTFINISHED	0x0000         // δ����
#define ACTIVE_STATE_FINISHED		0x0001         // �Ѽ���
#define ACTIVE_STATE_EXCEPTION		0xFFFF         // �豸�쳣

#define DT_LEN_MAGIC			4                 // ��ʶͷ����
#define DT_LEN_VERSION			4                 // �汾�ų���
#define DT_LEN_MACADDR			6					// MAC��ַ����
#define DT_LEN_USER_PWD_MD5		16					// MD5���ܺ󳤶�
#define DT_LEN_DNS				64					// DNS
#define DT_LEN_NAME				32					// �û���
#define DT_LEN_USER_PWD			32					// ���볤��
#define DT_LEN_FTP_USER			64
#define DT_LEN_FTP_PASSWD		64
#define DT_LEN_FILE_PATH		260					//windows: MAX_PATH
#define DT_LEN_PROTO_VER		16					// Э��汾�ų���
#define DT_LEN_SOFT_VER			64					// ����汾�ų���
#define DT_LEN_SECURE_CODE		32					// ��ȫ�볤��
#define DT_LEN_SECURE_MAIL		64					// ��ȫ���䳤��
#define DT_LEN_SERIAL			10					// �豸���к�
#define DT_LEN_UPDATE_INFO		14					// �豸������Ϣ

//
const u16 DT_PORT = 5513;							//
const u16 MULTICAST_PORT = 5512;					// �ಥ��ַ
const u16 MAXLEN_BCMSG = 1024;						//

// ������
const u32 DT_ERR_OK = 0;							// �����ɹ�
const u32 DT_ERR_SET_IP_FAILED = 1;					// ����IPʧ��
const u32 DT_ERR_CONNECT_CHECK_FAILED = 2;			// FTP����ʧ��
const u32 DT_ERR_FILE_PATH_WRONG = 3;			// �ļ�·������
const u32 DT_ERR_UPDATE_FILE_CREATE_FAILED = 4;		// �����ļ�����ʧ��
const u32 DT_ERR_DATABASE_OP_FAILED = 5;
const u32 DT_ERR_UPDATE_FILE_CHECK_FAILED = 6;		// �����ļ����ʧ��
const u32 DT_ERR_SYSUPDATE_FAILED = 7;				// ����ʧ��
const u32 DT_ERR_APPLOAD_MISS = 8;				// ȱ��apploader
const u32 DT_ERR_AUTHEN_FAILED = 9;					// ��Ȩʧ��
const u32 DT_ERR_ACTIVATE_WRONG = 10;				// �豸δ����
const u32 DT_ERR_AUTHEN_TIMES_FAILED = 11;			// ��Ȩʧ�ܴ�������
const u32 DT_ERR_OLD_VERSION_WRONG = 12;			// ��֧�ֵľɰ�ָ��
const u32 DT_ERR_UPDATE_NOACK = 13;					// ���������޻�Ӧ
const u32 DT_ERR_SET_GATEWAY_FAILED = 14;			// ����IPʧ��(����ʧ��)


const u32 DT_NETMASK_MODADDR = 0x01;
const u32 DT_NETMASK_MODREGPLAT = 0x02;
// 
//typedef void (*MsgAckCB)( UINT wMsg, LPVOID pParam, DWORD dwContext); û�õ���


// ��ϢID����
#define DECLARE_MESSAGE(m, id) const u16 m = id;

DECLARE_MESSAGE(NC_DEV_TRACK_REQ_CAST, 0x0001)		// �豸̽��㲥��Ϣ��TDTHeader
DECLARE_MESSAGE(DEV_NC_TRACK_ACK_CAST, 0x0002)		// �豸��Ϣ֪ͨ��Ϣ��TDTHeader+TDTDevInfo

DECLARE_MESSAGE(NC_DEV_MODADDR_REQ_CAST, 0x0003)		// �޸��豸��ַ��Ϣ: TDTHeader+TDTDevNetParam
DECLARE_MESSAGE(DEV_NC_MODADDR_ACK_CAST, 0x0004)		// �޸��豸��ַ��Ϣ: TDTHeader+u32(ErrorCode) (����)

DECLARE_MESSAGE(NC_DEV_REBOOT_REQ_CAST, 0x0005)		// �����豸��Ϣ������Ҫ����: TDTHeader

DECLARE_MESSAGE(NC_DEV_VERSION_REQ_CAST, 0x0006)		//��ȡ�汾��Ϣ: TDTHeader
DECLARE_MESSAGE(DEV_NC_VERSION_ACK_CAST, 0x0007)		//��ȡ�汾��ϢӦ��: TDTHeader+achVersion[32]

DECLARE_MESSAGE(NC_DEV_FTPUPDATE_REQ, 0x0008)		// FTP���� TDTHeader+TDTFtpUpdateInfo
DECLARE_MESSAGE(DEV_NC_FTPUPDATE_ACK, 0x0009)		// FTP���� TDTHeader+u32(ErrorCode)

DECLARE_MESSAGE(NC_DEV_FTPCFG_REQ, 0x0010)		// FTP���������ļ� TDTHeader+TDTFtpUpdateInfo
DECLARE_MESSAGE(DEV_NC_FTPCFG_ACK, 0x0011)		// FTP���������ļ� TDTHeader+u32(ErrorCode)

DECLARE_MESSAGE(NC_DEV_RESTORESETTING_REQ, 0x0012)		//�ָ��������� TDTHeader+TDTRestoreInfo
DECLARE_MESSAGE(DEV_NC_RESTORESETTING_ACK, 0x0013)		//�ָ��������� TDTHeader+u32(ErrorCode)

DECLARE_MESSAGE(NC_DEV_ACTIVATE_REQ, 0x0014)		//�豸�������� TDTHeader+TDTRestoreInfo
DECLARE_MESSAGE(DEV_NC_ACTIVATE_ACK, 0x0015)		//�豸�������� TDTHeader+u32(ErrorCode)

DECLARE_MESSAGE(NC_DEV_PWD_CHANGE_REQ, 0x0016)		//�û������޸����� TDTHeader+TDTRestoreInfo
DECLARE_MESSAGE(DEV_NC_PWD_CHANGE_ACK, 0x0017)		//�û������޸����� TDTHeader+u32(ErrorCode)

DECLARE_MESSAGE(DEV_NC_REBOOT_ACK, 0x0018)		//����Ӧ�� TDTHeader+u32(ErrorCode)

DECLARE_MESSAGE(NC_DEV_SSH_REQ, 0x0019)		//SSH�������� TDTHeader+u16
DECLARE_MESSAGE(DEV_NC_SSH_ACK, 0x001A)		//SSH����Ӧ�� TDTHeader+u32(ErrorCode)

DECLARE_MESSAGE(NC_DEV_UPDATE_INFO_REQ_CAST, 0x001B)		//��ȡ������Ϣ: TDTHeader
DECLARE_MESSAGE(DEV_NC_UPDATE_INFO_ACK_CAST, 0x001C)		//��ȡ������ϢӦ��: TDTHeader+TDTFtpUpdateInfo

// �ṹ�嶨��

// Windows OS���ֽڽ�������
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
	u8  abyMac[DT_LEN_MACADDR];						// MAC��ַ
};

struct TDTHeader
{
	TDMacAddr  tMacSrc;					// ������MAC��ַ(CHAR[12])  
	TDMacAddr  tMacDst;					// ������MAC��ַ(CHAR[12]) 	
	u16 wMsgID;											// ��ϢID
}
#ifndef WIN32
__attribute__((packed))
#endif
;

struct TDAuthen											// ��Ȩ��Ϣ
{
	s8 abyUser[DT_LEN_NAME];							// ��Ȩ�û���
	u8 abyPwd[DT_LEN_USER_PWD_MD5];						// ��Ȩ����
	u8 bAuthType;										// ��Ȩ����0x00��ʾ�û������뷽ʽ��0x01��ʾ��ȫ�뷽ʽ
	u8 bReserved;										// �����볤�ȣ����������޸�����ͼ���

	TDAuthen()
	{
		memset(this, 0, sizeof(TDAuthen));
	}
}
#ifndef WIN32
__attribute__((packed))
#endif
;

struct TDTHeaderAuthen									// ��Ҫ�Ӽ�Ȩ����Ϣͷ
{
	TDMacAddr  tMacSrc;									// ������MAC��ַ(CHAR[12])  
	TDMacAddr  tMacDst;									// ������MAC��ַ(CHAR[12]) 	
	u16	wAuthen;										// ��ȨID
	u16 wMsgID;											// ��ϢID
	TDAuthen tAuthen;									// ��Ȩ��Ϣ
};
#ifndef WIN32
__attribute__((packed))
#endif
;

struct TDTDevAddr
{
	// DHCP���������ͣ���һ�ֽ�: 0-û��DHCP���� 1-��������2-�ͻ��� 
	// �ڶ��ֽ�: 0-�ر� 1-���������磺0x0000��ʾû�з�������0x0201��ʾ�ͻ���(����)
	u16 wDHCPOpen;										// 0-�ر� 1-����DHCP
	u32 dwIpaddr;										// IP��ַ��������
	u32 dwNetmask;										// �������룬������
	u32 dwGateWay;										// Ĭ�����أ�������
}
#ifndef WIN32
__attribute__((packed))
#endif
;

struct TDTDvsRegPlat
{
	u8 byRegIPType;										// ע���ַ����, 0-ip��ַ�� 1-dns
	u8 byIsCMS;											// �Ƿ����ø��ؾ��⣬ 0-�����ã�1-����
	u32 dwRegIP;										// �����򣬸��ؾ����IP�Լ�ƽ̨��ip��ַ����
	s8  achRegDNS[DT_LEN_DNS];							// ע��ƽ̨DNS
	u16 wRegPort;										// ע��ƽ̨�˿ڣ�ֻ�������ø������ʱ�����޸ģ�����Ĭ��5510
}
#ifndef WIN32
__attribute__((packed))
#endif
;

// ƽ̨֧�ֹ���
typedef struct tagPlatForm
{
	u8 bR : 1;											// ���� Manual Reboot Mark
	u8 bF : 1;											// ���� FTP Upgrate Mark
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
	TDTDevAddr tAddr;									// IP��MASK��GW
	u32 dwTickCount;									// ����ʱ��
	u32 dwBrdID;										// �豸����ID
	TDTDvsRegPlat tRegPlat;						// ǰ��ע��ƽ̨����
	u8 byDevIdMask;										// ��ʶ�����Ƿ���DevId��Ϣ(1:��;0:�� 2:DEVID-NAMEʹ��UTF-8����)
	u8 byDevTypeMask;									// �Ƿ����豸�����ַ���(1:��;0:��)��Ϊ1ʱ���豸��Ϣ��TDTDevID����TDTDevType��Ϣ
	u8 bySrvTypeMask;									// �����ṩ���ͣ�0:IPC(ipdt)��1:VSǰ��(KDMSearch))
	u8 byMask3;											//�ж�����ϵ��modify by xuqinglai at 20101213
	TPlatForm tPlatForm;								// ƽ̨֧�ֹ���
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
	s8 achDevName[99];						// �豸����
	s8 achDevSerial[DT_LEN_SERIAL + 1];					// �豸���к�
	s8 achDevType[DT_LEN_NAME + 1];						// �豸��������
	s8 achDevUpdate[DT_LEN_UPDATE_INFO + 1];				// �豸������Ϣ
}
#ifndef WIN32
__attribute__((packed))
#endif
;

struct TDTDevEx											// ��չ��Ϣ
{
	s8 achDevSoftVersion[DT_LEN_SOFT_VER];				// ����汾��
	u16	wActiveState;									// �豸����״̬ 0xFFFF-�豸�쳣��0x0000-�豸δ���0x0001-�豸�Ѽ���
	u8 achSecureCode[DT_LEN_SECURE_CODE];				// ��ȫ��
	s8 achProTocolVer[DT_LEN_PROTO_VER];				// Э��汾��
	s8 achSecureMail[DT_LEN_SECURE_MAIL];				// ��ȫ����
}
#ifndef WIN32
__attribute__((packed))
#endif
;

struct TDevItem
{
	TDMacAddr tDevMacAddr;								// �豸MAC��ַ
	TDTDevInfo tDevInfo;								// ������Ϣ
	TDTDevID tDevID;									// ID
	u16 wTag;											// ��չ��ʶλ
	TDTDevEx tDevEx;									// ��չλ
	BOOL bNDT2;											// Э��ͷ�Ƿ�ΪNDT2

	TDevItem()
	{
		memset(this, 0, sizeof(TDevItem));
	}
};

struct TDevRetInfo
{
	TDMacAddr tDevMacAddr;									// �豸MAC��ַ
	u32 dwErrorCode;										// ������

	TDevRetInfo()
	{
		memset(this, 0, sizeof(TDevRetInfo));
	}
};

struct TDevUpProcess
{
	TDMacAddr tDevMacAddr;									// �豸MAC��ַ
	u32 dwErrorCode;										// 0����ѯ����ظ���
	//1���������ȣ�RESERVEЯ���ľ�����������ֵ��1~100,100����ҵ������������ɣ���
	//2������״̬��RESERVEЯ���ľ����������յ�״̬��0-ʧ�ܣ�1-�ɹ���2-״̬δ֪����
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
	u32 dwNetMask;					// DT_NETMASK_MODADDR -��ʾ��ַ��Ч DT_NETMASK_MODREGPLAT-��ʾע���ַ��Ч����Ҫ�޸�
	TDTDevAddr tAddr;		// IP��MASK��GW
	TDTDvsRegPlat tRegPlat;	// ǰ��ע��ƽ̨����
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
	char m_sDeviceKind[32];	// ����., DEVICE_KIND_2800, DEVICE_KIND_PU
	char m_sReserve[256];

	tagTDragDeviceInfo()
	{
		memset(this, 0, sizeof(tagTDragDeviceInfo));
	}
}TDragDeviceInfo, *PTDragDeviceInfo;