#include "dlg_hardware.h"
#include "MgrData.h"
#include "Dlg_MessageBox.h"
#include "MgrData.h"
#include <QMessageBox>
#include <QListView>
Dlg_HardWare *m_hardware;

void __stdcall VideoDrawCB(PLAYERHANDLE hPlayHandle, unsigned long dwPaintDC, unsigned long dwFrmID, unsigned long dwParam)
{
	Dlg_HardWare *pDlg = (Dlg_HardWare *)dwParam;
	HDC hdc = (HDC)dwPaintDC;

}

void WorkerThread::setMap(QMap<int, QCheckBox*> &tempMap)
{
	m_chkMap.clear();
	QMap<int, QCheckBox*>::iterator itor;
	for (itor = tempMap.begin(); itor != tempMap.end(); itor++)
	{
		m_chkMap.insert(itor.key(), itor.value());
	}
}

bool WorkerThread::LEDTest(int port, int status)
{
	u32 dwIP = MgrData::getInstance()->GetIP();
	ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dwIP);
	if (pCtrl != NULL)
	{
		u8 buf[2];
		buf[0] = port;
		buf[1] = status;
		bool bRet = pCtrl->PostTransparentMsg((void *)buf, 2, EV_SET_FACTORYMODE_LED, (u32)(CObserver*)m_hardware);
		if (!bRet)
		{
		}

		return bRet;
	}

	return false;
}

bool WorkerThread::FlashTest(int port, int status)
{
	u32 dwIP = MgrData::getInstance()->GetIP();
	ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dwIP);
	if (pCtrl != NULL)
	{
		u8 buf[2];
		buf[0] = 1 << port;
		buf[1] = status;
		bool bRet = pCtrl->PostTransparentMsg((void *)buf, 2, EV_SET_FACTORYMODE_TRIGGER_FPORT, (u32)(CObserver*)m_hardware);
		if (!bRet)
		{
		}

		return bRet;
	}

	return false;
}

bool WorkerThread::PLTest(int status)
{
	u32 dwIP = MgrData::getInstance()->GetIP();
	ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dwIP);
	if (pCtrl != NULL)
	{
		u8 buf[2];
		buf[0] = status;
		bool bRet = pCtrl->PostTransparentMsg((void *)buf, 1, EV_SET_FACTORYMODE_PL, (u32)(CObserver*)m_hardware);
		if (!bRet)
		{
		}

		return bRet;
	}

	return false;
}

bool WorkerThread::SerialTest(int port)
{
	u32 dwIP = MgrData::getInstance()->GetIP();
	ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dwIP);
	if (pCtrl != NULL)
	{
		u8 buf[2];
		buf[0] = port;
		bool bRet = pCtrl->PostTransparentMsg((void *)buf, 1, EV_SET_FACTORYMODE_SERIAL_REQ, (u32)(CObserver*)m_hardware);
		if (!bRet)
		{
		}
		return bRet;
	}

	return false;
}

bool WorkerThread::InputTest(int run)
{
	u32 dwIP = MgrData::getInstance()->GetIP();
	ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dwIP);
	if (pCtrl != NULL)
	{
		u8 buf[2];
		buf[0] = run;
		bool bRet = pCtrl->PostTransparentMsg((void *)buf, 1, EV_SET_FACTORYMODE_INPUT, (u32)(CObserver*)m_hardware);
		if (!bRet)
		{
		}

		return bRet;
	}

	return false;
}

bool WorkerThread::OutputTest(int port, int status)
{
	u32 dwIP = MgrData::getInstance()->GetIP();
	ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dwIP);
	if (pCtrl != NULL)
	{
		u8 buf[2];
		buf[0] = port;
		buf[1] = status;
		bool bRet = pCtrl->PostTransparentMsg((void *)buf, 2, EV_SET_FACTORYMODE_OUTPUT, (u32)(CObserver*)m_hardware);
		if (!bRet)
		{
		}

		return bRet;
	}

	return false;
}

HANDLE WorkerThread::InitCom(const char* comName, int baudRate)
{
	HANDLE hCom;
	hCom = CreateFileA(comName,//COM7口
		GENERIC_READ | GENERIC_WRITE, //允许读和写
		0, //独占方式
		NULL,
		OPEN_EXISTING, //打开而不是创建
		0, //同步方式
		NULL);
	if (hCom == (HANDLE)-1)
	{
		return NULL;
	}
	SetupComm(hCom, 100, 100); //输入缓冲区和输出缓冲区的大小都是100
	COMMTIMEOUTS TimeOuts;
	//设定读超时
	TimeOuts.ReadIntervalTimeout = MAXDWORD;
	TimeOuts.ReadTotalTimeoutMultiplier = 0;
	TimeOuts.ReadTotalTimeoutConstant = 0;
	//在读一次输入缓冲区的内容后读操作就立即返回，
	//而不管是否读入了要求的字符。
	//设定写超时
	TimeOuts.WriteTotalTimeoutMultiplier = 100;
	TimeOuts.WriteTotalTimeoutConstant = 500;
	SetCommTimeouts(hCom, &TimeOuts); //设置超时

	DCB dcb;
	GetCommState(hCom, &dcb);
	dcb.BaudRate = baudRate; //波特率为9600
	dcb.ByteSize = 8; //每个字节有8位
	dcb.Parity = NOPARITY; //无奇偶校验位
	dcb.StopBits = 1; //两个停止位
	SetCommState(hCom, &dcb);
	PurgeComm(hCom, PURGE_TXCLEAR | PURGE_RXCLEAR);
	return hCom;
}

int WorkerThread::ReadData(HANDLE handler, char* buffer, int len)
{
	Int32 ReadLen;
	DWORD dwError;
	BOOL bReadStat;
	OVERLAPPED m_osRead;

	memset(&m_osRead, 0, sizeof(OVERLAPPED));
	m_osRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	bReadStat = ReadFile(handler, buffer, (DWORD)len, (LPDWORD)(&ReadLen), &m_osRead);
	if (!bReadStat)
	{
		if (GetLastError() == ERROR_IO_PENDING)
		{
			while (!GetOverlappedResult(handler, &m_osRead, (LPDWORD)&ReadLen, TRUE))
			{
				dwError = GetLastError();
				if (dwError == ERROR_IO_INCOMPLETE)
				{
					continue;
				}
			}
			//PurgeComm(handler, PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);
			return ReadLen;
		}
		else
		{
			return KOSA_SFAIL;
		}
	}

	return ReadLen;
}

int WorkerThread::WriteData(HANDLE handler, char* buffer, int len)
{
	Int32 WriteLen;
	DWORD dwError;
	COMSTAT ComStat;
	DWORD dwErrorFlags;
	BOOL bWriteStat;
	OVERLAPPED m_osWrite;

	ClearCommError(handler, &dwErrorFlags, &ComStat);

	memset(&m_osWrite, 0, sizeof(OVERLAPPED));
	m_osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	bWriteStat = WriteFile(handler, buffer, (DWORD)len, (LPDWORD)(&WriteLen), &m_osWrite);
	if (!bWriteStat)
	{
		if (GetLastError() == ERROR_IO_PENDING)
		{
			while (!GetOverlappedResult(handler, &m_osWrite, (LPDWORD)&WriteLen, TRUE))
			{
				dwError = GetLastError();
				if (dwError == ERROR_IO_INCOMPLETE)
				{
					WriteLen += WriteLen;
					continue;
				}
				//PurgeComm(handler, PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);
				return WriteLen;
			}
		}
		else
		{
			return KOSA_SFAIL;
		}
	}

	return WriteLen;
}


void WorkerThread::run()
{

	if (m_chkMap[0]->isChecked())
	{
		m_dlg->SetInfoText(GBUTF8("正在测试F1"));
		for (int i = 0; i < 3; i++)
		{
			FlashTest(0, 1);
			Sleep(1000);
			FlashTest(0, 0);
			Sleep(1000);
		}
		m_dlg->SetInfoText(GBUTF8("测试F1完成"));
		Sleep(1000);
	}

	if (m_chkMap[1]->isChecked())
	{
		m_dlg->SetInfoText(GBUTF8("正在测试F2"));
		for (int i = 0; i < 3; i++)
		{
			FlashTest(1, 1);
			Sleep(1000);
			FlashTest(1, 0);
			Sleep(1000);
		}
		m_dlg->SetInfoText(GBUTF8("测试F2完成"));
		Sleep(1000);
	}

	if (m_chkMap[2]->isChecked())
	{
		m_dlg->SetInfoText(GBUTF8("正在测试F3"));
		for (int i = 0; i < 3; i++)
		{
			FlashTest(2, 1);
			Sleep(1000);
			FlashTest(2, 0);
			Sleep(1000);
		}
		m_dlg->SetInfoText(GBUTF8("测试F3完成"));
		Sleep(1000);
	}

	if (m_chkMap[3]->isChecked())
	{
		m_dlg->SetInfoText(GBUTF8("正在测试F4"));
		for (int i = 0; i < 3; i++)
		{
			FlashTest(3, 1);
			Sleep(1000);
			FlashTest(3, 0);
			Sleep(1000);
		}
		m_dlg->SetInfoText(GBUTF8("测试F4完成"));
		Sleep(1000);
	}
	
	if (m_chkMap[4]->isChecked())
	{
		m_dlg->SetInfoText(GBUTF8("正在测试LED1"));
		for (int i = 0; i < 3; i++)
		{
			LEDTest(0, 1);
			Sleep(1000);
			LEDTest(0, 0);
			Sleep(1000);
		}
		m_dlg->SetInfoText(GBUTF8("测试LED1完成"));
		Sleep(1000);
	}

	if (m_chkMap[5]->isChecked())
	{
		m_dlg->SetInfoText(GBUTF8("正在测试LED2"));
		for (int i = 0; i < 3; i++)
		{
			LEDTest(1, 1);
			Sleep(1000);
			LEDTest(1, 0);
			Sleep(1000);
		}
		m_dlg->SetInfoText(GBUTF8("测试LED2完成"));
		Sleep(1000);
	}

	if (m_chkMap[6]->isChecked())
	{
		m_dlg->SetInfoText(GBUTF8("正在测试LED3"));
		for (int i = 0; i < 3; i++)
		{
			LEDTest(2, 1);
			Sleep(1000);
			LEDTest(2, 0);
			Sleep(1000);
		}
		m_dlg->SetInfoText(GBUTF8("测试LED3完成"));
		Sleep(1000);
	}

	if (m_chkMap[17]->isChecked())
	{
		m_dlg->SetInfoText(GBUTF8("正在测试偏振镜"));
		PLTest(1);
		Sleep(4000);
		PLTest(0);
		Sleep(4000);
		m_dlg->SetInfoText(GBUTF8("测试偏振镜完成"));
		Sleep(1000);
	}

	if (m_chkMap[13]->isChecked())
	{
		m_dlg->SetInfoText(GBUTF8("正在测试IO输出1"));
		for (int i = 0; i < 3; i++)
		{
			OutputTest(1, 0);
			Sleep(1000);
			OutputTest(1, 1);
			Sleep(1000);
		}
		m_dlg->SetInfoText(GBUTF8("测试IO输出1完成"));
		Sleep(1000);
	}

	if (m_chkMap[14]->isChecked())
	{
		m_dlg->SetInfoText(GBUTF8("正在测试IO输出2"));
		for (int i = 0; i < 3; i++)
		{
			OutputTest(2, 0);
			Sleep(1000);
			OutputTest(2, 1);
			Sleep(1000);
		}
		m_dlg->SetInfoText(GBUTF8("测试IO输出2完成"));
		Sleep(1000);
	}

	if (m_chkMap[15]->isChecked())
	{
		m_dlg->SetInfoText(GBUTF8("正在测试IO输出3"));
		for (int i = 0; i < 3; i++)
		{
			OutputTest(3, 0);
			Sleep(1000);
			OutputTest(3, 1);
			Sleep(1000);
		}
		m_dlg->SetInfoText(GBUTF8("测试IO输出3完成"));
		Sleep(1000);
	}

	if (m_chkMap[16]->isChecked())
	{
		m_dlg->SetInfoText(GBUTF8("正在测试IO输出4"));
		for (int i = 0; i < 3; i++)
		{
			OutputTest(4, 0);
			Sleep(1000);
			OutputTest(4, 1);
			Sleep(1000);
		}
		m_dlg->SetInfoText(GBUTF8("测试IO输出4完成"));
		Sleep(1000);
	}
	

	if (m_chkMap[11]->isChecked() || m_chkMap[12]->isChecked())
	{
		m_dlg->SetInfoText(GBUTF8("正在测试IO输入"));
		InputTest(1);
		Sleep(60 * 1000);
		InputTest(0);
	}

	if (m_chkMap[7]->isChecked() && !m_chkMap[10]->isChecked())
	{
		{
			QMessageBox box;
			box.setWindowTitle(GBUTF8("提示"));
			box.setText(GBUTF8("请将串口插入串口1"));
			box.setStandardButtons(QMessageBox::Ok);
			box.exec();
		}
		

		QString str1 = m_hardware->GetCurSerialData();
		QString str2 = m_hardware->GetCurBaudRateData();
		int rate = str2.toInt();
		std::string name = str1.toStdString();
		HANDLE h = InitCom(name.c_str(), rate);
		if (h == NULL)
		{
			QMessageBox box;
			box.setWindowTitle(GBUTF8("错误"));
			box.setText(GBUTF8("	本地串口打开失败		"));
			box.setStandardButtons(QMessageBox::Ok);
			box.exec();
			emit signal_dlgClose();
			//m_dlg->close();
			return;
		}

		char buf[12];
		char recvBuf[12];
		bool isOk = false;
		memset(recvBuf, 0, 12);
		memcpy(buf, "com1", sizeof("com1"));
		for (int i = 0; i < 3; i++)
		{
			WriteData(h, buf, sizeof("com1"));
			Sleep(100);
			int len = ReadData(h, recvBuf, 12);
			if (len != -1 && strcmp(recvBuf, "com1") == 0)
			{
				isOk = true;
				break;
			}
		}

		if (isOk)
		{
			m_chkMap[7]->setStyleSheet("background-color:green;");
		}
		else
		{
			m_chkMap[7]->setStyleSheet("background-color:red;");
		}

		CloseHandle(h);

	}

	if (m_chkMap[8]->isChecked() && !m_chkMap[10]->isChecked())
	{
		{
			QMessageBox box;
			box.setWindowTitle(GBUTF8("提示"));
			box.setText(GBUTF8("	请将串口插入串口2	"));
			box.setStandardButtons(QMessageBox::Ok);
			box.exec();
		}
		QString str1 = m_hardware->GetCurSerialData();
		QString str2 = m_hardware->GetCurBaudRateData();
		int rate = str2.toInt();
		std::string name = str1.toStdString();
		HANDLE h = InitCom(name.c_str(), rate);
		if (h == NULL)
		{
			QMessageBox box;
			box.setWindowTitle(GBUTF8("错误"));
			box.setText(GBUTF8("	本地串口打开失败		"));
			box.setStandardButtons(QMessageBox::Ok);
			box.exec();
			emit signal_dlgClose();
			return;
		}

		char buf[12];
		char recvBuf[12];
		bool isOk = false;
		memset(recvBuf, 0, 12);
		memcpy(buf, "com2", sizeof("com2"));
		for (int i = 0; i < 3; i++)
		{
			WriteData(h, buf, sizeof("com2"));
			Sleep(100);
			int len = ReadData(h, recvBuf, 12);
			if (len != -1 && strcmp(recvBuf, "com2") == 0)
			{
				isOk = true;
				break;
			}
		}
		
		if (isOk)
		{
			m_chkMap[8]->setStyleSheet("background-color:green;");
		}
		else
		{
			m_chkMap[8]->setStyleSheet("background-color:red;");
		}

		CloseHandle(h);

	}
	
	if (m_chkMap[9]->isChecked() && !m_chkMap[10]->isChecked())
	{
		{
			QMessageBox box;
			box.setWindowTitle(GBUTF8("提示"));
			box.setText(GBUTF8("	请将串口插入串口2	"));
			box.setStandardButtons(QMessageBox::Ok);
			box.exec();
		}
		QString str1 = m_hardware->GetCurSerialData();
		QString str2 = m_hardware->GetCurBaudRateData();
		int rate = str2.toInt();
		std::string name = str1.toStdString();
		HANDLE h = InitCom(name.c_str(), rate);
		if (h == NULL)
		{
			QMessageBox box;
			box.setWindowTitle(GBUTF8("错误"));
			box.setText(GBUTF8("	本地串口打开失败		"));
			box.setStandardButtons(QMessageBox::Ok);
			box.exec();
			emit signal_dlgClose();
			return;
		}

		char buf[12];
		char recvBuf[12];
		bool isOk = false;
		memset(recvBuf, 0, 12);
		memcpy(buf, "com3", sizeof("com3"));
		for (int i = 0; i < 3; i++)
		{
			WriteData(h, buf, sizeof("com3"));
			Sleep(100);
			int len = ReadData(h, recvBuf, 12);
			if (len != -1 && strcmp(recvBuf, "com3") == 0)
			{
				isOk = true;
				break;
			}
		}

		if (isOk)
		{
			m_chkMap[9]->setStyleSheet("background-color:green;");
		}
		else
		{
			m_chkMap[9]->setStyleSheet("background-color:red;");
		}

		CloseHandle(h);

	}

	if (m_chkMap[10]->isChecked())
	{
		SerialTest(0);
	}

	m_dlg->SetInfoText(GBUTF8("测试完成"));
	Sleep(2000);

	emit signal_dlgClose();
}

Dlg_HardWare::Dlg_HardWare(QWidget *parent)
	: MyWidget(parent), m_thread(nullptr), m_tempDlg(nullptr)
{
	ui.setupUi(this);
	connect(ui.btn_test, SIGNAL(clicked()), this, SLOT(Slot_HardTestClicked()));
	connect(ui.btn_selectAll, SIGNAL(clicked()), this, SLOT(Slot_SelectAllClicked()));
	MgrData::getInstance()->GetMgrSubject()->Add(this);
	Init();
	
}


void Dlg_HardWare::InitData(bool is /* = true */)
{
	ui.video->ControlPlayVideo(is);
}

Dlg_HardWare::~Dlg_HardWare()
{
	if (m_thread)
	{
		m_thread->quit();
		m_thread->wait();
		SAFE_DELETE(m_thread);
	}
	
	
	ui.video->ControlPlayVideo(false);
	MgrData::getInstance()->GetMgrSubject()->Del(this);
	
}

void Dlg_HardWare::Slot_HardTestClicked()
{
	TLocalCfgInfo tLocalCfgInfo;
	MgrData::getInstance()->GetMgrLocalCfg()->GetLocalCfg(&tLocalCfgInfo);
	if (ui.cmb_localSerial->currentIndex() != tLocalCfgInfo.serialNum || ui.cmb_baudRate->currentIndex() != tLocalCfgInfo.serialRate)
	{
		tLocalCfgInfo.serialNum = ui.cmb_localSerial->currentIndex();
		tLocalCfgInfo.serialRate = ui.cmb_baudRate->currentIndex();
		MgrData::getInstance()->GetMgrLocalCfg()->SetLocalCfg(&tLocalCfgInfo, true);
	}

	QMap<int, QCheckBox*> tempMap;
	tempMap.insert(0, ui.ck_F1);
	tempMap.insert(1, ui.ck_F2);
	tempMap.insert(2, ui.ck_F3);
	tempMap.insert(3, ui.ck_F4);
	tempMap.insert(4, ui.ck_LED1);
	tempMap.insert(5, ui.ck_LED2);
	tempMap.insert(6, ui.ck_LED3);
	tempMap.insert(17, ui.ck_polariscope);
	tempMap.insert(13, ui.ck_outputIO1);
	tempMap.insert(14, ui.ck_outputIO2);
	tempMap.insert(15, ui.ck_outputIO3);
	tempMap.insert(16, ui.ck_outputIO4);
	tempMap.insert(11, ui.ck_inputIO1);
	tempMap.insert(12, ui.ck_inputIO2);
	tempMap.insert(10, ui.ck_loopTest);
	tempMap.insert(7, ui.ck_outputIO1);
	tempMap.insert(8, ui.ck_outputIO2);
	tempMap.insert(9, ui.ck_outputIO3);
	if (!m_thread)
	{
		m_thread = new WorkerThread;
		connect(m_thread, SIGNAL(signal_dlgClose()), this, SLOT(Slot_DlgClose()));
	}
	
	m_hardware = this;
	//m_thread->setMyWidget(this);
	m_thread->setMap(tempMap);
	Dlg_MessageBox dlg;
	m_tempDlg = &dlg;
	dlg.SetBtnYesVisible(false);
	m_thread->setDlg(&dlg);
	m_thread->start();
	dlg.exec();
	m_thread->quit();
	m_thread->wait();
}

void Dlg_HardWare::Slot_DlgClose()
{
	m_tempDlg->close();
	m_thread->quit();
	m_thread->wait();
}

void Dlg_HardWare::Slot_SerialDataChanged()
{
	QMessageBox box;
	box.setWindowTitle(GBUTF8("警告"));
	box.setText(GBUTF8("	串口循环测试失败！请单个串口测试！		"));
	box.setStandardButtons(QMessageBox::Ok);
	box.exec();
}

void Dlg_HardWare::OnObserverNotify(LPARAM lHint, LPVOID pHint)
{
	u32 dwEvtType = (u32)lHint;
	u32 dwId = (u32)pHint;

	switch (dwEvtType)
	{
	case EV_SET_FACTORYMODE_SERIAL_ACK:
	{
		Uint8 *reslut = (Uint8 *)pHint;
		if (*reslut == 0)
		{
			emit signal_serialDataChanged();
			ui.ck_COM1->setStyleSheet("background-color: red");
			ui.ck_COM2->setStyleSheet("background-color: red");
			ui.ck_COM3->setStyleSheet("background-color: red");
			
		}
		else
		{
			ui.ck_COM1->setStyleSheet("background-color: green");
			ui.ck_COM2->setStyleSheet("background-color: green");
			ui.ck_COM3->setStyleSheet("background-color: green");
		}
	}
		break;
	case EV_SET_FACTORYMODE_INPUT_ACK:
	{
		Uint32 *reslut = (Uint32 *)pHint;
		if (reslut[0])
		{
			ui.ck_inputIO1->setStyleSheet("background-color: red");
		}
		else
		{
			ui.ck_inputIO1->setStyleSheet("background-color: green");
		}

		if (reslut[1])
		{
			ui.ck_inputIO2->setStyleSheet("background-color: red");
		}
		else
		{
			ui.ck_inputIO2->setStyleSheet("background-color: green");
		}
	}
		break;
	default:
		break;
	}

}


void Dlg_HardWare::Slot_SelectAllClicked()
{
	ui.ck_inputIO1->setChecked(true);
	ui.ck_inputIO2->setChecked(true);

	ui.ck_F1->setChecked(true);
	ui.ck_F2->setChecked(true);
	ui.ck_F3->setChecked(true);
	ui.ck_F4->setChecked(true);

	ui.ck_LED1->setChecked(true);
	ui.ck_LED2->setChecked(true);
	ui.ck_LED3->setChecked(true);

	ui.ck_COM1->setChecked(true);
	ui.ck_COM2->setChecked(true);
	ui.ck_COM3->setChecked(true);
	ui.ck_loopTest->setChecked(true);

	ui.ck_outputIO1->setChecked(true);
	ui.ck_outputIO2->setChecked(true);
	ui.ck_outputIO3->setChecked(true);
	ui.ck_outputIO4->setChecked(true);

	ui.ck_polariscope->setChecked(true);

	if (strcmp(MgrData::getInstance()->GetDevInfo()->szDevType, "IPC626-AU") != 0)
	{
		ui.ck_inputIO1->setChecked(false);
		ui.ck_inputIO2->setChecked(false);

		ui.ck_outputIO1->setChecked(false);
		ui.ck_outputIO2->setChecked(false);
		ui.ck_outputIO3->setChecked(false);
		ui.ck_outputIO4->setChecked(false);
	}
	else
	{
		ui.ck_F2->setChecked(false);
		ui.ck_F3->setChecked(false);
		ui.ck_F4->setChecked(false);

		ui.ck_LED3->setChecked(false);
		ui.ck_COM3->setChecked(false);
		ui.ck_loopTest->setChecked(false);
		ui.ck_polariscope->setChecked(false);
	}
}



void Dlg_HardWare::ControlPlayStauts(bool is)
{
	//if (is)
	//{
	//	//应该显示视频下面
	//	MgrData::getInstance()->GetMgrVideo()->PlayVideo((unsigned long)ui.label_3->winId());
	//	MgrData::getInstance()->GetMgrVideo()->RegisterDrawCB(VideoDrawCB, (unsigned long)this);
	//}
	//else
	//{
	//	MgrData::getInstance()->GetMgrVideo()->RegisterDrawCB(NULL, NULL);
	//	MgrData::getInstance()->GetMgrVideo()->StopPlayVideo((unsigned long)ui.label_3->winId());
	//}
}


void Dlg_HardWare::Init()
{
	for (int i = 1; i < 10; i++)
	{
		ui.cmb_localSerial->addItem(QString("COM%1").arg(i));
	}
	ui.cmb_localSerial->setView(new QListView);
	ui.cmb_baudRate->setView(new QListView);
	ui.cmb_baudRate->addItem(QString("2400"));
	ui.cmb_baudRate->addItem(QString("4800"));
	ui.cmb_baudRate->addItem(QString("9600"));
	ui.cmb_baudRate->addItem(QString("19200"));
	ui.cmb_baudRate->addItem(QString("38400"));
	ui.cmb_baudRate->addItem(QString("57600"));
	ui.cmb_baudRate->addItem(QString("115200"));

	if (strcmp(MgrData::getInstance()->GetDevInfo()->szDevType, "IPC626-AU") != 0)
	{
		ui.ck_inputIO1->setEnabled(false);
		ui.ck_inputIO2->setEnabled(false);

		ui.ck_outputIO1->setEnabled(false);
		ui.ck_outputIO2->setEnabled(false);
		ui.ck_outputIO3->setEnabled(false);
		ui.ck_outputIO4->setEnabled(false);
	}
	else
	{
		ui.ck_F2->setEnabled(false);
		ui.ck_F3->setEnabled(false);
		ui.ck_F4->setEnabled(false);
		ui.ck_LED3->setEnabled(false);
		ui.ck_COM3->setEnabled(false);
		ui.cmb_localSerial->setEnabled(false);
		ui.cmb_baudRate->setEnabled(false);
		ui.ck_polariscope->setEnabled(false);
	}

	TLocalCfgInfo tLocalCfgInfo;
	MgrData::getInstance()->GetMgrLocalCfg()->GetLocalCfg(&tLocalCfgInfo);
	ui.cmb_localSerial->setCurrentIndex(tLocalCfgInfo.serialNum);
	ui.cmb_baudRate->setCurrentIndex(tLocalCfgInfo.serialRate);
	
}
