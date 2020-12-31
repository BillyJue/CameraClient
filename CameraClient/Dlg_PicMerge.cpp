#include "Dlg_PicMerge.h"
#include "Dlg_MergeDraw.h"
#include "MgrData.h"
#include <QListView>
#include <QTimer>
Dlg_PicMerge::Dlg_PicMerge(QWidget *parent)
	: MyWidget(parent), m_messageBox(nullptr)
{
	ui.setupUi(this);
	ui.cblayout2_1->setView(new QListView);
	ui.cblayout3_1->setView(new QListView);
	ui.cblayout4_1->setView(new QListView);
	ui.cblayout6_1->setView(new QListView);

	memset(&m_tMerge, 0, sizeof(m_tMerge));
	connect(this, SIGNAL(signal_mergePicChanged()), this, SLOT(Slot_MergePictureChanged()));
	connect(ui.cblayout2_1, SIGNAL(currentIndexChanged(int)), this, SLOT(Slot_CmbIndexChanged(int)));
	connect(ui.cblayout3_1, SIGNAL(currentIndexChanged(int)), this, SLOT(Slot_CmbIndexChanged(int)));
	connect(ui.cblayout4_1, SIGNAL(currentIndexChanged(int)), this, SLOT(Slot_CmbIndexChanged(int)));
	connect(ui.cblayout6_1, SIGNAL(currentIndexChanged(int)), this, SLOT(Slot_CmbIndexChanged(int)));
	connect(ui.btn_save, SIGNAL(clicked()), this, SLOT(Slot_BtnSaveClicked()));
	MgrData::getInstance()->GetMgrSubject()->Add(this);
	//test----------
	//InitData();
}


Dlg_PicMerge::~Dlg_PicMerge()
{
	SAFE_DELETE(m_messageBox);
	MgrData::getInstance()->GetMgrSubject()->Del(this);
}

void Dlg_PicMerge::OnSetTxtVisible()
{
	if (m_messageBox)
	{
		m_messageBox->close();
		SAFE_DELETE(m_messageBox);
	}
}

void Dlg_PicMerge::InitData(bool is)
{
	if (is)
	{
		GetParam();
	}
}

bool Dlg_PicMerge::SetParam()
{
	bool bRet = false;
	UpdateData(false);
	u32 dwIP = MgrData::getInstance()->GetIP();
	ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dwIP);
	if (pCtrl != NULL)
	{
		bRet = pCtrl->SetIpcMergeParam(&m_tMerge);
	}

	if (!bRet)
	{
		return false;
	}
	return true;
}


void Dlg_PicMerge::Slot_MergePictureChanged()
{
	UpdateData(true);
}

void Dlg_PicMerge::Slot_BtnSaveClicked()
{
	SetParam();
	if (!m_messageBox)
	{
		QTimer::singleShot(1500, this, &Dlg_PicMerge::OnSetTxtVisible);
		m_messageBox = new Dlg_MessageBox;
		m_messageBox->SetInfoText(GBUTF8("保存成功！"));
		m_messageBox->SetBtnNoVisible(false);
		m_messageBox->exec();
	}
}

void Dlg_PicMerge::Slot_CmbIndexChanged(int nIndex)
{
	/*if (this->sender()->property("id").toInt() == 111)
	{
		this->sender()->setProperty("id", 0);
		return;
	}*/

	if (this->sender() == ui.cblayout2_1)
	{
		QMap<int, QString> tempMap;
		int index = ui.cblayout2_1->currentIndex();
		switch (index)
		{
		case 0:
			m_tMerge.type2 = EMLABLELAYOUTTYPE_1_2;
			break;
		case 1:
			m_tMerge.type2 = EMLABLELAYOUTTYPE_2_1;
			break;
		case 2:
			m_tMerge.type2 = EMLABLELAYOUTTYPE_NONE;
			break;
		}

		SetMergeData(0, m_tMerge.type2, ui.merge_1);
		ui.merge_1->SetLayoutType(m_tMerge.type2);
		SetDrawVisible(ui.merge_1, m_tMerge.type2);
		return;
	}

	if (this->sender() == ui.cblayout4_1)
	{
		QMap<int, QString> tempMap;
		int index = ui.cblayout4_1->currentIndex();
		if ((!m_tMerge.bSupport1x4) && (index > 2))
		{
			index += 1;
		}

		switch (index)
		{
		case 0:
			m_tMerge.type4 = EMLABLELAYOUTTYPE_2_2;
			break;
		case 1:
			m_tMerge.type4 = EMLABLELAYOUTTYPE_1x2_1x2;
			break;
		case 2:
			m_tMerge.type4 = EMLABLELAYOUTTYPE_2x1_2x1;
			break;
		case 3:
			m_tMerge.type4 = EMLABLELAYOUTTYPE_1_4;
			break;
		case 4:
			m_tMerge.type4 = EMLABLELAYOUTTYPE_NONE;
			break;
		}
		SetMergeData(1, m_tMerge.type4, ui.merge_3);
		ui.merge_3->SetLayoutType(m_tMerge.type4);
		SetDrawVisible(ui.merge_3, m_tMerge.type4);
		return;
	}

	if (this->sender() == ui.cblayout6_1)
	{
		QMap<int, QString> tempMap;
		int index = ui.cblayout6_1->currentIndex();
		switch (index)
		{
		case 0:
			m_tMerge.type6 = EMLABLELAYOUTTYPE_2_3;
			break;
		case 1:
			m_tMerge.type6 = EMLABLELAYOUTTYPE_3_2;
			break;
		case 2:
			m_tMerge.type6 = EMLABLELAYOUTTYPE_NONE;
			break;
		}
		SetMergeData(2, m_tMerge.type6, ui.merge_4);
		ui.merge_4->SetLayoutType(m_tMerge.type6);
		SetDrawVisible(ui.merge_4, m_tMerge.type6);
		return;
	}

	if (this->sender() == ui.cblayout3_1)
	{
		QMap<int, QString> tempMap;
		int index = ui.cblayout3_1->currentIndex();
		int nType = m_tMerge.type3;
		switch (index)
		{
		case 0:
			m_tMerge.type3 = EMLABLELAYOUTTYPE_1_3;
			break;
		case 1:
			m_tMerge.type3 = EMLABLELAYOUTTYPE_3_1;
			break;
		case 2:
			m_tMerge.type3 = EMLABLELAYOUTTYPE_1x2_1x0;
			break;

		case 3:
			m_tMerge.type3 = EMLABLELAYOUTTYPE_2x1_1x0;
			break;
		case 4:
			m_tMerge.type3 = EMLABLELAYOUTTYPE_1x2_1;
			break;
		case 5:
			m_tMerge.type3 = EMLABLELAYOUTTYPE_2x1_1;
			break;
		case 6:
			m_tMerge.type3 = EMLABLELAYOUTTYPE_NONE;
			break;
		}
		SetMergeData(3, m_tMerge.type3, ui.merge_2);
		ui.merge_2->SetLayoutType(m_tMerge.type3);
		SetDrawVisible(ui.merge_2, m_tMerge.type3);
		return;
	}

}

void Dlg_PicMerge::OnObserverNotify(LPARAM lHint, LPVOID pHint)
{
	if (lHint == NMC_GET_IPC_MERGE_PIC)
	{
		TIPC_TImgMergePara* p = (TIPC_TImgMergePara*)pHint;
		memcpy(&m_tMerge, p, sizeof(m_tMerge));
		emit signal_mergePicChanged();
		//this->PostMessage(NMC_GET_PICINFO);
	}
}

void Dlg_PicMerge::UpdateData(bool bUpdate /*= true*/)
{
	// 从数据更新到界面
	if (bUpdate)
	{
		ui.itspicmergeenable->setChecked(m_tMerge.enable);
		ui.chkbcutvideo->setChecked(m_tMerge.bCutVideo);
		ui.editjpeg2qp->setText(QString("%1").arg(m_tMerge.jpegQp2));

		//2张合图
		{
			wchar_t num[4];

			ui.merge_1->SetLayoutType(m_tMerge.type2);
			switch (m_tMerge.type2)
			{
			case EMLABLELAYOUTTYPE_1_2:
				ui.cblayout2_1->setCurrentIndex(0);
				m_n2to1[0] = m_tMerge.layout2to1[0][0];
				m_n2to1[1] = m_tMerge.layout2to1[0][1];
				break;
			case EMLABLELAYOUTTYPE_2_1:
				ui.cblayout2_1->setCurrentIndex(1);
				m_n2to1[0] = m_tMerge.layout2to1[0][0];
				m_n2to1[1] = m_tMerge.layout2to1[1][0];
				break;
			case EMLABLELAYOUTTYPE_NONE:
			{
				m_n2to1[0] = 1;
				m_n2to1[1] = 2;
				ui.cblayout2_1->setCurrentIndex(2);
			}

				break;
			default:
				m_n2to1[0] = 1;
				m_n2to1[1] = 2;
				break;
			}
			QMap<int, QString> tempMap;

			for (int i = 0; i<2; i++)
			{
				tempMap[i] = QString("%1").arg(m_n2to1[i]);
			}
			ui.merge_1->SetDrawText(tempMap);
		}

		ui.editjpeg4qp->setText(QString("%1").arg(m_tMerge.jpegQp4));
		{
			if (m_tMerge.bSupport1x4)
			{
				ui.cblayout4_1->setProperty("id", 111);
				ui.cblayout4_1->addItem(GBUTF8("1x4"));
				ui.cblayout4_1->setProperty("id", 0);
			}
			ui.cblayout4_1->setProperty("id", 111);
			ui.cblayout4_1->addItem(GBUTF8("不合图"));
			ui.cblayout4_1->setProperty("id", 0);

		
			wchar_t num[4];
			ui.merge_3->SetLayoutType(m_tMerge.type4);
			switch (m_tMerge.type4)
			{
			case EMLABLELAYOUTTYPE_2_2:
				ui.cblayout4_1->setCurrentIndex(0);
				m_n4to1[0] = m_tMerge.layout4to1[0][0];
				m_n4to1[1] = m_tMerge.layout4to1[0][1];
				m_n4to1[2] = m_tMerge.layout4to1[1][0];
				m_n4to1[3] = m_tMerge.layout4to1[1][1];
				break;
			case EMLABLELAYOUTTYPE_1_4:
				ui.cblayout4_1->setCurrentIndex(3);
				m_n4to1[0] = m_tMerge.layout4to1[0][0];
				m_n4to1[1] = m_tMerge.layout4to1[0][1];
				m_n4to1[2] = m_tMerge.layout4to1[0][2];
				m_n4to1[3] = m_tMerge.layout4to1[0][3];
				break;
			case EMLABLELAYOUTTYPE_1x2_1x2:
				ui.cblayout4_1->setCurrentIndex(1);
				m_n4to1[0] = m_tMerge.layout4to1[0][0];
				m_n4to1[1] = m_tMerge.layout4to1[0][1];
				m_n4to1[2] = m_tMerge.layout4to1[1][0];
				m_n4to1[3] = m_tMerge.layout4to1[1][1];
				break;
			case EMLABLELAYOUTTYPE_2x1_2x1:
				ui.cblayout4_1->setCurrentIndex(2);
				m_n4to1[0] = m_tMerge.layout4to1[0][0];
				m_n4to1[1] = m_tMerge.layout4to1[0][1];
				m_n4to1[2] = m_tMerge.layout4to1[1][0];
				m_n4to1[3] = m_tMerge.layout4to1[1][1];
				break;

			case EMLABLELAYOUTTYPE_NONE:
			{
				m_n4to1[0] = 1;
				m_n4to1[1] = 2;
				m_n4to1[2] = 3;
				m_n4to1[3] = 4;
				ui.cblayout4_1->setCurrentIndex(4);

			}

				break;

			default:
				m_n4to1[0] = 1;
				m_n4to1[1] = 2;
				m_n4to1[2] = 3;
				m_n4to1[3] = 4;
				break;
			}
			QMap<int, QString> tempMap;
			for (int i = 0; i<4; i++)
			{
				tempMap[i] = QString("%1").arg(m_n4to1[i]);
			}
			ui.merge_3->SetDrawText(tempMap);

		}

		ui.editjpeg6qp->setText(QString("%1").arg(m_tMerge.jpegQp6));
		{
			ui.merge_4->SetLayoutType(m_tMerge.type6);
			switch (m_tMerge.type6)
			{
			case EMLABLELAYOUTTYPE_2_3:
				ui.cblayout6_1->setCurrentIndex(0);
				m_n6to1[0] = m_tMerge.layout6to1[0][0];
				m_n6to1[1] = m_tMerge.layout6to1[0][1];
				m_n6to1[2] = m_tMerge.layout6to1[0][2];
				m_n6to1[3] = m_tMerge.layout6to1[1][0];
				m_n6to1[4] = m_tMerge.layout6to1[1][1];
				m_n6to1[5] = m_tMerge.layout6to1[1][2];
				break;
			case EMLABLELAYOUTTYPE_3_2:
				ui.cblayout6_1->setCurrentIndex(1);

				m_n6to1[0] = m_tMerge.layout6to1[0][0];
				m_n6to1[1] = m_tMerge.layout6to1[0][1];
				m_n6to1[2] = m_tMerge.layout6to1[1][0];
				m_n6to1[3] = m_tMerge.layout6to1[1][1];
				m_n6to1[4] = m_tMerge.layout6to1[2][0];
				m_n6to1[5] = m_tMerge.layout6to1[2][1];
				break;
			case EMLABLELAYOUTTYPE_NONE:
			{
				m_n6to1[0] = 1;
				m_n6to1[1] = 2;
				m_n6to1[2] = 3;
				m_n6to1[3] = 4;
				m_n6to1[4] = 5;
				m_n6to1[5] = 6;
				ui.cblayout6_1->setCurrentIndex(2);

			}

				break;
			default:
				m_n6to1[0] = 1;
				m_n6to1[1] = 2;
				m_n6to1[2] = 3;
				m_n6to1[3] = 4;
				m_n6to1[4] = 5;
				m_n6to1[5] = 6;
				break;
			}

			QMap<int, QString> tempMap;
			for (int i = 0; i<6; i++)
			{
				tempMap[i] = QString("%1").arg(m_n6to1[i]);
			}
			ui.merge_4->SetDrawText(tempMap);

		}

		ui.editjpeg3qp->setText(QString("%1").arg(m_tMerge.jpegQp3));
		{
			QMap<int, QString> tempMap;
			int nComSelectIndex = -1;
			switch (m_tMerge.type3)
			{
			case EMLABLELAYOUTTYPE_1_3:
				nComSelectIndex = 0;
				//m_pCombo[3]->SelectItem(0);
				m_n3to1[0] = m_tMerge.layout3to1[0][0];
				m_n3to1[1] = m_tMerge.layout3to1[0][1];
				m_n3to1[2] = m_tMerge.layout3to1[0][2];
				m_n3to1[3] = 0; //初始化
				SetZeroArr(tempMap, 4, false);
				break;
			case EMLABLELAYOUTTYPE_3_1:
				nComSelectIndex = 1;
				m_n3to1[0] = m_tMerge.layout3to1[0][0];
				m_n3to1[1] = m_tMerge.layout3to1[1][0];
				m_n3to1[2] = m_tMerge.layout3to1[2][0];
				m_n3to1[3] = 0; //初始化
				SetZeroArr(tempMap, 4, false);
				break;
			case EMLABLELAYOUTTYPE_1x2_1x0:
				nComSelectIndex = 2;
				m_n3to1[0] = m_tMerge.layout3to1[0][0];
				m_n3to1[1] = m_tMerge.layout3to1[0][1];
				m_n3to1[2] = m_tMerge.layout3to1[1][0];
				m_n3to1[3] = m_tMerge.layout3to1[1][1];
				SetZeroArr(tempMap, 4, true);
				break;
			case EMLABLELAYOUTTYPE_2x1_1x0:
				nComSelectIndex = 3;
				m_n3to1[0] = m_tMerge.layout3to1[0][0];
				m_n3to1[1] = m_tMerge.layout3to1[0][1];
				m_n3to1[2] = m_tMerge.layout3to1[1][0];
				m_n3to1[3] = m_tMerge.layout3to1[1][1];
				SetZeroArr(tempMap, 4, true);
				break;

			case EMLABLELAYOUTTYPE_1x2_1:
				nComSelectIndex = 4;
				m_n3to1[0] = m_tMerge.layout3to1[0][0];
				m_n3to1[1] = m_tMerge.layout3to1[0][1];
				m_n3to1[2] = m_tMerge.layout3to1[1][0];
				m_n3to1[3] = m_tMerge.layout3to1[1][1];
				SetZeroArr(tempMap, 3, false);
				tempMap[3] = "";
				break;

			case EMLABLELAYOUTTYPE_2x1_1:
				nComSelectIndex = 5;
				m_n3to1[0] = m_tMerge.layout3to1[0][0];
				m_n3to1[1] = m_tMerge.layout3to1[0][1];
				m_n3to1[2] = m_tMerge.layout3to1[1][0];
				m_n3to1[3] = m_tMerge.layout3to1[1][1];
				SetZeroArr(tempMap, 3, false);
				tempMap[3] = "";
				break;

			case EMLABLELAYOUTTYPE_NONE:
			{
				m_n3to1[0] = 1;
				m_n3to1[1] = 2;
				m_n3to1[2] = 3;
				m_n3to1[3] = 0;
				nComSelectIndex = 6;
			}

				break;

			default:
				m_tMerge.type3 = EMLABLELAYOUTTYPE_1_3;
				nComSelectIndex = 0;
				m_n3to1[0] = 1;
				m_n3to1[1] = 2;
				m_n3to1[2] = 3;
				m_n3to1[3] = 0;
				SetZeroArr(tempMap, 4, false);

				break;
			}
			if (m_tMerge.type3 == EMLABLELAYOUTTYPE_1x2_1 || m_tMerge.type3 == EMLABLELAYOUTTYPE_2x1_1)
			{
				ui.merge_2->SetLayoutNum(3);
			}
			else
			{
				ui.merge_2->SetLayoutNum(4);
			}
			ui.merge_2->SetLayoutType(m_tMerge.type3);
			ui.merge_2->SetDrawText(tempMap);
			ui.cblayout3_1->setCurrentIndex(nComSelectIndex);

		}
	}
	// 从界面更新到数据
	else
	{
		Bool bSupport1x4 = m_tMerge.bSupport1x4;
		memset(&m_tMerge, 0, sizeof(m_tMerge));
		m_tMerge.bSupport1x4 = bSupport1x4;

		m_tMerge.enable = ui.itspicmergeenable->isChecked(); 
		m_tMerge.bCutVideo = ui.chkbcutvideo->isChecked();
		m_tMerge.jpegQp2 = ui.editjpeg2qp->text().toInt();
		
		{
			m_n2to1[0] = ui.merge_1->GetDrawText(0).toInt();
			m_n2to1[1] = ui.merge_1->GetDrawText(1).toInt();

			int index = ui.cblayout2_1->currentIndex();
			switch (index)
			{
			case 0:
				m_tMerge.type2 = EMLABLELAYOUTTYPE_1_2;
				m_tMerge.layout2to1[0][0] = m_n2to1[0];
				m_tMerge.layout2to1[0][1] = m_n2to1[1];
				break;
			case 1:
				m_tMerge.type2 = EMLABLELAYOUTTYPE_2_1;
				m_tMerge.layout2to1[0][0] = m_n2to1[0];
				m_tMerge.layout2to1[1][0] = m_n2to1[1];
				break;
			case 2:
				m_tMerge.type2 = EMLABLELAYOUTTYPE_NONE;
				m_tMerge.layout2to1[0][0] = m_n2to1[0];
				m_tMerge.layout2to1[0][1] = m_n2to1[1];
				break;
			}
		}

		m_tMerge.jpegQp4 = ui.editjpeg4qp->text().toInt();

		{
			m_n4to1[0] = ui.merge_3->GetDrawText(0).toInt();
			m_n4to1[1] = ui.merge_3->GetDrawText(1).toInt();
			m_n4to1[2] = ui.merge_3->GetDrawText(2).toInt();
			m_n4to1[3] = ui.merge_3->GetDrawText(3).toInt();

			int index = ui.cblayout4_1->currentIndex();
			switch (index)
			{
			case 0:
				m_tMerge.type4 = EMLABLELAYOUTTYPE_2_2;
				m_tMerge.layout4to1[0][0] = m_n4to1[0];
				m_tMerge.layout4to1[0][1] = m_n4to1[1];
				m_tMerge.layout4to1[1][0] = m_n4to1[2];
				m_tMerge.layout4to1[1][1] = m_n4to1[3];
				break;
			case 1:
				m_tMerge.type4 = EMLABLELAYOUTTYPE_1x2_1x2;
				m_tMerge.layout4to1[0][0] = m_n4to1[0];
				m_tMerge.layout4to1[0][1] = m_n4to1[1];
				m_tMerge.layout4to1[1][0] = m_n4to1[2];
				m_tMerge.layout4to1[1][1] = m_n4to1[3];
				break;
			case 2:
				m_tMerge.type4 = EMLABLELAYOUTTYPE_2x1_2x1;
				m_tMerge.layout4to1[0][0] = m_n4to1[0];
				m_tMerge.layout4to1[0][1] = m_n4to1[1];
				m_tMerge.layout4to1[1][0] = m_n4to1[2];
				m_tMerge.layout4to1[1][1] = m_n4to1[3];
				break;
			case 3:
				m_tMerge.type4 = EMLABLELAYOUTTYPE_1_4;
				m_tMerge.layout4to1[0][0] = m_n4to1[0];
				m_tMerge.layout4to1[0][1] = m_n4to1[1];
				m_tMerge.layout4to1[0][2] = m_n4to1[2];
				m_tMerge.layout4to1[0][3] = m_n4to1[3];
				break;

			case  4:
				m_tMerge.type4 = EMLABLELAYOUTTYPE_NONE;
				m_tMerge.layout4to1[0][0] = m_n4to1[0];
				m_tMerge.layout4to1[0][1] = m_n4to1[1];
				m_tMerge.layout4to1[1][0] = m_n4to1[2];
				m_tMerge.layout4to1[1][1] = m_n4to1[3];
				break;
			}
		}

		m_tMerge.jpegQp6 = ui.editjpeg6qp->text().toInt();
		{
			m_n6to1[0] = ui.merge_4->GetDrawText(0).toInt();
			m_n6to1[1] = ui.merge_4->GetDrawText(1).toInt();
			m_n6to1[2] = ui.merge_4->GetDrawText(2).toInt();
			m_n6to1[3] = ui.merge_4->GetDrawText(3).toInt();
			m_n6to1[4] = ui.merge_4->GetDrawText(4).toInt();
			m_n6to1[5] = ui.merge_4->GetDrawText(5).toInt();

			int index = ui.cblayout6_1->currentIndex();
			switch (index)
			{
			case 0:
				m_tMerge.type6 = EMLABLELAYOUTTYPE_2_3;
				m_tMerge.layout6to1[0][0] = m_n6to1[0];
				m_tMerge.layout6to1[0][1] = m_n6to1[1];
				m_tMerge.layout6to1[0][2] = m_n6to1[2];
				m_tMerge.layout6to1[1][0] = m_n6to1[3];
				m_tMerge.layout6to1[1][1] = m_n6to1[4];
				m_tMerge.layout6to1[1][2] = m_n6to1[5];
				break;
			case 1:
				m_tMerge.type6 = EMLABLELAYOUTTYPE_3_2;
				m_tMerge.layout6to1[0][0] = m_n6to1[0];
				m_tMerge.layout6to1[0][1] = m_n6to1[1];
				m_tMerge.layout6to1[1][0] = m_n6to1[2];
				m_tMerge.layout6to1[1][1] = m_n6to1[3];
				m_tMerge.layout6to1[2][0] = m_n6to1[4];
				m_tMerge.layout6to1[2][1] = m_n6to1[5];
				break;

			case 2:
				m_tMerge.type6 = EMLABLELAYOUTTYPE_NONE;
				m_tMerge.layout6to1[0][0] = m_n6to1[0];
				m_tMerge.layout6to1[0][1] = m_n6to1[1];
				m_tMerge.layout6to1[0][2] = m_n6to1[2];
				m_tMerge.layout6to1[1][0] = m_n6to1[3];
				m_tMerge.layout6to1[1][1] = m_n6to1[4];
				m_tMerge.layout6to1[1][2] = m_n6to1[5];
				break;
			}
		}

		m_tMerge.jpegQp3 = ui.editjpeg3qp->text().toInt();
		{
			m_n3to1[0] = ui.merge_2->GetDrawText(0).toInt();
			m_n3to1[1] = ui.merge_2->GetDrawText(1).toInt();
			m_n3to1[2] = ui.merge_2->GetDrawText(2).toInt();
			m_n3to1[3] = ui.merge_2->GetDrawText(3).toInt();
			//传出的参数： 颜色索引|0xf0;
			int index = ui.cblayout3_1->currentIndex();

			switch (index)
			{
			case 0:
				m_tMerge.type3 = EMLABLELAYOUTTYPE_1_3;
				m_tMerge.layout3to1[0][0] = m_n3to1[0];
				m_tMerge.layout3to1[0][1] = m_n3to1[1];
				m_tMerge.layout3to1[0][2] = m_n3to1[2];
				break;
			case 1:
				m_tMerge.type3 = EMLABLELAYOUTTYPE_3_1;
				m_tMerge.layout3to1[0][0] = m_n3to1[0];
				m_tMerge.layout3to1[1][0] = m_n3to1[1];
				m_tMerge.layout3to1[2][0] = m_n3to1[2];
				break;

			case 2:
				m_tMerge.type3 = EMLABLELAYOUTTYPE_1x2_1x0;
				SetZeroColor(ui.merge_2, m_n3to1, 4);
				m_tMerge.layout3to1[0][0] = m_n3to1[0];
				m_tMerge.layout3to1[0][1] = m_n3to1[1];
				m_tMerge.layout3to1[1][0] = m_n3to1[2];
				m_tMerge.layout3to1[1][1] = m_n3to1[3];
				break;

			case 3:
				m_tMerge.type3 = EMLABLELAYOUTTYPE_2x1_1x0;
				SetZeroColor(ui.merge_2, m_n3to1, 4);
				m_tMerge.layout3to1[0][0] = m_n3to1[0];
				m_tMerge.layout3to1[0][1] = m_n3to1[1];
				m_tMerge.layout3to1[1][0] = m_n3to1[2];
				m_tMerge.layout3to1[1][1] = m_n3to1[3];
				break;

			case 4:
				m_tMerge.type3 = EMLABLELAYOUTTYPE_1x2_1;
				m_tMerge.layout3to1[0][0] = m_n3to1[0];
				m_tMerge.layout3to1[0][1] = m_n3to1[1];
				m_tMerge.layout3to1[1][0] = m_n3to1[2];
				break;

			case 5:
				m_tMerge.type3 = EMLABLELAYOUTTYPE_2x1_1;
				m_tMerge.layout3to1[0][0] = m_n3to1[0];
				m_tMerge.layout3to1[1][0] = m_n3to1[2];
				m_tMerge.layout3to1[0][1] = m_n3to1[1];
				break;
			case 6:
				m_tMerge.type3 = EMLABLELAYOUTTYPE_NONE;
				/*m_tMerge.layout3to1[0][0] = m_n3to1[0];
				m_tMerge.layout3to1[1][0] = m_n3to1[1];
				m_tMerge.layout3to1[0][1] = m_n3to1[2];
				m_tMerge.layout3to1[1][1] = m_n3to1[3];*/
				break;
			}
		}
	}
}

void  Dlg_PicMerge::SetZeroArr(QMap<int, QString> &tempMap, int nLen, bool  isHasZero)
{
	wchar_t  num[4];
	for (int i = 0; i < nLen; i++)
	{
		//颜色逻辑
		Uint8 aa = m_n3to1[i] & 0xf0;
		if (isHasZero && (m_n3to1[i] & 0xf0))
		{
			int colIndex = m_n3to1[i] & 0x0f;
			ui.merge_2->SetZeroIndex(colIndex);
			tempMap[i] = QString("0");
		}
		else
		{
			tempMap[i] = QString("%1").arg(m_n3to1[i]);
		}
	}
}

bool Dlg_PicMerge::GetParam()
{
	bool bRet = false;
	u32 dwIP = MgrData::getInstance()->GetIP();
	ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dwIP);
	if (pCtrl != NULL)
	{
		bRet = pCtrl->GetIpcMergeParam(NULL, (u32)(CObserver*)this);
	}
	return true;
}


void Dlg_PicMerge::SetZeroColor(Dlg_MergeDraw * lab, int * pArr, int len)
{
	DWORD nSel = lab->GetZeroIndex();
	Uint8 zeroColr = nSel | 0xf0;
	for (int i = 0; i < len; i++)
	{
		if (pArr[i] == 0)
		{
			pArr[i] = zeroColr;
			break;
		}

	}
}


void Dlg_PicMerge::SetMergeData(int nIndex, Uint8 type, Dlg_MergeDraw * pLab)
{
	QMap<int, QString> tempMap;
	wchar_t  num[4];
	int nTempArr[9] = { -1, -1, -1, -1, -1, -1, -1, -1, -1 };
	if (nIndex == 0)
	{
		for (int i = 0; i < 2; i++)
		{
			tempMap[i] = QString("%1").arg(i + 1);
		}
		pLab->SetDrawText(tempMap);
	}
	else if (nIndex == 1)
	{
		for (int i = 0; i < 4; i++)
		{
			tempMap[i] = QString("%1").arg(i + 1);
		}
		pLab->SetDrawText(tempMap);
	}
	else if (nIndex == 2)
	{
		for (int i = 0; i < 6; i++)
		{
			tempMap[i] = QString("%1").arg(i + 1);
		}
		pLab->SetDrawText(tempMap);
	}
	else if (nIndex == 3)
	{

		if (type == EMLABLELAYOUTTYPE_2x1_1x0 || type == EMLABLELAYOUTTYPE_1x2_1x0)
		{
			SetZeroArr(tempMap, 4, true);
		}
		else
		{
			SetZeroArr(tempMap, 4, false);
		}

		if (type == EMLABLELAYOUTTYPE_1x2_1 || type == EMLABLELAYOUTTYPE_2x1_1)
		{
			pLab->SetLayoutNum(3);
		}
		else
		{
			pLab->SetLayoutNum(4);
		}
		pLab->SetDrawText(tempMap);
	}


}

void Dlg_PicMerge::SetDrawVisible(Dlg_MergeDraw *pLabelLayout, Uint8 type)
{
	if (type == EMLABLELAYOUTTYPE_NONE)
	{
		pLabelLayout->setVisible(false);
	}
	else
	{
		pLabelLayout->setVisible(true);
	}
}
