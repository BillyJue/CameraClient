#pragma once

#include <QWidget>
#include "ui_Dlg_PicMerge.h"
#include <subject.h>
#include "ivhs_struct.h"
#include "define.h"
#include "MyWidget.h"
#include "Dlg_MessageBox.h"
#include <QMap>
#define GBUTF8(da) QString::fromLocal8Bit(da)
class Dlg_PicMerge : public MyWidget, public CObserver
{
	Q_OBJECT

public:
	Dlg_PicMerge(QWidget *parent = Q_NULLPTR);

	~Dlg_PicMerge();

	void OnSetTxtVisible();

	void InitData(bool is);

	bool GetParam();

	bool SetParam();

	void SetDrawVisible(Dlg_MergeDraw *pLabelLayout, Uint8 type);

	void SetMergeData(int nIndex, Uint8 type, Dlg_MergeDraw * pLab);

	void SetZeroColor(Dlg_MergeDraw * lab, int * pArr, int len);

	void SetZeroArr(QMap<int, QString> &tempMap, int nLen, bool isHasZero);

	void OnObserverNotify(LPARAM lHint, LPVOID pHint);

	void UpdateData(bool bUpdate /*= true*/);
public slots:
	void Slot_MergePictureChanged();

	void Slot_CmbIndexChanged(int nIndex);

	void Slot_BtnSaveClicked();
signals:
	void signal_mergePicChanged();
private:
	Ui::Dlg_PicMerge ui;
	Dlg_MessageBox  *m_messageBox;
	TIPC_TImgMergePara m_tMerge;
	int m_n2to1[2];
	int m_n3to1[4];//3
	int m_n4to1[4];
	int m_n6to1[6];
};
