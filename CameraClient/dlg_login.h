#ifndef DLG_LOGIN_H
#define DLG_LOGIN_H
//#include <windows.h>
#include <QtWidgets/QWidget>
#include "ui_dlg_login.h"
#include "search_sdk_define.h"
#include "subject.h"
#include <QTreeWidgetItem>
#include "drop_shadow_widget.h"
//#include <windows.h>
class Dlg_Login : public DropShadowWidget, public CObserver
{
	Q_OBJECT

public:
	Dlg_Login(QWidget *parent = 0);

	~Dlg_Login();

	void Init();

	void OnObserverNotify(LPARAM lHint, LPVOID pHint);

	void AddItem(TDevItem *ptDevItem);

public slots:

	void On_btnSearch_clicked();

	void slot_login_clicked();

	void slot_item_clicked(QTreeWidgetItem *pItem, int column);

	void slot_item_doubleClicked(QTreeWidgetItem *pItem, int column);
	
	void Slot_TitleButtonClicked();
private:
	Ui::Dlg_LoginClass ui;
};

#endif // DLG_LOGIN_H
