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
	//��һ�������ǵ�ǰ�������ڶ�����������
	void signal_dataChanged(int, int);
private:
	int  m_nType; //0�������� 1:double Ĭ��0
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

	QMap<int, QComboBox*> m_map;//first �ǵ�ǰ��

signals:
	//��һ�������ǵ�ǰ�������ڶ���������comboboxѡ�еĵڼ���
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