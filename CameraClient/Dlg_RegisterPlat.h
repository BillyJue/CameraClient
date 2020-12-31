#pragma once

#include <QWidget>
#include "ui_Dlg_RegisterPlat.h"
#include "Dlg_PlatComponent.h"
#include "Dlg_PlatLabComponent.h"
#include <QVector>
#include <subject.h>
#include "ivhs_struct.h"
#include "MyWidget.h"
#include <QMap>
#include "Dlg_MessageBox.h"
#define GBUTF8(da) QString::fromLocal8Bit(da)
class Dlg_RegisterPlat : public MyWidget, public CObserver
{
	Q_OBJECT

public:
	Dlg_RegisterPlat(QWidget *parent = Q_NULLPTR);

	~Dlg_RegisterPlat();

	void OnSetTxtVisible();

	void InitData(bool is /* = true */);

	void GetParam();

	void ReLayout(int nIndex);

	//����չ��������, isFromPage���Ϊfalse ��ֵ��ֵ��ҳ�棬����ҳ�浽ֵ
	void SetPlatData(int i, bool isFromPage = false);

	void OnObserverNotify(LPARAM lHint, LPVOID pHint);
public slots:

	void Slot_BtnClicked();

	void Slot_BtnHidenClicked();

	void Slot_DataChanged();

	void Slot_BtnSaveClicked();

	void Slot_BtnFreshClicked();

signals:
	void data_changed();
private:
	Dlg_MessageBox *m_messageBox;
	Ui::Dlg_RegisterPlat ui;
	Dlg_PlatComponent *m_platComponent;
	QMap<int, Dlg_PlatLabComponent*> m_LabMap;
	int m_nIndex; //���� ���m_nExpandIndex��m_nIndex���������չ��
	int m_nExpandIndex; //��չ��Index
	TITS_SetServerReq m_tData;

};
