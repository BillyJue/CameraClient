#include "delegate.h"
#include <QListView>

LabelDelegate::LabelDelegate(QObject *parent /* = 0 */) :/*QItemDelegate(parent),*/ QStyledItemDelegate(parent)
{

}

QWidget *LabelDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QLabel *lab = new QLabel(parent);
	//lab->setStyleSheet("color:rgb(255,255,255);");
	lab->installEventFilter(const_cast<LabelDelegate*>(this));
	return lab;
}

void LabelDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	QString value = index.model()->data(index).toString();
	QLabel *lab = static_cast<QLabel*>(editor);
	lab->setText(value);
}
void LabelDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	QLabel *lab = static_cast<QLabel*>(editor);
	QString sValue = lab->text();
	model->setData(index, sValue);

}

void LabelDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	editor->setGeometry(option.rect);
}


EditDelegate::EditDelegate(QObject *parent /* = 0 */) :QStyledItemDelegate(parent), m_nType(0)
{
	g_EditDelegate = this;
}
QWidget *EditDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QLineEdit *lineEdit = new QLineEdit(parent);
	//lineEdit->setStyleSheet("background:rgb(0,0,255);");
	if (m_nType == 0)
	{
		lineEdit->setValidator(new QIntValidator(0, 1000000));
	}
	else if (m_nType == 1)
	{
		QDoubleValidator *dValidator = new QDoubleValidator;
		dValidator->setRange(-1000, 1000, 2);
		lineEdit->setValidator(dValidator);
	}
	lineEdit->installEventFilter(const_cast<EditDelegate*>(this));
	return lineEdit;
}
void EditDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	QString value = index.model()->data(index/*, Qt::EditRole*/).toString();
	QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
	lineEdit->setText(value);
	
}
void EditDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
	QString sValue = lineEdit->text();
	QString sOldText = model->data(index).toString();
	model->setData(index, sValue);//, Qt::EditRole
	if (sOldText != sValue)
	{
		emit g_EditDelegate->signal_dataChanged(index.row(), index.column());
	}


}

void EditDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	editor->setGeometry(option.rect);
}


ComboboxDelegate::ComboboxDelegate(QObject *parent /* = 0 */) :QStyledItemDelegate(parent)
{
	g_CmbDelegate = this;
}
QWidget *ComboboxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QComboBox *pComb = new QComboBox(parent);
	pComb->setView(new QListView);
	g_CmbDelegate->m_map.insert(index.row(), pComb);
	pComb->addItems(m_list);
	pComb->installEventFilter(const_cast<ComboboxDelegate*>(this));
	return pComb;
}
void ComboboxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	QString value = index.model()->data(index).toString();
	QComboBox *cmb = static_cast<QComboBox*>(editor);
	int nIndex = cmb->findText(value);
	cmb->setCurrentIndex(nIndex);
}
void ComboboxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	QComboBox *cmb = static_cast<QComboBox*>(editor);
	QString sValue = cmb->currentText();
	QString sOld = model->data(index).toString();
	//为了解决cmb不选默认为0
	if (sValue.isEmpty() && sOld == "0")
	{
		return;
	}
	model->setData(index, sValue);
	if (sOld != sValue)
	{
		emit g_CmbDelegate->signal_combDataChanged(index.row(), cmb->currentIndex());//, index.column()
	}
	
}

void ComboboxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	editor->setGeometry(option.rect);
}


void ComboboxDelegate::setComboxData(QStringList sList)
{
	m_list = sList;
}


CheckBoxDelegate::CheckBoxDelegate(QObject *parent /* = 0 */) :QStyledItemDelegate(parent)
{
	g_CheckDelegate = this;
}
QWidget *CheckBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QCheckBox *chkBox = new QCheckBox(parent);
	chkBox->installEventFilter(const_cast<CheckBoxDelegate*>(this));
	return chkBox;
}
void CheckBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	bool isChecked = index.model()->data(index).toString() == GBUTF8("是") ? true : false;
	QCheckBox *chkBox = static_cast<QCheckBox*>(editor);
	chkBox->setChecked(isChecked);
}
void CheckBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	QCheckBox *chkBox = static_cast<QCheckBox*>(editor);
	bool isChecked = chkBox->isChecked();
	QString sValue = GBUTF8("否");
	if (isChecked)
	{
		sValue = GBUTF8("是");
	}
	QString sOld = model->data(index).toString();
	model->setData(index, sValue);
	if (sOld != sValue)
	{
		emit g_CheckDelegate->signal_dataChanged(index.row(), index.column());
	}

}

void CheckBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	editor->setGeometry(option.rect);
}

