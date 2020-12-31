#pragma once

#include <QWidget>
#include <subject.h>
#include "ivhs_struct.h"
#include "define.h"
#include <QPixmap>
#include <QPainter>
#include <QLabel>
#include <QMouseEvent>
enum EMDRAWMATH
{
	EMDRAWMATH_GDI_ = 0, // GDI+
	EMDRAWMATH_IPICTRUE,
	EMDRAWMATH_COUNT
};

enum EMEVTTYPE
{
	EMEVTTYPE_NONE = 0,
	EMEVTTYPE_DRAW,
	EMEVTTYPE_MOVE,
	EMEVTTYPE_COUNT
};

enum ENSCLMODE
{
	// ԭͼ������ʾ
	EM_SCL_ORG = 0,
	// 16:9��ʾ
	EM_SCL_16_9,
	// 4:3��ʾ
	EM_SCL_4_3,
	// ȫ����ʾ
	EM_SCL_FULL
};

enum EMMOUSEEVTTYPE
{
	EMMOUSEEVTTYPE_NONE = 0,
	// ���ƶ�
	EMMOUSEEVTTYPE_PTMOVE,
	// �����ƶ�
	EMMOUSEEVTTYPE_AREAMOVE,
	// ����ƶ�
	EMMOUSEEVTTYPE_LEFTMOVE,
	// �ұ��ƶ�
	EMMOUSEEVTTYPE_RIGHTMOVE,
	// �ϱ��ƶ�
	EMMOUSEEVTTYPE_TOPMOVE,
	// �±��ƶ�
	EMMOUSEEVTTYPE_BOTTONMOVE,
	EMMOUSEEVTTYPE_COUNT
};


class Dlg_PictureDraw : public QWidget
{
	Q_OBJECT

public:
	Dlg_PictureDraw(QWidget *parent);

	~Dlg_PictureDraw();

	void SetDrawSize(int w, int h);

	bool OnRedrawAll();

	bool OnRedraw();

	QPixmap GetPixmap();

	void DeleteSelPoint();

	std::map<u32, tagDrawData*> GetDrawAllData();

	void AddPolygon(TIpcPolygon* pPolygon, int nType, int nId);

	void AddPolygon2Rect(TIpcPolygon* pPolygon, int nType, int nId);

	void GetLineOut(TLine_S* pLineOut, int nType);

	void GetRgnOut(TRoiArea_S* pRectOut, int nType);

	void GetPolygonOut(TIpcPolygon* pPolygon, int nType);

	bool OnSizeMouseMove(int x, int y);

	bool OnSizeButtonUp(int x, int y);

	bool CheckSizeState(int x, int y);

	bool OnSizeButtonDown(int x, int y);

	void SetPixSize(int w, int h);

	void AddRgn(TRoiArea_S* pRect, int nType, int nId);

	void AddRgn4(TRoiArea_S* pRect, int nType, int nId);

	void SetIsDrawShade(bool b){ m_bDrawShade = b; }

	void TransDataOut(tagDrawData* pOut);

	void AddTrapezium(TECHELON_S* pArea, int nType, int nId);

	bool IsDrawing(){ return m_tDrawDataCur.nPtNum > 0; }

	bool IsInBorder(int x, int y, tagDrawData* pData, u32& dwBorderType);
	bool IsInLineSegment(int x, int y, int x1, int y1, int x2, int y2);
	bool IsInCenter(int x, int y, tagDrawData* pData);
	bool IsInPt(int x, int y, tagDrawData* pData, int& nPtIndexOut);
	tagDrawData* GetCurDrawData();

	void SetHideDrawData(int nType, bool isHide);

	void AddLine(TLine_S* pLine, int nType, int nId, Uint32 nAttr = 0);

	void SetAddLineAttr(int nType, Uint32 attr);

	void DrawLine(HDC hdc, tagDrawData* pData, bool bDrawMove = false, bool isSelect = false);

	void DrawRect(HDC hdc, tagDrawData* pData);

	void DrawPoint(HDC hdc, tagDrawData* pData, bool bDrawMove = false);

	void DrawShade(HDC hdc, tagDrawData* pData);

	void DrawTxtId(HDC hdc, tagDrawData* pData);

	void DrawSpecific(HDC hdc, tagDrawData* pData, bool bDrawMove = false); // ������Ч���������Ҫ����

	void DrawRect(QPainter *painter, tagDrawData* pData);

	void DrawSpecific(QPainter *painter, tagDrawData* pData, bool bDrawMove = false); // ������Ч���������Ҫ����

	void DrawShade(QPainter* painter, tagDrawData* pData);

	void DrawPoint(QPainter *painter, tagDrawData* pData, bool bDrawMove = false);

	void CalcPosData(u8 byShowScaleType);

	bool TransDataAll(int nWOld, int nHOld, int nWNew, int nHNew);

	void RefreshLocalCoefficient();

	void GetRectOut(TRECTANGLE_S* pRectOut, int nType);

	void SetNewPic2(u32 dwId, int nType, bool bMjpeg = false);

	//void SetNewPic(u32 dwId, bool bMjpeg);

	void DelDrawData(int nType);

	COLORREF GetDrawClr(tagDrawData* pData);

	void SetColorByType(int type, int color);

	int  GetSelType(){ return m_nCurSelType; }

	void TransDataIn(tagDrawData* pOut);

	void TransData(tagDrawData* pIn, int nW1, int nH1, tagDrawData* pOut, int nW2, int nH2);

	void AddDrawData(tagDrawData* pData, int nType);

	bool GetDrawData(int nType, tagDrawData* pData);

	void GetTrapeziumOut(TECHELON_S* pAreaOut, int nType);
	
	void PaintBkImage(HDC hdc);

	void DrawGraph(HDC hdc);

	void OnDrawStop();

	void DelDrawDataAll();

	void AddRect(TRECTANGLE_S* pRect, int nType, int nId = -1);

	void Clear();

	bool SetSelType(int nSel);

	void DrawLine(QPainter *painter, tagDrawData* pData, bool bDrawMove = false, bool isSelect = false);

	void DrawTxtId(QPainter *painter, tagDrawData* pData);

	// ��ȡ��ǰ����ͼ������
	int  GetSelDrawMode(){ return m_nDrawingMode; }

	void SetCoefficient(int nW, int nH);

	void paintEvent(QPaintEvent *event);

	void mousePressEvent(QMouseEvent *event);

	void mouseReleaseEvent(QMouseEvent *event);

	void mouseMoveEvent(QMouseEvent *event);

signals:
	void signal_dataDrawChanged();

	void signal_popAttrShow();
private:
	float m_fScaleWidth;
	float m_fScaleHeight;
	QPixmap m_Pixmap;
	QPixmap m_srcPixmap;//ԭͼ
	int	m_nWCoeff, m_nHCoeff, m_nWCoeffOuter, m_nHCoeffOuter; // �ݺ����ת��ϵ��
	std::map<u32, tagDrawData*> m_mapDrawData;
	std::map<u32, tagDrawData*> m_mapTempDrawData;
	int m_nCurSelType;
	tagDrawData	m_tDrawDataCur;
	u32         m_dwEvtType;		// �¼����ͣ� 0���ޣ�1�� ���ƣ� 2���ƶ�
	int			m_nDrawingMode;
	// Image ͼ�� ,����;
	HDC			m_hMemDC;
	HBITMAP     m_bmpDraw;
	// ENSCLMODE ��������
	u8			m_byShowScaleType;
	// ͼƬ��С
	QSize		m_sizePic;

	bool m_bDrawShade;		// ����ѡ��������Ӱ
	u8 m_byMouseEvtType;

	int m_xMove;
	int m_yMove;
	int			m_nSizeP1;

	POINT		m_ptMouseDown;
	tagDrawData	m_tDrawDataOld;
	
};
