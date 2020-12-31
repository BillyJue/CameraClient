// portmanager.cpp: implementation of the CMgrPort class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "mgr_port.h"
#include "winsock2.h"
#include <QDebug>
const TCHAR* szPortKeyWord2 = L"kedacom_portmanager";			// NVR		
const TCHAR* szPortKeyWord3 = L"Global\\PortDistribution";		// CU
const TCHAR* szPortKeyWord = L"its_portmanager";				// ���ܽ�ͨ
const int g_wStartPort = 30000;										// ����˿ڵ���ʼλ��


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
				// 				TraceInfo("����IP��ַ:%d:%d:%d:%d\n", dwLocalIP>>24, (dwLocalIP<<8)>>24, 
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
			// Ѱ����dwDestIPͬ���ε�IP
			BOOL32 bFind = FALSE;
			int nPos = 31;
			DWORD dwIP = 0;
			for (int nAdapter = 0; tHost->h_addr_list[nAdapter]; nAdapter++)
			{
				memcpy(&dwIP, tHost->h_addr_list[nAdapter], tHost->h_length);
				dwIP = ntohl(dwIP);
				// ����Ŀ��IPƥ��λ�����ı���IP
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

			// ���û���ҵ�ͬ���ε�IP��ʹ�õ�һ��IP
			if (bFind == FALSE)
			{
				memcpy(&dwLocalIP, tHost->h_addr_list[0], tHost->h_length);
				dwLocalIP = ntohl(dwLocalIP);
			}
			// ��ӡIP��ַ��Ϣ
			// 			TraceDebug("�豸IP��ַ:%d:%d:%d:%d\n", dwDestIP>>24, (dwDestIP<<8)>>24, 
			// 				(dwDestIP<<16)>>24, (dwDestIP<<24)>>24 );
			// 			TraceDebug("����IP��ַ:%d:%d:%d:%d\n", dwLocalIP>>24, (dwLocalIP<<8)>>24, 
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
	// ���˿�
	SOCKET tsocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (INVALID_SOCKET == tsocket)
	{
		qCritical() << ("����SOCKETʧ�ܣ��޷����˿�%d\n", wLocalPort);
		return TRUE;
	}

	sockaddr_in tSockAddr;
	tSockAddr.sin_family = AF_INET;
	tSockAddr.sin_port = htons(wLocalPort);
	// ��ȡ����IP
	u32 dwLocalIP = 0;
	if (!GetLocalIP(dwLocalIP))
	{
		qCritical() << (("[CKDMPUSearch] Initialize: ��ȡ����IPʧ��"));
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
		qCritical() << ("UDP�˿�%d��ռ�ã�socket����ţ�%d\n", wLocalPort, nErrorCode);
		closesocket(tsocket);
		return TRUE;
	}

	closesocket(tsocket);

	return FALSE;
}


/*=============================================================================
�� �� ���� GetIdlePort
��    �ܣ� ���������Ŀ��ж˿ڣ�������ռ��
�㷨ʵ�֣� 1���ڹ���˿��ڲ��ң�2�����˿��Ƿ��ѱ�ռ��
ȫ�ֱ�����
��    ���� WORD& wFirstIdlePort - ���صĿ��ж˿���ʼ��
WORD wStartPort - ָ���Ŀ�ʼ�˿ںţ�����NCUI������������
WORD wPortNumber - ��Ҫ��������˿ڣ�����NCUI��74*4����IPCCTRL��8
�� �� ֵ�� BOOL
=============================================================================*/
BOOL CMgrPort::AllocSharePort(WORD& wFirstIdlePort, WORD wStartPort, WORD wPortNumber)
{
	BYTE m_abySharedPort[MAXWORD];		 // �����ڴ�˿�,1:��ʾ�ö˿��ѱ�����NCռ��,0:����
	BYTE m_abySharedPort2[MAXWORD];		// nvr
	BYTE m_abySharedPort3[MAXWORD];		// ƽ̨��
	memset(m_abySharedPort, 0, sizeof(m_abySharedPort2));
	memset(m_abySharedPort2, 0, sizeof(m_abySharedPort2));
	memset(m_abySharedPort3, 0, sizeof(m_abySharedPort3));

	// ��ȡnvr�����ڴ�
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


	// ��ȡcu�����ڴ�
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

	// ��ȡits�����ڴ�
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

	// �ҵ����ж˿���ʼ��
	WORD wFirstPort = wStartPort;
	//////////////////////////////////////////////////////////////////////////
	do
	{
		DWORD dwMaxPort = (DWORD)wFirstPort + (DWORD)wPortNumber;
		if (dwMaxPort > MAXWORD)
		{   // �˿ں��ѳ���ָ���ķ�Χ
			return FALSE;
		}

		// �ڹ����ڴ��в��ң��жϸö˿��Ƿ��ѱ�����NCռ��
		WORD wPort = wFirstPort;
		for (; wPort < dwMaxPort; wPort++)
		{
			if (m_abySharedPort[wPort - 1] != 0)
			{   // �ö˿��ѱ�ռ��-->�ö˿ڶ�����
				break;
			}

			if (m_abySharedPort2[wPort - 1] != 0)
			{   // �ö˿��ѱ�ռ��-->�ö˿ڶ�����
				break;
			}

			if (m_abySharedPort3[wPort - 1] != 0)
			{   // �ö˿��ѱ�ռ��-->�ö˿ڶ�����
				break;
			}
		}


		if (wPort == dwMaxPort)
		{   // �ҵ�һ��δ������NCռ�õĶ˿�-->���緽ʽ���ö˿��Ƿ����
			for (wPort = wFirstPort; wPort < dwMaxPort; wPort++)
			{
				if (CheckPortIsUsed(wPort))
				{   // �˿ڶ����κ�һ���˿��ѱ�ռ��-->�������������˿ڶ�
					break;
				}
			}
			if (wPort == dwMaxPort)
			{   // �˿ڶ������ж˿ھ�����-->�ҵ����ж˿ڶ�
				break;
			}
		}
		//         wFirstPort += wPortNumber;
		// Ϊ���ڼ���˿ڵ������԰ٵ�����������
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

	// д�빲���ڴ�
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
	BYTE m_abySharedPort[MAXWORD];		 // �����ڴ�˿�,1:��ʾ�ö˿��ѱ�����NCռ��,0:����
	BYTE m_abySharedPort2[MAXWORD];		// nvr
	BYTE m_abySharedPort3[MAXWORD];		// ƽ̨��
	memset(m_abySharedPort, 0, sizeof(m_abySharedPort2));
	memset(m_abySharedPort2, 0, sizeof(m_abySharedPort2));
	memset(m_abySharedPort3, 0, sizeof(m_abySharedPort3));

	// �ͷŹ����ڴ�
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

