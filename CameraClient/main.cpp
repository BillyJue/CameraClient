
#include "dlg_login.h"
#include <QtWidgets/QApplication>
#include <stdio.h>
#include <stdlib.h>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include "Dlg_PlatComponent.h"

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
	QDateTime current_date_time = QDateTime::currentDateTime();
	QString sTime = current_date_time.toString("hh:ss:mm");
	QString txt;
	switch (type) {
	case QtDebugMsg:
		txt = QString("%1: %2\n").arg(sTime).arg(msg);
		break;
	case QtWarningMsg:
		txt = QString("%1: %2\n").arg(sTime).arg(msg);
		break;
	case QtCriticalMsg:
		txt = QString("%1: %2\n").arg(sTime).arg(msg);
		break;
	case QtFatalMsg:
		txt = QString("%1: %2\n").arg(sTime).arg(msg);
	}
	QString sFileName = current_date_time.toString("yyyy-MM-dd");
	QFile outputFile(QString("%1.txt").arg(sFileName));
	outputFile.open(QIODevice::WriteOnly | QIODevice::Append);
	QTextStream ts(&outputFile);  //
	ts << txt << endl;
}

int main(int argc, char *argv[])
{
	
	qInstallMessageHandler(myMessageOutput);
	QApplication a(argc, argv);
	QFile file(":/Dlg_Login/qss.qss");
	file.open(QFile::ReadOnly);
	QString str = QString(file.readAll());
	qApp->setStyleSheet(str);
	Dlg_Login  w;
	w.show();
	int nRet = a.exec();
	
	return 0;
}
