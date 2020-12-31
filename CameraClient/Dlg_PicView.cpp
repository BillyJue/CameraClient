#include "Dlg_PicView.h"
#include "MgrData.h"
#include <QMutex>
#include <QPixmap>
#include <QPixmapCache>
#include <QIntValidator>
#include "Dlg_DebugParam.h"
#include <shellapi.h>
void __stdcall VideoDrawCB2(PLAYERHANDLE hPlayHandle, unsigned long dwPaintDC, unsigned long dwFrmID, unsigned long dwParam)
{
	Dlg_PicView *pDlg = (Dlg_PicView *)dwParam;
	HDC hdc = (HDC)dwPaintDC;

}

Dlg_PicView::Dlg_PicView(QWidget *parent)
	: MyWidget(parent),m_nCurPage(1),m_nSumPage(1)
	, m_bDebug(false), m_dwId(0), m_bPalyVideo(false)
{
	m_nItemMax = PAGE_NUM;
	ui.setupUi(this);
	connect(this, SIGNAL(signal_dataChanged()), this, SLOT(Slot_DataChanged()));
	connect(this, SIGNAL(signal_freshPic(unsigned int, unsigned long)), this, SLOT(Slot_FreshPic(unsigned int, unsigned long)));
	connect(ui.chk_debug, SIGNAL(stateChanged(int)), this, SLOT(Slot_CheckDebugParam(int)));
	connect(ui.chk_savePic, SIGNAL(stateChanged(int)), this, SLOT(Slot_CheckSavePic(int)));
	connect(ui.chk_originShow, SIGNAL(stateChanged(int)), this, SLOT(Slot_OriginPictureShow(int)));
	connect(ui.treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(Slot_TreeWidgetItemClicked(QTreeWidgetItem*, int)));
	connect(ui.btn_stop, SIGNAL(clicked()), this, SLOT(Slot_StopVideo()));
	connect(ui.btn_startVideo, SIGNAL(clicked()), this, SLOT(Slot_StartVideo()));
	connect(ui.btn_startRecv, SIGNAL(clicked()), this, SLOT(Slot_StartRecv()));
	connect(ui.btn_stopRecv, SIGNAL(clicked()), this, SLOT(Slot_StopRecv()));
	connect(ui.btn_snap, SIGNAL(clicked()), this, SLOT(Slot_Snap()));
	connect(ui.btn_debugParam, SIGNAL(clicked()), this, SLOT(Slot_DebugParam()));

	MgrData::getInstance()->GetMgrSubject()->Add(this);

	if (MgrData::getInstance()->GetMgrEvtLst())
	{
		MgrData::getInstance()->GetMgrEvtLst()->AddSender(this);
	}
	
	//监听
	ui.label_11->installEventFilter(this);
	ui.label_13->installEventFilter(this);
	ui.label_15->installEventFilter(this);
	ui.label_17->installEventFilter(this);
	ui.label_18->installEventFilter(this);
	ui.ledt_go->setValidator(new QIntValidator(0, 1000, this));
	QPixmapCache::setCacheLimit(1);

	////设置表头
	//QHeaderView *view = ui.treeWidget->header();
	//view->setStyleSheet("QHeaderView{font-size: 14px;} \
	//					QHeaderView::section{ \
	//					color:white; padding: 4px; \
	//					height: 24px; border: 0px solid #567dbc; \
	//					border - left: 0px; border - right: 0px; background: #ababff}");

	ui.treeWidget->setColumnWidth(1, 120);
	ui.treeWidget->setColumnWidth(7, 150);
	ui.treeWidget->setColumnWidth(8, 100);
}

void Dlg_PicView::ReceveVideoStream(bool is)
{
	if (is)
	{
		int dwId = 0;
		BOOL bHandle = true;
		ui.lab_picShow->SetNewPic2(dwId, 0, TRUE);
	}
	
}

void Dlg_PicView::InitData(bool is)
{
	if (!is)
	{
		ControlPlayStauts(is);
		return;
	}
	GetParamScenceInfo();
	MgrItsCtrl* pMgrItsCtrl = MgrData::getInstance()->GetMgrItsCtrl();
	if (pMgrItsCtrl != NULL)
	{
		pMgrItsCtrl->StartReceive();
		ui.btn_startRecv->setEnabled(false);
		ui.btn_stopRecv->setEnabled(true);
	}
	//emit ui.btn_startRecv->clicked();

	Init();
	{
		TLocalCfgInfo m_tLocalCfgInfo;
		MgrData::getInstance()->GetMgrLocalCfg()->GetLocalCfg(&m_tLocalCfgInfo);

		if (m_tLocalCfgInfo.ispic)
		{

		}
		else
		{
			ControlPlayStauts(is);
		}
	}

}

void Dlg_PicView::Slot_TreeWidgetItemClicked(QTreeWidgetItem *pItem, int nColumn)
{
	int dwId = pItem->text(0).toInt();
	PTEVTVEHICLEINFO pInfo = MgrData::getInstance()->GetMgrEvtLst()->GetItem(dwId);
	if (pInfo == NULL) return;
	UpdateEvtInfo(pInfo);

	// 显示图片
	BOOL bHandled;
	OnRefreshPic(0, dwId, 0, bHandled);

}

void Dlg_PicView::Slot_OPenPicutureShow()
{
	QPushButton *pBtn = dynamic_cast<QPushButton*>(this->sender());
	int nIndex = -1;
	nIndex = pBtn->property("nIndex").toInt();
	int dwId = 0;
	OpenShowPic(nIndex, dwId);

}

void Dlg_PicView::OpenShowPic(int index, u32 dwId)
{
	PTEVTVEHICLEINFO pInfo = MgrData::getInstance()->GetMgrEvtLst()->GetItem(index);
	if (pInfo == NULL) return;

	int nShow = 0;
	for (int i = 0; i < (int)pInfo->tVechileData.dwImageNum; i++)
	{
		if (pInfo->tVechileData.atImage[i].dwType == VEH_IMAGE_TYPE_MONTAGE)
		{
			nShow = i;
			break;
		}
	}

	wchar_t strParm[MAX_PATH];
	wsprintf(strParm, TEXT("%s%s"), pInfo->szDir, pInfo->szImageName[nShow]);

	wchar_t strSystemDir[MAX_PATH];
	::GetSystemDirectoryW(strSystemDir, MAX_PATH);
	wchar_t strRundll[MAX_PATH];
	wsprintf(strRundll, TEXT("%s%s"), strSystemDir, TEXT("\\rundll32.exe"));

	wchar_t strParm2[MAX_PATH];
	wsprintf(strParm2, TEXT("shimgvw.dll imageview_fullscreen %s"), strParm);

	::ShellExecuteW(NULL, TEXT("Open"), strRundll, strParm2, NULL, SW_SHOW);
}


void Dlg_PicView::OnShowOriginalPic(bool flg)
{
	if (flg)
	{
		if (m_tSeceneCfg.tMaxParam.dwMaxWidth != 0 && m_tSeceneCfg.tMaxParam.dwMaxHeight != 0)
		{
			//ui.lab_picShow->setFixedWidth(m_tSeceneCfg.tMaxParam.dwMaxWidth);
			//ui.lab_picShow->setFixedHeight(m_tSeceneCfg.tMaxParam.dwMaxHeight);
			ui.lab_picShow->setFixedSize(m_tSeceneCfg.tMaxParam.dwMaxWidth, m_tSeceneCfg.tMaxParam.dwMaxHeight);
			ui.scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
			ui.scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
			
		}
	}
	else
	{

		QSize sz = ui.scrollArea->size();
		ui.lab_picShow->setFixedSize(sz);
		ui.lab_picShow->update();
		ui.scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		ui.scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		//ui.lab_picShow->setFixedWidth(m_tSeceneCfg.tMaxParam.dwMaxWidth);
		//ui.lab_picShow->setFixedHeight(m_tSeceneCfg.tMaxParam.dwMaxHeight);
	}
}

void Dlg_PicView::Slot_OriginPictureShow(int nstauts)
{
	bool bSelected = ui.chk_originShow->isChecked();
	OnShowOriginalPic(bSelected);
}

void Dlg_PicView::Slot_CheckDebugParam(int nStauts)
{
	bool bStart = ui.chk_debug->isChecked();
	OnDebugMode(bStart);
}

void Dlg_PicView::Slot_CheckSavePic(int nStatus)
{
	bool bStart = ui.chk_savePic->isChecked();
	MgrData::getInstance()->GetMgrMjpegCtrl()->SetMJpegSave(bStart);
}

void Dlg_PicView::SetPlayVideo(bool flag)
{
	if (flag)
	{
		m_bPalyVideo = true;
		MgrData::getInstance()->GetMgrVideo()->PlayVideo();
		ui.btn_stop->setText(GBUTF8("停止播放"));
		ui.btn_startVideo->setEnabled(true);
		
	}
	else
	{
		m_bPalyVideo = false;
		MgrData::getInstance()->GetMgrVideo()->StopPlayVideo();
		ui.btn_stop->setText(GBUTF8("开始播放"));
		ui.btn_startVideo->setEnabled(false);
		ui.btn_startVideo->setText(GBUTF8("开始录像"));
	}
}

void Dlg_PicView::Slot_Snap()
{
	u32 dIP = MgrData::getInstance()->GetIP();
	ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dIP);
	if (pCtrl != NULL)
	{
		pCtrl->IpcEvt_Snap(1);
	}
}

void Dlg_PicView::Slot_DebugParam()
{
	Dlg_DebugParam dlg;
	dlg.exec();
}



void Dlg_PicView::Slot_StartRecv()
{
	MgrItsCtrl* pMgrItsCtrl = MgrData::getInstance()->GetMgrItsCtrl();
	if (pMgrItsCtrl != NULL)
	{
		pMgrItsCtrl->StartReceive();
		ui.btn_startRecv->setEnabled(false);
		ui.btn_stopRecv->setEnabled(true);
	}
}
void Dlg_PicView::Slot_StopRecv()
{
	MgrItsCtrl* pMgrItsCtrl = MgrData::getInstance()->GetMgrItsCtrl();
	if (pMgrItsCtrl != NULL)
	{
		pMgrItsCtrl->StopReceive();
		ui.btn_startRecv->setEnabled(true);
		ui.btn_stopRecv->setEnabled(false);
	}
}

void Dlg_PicView::Slot_StopVideo()
{
	m_bPalyVideo = !m_bPalyVideo;
	SetPlayVideo(m_bPalyVideo);
}

void Dlg_PicView::SetRecVideo(bool flag)
{
	if (flag)
	{
		m_bRecordVideo = TRUE;
		char filepath[MAX_PATH_LEN];
		wchar2char(filepath, MgrData::getInstance()->GetMgrLocalCfg()->GetVideoPath());
		MgrData::getInstance()->GetMgrEvtLst()->MakeDir(MgrData::getInstance()->GetMgrLocalCfg()->GetVideoPath());
		MgrData::getInstance()->GetMgrVideo()->StartRecVideo(filepath);
		ui.btn_startVideo->setText(GBUTF8("停止录像"));
		//m_pRecordState->SetText(pInf2);
	}
	else
	{
		m_bRecordVideo = FALSE;

		ui.btn_startVideo->setText(GBUTF8("开始录像"));
		MgrData::getInstance()->GetMgrVideo()->StopRecVideo();
	}

}

void Dlg_PicView::Slot_StartVideo()
{
	if (m_bRecordVideo)
	{
		SetRecVideo(false);
	}
	else
	{
		SetRecVideo(true);
	}
}

bool Dlg_PicView::eventFilter(QObject *obj, QEvent *event)
{
	if ((obj == ui.label_11) && (event->type() == QEvent::MouseButtonRelease))
	{
		FirstPage();
		return true;
	}
	else if ((obj == ui.label_15) && (event->type() == QEvent::MouseButtonRelease))
	{
		NextPage();
		return true;
	}
	else if ((obj == ui.label_13) && (event->type() == QEvent::MouseButtonRelease))
	{
		PrePage();
		return true;
	}
	else if ((obj == ui.label_17) && (event->type() == QEvent::MouseButtonRelease))
	{
		LastPage();
		return true;
	}
	else if ((obj == ui.label_18) && (event->type() == QEvent::MouseButtonRelease))
	{
		GoToPage();
		return true;
	}
	return QWidget::eventFilter(obj, event);
}

void Dlg_PicView::Slot_FreshPic(unsigned int nMsgType, unsigned long parm)
{
	BOOL bHandled = TRUE;
	OnRefreshPic(nMsgType, parm, 1, bHandled);
}

Dlg_PicView::~Dlg_PicView()
{
	ReceveVideoStream(false);
	ControlPlayStauts(false);
	MgrData::getInstance()->GetMgrSubject()->Del(this);
	MgrData::getInstance()->GetMgrEvtLst()->DelSender(this);
	//SAFE_DELETE(m_signalMapper);
}

void Dlg_PicView::FirstPage()
{
	ListSetShowPage(1);
}

void Dlg_PicView::NextPage()
{
	int nSumPage = GetSumPage();

	int nCurPage = m_nCurPage;
	nCurPage++;

	if (nCurPage <= nSumPage)
	{
		ListSetShowPage(nCurPage);
	}
}

void Dlg_PicView::PrePage()
{
	int nCurPage = m_nCurPage;
	nCurPage--;

	if (nCurPage != 0)
	{
		ListSetShowPage(nCurPage);
	}
}

void Dlg_PicView::LastPage()
{
	int nSumPage = GetSumPage();

	ListSetShowPage(nSumPage);
}

void Dlg_PicView::GoToPage()
{
	int nCurPage = ui.ledt_go->text().toInt();
	int nSumPage = GetSumPage();
	if (nCurPage > nSumPage || nCurPage < 1)
		return;

	ListSetShowPage(nCurPage);
}

void Dlg_PicView::ControlPlayStauts(bool is)
{
	if (is)
	{
		//应该显示视频下面
		MgrData::getInstance()->GetMgrVideo()->PlayVideo((unsigned long)ui.label_16->winId());//ui.label_16
		MgrData::getInstance()->GetMgrVideo()->RegisterDrawCB(VideoDrawCB2, (unsigned long)this);
	}
	else
	{
		MgrData::getInstance()->GetMgrVideo()->RegisterDrawCB(NULL, NULL);
		MgrData::getInstance()->GetMgrVideo()->StopPlayVideo((unsigned long)ui.label_16->winId());
	}
}


void Dlg_PicView::Init()
{
	
	{
		m_bPalyVideo = true;
		ui.btn_stop->setText(GBUTF8("停止播放"));
		ui.btn_startVideo->setEnabled(true);
		ui.btn_startVideo->setText(GBUTF8("开始录像"));
		m_bRecordVideo = FALSE;
		if (ui.chk_debug->isChecked())
		{
			ui.chk_debug->setChecked(false);
			OnDebugMode(false);
		}
	}


	return ;
}

void Dlg_PicView::OnDebugMode(bool flg)
{
	if (flg)
	{
		 
		if (MgrData::getInstance()->GetMgrMjpegCtrl() != NULL)
		{
			MgrData::getInstance()->GetMgrMjpegCtrl()->AddHandle(this);
			MgrData::getInstance()->GetMgrMjpegCtrl()->StartRecvMjpeg((HWND)this->winId());
		}
	}
	else
	{
		if (MgrData::getInstance()->GetMgrMjpegCtrl() != NULL)
		{
			MgrData::getInstance()->GetMgrMjpegCtrl()->DelHandle(this);
			MgrData::getInstance()->GetMgrMjpegCtrl()->StopRecvMjpeg((HWND)this->winId());
		}
	}
	m_bDebug = flg;
}

int Dlg_PicView::GetSumPage()
{
	int nSumPage = 0;
	int nSumItem = 0;

	nSumItem = MgrData::getInstance()->GetMgrEvtLst()->GetItemCount();
	nSumPage = nSumItem / m_nItemMax;
	if (nSumItem == 0 || (nSumItem % m_nItemMax) > 0)
	{
		nSumPage++;
	}

	m_nSumPage = nSumPage;

	return nSumPage;
}

void Dlg_PicView::OnFreshPage()
{
	int nSumPage = GetSumPage();
	int nCurPage;
	nCurPage = m_nCurPage;
	ui.label_9->setText(QString("%1/%2").arg(nCurPage).arg(nSumPage));
}

void Dlg_PicView::ListSetShowPage(u32 page)
{
	if (m_nCurPage == page)
	{
		return;
	}

	m_nCurPage = page;

	for (int j = ui.treeWidget->topLevelItemCount(); j >= 0; j--)
	{
		delete ui.treeWidget->topLevelItem(j);
	}
    
	u32 dwStartId = (m_nCurPage - 1) * PAGE_NUM + 1;
	u32 dwEndId = m_nCurPage * PAGE_NUM;

	for (int i = dwStartId; i <= dwEndId; i++)
	{
		if (i <= MgrData::getInstance()->GetMgrEvtLst()->GetItemCount())
		{
			ListAddItemText(i);
		}
	}

	QTreeWidgetItem *pItem = ui.treeWidget->topLevelItem(0);
	ui.treeWidget->itemClicked(pItem, 0);

	OnFreshPage();
}

void Dlg_PicView::ListAddItem(u32 dwId)
{
	PTEVTVEHICLEINFO pData = MgrData::getInstance()->GetMgrEvtLst()->GetItem(dwId);
	if (pData == NULL)
	{
		return;
	}

	if (m_nCurPage != m_nSumPage)
	{
		OnFreshPage();
		return;
	}

	int sumItem = ui.treeWidget->topLevelItemCount();
	if (sumItem == PAGE_NUM)
	{
		m_nCurPage++;
		for (int j = ui.treeWidget->topLevelItemCount(); j >= 0; j--)
		{
			delete ui.treeWidget->topLevelItem(j);
		}
		OnFreshPage();
	}

	ListAddItemText(dwId);
	QTreeWidgetItem *pItem = ui.treeWidget->topLevelItem(0);
	ui.treeWidget->itemClicked(pItem, 0);
	//m_pEvtList->SelectItem(0);
}

QWidget* Dlg_PicView::CreateLabWidget()
{
	QWidget *pWidget = new QWidget;
	//pWidget->setFixedSize(100, 30);
	QLabel *lab = new QLabel;
	lab->setFixedHeight(25);
	lab->setAlignment(Qt::AlignCenter);
	//btn->setStyleSheet("color:rgb(255,255,0);background-color:blue;border-radius:2px");
	QVBoxLayout *lay = new QVBoxLayout;
	lay->setContentsMargins(0, 0, 0, 0);
	QSpacerItem *itemer1 = new QSpacerItem(10, 1, QSizePolicy::Maximum, QSizePolicy::Expanding);
	QSpacerItem *itemer2 = new QSpacerItem(10, 1, QSizePolicy::Maximum, QSizePolicy::Expanding);
	lay->addItem(itemer1);
	lay->addWidget(lab);
	lay->addItem(itemer2);
	pWidget->setLayout(lay);
	pWidget->setContentsMargins(0, 0, 0, 0);
	pWidget->setProperty("lab", QVariant::fromValue(lab));
	return pWidget;
}

QWidget* Dlg_PicView::CreateButtonWidget(int nIndex)
{
	QWidget *pWidget = new QWidget;
	//pWidget->setFixedSize(100, 30);
	QPushButton *btn = new QPushButton;
	btn->setProperty("nIndex", nIndex);
	btn->setText(" ...");
	btn->setFixedSize(80, 25);
	//btn->setStyleSheet("color:rgb(255,255,0);background-color:blue;border-radius:2px");
	QVBoxLayout *lay = new QVBoxLayout;
	lay->setContentsMargins(0, 0, 0, 0);
	QSpacerItem *itemer1 = new QSpacerItem(10, 1, QSizePolicy::Maximum, QSizePolicy::Expanding);
	QSpacerItem *itemer2 = new QSpacerItem(10, 1, QSizePolicy::Maximum, QSizePolicy::Expanding);
	lay->addItem(itemer1);
	lay->addWidget(btn);
	lay->addItem(itemer2);
	pWidget->setLayout(lay);
	pWidget->setContentsMargins(0, 0, 0, 0);
	pWidget->setProperty("btn", QVariant::fromValue(btn));
	return pWidget;
}


void Dlg_PicView::ListAddItemText(u32 dwId)
{
	PTEVTVEHICLEINFO pData = MgrData::getInstance()->GetMgrEvtLst()->GetItem(dwId);
	if (pData == NULL)
	{
		return;
	}


	QTreeWidgetItem *pItem = new QTreeWidgetItem;
	ui.treeWidget->insertTopLevelItem(0, pItem);
	for (int i = 0; i < 9; i++)
	{
		pItem->setTextAlignment(i, Qt::AlignVCenter | Qt::AlignHCenter);
	}
	
	qulonglong uDwId = dwId;
	pItem->setData(0, Qt::UserRole + 1, QVariant(uDwId));
	pItem->setText(0, QString("%1").arg(dwId));
	
	QString sPlateNum = QString::fromStdWString(Vechile::GetPlateNum(&(pData->tVechileData)));
	QWidget *pWid = CreateLabWidget();
	ui.treeWidget->setItemWidget(pItem, 1, pWid);
	QLabel* pLab = pWid->property("lab").value<QLabel*>();
	pLab->setText(sPlateNum);
	QString sBkColor = Vechile::GetPlateColor2(&(pData->tVechileData));	
	QString sTxtColor = Vechile::GetPlateTextColor(&(pData->tVechileData));
	QString sSheet = QString("background-color:%1; color:%2;").arg(sBkColor).arg(sTxtColor); //
	pLab->setStyleSheet(sSheet);
	
	pItem->setText(2, QString("%1").arg(pData->tVechileData.tInfo.dwLaneNum));
	pItem->setText(3, QString::fromStdWString(Vechile::GetVehSpeed(&(pData->tVechileData))));

	DWORD vColor = Vechile::GetVehiclecColor2(&(pData->tVechileData));
	if (vColor != 0x00000000)
	{
		pItem->setBackgroundColor(4, vColor);
		pItem->setText(4, "");
		pItem->setTextColor(4, Qt::white);
	}
	pItem->setText(5, QString::fromStdWString(Vechile::GetVehiclecLogo(&(pData->tVechileData))));
	pItem->setText(6, QString::fromStdWString(Vechile::GetIllegalType(&(pData->tVechileData))));
	pItem->setText(7, QString::fromStdWString(Vechile::GetThroughTime(&(pData->tVechileData))));

	QWidget *pWidget = CreateButtonWidget(dwId);
	QPushButton *btn = pWidget->property("btn").value<QPushButton*>();
	btn->setProperty("nIndex", uDwId);
	connect(btn, SIGNAL(clicked()), this, SLOT(Slot_OPenPicutureShow()));
	ui.treeWidget->setItemWidget(pItem, 8, pWidget);

}



void Dlg_PicView::Slot_DataChanged()
{
	ProcessEventMsg();
}

void  Dlg_PicView::UpdateEvtInfo(PTEVTVEHICLEINFO pData)
{
	if (pData == NULL)
	{
		ui.lab_lane->setText("");
		ui.lab_carNumber->setText("");
		ui.lab_color->setText("");
		ui.lab_carLogo->setText("");
		ui.lab_carType->setText("");
		ui.lab_eventType->setText("");
		ui.lab_time->setText("");
		return;
	}
	QString sLane = QString::fromStdWString(Vechile::GetLaneNum(&(pData->tVechileData)));
	ui.lab_lane->setText(sLane);

	QString sTextcolor = Vechile::GetPlateTextColor(&(pData->tVechileData));
	QString sBkColor = Vechile::GetPlateColor2(&(pData->tVechileData));
	if (sBkColor == "0")
	{
		sBkColor = "white";
	}
	QString sNumber = QString::fromStdWString(Vechile::GetPlateNum(&(pData->tVechileData)));
	ui.lab_carNumber->setText(sNumber);
	ui.lab_carNumber->setStyleSheet(QString("QLabel{color:%1; background-color:%2}").arg(sTextcolor).arg(sBkColor));

	DWORD  vBkColor = Vechile::GetVehiclecColor2(&(pData->tVechileData));
	if (vBkColor == 0x00000000)
	{
		vBkColor = 0xFFFFFFFF;
		ui.lab_color->setText(GBUTF8("未知"));
	}
	else
	{
		ui.lab_color->setText("");
	}

	ui.lab_color->setStyleSheet(QString("background-color:%1").arg(vBkColor));

	QString sLogo = QString::fromStdWString(Vechile::GetVehiclecLogo(&(pData->tVechileData)));
	ui.lab_carLogo->setText(sLogo);

	QString sCarType = QString::fromStdWString(Vechile::GetVehiclecType(&(pData->tVechileData)));
	ui.lab_carType->setText(sCarType);

	QString sIllegalType = QString::fromStdWString(Vechile::GetIllegalType(&(pData->tVechileData)));
	ui.lab_eventType->setText(sIllegalType);

	QString sTime = QString::fromStdWString(Vechile::GetThroughTime(&(pData->tVechileData)));
	ui.lab_time->setText(sTime);
}


void  Dlg_PicView::UpdateEvtInfo()
{
	PTEVTVEHICLEINFO pData = MgrData::getInstance()->GetMgrEvtLst()->GetItem(m_dwId);
	if (pData == NULL)
	{
		ui.lab_lane->setText("");
		ui.lab_carNumber->setText("");
		ui.lab_color->setText("");
		ui.lab_carLogo->setText("");
		ui.lab_carType->setText("");
		ui.lab_eventType->setText("");
		ui.lab_time->setText("");
		return;
	}
	QString sLane = QString::fromStdWString(Vechile::GetLaneNum(&(pData->tVechileData)));
	ui.lab_lane->setText(sLane);

	QString sTextcolor = Vechile::GetPlateTextColor(&(pData->tVechileData));
	QString sBkColor = Vechile::GetPlateColor2(&(pData->tVechileData));
	if (sBkColor == "0")
	{
		sBkColor = "white";
	}
	QString sNumber = QString::fromStdWString(Vechile::GetPlateNum(&(pData->tVechileData)));
	ui.lab_carNumber->setText(sNumber);
	ui.lab_carNumber->setStyleSheet(QString("QLabel{color:%1; background-color:%2}").arg(sTextcolor).arg(sBkColor));

	DWORD  vBkColor = Vechile::GetVehiclecColor2(&(pData->tVechileData));
	if (vBkColor == 0x00000000)
	{
		vBkColor = 0xFFFFFFFF;
		ui.lab_color->setText(GBUTF8("未知"));
	}
	else
	{
		ui.lab_color->setText("");
	}
	
	ui.lab_color->setStyleSheet(QString("background-color:%1").arg(vBkColor));

	QString sLogo = QString::fromStdWString(Vechile::GetVehiclecLogo(&(pData->tVechileData)));
	ui.lab_carLogo->setText(sLogo);
	
	QString sCarType = QString::fromStdWString(Vechile::GetVehiclecType(&(pData->tVechileData)));
	ui.lab_carType->setText(sCarType);

	QString sIllegalType = QString::fromStdWString(Vechile::GetIllegalType(&(pData->tVechileData)));
	ui.lab_eventType->setText(sIllegalType);

	QString sTime = QString::fromStdWString(Vechile::GetThroughTime(&(pData->tVechileData)));
	ui.lab_time->setText(sTime);

}

/*
void Dlg_PicView::SetNewPic(u32 dwId, int nType, bool bMjpeg)
{
	QMutex mutex;
	mutex.lock();
	TImageBuf* pImageBuf;
	if (bMjpeg)
	{
		pImageBuf = MgrData::getInstance()->GetMgrMjpegCtrl()->GetAndLockImageBuf(dwId);
	}
	else
	{
		pImageBuf = MgrData::getInstance()->GetMgrEvtLst()->GetAndLockImageBuf(dwId);
	}
	if (pImageBuf == NULL)
	{
		return;
	}

	// 取内存中的数据
	int nShow = 0;

	if (nType != 0)
	{
		//nShow = 0xffff;
		for (int i = 0; i < (int)pImageBuf->dwBufNum; i++)
		{
			if (pImageBuf->anBufType[i] == nType)
			{
				nShow = i;
				break;
			}
		}
	}

	if (nShow < (int)pImageBuf->dwBufNum)
	{
		QPixmap pix;
		pix.loadFromData((const uchar *)pImageBuf->apBuf[nShow], pImageBuf->anBufLen[nShow]);
		int nHeight = ui.lab_picShow->size().height();
		int nWidth = ui.lab_picShow->size().width();
		pix = pix.scaled(nWidth, nHeight);		
		ui.lab_picShow->setPixmap(pix);

	}

	MgrData::getInstance()->GetMgrEvtLst()->UnlockImageBuf();
}
*/



LRESULT Dlg_PicView::OnRefreshPic(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	u32 dwId = (u32)wParam;
	// 图片浏览取缓冲区中的图片
	if (uMsg == NMC_EVT_VIDEO_PIC)
	{
		dwId = 0;
	}

	if (lParam)
	{
		ui.lab_picShow->SetNewPic2(dwId, 0, TRUE);
	}
	else
	{
		ui.lab_picShow->SetNewPic2(dwId, VEH_IMAGE_TYPE_MONTAGE, FALSE);
	}
	return 0;
}

void Dlg_PicView::ProcessEventMsg()
{
	TMAPEVTINFOITE ite = m_mapEvtInfo.begin();
	while (ite != m_mapEvtInfo.end())
	{
		u32 dwEvtType = ite->second;
		u32 dwId = ite->first;

		if (dwEvtType == NMC_EVT_ADD)
		{
			// 添加到列表
			ListAddItem(dwId);

			// 显示图片
			if (m_bDebug == FALSE)
			{
				BOOL bHandled;
				OnRefreshPic(dwEvtType, dwId, 0, bHandled);
			}

			// 显示过车信息
			m_dwId = dwId;
			UpdateEvtInfo();

		}
		else if (dwEvtType == NMC_EVT_DELALL)
		{
			for (int j = ui.treeWidget->topLevelItemCount(); j >= 0; j--)
			{
				delete ui.treeWidget->topLevelItem(j);
			}
			m_nCurPage = 1;
			OnFreshPage();
		}
		else if (dwEvtType == NMC_EVT_SEL)
		{
			// 显示图片
			BOOL bHandled;
			OnRefreshPic(dwEvtType, dwId, 0, bHandled);

			// 显示过车信息
			m_dwId = dwId;
			UpdateEvtInfo();
		}

		m_mapEvtInfo.erase(ite);

		ite = m_mapEvtInfo.begin();
	}
}

bool Dlg_PicView::GetParamScenceInfo()
{
	u32 dIP = MgrData::getInstance()->GetIP();
	MgrItsCtrl* pMgrItsCtrl = MgrData::getInstance()->GetMgrItsCtrl();
	if (pMgrItsCtrl != NULL)
	{
		ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dIP);
		if (pCtrl != NULL)
		{
			int sceneIndex = 1;
			bool bRet = pCtrl->GetIpcScence(&sceneIndex, (u32)(CObserver*)this);
			if (!bRet)
			{
			}
		}
	}
	return true;
}


void Dlg_PicView::OnObserverNotify(LPARAM lHint, LPVOID pHint)
{
	u32 dwEvtType = (u32)lHint;
	u32 dwId = (u32)pHint;

	if (dwEvtType == NMC_GET_IPC_SCENCEINFO)
	{
		TIPC_GetSceneCfgRsp* p = (TIPC_GetSceneCfgRsp*)pHint;
		if (p->nRetCode == 0)
		{
			memcpy(&m_tSeceneCfg, p, sizeof(m_tSeceneCfg));
		}
	}
	else if (dwEvtType == NMC_EVT_SEL)
	{
		m_mapEvtInfo[(u32)pHint] = (u32)lHint;
		emit signal_dataChanged();
		
	}
	else if (dwEvtType == NMC_EVT_ADD)
	{
		m_mapEvtInfo[(u32)pHint] = (u32)lHint;
		emit signal_dataChanged();
	}
	else if (dwEvtType == NMC_EVT_DELALL)
	{
		m_mapEvtInfo[(u32)pHint] = (u32)lHint;
		emit signal_dataChanged();
	}
	else if (dwEvtType == NMC_EVT_VIDEO_PIC)
	{
		emit signal_freshPic(NMC_EVT_VIDEO_PIC, dwId);
		//this->PostMessage(NMC_EVT_VIDEO_PIC, dwId);
	}
}
