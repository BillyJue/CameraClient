#include "Dlg_SnapManager.h"
#include "MgrData.h"
#include <QPushButton>
#include <time.h>
#include <QTreeWidget>
#include <QHBoxLayout>
#include <wininet.h>
#include "Dlg_MessageBox.h"
#include <QFuture>
#include <QtConcurrent> 
#include <QFileDialog>
#include <shellapi.h>

static void OnExportSelected(void *ptr, QVector<int> vec, QString sdir)
{
	Dlg_SnapManager *pSnap = (Dlg_SnapManager*)ptr;
	if (!pSnap)
	{
		return;
	}
	pSnap->GetProgressBar()->setRange(0, vec.size());
	emit pSnap->signal_progressBarChanged(0);
	for (int i = 0; i < vec.size(); i++)
	{
		if (pSnap->GetOriginPicCheck()->isChecked() || pSnap->GetIllegalMergePicCheck()->isChecked())
		{
			pSnap->ExportImg(pSnap->GetPicQueryResult().aPicFileInfo[vec[i]], sdir);
		}

		//���ع����ļ�¼;	
		if (pSnap->GetSearchResultCheck()->isChecked())
		{
			pSnap->ExportRecord(pSnap->GetPicQueryResult().aPicFileInfo[vec[i]]);
		}
		if (pSnap->GetPicQueryResult().aPicFileInfo[vec[i]].dwVioNum != 0)
		{
			if (pSnap->GetVideoCheck()->isChecked())
			{
				pSnap->ExportVideo(pSnap->GetPicQueryResult().aPicFileInfo[vec[i]]);
			}
		}
		emit pSnap->signal_progressBarChanged(i + 1);
		//pSnap->GetProgressBar()->setValue(i+1);
	}
}

void Dlg_SnapManager::OnExportAll(void* p)
{
	Dlg_SnapManager* dlg = (Dlg_SnapManager*)p;
	dlg->GetProgressBar()->setValue(0);

	if (dlg->GetSumPage() == 1)
	{
		dlg->GetProgressBar()->setRange(0, dlg->GetIPC_GetPicResult()->dwMaxNum);
		dlg->GetProgressBar()->setValue(0);
		for (int i = 0; i < dlg->GetIPC_GetPicResult()->dwMaxNum; i++)
		{
			dlg->GetProgressBar()->setValue(i + 1);

			if (dlg->GetOriginPicCheck()->isChecked() || dlg->GetIllegalMergePicCheck()->isChecked())
			{
				dlg->ExportImg(dlg->GetIPC_GetPicResult()->aPicFileInfo[i], dlg->GetDirPath());
			}

			//���ع����ļ�¼;	
			if (dlg->GetSearchResultCheck()->isChecked())
			{
				dlg->ExportRecord(dlg->GetIPC_GetPicResult()->aPicFileInfo[i]);
			}

			if (dlg->GetPicQueryResult().aPicFileInfo[i].dwVioNum != 0)
			{

				if (dlg->GetVideoCheck()->isChecked())
				{
					dlg->ExportVideo(dlg->GetIPC_GetPicResult()->aPicFileInfo[i]);
				}
			}
		}
	}
	else
	{
		TIPC_PicQueryResult *pp = dlg->GetIPC_GetPicResult();
		dlg->GetProgressBar()->setRange(0, dlg->GetIPC_GetPicResult()->dwMaxNum);
		dlg->GetProgressBar()->setValue(0);

		for (int i = 0; i < (dlg->GetSumPage() - 1); i++)
		{

			for (int j = 0; j < PAGE_NUM; j++)
			{
				if (dlg->GetOriginPicCheck()->isChecked() || dlg->GetIllegalMergePicCheck()->isChecked())
				{
					TIPC_PicQueryResult *pp = dlg->GetIPC_GetPicResult();
					dlg->ExportImg(dlg->GetIPC_GetPicResult()->aPicFileInfo[j], dlg->GetDirPath());
				}
				//���ع����ļ�¼;	
				if (dlg->GetSearchResultCheck()->isChecked())
				{
					dlg->ExportRecord(dlg->GetIPC_GetPicResult()->aPicFileInfo[j]);
					
				}

				if (dlg->GetIPC_GetPicResult()->aPicFileInfo[j].dwVioNum != 0)
				{

					if (dlg->GetVideoCheck()->isChecked())
					{
						dlg->ExportVideo(dlg->GetIPC_GetPicResult()->aPicFileInfo[j]);
					}
				
				}
				emit dlg->signal_progressBarChanged(i*PAGE_NUM + j + 1);
			}
			dlg->GetPicQuery()->dwQueryBeginNum += PAGE_NUM;
			dlg->GetPic(dlg->GetPicQuery());
		}

		for (int i = 0; i < (dlg->GetIPC_GetPicResult()->dwMaxNum - PAGE_NUM*(dlg->GetSumPage() - 1)); i++)
		{

			if (dlg->GetOriginPicCheck()->isChecked() || dlg->GetIllegalMergePicCheck()->isChecked())
			{
				dlg->ExportImg(dlg->GetIPC_GetPicResult()->aPicFileInfo[i], dlg->GetDirPath());
			}

			//���ع����ļ�¼;	
			if (dlg->GetSearchResultCheck()->isChecked())
			{
				dlg->ExportRecord(dlg->GetIPC_GetPicResult()->aPicFileInfo[i]);
			}

			if (dlg->GetPicQueryResult().aPicFileInfo[i].dwVioNum != 0)
			{
				if (dlg->GetVideoCheck()->isChecked())
				{
					dlg->ExportVideo(dlg->GetIPC_GetPicResult()->aPicFileInfo[i]);
				}
			}
			emit dlg->signal_progressBarChanged(PAGE_NUM*(dlg->GetSumPage() - 1) + i + 1);
			//dlg->GetProgressBar()->setValue(PAGE_NUM*(dlg->GetSumPage() - 1) + i + 1);
		}

		////���²�ѯһ�Σ���ֹ�û��ظ� ȫ������;
		dlg->GetPicQuery()->dwQueryBeginNum = 0;
		dlg->GetPic(dlg->GetPicQuery());
	}
	
	}


Dlg_SnapManager::Dlg_SnapManager(QWidget *parent)
	: MyWidget(parent), m_nQueryStartNum(0)
	, m_nCurPage(1), m_nSumPage(1), m_nRecordIndex(0), m_dlgMessageBox(nullptr)
{
	ui.setupUi(this);
	Init();
	MgrData::getInstance()->GetMgrSubject()->Add(this);
	connect(ui.chk_start, SIGNAL(stateChanged(int)), this, SLOT(Slot_SetEnable(int)));
	connect(ui.chk_end, SIGNAL(stateChanged(int)), this, SLOT(Slot_SetEnable(int)));
	connect(ui.chk_laneNum, SIGNAL(stateChanged(int)), this, SLOT(Slot_SetEnable(int)));
	connect(ui.chk_illegalType, SIGNAL(stateChanged(int)), this, SLOT(Slot_SetEnable(int)));
	connect(ui.chk_carNumber, SIGNAL(stateChanged(int)), this, SLOT(Slot_SetEnable(int)));
	connect(ui.chk_all, SIGNAL(stateChanged(int)), this, SLOT(Slot_SetEnable(int)));
	connect(this, SIGNAL(signal_searchDataChanged()), this, SLOT(Slot_SearchChanged()));
	connect(this, SIGNAL(signal_progressBarChanged(int)), ui.progressBar, SLOT(setValue(int)));
	connect(ui.btn_search, SIGNAL(clicked()), this, SLOT(Slot_BtnSearchClicked()));
	connect(ui.btn_export, SIGNAL(clicked()), this, SLOT(Slot_BtnExportClicked()));
	connect(ui.btn_exportAll, SIGNAL(clicked()), this, SLOT(Slot_BtnExportAllClicked()));
	connect(ui.treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(Slot_TreeWidgetDoubleClicked(QTreeWidgetItem *, int)));
	ui.label_10->installEventFilter(this);
	ui.label_11->installEventFilter(this);
	ui.label_8->installEventFilter(this);
	ui.label_12->installEventFilter(this);
	ui.label_15->installEventFilter(this);
}

void Dlg_SnapManager::InitData(bool is)
{

}

void aa(void * ptr)
{
	int a = 1;
	int b = 2;
}

void Dlg_SnapManager::Slot_BtnExportAllClicked()
{
	QString dir = QFileDialog::getExistingDirectory(this, GBUTF8("�ļ���"), ""
		, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if (dir.isEmpty())
	{
		return;
	}
	m_dirPath = dir;
	m_nRecordIndex = 1;
	QFuture<void> future = QtConcurrent::run(Dlg_SnapManager::OnExportAll, (void*)this);
	//future.resume();
}


BOOL Dlg_SnapManager::ExportVideo(TIPC_PicFileInfo t_PicFileInfo)
{
	static HINTERNET hInternet = NULL;
	HINTERNET hFtpSession = NULL;
	if (hInternet == NULL)
	{
		hInternet = InternetOpen(NULL, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	}

	wchar_t ip[32];
	in_addr ipaddr;
	ipaddr.S_un.S_addr = MgrData::getInstance()->GetIP();
	char2wchar(ip, inet_ntoa(ipaddr));

	if (hFtpSession == NULL && hInternet != NULL)
	{
		hFtpSession = InternetConnect(hInternet, ip, INTERNET_DEFAULT_FTP_PORT, MgrData::getInstance()->GetMgrUser()->GetCurUser()->szName,
			MgrData::getInstance()->GetMgrUser()->GetCurUser()->szPassword, INTERNET_SERVICE_FTP, 0, 0);
	}

	wchar_t remotefile[1024] = { 0 };		  //ͼƬ·��

	wchar_t localfile[MAX_PATH];		 //���ش洢·��
	m_dirPath.toWCharArray(localfile);
	for (int i = 1; i <= t_PicFileInfo.dwVioNum; i++)
	{

		GetFileName(t_PicFileInfo, remotefile, localfile, i, false);
		if (hFtpSession != NULL)
		{
			BOOL s = FtpGetFile(hFtpSession, remotefile, localfile, 0, 0, FTP_TRANSFER_TYPE_BINARY, 0);
			//memcpy(localfile, m_achPicPath, MAX_PATH);
		}

	}
	InternetCloseHandle(hFtpSession);
	return TRUE;
}


void Dlg_SnapManager::Slot_BtnExportClicked()
{
	QVector<int> indexVec;
	QTreeWidgetItem *pItem;
	for (int i = 0; i < ui.treeWidget->topLevelItemCount(); i++)
	{
		pItem = ui.treeWidget->topLevelItem(i);
		QWidget *pWidget = ui.treeWidget->itemWidget(pItem, 0);
		QCheckBox *pCheckBox =pWidget->property("check").value<QCheckBox*>();
		if (!pCheckBox)
		{
			continue;
		}
		if (pCheckBox->isChecked())
		{
			indexVec.push_back(i);
		}
	}
	if (indexVec.size() == 0)
	{
		Dlg_MessageBox box;
		box.SetBtnNoVisible(false);
		box.SetInfoText(GBUTF8("�빴ѡ���ص�ͼƬ,лл����!"));
		box.SetTitleInfo(GBUTF8("����"));
		box.exec();
		return;
	}
	if ((!ui.chk_originPic->isChecked()) && (!ui.chk_illegalMergePic->isChecked()) 
		&& (!ui.chk_video->isChecked()) && (!ui.chk__findResult->isChecked()))
	{
		Dlg_MessageBox box;
		box.SetBtnNoVisible(false);
		box.SetInfoText(GBUTF8("��ѡ��Ҫ���ص����ͣ�"));
		box.SetTitleInfo(GBUTF8("����"));
		box.exec();
		return;
	}
	QString dir = QFileDialog::getExistingDirectory(this, GBUTF8("�ļ���"), ""
		,QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if (dir.isEmpty())
	{
		return;
	}
	m_dirPath = dir;
	m_nRecordIndex = 1;
	QFuture<void> future = QtConcurrent::run(OnExportSelected, (void*)this, indexVec, dir);
	future.waitForFinished();
	//Dlg_MessageBox box;
	//box.SetBtnNoVisible(false);
	//box.SetInfoText(GBUTF8("�����ɹ�"));
	//box.SetTitleInfo(GBUTF8("��ʾ"));
	//box.exec();
}

BOOL Dlg_SnapManager::ExportRecord(TIPC_PicFileInfo t_PicFileInfo)
{
	FILE* fp;
	char filepath[256] = { 0 };
	char type[32] = { 0 };
	QDateTime dateTime = QDateTime::fromMSecsSinceEpoch(t_PicFileInfo.dwThroughTime);
	QString sDateTime = dateTime.toString("yyyy-MM-dd HH:mm:ss");

	QString sPathFileName = QString("%1\\%2.txt").arg(m_dirPath).arg(sDateTime);
	fp = fopen(filepath, "a+");
	if (strcmp(t_PicFileInfo.achIllegalType, "00000") == 0 || strcmp(t_PicFileInfo.achIllegalType, "") == 0)
	{
		strcpy(type, "����");
	}
	else if (strcmp(t_PicFileInfo.achIllegalType, "10001") == 0)
	{
		strcpy(type, "����");
	}
	else if (strcmp(t_PicFileInfo.achIllegalType, "10003") == 0)
	{
		strcpy(type, "�����");
	}
	else if (strcmp(t_PicFileInfo.achIllegalType, "10002") == 0)
	{
		strcpy(type, "����");
	}
	else if (strcmp(t_PicFileInfo.achIllegalType, "10005") == 0)
	{
		strcpy(type, "�������򳵵���ʻ");
	}
	else if (strcmp(t_PicFileInfo.achIllegalType, "10010") == 0)
	{
		strcpy(type, "������");
	}
	else if (strcmp(t_PicFileInfo.achIllegalType, "10007") == 0)
	{
		strcpy(type, "ѹʵ��");
	}
	else if (strcmp(t_PicFileInfo.achIllegalType, "10008") == 0)
	{
		strcpy(type, "Υ��ͣ��");
	}
	else if (strcmp(t_PicFileInfo.achIllegalType, "10014") == 0)
	{
		strcpy(type, "Υ����ͷ");
	}
	else if (strcmp(t_PicFileInfo.achIllegalType, "10016") == 0)
	{
		strcpy(type, "ʵ�߱��");
	}

	fprintf(fp, "%d_%s_%s_%s\n", m_nRecordIndex, t_PicFileInfo.achFrontPlateNum, type, sDateTime.toStdString().c_str());

	m_nRecordIndex++;

	fclose(fp);
	return TRUE;
}


bool Dlg_SnapManager::eventFilter(QObject *obj, QEvent *event)
{
	if ((obj == ui.label_10) && (event->type() == QEvent::MouseButtonRelease))
	{
		FirstPage();
		return true;
	}
	else if ((obj == ui.label_8) && (event->type() == QEvent::MouseButtonRelease))
	{
		NextPage();
		return true;
	}
	else if ((obj == ui.label_11) && (event->type() == QEvent::MouseButtonRelease))
	{
		PrePage();
		return true;
	}
	else if ((obj == ui.label_12) && (event->type() == QEvent::MouseButtonRelease))
	{
		LastPage();
		return true;
	}
	else if ((obj == ui.label_15) && (event->type() == QEvent::MouseButtonRelease))
	{
		GoToPage();
		return true;
	}
	return QWidget::eventFilter(obj, event);
}

void Dlg_SnapManager::Slot_BtnSearchClicked()
{
	SearchPicData();
}

void Dlg_SnapManager::GoToPage()
{
	int nIndex = ui.ledt_go->text().toInt();
	if ((nIndex < 1) || (nIndex > m_nSumPage))
	{
		return;
	}
	ui.chk_all->setChecked(false);
	m_nCurPage = nIndex;
	SearchPicData();

}

void Dlg_SnapManager::FirstPage()
{
	ui.chk_all->setChecked(false);
	m_nCurPage = 1;
	SearchPicData();
}

void Dlg_SnapManager::LastPage()
{
	ui.chk_all->setChecked(false);
	m_nCurPage = m_nSumPage;
	SearchPicData();
}

void Dlg_SnapManager::PrePage()
{
	if (m_nCurPage <= 1)
	{
		return;
	}
	ui.chk_all->setChecked(false);
	m_nCurPage--;
	SearchPicData();
}

void Dlg_SnapManager::NextPage()
{
	if (m_nSumPage == m_nCurPage)
	{
		return;
	}
	m_nCurPage++;
	SearchPicData();
}

void Dlg_SnapManager::Slot_FirstPageClicked()
{
	m_nQueryStartNum = 0;
	m_nCurPage = 1;
}

void Dlg_SnapManager::Slot_SearchChanged()
{
	ui.label_14->setText(GBUTF8("����%1����¼").arg(m_tIPC_PicQueryResult.dwMaxNum));
	AddTreeWidgetItem();
}

QWidget* Dlg_SnapManager::CreateLabWidget()
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


void Dlg_SnapManager::AddTreeWidgetItem()
{
	int nPageSum = m_tIPC_PicQueryResult.dwMaxNum %PAGE_NUM == 0 ? m_tIPC_PicQueryResult.dwMaxNum / PAGE_NUM \
		: m_tIPC_PicQueryResult.dwMaxNum / PAGE_NUM + 1;
	m_nSumPage = nPageSum;
	ui.label_9->setText(QString("%1/%2").arg(m_nCurPage).arg(nPageSum));

	ui.treeWidget->clear();
	//connect(ui.treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(Slot_TreeWidgetDoubleClicked(QTreeWidgetItem *, int)));
	int nLen = 0;
	if (m_tIPC_PicQueryResult.dwMaxNum - m_tIPC_PicQueryResult.dwBeignNum >= PAGE_NUM)
	{
		nLen = PAGE_NUM;//PAGE_NUM*m_nCurPage;
	}
	else
	{
		nLen = m_tIPC_PicQueryResult.dwNum%PAGE_NUM;//PAGE_NUM*(m_nCurPage-1) +
	}
	for (int i = 0; i < nLen; i++)//PAGE_NUM*(m_nCurPage - 1) 
	{
		TIPC_PicFileInfo * pFileInfo = &m_tIPC_PicQueryResult.aPicFileInfo[i];

		QString sIllegalName;
		//Υ������
		if (strcmp(pFileInfo->achIllegalType, "00000") == 0 || strcmp(pFileInfo->achIllegalType, "") == 0)
		{
			sIllegalName = GBUTF8("����");
		}
		else
		{
			TVechileData tData;
			tData.tInfo.dwIllegalState = 1;
			memcpy(tData.tInfo.achIllegalType, pFileInfo->achIllegalType, MAX_SIZE_ILLEGAL_TYPE);
			std::wstring str = Vechile::GetIllegalType(&tData);
			sIllegalName = QString::fromStdWString(str);
		}

		
		//pCheck->setFixedWidth(60);
		QString sTime = QDateTime::fromTime_t(pFileInfo->dwThroughTime).toString("yyyy-MM-dd HH:mm:ss");
		QString sSeq = QString("%1").arg(PAGE_NUM*(m_nCurPage - 1) + i+1);
		QString sLaneNum = QString("%1").arg(pFileInfo->dwLaneNum);
		wchar_t platNum[80];
		ZeroMemory(platNum, 80);
		char2wchar(platNum, pFileInfo->achFrontPlateNum);
		QString sCarNum = QString::fromWCharArray(platNum);
		QString sPeed = QString("%1 km/h").arg(pFileInfo->dwSpeed);
		QTreeWidgetItem *pItem = new QTreeWidgetItem;
		ui.treeWidget->addTopLevelItem(pItem);
		pItem->setText(1, sSeq);
		pItem->setTextAlignment(1, Qt::AlignCenter);
		pItem->setText(2, sLaneNum);
		pItem->setTextAlignment(2, Qt::AlignCenter);
		pItem->setText(3, sTime);
		pItem->setTextAlignment(3, Qt::AlignCenter);
		pItem->setText(4, sIllegalName);
		pItem->setTextAlignment(4, Qt::AlignCenter);

		QWidget *pCarNumWidget = CreateLabWidget();
		QLabel *pLab = pCarNumWidget->property("lab").value<QLabel*>();
		pLab->setText(sCarNum);
		 
		QString sBkColor = Vechile::GetPlateColor2(pFileInfo->dwPlateColor);
		QString sTxtColor = "black";
		if (sBkColor != "0")
		{
			sTxtColor = "white";
		}
		else
		{
			sBkColor = "white";
			sTxtColor = "black";
		}
		QString sSheet = QString("background-color:%1; color:%2;").arg(sBkColor).arg(sTxtColor); //
		pLab->setStyleSheet(sSheet);
		ui.treeWidget->setItemWidget(pItem, 5, pCarNumWidget);
		//pItem->setText(5, sCarNum);

		pItem->setTextAlignment(5, Qt::AlignCenter);
		pItem->setText(6, sPeed);
		pItem->setTextAlignment(6, Qt::AlignCenter);
		
		QWidget *pWidget = new QWidget;
		QCheckBox *pCheck = new QCheckBox;
		pCheck->setText("       ");
		pWidget->setProperty("check", QVariant::fromValue(pCheck));
		pWidget->setFixedWidth(80);
		QHBoxLayout *lay = new QHBoxLayout;
		lay->setContentsMargins(0, 0, 0, 0);
		QSpacerItem *h1 = new QSpacerItem(2, 1, QSizePolicy::Expanding, QSizePolicy::Minimum);
		QSpacerItem *h2 = new QSpacerItem(2, 1, QSizePolicy::Expanding, QSizePolicy::Minimum);
		lay->addItem(h1);
		lay->addWidget(pCheck);
		lay->addItem(h2);
		pWidget->setLayout(lay);
		ui.treeWidget->setItemWidget(pItem, 0, pWidget);
	}
}


void Dlg_SnapManager::OnObserverNotify(LPARAM lHint, LPVOID pHint)
{
	u32 dwEvtType = (u32)lHint;
	u32 dwId = (u32)pHint;

	switch (dwEvtType)
	{
	case NMC_QUERY_SD_RECORD:
	{
		TIPC_PicQueryResult *p = (TIPC_PicQueryResult*)pHint;
		memcpy(&m_tIPC_PicQueryResult, p, sizeof(TIPC_PicQueryResult));
		memcpy(&m_tIPC_GetPicQuery, &m_tIPC_PicQuery, sizeof(TIPC_PicQuery));
		memcpy(&m_tIPC_GetPicResult, p, sizeof(TIPC_PicQueryResult));		 //ֻ�е��search��ť��ʱ������һ�����������ĸ���;
		signal_searchDataChanged();
		break;
	}
	case NMC_GET_SD_RECORD:
	{
		TIPC_PicQueryResult *p = (TIPC_PicQueryResult*)pHint;
		memcpy(&m_tIPC_GetPicResult, p, sizeof(TIPC_PicQueryResult));
		
		break;
	}

	}
}

void Dlg_SnapManager::Init()
{
	ui.progressBar->setValue(0);
	ui.ledt_go->setValidator(new QIntValidator(0, 1000, this));
	ui.treeWidget->clear();
	ui.treeWidget->setColumnCount(8);
	ui.treeWidget->setHeaderHidden(true);
	ui.treeWidget->setColumnWidth(0, 80);
	ui.treeWidget->setColumnWidth(1, 80);
	ui.treeWidget->setColumnWidth(2, 100);
	ui.treeWidget->setColumnWidth(3, 200);
	ui.treeWidget->setColumnWidth(4, 100);
	ui.treeWidget->setColumnWidth(5, 100);
	ui.treeWidget->setColumnWidth(6, 100);
	ui.treeWidget->setColumnWidth(7, 100);

	ui.ledt_laneNum->setText("1");
	ui.ledt_laneNum->setValidator(new QIntValidator(0, 20, this));
	ui.date_start->setEnabled(false);
	ui.date_end->setEnabled(false);
	ui.ledt_laneNum->setEnabled(false);
	ui.cmb_illegalType->setEnabled(false);
	ui.ledt_carNumber->setEnabled(false);
	ui.chk_likefind->setEnabled(false);

	QStringList listItem;
	listItem.append(GBUTF8("����"));
	listItem.append(GBUTF8("ȫ��Υ��"));
	listItem.append(GBUTF8("����"));
	listItem.append(GBUTF8("����"));
	listItem.append(GBUTF8("����������ʻ"));
	listItem.append(GBUTF8("������ռ�÷ǻ�������"));
	listItem.append(GBUTF8("ռ��Ӧ������"));
	listItem.append(GBUTF8("ռ�ù�������"));
	listItem.append(GBUTF8("���ͳ�ռ��С�ͳ���"));
	listItem.append(GBUTF8("��ֹ���˳�ͨ��"));
	listItem.append(GBUTF8("ѹʵ��"));
	listItem.append(GBUTF8("ʵ�߱��"));
	listItem.append(GBUTF8("�����"));
	listItem.append(GBUTF8("�������򳵵���ʻ"));
	listItem.append(GBUTF8("Խ��ͣ��"));
	listItem.append(GBUTF8("·������"));
	listItem.append(GBUTF8("����δϵ��ȫ��"));
	listItem.append(GBUTF8("����δϵ��ȫ��"));
	listItem.append(GBUTF8("���ݿ�����绰"));
	listItem.append(GBUTF8("��ֹ��ת"));
	listItem.append(GBUTF8("����Сת"));
	listItem.append(GBUTF8("Υ��ͣ��"));
	listItem.append(GBUTF8("Υ����ͷ"));
	listItem.append(GBUTF8("������ͣ��"));
	listItem.append(GBUTF8("������δ��������"));
	listItem.append(GBUTF8("����·Υ������"));
	listItem.append(GBUTF8("ת��δ��ֱ��"));
	listItem.append(GBUTF8("�ǻ����������"));
	listItem.append(GBUTF8("�����������涨��ʻ"));
	listItem.append(GBUTF8("δ�����г���ȫ����"));
	listItem.append(GBUTF8("��������"));
	listItem.append(GBUTF8("Υ������"));
	listItem.append(GBUTF8("�ǻ�����ռ�û�������"));
	listItem.append(GBUTF8("�ǻ���������"));
	listItem.append(GBUTF8("�ǻ�������װɡ��"));

	ui.cmb_illegalType->addItems(listItem);
	ui.cmb_illegalType->setCurrentIndex(0);

	QDateTime curTime = QDateTime::currentDateTime();
	//QString sStartTime = curTime.toString("yyyy/MM/dd HH:mm:ss");
	ui.date_end->setDateTime(curTime);
	ui.date_start->setDateTime(curTime);

}

void Dlg_SnapManager::Slot_SetEnable(int nEnable)
{
	QObject* senderObj = this->sender();
	if (senderObj == ui.chk_start)
	{
		ui.date_start->setEnabled(nEnable);
	}
	else if (senderObj == ui.chk_end)
	{
		ui.date_end->setEnabled(nEnable);
	}
	else if (senderObj == ui.chk_laneNum)
	{
		ui.ledt_laneNum->setEnabled(nEnable);
	}
	else if (senderObj == ui.chk_illegalType)
	{
		ui.cmb_illegalType->setEnabled(nEnable);
	}
	else if (senderObj == ui.chk_carNumber)
	{
		ui.ledt_carNumber->setEnabled(nEnable);
		ui.chk_likefind->setEnabled(nEnable);
	}
	else if (senderObj == ui.chk_all)
	{
		if (ui.chk_all->isChecked())
		{
			ui.chk_all->setText(GBUTF8("ȫ��ѡ"));
		}
		else
		{
			ui.chk_all->setText(GBUTF8("ȫѡ"));
		}
		QTreeWidgetItem *pItem;
		for (int i = 0; i < ui.treeWidget->topLevelItemCount(); i++)
		{
			pItem = ui.treeWidget->topLevelItem(i);
			QWidget *pWidget = ui.treeWidget->itemWidget(pItem, 0);
			QCheckBox *pCheckBox = pWidget->property("check").value<QCheckBox*>();
			if (!pCheckBox)
			{
				continue;
			}
			pCheckBox->setChecked(nEnable);
		}

		//SearchPicData();
		//SearchPic();
	}
	
}



Dlg_SnapManager::~Dlg_SnapManager()
{
	MgrData::getInstance()->GetMgrSubject()->Del(this);
}
//��ͼƬ
void Dlg_SnapManager::Slot_TreeWidgetDoubleClicked(QTreeWidgetItem *pItem, int nColumn)
{
	int iIndex = pItem->text(1).toInt() - 1;
	QString sDir = QString("C:\\IPC Control\\SdSelectedPic");
	QDir dir(sDir);
	if (!dir.exists())
	{
		dir.mkdir(sDir);
	}
	wchar_t filePath[MAX_PATH];
	ZeroMemory(filePath, MAX_PATH);
	sDir.toWCharArray(filePath);

	wchar_t wsFilePathBk[MAX_PATH] = { 0 };
	memcpy(wsFilePathBk, filePath, MAX_PATH);

	//�ж�������������Υ�����ͻ��ǿ����¼�;�����Υ������ �к�ͼ����ʾ��ͼ;(��δʵ��, ��������ʵ��)
	//if( strcmp(m_tIPC_PicQueryResult.aPicFileInfo[iIndex].achIllegalType, "") == 0)

	int count = 0;
	ExportImg(m_tIPC_PicQueryResult.aPicFileInfo[iIndex], filePath, &count);

	wchar_t fileName[128] = { 0 };
	GetFileName(m_tIPC_PicQueryResult.aPicFileInfo[iIndex], NULL, fileName, count, true);
	std::wstring s = fileName;
	std::wstring name = s.substr(1);


	wcscat(wsFilePathBk, TEXT("\\"));


	wchar_t strParm[MAX_PATH];
	wsprintf(strParm, TEXT("%s%s"), wsFilePathBk, name.c_str());

	wchar_t strSystemDir[MAX_PATH];
	::GetSystemDirectoryW(strSystemDir, MAX_PATH);
	wchar_t strRundll[MAX_PATH];
	wsprintf(strRundll, TEXT("%s%s"), strSystemDir, TEXT("\\rundll32.exe"));

	wchar_t strParm2[MAX_PATH];
	wsprintf(strParm2, TEXT("shimgvw.dll imageview_fullscreen %s"), strParm);

	::ShellExecuteW(NULL, TEXT("Open"), strRundll, strParm2, NULL, SW_SHOW);

	memset(filePath, 0, MAX_PATH);
}

void Dlg_SnapManager::GetFileName(TIPC_PicFileInfo t_PicFileInfo, wchar_t* remotefile, wchar_t* localfile, int i, bool flag) //flag��ʾ�Ƿ����ص�Ϊ ͼƬ������Ƶ;
{
	char fileExten[32] = { 0 };
	itoa(i, fileExten, 10);
	if (flag)
	{
		strcat(fileExten, ".jpg");
		strcat(t_PicFileInfo.achPicPath, fileExten);
		if (remotefile != NULL)
		{
			char2wchar(remotefile, t_PicFileInfo.achPicPath);
		}
	}
	else
	{
		strcat(fileExten, ".avi");
		strcat(t_PicFileInfo.achVioPath, fileExten);
		if (remotefile != NULL)
		{
			char2wchar(remotefile, t_PicFileInfo.achVioPath);
		}
	}


	char snapTime[129] = { 0 };

	time_t dwSec = t_PicFileInfo.dwThroughTime;
	struct tm tLocalTime;
	localtime_s(&tLocalTime, &dwSec);
	sprintf(snapTime, "%04d%02d%02d%02d%02d%02d",
		tLocalTime.tm_year + 1900,
		tLocalTime.tm_mon + 1,
		tLocalTime.tm_mday,
		tLocalTime.tm_hour,
		tLocalTime.tm_min,
		tLocalTime.tm_sec);

	wchar_t fileName[256] = { 0 };
	wchar_t plateNum[32] = { 0 };
	char2wchar(fileName, snapTime);
	char2wchar(plateNum, t_PicFileInfo.achFrontPlateNum);
	wcscat(fileName, TEXT("_"));
	wcscat(fileName, plateNum);

	wchar_t wsFileExten[32] = { 0 };
	char2wchar(wsFileExten, fileExten);
	wcscat(localfile, TEXT("\\"));
	wcscat(localfile, fileName);
	wcscat(localfile, TEXT("_"));
	//�����һ��recordID��;��ֹ�����ظ�;
	char recordID[32] = { 0 };
	itoa(t_PicFileInfo.dwRecordID, recordID, 10);
	wchar_t wsRecordID[32] = { 0 };
	char2wchar(wsRecordID, recordID);
	wcscat(localfile, wsRecordID);
	wcscat(localfile, TEXT("_"));

	wcscat(localfile, wsFileExten);
}

BOOL Dlg_SnapManager::GetPic(TIPC_PicQuery* tIPC_PicQuery)
{
	bool bRet = false;
	u32 dIP = MgrData::getInstance()->GetIP();
	ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dIP);
	if (pCtrl != NULL)
	{
		bRet = pCtrl->GetSdRecord(tIPC_PicQuery, (u32)(CObserver*)this);
	}
	return TRUE;
}


BOOL Dlg_SnapManager::ExportImg(TIPC_PicFileInfo t_PicFileInfo, QString sPath)  //����ͼƬ���ͷ��ർ��ͼƬ;
{
	/*static*/ HINTERNET hInternet = NULL;
	HINTERNET hFtpSession = NULL;
	if (hInternet == NULL)
	{
		hInternet = InternetOpen(NULL, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	}


	wchar_t ip[32];
	in_addr ipaddr;
	ipaddr.S_un.S_addr = MgrData::getInstance()->GetIP();
	char2wchar(ip, inet_ntoa(ipaddr));

	if (hFtpSession == NULL && hInternet != NULL)
	{
		hFtpSession = InternetConnect(hInternet, ip, INTERNET_DEFAULT_FTP_PORT, MgrData::getInstance()->GetMgrUser()->GetCurUser()->szName,
			MgrData::getInstance()->GetMgrUser()->GetCurUser()->szPassword, INTERNET_SERVICE_FTP, 0, 0);
	}

	wchar_t remotefile[1024] = { 0 };		  //ͼƬ·��
	wchar_t localfile[MAX_PATH];
	ZeroMemory(localfile, MAX_PATH);
	sPath.toWCharArray(localfile);
	
	//--��ȡͼƬ������ͼƬ����; ��8λ��ʾͼƬ����; ��24λ��ʾͼƬ����(ԭͼΪ1, ��ͼΪ2, �汾����֮ǰ��ͼƬ��Ϊ0);
	u32 picCount = (u32)(t_PicFileInfo.dwPicNum & 0xff);  //ȡ���Ͱ�λ��,��ʾͼƬ����;

	//��24λ��ʾ6��ͼƬ������; ÿ4λһ��;
	//1���ϳ�ͼƬ 2��ԭͼ 0:�汾����ǰSD���е�ͼƬ;
	for (int i = 1; i <= picCount; i++)
	{
		if (ui.chk_originPic->isChecked() && !ui.chk_illegalMergePic->isChecked()) //������ԭͼ
		{
			//�ж�ͼƬ����
			u32 picType = (u32)(t_PicFileInfo.dwPicNum >> (8 + (i - 1) * 4));
			picType &= 0xf;
			if (picType != 1 && picType != 0)
			{
				GetFileName(t_PicFileInfo, remotefile, localfile, i, true);

				if (hFtpSession != NULL)
				{
					BOOL s = FtpGetFile(hFtpSession, remotefile, localfile, 0, 0, FTP_TRANSFER_TYPE_BINARY, 0);
					//memcpy(localfile, m_achPicPath, MAX_PATH);
				}
			}

		}
		else if (!ui.chk_originPic->isChecked() && ui.chk_illegalMergePic->isChecked())// �����غϳ�ͼƬ
		{
			//�ж�ͼƬ����
			u32 picType = (u32)(t_PicFileInfo.dwPicNum >> (8 + (i - 1) * 4));
			picType &= 0xf;
			if (picType == 1 || picType == 0)
			{
				GetFileName(t_PicFileInfo, remotefile, localfile, i, true);

				if (hFtpSession != NULL)
				{
					BOOL s = FtpGetFile(hFtpSession, remotefile, localfile, 0, 0, FTP_TRANSFER_TYPE_BINARY, 0);
					//memcpy(localfile, m_achPicPath, MAX_PATH);
				}
			}
		}
		else if(ui.chk_originPic->isChecked() && ui.chk_illegalMergePic->isChecked()) //ԭͼ�ͺϳ�ͼƬ������,�����ж�ͼƬ����;
		{
			//	u32 picType = (u32)(t_PicFileInfo.dwPicNum >> (8+(i-1)*4));
			//	picType &= 0xf;

			GetFileName(t_PicFileInfo, remotefile, localfile, i, true);

			if (hFtpSession != NULL)
			{
				BOOL s = FtpGetFile(hFtpSession, remotefile, localfile, 0, 0, FTP_TRANSFER_TYPE_BINARY, 0);
				//memcpy(localfile, m_achPicPath, MAX_PATH);
			}
		}
	}

	InternetCloseHandle(hFtpSession);
	return TRUE;
}



BOOL Dlg_SnapManager::ExportImg(TIPC_PicFileInfo t_PicFileInfo, wchar_t* path, int* count)
{
	static HINTERNET hInternet = NULL;
	HINTERNET hFtpSession = NULL;
	if (hInternet == NULL)
	{
		hInternet = InternetOpen(NULL, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	}

	wchar_t ip[32];
	in_addr ipaddr;
	ipaddr.S_un.S_addr = MgrData::getInstance()->GetIP();
	char2wchar(ip, inet_ntoa(ipaddr));

	if (hFtpSession == NULL && hInternet != NULL)
	{
		hFtpSession = InternetConnect(hInternet, ip, INTERNET_DEFAULT_FTP_PORT, MgrData::getInstance()->GetMgrUser()->GetCurUser()->szName,
			MgrData::getInstance()->GetMgrUser()->GetCurUser()->szPassword, INTERNET_SERVICE_FTP, 0, 0);
	}

	wchar_t remotefile[1024] = { 0 };		  //ͼƬ·��
	wchar_t* localfile = path;

	//�ж�ͼƬ������ �����Υ���¼���򿪺�ͼ;


	//�ж�ͼƬ����
	//--��ȡͼƬ������ͼƬ����; ��8λ��ʾͼƬ����; ��24λ��ʾͼƬ����(ԭͼΪ1, ��ͼΪ2, �汾����֮ǰ��ͼƬ��Ϊ0);
	u32 picCount = (u32)(t_PicFileInfo.dwPicNum & 0xff);  //ȡ���Ͱ�λ��,��ʾͼƬ����;

	//��24λ��ʾ6��ͼƬ������; ÿ4λһ��;
	//1���ϳ�ͼƬ 2��ԭͼ 0:�汾����ǰSD���е�ͼƬ;
	bool bFindCompositeImg = false;
	for (int i = 1; i <= picCount; i++)
	{
		u32 picType = (u32)(t_PicFileInfo.dwPicNum >> (8 + (i - 1) * 4));
		picType &= 0xf;
		if (picType == 1 || picType == 0)
		{
			GetFileName(t_PicFileInfo, remotefile, localfile, i, true);
			if (hFtpSession != NULL)
			{
				FtpGetFile(hFtpSession, remotefile, localfile, 0, 0, FTP_TRANSFER_TYPE_BINARY, 0);
				wchar_t* localfile = path;
			}
			bFindCompositeImg = true;
			*count = i;
			break;
		}
	}
	if (!bFindCompositeImg)
	{
		GetFileName(t_PicFileInfo, remotefile, localfile, 1, true);
		if (hFtpSession != NULL)
		{
			FtpGetFile(hFtpSession, remotefile, localfile, 0, 0, FTP_TRANSFER_TYPE_BINARY, 0);
			wchar_t* localfile = path;
		}
		*count = 1;
	}

	InternetCloseHandle(hFtpSession);
	return TRUE;
}





void Dlg_SnapManager::SearchPicData()
{
	m_tIPC_PicQuery.dwLaneNumEnable = ui.chk_laneNum->isChecked() ? 1 : 0;
	m_tIPC_PicQuery.dwLaneNum = ui.ledt_laneNum->text().toInt();

	
	m_tIPC_PicQuery.dwStartTimeEnable = ui.chk_start->isChecked() ? 1 : 0;
	m_tIPC_PicQuery.dwStartTime = ui.date_start->dateTime().toTime_t();

	m_tIPC_PicQuery.dwEndTimeEnable = ui.chk_end->isChecked() ? 1 : 0;
	m_tIPC_PicQuery.dwEndTime = ui.date_end->dateTime().toTime_t();

	m_tIPC_PicQuery.dwFrontPlateNumEnable = ui.chk_carNumber->isChecked() ? 1 : 0;
	m_tIPC_PicQuery.dwVaguePlateMatch = ui.chk_likefind->isChecked() ? 1 : 0;
	memset(m_tIPC_PicQuery.achFrontPlateNum, 0, MAX_SIZE_PLATE_NUM);
	QString sCarNumber = ui.ledt_carNumber->text();
	memcpy(m_tIPC_PicQuery.achFrontPlateNum, sCarNumber.toStdString().c_str(), 16);

	m_tIPC_PicQuery.dwIllegalTypeEnable = ui.chk_illegalType->isChecked() ? 1 : 0;
	int sel = ui.cmb_illegalType->currentIndex();
	memset(m_tIPC_PicQuery.achIllegalType, 0, MAX_SIZE_ILLEGAL_TYPE);

	char achIllegaNormal[MAX_SIZE_ILLEGAL_TYPE] = { 0 };

	switch (sel)
	{
	case 0:
		strcpy_s(m_tIPC_PicQuery.achIllegalType, MAX_SIZE_ILLEGAL_TYPE, achIllegaNormal);       //����
		break;
	case 1:
		strcpy_s(m_tIPC_PicQuery.achIllegalType, MAX_SIZE_ILLEGAL_TYPE, "1");	  //ȫ��Υ��
		break;
	case 2:
		strcpy_s(m_tIPC_PicQuery.achIllegalType, MAX_SIZE_ILLEGAL_TYPE, "10001");	//����
		break;
	case 3:
		strcpy_s(m_tIPC_PicQuery.achIllegalType, MAX_SIZE_ILLEGAL_TYPE, "10002");   //����
		break;
	case 4:
		strcpy_s(m_tIPC_PicQuery.achIllegalType, MAX_SIZE_ILLEGAL_TYPE, "10010");   //�����涨������ʻ
		break;
	case 5:
		strcpy_s(m_tIPC_PicQuery.achIllegalType, MAX_SIZE_ILLEGAL_TYPE, "10015");   /*������ռ�÷ǻ�������*/
		break;
	case 6:
		strcpy_s(m_tIPC_PicQuery.achIllegalType, MAX_SIZE_ILLEGAL_TYPE, "10023");	 /*ռ��Ӧ������*/
		break;
	case 7:
		strcpy_s(m_tIPC_PicQuery.achIllegalType, MAX_SIZE_ILLEGAL_TYPE, "10013");    /*ռ�ù�������*/
		break;
	case 8:
		strcpy_s(m_tIPC_PicQuery.achIllegalType, MAX_SIZE_ILLEGAL_TYPE, "10107");     /*���ͳ�ռ��С�ͳ�����*/
		break;
	case 9:
		strcpy_s(m_tIPC_PicQuery.achIllegalType, MAX_SIZE_ILLEGAL_TYPE, "10025");	  /*��ֹ���˳���ͨ��*/
		break;
	case 10:
		strcpy_s(m_tIPC_PicQuery.achIllegalType, MAX_SIZE_ILLEGAL_TYPE, "10017");	  /*ѹʵ�� */
		break;
	case 11:
		strcpy_s(m_tIPC_PicQuery.achIllegalType, MAX_SIZE_ILLEGAL_TYPE, "10016");	  /*ʵ�߱�� */
		break;
	case 12:
		strcpy_s(m_tIPC_PicQuery.achIllegalType, MAX_SIZE_ILLEGAL_TYPE, "10003");	  /*�����������*/
		break;
	case 13:
		strcpy_s(m_tIPC_PicQuery.achIllegalType, MAX_SIZE_ILLEGAL_TYPE, "10005");	   /*�������򳵵���ʻ*/
		break;
	case 14:
		strcpy_s(m_tIPC_PicQuery.achIllegalType, MAX_SIZE_ILLEGAL_TYPE, "10012");	  /* Խ��ͣ��*/
		break;
	case 15:
		strcpy_s(m_tIPC_PicQuery.achIllegalType, MAX_SIZE_ILLEGAL_TYPE, "10103");	  /*·������*/
		break;
	case 16:
		strcpy_s(m_tIPC_PicQuery.achIllegalType, MAX_SIZE_ILLEGAL_TYPE, "10018");	  /*����δϵ��ȫ�� */
		break;
	case 17:
		strcpy_s(m_tIPC_PicQuery.achIllegalType, MAX_SIZE_ILLEGAL_TYPE, "10020");	  /*���ݿ�����绰 */
		break;
	case 18:
		strcpy_s(m_tIPC_PicQuery.achIllegalType, MAX_SIZE_ILLEGAL_TYPE, "10019");	  /*����δϵ��ȫ�� */
		break;
	case 19:
		strcpy_s(m_tIPC_PicQuery.achIllegalType, MAX_SIZE_ILLEGAL_TYPE, "10011");	  /* ��ֹ��ת*/
		break;
	case 20:
		strcpy_s(m_tIPC_PicQuery.achIllegalType, MAX_SIZE_ILLEGAL_TYPE, "10106");	   /*����Сת*/
		break;
	case 21:
		strcpy_s(m_tIPC_PicQuery.achIllegalType, MAX_SIZE_ILLEGAL_TYPE, "10008");	   /*Υ��ͣ��*/
		break;
	case 22:
		strcpy_s(m_tIPC_PicQuery.achIllegalType, MAX_SIZE_ILLEGAL_TYPE, "10014");	  /*Υ����ͷ */
		break;
	case 23:
		strcpy_s(m_tIPC_PicQuery.achIllegalType, MAX_SIZE_ILLEGAL_TYPE, "10111");	   /*���к����������ͣ��*/
		break;
	case 24:
		strcpy_s(m_tIPC_PicQuery.achIllegalType, MAX_SIZE_ILLEGAL_TYPE, "10026");	    /*������δ��������*/
		break;
	case 25:
		strcpy_s(m_tIPC_PicQuery.achIllegalType, MAX_SIZE_ILLEGAL_TYPE, "10102");	  /*����·Υ������*/
		break;
	case 26:
		strcpy_s(m_tIPC_PicQuery.achIllegalType, MAX_SIZE_ILLEGAL_TYPE, "10105");	   /*ת��δ��ֱ��*/
		break;
	case 27:
		strcpy_s(m_tIPC_PicQuery.achIllegalType, MAX_SIZE_ILLEGAL_TYPE, "10108");	   /*���˻�ǻ����������*/
		break;
	case 28:
		strcpy_s(m_tIPC_PicQuery.achIllegalType, MAX_SIZE_ILLEGAL_TYPE, "10021");	   /*�����������涨��ʻ */
		break;
	case 29:
		strcpy_s(m_tIPC_PicQuery.achIllegalType, MAX_SIZE_ILLEGAL_TYPE, "10027");	   /*�����涨�����г���ȫ���� */
		break;
	case 30:
		strcpy_s(m_tIPC_PicQuery.achIllegalType, MAX_SIZE_ILLEGAL_TYPE, "10024");	   /*�������� */
		break;
	case 31:
		strcpy_s(m_tIPC_PicQuery.achIllegalType, MAX_SIZE_ILLEGAL_TYPE, "10028");	   /*Υ������ */
		break;
	case 32:
		strcpy_s(m_tIPC_PicQuery.achIllegalType, MAX_SIZE_ILLEGAL_TYPE, "10601");	   /*�ǻ�����ռ�û������� */
		break;
	case 33:
		strcpy_s(m_tIPC_PicQuery.achIllegalType, MAX_SIZE_ILLEGAL_TYPE, "10602");	   /*�ǻ��������� */
		break;
	case 34:
		strcpy_s(m_tIPC_PicQuery.achIllegalType, MAX_SIZE_ILLEGAL_TYPE, "10603");	   /*�ǻ�������װɡ�� */
	default:
		break;
	}

	//����״̬ʹ��
	m_tIPC_PicQuery.dwUpdateFlagEnable = 0;

	//����״̬
	m_tIPC_PicQuery.dwUpdateFlag = 1;

	//��ѯ��ʼλ��;

	m_tIPC_PicQuery.dwQueryBeginNum = PAGE_NUM*(m_nCurPage-1);
	//ϣ���Ĳ�ѯ��Ŀ;
	m_tIPC_PicQuery.dwQueryNum = PAGE_NUM;

	if (ui.chk_start->isChecked() && ui.chk_end->isChecked())
	{
		time_t startSec = ui.date_start->dateTime().toTime_t();
		time_t endSec = ui.date_end->dateTime().toTime_t();
		if (startSec > endSec)
		{
			return ;
		}
	}

	//m_tIPC_PicQuery.dwQueryBeginNum = m_nQueryStartNum;

	bool bRet = false;

	u32 dIP = MgrData::getInstance()->GetIP();
	ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dIP);
	if (pCtrl != NULL)
	{
		bRet = pCtrl->QuerySdRecord(&m_tIPC_PicQuery, (u32)(CObserver*)this);
	}
}


