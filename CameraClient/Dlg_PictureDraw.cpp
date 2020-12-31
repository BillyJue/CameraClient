#include "Dlg_PictureDraw.h"
#include <QPainter>
#include "MgrData.h"
#include "global.h"
#include <QImage>
#include <QPen>
#include <math.h>
#include <QVector>
Dlg_PictureDraw::Dlg_PictureDraw(QWidget *parent)
	: QWidget(parent)
{
	m_fScaleWidth = 4;
	m_fScaleHeight = 4;
	m_nWCoeffOuter = 1024;
	m_nHCoeffOuter = 720;
	m_nWCoeff = 1280;
	m_nHCoeff = 720;
	m_nCurSelType = -1;
	m_dwEvtType = EMEVTTYPE_NONE;
	m_nDrawingMode = EMDRAWMODE_NONE;
	m_byShowScaleType = EM_SCL_FULL;
	m_bDrawShade = true;		// 绘制选中区域阴影
	m_byMouseEvtType = EMMOUSEEVTTYPE_NONE;
	m_sizePic.setWidth(-1);
	m_sizePic.setHeight(-1);
	setPalette(QPalette(Qt::gray));
	this->setMouseTracking(true);
	setAutoFillBackground(true);
	setCursor(QCursor(Qt::ArrowCursor));
}

void Dlg_PictureDraw::SetDrawSize(int w, int h)
{
	resize(w, h);
	update();
}

void Dlg_PictureDraw::SetPixSize(int w, int h)
{
	m_sizePic.setWidth(w);
	m_sizePic.setHeight(h);
}

//void Dlg_PictureDraw::SetNewPic(u32 dwId, bool bMjpeg)
//{
//
//	TImageBuf* pImageBuf;
//	if (bMjpeg)
//	{
//		pImageBuf = MgrData::getInstance()->GetMgrMjpegCtrl()->GetAndLockImageBuf(dwId);
//	}
//	else
//	{
//		pImageBuf = MgrData::getInstance()->GetMgrEvtLst()->GetAndLockImageBuf(dwId);
//	}
//
//	if (pImageBuf == NULL)
//	{
//		if (m_pIPicture != NULL)
//		{
//			m_pIPicture->Release();
//			m_pIPicture = NULL;
//		}
//
//		if (m_pImagePic != NULL)
//		{
//			delete m_pImagePic;
//			m_pImagePic = NULL;
//		}
//
//		m_dwPicId = dwId;
//		m_pMgrData->GetMgrEvtLst()->UnlockImageBuf();
//		return;
//	}
//
//	// 取内存中的数据
//	int nShow = 0;
//	if (nShow < (int)pImageBuf->dwBufNum)
//	{
//		bool bSet = false;
//		SIZE sz;
//		if (m_nDrowMath == EMDRAWMATH_IPICTRUE)
//		{
//			//if(m_pIPicture == NULL)
//			{
//				bSet = true;
//				JPG2IPic(pImageBuf->apBuf[nShow], pImageBuf->anBufLen[nShow], m_hIPictureMem, &m_pIPicture, sz, m_szPicHimetric);
//			}
//		}
//		else
//		{
//			//if(m_pImagePic == NULL)
//			{
//				bSet = true;
//				JPG2Image(pImageBuf->apBuf[nShow], pImageBuf->anBufLen[nShow], m_hImageMemPic, &m_pImagePic, sz);
//			}
//		}
//
//		if (bSet && (sz.cx != m_sizePic.cx || sz.cy != m_sizePic.cy))
//		{
//			m_sizePic.cx = sz.cx; m_sizePic.cy = sz.cy;
//
//			// 原始大小
//			if (m_byShowScaleType == EM_SCL_ORG)
//				CalcPosData(m_byShowScaleType);
//
//			//m_dwPicId = dwId;
//		}
//
//		if (bSet)
//		{
//			m_dwPicId = dwId;
//		}
//
//	}
//
//	m_pMgrData->GetMgrEvtLst()->UnlockImageBuf();
//}



Dlg_PictureDraw::~Dlg_PictureDraw()
{

}

void Dlg_PictureDraw::Clear()
{
	DelDrawDataAll();
};

void Dlg_PictureDraw::DelDrawDataAll()
{
	QMutex mutx;
	mutx.lock();
	std::map<u32, tagDrawData*>::iterator ite;
	ite = m_mapDrawData.begin();
	while (ite != m_mapDrawData.end())
	{
		if (ite->second != NULL)
		{
			delete ite->second;
			ite->second = NULL;
		}
		ite++;
	}
	m_mapDrawData.clear();
	mutx.unlock();
}




void Dlg_PictureDraw::SetCoefficient(int nW, int nH)
{
	if (nW == 0 || nH == 0)
	{
		return;
	}
	m_nWCoeffOuter = nW; m_nHCoeffOuter = nH;
}

COLORREF Dlg_PictureDraw::GetDrawClr(tagDrawData* pData)
{
	if (pData == NULL) return 0xFFFFFFFF;

	COLORREF clr;

	if (pData->color != 0)
	{
		clr = pData->color;
	}
	else
	{
		clr = COLOR_BLUE;
	}

	return clr;
}

void Dlg_PictureDraw::AddRgn(TRoiArea_S* pRect, int nType, int nId)
{
	tagDrawData tTmp;
	data2::Rgn2DrawData(pRect, &tTmp);
	//TransDataIn(&tTmp);
	tTmp.nId = nId;
	tTmp.color = 0;
	AddDrawData(&tTmp, nType);
}

void Dlg_PictureDraw::DeleteSelPoint()
{
	tagDrawData tData;
	std::map<u32, tagDrawData*>::iterator ite;
	ite = m_mapDrawData.find(m_nCurSelType);
	if (ite != m_mapDrawData.end())
	{
		ite->second->nPtNum = 0;
		for (int i = 0; i < ite->second->nPtNum; i++)
		{
			ite->second->x[i] = 0;
			ite->second->y[i] = 0;
		}
	}
	//this->update();
	
}

void Dlg_PictureDraw::AddRgn4(TRoiArea_S* pRect, int nType, int nId)
{
	tagDrawData tTmp;
	data2::Rgn42DrawData(pRect, &tTmp);
	tTmp.nId = nId;
	tTmp.color = 0;
	AddDrawData(&tTmp, nType);
}


bool Dlg_PictureDraw::OnRedraw()
{
	//1
	m_tDrawDataCur.Empty();

	if (m_nCurSelType == -1)
	{
		return false;
	}
	else if (m_nDrawingMode == EMDRAWMODE_NONE)
	{
		return false;
	}

	m_tDrawDataCur.nType = m_nCurSelType;
	m_tDrawDataCur.nDrawMode = m_nDrawingMode;

	tagDrawData *ptData = GetCurDrawData();
	if (ptData != NULL)
	{
		m_tDrawDataCur.nId = ptData->nId;
		ptData->clearPT();
	}

	// 
	update();

	return true;
}


bool Dlg_PictureDraw::OnRedrawAll()
{
	QMutex mutx;
	mutx.lock();
	tagDrawData* p;
	std::map<u32, tagDrawData*>::iterator ite;
	ite = m_mapDrawData.begin();
	while (ite != m_mapDrawData.end())
	{
		if (ite->second != NULL)
		{
			p = (tagDrawData*)ite->second;
			p->clearPT();
		}
		ite++;
	}
	mutx.unlock();

	return true;
}

void Dlg_PictureDraw::DrawLine(HDC hdc, tagDrawData* pData, bool bDrawMove, bool isSelect)
{
	COLORREF clr = GetDrawClr(pData);
	float fWidth = size().width();
	float fHeight = size().height();
	m_fScaleWidth = m_sizePic.width() / fWidth;
	m_fScaleHeight = m_sizePic.height() / fHeight;
	HPEN hPen;
	if (isSelect)
	{
		hPen = ::CreatePen(PS_SOLID, 4, RGB(GetBValue(clr), GetGValue(clr), GetRValue(clr)));
	}
	else
	{
		hPen = ::CreatePen(PS_SOLID, 2, RGB(GetBValue(clr), GetGValue(clr), GetRValue(clr)));
	}
	HPEN hPenOld = (HPEN)::SelectObject(hdc, hPen);

	for (int i = 0; i < pData->nPtNum; i++)
	{
		if (i >= MAXDRAWPOINTS) break;

		if (i == 0)
		{
			::MoveToEx(hdc, pData->x[i] /m_fScaleWidth, pData->y[i]/m_fScaleHeight, NULL);
		}
		else
		{
			::LineTo(hdc, pData->x[i] / m_fScaleWidth, pData->y[i] / m_fScaleHeight);
		}
	}

	// 封闭
	if (!bDrawMove &&
		((pData->nDrawMode == EMDRAWMODE_RGN_4 && pData->nPtNum == 4)
		|| (pData->nDrawMode == EMDRAWMODE_RECT && pData->nPtNum == 4)
		|| (pData->nDrawMode == EMDRAWMODE_RGN && pData->nPtNum >= 4)
		|| (pData->nDrawMode == EMDRAWMODE_TRAPEZIUM && pData->nPtNum == 4))
		)
	{
		::LineTo(hdc, pData->x[0] / m_fScaleWidth, pData->y[0] / m_fScaleHeight);
	}

	if (bDrawMove && pData->nPtNum > 0 && pData->nDrawMode != EMDRAWMODE_RECT)
	{
		::LineTo(hdc, m_xMove, m_yMove);
	}

	::SelectObject(hdc, hPenOld);
	DeleteObject(hPen);
}

void Dlg_PictureDraw::DrawLine(QPainter *painter, tagDrawData* pData, bool bDrawMove, bool isSelect)
{
	float fWidth = size().width();
	float fHeight = size().height();
	m_fScaleWidth = m_sizePic.width() / fWidth;
	m_fScaleHeight = m_sizePic.height() / fHeight;

	COLORREF clr = GetDrawClr(pData);
	QPen ppen;
	ppen.setColor(clr);
	ppen.setStyle(Qt::SolidLine);
	int nPenWidth = 2;
	if (isSelect)
	{
		nPenWidth = 4;
	}
	ppen.setWidth(nPenWidth);
	ppen.setWidth(4);
	painter->setPen(ppen);
	
	int nTempX = 0;
	int nTempY = 0;
	for (int i = 0; i < pData->nPtNum-1; i++)
	{
		if (i >= MAXDRAWPOINTS) break;
		//dddddddddd
		painter->drawLine(pData->x[i] / m_fScaleWidth, pData->y[i] / m_fScaleHeight, pData->x[i + 1] / m_fScaleWidth, pData->y[i + 1] / m_fScaleHeight);
		int nW = size().width();
		int nH = size().height();
		
	}
	nTempX = pData->x[pData->nPtNum - 1];
	nTempY = pData->y[pData->nPtNum - 1];

	
	// 封闭
	if (!bDrawMove &&
		((pData->nDrawMode == EMDRAWMODE_RGN_4 && pData->nPtNum == 4)
		|| (pData->nDrawMode == EMDRAWMODE_RECT && pData->nPtNum == 4)
		|| (pData->nDrawMode == EMDRAWMODE_RGN && pData->nPtNum >= 4)
		|| (pData->nDrawMode == EMDRAWMODE_TRAPEZIUM && pData->nPtNum == 4))
		)
	{
		nTempY = pData->x[0];
		nTempY = pData->y[0];
		painter->drawLine(pData->x[pData->nPtNum - 1] / m_fScaleWidth, pData->y[pData->nPtNum - 1] / m_fScaleHeight, pData->x[0] / m_fScaleWidth, pData->y[0] / m_fScaleHeight);
	}


	if (bDrawMove && pData->nPtNum > 0 && pData->nDrawMode != EMDRAWMODE_RECT)
	{
		painter->drawLine(m_xMove / m_fScaleWidth, m_yMove / m_fScaleHeight, nTempX / m_fScaleWidth, nTempY / m_fScaleHeight);
	}
	
	

}

void Dlg_PictureDraw::DrawTxtId(HDC hdc, tagDrawData* pData)
{
	if (!pData || pData->nPtNum <= 0)
	{
		return;
	}
	
	bool bHadData = false;
	for (int i = 0; i < pData->nPtNum; i++)
	{
		if (pData->x[i] != 0 || pData->y[i] != 0)
		{
			bHadData = true;
			break;
		}
	}
	if (!bHadData || pData->nId == -1) return;


	COLORREF clr = GetDrawClr(pData);
	COLORREF clrOld = ::GetTextColor(hdc);
	::SetTextColor(hdc, clr);
	//////////////////////////////////////////////////////////////////////////
	wchar_t szTxt[33];
	wsprintf(szTxt, TEXT("%d"), pData->nId);

	int x, y;
	if (pData->nDrawMode == EMDRAWMODE_LINE)
	{
		x = pData->x[0] / m_fScaleWidth + (pData->x[1] - pData->x[0]) /( m_fScaleWidth*2) - 8;
		y = pData->y[0] / m_fScaleHeight + (pData->y[1] - pData->y[0])/(m_fScaleHeight * 2) - 8;
	}
	else
	{
		x = pData->x[0]/m_fScaleWidth + 1;
		y = pData->y[0]/m_fScaleHeight + 1;
	}
	wchar_t *pStr;
	if (pData->nId > 20000)
	{
		wsprintf(szTxt, TEXT("%d"), pData->nId - 20000);
		pStr = TEXT("行人检测区域");
		for (int k = 0; k < wcslen(pStr); k++)
		{
			::TextOut(hdc, x, y + 16 * k, pStr + k, 1);
		}
		::SetTextColor(hdc, clrOld);
		y -= 25;
	}
	else if (pData->nId > 10000)
	{
		wsprintf(szTxt, TEXT("%d"), pData->nId - 10000);
		pStr = TEXT("行人高度");
		for (int k = 0; k < wcslen(pStr); k++)
		{
			::TextOut(hdc, x, y + 16 * k, pStr + k, 1);
		}
		::SetTextColor(hdc, clrOld);
		y -= 25;
	}

	/*if (pData->nId == 9999 || pData->nId == 9998)
	{
	wchar_t *pStr = TEXT("行人检测区域");
	if (pData->nId == 9998)
	{
	pStr = TEXT("行人高度");
	}

	for (int k = 0; k < wcslen(pStr); k++)
	{
	::TextOut(hdc, x, y + 16 * k, pStr + k, 1);
	}
	::SetTextColor(hdc, clrOld);
	return;
	}
	*/

	::TextOut(hdc, x, y, szTxt, wcslen(szTxt));
	//////////////////////////////////////////////////////////////////////////
	::SetTextColor(hdc, clrOld);
}

void Dlg_PictureDraw::DrawTxtId(QPainter *painter, tagDrawData* pData)
{
	if (pData->nPtNum <= 0) return;

	bool bHadData = false;
	for (int i = 0; i < pData->nPtNum; i++)
	{
		if (pData->x[i] != 0 || pData->y[i] != 0)
		{
			bHadData = true;
			break;
		}
	}
	if (!bHadData || pData->nId == -1) return;

	if (pData->nId == 0)
	{
		int a = 1;
	}

	COLORREF clr = GetDrawClr(pData);

	//////////////////////////////////////////////////////////////////////////
	wchar_t szTxt[33];
	wsprintf(szTxt, TEXT("%d"), pData->nId);

	int x, y;
	if (pData->nDrawMode == EMDRAWMODE_LINE)
	{
		x = pData->x[0] + (pData->x[1] - pData->x[0]) / 2;
		y = pData->y[0] + (pData->y[1] - pData->y[0]) / 2;
	}
	else
	{
		x = pData->x[0] + 1;
		y = pData->y[0] + 1;
	}

	wchar_t *pStr;
	if (pData->nId > 20000)
	{
		wsprintf(szTxt, TEXT("%d"), pData->nId - 20000);
		pStr = TEXT("行人检测区域");
		for (int k = 0; k < wcslen(pStr); k++)
		{
			painter->drawText(x / m_fScaleWidth, y / m_fScaleHeight + 12+ 20 * k, QString::fromWCharArray(pStr + k, 1));
		}
		y -= 50;
	}
	else if (pData->nId > 10000)
	{
		wsprintf(szTxt, TEXT("%d"), pData->nId - 10000);
		pStr = TEXT("行人高度");
		for (int k = 0; k < wcslen(pStr); k++)
		{
			painter->drawText(x / m_fScaleWidth, y / m_fScaleHeight + 12 + 20 * k, QString::fromWCharArray(pStr + k, 1));
		}
		y -= 50;
	}

	/*if (pData->nId == 9999 || pData->nId == 9998)
	{
	wsprintf(szTxt, TEXT("%s"), TEXT("行人检测区域"));
	if (pData->nId == 9998)
	{
	wsprintf(szTxt, TEXT("%s"), TEXT("行人高度"));

	}
	}*/

	QPen pen = painter->pen();
	QFont ff = painter->font();
	ff.setPointSizeF(12);
	ff.setBold(true);
	painter->setFont(ff);

	pen.setWidth(6);

	pen.setColor(Qt::yellow);
	painter->setPen(pen);
	painter->drawText(x / m_fScaleWidth, y / m_fScaleHeight + 12, QString::fromWCharArray(szTxt));
}


void Dlg_PictureDraw::AddLine(TLine_S* pLine, int nType, int nId, Uint32 nPtr)
{
	tagDrawData tTmp;
	data2::Line2DrawData(pLine, &tTmp);
	tTmp.nId = nId;
	tTmp.color = 0;
	tTmp.attribute = nPtr;
	AddDrawData(&tTmp, nType);
}


void Dlg_PictureDraw::DrawGraph(HDC hdc)
{
	// 绘制时数据转换，避免存储反复转换的误差
	tagDrawData tData;
	bool isSelect = false;
	std::map<u32, tagDrawData*>::iterator ite;
	ite = m_mapDrawData.begin();
	while (ite != m_mapDrawData.end())
	{
		if (ite->second != NULL && ite->second->isHide == false)
		{
			if (ite->second->nType == m_nCurSelType)
			{
				isSelect = TRUE;
				memcpy(&tData, ite->second, sizeof(tagDrawData));
			}
			else
			{
				DrawLine(hdc, ite->second);
				DrawTxtId(hdc, ite->second);
			}
		}
		ite++;
	}

	if (isSelect)
	{
		DrawLine(hdc, &tData);
		DrawTxtId(hdc, &tData);
		DrawSpecific(hdc, &tData);
	}

	if (m_tDrawDataCur.nPtNum > 0)
	{
		if (m_nDrawingMode == EMDRAWMODE_RECT)
		{
			DrawRect(hdc, &m_tDrawDataCur);
		}
		else
		{
			DrawLine(hdc, &m_tDrawDataCur, true);
		}

		DrawTxtId(hdc, &m_tDrawDataCur);
		DrawSpecific(hdc, &m_tDrawDataCur, true);
	}
}

void Dlg_PictureDraw::GetPolygonOut(TIpcPolygon* pPolygon, int nType)
{
	tagDrawData tData;
	if (GetDrawData(nType, &tData))
	{
		TransDataOut(&tData);
		pPolygon->pointNum = tData.nPtNum;
		for (int i = 0; i < tData.nPtNum; i++)
		{
			pPolygon->point[i].X = tData.x[i];
			pPolygon->point[i].Y = tData.y[i];
		}
	}
	else
	{
		memset(pPolygon, 0, sizeof(TIpcPolygon));
	}
}

void Dlg_PictureDraw::GetRgnOut(TRoiArea_S* pRectOut, int nType)
{
	tagDrawData tData;
	if (GetDrawData(nType, &tData))
	{
		TransDataOut(&tData);
		data2::DrawData2Rgn(pRectOut, &tData);
	}
	else
	{
		memset(pRectOut, 0, sizeof(TRECTANGLE_S));
	}
}


void Dlg_PictureDraw::paintEvent(QPaintEvent *event)
{

	//m_Pixmap = m_Pixmap.scaled(480, 270, Qt::KeepAspectRatioByExpanding);
	QImage img = m_Pixmap.toImage();
	QSize s = img.size();
	m_sizePic = s;
	img = img.scaled(size().width(), size().height(), Qt::IgnoreAspectRatio);
	QPainter painter(this);
	painter.drawImage(QPoint(0, 0), img);

	// 绘制时数据转换，避免存储反复转换的误差
	tagDrawData tData;
	bool isSelect = false;
	std::map<u32, tagDrawData*>::iterator ite;
	ite = m_mapDrawData.begin();
	while (ite != m_mapDrawData.end())
	{
		if (ite->second != NULL && ite->second->isHide == false)
		{
			if (ite->second->nType == m_nCurSelType)
			{
				isSelect = TRUE;
				memcpy(&tData, ite->second, sizeof(tagDrawData));
			}
			else
			{

				DrawLine(&painter, ite->second);
				DrawTxtId(&painter, ite->second);
			}
		}
		ite++;
	}

	if (isSelect)
	{
		DrawLine(&painter, &tData);
		DrawTxtId(&painter, &tData);
		DrawSpecific(&painter, &tData);
	}

	if (m_tDrawDataCur.nPtNum > 0)
	{
		if (m_nDrawingMode == EMDRAWMODE_RECT)
		{
			DrawRect(&painter, &m_tDrawDataCur);
		}
		else
		{
			DrawLine(&painter, &m_tDrawDataCur, true);
		}

		DrawTxtId(&painter, &m_tDrawDataCur);
		DrawSpecific(&painter, &m_tDrawDataCur, true);
	}

	QWidget::paintEvent(event);
}


void Dlg_PictureDraw::DrawRect(HDC hdc, tagDrawData* pData)
{
	bool bHadData = false;
	for (int i = 0; i < pData->nPtNum; i++)
	{
		if (pData->x[i] != 0 || pData->y[i] != 0)
		{
			bHadData = true;
			break;
		}
	}
	if (!bHadData) return;


	COLORREF clr = GetDrawClr(pData);

	HPEN hPen = ::CreatePen(PS_SOLID, 2, RGB(GetBValue(clr), GetGValue(clr), GetRValue(clr)));
	HPEN hPenOld = (HPEN)::SelectObject(hdc, hPen);


	::MoveToEx(hdc, pData->x[0]/m_fScaleWidth, pData->y[0]/m_fScaleHeight, NULL);
	::LineTo(hdc, m_xMove, pData->y[0]/m_fScaleHeight);
	::LineTo(hdc, m_xMove, m_yMove);
	::LineTo(hdc, pData->x[0]/m_fScaleWidth, m_yMove);
	::LineTo(hdc, pData->x[0]/m_fScaleWidth, pData->y[0]/m_fScaleHeight);

	::SelectObject(hdc, hPenOld);
	::DeleteObject(hPen);
}


void Dlg_PictureDraw::DrawRect(QPainter *painter, tagDrawData* pData)
{
	bool bHadData = false;
	for (int i = 0; i < pData->nPtNum; i++)
	{
		if (pData->x[i] != 0 || pData->y[i] != 0)
		{
			bHadData = true;
			break;
		}
	}
	if (!bHadData) return;


	COLORREF clr = GetDrawClr(pData);
	QPen pen = painter->pen();
	pen.setStyle(Qt::SolidLine);
	pen.setWidth(2);
	pen.setColor(clr);
	painter->setPen(pen);
	painter->drawLine(m_xMove / m_fScaleWidth, pData->y[0] / m_fScaleHeight, pData->x[0] / m_fScaleWidth, pData->y[0] / m_fScaleHeight);
	painter->drawLine(m_xMove / m_fScaleWidth, pData->y[0] / m_fScaleHeight, m_xMove / m_fScaleWidth, m_yMove / m_fScaleHeight);
	painter->drawLine(m_xMove / m_fScaleWidth, m_yMove / m_fScaleHeight, pData->x[0] / m_fScaleWidth, m_yMove / m_fScaleHeight);
	painter->drawLine(pData->x[0] / m_fScaleWidth, m_yMove / m_fScaleHeight, pData->x[0] / m_fScaleWidth, pData->y[0] / m_fScaleHeight);

}

// 绘制特效，特殊的需要绘制
void Dlg_PictureDraw::DrawSpecific(HDC hdc, tagDrawData* pData, bool bDrawMove)
{
	// 绘制阴影
	DrawShade(hdc, pData);

	// 绘制线
	DrawLine(hdc, pData, bDrawMove, true);

	// 绘制点	
	DrawPoint(hdc, pData);
}


void Dlg_PictureDraw::DrawSpecific(QPainter*painter, tagDrawData* pData, bool bDrawMove)
{
	// 绘制阴影
	DrawShade(painter, pData);

	// 绘制线
	DrawLine(painter, pData, bDrawMove, true);

	// 绘制点	
	DrawPoint(painter, pData);
}



void Dlg_PictureDraw::DrawPoint(HDC hdc, tagDrawData* pData, bool bDrawMove)
{
	//if (pData->nDrawMode == EMDRAWMODE_RECT || !m_bDrawPoint) return;

	//COLORREF clr1 = GetDrawClr(pData);
	//Graphics graphics(hdc);

	//for (int i = 0; i < pData->nPtNum; i++)
	//{
	//	if (i >= MAXDRAWPOINTS) break;

	//	RECT rc1;
	//	rc1.left = pData->x[i] - POINT_R; rc1.right = pData->x[i] + POINT_R;
	//	rc1.top = pData->y[i] - POINT_R; rc1.bottom = pData->y[i] + POINT_R;


	//	// 是当前移动的点
	//	COLORREF clr = clr1;
	//	//if(m_byMouseEvtType == EMMOUSEEVTTYPE_PTMOVE && m_nSizeP1 == i + 1)
	//	//{
	//	//	clr = COLORLINEDRAWING;
	//	//}

	//	// 			SolidBrush mySolidBrush(Color(255, 255, 0, 0)); 
	//	// 			graphics.FillRectangle(&mySolidBrush, 20, 10, 80, 60);

	//	LinearGradientBrush myLinearGradientBrush(TransRect(rc1),
	//		Color(255, GetBValue(clr), GetGValue(clr), GetRValue(clr)), Color(255, GetBValue(clr), GetGValue(clr) - 128, GetRValue(clr)), WrapModeTile);
	//	graphics.FillEllipse(&myLinearGradientBrush, TransRect(rc1));
	//}

	//if (bDrawMove)
	//{
	//	RECT rc1;
	//	rc1.left = m_xMove - POINT_R; rc1.right = m_xMove + POINT_R;
	//	rc1.top = m_yMove - POINT_R; rc1.bottom = m_yMove + POINT_R;

	//	// 是当前移动的点
	//	//COLORREF clr = COLORLINEDRAWING;
	//	COLORREF clr = clr1;

	//	LinearGradientBrush myLinearGradientBrush(TransRect(rc1),
	//		Color(255, GetBValue(clr), GetGValue(clr), GetRValue(clr)), Color(255, GetBValue(clr), GetGValue(clr) - 128, GetRValue(clr)), WrapModeTile);
	//	graphics.FillEllipse(&myLinearGradientBrush, TransRect(rc1));
	//}

}

void Dlg_PictureDraw::DrawPoint(QPainter *painter, tagDrawData* pData, bool bDrawMove)
{
	//if (pData->nDrawMode == EMDRAWMODE_RECT || !m_bDrawPoint) return;

	COLORREF clr1 = GetDrawClr(pData);

	for (int i = 0; i < pData->nPtNum; i++)
	{
		if (i >= MAXDRAWPOINTS) break;

		QRect rc1;
		rc1.setLeft(pData->x[i]/m_fScaleWidth - POINT_R);
		rc1.setRight(pData->x[i] / m_fScaleWidth + POINT_R);
		rc1.setTop(pData->y[i]/m_fScaleHeight - POINT_R);
		rc1.setBottom(pData->y[i] / m_fScaleHeight + POINT_R);

		// 是当前移动的点
		COLORREF clr = clr1;
		painter->drawEllipse(QRect(rc1.left(), rc1.top(), rc1.right() - rc1.left(), rc1.bottom() - rc1.top()));
	}
	//先注释
	//if (bDrawMove)
	//{
	//	RECT rc1;
	//	rc1.left = m_xMove - POINT_R; rc1.right = m_xMove + POINT_R;
	//	rc1.top = m_yMove - POINT_R; rc1.bottom = m_yMove + POINT_R;

	//	// 是当前移动的点
	//	//COLORREF clr = COLORLINEDRAWING;
	//	COLORREF clr = clr1;

	//	LinearGradientBrush myLinearGradientBrush(TransRect(rc1),
	//		Color(255, GetBValue(clr), GetGValue(clr), GetRValue(clr)), Color(255, GetBValue(clr), GetGValue(clr) - 128, GetRValue(clr)), WrapModeTile);
	//	graphics.FillEllipse(&myLinearGradientBrush, TransRect(rc1));
	//}

}

void Dlg_PictureDraw::DrawShade(HDC hdc, tagDrawData* pData)
{
	//// 
	//if (!m_bDrawShade || m_dwEvtType == EMEVTTYPE_DRAW || m_byMouseEvtType != EMMOUSEEVTTYPE_AREAMOVE)
	//	return;

	//if (pData->nPtNum < 3)
	//	return;

	//COLORREF clr = GetDrawClr(pData);
	//Graphics graphics(hdc);

	//HatchBrush myHatchBrush(
	//	(HatchStyle)HatchStyleWideDownwardDiagonal,
	//	Color(128, GetBValue(clr), GetGValue(clr), GetRValue(clr)),
	//	Color(0, GetBValue(clr), GetGValue(clr), GetRValue(clr)));

	//Point apt[12];
	//for (int i = 0; i < pData->nPtNum; i++)
	//{
	//	apt[i].X = pData->x[i];
	//	apt[i].Y = pData->y[i];
	//}
	//graphics.FillPolygon(&myHatchBrush, apt, pData->nPtNum);

}


void Dlg_PictureDraw::DrawShade(QPainter* painter, tagDrawData* pData)
{
	// 
	if (!m_bDrawShade || m_dwEvtType == EMEVTTYPE_DRAW || m_byMouseEvtType != EMMOUSEEVTTYPE_AREAMOVE)
	{
		setCursor(QCursor(Qt::CrossCursor));
		return;
	}
		
	setCursor(QCursor(Qt::SizeAllCursor));
	if (pData->nPtNum < 3)
		return;

	COLORREF clr = GetDrawClr(pData);
	painter->setBrush(QBrush(clr, Qt::DiagCrossPattern));


	QPoint apt[12];
	for (int i = 0; i < pData->nPtNum; i++)
	{
		apt[i].setX(pData->x[i]/m_fScaleWidth);
		apt[i].setY(pData->y[i]/m_fScaleHeight);
	}
	painter->drawPolygon(apt, pData->nPtNum);

}



///

tagDrawData* Dlg_PictureDraw::GetCurDrawData()
{
	QMutex mutx;
	mutx.lock();
	std::map<u32, tagDrawData*>::iterator ite;
	ite = m_mapDrawData.find(m_nCurSelType);
	if (ite != m_mapDrawData.end())
	{
		return ite->second;
	}
	mutx.unlock();
	return NULL;
}

void Dlg_PictureDraw::SetAddLineAttr(int nType, Uint32 attr)
{
	tagDrawData *pData = NULL;
	std::map<u32, tagDrawData*>::iterator ite;
	ite = m_mapDrawData.find(nType);
	if (ite != m_mapDrawData.end())
	{
		pData = ite->second;
	}
	if (pData)
	{
		pData->attribute = attr;
	}
}

void Dlg_PictureDraw::SetHideDrawData(int nType, bool isHide)
{
	QMutex mutx;
	mutx.lock();
	std::map<u32, tagDrawData*>::iterator ite;
	ite = m_mapDrawData.find(nType);
	if (ite != m_mapDrawData.end())
	{
		ite->second->isHide = isHide;
	}
	mutx.unlock();
}

bool Dlg_PictureDraw::OnSizeButtonDown(int x, int y)
{
	int x1 = x, y1 = y;
	if (CheckSizeState(x, y))
	{
		m_dwEvtType = EMEVTTYPE_MOVE;
		m_ptMouseDown.x = x;
		m_ptMouseDown.y = y;
		RECT rc;
		bool bClip = false;

		if (m_byMouseEvtType == EMMOUSEEVTTYPE_AREAMOVE)
		{
			memcpy(&m_tDrawDataOld, GetCurDrawData(), sizeof(tagDrawData));

			rc = trag::GetPolygonRect(m_tDrawDataOld.x, m_tDrawDataOld.y, m_tDrawDataOld.nPtNum);
			bClip = true;
		}
		else if (m_byMouseEvtType == EMMOUSEEVTTYPE_PTMOVE)
		{
			tagDrawData* pData = GetCurDrawData();
			rc.left = pData->x[m_nSizeP1 - 1]; rc.right = pData->x[m_nSizeP1 - 1];
			rc.top = pData->y[m_nSizeP1 - 1]; rc.bottom = pData->y[m_nSizeP1 - 1];

			bClip = true;
		}
		else if (m_byMouseEvtType == EMMOUSEEVTTYPE_LEFTMOVE)
		{
			memcpy(&m_tDrawDataOld, GetCurDrawData(), sizeof(tagDrawData));
			rc = trag::GetPolygonRect(m_tDrawDataOld.x, m_tDrawDataOld.y, m_tDrawDataOld.nPtNum);
			bClip = true;
		}
		else if (m_byMouseEvtType == EMMOUSEEVTTYPE_RIGHTMOVE)
		{
			memcpy(&m_tDrawDataOld, GetCurDrawData(), sizeof(tagDrawData));
			rc = trag::GetPolygonRect(m_tDrawDataOld.x, m_tDrawDataOld.y, m_tDrawDataOld.nPtNum);
			bClip = true;
		}
		else if (m_byMouseEvtType == EMMOUSEEVTTYPE_TOPMOVE)
		{
			memcpy(&m_tDrawDataOld, GetCurDrawData(), sizeof(tagDrawData));
			rc = trag::GetPolygonRect(m_tDrawDataOld.x, m_tDrawDataOld.y, m_tDrawDataOld.nPtNum);
			bClip = true;
		}
		else if (m_byMouseEvtType == EMMOUSEEVTTYPE_BOTTONMOVE)
		{
			memcpy(&m_tDrawDataOld, GetCurDrawData(), sizeof(tagDrawData));
			rc = trag::GetPolygonRect(m_tDrawDataOld.x, m_tDrawDataOld.y, m_tDrawDataOld.nPtNum);
			bClip = true;
		}

		if (bClip)
		{
			setCursor(QCursor(Qt::SizeAllCursor));
		}

		return true;
	}

	return false;
}


void Dlg_PictureDraw::mousePressEvent(QMouseEvent *event)
{
	int x = event->pos().x();
	int y = event->pos().y();
	float fWidth = size().width();
	float fHeight = size().height();
	x = x* m_sizePic.width() / fWidth;
	y = y* m_sizePic.height() / fHeight;
	
	if (m_tDrawDataCur.nDrawMode == EMDRAWMODE_LINE && m_tDrawDataCur.nPtNum == 1)
	{
		emit signal_popAttrShow();
	}
	
	if (m_dwEvtType == EMEVTTYPE_NONE)
	{
		tagDrawData* pData = GetCurDrawData();
		if (pData == NULL)
		{
			return;
		}
	}
	if (!IsDrawing())
	{
		if (OnSizeButtonDown(x, y))
		{
			return;
		}

		m_dwEvtType = EMEVTTYPE_DRAW;
		setCursor(QCursor(Qt::CrossCursor));
	}
	// 首点，置状态，数据
	if (m_tDrawDataCur.nPtNum == 0)
	{
		m_tDrawDataCur.nType = m_nCurSelType;
		m_tDrawDataCur.nDrawMode = m_nDrawingMode;
		m_tDrawDataCur.nId = GetCurDrawData()->nId;
		m_tDrawDataCur.color = GetCurDrawData()->color;
		m_tDrawDataCur.attribute = GetCurDrawData()->attribute;
		// 
		SetHideDrawData(m_nCurSelType, true);
	}
	m_xMove = x;
	m_yMove = y;
	m_tDrawDataCur.addPT(x, y);
	this->update();
	if (m_tDrawDataCur.nDrawMode == EMDRAWMODE_LINE)
	{
		if (m_tDrawDataCur.nPtNum == 2)
		{
			AddDrawData(&m_tDrawDataCur, m_nCurSelType);
			OnDrawStop();
		}
	}

	else if (m_tDrawDataCur.nDrawMode == EMDRAWMODE_RGN_4)
	{

		if (m_tDrawDataCur.nPtNum == 4)
		{
			// // 在第一个点附近，重新点了一下,结束绘制
			if ((abs(x - m_tDrawDataCur.x[0]) < 10 && abs(y - m_tDrawDataCur.y[0]) < 10))
			{
				m_tDrawDataCur.delPT();
			}

			AddDrawData(&m_tDrawDataCur, m_nCurSelType);
			OnDrawStop();
		}
	}
	else if (m_tDrawDataCur.nDrawMode == EMDRAWMODE_TRAPEZIUM)
	{
		if (m_tDrawDataCur.nPtNum == 4)
		{
			int min;
			int i, j;
			int temp;

			for (i = 0; i < 4; i++)
			{
				min = i;
				for (j = i; j < 4; j++)
				{
					if (m_tDrawDataCur.y[j] < m_tDrawDataCur.y[i])
					{
						min = j;
					}
				}
				if (min != i)
				{
					temp = m_tDrawDataCur.y[i];
					m_tDrawDataCur.y[i] = m_tDrawDataCur.y[min];
					m_tDrawDataCur.y[min] = temp;

					temp = m_tDrawDataCur.x[i];
					m_tDrawDataCur.x[i] = m_tDrawDataCur.x[min];
					m_tDrawDataCur.x[min] = temp;
				}
			}

			if (m_tDrawDataCur.x[0] > m_tDrawDataCur.x[1])
			{
				temp = m_tDrawDataCur.y[0];
				m_tDrawDataCur.y[0] = m_tDrawDataCur.y[1];
				m_tDrawDataCur.y[1] = temp;

				temp = m_tDrawDataCur.x[0];
				m_tDrawDataCur.x[0] = m_tDrawDataCur.x[1];
				m_tDrawDataCur.x[1] = temp;
			}

			if (m_tDrawDataCur.x[2] < m_tDrawDataCur.x[3])
			{
				temp = m_tDrawDataCur.y[2];
				m_tDrawDataCur.y[2] = m_tDrawDataCur.y[3];
				m_tDrawDataCur.y[3] = temp;

				temp = m_tDrawDataCur.x[2];
				m_tDrawDataCur.x[2] = m_tDrawDataCur.x[3];
				m_tDrawDataCur.x[3] = temp;
			}

			m_tDrawDataCur.y[1] = m_tDrawDataCur.y[0];
			m_tDrawDataCur.y[2] = m_tDrawDataCur.y[3];

			AddDrawData(&m_tDrawDataCur, m_nCurSelType);
			OnDrawStop();
			emit signal_dataDrawChanged();
			//m_pMgrData->Notify(NULL, NMC_GET_IPC_DRAW_CHANGE, (LPVOID)this);
		}
	}
	else if (m_tDrawDataCur.nDrawMode == EMDRAWMODE_RGN)
	{
		switch (m_tDrawDataCur.nPtNum)
		{
		case 1:
		case 2:
		case 3:
		case 4:
			break;
		case 5:
		case 6:
		case 7:
		{
			// 在第一个点附近，结束绘制
			if ((abs(x - m_tDrawDataCur.x[0]) < 10 && abs(y - m_tDrawDataCur.y[0]) < 10))
			{
				m_tDrawDataCur.delPT();
				AddDrawData(&m_tDrawDataCur, m_nCurSelType);
				OnDrawStop();
			}
		}
			break;
		case 8:
		{
			if ((abs(x - m_tDrawDataCur.x[0]) < 10 && abs(y - m_tDrawDataCur.y[0]) < 10))
			{
				m_tDrawDataCur.delPT();
			}
			AddDrawData(&m_tDrawDataCur, m_nCurSelType);
			OnDrawStop();
		}
			break;
		default:
			OnDrawStop();
			break;
		}
	}


}


bool Dlg_PictureDraw::OnSizeButtonUp(int x, int y)
{
	// 移动中
	if (m_dwEvtType == EMEVTTYPE_MOVE)
	{
		m_byMouseEvtType = EMMOUSEEVTTYPE_NONE;
		m_nSizeP1 = 0;
		m_dwEvtType = EMEVTTYPE_NONE;
		setCursor(QCursor(Qt::SizeAllCursor));
		emit signal_dataDrawChanged();
	}

	CheckSizeState(x, y);

	return false;
}

void Dlg_PictureDraw::mouseReleaseEvent(QMouseEvent *event)
{
	int x = event->pos().x();
	int y = event->pos().y();
	float fWidth = size().width();
	float fHeight = size().height();
	x = x* m_sizePic.width() / fWidth;
	y = y* m_sizePic.height() / fHeight;
	if (IsDrawing())
	{
		if (m_tDrawDataCur.nDrawMode == EMDRAWMODE_RECT)
		{
			// 先转换
			
			int x1 = m_tDrawDataCur.x[0];
			int y1 = m_tDrawDataCur.y[0];

			// 第一个点添加到最后，形成封闭形状
			m_tDrawDataCur.clearPT();
			m_tDrawDataCur.addPT(x1, y1);
			m_tDrawDataCur.addPT(x, y1);
			m_tDrawDataCur.addPT(x, y);
			m_tDrawDataCur.addPT(x1, y);
			if (x == x1 && y == y1)
			{
				SetHideDrawData(m_nCurSelType, false);
			}
			else
			{
				AddDrawData(&m_tDrawDataCur, m_nCurSelType);
				emit signal_dataDrawChanged();
			}
			OnDrawStop();
		}
	}
	else
	{
		OnSizeButtonUp(x, y);
	}
	QWidget::mouseReleaseEvent(event);
}

static bool isInPolygon(QPoint checkPoint, QVector<QPoint> &polygonPoints) {
	int counter = 0;
	int i;
	int xinters;
	QPoint p1, p2;
	int pointCount = polygonPoints.size();
	p1 = polygonPoints[0];

	for (i = 1; i <= pointCount; i++) {
		p2 = polygonPoints[i % pointCount];
		if (
			checkPoint.x() > min(p1.x(), p2.x()) &&
			checkPoint.x() <= max(p1.x(), p2.x())
			) {
			if (checkPoint.y() <= max(p1.y(), p2.y())) {
				if (p1.x() != p2.x()) {
					xinters =
						(checkPoint.x() - p1.x()) *
						(p2.y() - p1.y()) /
						(p2.x() - p1.x()) + p1.y();
					if (p1.y() == p2.y() || checkPoint.y() <= xinters) {
						counter++;
					}
				}
			}
		}
		p1 = p2;
	}
	if (counter % 2 == 0) {
		return false;
	}
	else {
		return true;
	}
}

bool Dlg_PictureDraw::IsInBorder(int x, int y, tagDrawData* pData, u32& dwBorderType)
{
	// 上边
	if (IsInLineSegment(x, y, pData->x[0], pData->y[0], pData->x[1], pData->y[1]))
	{
		dwBorderType = EMMOUSEEVTTYPE_TOPMOVE;
		return true;
	}

	// 右边
	if (IsInLineSegment(x, y, pData->x[1], pData->y[1], pData->x[2], pData->y[2]))
	{
		dwBorderType = EMMOUSEEVTTYPE_RIGHTMOVE;
		return true;
	}

	// 下边
	if (IsInLineSegment(x, y, pData->x[3], pData->y[3], pData->x[2], pData->y[2]))
	{
		dwBorderType = EMMOUSEEVTTYPE_BOTTONMOVE;
		return true;
	}

	// 左边
	if (IsInLineSegment(x, y, pData->x[0], pData->y[0], pData->x[3], pData->y[3]))
	{
		dwBorderType = EMMOUSEEVTTYPE_LEFTMOVE;
		return true;
	}

	return false;
}

bool Dlg_PictureDraw::IsInLineSegment(int x, int y, int x1, int y1, int x2, int y2)
{
	int max_X;
	int min_X;
	int max_Y;
	int min_Y;

	if (x1 < x2)
	{
		max_X = x2;
		min_X = x1;
	}
	else
	{
		max_X = x1;
		min_X = x2;
	}

	if (y1 < y2)
	{
		max_Y = y2;
		min_Y = y1;
	}
	else
	{
		max_Y = y1;
		min_Y = y2;
	}

	if (x1 == x2)
	{
		if (y > min_Y && y < max_Y)
		{
			if (x < (x1 + SIZBOX_BORDER) && x >(x1 - SIZBOX_BORDER))
			{
				return true;
			}
		}
	}
	else if (y1 == y2)
	{
		if (x > min_X && x < max_X)
		{
			if (y < (y1 + SIZBOX_BORDER) && y >(y1 - SIZBOX_BORDER))
			{
				return true;
			}
		}
	}
	else
	{
		if (y > min_Y && y < max_Y && x > min_X && x < max_X)
		{
			int value;
			float a;
			float b;
			a = (float)(y1 - y2) / (float)(x1 - x2);
			b = (float)y1 - (float)(a*x1);
			value = a*x + b;

			if (y < (value + SIZBOX_BORDER) && y >(value - SIZBOX_BORDER))
			{
				return true;
			}
		}
	}

	return false;
}

bool Dlg_PictureDraw::IsInCenter(int x, int y, tagDrawData* pData)
{
	if (pData == NULL)  return false;

	bool bRet = trag::ptInPolygon(pData->x, pData->y, pData->nPtNum, x, y);
	return bRet;
}

bool Dlg_PictureDraw::CheckSizeState(int x, int y)
{
	bool bIn = false;
	u32 dwState = (ULONG_PTR)IDC_CROSS;

	tagDrawData  tData;
	if (!GetDrawData(m_nCurSelType, &tData))
		return false;

	tagDrawData* pData = GetCurDrawData();
	if (pData == NULL)  return false;

	if (pData->nDrawMode == EMDRAWMODE_RECT)
	{
		u32 nBorderType;
		bIn = IsInBorder(x, y, pData, nBorderType);

		if (bIn)
		{
			m_byMouseEvtType = (u8)nBorderType;
			if (m_byMouseEvtType == EMMOUSEEVTTYPE_LEFTMOVE || m_byMouseEvtType == EMMOUSEEVTTYPE_RIGHTMOVE)
			{
				dwState = (ULONG_PTR)IDC_SIZEWE;
			}
			else if (m_byMouseEvtType == EMMOUSEEVTTYPE_TOPMOVE || m_byMouseEvtType == EMMOUSEEVTTYPE_BOTTONMOVE)
			{
				dwState = (ULONG_PTR)IDC_SIZENS;
			}
		}
		else
		{
			bIn = IsInCenter(x, y, pData);
			if (bIn)
			{
				dwState = (ULONG_PTR)IDC_SIZEALL;
				m_byMouseEvtType = EMMOUSEEVTTYPE_AREAMOVE;
			}
		}
	}
	else if (pData->nDrawMode == EMDRAWMODE_TRAPEZIUM)
	{
		u32 nBorderType;
		bIn = IsInBorder(x, y, pData, nBorderType);

		if (bIn)
		{
			m_byMouseEvtType = (u8)nBorderType;
			if (m_byMouseEvtType == EMMOUSEEVTTYPE_LEFTMOVE || m_byMouseEvtType == EMMOUSEEVTTYPE_RIGHTMOVE)
			{
				dwState = (ULONG_PTR)IDC_SIZEWE;
			}
			else if (m_byMouseEvtType == EMMOUSEEVTTYPE_TOPMOVE || m_byMouseEvtType == EMMOUSEEVTTYPE_BOTTONMOVE)
			{
				dwState = (ULONG_PTR)IDC_SIZENS;
			}
		}
		else
		{
			bIn = IsInCenter(x, y, pData);
			if (bIn)
			{
				dwState = (ULONG_PTR)IDC_SIZEALL;
				m_byMouseEvtType = EMMOUSEEVTTYPE_AREAMOVE;
			}
		}
	}
	else
	{
		int nPtIndex;
		bIn = IsInPt(x, y, pData, nPtIndex);
		if (bIn)
		{
			m_nSizeP1 = nPtIndex;
			m_byMouseEvtType = EMMOUSEEVTTYPE_PTMOVE;
			dwState = (ULONG_PTR)IDC_SIZEALL;
		}

		if (!bIn)
		{
			bIn = IsInCenter(x, y, pData);
			if (bIn)
			{
				dwState = (ULONG_PTR)IDC_SIZEALL;
				m_byMouseEvtType = EMMOUSEEVTTYPE_AREAMOVE;
			}
		}
	}

	if (!bIn)
	{
		m_byMouseEvtType = EMMOUSEEVTTYPE_NONE;
	}

	update();

	return bIn;
}

bool Dlg_PictureDraw::IsInPt(int x, int y, tagDrawData* pData, int& nPtIndexOut)
{
	if (pData == NULL)  return false;

	// 检查端点
	int nPt = 0;
	if (pData->nDrawMode != EMDRAWMODE_RECT)
	{
		for (int i = 0; i < pData->nPtNum; i++)
		{
			RECT rc1;
			rc1.left = pData->x[i] - POINT_R; rc1.right = pData->x[i] + POINT_R;
			rc1.top = pData->y[i] - POINT_R; rc1.bottom = pData->y[i] + POINT_R;

			POINT pt;
			pt.x = x; pt.y = y;

			if (PtInRect(&rc1, pt))
			{
				nPt = i + 1;
				break;
			}

		}
	}

	if (nPt > 0)
	{
		nPtIndexOut = nPt;
	}

	return (nPt != 0);
}

bool Dlg_PictureDraw::OnSizeMouseMove(int x, int y)
{
	// 移动中
	if (m_dwEvtType == EMEVTTYPE_MOVE)
	{

		if (m_byMouseEvtType == EMMOUSEEVTTYPE_AREAMOVE)
		{
			int xOffset = x - m_ptMouseDown.x;
			int yOffset = y - m_ptMouseDown.y;

			tagDrawData* pData = GetCurDrawData();
			for (int i = 0; i < pData->nPtNum; i++)
			{
				pData->x[i] = m_tDrawDataOld.x[i] + xOffset;
				pData->y[i] = m_tDrawDataOld.y[i] + yOffset;
			}

			update();
		}
		else if (m_byMouseEvtType == EMMOUSEEVTTYPE_PTMOVE)
		{
			tagDrawData* pData = GetCurDrawData();
			pData->x[m_nSizeP1 - 1] = x;
			pData->y[m_nSizeP1 - 1] = y;

			update();
		}
		else  if (m_byMouseEvtType == EMMOUSEEVTTYPE_LEFTMOVE)
		{
			int xOffset = x - m_ptMouseDown.x;

			tagDrawData* pData = GetCurDrawData();
			pData->x[0] = m_tDrawDataOld.x[0] + xOffset;
			pData->x[3] = m_tDrawDataOld.x[3] + xOffset;

			update();
		}
		else  if (m_byMouseEvtType == EMMOUSEEVTTYPE_RIGHTMOVE)
		{
			int xOffset = x - m_ptMouseDown.x;

			tagDrawData* pData = GetCurDrawData();
			pData->x[1] = m_tDrawDataOld.x[1] + xOffset;
			pData->x[2] = m_tDrawDataOld.x[2] + xOffset;

			update();
		}
		else  if (m_byMouseEvtType == EMMOUSEEVTTYPE_TOPMOVE)
		{
			int yOffset = y - m_ptMouseDown.y;

			tagDrawData* pData = GetCurDrawData();
			pData->y[0] = m_tDrawDataOld.y[0] + yOffset;
			pData->y[1] = m_tDrawDataOld.y[1] + yOffset;

			update();
		}
		else  if (m_byMouseEvtType == EMMOUSEEVTTYPE_BOTTONMOVE)
		{
			int yOffset = y - m_ptMouseDown.y;

			tagDrawData* pData = GetCurDrawData();
			pData->y[2] = m_tDrawDataOld.y[2] + yOffset;
			pData->y[3] = m_tDrawDataOld.y[3] + yOffset;

			update();
		}
	}
	else
	{
		CheckSizeState(x, y);
	}

	return false;
}



void Dlg_PictureDraw::mouseMoveEvent(QMouseEvent *event)
{
	
	int x = event->pos().x();
	int y = event->pos().y();
	float fWidth = size().width();
	float fHeight = size().height();
	x = x* m_sizePic.width() / fWidth;
	y = y* m_sizePic.height() / fHeight;
	if (IsDrawing())
	{
		m_xMove = x;
		m_yMove = y;
		update();
	}
	else
	{
		OnSizeMouseMove(x, y);
	}
	
}

void Dlg_PictureDraw::AddPolygon2Rect(TIpcPolygon* pPolygon, int nType, int nId)
{

	tagDrawData data;
	data.nDrawMode = EMDRAWMODE_RECT;
	data.nPtNum = 4;
	for (int i = 0; i < data.nPtNum; i++)
	{
		data.x[i] = pPolygon->point[i].X;
		data.y[i] = pPolygon->point[i].Y;
	}

	//TransDataIn(&data);
	data.nId = nId;
	data.color = 0;
	AddDrawData(&data, nType);
}

void Dlg_PictureDraw::AddPolygon(TIpcPolygon* pPolygon, int nType, int nId)
{
	tagDrawData data;
	data.nDrawMode = EMDRAWMODE_RGN;
	data.nPtNum = pPolygon->pointNum;
	for (int i = 0; i < data.nPtNum; i++)
	{
		data.x[i] = pPolygon->point[i].X;
		data.y[i] = pPolygon->point[i].Y;
	}
	data.nId = nId;
	data.color = 0;
	AddDrawData(&data, nType);
}

void Dlg_PictureDraw::AddTrapezium(TECHELON_S* pArea, int nType, int nId)
{
	tagDrawData data;
	data.nDrawMode = EMDRAWMODE_TRAPEZIUM;
	data.nPtNum = 4;
	data.x[0] = pArea->X1;
	data.y[0] = pArea->Y1;
	data.x[1] = pArea->X1 + pArea->W1;
	data.y[1] = pArea->Y1;
	data.x[2] = pArea->X2 + pArea->W2;;
	data.y[2] = pArea->Y2;
	data.x[3] = pArea->X2;
	data.y[3] = pArea->Y2;

	//TransDataIn(&data);
	data.nId = nId;
	data.color = 0;
	AddDrawData(&data, nType);
}

void Dlg_PictureDraw::OnDrawStop()
{
	//::ClipCursor(NULL);
	m_dwEvtType = EMEVTTYPE_NONE;

	m_tDrawDataCur.Empty();
	update();

	setCursor(QCursor(Qt::ArrowCursor));
	// 正在绘制，退出绘制状态
	//::ReleaseCapture();
}

bool  Dlg_PictureDraw::GetDrawData(int nType, tagDrawData* pData)
{
	QMutex mutx;
	mutx.lock();
	std::map<u32, tagDrawData*>::iterator ite;
	ite = m_mapDrawData.find(nType);
	if (ite != m_mapDrawData.end())
	{
		memcpy(pData, ite->second, sizeof(tagDrawData));
		mutx.unlock();
		return true;
	}
	mutx.unlock();
	return false;
}



void Dlg_PictureDraw::GetTrapeziumOut(TECHELON_S* pAreaOut, int nType)
{
	tagDrawData tData;
	if (GetDrawData(nType, &tData))
	{
		//TransDataOut(&tData);
		pAreaOut->X1 = tData.x[0];
		pAreaOut->Y1 = tData.y[0];
		pAreaOut->W1 = tData.x[1] - tData.x[0];
		pAreaOut->X2 = tData.x[3];
		pAreaOut->Y2 = tData.y[3];
		pAreaOut->W2 = tData.x[2] - tData.x[3];
	}
	else
	{
		memset(pAreaOut, 0, sizeof(TECHELON_S));
	}
}

static void DrawData2RectTemp(TRECTANGLE_S* pRect, tagDrawData* pData)
{
	int i = 0;

	if (pData == NULL || pRect == NULL) return;
	int nTempX, nTempY;
	memset(pRect, 0, sizeof(TRECTANGLE_S));
	if (pData->nDrawMode == EMDRAWMODE_RECT)
	{
		nTempX = pData->x[0];
		nTempY = pData->y[0];
		int nCross = -1;
		for (int i = 1; i < 4; i++)
		{
			if (nTempX != pData->x[i] && nTempY != pData->y[i])
			{
				nCross = i;
				break;
			}
		}


		int nMinX = pData->x[0];
		int nPos = 0;
		for (int i = 0; i < 4; i++)
		{
			if (nMinX > pData->x[i])
			{
				nMinX = pData->x[i];
				nPos = i;
			}
			for (int j = 0; j < 4; j++)
			{
				if (pData->y[nMinX] > pData->y[j])
				{
					break;
				}
			}
		}
		pRect->X = pData->x[nPos];
		pRect->Y = pData->y[nPos];
		pRect->W = abs(pData->x[nCross] - pData->x[0]);
		pRect->H = abs(pData->y[nCross] - pData->y[0]);
	}
}


void Dlg_PictureDraw::GetRectOut(TRECTANGLE_S* pRectOut, int nType)
{
	tagDrawData tData;
	if (GetDrawData(nType, &tData))
	{
		//TransDataOut(&tData);
		//DrawData2RectTemp(pRectOut, &tData);
		data2::DrawData2Rect(pRectOut, &tData);
	}
	else
	{
		memset(pRectOut, 0, sizeof(TRECTANGLE_S));
	}
}

// 转为外部数据
void Dlg_PictureDraw::TransDataOut(tagDrawData* pOut)
{
	// 转换为外部场景
	//TransData(pOut, m_nWCoeff, m_nHCoeff, NULL, m_nWCoeffOuter, m_nHCoeffOuter);
}

bool Dlg_PictureDraw::SetSelType(int nSel)
{
	// 取消选中
	if (-1 == nSel)
	{
		m_nCurSelType = -1;
		OnDrawStop();
	}
	// 选中
	else
	{
		tagDrawData tData;
		bool bRet = GetDrawData(nSel, &tData);
		if (bRet)
		{
			m_nCurSelType = nSel;
			m_nDrawingMode = tData.nDrawMode;
			m_dwEvtType = EMEVTTYPE_NONE;
		}
		else
		{
			m_nCurSelType = -1;
			m_dwEvtType = EMEVTTYPE_NONE;
			m_tDrawDataCur.Empty();
		}
	}

	update();
	// 高亮显示
	//Invalidate();

	return true;
}

// 将外部传输转换为绘制数据
bool Dlg_PictureDraw::TransDataAll(int nWOld, int nHOld, int nWNew, int nHNew)
{
	QMutex mutx;
	mutx.lock();

	// 相等或者宽高为0全部不转换
	if (nWOld == 0 || nHOld == 0 || nWNew == 0 || nHNew == 0 || (nWNew == nWOld && nHNew == nHOld))
	{
		return false;
	}

	// 转换为当前坐标
	tagDrawData* pTmp = NULL;
	std::map<u32, tagDrawData*>::iterator ite;
	ite = m_mapDrawData.begin();
	while (ite != m_mapDrawData.end())
	{
		TransData(ite->second, nWOld, nHOld, NULL, nWNew, nHNew);
		ite++;
	}

	return true;
}

//

// 刷新本地系数，宽高改变后需要调用
void  Dlg_PictureDraw::RefreshLocalCoefficient()
{
	QRect rcClient = this->rect();
	if (m_nWCoeff == 0 || m_nHCoeff == 0 || rcClient.width() == 0 || rcClient.height() == 0 || (m_nWCoeff == rcClient.width() && m_nHCoeff == rcClient.height()))
		return;

	bool bRet = TransDataAll(m_nWCoeff, m_nHCoeff, rcClient.width(), rcClient.height());
	if (bRet)
	{
		m_nWCoeff = rcClient.width();
		m_nHCoeff = rcClient.height();
		//CalcPosData();
	}
}

void Dlg_PictureDraw::CalcPosData(u8 byShowScaleType)
{
	//if (m_nWCoeff == 0 || m_nHCoeff == 0)
	//	return;

	//if (byShowScaleType == -1)
	//{
	//	byShowScaleType = m_byShowScaleType;
	//}
	//m_byShowScaleType = byShowScaleType;

	////
	//bool bHor = true;
	//int t = 0, b = 0, l = 0, r = 0;

	//switch (m_byShowScaleType)
	//{
	//case  EM_SCL_FULL:
	//{
	//	t = 0; b = 0; l = 0; r = 0;
	//}
	//	break;
	//case  EM_SCL_16_9:
	//{
	//	int w = 16;
	//	int h = 9;
	//	bHor = (float)w / (float)h > (float)m_nWCoeff / (float)m_nHCoeff;
	//	// 上下填充
	//	if (bHor)
	//	{
	//		l = 0; r = 0;

	//		int nTmp = (m_nHCoeff - m_nWCoeff * h / w) / 2;
	//		t = nTmp;
	//		b = nTmp;
	//	}
	//	else
	//	{
	//		t = 0; b = 0;

	//		int nTmp = (m_nWCoeff - m_nHCoeff * w / h) / 2;
	//		l = nTmp;
	//		r = nTmp;
	//	}
	//}
	//	break;
	//case  EM_SCL_ORG:
	//{
	//	int w = m_sizePic.cx;
	//	int h = m_sizePic.cy;
	//	bHor = (float)w / (float)h > (float)m_nWCoeff / (float)m_nHCoeff;
	//	// 上下填充
	//	if (bHor)
	//	{
	//		l = 0; r = 0;

	//		int nTmp = (m_nHCoeff - m_nWCoeff * h / w) / 2;
	//		t = nTmp;
	//		b = nTmp;
	//	}
	//	else
	//	{
	//		t = 0; b = 0;

	//		int nTmp = (m_nWCoeff - m_nHCoeff * w / h) / 2;
	//		l = nTmp;
	//		r = nTmp;
	//	}
	//}
	//	break;
	//}

	//m_rcPicDraw = CRect(l, t, m_nWCoeff - r, m_nHCoeff - b);
}



void Dlg_PictureDraw::PaintBkImage(HDC hdc)
{
	/*
	QRect rcClient = this->rect();
	if (m_nWCoeff != rcClient.width() && m_nHCoeff != rcClient.height())
	{
	if (m_hMemDC != NULL){ DeleteDC(m_hMemDC); m_hMemDC = NULL; };
	if (m_bmpDraw != NULL){ DeleteObject(m_bmpDraw); m_bmpDraw = NULL; };
	}

	RefreshLocalCoefficient();

	if (m_bmpDraw == NULL)
	{
	CreateBitmap(hdc);
	}

	// 绘制图片
	DrawPic(m_hMemDC);
	// 绘制区域线条
	DrawGraph(m_hMemDC);

	// 拷贝
	CRect rc1 = this->GetPos();
	CRect rc2 = TransToScreenData(m_rcPicDraw);
	rc2.Offset(rc1.left, rc1.top);
	//::StretchBlt(hdc, rc2.left, rc2.top,  rc2.Width(), rc2.Height(), m_hMemDC, 0, 0, m_nWCoeff, m_nHCoeff, SRCCOPY);
	::BitBlt(hdc, rc2.left, rc2.top, rc2.Width(), rc2.Height(), m_hMemDC, 0, 0, SRCCOPY);

	//
	*/

}





void Dlg_PictureDraw::AddDrawData(tagDrawData* pData, int nType)
{
	QMutex mutx;
	mutx.lock();
	// 转换为当前坐标
	tagDrawData* pTmp = NULL;
	std::map<u32, tagDrawData*>::iterator ite;
	ite = m_mapDrawData.find(nType);
	if (ite == m_mapDrawData.end())
	{
		pTmp = new tagDrawData;
		m_mapDrawData[nType] = pTmp;
	}
	else
	{
		pTmp = ite->second;
	}

	memcpy(pTmp, pData, sizeof(tagDrawData));
	pTmp->nType = nType;

	mutx.unlock();
	/*if (this->isVisible())
	{
	update();
	}*/
}

// 转为内部数据
void Dlg_PictureDraw::TransDataIn(tagDrawData* pOut)
{
	// 转换为当前坐标
	//TransData(pOut, m_nWCoeffOuter, m_nHCoeffOuter, NULL, m_nWCoeff, m_nHCoeff);
}

void Dlg_PictureDraw::TransData(tagDrawData* pIn, int nW1, int nH1, tagDrawData* pOut, int nW2, int nH2)
{
	if (nW1 == -1)
	{
		QRect rc = this->rect();
		nW1 = rc.width();
	}
	if (nH1 == -1)
	{
		QRect rc = this->rect();
		nH1 = rc.height();
	}
	if (nW2 == -1)
	{
		nW2 = m_nWCoeff;
	}
	if (nH2 == -1)
	{
		nH2 = m_nHCoeff;
	}

	if (nW1 == nW2 && nH1 == nH2)
	{
		if (pOut != NULL)
		{
			memcpy(pOut, pIn, sizeof(tagDrawData));
		}

		return;
	}

	if (pOut != NULL)
	{
		memcpy(pOut, pIn, sizeof(tagDrawData));
	}
	for (int i = 0; i < pIn->nPtNum; i++)
	{
		if (i >= MAXDRAWPOINTS) break;;

		int x = (pIn->x[i] == nW1) ? nW2 : pIn->x[i] * nW2 / nW1;
		pIn->x[i] = x;
		int y = (pIn->y[i] == nH1) ? nH2 : pIn->y[i] * nH2 / nH1;
		pIn->y[i] = y;
	}

}



void Dlg_PictureDraw::DelDrawData(int nType)
{
	QMutex mutx;
	mutx.lock();
	std::map<u32, tagDrawData*>::iterator ite;
	ite = m_mapDrawData.find(nType);
	if (ite != m_mapDrawData.end())
	{
		if (ite->second != NULL)
		{
			delete ite->second;
			ite->second = NULL;
		}

		m_mapDrawData.erase(ite);;
	}
	mutx.unlock();

	//this->update();
}

void Dlg_PictureDraw::GetLineOut(TLine_S* pLineOut, int nType)
{
	tagDrawData tData;
	if (GetDrawData(nType, &tData))
	{
		//TransDataOut(&tData);
		data2::DrawData2Line(pLineOut, &tData);
	}
	else
	{
		//memset(pLineOut, 0, sizeof(TLine_S));
	}
}

QPixmap Dlg_PictureDraw::GetPixmap()
{
	return m_srcPixmap;
}

std::map<u32, tagDrawData*> Dlg_PictureDraw::GetDrawAllData()
{
	m_mapTempDrawData.clear();
	std::map<u32, tagDrawData*>::iterator ite;
	ite = m_mapDrawData.begin();
	while (ite != m_mapDrawData.end())
	{
		tagDrawData* tmpData = new tagDrawData;
		memcpy(tmpData, ite->second, sizeof(tagDrawData));
		m_mapTempDrawData[ite->first] = tmpData;
		ite++;
	}
	return m_mapTempDrawData;
}
#include <QDebug>
void Dlg_PictureDraw::SetNewPic2(u32 dwId, int nType, bool bMjpeg)
{
	QMutex mutex;
	mutex.lock();
	TImageBuf* pImageBuf;
	if (bMjpeg)
	{
		qCritical() << "GetMgrMjpegCtrl getBuff";
		pImageBuf = MgrData::getInstance()->GetMgrMjpegCtrl()->GetAndLockImageBuf(dwId);
	}
	else
	{
		qCritical() << "GetMgrEvtLst getBuff";
		pImageBuf = MgrData::getInstance()->GetMgrEvtLst()->GetAndLockImageBuf(dwId);
	}
	if (pImageBuf == NULL)
	{
		return;
	}

	// 取内存中的数据
	int nShow = 0;

	if (nType != 0)
	{
		//nShow = 0xffff;
		for (int i = 0; i < (int)pImageBuf->dwBufNum; i++)
		{
			if (pImageBuf->anBufType[i] == nType)
			{
				nShow = i;
				break;
			}
		}
	}
	qCritical() << "test0000000000000000000" << nShow << pImageBuf->dwBufNum;
	if (nShow < (int)pImageBuf->dwBufNum)
	{
		m_Pixmap.loadFromData((const uchar *)pImageBuf->apBuf[nShow], pImageBuf->anBufLen[nShow]);
		m_srcPixmap.loadFromData((const uchar *)pImageBuf->apBuf[nShow], pImageBuf->anBufLen[nShow]);
		//m_srcPixmap = m_Pixmap;
	}
	if (bMjpeg)
	{
		
	}
	else
	{
		MgrData::getInstance()->GetMgrEvtLst()->UnlockImageBuf();
	}
	
	update();

}

void Dlg_PictureDraw::SetColorByType(int type, int color)
{
	tagDrawData tData;
	GetDrawData(type, &tData);
	tData.color = color;
	AddDrawData(&tData, type);
}

void Dlg_PictureDraw::AddRect(TRECTANGLE_S* pRect, int nType, int nId)
{
	tagDrawData tTmp;
	data2::Rect2DrawData(pRect, &tTmp);
	//TransDataIn(&tTmp);
	tTmp.nId = nId;
	tTmp.color = 0;
	AddDrawData(&tTmp, nType);
}

