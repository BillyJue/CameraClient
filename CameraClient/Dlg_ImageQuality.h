#pragma once

#include <QWidget>
#include "ui_Dlg_ImageQuality.h"
#include <subject.h>
#include "ivhs_struct.h"
#include "define.h"
#define GBUTF8(da) QString::fromLocal8Bit(da)
#include <QMap>
#include <QSlider>
#include <QLineEdit>
#include "MyWidget.h"
#include "Dlg_MessageBox.h"
class Dlg_ImageQuality : public MyWidget, public CObserver
{
	Q_OBJECT

public:
	Dlg_ImageQuality(QWidget *parent = Q_NULLPTR);

	~Dlg_ImageQuality();

	void InitData(bool is);

	void OnObserverNotify(LPARAM lHint, LPVOID pHint);

	bool GetParam();

	void SetParam();

	void OnSetTxtVisible();

	Uint32 OnShutterValueToClient(Uint32 shutter);

	Uint32 OnShutterValueToIpc(Uint32 index);

	void SetData();

	bool eventFilter(QObject *object, QEvent *event);

	int GetSpecifyBitValue(Uint32 dwSrc, int nIndex);
	
	u32 SetSpecifyBitValue(Uint32& dwSrcOut, Uint32 dwValue, int nIndex);

public slots:
	void Slot_ImageQuatityChanged();

	void Slot_SetSliderChanged(const QString &sText);

	void Slot_TabIndexChanged();

	void Slot_SetExposureChanged(const QString &);

	void Slot_SetCmbExposureIndexChanged(int nIndex);

	void Slot_SetWhiteBalanceChanged(const QString &);

	void Slot_SetCmbWhiteBalanceIndexChanged(int nIndex);

	void Slot_SetSnapInputChanged(const QString & sValue);

	void Slot_SetCmbSnapIndexChanged(int nIndex);

	void Slot_Snap_SetChkChanged(int nIndex);

	void Slot_SetCmbLEDIndexChanged(int nIndex);

	void Slot_SetLEDInputChanged(const QString &);

	void Slot_LedRadioChanged(bool is);

	void Slot_BtnSaveClicked();

	void Slot_SetEditData(int nValue);
signals:
	void signal_imageQuatityChanged();
private:
	Ui::Dlg_ImageQuality ui;
	TIPC_KdISPCfg		        m_tCfg;
	Uint32                      m_nManuShutter;
	Uint32                      m_nMaxShutter;
	//Í¼Ïñµ÷½Ú
	QMap<QSlider*, QLineEdit*>	m_mapTemp1;
	QMap<QLineEdit*, QSlider*>	m_mapTemp2;
	Dlg_MessageBox				*m_messageBox;
	
};
