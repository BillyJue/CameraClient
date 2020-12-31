#include "dlg_illegaldict.h"
#include "MgrData.h"
#include <QLineEdit>
#include <QHeaderView>
#include <QTimer>

/*
	QHeaderView{font-size: 14px;}
	QHeaderView::section
	{
	color:white;
	padding: 4px;
	height: 24px;
	border: 2px solid #567dbc;
	border - left: 0px;
	border - right: 0px;
	background:  #567dbc;
	}

	QTreeView::item:hover
	{
	background-color:rgb(0,255,0,50);
	}

	QTreeView::item:selected{
	background-color:rgb(0,0,255);
	}


	QTreeWidget::item{
	height:30px;
	text-align:center;

	}
	QTreeWidget{
	outline:0px;

	}


*/

Dlg_IllegalDict::Dlg_IllegalDict(QWidget *parent)
	: MyWidget(parent), m_messageBox(nullptr)
{
	ui.setupUi(this);
	MgrData::getInstance()->GetMgrSubject()->Add(this);
	connect(ui.treeWidget, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(Slot_TreeWidgetItemClicked(QTreeWidgetItem*, int)));
	connect(this, SIGNAL(dataChanged()), this, SLOT(Slot_DataChanged()));
	connect(ui.btn_reset, SIGNAL(clicked()), this, SLOT(Slot_ResetClicked()));
	connect(ui.btn_save, SIGNAL(clicked()), this, SLOT(Slot_SaveClicked()));
	
	editDelegateName = new EditDelegate;
	editDelegateCode = new EditDelegate;
	//editDelegateCode->SetIsValidator(true);
	labDelegateSeq = new LabelDelegate;
	labDelegateNumber = new LabelDelegate;
	
	ui.treeWidget->setItemDelegateForColumn(0, labDelegateSeq);
	ui.treeWidget->setItemDelegateForColumn(3, labDelegateNumber);
	ui.treeWidget->setItemDelegateForColumn(1, editDelegateName);
	ui.treeWidget->setItemDelegateForColumn(2, editDelegateCode);
	
	//InitData();
	ui.treeWidget->header()->sectionResizeMode(QHeaderView::ResizeToContents);
	//ui.treeWidget->header()->setStretchLastSection(true);

}

void Dlg_IllegalDict::OnSetTxtVisible()
{
	if (m_messageBox)
	{
		m_messageBox->close();
		SAFE_DELETE(m_messageBox);
	}
}

void Dlg_IllegalDict::InitData(bool is)
{
	if (is)
	{
		GetParam();
	}
	
}

void Dlg_IllegalDict::Slot_ResetClicked()
{

}

void Dlg_IllegalDict::Slot_SaveClicked()
{
	for (int i = 0; i < ui.treeWidget->topLevelItemCount(); i++)
	{
		QTreeWidgetItem *pItem = ui.treeWidget->topLevelItem(i);
		wchar_t wdev[129];
		wchar_t wdevCode[129];

		pItem->text(1).toWCharArray(wdev);
		Uint32 uCode= pItem->text(2).toUInt();
		memcpy(m_tIllegalDict.tIllegalCode[i].illegalName, wdev, 129);
		m_tIllegalDict.tIllegalCode[i].code = uCode;
	}
	SetParam();
	if (!m_messageBox)
	{
		QTimer::singleShot(1500, this, &Dlg_IllegalDict::OnSetTxtVisible);
		m_messageBox = new Dlg_MessageBox;
		m_messageBox->SetInfoText(GBUTF8("±£´æ³É¹¦£¡"));
		m_messageBox->SetBtnNoVisible(false);
		m_messageBox->exec();
	}

}

void Dlg_IllegalDict::Slot_DataChanged()
{
	//ui.treeWidget->clear();
	for (int i = 0; i < m_tIllegalDict.sum; i++)
	{
		wchar_t wdev[129];
		char2wchar(wdev, m_tIllegalDict.tIllegalCode[i].illegalName);
		QString sIllegalName = QString::fromWCharArray(wdev);
		QTreeWidgetItem *pItem = new QTreeWidgetItem;
		pItem->setSizeHint(0, QSize(120, 30));
		pItem->setText(0, QString("%1").arg(i + 1));
		pItem->setText(1, sIllegalName);
		pItem->setText(2, QString("%1").arg(m_tIllegalDict.tIllegalCode[i].code));
		pItem->setText(3, QString("%1").arg(m_tIllegalDict.tIllegalCode[i].ivhsEventCode));

		ui.treeWidget->insertTopLevelItem(i, pItem);
	}
}

void Dlg_IllegalDict::SetParam()
{
	u32 dIP = MgrData::getInstance()->GetIP();
	ItsCtrl* pItsCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dIP);
	if (pItsCtrl == NULL)
	{
		return;
	}
	pItsCtrl->SetIpcIllegalDict(&m_tIllegalDict);
}

bool Dlg_IllegalDict::GetParam()
{
	MgrItsCtrl* pMgrItsCtrl = MgrData::getInstance()->GetMgrItsCtrl();
	if (pMgrItsCtrl != NULL)
	{
		u32 dIP = MgrData::getInstance()->GetIP();
		ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dIP);
		if (pCtrl != NULL)
		{
			bool bRet = pCtrl->GetIpcIllegalDict(NULL, (u32)(CObserver*)this);
			if (!bRet)
			{
			}
		}
	}
	return true;
}

void Dlg_IllegalDict::Slot_TreeWidgetItemClicked(QTreeWidgetItem *pItem, int column)
{

	if (column != 0 && column != 3)
	{
		pItem->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
	}
	else
	{
		pItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
	}
	
	
}

Dlg_IllegalDict::~Dlg_IllegalDict()
{
	MgrData::getInstance()->GetMgrSubject()->Del(this);
	SAFE_DELETE(editDelegateCode);
	SAFE_DELETE(editDelegateName);
	SAFE_DELETE(labDelegateSeq);
	SAFE_DELETE(labDelegateNumber);
	SAFE_DELETE(m_messageBox);
}

void Dlg_IllegalDict::OnObserverNotify(LPARAM lHint, LPVOID pHint)
{
	u32 dwEvtType = (u32)lHint;
	u32 dwId = (u32)pHint;
	switch (dwEvtType)
	{
	case NMC_GET_IPC_ILLEGALDICT:
	{
		TIPC_IllegalDict* p = (TIPC_IllegalDict*)pHint;

		memcpy(&m_tIllegalDict, p, sizeof(TIPC_IllegalDict));
		emit dataChanged();
		
	}
		break;
	default:
		break;
	}
}

