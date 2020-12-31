#include "Mgr_PictureSave.h"
#include "global.h"
#include "MgrData.h"
#include <QDir>
#include <QDateTime>
Mgr_PictureSave::Mgr_PictureSave(QWidget *parent)
	: QWidget(parent)
{

}

Mgr_PictureSave::~Mgr_PictureSave()
{
	ClearData();
}

void Mgr_PictureSave::AddDrawData(tagDrawData* pData, int nType)
{
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

}

void Mgr_PictureSave::AddRgn(TRoiArea_S* pRect, int nType, int nId)
{
	tagDrawData tTmp;
	data2::Rgn2DrawData(pRect, &tTmp);
	tTmp.nId = nId;
	tTmp.color = 0;
	AddDrawData(&tTmp, nType);
}

void Mgr_PictureSave::SetImage(QPixmap pImage)
{
	m_pixMap = pImage;

}

void Mgr_PictureSave::ClearData()
{
	std::map<u32, tagDrawData*>::iterator ite;
	ite = m_mapDrawData.begin();
	while (ite != m_mapDrawData.end())
	{
		delete ite->second;
		ite->second = NULL;
		ite++;
	}
	m_mapDrawData.clear();
}

void Mgr_PictureSave::SetDrawData(std::map<u32, tagDrawData*> &tempMap)
{
	ClearData();
	std::map<u32, tagDrawData*>::iterator ite;
	ite = tempMap.begin();
	while (ite != tempMap.end())
	{
		m_mapDrawData[ite->first] = ite->second;
		ite++;
	}

}

QString Mgr_PictureSave::MakeMutiDir(const QString path){
	QDir dir(path);
	if (dir.exists(path)){
		return path;
	}
	QString parentDir = MakeMutiDir(path.mid(0, path.lastIndexOf('\\')));
	QString dirname = path.mid(path.lastIndexOf('\\') + 1);
	QDir parentPath(parentDir);
	if (!dirname.isEmpty())
		parentPath.mkpath(dirname);
	return parentDir + "\\" + dirname;
}

void Mgr_PictureSave::SavePicture()
{
	QPainter painter(&m_pixMap);
	tagDrawData tData;
	bool isSelect = false;
	tagDrawData pTagData;
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
				DrawLine(&painter, ite->second, false, false, true);
				DrawTxtId(&painter, ite->second);
			}
		}
		ite++;
	}

	if (isSelect)
	{
		DrawLine(&painter, &tData, false, false, true);
		DrawTxtId(&painter, &tData);
	}
	TLocalCfgInfo localcfg;
	MgrData::getInstance()->GetMgrLocalCfg()->GetLocalCfg(&localcfg);
	QString sPath = QString::fromWCharArray(localcfg.picturepath) + GBUTF8("\\违法停车配置");
	sPath = MakeMutiDir(sPath);
	QDateTime curDate = QDateTime::currentDateTime();
	QString sTime = curDate.toString("yyyy-MM-dd");
	u32 dwAddr = MgrData::getInstance()->GetIP();
	u8 *pch = (u8 *)&dwAddr;
	wchar_t szIPAddr[64] = { 0 };
	wsprintf(szIPAddr, TEXT("%d.%d.%d.%d"), *pch, *(pch + 1), *(pch + 2), *(pch + 3));
	QString sFileName = QString("%1_%2_%3.jpg").arg(QString::fromWCharArray(szIPAddr)).arg(sTime).arg(m_nScenceIndex + 1);
	sPath = QString("%1\\%2").arg(sPath).arg(sFileName);
	m_pixMap.save(sPath);
}


void Mgr_PictureSave::SetCurrentScenceIndex(int nIndex)
{
	m_nScenceIndex = nIndex;

}

COLORREF Mgr_PictureSave::GetDrawClr(tagDrawData* pData)
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

void Mgr_PictureSave::DrawLine(QPainter *painter, tagDrawData* pData, bool bDrawMove, bool isSelect, bool bLineColor)
{
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
	for (int i = 0; i < pData->nPtNum - 1; i++)
	{
		if (i >= MAXDRAWPOINTS) break;
		//dddddddddd
		painter->drawLine(pData->x[i], pData->y[i], pData->x[i + 1], pData->y[i + 1]);
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
		painter->drawLine(pData->x[pData->nPtNum - 1], pData->y[pData->nPtNum - 1], pData->x[0], pData->y[0]);
	}
}

void Mgr_PictureSave::DrawTxtId(QPainter *painter, tagDrawData* pData)
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
	if (pData->nId == 9999 || pData->nId == 9998)
	{
		wsprintf(szTxt, TEXT("%s"), TEXT("行人检测区域"));
		if (pData->nId == 9998)
		{
			wsprintf(szTxt, TEXT("%s"), TEXT("行人高度"));

		}
	}

	QPen pen = painter->pen();
	QFont ff = painter->font();
	ff.setPointSizeF(12);
	ff.setBold(true);
	painter->setFont(ff);

	pen.setWidth(6);

	pen.setColor(clr);
	painter->setPen(pen);
	painter->drawText(x, y, QString::fromWCharArray(szTxt));
}


void Mgr_PictureSave::paintEvent(QPaintEvent *event)
{
	QWidget::paintEvent(event);
}

