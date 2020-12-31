#pragma once

#include <QWidget>
#include <QPen>
#include <QBrush>
#include <Windows.h>
#include <QPixmap>
#include "Dlg_PictureDraw.h"
class Dlg_Video : public QWidget
{
	Q_OBJECT

public:
	Dlg_Video(QWidget *parent);

	~Dlg_Video();

	void SetPixSize(int w, int  h);

	void SetPicDrawSize(int w, int h);

	void ControlPlayVideo(bool is);

	void paintEvent(QPaintEvent *event);

	void DrawData(HDC hdc);

	void ClearDrawData();

	void SetParkScenceDrawData(TIPC_illgParkPara *ptParkScence);

	void SetScenceDrawData(TIPC_GetSceneCfgRsp *ptSeceneCfg);

	void SetLoopDrawData(TIPC_SetVideoLoopReq *ptVideoLoopInfo);

	QPaintEngine * paintEngine() const;

	bool SetSelType(int nSel);
public slots:
	void Slot_DataChanged();
signals:
	void signal_dataChanged();

private:
	Dlg_PictureDraw * m_pixDraw;
	bool m_bDraw;

	int m_nSceneWidth;
	int m_nSceneHeight;
};
