#pragma once

#include <QWidget>
#include "ui_Dlg_IllegalDict.h"
#include <windows.h>
#include <subject.h>
#include "ivhs_struct.h"
#include "define.h"
#include <QLabel>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QItemDelegate>
#include "delegate.h"
#include "MyWidget.h"
#include "Dlg_MessageBox.h"
#define GBUTF8(da) QString::fromLocal8Bit(da)
/*
class LabelDelegate : public QItemDelegate
{
	Q_OBJECT
public:
	LabelDelegate(QObject *parent = 0);
	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	void setEditorData(QWidget *editor, const QModelIndex &index) const;
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

	void updateEditorGeometry(QWidget *editor,
		const QStyleOptionViewItem &option,
		const QModelIndex &index) const;
	
};

class EditDelegate : public QItemDelegate
{
	Q_OBJECT
public:
	EditDelegate(QObject *parent = 0);
	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	void setEditorData(QWidget *editor, const QModelIndex &index) const;
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

	void updateEditorGeometry(QWidget *editor,
		const QStyleOptionViewItem &option,
		const QModelIndex &index) const ;
	void SetIsValidator(bool is){ m_isAddValidator = is; }
private:
	bool m_isAddValidator;
};
*/
class Dlg_IllegalDict : public MyWidget, public CObserver
{
	Q_OBJECT

public:
	Dlg_IllegalDict(QWidget *parent = Q_NULLPTR);

	~Dlg_IllegalDict();

	void InitData(bool is);

	void OnSetTxtVisible();

	bool GetParam();

	void SetParam();

	void OnObserverNotify(LPARAM lHint, LPVOID pHint);

public slots:

	void Slot_TreeWidgetItemClicked(QTreeWidgetItem *pItem, int column);

	void Slot_DataChanged();

	void Slot_SaveClicked();

	void Slot_ResetClicked();

signals:
	void dataChanged();
private:
	Ui::Dlg_IllegalDict ui;
	Dlg_MessageBox		*m_messageBox;
	TIPC_IllegalDict	m_tIllegalDict;

	EditDelegate *editDelegateName;

	EditDelegate *editDelegateCode;

	LabelDelegate*labDelegateSeq;

	LabelDelegate*labDelegateNumber;
};
