// portmanager.cpp: implementation of the CMgrPort class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "mgr_port.h"
#include "winsock2.h"
#include <QDebug>
const TCHAR* szPortKeyWord2 = L"kedacom_portmanager";			// NVR		
const TCHAR* szPortKeyWord3 = L"Global\\PortDistribution";		// CU
const TCHAR* szPortKeyWord = L"its_portmanager";				// 智能交通
const int g_wStartPort = 30000;										// 分配端口的起始位置


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMgrPort::CMgrPort()
{
	memset(m_abySharedPortState, 0, sizeof(m_abySharedPortState));
	memset(m_hSharedFile, 0, sizeof(m_hSharedFile));
	m_wFirstPort = g_wStartPort;
	m_wXmlpPort = 0;

	Init();
}

CMgrPort::~CMgrPort()
{
	Release();
}

void CMgrPort::Init()
{
	AllocSharePort(m_wFirstPort, g_wStartPort, 200);

	m_wXmlpPort = AllocPort(1);
}
void CMgrPort::Release()
{
	FreePort(m_wFirstPort, 200);
	FreeSharePort(m_wFirstPort, 200);
}
//
u16  CMgrPort::AllocPort(u16 wPortNumber, int nOffset)
{
	int nStart = m_wFirstPort + nOffset;
	for (int i = nStart; i < nStart + 200; i++)
	{
		int j = 0;
		for (j = i; j < nStart + wPortNumber; j++)
			if (m_abySharedPortState[i] != 0)
			{
				break;;
			}

		if (j == nStart + wPortNumber)
		{
			for (j = i; j < nStart + wPortNumber; j++)
				m_abySharedPortState[i] = 1;
			return i;
		}
	}

	return 0;
}
void CMgrPort::FreePort(u16 wStartPort, u16 wPortNumber)
{
	for (int i = wStartPort; i < m_wFirstPort + wPortNumber; i++)
	{
		m_abySharedPortState[i] = 0;
	}
}

BOOL32 GetLocalIP(DWORD &dwLocalIP)
{
	char szHostName[MAX_PATH] = { 0 };
	DWORD dwDestIP = 0;

	gethostname(szHostName, MAX_PATH);
	hostent* tHost = gethostbyname(szHostName);

	if (tHost != NULL)
	{
		if (dwDestIP == 0)
		{
			if (tHost->h_addr_list[0] != NULL)
			{
				memcpy(&dwLocalIP, tHost->h_addr_list[0], tHost->h_length);
				dwLocalIP = ntohl(dwLocalIP);
				// 				TraceInfo("本机IP地址:%d:%d:%d:%d\n", dwLocalIP>>24, (dwLocalIP<<8)>>24, 
				// 					(dwLocalIP<<16)>>24, (dwLocalIP<<24)>>24 );
				return TRUE;
			}
			else
			{
				dwLocalIP = 0;
				//TraceInfo("dwLocalIP= %d\n", dwLocalIP);
				return FALSE;
			}
		}
		else
		{
			// 寻找与dwDestIP同网段的IP
			BOOL32 bFind = FALSE;
			int nPos = 31;
			DWORD dwIP = 0;
			for (int nAdapter = 0; tHost->h_addr_list[nAdapter]; nAdapter++)
			{
				memcpy(&dwIP, tHost->h_addr_list[nAdapter], tHost->h_length);
				dwIP = ntohl(dwIP);
				// 找与目的IP匹配位数最多的本机IP
				if (dwIP >> 24 == dwDestIP >> 24)
				{
					bFind = true;

					int nIndex = 24;
					int nTemp1 = 0, nTemp2 = 0;
					do
					{
						nIndex--;
						nTemp1 = ((1 << nIndex) & dwIP);
						nTemp2 = ((1 << nIndex) & dwDestIP);
					} while (nTemp1 == nTemp2);

					if (nIndex < nPos)
					{
						dwLocalIP = dwIP;
						nPos = nIndex;
					}
				}
			}

			// 如果没有找到同网段的IP，使用第一个IP
			if (bFind == FALSE)
			{
				memcpy(&dwLocalIP, tHost->h_addr_list[0], tHost->h_length);
				dwLocalIP = ntohl(dwLocalIP);
			}
			// 打印IP地址信息
			// 			TraceDebug("设备IP地址:%d:%d:%d:%d\n", dwDestIP>>24, (dwDestIP<<8)>>24, 
			// 				(dwDestIP<<16)>>24, (dwDestIP<<24)>>24 );
			// 			TraceDebug("本机IP地址:%d:%d:%d:%d\n", dwLocalIP>>24, (dwLocalIP<<8)>>24, 
			// 				(dwLocalIP<<16)>>24, (dwLocalIP<<24)>>24 );
		}
	}
	else
	{
		dwLocalIP = 0;
		qCritical()<< "dwLocalIP= %d\n", dwLocalIP;
		return FALSE;
	}

	qCritical() << "dwLocalIP= %d\n", dwLocalIP;

	return TRUE;
}

BOOL CMgrPort::CheckPortIsUsed(u16 wLocalPort)
{
	// 检测端口
	SOCKET tsocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (INVALID_SOCKET == tsocket)
	{
		qCritical() << ("创建SOCKET失败，无法检测端口%d\n", wLocalPort);
		return TRUE;
	}

	sockaddr_in tSockAddr;
	tSockAddr.sin_family = AF_INET;
	tSockAddr.sin_port = htons(wLocalPort);
	// 获取本地IP
	u32 dwLocalIP = 0;
	if (!GetLocalIP(dwLocalIP))
	{
		qCritical() << (("[CKDMPUSearch] Initialize: 获取本机IP失败"));
		tSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	else
	{
		tSockAddr.sin_addr.s_addr = htonl(dwLocalIP);
	}

	int nRes = bind(tsocket, (struct sockaddr *)&tSockAddr, sizeof(tSockAddr));
	if (nRes < 0)
	{
		int nErrorCode = WSAGetLastError();
		qCritical() << ("UDP端口%d被占用，socket错误号：%d\n", wLocalPort, nErrorCode);
		closesocket(tsocket);
		return TRUE;
	}

	closesocket(tsocket);

	return FALSE;
}


/*=============================================================================
函 数 名： GetIdlePort
功    能： 搜索连续的空闲端口，并立即占用
算法实现： 1、在共享端口内查找；2、检查端口是否已被占用
全局变量：
参    数： WORD& wFirstIdlePort - 返回的空闲端口起始号
WORD wStartPort - 指定的开始端口号，例如NCUI就有整个设置
WORD wPortNumber - 需要连续多个端口，例如NCUI是74*4，而IPCCTRL是8
返 回 值： BOOL
=============================================================================*/
BOOL CMgrPort::AllocSharePort(WORD& wFirstIdlePort, WORD wStartPort, WORD wPortNumber)
{
	BYTE m_abySharedPort[MAXWORD];		 // 共享内存端口,1:表示该端口已被其他NC占用,0:空闲
	BYTE m_abySharedPort2[MAXWORD];		// nvr
	BYTE m_abySharedPort3[MAXWORD];		// 平台的
	memset(m_abySharedPort, 0, sizeof(m_abySharedPort2));
	memset(m_abySharedPort2, 0, sizeof(m_abySharedPort2));
	memset(m_abySharedPort3, 0, sizeof(m_abySharedPort3));

	// 读取nvr共享内存
	HANDLE hReceiveMap = OpenFileMapping(FILE_MAP_READ, FALSE, szPortKeyWord2);
	if (hReceiveMap != NULL)
	{
		LPBYTE lpData = (LPBYTE)MapViewOfFile(hReceiveMap, FILE_MAP_READ, 0, 0, 0);
		if (lpData != NULL)
		{
			memcpy(m_abySharedPort2, lpData, sizeof(m_abySharedPort2));
			UnmapViewOfFile(lpData);
		}

		CloseHandle(hReceiveMap);
		hReceiveMap = NULL;
	}


	// 读取cu共享内存
	hReceiveMap = OpenFileMapping(FILE_MAP_READ, FALSE, szPortKeyWord3);
	if (hReceiveMap != NULL)
	{
		LPBYTE lpData = (LPBYTE)MapViewOfFile(hReceiveMap, FILE_MAP_READ, 0, 0, 0);
		if (lpData != NULL)
		{
			memcpy(m_abySharedPort3, lpData, sizeof(m_abySharedPort3));
			UnmapViewOfFile(lpData);
		}

		CloseHandle(hReceiveMap);
		hReceiveMap = NULL;
	}

	// 读取its共享内存
	hReceiveMap = OpenFileMapping(FILE_MAP_READ, FALSE, szPortKeyWord);
	if (hReceiveMap != NULL)
	{
		LPBYTE lpData = (LPBYTE)MapViewOfFile(hReceiveMap, FILE_MAP_READ, 0, 0, 0);
		if (lpData != NULL)
		{
			memcpy(m_abySharedPort, lpData, sizeof(m_abySharedPort));
			UnmapViewOfFile(lpData);
		}

		CloseHandle(hReceiveMap);
		hReceiveMap = NULL;
	}

	// 找到空闲端口起始号
	WORD wFirstPort = wStartPort;
	//////////////////////////////////////////////////////////////////////////
	do
	{
		DWORD dwMaxPort = (DWORD)wFirstPort + (DWORD)wPortNumber;
		if (dwMaxPort > MAXWORD)
		{   // 端口号已超出指定的范围
			return FALSE;
		}

		// 在共享内存中查找，判断该端口是否已被其他NC占用
		WORD wPort = wFirstPort;
		for (; wPort < dwMaxPort; wPort++)
		{
			if (m_abySharedPort[wPort - 1] != 0)
			{   // 该端口已被占用-->该端口段弃用
				break;
			}

			if (m_abySharedPort2[wPort - 1] != 0)
			{   // 该端口已被占用-->该端口段弃用
				break;
			}

			if (m_abySharedPort3[wPort - 1] != 0)
			{   // 该端口已被占用-->该端口段弃用
				break;
			}
		}


		if (wPort == dwMaxPort)
		{   // 找到一段未被其他NC占用的端口-->网络方式检查该端口是否空闲
			for (wPort = wFirstPort; wPort < dwMaxPort; wPort++)
			{
				if (CheckPortIsUsed(wPort))
				{   // 端口段中任何一个端口已被占用-->继续查找其他端口段
					break;
				}
			}
			if (wPort == dwMaxPort)
			{   // 端口段中所有端口均空闲-->找到空闲端口段
				break;
			}
		}
		//         wFirstPort += wPortNumber;
		// 为便于计算端口的增加以百的整数倍递增
		wFirstPort += (wPortNumber / 100 + 1) * 100;
	} while (TRUE);


	wFirstIdlePort = wFirstPort;
	for (WORD wPort = wFirstPort; wPort < wFirstPort + wPortNumber; wPort++)
	{
		m_abySharedPort[wPort - 1] = 1;
		m_abySharedPort2[wPort - 1] = 1;
		m_abySharedPort3[wPort - 1] = 1;
	}
	//////////////////////////////////////////////////////////////////////////

	// 写入共享内存
	m_hSharedFile[0] = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE | SEC_COMMIT, 0, MAXWORD, szPortKeyWord);
	if (m_hSharedFile[0] != NULL)
	{
		LPBYTE lpData = (LPBYTE)MapViewOfFile(m_hSharedFile[0], FILE_MAP_WRITE, 0, 0, 0);
		if (lpData != NULL)
		{
			memcpy(lpData, m_abySharedPort, sizeof(m_abySharedPort));
		}
	}

	// nvr
	m_hSharedFile[1] = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE | SEC_COMMIT, 0, MAXWORD, szPortKeyWord2);
	if (m_hSharedFile[1] != NULL)
	{
		LPBYTE lpData = (LPBYTE)MapViewOfFile(m_hSharedFile[1], FILE_MAP_WRITE, 0, 0, 0);
		if (lpData != NULL)
		{
			memcpy(lpData, m_abySharedPort2, sizeof(m_abySharedPort2));
		}
	}
	// cu
	m_hSharedFile[2] = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE | SEC_COMMIT, 0, MAXWORD, szPortKeyWord3);
	if (m_hSharedFile[2] != NULL)
	{
		LPBYTE lpData = (LPBYTE)MapViewOfFile(m_hSharedFile[2], FILE_MAP_WRITE, 0, 0, 0);
		if (lpData != NULL)
		{
			memcpy(lpData, m_abySharedPort3, sizeof(m_abySharedPort3));
		}
	}



	return TRUE;
}


void CMgrPort::FreeSharePort(WORD wStartPort, WORD wPortNumber)
{
	BYTE m_abySharedPort[MAXWORD];		 // 共享内存端口,1:表示该端口已被其他NC占用,0:空闲
	BYTE m_abySharedPort2[MAXWORD];		// nvr
	BYTE m_abySharedPort3[MAXWORD];		// 平台的
	memset(m_abySharedPort, 0, sizeof(m_abySharedPort2));
	memset(m_abySharedPort2, 0, sizeof(m_abySharedPort2));
	memset(m_abySharedPort3, 0, sizeof(m_abySharedPort3));

	// 释放共享内存
	if (m_hSharedFile[0] != NULL)
	{
		LPBYTE lpData = (LPBYTE)MapViewOfFile(m_hSharedFile[0], FILE_MAP_WRITE, 0, 0, 0);
		if (lpData != NULL)
		{
			memcpy(m_abySharedPort, lpData, sizeof(m_abySharedPort));
			for (WORD wPort = wStartPort; wPort < wStartPort + wPortNumber; wPort++)
				m_abySharedPort[wPort - 1] = 1;
			//
			memcpy(lpData, m_abySharedPort, sizeof(m_abySharedPort));

			UnmapViewOfFile(lpData);
		}

		CloseHandle(m_hSharedFile[0]);
		m_hSharedFile[0] = NULL;
	}

	if (m_hSharedFile[1] != NULL)
	{
		LPBYTE lpData = (LPBYTE)MapViewOfFile(m_hSharedFile[1], FILE_MAP_WRITE, 0, 0, 0);
		if (lpData != NULL)
		{
			memcpy(m_abySharedPort2, lpData, sizeof(m_abySharedPort2));
			for (WORD wPort = wStartPort; wPort < wStartPort + wPortNumber; wPort++)
				m_abySharedPort2[wPort - 1] = 1;
			//
			memcpy(lpData, m_abySharedPort2, sizeof(m_abySharedPort2));

			UnmapViewOfFile(lpData);
		}

		CloseHandle(m_hSharedFile[0]);
		m_hSharedFile[0] = NULL;
	}

	if (m_hSharedFile[2] != NULL)
	{
		LPBYTE lpData = (LPBYTE)MapViewOfFile(m_hSharedFile[2], FILE_MAP_WRITE, 0, 0, 0);
		if (lpData != NULL)
		{
			memcpy(m_abySharedPort3, lpData, sizeof(m_abySharedPort3));
			for (WORD wPort = wStartPort; wPort < wStartPort + wPortNumber; wPort++)
				m_abySharedPort3[wPort - 1] = 1;
			//
			memcpy(lpData, m_abySharedPort3, sizeof(m_abySharedPort3));

			UnmapViewOfFile(lpData);
		}

		CloseHandle(m_hSharedFile[0]);
		m_hSharedFile[0] = NULL;
	}

}

