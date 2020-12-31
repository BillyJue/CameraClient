#include "dlg_deviceInfo.h"
#include "define.h"
#include <MgrData.h>
#include <QClipboard>
#include <QPixmap>
#include <QPainter>
#include <QPen>
#include <QTimer>
TIPC_DevInfo m_tDevInfo;
Dlg_DeviceInfo::Dlg_DeviceInfo(QWidget *parent)
	: MyWidget(parent), m_bUpdateOsd(false)
	, m_tempPix(nullptr), m_messageBox(nullptr)
{
	m_bLicense = false;
	memset(&m_tOsdImag, 0, sizeof(TIPC_OsdImgData));
	MgrData::getInstance()->GetMgrSubject()->Add(this);
	ui.setupUi(this);
	DWORD dIp = MgrData::getInstance()->GetIP();
	
	connect(ui.btn_loadSecret, SIGNAL(clicked()), this, SLOT(slot_loadSecret_clicked()));
	connect(ui.btn_copy, SIGNAL(clicked()), this, SLOT(slot_copy_clicked()));
	connect(ui.btn_save, SIGNAL(clicked()), this, SLOT(slot_save_clicked()));
	connect(ui.ledt_deviceInfo_2, SIGNAL(textChanged(const QString &)), this, SLOT(slot_textchanged(const QString &)));
	ui.stackedWidget->setCurrentIndex(0);
	ui.label_16->setVisible(false);
	ui.btn_copy->setVisible(false);
	ui.label_10->setVisible(false);
	ui.stackedWidget->setVisible(false);
	ui.btn_loadSecret->setVisible(false);
	ui.label_11->setVisible(false);
	
	ui.lab_serizeNumber_2->setVisible(false);
	ui.lab_support_2->setVisible(false);
}

void Dlg_DeviceInfo::InitData(bool is /* = true */)
{
	if (is)
	{
		GetParamDevInfo();
	}
	
}

void Dlg_DeviceInfo::slot_loadSecret_clicked()
{
	QString sText = ui.btn_loadSecret->text();
	if (ui.stackedWidget->currentIndex() == 0)
	{
		m_bLicense = true;
		ui.stackedWidget->setCurrentIndex(1);
		ui.btn_loadSecret->setText(GBUTF8("±£¥Ê√‹‘ø"));
	}
	else
	{
		m_bLicense = false;
		ui.stackedWidget->setCurrentIndex(0);
		ui.btn_loadSecret->setText(GBUTF8("µº»Î√‹‘ø"));

		//±£¥Ê√‹‘ø
		{
			QString sSceret = ui.lineEdit_2->text();
			QByteArray arr = sSceret.toLatin1();
			char *sz = arr.data();
			unsigned int license[16];
			memset(license, 0, sizeof(license));
			sscanf(sz, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", &license[0], &license[1], &license[2]
				, &license[3], &license[4], &license[5], &license[6], &license[7], &license[8], &license[9], &license[10], &license[11]
				, &license[12], &license[13], &license[14], &license[15]);
			for (int i = 0; i < 16; i++)
			{
				m_tDevInfo.szLicense[i] = license[i];
			}
			m_tDevInfo.szLicense[16] = 0;
		}
		char szz[129];
		memset(szz, 0, 129);
		for (int i = 0; i < 16; i++)
		{
			sprintf(szz, "%s%d,", szz, m_tDevInfo.szLicense[i]);
		}
		QString sLicense = QString(QLatin1String(szz));
		ui.lab_seceret_3->setText(sLicense);
	}
}

void Dlg_DeviceInfo::slot_textchanged(const QString &str)
{
	m_bUpdateOsd = true;
}

void Dlg_DeviceInfo::OnSetTxtVisible()
{
	if (m_messageBox)
	{
		m_messageBox->close();
		SAFE_DELETE(m_messageBox);
	}
}

void Dlg_DeviceInfo::slot_save_clicked()
{
	upDateDevInfo(false);
	for (int i = 0; i < 10; i++)
	{
		wchar_t ws[129];
		ZeroMemory(ws, 129);
		char2wchar(ws, m_tDevInfo.szSceneName[i]);
		QString strTemp = QString::fromWCharArray(ws);
		if (strTemp.isEmpty())
		{
			QString sText = ui.ledt_deviceInfo_2->text();
			ZeroMemory(ws, 129);
			sText.toWCharArray(ws);
			wchar2char(m_tDevInfo.szSceneName[i], ws);
			m_tDevInfo.szSceneName[i][127] = 0;
		}

	}

	OnSetOsdPic();
	SetParamDevInfo();
	if (!m_messageBox)
	{
		QTimer::singleShot(1200, this, &Dlg_DeviceInfo::OnSetTxtVisible);
		m_messageBox = new Dlg_MessageBox;
		m_messageBox->SetInfoText(GBUTF8("±£¥Ê≥…π¶£°"));
		m_messageBox->SetBtnNoVisible(false);
		m_messageBox->exec();
	}
	
}

void Dlg_DeviceInfo::slot_copy_clicked()
{
	QClipboard *board = QApplication::clipboard();
	QString sCopyText = ui.lab_serizeNumber_2->text();
	board->setText(sCopyText);
}

Dlg_DeviceInfo::~Dlg_DeviceInfo()
{
	MgrData::getInstance()->GetMgrSubject()->Del(this);
	if (m_tOsdImag.szBuf != NULL)
	{
		delete m_tOsdImag.szBuf;
		m_tOsdImag.szBuf = NULL;
	}
	if (m_tempPix)
	{
		delete m_tempPix;
		m_tempPix = nullptr;
	}
}


int Dlg_DeviceInfo::BmpToFont(const char *pFilePath, char *pBuf)
{
	BITMAPFILEHEADER bmp_header = { 0 };
	BITMAPINFOHEADER bmp_info = { 0 };
	FILE *pFile = fopen(pFilePath, "rb+");
	if (pFile == NULL)
	{
		return -1;
	}

	fread(&bmp_header.bfType, sizeof(bmp_header.bfType), 1, pFile);
	fread(&bmp_header.bfSize, sizeof(bmp_header.bfSize), 1, pFile);
	fread(&bmp_header.bfReserved1, sizeof(bmp_header.bfReserved1), 1, pFile);
	fread(&bmp_header.bfReserved2, sizeof(bmp_header.bfReserved2), 1, pFile);
	fread(&bmp_header.bfOffBits, sizeof(bmp_header.bfOffBits), 1, pFile);
	fread(&bmp_info, sizeof(BITMAPINFOHEADER), 1, pFile);

	int realWidth = (((bmp_info.biWidth * bmp_info.biBitCount) + 31) >> 5) << 2;

	char *pBufBmp = (char *)malloc(bmp_info.biHeight * realWidth);
	fseek(pFile, bmp_header.bfOffBits, SEEK_SET);
	fread(pBufBmp, 1, bmp_info.biHeight * realWidth, pFile);
	fclose(pFile);

	if (bmp_info.biBitCount != 1)
	{
		free(pBufBmp);
		return -1;
	}

	int num = bmp_info.biWidth / 16;
	int n = 0;

	for (int i = 0; i < num; i++)
	{
		for (int j = 16; j != 0; j--)
		{
			memcpy(pBuf + n, pBufBmp + (j - 1)*realWidth + i * 2, 2);
			n += 2;
		}
	}

	free(pBufBmp);
	return 0;
}


void Dlg_DeviceInfo::SetParamDevInfo()
{
	MgrItsCtrl* pMgrItsCtrl = MgrData::getInstance()->GetMgrItsCtrl();
	
	if (pMgrItsCtrl != NULL)
	{
		DWORD dIp = MgrData::getInstance()->GetIP();
		ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dIp);
		if (pCtrl != NULL)
		{
			bool bRet = pCtrl->SetIpcDevInfo(&m_tDevInfo);
			if (!bRet)
			{
			}
		}
	}
}

void Dlg_DeviceInfo::GetParamDevInfo()
{
	MgrItsCtrl* pMgrItsCtrl = MgrData::getInstance()->GetMgrItsCtrl();
	if (pMgrItsCtrl != NULL)
	{
		DWORD dIp = MgrData::getInstance()->GetIP();
		ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dIp);
		if (pCtrl != NULL)
		{
			bool bRet = pCtrl->GetIpcDevInfo(NULL, (u32)(CObserver*)this);
			if (!bRet)
			{
			}
		}
	}
}

// π˝¬À∑«∫∫◊÷∫Õ÷ÿ∏¥∫∫◊÷
void Dlg_DeviceInfo::FilterOsdName(char *p1, char *p2)
{
	char a, b;
	int n = 0;
	int m = 0;
	int len = strlen(p2);
	while ((len - m) > 1)
	{
		a = *(p2 + m);
		b = *(p2 + m + 1);
		// ≈–∂œ «∑ÒŒ™∫∫◊÷
		if ((unsigned char)a >= 0xB0 && (unsigned char)b >= 0xA1)
		{
			bool isRepeat = false;
			// ≈–∂œ∫∫◊÷ «∑Ò÷ÿ∏¥
			for (int i = 0; i < n; i += 2)
			{
				if (p1[i] == a && p1[i + 1] == b)
				{
					isRepeat = true;
				}
			}

			if (isRepeat == false)
			{
				p1[n++] = a;
				p1[n++] = b;
			}
			m += 2;
		}
		else
		{
			m++;
		}
	}
}



void Dlg_DeviceInfo::OnCreateOsdPic(QString filePath)
{
	char *pp = m_tDevInfo.szDevName;
	m_tempPix = new QPixmap(512, 16);
	m_tempPix->fill(Qt::black);
	QPainter painter(m_tempPix);
	wchar_t osdText[640] = { 0 };
	char osdtemp[320] = { 0 };
	char osdtemp2[320] = { 0 };
	memcpy(osdtemp2, m_tDevInfo.szDevName, strlen(m_tDevInfo.szDevName));
	for (int i = 0; i < 4; i++)
	{
		int len = strlen(osdtemp2);
		memcpy(&osdtemp2[len], m_tDevInfo.szSceneName[i], strlen(m_tDevInfo.szSceneName[i]));
	}
	FilterOsdName((char *)osdtemp, (char *)osdtemp2);
	char2wchar(osdText, osdtemp);
	memcpy(m_tOsdImag.szOsdContent, osdtemp, 320);
	wchar_t cContent[129];
	char2wchar(cContent, m_tOsdImag.szOsdContent);
	QString sContent = QString::fromWCharArray(cContent);
	QPen p = painter.pen();
	p.setColor(QColor(255, 255, 255));
	QFont sFont = painter.font();
	sFont.setPointSize(16);
	sFont.setFamily(GBUTF8("ÀŒÃÂ"));
	painter.setFont(sFont);
	painter.setPen(p);
	painter.drawText(0, 0, sContent);
	m_tempPix->save(filePath);
	
	
	

}


void Dlg_DeviceInfo::OnSetOsdPic()
{
	if (!m_bUpdateOsd)
	{
		return;
	}

	m_bUpdateOsd = false;

	QString sfilePath = MgrData::getInstance()->GetUserPath(); 
	sfilePath += "osd.bmp";
	OnCreateOsdPic(sfilePath);

	m_tOsdImag.dwSize = strlen(m_tOsdImag.szOsdContent) * 16;
	m_tOsdImag.szBuf = new char[m_tOsdImag.dwSize];

	std::string filePath2 = sfilePath.toStdString();
	BmpToFont(filePath2.c_str(), (char *)m_tOsdImag.szBuf);

	MgrItsCtrl* pMgrItsCtrl = MgrData::getInstance()->GetMgrItsCtrl();
	if (pMgrItsCtrl != NULL)
	{
		DWORD dIp = MgrData::getInstance()->GetIP();
		ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dIp);
		if (pCtrl != NULL)
		{
			bool bRet = pCtrl->SetOsdPic(&m_tOsdImag);
			if (!bRet)
			{
			}
		}
	}

	delete m_tOsdImag.szBuf;
	m_tOsdImag.szBuf = NULL;
}

#include <QTextCodec>
void Dlg_DeviceInfo::upDateDevInfo(bool is)
{
	if (is)
	{
		//÷µœ‘ æ
	
		char sName[129];
		ZeroMemory(sName, 129);
		wchar_t szName[129];
		ZeroMemory(szName, 129);
		sprintf(sName, "%s", m_tDevInfo.szDevName);
		char2wchar(szName, sName);
		QString sDevName = QString::fromWCharArray(szName);

		char cdev[129];
		wchar_t wdev[129];
		ZeroMemory(cdev, 129);
		ZeroMemory(wdev, 129);
		sprintf(cdev, "%s", m_tDevInfo.szDevType);
		char2wchar(wdev, cdev);
		QString sDevType = QString::fromWCharArray(wdev);

		wchar_t sWzDevLabel[MAX_SIZE_LEN32];
		ZeroMemory(sWzDevLabel, MAX_SIZE_LEN32);
		char2wchar(sWzDevLabel, m_tDevInfo.szDevLabel);

		ui.ledt_deviceInfo_2->setText(sDevName);//GBUTF8("%1").arg(m_tDevInfo.szDevName)
		ui.ledt_deviceNumber_2->setText(QString("%1").arg(QString::fromWCharArray(sWzDevLabel)));
		ui.lab_sbxh_2->setText(QString("%1").arg(sDevType));//m_tDevInfo.szDevType
		ui.lab_softVersion_2->setText(QString("%1").arg(m_tDevInfo.szSoftWareVer));
		ui.lab_baseSoftVersion_2->setText(QString("%1").arg(m_tDevInfo.szSoftWareBaseVer));
		char	sz[129];
		ZeroMemory(sz, 129);
		sprintf(sz, "%s", m_tDevInfo.szSn);
		int len = strlen(m_tDevInfo.szSn);
		for (int i = 0; i < len; i++)
		{
			int value = m_tDevInfo.szSn[i];
			if (m_tDevInfo.szSn[i] <= '9' &&  m_tDevInfo.szSn[i] >= '0')
			{
				value = value - 0x30;
			}

			if (i == 0)
			{
				sprintf(sz, "%s(%02x", sz, value);
			}
			else
			{
				sprintf(sz, "%s,%02x", sz, value);
			}

			if (i == len - 1)
			{
				sprintf(sz, "%s)", sz);
			}

		}
		ui.lab_serizeNumber_2->setText(QString("%1").arg(sz));


		memset(sz, 0, 129);
		for (int i = 0; i < 16; i++)
		{
			sprintf(sz, "%s%d,", sz, m_tDevInfo.szLicense[i]);
		}
		QString sLicense = QString(QLatin1String(sz));
		ui.lab_seceret_3->setText(sLicense);
	}
	else
	{
		char* ptr;
		QByteArray ba;
		QString str = ui.ledt_deviceInfo_2->text();
		wchar_t pDevName[129];
		ZeroMemory(pDevName, 129);
		str.toWCharArray(pDevName);
		ZeroMemory(m_tDevInfo.szDevName, MAX_SIZE_LEN128);
		wchar2char(m_tDevInfo.szDevName, pDevName);
		
		wchar_t pNumber[129];
		ZeroMemory(pNumber, 129);
		QString sNumber = ui.ledt_deviceNumber_2->text();
		sNumber.toWCharArray(pNumber);
		ZeroMemory(m_tDevInfo.szDevLabel, MAX_SIZE_LEN32);
		wchar2char(m_tDevInfo.szDevLabel, pNumber);
		if (m_bLicense)
		{
			//±£¥Ê√‹‘ø
			{
				QString sSceret = ui.lineEdit_2->text();
				QByteArray arr = sSceret.toLatin1();
				char *sz = arr.data();
				unsigned int license[16];
				memset(license, 0, sizeof(license));
				sscanf(sz, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", &license[0], &license[1], &license[2]
					, &license[3], &license[4], &license[5], &license[6], &license[7], &license[8], &license[9], &license[10], &license[11]
					, &license[12], &license[13], &license[14], &license[15]);
				for (int i = 0; i < 16; i++)
				{
					m_tDevInfo.szLicense[i] = license[i];
				}
				m_tDevInfo.szLicense[16] = 0;
			}
		}
		
	}
}



void Dlg_DeviceInfo::OnObserverNotify(LPARAM lHint, LPVOID pHint)
{

	u32 dwEvtType = (u32)lHint;
	u32 dwId = (u32)pHint;

	switch (dwEvtType)
	{
	case NMC_GET_IPC_DEVINFO:
	{
		TIPC_GetDevRsp* p = (TIPC_GetDevRsp*)pHint;
		if (p->nRetCode == 0)
		{
			memcpy(&m_tDevInfo, &(p->tDev), sizeof(TIPC_DevInfo));
			upDateDevInfo();
		}
	}
	break;
	default:
		break;
	}
}