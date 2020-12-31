#pragma once

#include <QWidget>
#include <QDialog>
#include "ui_Dlg_SceneDraw.h"
#include <subject.h>
#include "ivhs_struct.h"
#include "define.h"
#define GBUTF8(da) QString::fromLocal8Bit(da)
class Dlg_SceneDraw : public QDialog, public CObserver
{
	Q_OBJECT

public:
	Dlg_SceneDraw(QWidget *parent = Q_NULLPTR);

	~Dlg_SceneDraw();

	void SetIsParkScene(bool is);

	void SetScenceIndex(int nIndex);

	void ClearListWidgetItem();

	void OnSnap();

	void SetData(TIPC_SceneConfig& tData, int w, int h);

	void SetData(TIPC_illgParkPara& tData, int w, int h);

	void GetData(TIPC_SceneConfig& tData);

	void GetData(TIPC_illgParkPara& tData);

	void OnObserverNotify(LPARAM lHint, LPVOID pHint);

public slots:
	void Slot_AttrShowDialog();

	void Slot_BtnLoadSceneClicked();

	void Slot_BtnOKClicked();

	void Slot_BtnCancelClicked();

	void Slot_BtnAddTrapeizumClicked();

	void Slot_BtnAddRuleLineClicked();

	void Slot_BtnRepaintAllClicked();

	void Slot_BtnRepaintClicked();

	void Slot_BtnDeleteClicked();

	void Slot_ListWidgetItemClicked(QListWidgetItem *pItem);

	void Slot_BtnAddPolygonClicked();

	void Slot_BtnSavePicClicked();

private:
	Ui::Dlg_SceneDraw ui;
	int						m_nSelType;
	bool					m_bEnable;//如果不删除按钮只删除数据为true
	int						m_scenceIndex;
	int						m_picWidth;
	int						m_picHeight;
	TIPC_SceneConfig        m_tSenceData;
	TIPC_illgParkPara       m_tIllgParkPara;
	bool					m_bScenePark;
};
