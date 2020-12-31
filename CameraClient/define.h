/*****************************************************************************
ģ����  : define
�ļ���  : define.h 
����ļ���
ʵ�ֹ��ܣ�������
����    �����ǿ�
�汾    ��IPC-V7.0-R1.0 Copyright (C) 2009 - 2012 All Rights Reserved
-----------------------------------------------------------------------------
�޸ļ�¼:
��  ��        �汾        �޸���        �߶���    �޸�����
20151111      v1.0        ���ǿ�                  ����
******************************************************************************/
#pragma once

#include "kdvtype.h"
#include "ivhs_struct.h"
#include <map>
#include <list>


//===================�豸PID===========================
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
#define  WEBSERVERPORT					80							// httpЭ��˿� 80
#define	 DEFTITLE						TEXT("������")
#define  DECODERMAX						260							// ���������������
#define  DEFLANG						TEXT("zh")					// Ĭ������

#define	 MINIINFO						TEXT("600,400")				// ������С���
#define  DLGMAX							32				// ����������� 
#define  SCREENNUM						1							// ��������
#define  DEFLAYOUT						"vh_equ_1"					// Ĭ�ϲ��ַ���,1����
#define  FRAMEBORDER					4							// ������ǿ��


//====== Generic VER notification codes =================================
//
#define PRODUCT_VER						"1.1.1"//Version

//====== Generic layout notification codes =================================
//
#define PLAN_DEF						TEXT("def")			// Ĭ��Ԥ��
#define LAYOUT_GP_1						TEXT("gp_v1h1")		// �ȱ�1����
#define LAYOUT_GP_4						TEXT("gp_v2h2")		// �ȱ�4����
#define LAYOUT_GP_9						TEXT("gp_v3h3")		// �ȱ�9����
#define LAYOUT_GP_16					TEXT("gp_v4h4")		// �ȱ�16����

//====== Generic WM_NOTIFY notification codes =================================
//
#define NMC_FIRST                       (WM_USER+10000)		// generic to all controls
#define NMC_CHANGELANG                  (NMC_FIRST+1)		// �����л�			
#define NMC_CHANGEUI                    (NMC_FIRST+2)		// Ƥ���л�
#define NMC_DESTROY                     (NMC_FIRST+3)		// ���ٴ���
#define NMC_PLAYCTRLOVER                (NMC_FIRST+4)		// �����������
#define NMC_RELAYOUT                    (NMC_FIRST+5)		// ���²���
#define NMC_VIEWLOADPLAN                (NMC_FIRST+6)		// ����Ԥ������
#define NMC_ALARM                       (NMC_FIRST+7)		// �澯����
#define NMC_PLAY_CHANGE                 (NMC_FIRST+8)		// ���Ÿı�
#define NMC_ERROR						(NMC_FIRST+9)		// generic to all controls

//----------
#define NMC_DATE_ERROR					(NMC_FIRST+10)		//����ʱ�����

#define WM_ADDLISTITEM					(NMC_FIRST+11)      //��ʾ������Ϣ

#define WM_CHECKPOSITION				(NMC_FIRST+12)		//���Ԥ��λ


// UI�ȳ���������
#define NMC_CFG_FIRST                   (NMC_FIRST+1000)			//
#define NMC_CFG_SETIP					(NMC_CFG_FIRST+1)			// ����ָ������IP
#define NMC_CFG_SETSVRIP				(NMC_CFG_FIRST+2)			// ���÷�������IP
#define NMC_CFG_SETIPCIP				(NMC_CFG_FIRST+3)			// ����IPCIP
#define NMC_CFG_SETSVRADNIPCIP		    (NMC_CFG_FIRST+4)			// �����豸���в�������ҳ���IP

// �¼���
//
#define NMC_EVT_FIRST                   (NMC_CFG_FIRST+1000)		// 
#define NMC_EVT_LOGIN					(NMC_EVT_FIRST+1)			// ����
#define NMC_EVT_LOGOUT					(NMC_EVT_FIRST+2)			// �ǳ�
#define NMC_EVT_STARTVIDEO				(NMC_EVT_FIRST+3)			// ��ʼ����
#define NMC_EVT_STOPVIDEO				(NMC_EVT_FIRST+4)			// ֹͣ����
#define NMC_EVT_VIDEOSTATEREFRESH		(NMC_EVT_FIRST+5)			// ��������ˢ��
#define NMC_EVT_ADD						(NMC_EVT_FIRST+11)			// �羯������Ϣ����
#define NMC_EVT_DEL						(NMC_EVT_FIRST+12)			// �羯������Ϣɾ��
#define NMC_EVT_DELALL					(NMC_EVT_FIRST+13)			// �羯������Ϣɾ��
#define NMC_EVT_SEL						(NMC_EVT_FIRST+14)			// �羯������Ϣѡ��
#define NMC_EVT_DLG_CLOSE				(NMC_EVT_FIRST+15)			// ���ڹرգ���������
#define NMC_EVT_LISTCTRL_CHANGE			(NMC_EVT_FIRST+16)			// dlglistCtrl �������ݷ����ı䣻
#define NMC_EVT_BUILDLOGICLANE			(NMC_EVT_FIRST+17)			// �����߼������ɹ�
#define NMC_EVT_VIDEO_PIC				(NMC_EVT_FIRST+18)			// ��Ƶ��ˢ��
#define NMC_EVT_VIDEO_PIC_DISCONNECT    (NMC_EVT_FIRST+19)          // ��Ƶ������
#define NMC_EVT_FLOWDETECTION           (NMC_EVT_FIRST+20)          // ��������Ϣ
#define NMC_EVT_SYSTEM_UPGRADE			(NMC_EVT_FIRST+21)			// ipcϵͳ����
#define NMC_EVT_STAT_FLOWDETECTION      (NMC_EVT_FIRST+22)          // ͳ�Ƴ�������Ϣ
#define NMC_EVT_IO_SIGNAL               (NMC_EVT_FIRST+23)          // IO�ź���Ϣ
#define NMC_EVT_IO_SIGNAL_DISCONNECT    (NMC_EVT_FIRST+24)          // IO�źŶ�����Ϣ
#define NMC_EVT_HARDWARE_TEST           (NMC_EVT_FIRST+25)          // Ӳ��������Ϣ
#define NMC_EVT_SAVE_PARAM              (NMC_EVT_FIRST+26)          // �������
#define NMC_EVT_EXT_TYPE				(NMC_EVT_FIRST+27)          // led�����


// ����������ȡ��
//
#define NMC_GET_FIRST                   (NMC_EVT_FIRST+1000)		// 
#define NMC_GET_OSDINFO					(NMC_GET_FIRST+1)			// ��ȡOSD��Ϣ
#define NMC_GET_PICINFO					(NMC_GET_FIRST+2)			// ��ȡpic��Ϣ
#define NMC_GET_OSDFONTINFO				(NMC_GET_FIRST+3)			// ��ȡOSD������Ϣ
#define NMC_GET_UICAP					(NMC_GET_FIRST+4)			// ��ȡUI������

// ����������ȡ��������
//
#define NMC_GET_SVR_FIRST               (NMC_GET_FIRST+1000)		// 
#define NMC_GET_SVR_DEVLIST				(NMC_GET_SVR_FIRST+1)		// ��ȡ�豸�б���Ϣ
#define NMC_GET_SVR_DEVINFO				(NMC_GET_SVR_FIRST+2)		// ��ȡ�豸��Ϣ
#define NMC_GET_SVR_ASSDEVINFO			(NMC_GET_SVR_FIRST+3)		// ��ȡ�����豸��Ϣ
#define NMC_GET_SVR_PLATEINFO      		(NMC_GET_SVR_FIRST+4)		// ��ȡplate��Ϣ
#define NMC_GET_SVR_RECINFO				(NMC_GET_SVR_FIRST+5)		// ��ȡrec��Ϣ
#define NMC_GET_SVR_STATISTICSINFO		(NMC_GET_SVR_FIRST+6)		// ��ȡͳ����Ϣ

// ����������ȡIPC��
//
#define NMC_GET_IPC_FIRST               (NMC_GET_SVR_FIRST+1000)	
#define NMC_GET_IPC_DEVINFO				(NMC_GET_IPC_FIRST+1)		// ��ȡ�豸��Ϣ
#define NMC_GET_IPC_SCENCEINFO			(NMC_GET_IPC_FIRST+2)		// ��ȡ������Ϣ
#define NMC_GET_IPC_VIDEOLOOPINFO		(NMC_GET_IPC_FIRST+3)		// ��ȡ�������Ϣ
#define NMC_GET_IPC_LOGICLANEINFO		(NMC_GET_IPC_FIRST+4)		// ��ȡ�߼�������Ϣ
#define NMC_GET_IPC_RECOGAREA			(NMC_GET_IPC_FIRST+5)		// ��ȡʶ��������Ϣ
#define NMC_GET_IPC_IMAGEQUALITY		(NMC_GET_IPC_FIRST+6)		// ��ȡͼ��������Ϣ
#define NMC_GET_IPC_IDENTPARAM			(NMC_GET_IPC_FIRST+7)		// ��ȡʶ�������Ϣ
#define NMC_GET_IPC_POWERSYN			(NMC_GET_IPC_FIRST+8)		// ��ȡ��Դͬ����Ϣ
#define NMC_GET_IPC_REDLIGHT			(NMC_GET_IPC_FIRST+9)		// ��ȡ��Ʋ�����Ϣ
#define NMC_GET_IPC_VELOMETER			(NMC_GET_IPC_FIRST+10)		// ��ȡ��Ƶ������Ϣ
#define NMC_GET_IPC_REDARLOOP			(NMC_GET_IPC_FIRST+11)		// ��ȡ�״�������Ϣ
#define NMC_GET_IPC_COILLOOP			(NMC_GET_IPC_FIRST+12)		// ��ȡ��Ȧ�������Ϣ
#define NMC_GET_IPC_REDSIGLOOP			(NMC_GET_IPC_FIRST+13)		// �����źż������Ϣ
#define NMC_GET_IPC_REDENHLOOP			(NMC_GET_IPC_FIRST+14)		// ������ǿ�������Ϣ
#define NMC_GET_IPC_IOLOOP				(NMC_GET_IPC_FIRST+15)		// ��io�������Ϣ
#define NMC_GET_IPC_DRAW_CHANGE			(NMC_GET_IPC_FIRST+16)		// ���Ʊ仯֪ͨ��Ϣ
#define NMC_SET_IPC_CREATE_LOOP			(NMC_GET_IPC_FIRST+17)		// ���ɼ������Ϣ
#define NMC_GET_IPC_SERIALCFG           (NMC_GET_IPC_FIRST+18)      // ��ȡ��������
#define NMC_GET_IPC_DEBUG_PARAM			(NMC_GET_IPC_FIRST+19)		// ��ȡ������Ϣ
#define NMC_GET_IPC_ITS_VERSION	        (NMC_GET_IPC_FIRST+20)		// ��ȡ����汾��Ϣ
#define NMC_IPC_SHOW_ORIGINAL_PIC       (NMC_GET_IPC_FIRST+21)		// ԭͼ��ʾ
#define NMC_GET_IPC_CONFIG              (NMC_GET_IPC_FIRST+22)      // ��ȡ�����ļ�;
#define NMC_GET_DEV_INFO                (NMC_GET_IPC_FIRST+23)      // ��������豸;
#define NMC_GET_DEV_SEARCH              (NMC_GET_IPC_FIRST+24)      // �����豸;
#define NMC_GET_IPC_PARKSCENCE          (NMC_GET_IPC_FIRST+25)      // Υͣ����;
#define NMC_IPC_LOGOUT                  (NMC_GET_IPC_FIRST+26)      // �˳�����;
#define NMC_IPC_LIGHT_BRIGHTNESS        (NMC_GET_IPC_FIRST+27)      // ����������;
#define NMC_QUERY_SD_RECORD             (NMC_GET_IPC_FIRST+28)      // ��ѯ������¼
#define NMC_GET_SD_RECORD				(NMC_GET_IPC_FIRST+29)		// ��ȡ������¼
#define NMC_GET_IPC_SCENEPOLL			(NMC_GET_IPC_FIRST+29)		// ��ȡ������Ѳ����
#define NMC_GET_IPC_FLOWDETECTION		(NMC_GET_IPC_FIRST+30)		// ��ȡ����������
#define NMC_GET_IPC_ILLEGALDICT 		(NMC_GET_IPC_FIRST+31)		// ��ȡΥ���ֵ����
#define NMC_GET_IPC_MERGE_PIC           (NMC_GET_IPC_FIRST+32)		// ��ȡ�ϳ�ͼƬ����
#define NMC_IPC_SERIAL_TEST        	    (NMC_GET_IPC_FIRST+33)		// ���ڲ��Խ��
#define NMC_GET_IPC_SIGNAL_PROTOCOL     (NMC_GET_IPC_FIRST+34)		// ��ȡ�ź�Э������
#define NMC_GET_IPC_SCENCE_ID           (NMC_GET_IPC_FIRST+35)		// ��ȡ��ǰ����ID
#define NMC_GET_IPC_INFO_SYNC           (NMC_GET_IPC_FIRST+36)		// ��ȡ��Ϣͬ������
#define NMC_GET_IPC_PARKPARAM           (NMC_GET_IPC_FIRST+37)      // Υͣ����;



#define NMC_LAST						(NMC_FIRST+20000)	// generic to all controls

#define WM_FIRST						(NMC_LAST+1)		// send/post
#define WM_PREEVT						(WM_FIRST+2)		// �¼�����
#define WM_UPDATEPARAM					(WM_FIRST+3)		// ��������

//====== Generic WebEvent notification codes =================================

#define EVS_VIEW_TOOL					TEXT("viewtool")// ���Ź��ߴ���
#define EVS_DEVTREE						TEXT("devtree")	// �����豸������
#define EVS_VIEW_PLAYLAYOUT				TEXT("viewplaylayout")	// �����豸������
#define EVS_VIEW_PLAY					TEXT("viewplay")	// ���Ŵ���
#define EVS_VIEW_TOPTOOLBAR				TEXT("viewplaytoptoolbar")	// ���Ź�����
#define EVS_VIEW_BOTTOMTOOLBAR			TEXT("viewplaybottomtoolbar")	// ���Ź�����
#define EVS_VIEW_ALARM					TEXT("viewalarm")	// ����澯����
#define EVS_VIEW_ALARM_SHOW				TEXT("viewalarmshow")	// ����澯��ʾ
#define EVS_VIEW_PLAYINFO				TEXT("viewplayinfo")	// �����Ϣ
#define EVS_SPEED						TEXT("speed")		// ����澯����
#define EVS_ALARM						TEXT("alarm")	// �澯
#define EVS_STATISTICS					TEXT("statistics")	// ͳ��
#define EVS_CONFIG						TEXT("config")	// ����
#define EVS_TITLE						TEXT("title")	// ���ⴰ��
#define EVS_GETTITLE					TEXT("gettitle")// ��ȡ����ͷ
#define EVS_GETMINIINFO					TEXT("getminiinfo")// ��ȡ��С�����Ϣ
#define EVS_BASECONFIG					TEXT("baseconfig") //��������ҳ��
#define EVS_VIEWSRC						TEXT("viewsrc")   //��ƵԴ����
#define EVS_ALGCONFIG					TEXT("algconfig")  //�̻���
#define EVS_DLG_VIEW_LAYOUT				TEXT("view")	// Ԥ������

// ����Ϊ���ڵ�

// ��������
// ����������
#define EVS_CFG_OSD					               TEXT("its_cfg_osd")	           // OSD
#define EVS_CFG_PICMERGE			               TEXT("its_cfg_picmerge")  // ͼƬ�ϳ�����
#define EVS_DLG_ITS_SYSCFG_PLATE        TEXT("its_cfg_sysplate")     // ƽ̨����

// ��������������
#define EVS_DLG_BASE				TEXT("base")			// ��������
#define EVS_DLG_MAIN				TEXT("main")			// ������
#define EVS_DLG_LOGIN				TEXT("login")			// ��¼����
#define EVS_DLG_TST					TEXT("dlg_tst")			// �������Դ���
#define EVS_DLG_PICSHOW				TEXT("dlg_pichow")		// ͼƬ��ʾ
#define EVS_DLG_EVTLST				TEXT("dlg_evtlst")		// �����¼���ʾ����
#define EVS_DLG_ITS_CFG_IPCMGR		TEXT("its_cfg_dev")		// �����豸����
#define EVS_DLG_ITS_IPCCFG_DEVINFO	TEXT("its_cfg_normal")	// ��ͨ��Ϣ
#define EVS_DLG_ITS_IPCCFG_ASSDEV	TEXT("its_cfg_assdev")	// �������
#define EVS_DLG_ITS_CFG_SYSMGR		TEXT("its_cfg_sys")		// ϵͳ���ý���
#define EVS_DLG_ITS_SYSCFG_INFO		TEXT("its_cfg_sysinfo")	// ϵͳ��Ϣ
#define EVS_DLG_ITS_SYSCFG_REC		TEXT("its_cfg_sysrec")	// ¼������
#define EVS_DLG_ITS_STATISTICS		TEXT("its_statistics")	// ͳ��
#define EVS_DLG_SERVER_IPCMGR       TEXT("server_ipcmgr")	// �豸�������

// IPC ������
#define EVS_DLG_IPC_MGR				TEXT("ipc_mgr")			// ipcͼƬ������
#define EVS_DLG_IPC_VIEW			TEXT("ipc_picview")		// ����Ԥ�����¼���ʾ����
#define EVS_DLG_IPC_PICINFO			TEXT("ipc_picinfo")		// ipcͼƬ��Ϣ
#define EVS_DLG_IPC_PICVIEW_TOOL	TEXT("ipc_picview_tool")// ipcͼƬ���������
#define EVS_DLG_IPC_SCENE			TEXT("ipc_scene")		// ipc��������
#define EVS_DLG_IPC_DETECTION		TEXT("ipc_detection")	// ipc�������
#define EVS_DLG_IPC_LOGICLANE		TEXT("ipc_logiclane")	// ipc�߼�����
#define EVS_DLG_IPC_IDENTPARAM		TEXT("ipc_identparam")	// ipcʶ�����
#define EVS_DLG_IPC_VELOMETER 		TEXT("ipc_velometer")	// ipc��Ƶ����
#define EVS_DLG_IPC_IMAGEQUALITY 	TEXT("ipc_imagequality")// ipcͼƬ����
#define EVS_DLG_IPC_REDLIGHT 		TEXT("ipc_redlight")	// ipc��Ʋ���
#define EVS_DLG_IPC_POWERSYN 		TEXT("ipc_powersyn")	// ipc��Դͬ�� 
#define EVS_DLG_IPC_LISTCTRL 		TEXT("ipc_listctrl")	// ipcList �ؼ� 
#define EVS_DLG_IPC_SERIAL 		    TEXT("ipc_serial")		// ipc�������� 
#define EVS_DLG_IPC_LOCALCFG 		TEXT("ipc_localcfg")	// ipc�������� 
#define EVS_DLG_IPC_SYSMAINTENANCE  TEXT("ipc_sysmaintenance") //ipcϵͳά������
#define EVS_DLG_IPC_PARKSCENE       TEXT("ipc_parkscene")	//ipcΥͣ��������
#define EVS_DLG_IPC_VIDEO           TEXT("ipc_video")		//ipcͼƬ���������Ƶ���Ŵ���
#define EVS_DLG_IPC_SDPIC           TEXT("ipc_sdpic")       //ipcץ�Ĺ�����(sd���洢ͼƬ��ѯ)
#define EVS_DLG_IPC_BROWSE          TEXT("ipc_browse")      //ipc��Ƶ���
#define EVS_DLG_IPC_LIGHT           TEXT("ipc_light")       //ipc�ƹ���������
#define EVS_DLG_IPC_DEVINFO 		TEXT("ipc_devinfo")     // ipc�豸��Ϣ
#define EVS_DLG_IPC_SCENEPOLL 		TEXT("ipc_scenepoll")   // ipc������Ѳ
#define EVS_DLG_IPC_FLOWDETECTION	TEXT("ipc_flowdetection")// ipc�������
#define EVS_DLG_IPC_ILLEGALDICT 	TEXT("ipc_illegaldict") // ipcΥ���ֵ�
#define EVS_DLG_IPC_MANUALCAP 	TEXT("ipc_manualcapture") // ipcΥ��ץ��
#define EVS_DLG_IPC_OTHER 	TEXT("ipc_other") // ipc��������
#define EVS_DLG_IPC_HARDWARE 	TEXT("ipc_hardware") // ipcӲ������
#define EVS_DLG_IPC_SIGNAL_PROTOCOL TEXT("ipc_signal_protocol")		// ipc�ź�Э������
#define EVS_DLG_IPC_INFO_SYNC TEXT("ipc_info_sync")		// ipc��Ϣͬ������ 


//----------------------
#define EVS_DLG_IPC_EPTZ			TEXT("ipc_eptz")		//ipcEPTZ����
#define EVS_DLG_IPC_EPTZCTRL		TEXT("ipc_eptzctrl")    //ipcEPTZ����
 

// �¼�����
#define EVS_EVT_SET_UITYPE			TEXT("evt_uitype")		// ���ô�������
#define EVS_EVT_IS_LOGIN			TEXT("evt_islogin")		// �Ƿ��¼
#define EVS_EVT_ADDLINK				TEXT("evt_addlink")		// ��������
#define EVS_EVT_DELLINK				TEXT("evt_dellink")		// ɾ������
#define EVS_EVT_REGISTER			TEXT("evt_register")	// ע��
#define EVS_EVT_STARTRCV			TEXT("evt_startrecv")	// ��ʼ����
#define EVS_EVT_STOPRCV				TEXT("evt_startrecv")	// ֹͣ����
#define EVS_EVT_ADDDEVICE			TEXT("evt_add_device")  // ����豸
#define EVS_EVT_DELDVICE			TEXT("evt_del_device")  // ɾ���豸
#define EVS_EVT_LOGIN				TEXT("evt_login")		// ��¼: IP, PORT ,�û����� ���룻
#define EVS_EVT_SETIP				TEXT("evt_set_ip")		// ����ָ������IP
#define EVS_EVT_SET_IP_SVR			TEXT("evt_set_ip_svr")	// ϵͳ�������ã����ö�Ӧ��nvr
#define EVS_EVT_SETIPCIP			TEXT("evt_set_ip_ipc")	// ����ipc ip
#define EVS_EVT_SET_IP_SVRANDIPC	TEXT("evt_set_ip_svrandipc") // ��������,ָ������
#define EVS_EVT_IS_EXIT				TEXT("evt_isexit")		// �Ƿ��˳�

// Ӧ��
#define ACK_ERR						TEXT("err")				// 
#define ACK_OK						TEXT("ok")				// 


//====== Generic normal notification codes =================================

#define USERMAXLEN					32				// �û�������󳤶�
#define PASSWORDMAXLEN				32				// �������󳤶�
#define MAXPICBUF					6144000			// ���ͼƬ��������С
#define MAX_PATH_LEN                256             // ���·������

//====== Generic color notification codes =================================

#define COLORSEL					    0xFFFF0000		// ѡ�еĶ�����ɫ
#define COLOROVER					0xFFFF0000		// ѡ�еĶ�����ɫ
#define COLORLINENOR				0x0003a7fd		// ��ͨ������ɫ
#define COLORLINEDRAWING		0xFFFFFF00		// ���ڻ���������ɫ
#define COLOR_YELLOW             0xFFFFFF00
#define COLOR_WHITE                0xFFFFFFFF
#define COLOR_BLUE                  0xFF0000FF
#define COLOR_PURPLE              0xFF9400D3
#define COLOR_RED                    0xFFFF0000
#define COLOR_GREEN               0xFF00FF00
#define COLOR_BLACK               0xFF000000

//====== Generic size boder =================================

#define SIZBOX_BORDER				6				// �����϶��߿򳤶�
#define POINT_R						3				// ������Ƶİ뾶



//====== Generic enum notification codes =================================
enum EMDATATYPE
{
	EMDATATYPE_NVR = 0,
	EMDATATYPE_IPC,
	EMDATATYPE_SERVER,
	EMDATATYPE_COUNT
};

// ������������
enum EFONTTYPE
{
	FONT_UI_SML = 0,
	FONT_UI_MID,
	FONT_UI_BIG,
	FONT_COUNT
};

// ������������
enum EUICONFIG
{
	CFG_UI_CLR = 0,
	CFG_UI_CLR_TXT,				// �ı���ɫ
	CFG_UI_CLR_TXT_SEL,			// ѡ���ı���ɫ
	CFG_UI_CLR_DLGBK,			// ��ͨ���ڵ���ɫ
	CFG_UI_CLR_LAYOUT_BK,		// ���ֿ�ı���ɫ
	CFG_UI_CLR_LAYOUT_BORDER,	// ���ֿ�߿���ɫ
	CFG_UI_CLR_LAYOUT_SEL,		// ѡ��
	CFG_UI_CLR_LAYOUT_FLICKER,	// ��˸
	CFG_UI_COUNT
};

// ��Ƶ�������
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

// ��������Э����������
enum ESTREAMTRANSPORTTYPE
{
	E_STT_NULL = 0,
	E_STT_RTSP,
	E_STT_RTP,
	E_STT_COUNT
};

//====== Generic typedef notification codes =================================

typedef std::map<std::wstring , std::wstring> TMAPWS;
//typedef std::map<std::wstring , Image*> TMAPIMAGE;  ��ע�ͣ���δ�õ�
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
// ���Ų���
typedef struct tagPlayInfo
{
	void*	pWnd;					// ���ھ��
	int		nScreenIndex;			// ��Ļ����
	int		nLayoutIndex;			// ����λ������
	u8		byState1;				// EMPLAYFLOW: 0.���У� 1.���������3.����У� 3.ֹͣ���
	u8		byState2;				// 1���澯��2������
	u32		dwPuIP;					// ǰ��IP
	u16		wPuPort;				// ǰ��ͨ�Ŷ˿�
	u8		byTransportType;		// Э������ �� ESTREAMTRANSPORTTYPE
	s8		szURL[129];				// rtsp����URL	
	u32		dwPlayID;				// ���ž��,ǰ�˻ش��ģ����ڿ��Ʋ���
	void*   pDecMedia;				// ������ָ��

	//		
	u32		dwLocIP;				// ����IP
	bool	abyStreamType[E_VPT_COUNT];	// ��������, true��ʾ����
	u16		awRtpPort[E_VPT_COUNT];	// ��������rtp�˿�
	u16		awRtcpPort[E_VPT_COUNT];	// ��������rtcp�˿�

	u32		dwReplayNum;			// �ظ����󲥷ŵĴ���

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
		byState1	= 0;			// EMPLAYFLOW: 0.���У� 1.���������3.����У� 3.ֹͣ���
		byState2	= 0;			// 1���澯��2������
		dwPuIP		= 0;			// ǰ��IP
		wPuPort		= 0;			// ǰ��ͨ�Ŷ˿�
		byTransportType	= 0;		// Э������ �� ESTREAMTRANSPORTTYPE
		strcpy(szURL, "");			// rtsp����URL	
		dwPlayID	= 0;			// ���ž��,ǰ�˻ش��ģ����ڿ��Ʋ���
		pDecMedia	= 0;			// ������ָ��	
		dwLocIP		= 0;			// ����IP
		dwReplayNum	= 0;
		for (int i =0; i< E_VPT_COUNT; i++)
		{
			abyStreamType[i]= 0;	// ��������, true��ʾ����
			awRtpPort[i]	= 0;	// ��������rtp�˿�
			awRtcpPort[i]	= 0;	// ��������rtcp�˿�
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


// ������Ϣ���������ļ��л�ȡ
typedef struct tagLayoutInfo
{
	int		nLayoutNum;			// ��������
	int		xOrgin,yOrgin;		// ��ʼλ��
	int		nWidth, nHeight;	// ���򻮷�
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

// Ԥ����Ϣ
typedef struct tagPlanInfo
{
	wchar_t		wsLayoutType[33];		// ���ַ�������
	u16			wFullScreen;			// ȫ������������0����ȫ��
	int			wFullWindow;			// �������ڵ�������0��������
	TPLAYINFO	tPlayInfo[DLGMAX];		// ���ּ�ص���Ϣ

	tagPlanInfo()
	{
		empty();
	}

	void empty()
	{
		memset(this, 0, sizeof(tagPlanInfo));
	}
}TPLANINFO, *PTPLANINFO;

// �豸��Ϣ
typedef struct tagDevInfo
{
	wchar_t		wsName[33];		// ���ַ�������
	u32			dwPuIP;			// ǰ��IP
	u16			wPuPort;		// ǰ��ͨ�Ŷ˿�

	tagDevInfo()
	{
		empty();
	}

	void empty()
	{
		memset(this, 0, sizeof(tagDevInfo));
	}
}TDEVINFO, *PTDEVINFO;

// �澯����
enum EALARMTYPE
{
	E_AT_NULL = 0,
	E_AT_HYJC,			// ������
	E_AT_MOREPEOPLE,	// ��������
	E_AT_COUNT
};

// �澯��Ϣ
typedef struct tagAlarmInfo_Its
{
	u32			dwType;					// �澯���ͣ�EALARMTYPE
	u32			dwIP;					// �澯IP
	SYSTEMTIME	tDateTime;				// �澯ʱ��
	wchar_t		szAddr[33];				// �ص�
	wchar_t		szPicAddr[65];			// ͼƬ��ַ
	wchar_t		szRecAddr[65];			// ¼���ַ
	SYSTEMTIME	tRecDateTimeStart;		// ¼��ʼʱ��
	SYSTEMTIME	tRecDateTimeEnd;		// ¼�����ʱ��

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

#define  MAXNAMELEN	65			    // ���Ƶ���󳤶�
#define  MAXPATHLEN	129				// �洢·���ַ�������
// ���Ӿ��������Ϣ
typedef struct tagEvtVehicleInfo
{
	u32			dwId;
	u32         dwIP;
	KOSA_HANDLE	dwHandle;
	TVechileData tVechileData;
	wchar_t		szDir[MAXPATHLEN];	// �洢·��
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

// ����ͼƬ�洢�ռ�
struct TImageBuf
{
	u32		dwID;		// dwID == 0��ʾ����
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

// �û�����
enum EUSERTYPE
{
	E_AT_NOR = 0

};
// �û���Ϣ
typedef struct tagUserInfo_Its
{
	u32			dwType;
	wchar_t		szName[33];		// ����
	wchar_t		szPassword[33];	// �û���

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

// ����������Ϣ
typedef struct tagLocalCfgInfo
{
	wchar_t		picturepath[MAX_PATH_LEN];			// ����ͼƬ�洢·��
	wchar_t		videopath[MAX_PATH_LEN];	        // ������Ƶ�洢·��
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


//ͼƬ�ϳ�
typedef struct tagPhotoMergeInfo
{
	//ͼƬ�ϳɲ���

	//�ϳ�ͼƬOSD����
}TPhotoMergeInfo, *PTPhotoMergeInfo;

// ��������
enum EMDRAWMODE
{
	EMDRAWMODE_NONE  = 0,
	// ֱ��
	EMDRAWMODE_LINE,
	// ����
	EMDRAWMODE_RECT,
	// �����
	EMDRAWMODE_RGN,
	// �ı���
	EMDRAWMODE_RGN_4,
	// ����
	EMDRAWMODE_TRAPEZIUM,
	EMDRAWMODE_COUNT
};
#define  MAXDRAWPOINTS 8   // ������8����
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

// ��Ƶ�����Ϣ
enum EMVIDEOSTATE
{
	EMVIDEOSTATE_STOP = 1,		// ֹͣ
	EMVIDEOSTATE_START,			// ��ʼ
	EMVIDEOSTATE_RELEASE,		// �ͷ�
};
typedef struct tagVideoInfo
{
	bool	bSet;			// �Ƿ�����
	bool	bReqing;		// �Ƿ�������
	u32		dwReqResult;	// ������, 0, �ޣ� 1������ʧ��,����������
	u32		dwReqTime;		// �����ʱ��
	u8		byFlow;			// EMVIDEOSTATE
	u8		byNextFlow;    // �ش���ȶ�һ����������һ������
	u8		byVideoType;	// 
	u32		dwContext;		// ������,�ݲ�ʹ�ã����ص�ģʽʹ��
	u32		dwErr;			// 1:����δ����
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

// �ָ�ϵͳĬ���ֽڶ��뷽ʽ
#pragma pack(pop)

