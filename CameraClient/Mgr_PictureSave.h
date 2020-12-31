#pragma once

#include <QObject>
#include <QPainter>
#include <QWidget>
#include "define.h"
#include <QPixmap>
#define GBUTF8(da) QString::fromLocal8Bit(da)
class Mgr_PictureSave : public QWidget
{
	Q_OBJECT

public:
	Mgr_PictureSave(QWidget *parent);

	~Mgr_PictureSave();

	void paintEvent(QPaintEvent *event);

	void SetCurrentScenceIndex(int nIndex);

	void SavePicture();

	QString MakeMutiDir(const QString path);

	void ClearData();

	void SetDrawData(std::map<u32, tagDrawData*> &tempMap);

	void SetImage(QPixmap pImage);

	void AddDrawData(tagDrawData* pData, int nType);

	void AddRgn(TRoiArea_S* pRect, int nType, int nId);

	COLORREF GetDrawClr(tagDrawData* pData);

	void DrawLine(QPainter *painter, tagDrawData* pData, bool bDrawMove = false, bool isSelect = false, bool bLineColor = false);

	void DrawTxtId(QPainter *painter, tagDrawData* pData);

private:
	int m_nWCoeffOuter;
	int m_nHCoeffOuter;
	std::map<u32, tagDrawData*> m_mapDrawData;
	int m_nCurSelType;
	int m_nScenceIndex;
	QPixmap m_pixMap;
};
