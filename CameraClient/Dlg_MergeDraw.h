#pragma once

#include <QWidget>
#include "ivhs_struct.h"
#include "define.h"
#include <QMap>
#include <QMouseEvent>
#include <QPainter>
enum EMLABLELAYOUTTYPE
{
	EMLABLELAYOUTTYPE_1 = 0,			// һ��һ��
	EMLABLELAYOUTTYPE_1_2,			//1   һ�ж���
	EMLABLELAYOUTTYPE_1_3,			//2   һ��3��
	EMLABLELAYOUTTYPE_1_4,			//3   һ��4��
	EMLABLELAYOUTTYPE_1_5,			//4   һ��5��
	EMLABLELAYOUTTYPE_1_6,			//5   һ��6��
	EMLABLELAYOUTTYPE_2_1,			//6   2��1��
	EMLABLELAYOUTTYPE_2_2,			//7   2��2��
	EMLABLELAYOUTTYPE_2_3,			//8   2��3��
	EMLABLELAYOUTTYPE_2_4,			//9   2��4��
	EMLABLELAYOUTTYPE_2_4_3,		//10  2��4�� Ʒ����
	EMLABLELAYOUTTYPE_2_4_3_Invert,	//11  2��4�� ��Ʒ����
	EMLABLELAYOUTTYPE_2_5,			//12  2��5��
	EMLABLELAYOUTTYPE_2_6,			//13  2��6��
	EMLABLELAYOUTTYPE_3_1,			//14  3��1��
	EMLABLELAYOUTTYPE_3_2,			//15  3��2��
	EMLABLELAYOUTTYPE_3_3,			//16  3��3��
	EMLABLELAYOUTTYPE_3_4,			//17  3��4��
	EMLABLELAYOUTTYPE_3_5,			//18  3��5��
	EMLABLELAYOUTTYPE_3_6,			//19  3��6��
	EMLABLELAYOUTTYPE_4_1,			//20  3��1��
	EMLABLELAYOUTTYPE_4_2,			//21  3��2��
	EMLABLELAYOUTTYPE_4_3,			//22  3��3��
	EMLABLELAYOUTTYPE_4_4,			//23  3��4��
	EMLABLELAYOUTTYPE_4_5,			//24  3��5��
	EMLABLELAYOUTTYPE_4_6,			//25  3��6��
	EMLABLELAYOUTTYPE_5_1,			//26  3��1��
	EMLABLELAYOUTTYPE_5_2,			//27  3��2��
	EMLABLELAYOUTTYPE_5_3,			//28  3��3��
	EMLABLELAYOUTTYPE_5_4,			//29  3��4��
	EMLABLELAYOUTTYPE_5_5,			//30  3��5��
	EMLABLELAYOUTTYPE_5_6,			//31  3��6��
	EMLABLELAYOUTTYPE_6_1,			//32  3��1��
	EMLABLELAYOUTTYPE_6_2,			//33  3��2��
	EMLABLELAYOUTTYPE_6_3,			//34  3��3��
	EMLABLELAYOUTTYPE_6_4,			//35  3��4��
	EMLABLELAYOUTTYPE_6_5,			//36  3��5��
	EMLABLELAYOUTTYPE_6_6,			//37  3��6��

	EMLABLELAYOUTTYPE_6_4_12,       //38   ���϶����ĺ�һ�����Σ�����һˮƽ���з����·�
	EMLABLELAYOUTTYPE_6_4_21,       //39   ���϶����ĺ�һ�����Σ�����һ��ֱ���з����ҷ�

	EMLABLELAYOUTTYPE_1x2_1x2,      //40	�ĺ϶�: 1x2 + 1x2
	EMLABLELAYOUTTYPE_2x1_2x1,      //41	�ĺ϶�: 2x1 + 2x1 
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
	//ֻ��Ϊ4��ͼ���õ�����Ҫ��Ϊ��3����Ҳ�п�����4��
	void SetLayoutNum(int number){ m_nLayoutNum = number; }
	int    GetZeroIndex();
	void   SetZeroIndex(int sel);
	int    GetZeroPos();  //�϶��п��ܵ���λ�øı䣬��ñ���һ��

	QString GetDrawText(int nIndex){ return m_astrDrawText[nIndex]; }


private:
	void CaleLayoutRect();
	// ת����ʼ����
	void ConvertStartPos(int n1, int n2, int nStart, int a[], int max);
	// ת����������
	void ConvertEndPos(int n1, int n2, int nStart, int a[], int max);
	// ����ת��
	void Convert(int w1, int w2, int h1, int h2, int xOffset = 0, int yOffset = 0);

private:
	int		m_nLayoutType;
	DWORD	m_dwCurBorderColor;
	DWORD	m_dwDrawColor;
	RECT	m_arcLayout[81];	//
	int		m_nLayoutNum;		// ������Ŀ
	int		m_nRow;				// ����     
	int		m_nCol;				// ����     
	RECT	m_arcDrawLayout[81];	// ��Ҫ���Ƶ�����
	QMap<int, QString> m_astrDrawText;
	DWORD	m_nRGB[7];
	int		m_nSelect;
	int       m_nSelectNext;

	int		m_zeroColorIndex;
	int		m_zeroPos; //0��λ��
	bool	m_bControlZero;//����0λ�ò������϶�
};
