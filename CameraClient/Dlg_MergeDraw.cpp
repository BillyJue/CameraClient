#include "Dlg_MergeDraw.h"

Dlg_MergeDraw::Dlg_MergeDraw(QWidget *parent)
	: QWidget(parent)
{
	m_dwCurBorderColor = 0x00000000;
	m_dwDrawColor = 0x00ff7f27; // 橙色
	m_nLayoutNum = 0;
	m_nSelect = -1;
	m_nSelectNext = -1;
	m_nRGB[0] = RGB(255, 255, 255);
	m_nRGB[1] = RGB(255, 0, 0);
	m_nRGB[2] = RGB(255, 255, 0);
	m_nRGB[3] = RGB(0, 255, 0);
	m_nRGB[4] = RGB(0, 0, 255);
	m_nRGB[5] = 0x00A020F0;
	m_nRGB[6] = RGB(0, 0, 0);
	m_zeroColorIndex = 0;
	m_zeroPos = -1;
	m_bControlZero = false;

	setPalette(QPalette(Qt::white));
	this->setMouseTracking(true);
	setAutoFillBackground(true);
}

Dlg_MergeDraw::~Dlg_MergeDraw()
{

}

//拖动可能导致位置改变，最好重新获取一下
int Dlg_MergeDraw::GetZeroPos()
{
	m_zeroPos = -1;
	for (int i = 0; i < m_astrDrawText.size(); i++)
	{
		if (m_astrDrawText[i] == "0")
		{
			m_zeroPos = i;
			return m_zeroPos;
		}
	}
	return  m_zeroPos;
}

int   Dlg_MergeDraw::GetZeroIndex()
{
	if (m_zeroColorIndex == -1)
	{
		m_zeroColorIndex = 0;
	}
	return m_zeroColorIndex;
}
void Dlg_MergeDraw::SetZeroIndex(int sel)
{
	m_zeroColorIndex = sel;
}

// 转换开始坐标
void Dlg_MergeDraw::ConvertStartPos(int n1, int n2, int nStart, int a[], int max)
{
	int minUnit = n2 / max;
	minUnit = minUnit > 30 ? 30 : minUnit;
	int offset = (n2 - minUnit*n1) / 2;
	a[0] = nStart + offset;
	for (int i = 1; i < n1; i++)
	{
		a[i] = a[i - 1] + minUnit;
	}
}

// 转换结束坐标
void Dlg_MergeDraw::ConvertEndPos(int n1, int n2, int nStart, int a[], int max)
{
	int minUnit = n2 / max;
	minUnit = minUnit>30 ? 30 : minUnit;
	int offset = (n2 - minUnit*n1) / 2;
	a[0] = nStart + offset + minUnit;
	for (int i = 1; i < n1; i++)
	{
		a[i] = a[i - 1] + minUnit;
	}
}

// 坐标转换
void Dlg_MergeDraw::Convert(int w1, int w2, int h1, int h2, int xOffset, int yOffset)
{
	int i;
	int al[32], ar[32], at[32], ab[32];
	i = w1>h1 ? w1 : h1;
	ConvertStartPos(w1, w2, xOffset, al, i);
	ConvertEndPos(w1, w2, xOffset, ar, i);
	ConvertStartPos(h1, h2, yOffset, at, i);
	ConvertEndPos(h1, h2, yOffset, ab, i);

	for (i = 0; i < 81; i++)
	{
		m_arcLayout[i].left = 0;
		m_arcLayout[i].top = 0;
		m_arcLayout[i].right = 0;
		m_arcLayout[i].bottom = 0;
	}

	int nLen = w1 * h1;
	for (i = 0; i < nLen; i++)
	{
		m_arcLayout[i].left = al[i % w1];
		m_arcLayout[i].top = at[i / w1];
		m_arcLayout[i].right = ar[i % w1];
		m_arcLayout[i].bottom = ab[i / w1];
	}
}



void Dlg_MergeDraw::SetDrawText(QMap<int, QString> tempMap)
{
	m_zeroPos = -1;
	for (int i = 0; i < tempMap.size(); i++)
	{
		m_astrDrawText[i] = tempMap[i];
		if (m_astrDrawText[i] == "0")//&&(m_nLayoutNum == 4)
		{
			m_zeroPos = i;
		}
	}
	if (m_nLayoutNum == 3)
	{
		m_zeroPos = -1;
	}
	this->update();
}

void Dlg_MergeDraw::paintEvent(QPaintEvent *event)
{
	QBrush bBrush;
	bBrush.setColor(m_dwDrawColor);
	QPainter painter(this);
	//painter.setBrush(bBrush);
	for (int i = 0; i < m_nLayoutNum; i++)
	{
		if (m_nSelect != -1 && m_nSelect == i)
		{
			continue;
		}
		
		painter.fillRect(m_arcDrawLayout[i].left, m_arcDrawLayout[i].top,
			qAbs(m_arcDrawLayout[i].right - m_arcDrawLayout[i].left), qAbs(m_arcDrawLayout[i].bottom - m_arcDrawLayout[i].top), m_dwDrawColor);

		painter.drawRect(m_arcDrawLayout[i].left, m_arcDrawLayout[i].top,
			qAbs(m_arcDrawLayout[i].right - m_arcDrawLayout[i].left), qAbs(m_arcDrawLayout[i].bottom - m_arcDrawLayout[i].top));
	}
	

	if (m_nSelect != -1)
	{
		painter.fillRect(m_arcDrawLayout[m_nSelect].left, m_arcDrawLayout[m_nSelect].top,
			qAbs(m_arcDrawLayout[m_nSelect].right - m_arcDrawLayout[m_nSelect].left)
			, qAbs(m_arcDrawLayout[m_nSelect].bottom - m_arcDrawLayout[m_nSelect].top), 0xFFFF0000);

		painter.drawRect(m_arcDrawLayout[m_nSelect].left, m_arcDrawLayout[m_nSelect].top,
			qAbs(m_arcDrawLayout[m_nSelect].right - m_arcDrawLayout[m_nSelect].left)
			, qAbs(m_arcDrawLayout[m_nSelect].bottom - m_arcDrawLayout[m_nSelect].top));
	}
	if (m_zeroPos != -1)
	{
		painter.fillRect(m_arcDrawLayout[m_zeroPos].left, m_arcDrawLayout[m_zeroPos].top,
			qAbs(m_arcDrawLayout[m_zeroPos].right - m_arcDrawLayout[m_zeroPos].left)
			, qAbs(m_arcDrawLayout[m_zeroPos].bottom - m_arcDrawLayout[m_zeroPos].top), m_nRGB[m_zeroColorIndex]);

		painter.drawRect(m_arcDrawLayout[m_zeroPos].left, m_arcDrawLayout[m_zeroPos].top,
			qAbs(m_arcDrawLayout[m_zeroPos].right - m_arcDrawLayout[m_zeroPos].left)
			, qAbs(m_arcDrawLayout[m_zeroPos].bottom - m_arcDrawLayout[m_zeroPos].top));
	}
	
	QPen pen;
	pen.setColor(Qt::transparent);
	pen.setWidth(1);
	pen.setStyle(Qt::SolidLine);
	for (int i = 0; i < m_nLayoutNum; i++)
	{
		if (!m_astrDrawText[i].isEmpty())
		{
			painter.drawText(m_arcDrawLayout[i].left, m_arcDrawLayout[i].top, qAbs(m_arcDrawLayout[i].right - m_arcDrawLayout[i].left)
				, qAbs(m_arcDrawLayout[i].bottom - m_arcDrawLayout[i].top), Qt::AlignHCenter | Qt::AlignVCenter, m_astrDrawText[i]);
		}
	}
	/*QPen pen2;
	pen2.setColor(m_dwCurBorderColor);
	pen2.setWidth(1);
	pen2.setStyle(Qt::SolidLine);
	painter.setPen(pen2);
	painter.setBrush(Qt::NoBrush);
	for (int i = 0; i < m_nLayoutNum; i++)
	{
		painter.drawRect(m_arcDrawLayout[i].left, m_arcDrawLayout[i].top, m_arcDrawLayout[i].right, m_arcDrawLayout[i].bottom);
	}*/
}


void Dlg_MergeDraw::mousePressEvent(QMouseEvent *event)
{
	QPoint qpt = event->pos();
	POINT pt;
	pt.x = qpt.x();
	pt.y = qpt.y();

	QRect qrc = this->rect();
	m_bControlZero = false;
	for (int i = 0; i < m_nLayoutNum; i++)
	{
		if (PtInRect(&m_arcDrawLayout[i], pt) && (m_zeroPos == i))
		{
			m_zeroColorIndex++;
			m_zeroColorIndex = (m_zeroColorIndex % 7);
			m_bControlZero = false;
			break;

		}
	}

	for (int i = 0; i < m_nLayoutNum; i++)
	{
		if (PtInRect(&m_arcDrawLayout[i], pt))
		{
			m_nSelect = i;
			break;
		}
	}
	this->update();
}

void Dlg_MergeDraw::mouseMoveEvent(QMouseEvent *event)
{
	POINT pt;
	pt.x = event->pos().x();
	pt.y = event->pos().y();
	/*RECT rc;
	rc = GetPos();
	*/
	if (m_nSelect == -1)
	{
		return;
	}

	////if (PtInRect(&rc, pt) == false)
	//{
	//	if (m_nSelectNext != -1)
	//	{
	//		m_nSelectNext = -1;
	//		this->update();
	//	}
	//	return;
	//}

	int next = -1;
	for (int i = 0; i < m_nLayoutNum; i++)
	{
		if (PtInRect(&m_arcDrawLayout[i], pt))
		{
			next = i;
		}
	}

	if (next != m_nSelectNext)
	{
		m_nSelectNext = next;
		this->update();
	}

	return;
}

void Dlg_MergeDraw::mouseReleaseEvent(QMouseEvent *event)
{
	if (m_nSelect == -1)
	{
		GetZeroPos();// 不能在最下面加，点击的有可能m_nSelect为-1
		if (m_nLayoutNum == 3)
		{
			m_zeroPos = -1;
		}
		return;
	}

	if (m_nSelectNext == -1 || m_nSelect == m_nSelectNext || m_bControlZero)
	{
		GetZeroPos();
		if (m_nLayoutNum == 3)
		{
			m_zeroPos = -1;
		}
		m_nSelect = -1;
		m_nSelectNext = -1;
		this->update();
		return;
	}

	QString str = m_astrDrawText[m_nSelectNext];
	//不能拖动到0的位置
	if ((str == "0" || m_astrDrawText[m_nSelect] == "0") && (m_nSelectNext == 0 || m_nSelect == 0))
	{
		m_nSelect = -1;
		m_nSelectNext = -1;
		this->update();
		return;
	}
	////Up的时候不能拖动到0的位置
	//if (str == "0")
	//{
	//	m_nSelect = -1;
	//	m_nSelectNext = -1;
	//	this->update();
	//	return;
	//}
	m_astrDrawText[m_nSelectNext] = m_astrDrawText[m_nSelect];
	m_astrDrawText[m_nSelect] = str;
	m_nSelect = -1;
	m_nSelectNext = -1;
	GetZeroPos();
	if (m_nLayoutNum == 3)
	{
		m_zeroPos = -1;
	}
	this->update();
	return;
}

void Dlg_MergeDraw::CaleLayoutRect()
{
	QRect rc = this->rect();
	// 计算位置
	switch (m_nLayoutType)
	{
	case EMLABLELAYOUTTYPE_1:
	{
		m_nRow = 1;				// 行数     
		m_nCol = 1;				// 列数  

		Convert(m_nCol, rc.right() - rc.left(), m_nRow, rc.bottom() - rc.top(), rc.left(), rc.top());
		for (int i = 0; i < 81; i++) m_arcDrawLayout[i] = m_arcLayout[i];
		m_nLayoutNum = 1;

	}
		break;
	case EMLABLELAYOUTTYPE_1_2:
	{
		m_nRow = 1;				// 行数     
		m_nCol = 2;				// 列数  

		Convert(m_nCol, rc.right() - rc.left(), m_nRow, rc.bottom() - rc.top(), rc.left(), rc.top());
		for (int i = 0; i < 81; i++) m_arcDrawLayout[i] = m_arcLayout[i];
		m_nLayoutNum = 2;
	}
		break;
	case EMLABLELAYOUTTYPE_1_3:
	{
		m_nRow = 1;				// 行数     
		m_nCol = 3;				// 列数  

		Convert(m_nCol, rc.right() - rc.left(), m_nRow, rc.bottom() - rc.top(), rc.left(), rc.top());
		for (int i = 0; i < 81; i++) m_arcDrawLayout[i] = m_arcLayout[i];
		m_nLayoutNum = 3;
	}
		break;
	case EMLABLELAYOUTTYPE_1_4:
	{
		m_nRow = 1;				// 行数     
		m_nCol = 4;				// 列数  

		Convert(m_nCol, rc.right() - rc.left(), m_nRow, rc.bottom() - rc.top(), rc.left(), rc.top());
		for (int i = 0; i < 81; i++) m_arcDrawLayout[i] = m_arcLayout[i];
		m_nLayoutNum = 4;
	}
		break;
	case EMLABLELAYOUTTYPE_1_5:
	{
		m_nRow = 1;				// 行数     
		m_nCol = 5;				// 列数  

		Convert(m_nCol, rc.right() - rc.left(), m_nRow, rc.bottom() - rc.top(), rc.left(), rc.top());
		for (int i = 0; i < 81; i++) m_arcDrawLayout[i] = m_arcLayout[i];
		m_nLayoutNum = 5;
	}
		break;
	case EMLABLELAYOUTTYPE_1_6:
	{
		m_nRow = 1;				// 行数     
		m_nCol = 6;				// 列数  

		Convert(m_nCol, rc.right() - rc.left(), m_nRow, rc.bottom() - rc.top(), rc.left(), rc.top());
		for (int i = 0; i < 81; i++) m_arcDrawLayout[i] = m_arcLayout[i];
		m_nLayoutNum = 6;
	}
		break;
	case EMLABLELAYOUTTYPE_2_1:
	{
		m_nRow = 2;				// 行数     
		m_nCol = 1;				// 列数  

		Convert(m_nCol, rc.right() - rc.left(), m_nRow, rc.bottom() - rc.top(), rc.left(), rc.top());
		for (int i = 0; i < 81; i++) m_arcDrawLayout[i] = m_arcLayout[i];
		m_nLayoutNum = 4;
	}
		break;
	case EMLABLELAYOUTTYPE_2_2:
	{
		m_nRow = 2;				// 行数     
		m_nCol = 2;				// 列数  

		Convert(m_nCol, rc.right() - rc.left(), m_nRow, rc.bottom() - rc.top(), rc.left(), rc.top());
		for (int i = 0; i < 81; i++) m_arcDrawLayout[i] = m_arcLayout[i];
		m_nLayoutNum = 4;
	}
		break;
	case EMLABLELAYOUTTYPE_2_3:
	{
		m_nRow = 2;				// 行数     
		m_nCol = 3;				// 列数  

		Convert(m_nCol, rc.right() - rc.left(), m_nRow, rc.bottom() - rc.top(), rc.left(), rc.top());
		for (int i = 0; i < 81; i++) m_arcDrawLayout[i] = m_arcLayout[i];
		m_nLayoutNum = 6;
	}
		break;
	case EMLABLELAYOUTTYPE_2_4_3:
	{
		m_nRow = 2;				// 行数     
		m_nCol = 4;				// 列数  

		Convert(m_nCol, rc.right() - rc.left(), m_nRow, rc.bottom() - rc.top(), rc.left(), rc.top());
		m_arcDrawLayout[0] = m_arcLayout[2 - 1];
		m_arcDrawLayout[0].right = m_arcLayout[2 + 1 - 1].right;
		m_arcDrawLayout[1] = m_arcLayout[5 - 1];
		m_arcDrawLayout[1].right = m_arcLayout[5].right;
		m_arcDrawLayout[2] = m_arcLayout[7 - 1];
		m_arcDrawLayout[2].right = m_arcLayout[7].right;
		m_nLayoutNum = 3;
	}
		break;
	case EMLABLELAYOUTTYPE_2_4_3_Invert:
	{
		m_nRow = 2;				// 行数     
		m_nCol = 4;				// 列数  

		Convert(m_nCol, rc.right() - rc.left(), m_nRow, rc.bottom() - rc.top(), rc.left(), rc.top());
		m_arcDrawLayout[0] = m_arcLayout[1 - 1];
		m_arcDrawLayout[0].right = m_arcLayout[1].right;
		m_arcDrawLayout[1] = m_arcLayout[3 - 1];
		m_arcDrawLayout[1].right = m_arcLayout[3].right;
		m_arcDrawLayout[2] = m_arcLayout[6 - 1];
		m_arcDrawLayout[2].right = m_arcLayout[6].right;
		m_nLayoutNum = 3;
	}
		break;
	case EMLABLELAYOUTTYPE_3_1:
	{
		m_nRow = 3;				// 行数     
		m_nCol = 1;				// 列数  

		Convert(m_nCol, rc.right() - rc.left(), m_nRow, rc.bottom() - rc.top(), rc.left(), rc.top());
		for (int i = 0; i < 81; i++) m_arcDrawLayout[i] = m_arcLayout[i];
		m_nLayoutNum = 3;
	}
		break;
	case EMLABLELAYOUTTYPE_3_2:
	{
		m_nRow = 3;				// 行数     
		m_nCol = 2;				// 列数  

		Convert(m_nCol, rc.right() - rc.left(), m_nRow, rc.bottom() - rc.top(), rc.left(), rc.top());
		for (int i = 0; i < 81; i++) m_arcDrawLayout[i] = m_arcLayout[i];
		m_nLayoutNum = 6;
	}
		break;
	case EMLABLELAYOUTTYPE_4_1:
	{
		m_nRow = 4;				// 行数     
		m_nCol = 1;				// 列数  

		Convert(m_nCol, rc.right() - rc.left(), m_nRow, rc.bottom() - rc.top(), rc.left(), rc.top());
		for (int i = 0; i < 81; i++) m_arcDrawLayout[i] = m_arcLayout[i];
		m_nLayoutNum = 4;
	}
		break;
	case EMLABLELAYOUTTYPE_5_1:
	{
		m_nRow = 5;				// 行数     
		m_nCol = 1;				// 列数  

		Convert(m_nCol, rc.right() - rc.left(), m_nRow, rc.bottom() - rc.top(), rc.left(), rc.top());
		for (int i = 0; i < 81; i++) m_arcDrawLayout[i] = m_arcLayout[i];
		m_nLayoutNum = 5;
	}
		break;
	case EMLABLELAYOUTTYPE_6_1:
	{
		m_nRow = 6;				// 行数     
		m_nCol = 1;				// 列数  

		Convert(m_nCol, rc.right() - rc.left(), m_nRow, rc.bottom() - rc.top(), rc.left(), rc.top());
		for (int i = 0; i < 81; i++) m_arcDrawLayout[i] = m_arcLayout[i];
		m_nLayoutNum = 6;
	}
		break;
	case EMLABLELAYOUTTYPE_6_4_12:
	{
		m_nRow = 3;				// 行数     
		m_nCol = 2;				// 列数  

		Convert(m_nCol, rc.right() - rc.left(), m_nRow, rc.bottom() - rc.top(), rc.left(), rc.top());
		int h = m_arcLayout[0].bottom - m_arcLayout[0].top - 4;//高度各减4，留作四合一和二合一之间的间隙
		m_arcLayout[0].bottom = m_arcLayout[1].bottom = m_arcLayout[0].top + h;
		m_arcLayout[2].top = m_arcLayout[3].top = m_arcLayout[0].bottom;
		m_arcLayout[2].bottom = m_arcLayout[3].bottom = m_arcLayout[2].top + h;
		m_arcLayout[4].top = m_arcLayout[5].top = m_arcLayout[4].top - 2;
		for (int i = 0; i < 81; i++) m_arcDrawLayout[i] = m_arcLayout[i];
		m_nLayoutNum = 6;
	}
		break;
	case EMLABLELAYOUTTYPE_6_4_21:
	{
		m_nRow = 2;				// 行数     
		m_nCol = 3;				// 列数  

		Convert(m_nCol, rc.right() - rc.left(), m_nRow, rc.bottom() - rc.top(), rc.left(), rc.top());
		int h = m_arcLayout[0].right - m_arcLayout[0].left - 4;//宽度各减4，留作四合一和二合一之间的间隙
		m_arcLayout[0].right = m_arcLayout[3].right = m_arcLayout[0].left + h;
		m_arcLayout[1].left = m_arcLayout[4].left = m_arcLayout[0].right;
		m_arcLayout[1].right = m_arcLayout[4].right = m_arcLayout[1].left + h;
		m_arcLayout[2].left = m_arcLayout[5].left = m_arcLayout[2].left - 2;
		for (int i = 0; i < 81; i++) m_arcDrawLayout[i] = m_arcLayout[i];
		m_nLayoutNum = 6;
	}
		break;

	case EMLABLELAYOUTTYPE_1x2_1x2:
	case EMLABLELAYOUTTYPE_1x2_1x0:
	{
		m_nRow = 2;				// 行数     
		m_nCol = 2;				// 列数  

		Convert(m_nCol, rc.right() - rc.left(), m_nRow, rc.bottom() - rc.top(), rc.left(), rc.top());
		m_arcLayout[2].top += 4;
		m_arcLayout[2].bottom += 4;
		m_arcLayout[3].top += 4;
		m_arcLayout[3].bottom += 4;
		for (int i = 0; i < 81; i++) m_arcDrawLayout[i] = m_arcLayout[i];
		m_nLayoutNum = 4;
	}
		break;

	case EMLABLELAYOUTTYPE_2x1_2x1:
	case EMLABLELAYOUTTYPE_2x1_1x0:
	{
		m_nRow = 2;				// 行数     
		m_nCol = 2;				// 列数  

		Convert(m_nCol, rc.right() - rc.left(), m_nRow, rc.bottom() - rc.top(), rc.left(), rc.top());
		m_arcLayout[1].left += 4;
		m_arcLayout[1].right += 4;
		m_arcLayout[3].left += 4;
		m_arcLayout[3].right += 4;
		for (int i = 0; i < 81; i++) m_arcDrawLayout[i] = m_arcLayout[i];
		m_nLayoutNum = 4;
	}
		break;

	case EMLABLELAYOUTTYPE_1x2_1:
	{
		m_nRow = 2;				// 行数     
		m_nCol = 2;				// 列数  

		Convert(m_nCol, rc.right() - rc.left(), m_nRow, rc.bottom() - rc.top(), rc.left(), rc.top());
		m_arcLayout[2].top += 4;
		m_arcLayout[2].bottom += 4;
		m_arcLayout[3].top += 4;
		m_arcLayout[3].bottom += 4;
		for (int i = 0; i < 81; i++) m_arcDrawLayout[i] = m_arcLayout[i];
		m_nLayoutNum = 3;
	}
		break;

	case EMLABLELAYOUTTYPE_2x1_1:
	{
		m_nRow = 2;				// 行数     
		m_nCol = 2;				// 列数  

		Convert(m_nCol, rc.right() - rc.left(), m_nRow, rc.bottom() - rc.top(), rc.left(), rc.top());
		m_arcLayout[1].left += 4;
		m_arcLayout[1].right += 4;
		m_arcLayout[3].left += 4;
		m_arcLayout[3].right += 4;
		for (int i = 0; i < 81; i++) m_arcDrawLayout[i] = m_arcLayout[i];
		m_nLayoutNum = 3;
	}
		break;
	}
}
