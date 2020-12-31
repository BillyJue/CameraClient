#pragma once

#include <QWidget>
#include "ui_Dlg_HardWare.h"
#include <windows.h>
#include <subject.h>
#include "ivhs_struct.h"
#include "define.h"
#include <QLabel>
#define GBUTF8(da) QString::fromLocal8Bit(da)
#include <QThread>
#include <QCheckBox>
#include "MyWidget.h"
#include "Dlg_MessageBox.h"
class WorkerThread : public QThread
{
	Q_OBJECT
public:
		void run();

		//void setMyWidget(Dlg_HardWare * dlg){ m_hardware = dlg; }

		void setDlg(Dlg_MessageBox* dlg){ m_dlg = dlg; }

		void setMap(QMap<int, QCheckBox*> &tempMap);

		bool LEDTest(int port, int status);

		bool FlashTest(int port, int status);

		bool PLTest(int status);

		bool SerialTest(int port);

		bool InputTest(int run);

		bool OutputTest(int port, int status);

		int ReadData(HANDLE handler, char* buffer, int len);

		int WriteData(HANDLE handler, char* buffer, int len);

		HANDLE InitCom(const char* comName, int baudRate);

signals:
		void signal_dlgClose();
private:
	QMap<int, QCheckBox*> m_chkMap;
	Dlg_MessageBox *m_dlg;

};

class Dlg_HardWare : public MyWidget, public CObserver
{
	Q_OBJECT
		
public:
	Dlg_HardWare(QWidget *parent = Q_NULLPTR);

	~Dlg_HardWare();

	void run();

	void InitData(bool is /* = true */);

	QString GetCurSerialData(){ return ui.cmb_localSerial->currentText(); }

	QString GetCurBaudRateData(){ return ui.cmb_baudRate->currentText(); }

	void Init();  

	void ControlPlayStauts(bool is); //¿ØÖÆ²¥·Å×´Ì¬

	void OnObserverNotify(LPARAM lHint, LPVOID pHint);

public slots:

	void Slot_HardTestClicked();

	void Slot_SelectAllClicked();

	void Slot_DlgClose();

	void Slot_SerialDataChanged();

signals:
	void signal_serialDataChanged();

private:
	Ui::Dlg_HardWare ui;
	WorkerThread *m_thread;
	Dlg_MessageBox *m_tempDlg;
	//friend class WorkerThread;
};
