#pragma once

#include <QWidget>
#include "ui_dlg_deviceInfo.h"
#include <windows.h>
#include <subject.h>
#include "ivhs_struct.h"
#include <QPixmap>
#include "Dlg_MessageBox.h"
#include "MyWidget.h"
#define GBUTF8(da) QString::fromLocal8Bit(da)
class Dlg_DeviceInfo : public MyWidget, public CObserver
{
	Q_OBJECT

public:
	Dlg_DeviceInfo(QWidget *parent = Q_NULLPTR);

	~Dlg_DeviceInfo();

	void InitData(bool is /* = true */);

	void OnObserverNotify(LPARAM lHint, LPVOID pHint);

	void SetParamDevInfo();

	void GetParamDevInfo();

	void upDateDevInfo(bool is = true); 

	void OnSetOsdPic();

	void OnSetTxtVisible();

	void OnCreateOsdPic(QString filePath);

	void FilterOsdName(char *p1, char *p2);

	int  BmpToFont(const char *pFilePath, char *pBuf);

public slots:

	void slot_loadSecret_clicked();

	void slot_copy_clicked();

	void slot_textchanged(const QString &str);

	void slot_save_clicked();

private:
	Ui::dlg_deviceInfo ui;

	bool m_bUpdateOsd;

	bool m_bLicense;

	TIPC_OsdImgData	m_tOsdImag;

	QPixmap *m_tempPix;

	Dlg_MessageBox*m_messageBox;
};
