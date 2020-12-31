#pragma once

#include <QWidget>
#include "ivhs_struct.h"
#include "define.h"
#include <QMap>
#include <QMouseEvent>
#include <QPainter>
enum EMLABLELAYOUTTYPE
{
	EMLABLELAYOUTTYPE_1 = 0,			// 一行一列
	EMLABLELAYOUTTYPE_1_2,			//1   一行二列
	EMLABLELAYOUTTYPE_1_3,			//2   一行3列
	EMLABLELAYOUTTYPE_1_4,			//3   一行4列
	EMLABLELAYOUTTYPE_1_5,			//4   一行5列
	EMLABLELAYOUTTYPE_1_6,			//5   一行6列
	EMLABLELAYOUTTYPE_2_1,			//6   2行1列
	EMLABLELAYOUTTYPE_2_2,			//7   2行2列
	EMLABLELAYOUTTYPE_2_3,			//8   2行3列
	EMLABLELAYOUTTYPE_2_4,			//9   2行4列
	EMLABLELAYOUTTYPE_2_4_3,		//10  2行4列 品子形
	EMLABLELAYOUTTYPE_2_4_3_Invert,	//11  2行4列 倒品子形
	EMLABLELAYOUTTYPE_2_5,			//12  2行5列
	EMLABLELAYOUTTYPE_2_6,			//13  2行6列
	EMLABLELAYOUTTYPE_3_1,			//14  3行1列
	EMLABLELAYOUTTYPE_3_2,			//15  3行2列
	EMLABLELAYOUTTYPE_3_3,			//16  3行3列
	EMLABLELAYOUTTYPE_3_4,			//17  3行4列
	EMLABLELAYOUTTYPE_3_5,			//18  3行5列
	EMLABLELAYOUTTYPE_3_6,			//19  3行6列
	EMLABLELAYOUTTYPE_4_1,			//20  3行1列
	EMLABLELAYOUTTYPE_4_2,			//21  3行2列
	EMLABLELAYOUTTYPE_4_3,			//22  3行3列
	EMLABLELAYOUTTYPE_4_4,			//23  3行4列
	EMLABLELAYOUTTYPE_4_5,			//24  3行5列
	EMLABLELAYOUTTYPE_4_6,			//25  3行6列
	EMLABLELAYOUTTYPE_5_1,			//26  3行1列
	EMLABLELAYOUTTYPE_5_2,			//27  3行2列
	EMLABLELAYOUTTYPE_5_3,			//28  3行3列
	EMLABLELAYOUTTYPE_5_4,			//29  3行4列
	EMLABLELAYOUTTYPE_5_5,			//30  3行5列
	EMLABLELAYOUTTYPE_5_6,			//31  3行6列
	EMLABLELAYOUTTYPE_6_1,			//32  3行1列
	EMLABLELAYOUTTYPE_6_2,			//33  3行2列
	EMLABLELAYOUTTYPE_6_3,			//34  3行3列
	EMLABLELAYOUTTYPE_6_4,			//35  3行4列
	EMLABLELAYOUTTYPE_6_5,			//36  3行5列
	EMLABLELAYOUTTYPE_6_6,			//37  3行6列

	EMLABLELAYOUTTYPE_6_4_12,       //38   六合二，四合一田字形，二合一水平排列放在下方
	EMLABLELAYOUTTYPE_6_4_21,       //39   六合二，四合一田字形，二合一垂直排列放在右方

	EMLABLELAYOUTTYPE_1x2_1x2,      //40	四合二: 1x2 + 1x2
	EMLABLELAYOUTTYPE_2x1_2x1,      //41	四合二: 2x1 + 2x1 
	EMLABLELAYOUTTYPE_1x2_1x0,
	EMLABLELAYOUTTYPE_2x1_1x0,
	EMLABLELAYOUTTYPE_1x2_1,
	EMLABLELAYOUTTYPE_2x1_1,
	EMLABLELAYOUTTYPE_NONE = 255
};

class Dlg_MergeDraw : public QWidget
{
	Q_OBJECT

public:
	Dlg_MergeDraw(QWidget *parent);

	~Dlg_MergeDraw();

	void paintEvent(QPaintEvent *event);

	void mousePressEvent(QMouseEvent *event);

	void mouseReleaseEvent(QMouseEvent *event);

	void mouseMoveEvent(QMouseEvent *event);

	void SetLayoutType(int nType){ m_nLayoutType = nType; CaleLayoutRect(); this->update(); }

	int	 GetLayoutType(){ return m_nLayoutType; };

	void SetDrawText(QMap<int, QString> tempMap);
	//只有为4张图才用到，主要因为有3个，也有可能有4个
	void SetLayoutNum(int number){ m_nLayoutNum = number; }
	int    GetZeroIndex();
	void   SetZeroIndex(int sel);
	int    GetZeroPos();  //拖动有可能导致位置改变，最好遍历一下

	QString GetDrawText(int nIndex){ return m_astrDrawText[nIndex]; }


private:
	void CaleLayoutRect();
	// 转换开始坐标
	void ConvertStartPos(int n1, int n2, int nStart, int a[], int max);
	// 转换结束坐标
	void ConvertEndPos(int n1, int n2, int nStart, int a[], int max);
	// 坐标转换
	void Convert(int w1, int w2, int h1, int h2, int xOffset = 0, int yOffset = 0);

private:
	int		m_nLayoutType;
	DWORD	m_dwCurBorderColor;
	DWORD	m_dwDrawColor;
	RECT	m_arcLayout[81];	//
	int		m_nLayoutNum;		// 布局数目
	int		m_nRow;				// 行数     
	int		m_nCol;				// 列数     
	RECT	m_arcDrawLayout[81];	// 需要绘制的区域；
	QMap<int, QString> m_astrDrawText;
	DWORD	m_nRGB[7];
	int		m_nSelect;
	int       m_nSelectNext;

	int		m_zeroColorIndex;
	int		m_zeroPos; //0的位置
	bool	m_bControlZero;//控制0位置不允许拖动
};
