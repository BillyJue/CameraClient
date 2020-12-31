#pragma once

#include <QWidget>
#include "ui_Dlg_SnapManager.h"
#include <subject.h>
#include "ivhs_struct.h"
#include <QMap>
#include <QProgressBar>
#include <QDir>
#include "Dlg_MessageBox.h"
#include "MyWidget.h"
#define GBUTF8(da) QString::fromLocal8Bit(da)
const int PAGE_NUM = 30;
class Dlg_SnapManager : public MyWidget, public CObserver
{
	Q_OBJECT

public:
	Dlg_SnapManager(QWidget *parent = Q_NULLPTR);

	~Dlg_SnapManager();

	void InitData(bool is /* = true */);

	void Init();

	QWidget* CreateLabWidget();

	void OnObserverNotify(LPARAM lHint, LPVOID pHint);

	void SearchPicData();

	void FirstPage();

	void GoToPage();

	void LastPage();

	void PrePage();

	void NextPage();

	void AddTreeWidgetItem();

	BOOL ExportRecord(TIPC_PicFileInfo t_PicFileInfo);

	//void OnExportSelected(void *p, QVector<int> vec);
	static void OnExportAll(void* p);

	BOOL ExportImg(TIPC_PicFileInfo t_PicFileInfo, QString sPath);

	BOOL ExportImg(TIPC_PicFileInfo t_PicFileInfo, wchar_t* path, int* count);

	BOOL ExportVideo(TIPC_PicFileInfo t_PicFileInfo);

	BOOL GetPic(TIPC_PicQuery *tIPC_PicQuery);

	//unsigned int OnExportAll(void* p);

	bool eventFilter(QObject *watched, QEvent *event);
	//flag表示是否下载的为 图片还是视频;
	void GetFileName(TIPC_PicFileInfo t_PicFileInfo, wchar_t* remotefile, wchar_t* localfile, int i, bool flag);

	QTreeWidget *GetTreeWidget(){ return ui.treeWidget; }

	QCheckBox *GetOriginPicCheck(){ return ui.chk_originPic; }

	QCheckBox *GetIllegalMergePicCheck(){ return ui.chk_illegalMergePic; }

	QCheckBox *GetVideoCheck(){ return ui.chk_video; }

	QCheckBox *GetSearchResultCheck(){ return ui.chk__findResult; }

	TIPC_PicQueryResult GetPicQueryResult(){ return m_tIPC_PicQueryResult; }

	QProgressBar * GetProgressBar(){ return ui.progressBar; }

	TIPC_PicQueryResult* GetIPC_GetPicResult(){ return &m_tIPC_GetPicResult; }

	int GetSumPage(){ return m_nSumPage; }

	QString GetDirPath(){ return m_dirPath; }

	TIPC_PicQuery* GetPicQuery(){ return &m_tIPC_GetPicQuery; }
public slots:
	void Slot_SetEnable(int nEnable);

	void Slot_FirstPageClicked();

	void Slot_BtnSearchClicked();

	void Slot_SearchChanged();

	void Slot_TreeWidgetDoubleClicked(QTreeWidgetItem *pItem, int nColumn);
	
	void Slot_BtnExportClicked();

	void Slot_BtnExportAllClicked();
signals:
	void signal_searchDataChanged();

	void signal_progressBarChanged(int);
protected:
	TIPC_PicQuery		m_tIPC_GetPicQuery;
	TIPC_PicQueryResult	m_tIPC_GetPicResult;

	TIPC_PicQuery       m_tIPC_PicQuery;       //查询条件
	TIPC_PicFileInfo    m_tIPC_PicFileInfo;	   //查询返回结构体
	TIPC_PicQueryResult m_tIPC_PicQueryResult; //记录结构体;
	QString				m_dirPath;
private:
	Ui::Dlg_SnapManager ui;
	
	int m_nQueryStartNum;//页数乘个数
	int m_nSumPage; //查询个数
	int m_nCurPage; //当前页
	int m_nRecordIndex;
	Dlg_MessageBox *m_dlgMessageBox;
};
