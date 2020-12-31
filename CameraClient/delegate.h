#pragma once

#include <QObject>
#include <QItemDelegate>
#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QMap>
#include <QStyledItemDelegate>
#define GBUTF8(da) QString::fromLocal8Bit(da)
class LabelDelegate : /*public QItemDelegate,*/ public QStyledItemDelegate
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

class EditDelegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	EditDelegate(QObject *parent = 0);
	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	void setEditorData(QWidget *editor, const QModelIndex &index) const;
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

	void updateEditorGeometry(QWidget *editor,
		const QStyleOptionViewItem &option,
		const QModelIndex &index) const;
	//void SetIsValidator(bool is){ m_isAddValidator = is; }
	void SetValidatorType(int nType){ m_nType = nType; }
signals:
	//第一个参数是当前行数，第二个参数列数
	void signal_dataChanged(int, int);
private:
	int  m_nType; //0：整数， 1:double 默认0
	EditDelegate* g_EditDelegate;
	
};

class ComboboxDelegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	ComboboxDelegate(QObject *parent = 0);
	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	void setEditorData(QWidget *editor, const QModelIndex &index) const;
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

	void updateEditorGeometry(QWidget *editor,
		const QStyleOptionViewItem &option,
		const QModelIndex &index) const;

	void setComboxData(QStringList sList);

	QMap<int, QComboBox*> m_map;//first 是当前行

signals:
	//第一个参数是当前行数，第二个参数是combobox选中的第几个
	void signal_combDataChanged(int, int);

private:
	QStringList m_list;

	QComboBox *m_cmb;
	
	ComboboxDelegate *g_CmbDelegate;

	
};

class CheckBoxDelegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	CheckBoxDelegate(QObject *parent = 0);
	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	void setEditorData(QWidget *editor, const QModelIndex &index) const;
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

	void updateEditorGeometry(QWidget *editor,
		const QStyleOptionViewItem &option,
		const QModelIndex &index) const;
signals:
	void signal_dataChanged(int, int);

private:
	CheckBoxDelegate *g_CheckDelegate;
};