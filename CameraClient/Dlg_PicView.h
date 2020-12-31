#pragma once

#include <QWidget>
#include "ui_Dlg_PicView.h"
#include <subject.h>
#include "ivhs_struct.h"
#include "define.h"
#include <QSignalMapper>
#include "MyWidget.h"
#define GBUTF8(da) QString::fromLocal8Bit(da)
#define  PAGE_NUM 20
class Dlg_PicView : public MyWidget, public CObserver
{
	Q_OBJECT

public:
	Dlg_PicView(QWidget *parent = Q_NULLPTR);

	~Dlg_PicView();

	virtual void ReceveVideoStream(bool is = true);

	virtual void InitData(bool is);

	QWidget* CreateButtonWidget(int nIndex);

	QWidget* CreateLabWidget();

	void OpenShowPic(int index, u32 dwId);

	void OnShowOriginalPic(bool flg);

	bool GetParamScenceInfo();

	void Init();

	void SetPlayVideo(bool flag);

	void ControlPlayStauts(bool is);

	void FirstPage();

	void NextPage();

	void GoToPage();

	void LastPage();

	void PrePage();

	void OnObserverNotify(LPARAM lHint, LPVOID pHint);

	void ProcessEventMsg();

	void OnFreshPage();

	void SetRecVideo(bool flag);

	void PicShowUpdate(){ ui.lab_picShow->update(); }

	LRESULT OnRefreshPic(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	//void SetNewPic(u32 dwId, int nType, bool bMjpeg = false);

	void ListSetShowPage(u32 page);

	void UpdateEvtInfo();

	void  UpdateEvtInfo(PTEVTVEHICLEINFO pData);

	void ListAddItem(u32 dwId);

	void ListAddItemText(u32 dwId);

	int GetSumPage();

	void OnDebugMode(bool flg);

	bool eventFilter(QObject *obj, QEvent *event);

public slots:
	void Slot_DataChanged();

	void Slot_FreshPic(unsigned int nMsgType, unsigned long parm);

	void Slot_StopVideo();

	void Slot_StartVideo();

	void Slot_StartRecv();

	void Slot_StopRecv();

	void Slot_DebugParam();

	void Slot_Snap();

	void Slot_CheckDebugParam(int nStauts);

	void Slot_CheckSavePic(int nStatus);

	void Slot_OriginPictureShow(int nstauts);

	void Slot_OPenPicutureShow();

	void Slot_TreeWidgetItemClicked(QTreeWidgetItem *pItem, int nColumn);

signals:
	void signal_dataChanged();

	void signal_freshPic(unsigned int, unsigned long);
private:
	Ui::Dlg_PicView ui;
	TIPC_GetSceneCfgRsp			m_tSeceneCfg;
	TMAPEVTINFO	                m_mapEvtInfo;
	int							m_nCurPage;
	int							m_nSumPage;
	int							m_nItemMax;
	bool						m_bDebug;
	u32							m_dwId;
	bool						m_bPalyVideo;
	bool						m_bRecordVideo;
	QSignalMapper				*m_signalMapper;
};
