#include "Dlg_LightConfig.h"
#include "MgrData.h"
#include <Dlg_MessageBox.h>
#include <QTimer>
Dlg_LightConfig::Dlg_LightConfig(QWidget *parent)
	: MyWidget(parent), m_messageBox(nullptr)
{
	ui.setupUi(this);
	MgrData::getInstance()->GetMgrSubject()->Add(this);
	connect(this, SIGNAL(signal_dataChanged()), this, SLOT(Slot_DataChanged()));
	connect(ui.btn_save, SIGNAL(clicked()), this, SLOT(Slot_BtnSaveClicked()));
	Init();
	
}

void Dlg_LightConfig::InitData(bool is)
{
	if (is)
	{
		GetLightParam();
	}
}

void Dlg_LightConfig::OnSetTxtVisible()
{
	if (m_messageBox)
	{
		m_messageBox->close();
		SAFE_DELETE(m_messageBox);
	}
}

Dlg_LightConfig::~Dlg_LightConfig()
{
	SAFE_DELETE(m_messageBox);
	MgrData::getInstance()->GetMgrSubject()->Del(this);
}

void Dlg_LightConfig::Slot_DataChanged()
{
	UpdateData_Light(false);
}

void Dlg_LightConfig::Slot_BtnSaveClicked()
{
	UpdateData_Light(true);
	SetLightParam();
	if (!m_messageBox)
	{
		QTimer::singleShot(1500, this, &Dlg_LightConfig::OnSetTxtVisible);
		m_messageBox = new Dlg_MessageBox;
		m_messageBox->SetInfoText(GBUTF8("±£´æ³É¹¦£¡"));
		m_messageBox->SetBtnNoVisible(false);
		m_messageBox->exec();
	}

}

void Dlg_LightConfig::OnObserverNotify(LPARAM lHint, LPVOID pHint)
{
	u32 dwEvtType = (u32)lHint;
	u32 dwId = (u32)pHint;

	switch (dwEvtType)
	{
	case NMC_IPC_LIGHT_BRIGHTNESS:
	{
		TIPC_LightParam* p = (TIPC_LightParam*)pHint;
		memcpy(&m_tLightParam, p, sizeof(m_tLightParam));
		emit signal_dataChanged();
	}
		break;
	default:
		break;
	}
}

void Dlg_LightConfig::UpdateData_Light(bool bUpdate)
{
	Uint8 *p;
	wchar_t ws[129];
	if (bUpdate)
	{
		p = (Uint8*)&m_tLightParam;
		for (int i = 0; i < 18; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				QString sKey = QString("%1_%2").arg(i+1).arg(j+1);
				QLineEdit *pEdit = m_map[sKey];
				p[i * 4 + j] = pEdit->text().toInt();
			}
		}
	}
	else
	{
		p = (Uint8*)&m_tLightParam;
		for (int i = 0; i < 18; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				QString sKey = QString("%1_%2").arg(i+1).arg(j+1);
				QLineEdit *pEdit = m_map[sKey];
				pEdit->setText(QString("%1").arg(p[i * 4 + j]));
				/*wsprintf(ws, TEXT("%d"), p[i * 4 + j]);
				pEdit->SetText(ws);*/
			}
		}
	}

}

void Dlg_LightConfig::SetLightParam()
{
	u32 dIP = MgrData::getInstance()->GetIP();
	ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dIP);
	if (pCtrl == NULL)
	{
		return;
	}

	pCtrl->SetIpcLightBrightness(&m_tLightParam);
}


void Dlg_LightConfig::Init()
{
	m_map.insert(QString("1_1"), ui.ledt_1_1);
	m_map.insert(QString("1_2"), ui.ledt_1_2);
	m_map.insert(QString("1_3"), ui.ledt_1_3);
	m_map.insert(QString("1_4"), ui.ledt_1_4);

	m_map.insert(QString("2_1"), ui.ledt_2_1);
	m_map.insert(QString("2_2"), ui.ledt_2_2);
	m_map.insert(QString("2_3"), ui.ledt_2_3);
	m_map.insert(QString("2_4"), ui.ledt_2_4);

	m_map.insert(QString("3_1"), ui.ledt_3_1);
	m_map.insert(QString("3_2"), ui.ledt_3_2);
	m_map.insert(QString("3_3"), ui.ledt_3_3);
	m_map.insert(QString("3_4"), ui.ledt_3_4);

	m_map.insert(QString("4_1"), ui.ledt_4_1);
	m_map.insert(QString("4_2"), ui.ledt_4_2);
	m_map.insert(QString("4_3"), ui.ledt_4_3);
	m_map.insert(QString("4_4"), ui.ledt_4_4);

	m_map.insert(QString("5_1"), ui.ledt_5_1);
	m_map.insert(QString("5_2"), ui.ledt_5_2);
	m_map.insert(QString("5_3"), ui.ledt_5_3);
	m_map.insert(QString("5_4"), ui.ledt_5_4);

	m_map.insert(QString("6_1"), ui.ledt_6_1);
	m_map.insert(QString("6_2"), ui.ledt_6_2);
	m_map.insert(QString("6_3"), ui.ledt_6_3);
	m_map.insert(QString("6_4"), ui.ledt_6_4);

	m_map.insert(QString("7_1"), ui.ledt_7_1);
	m_map.insert(QString("7_2"), ui.ledt_7_2);
	m_map.insert(QString("7_3"), ui.ledt_7_3);
	m_map.insert(QString("7_4"), ui.ledt_7_4);

	m_map.insert(QString("8_1"), ui.ledt_8_1);
	m_map.insert(QString("8_2"), ui.ledt_8_2);
	m_map.insert(QString("8_3"), ui.ledt_8_3);
	m_map.insert(QString("8_4"), ui.ledt_8_4);

	m_map.insert(QString("9_1"), ui.ledt_9_1);
	m_map.insert(QString("9_2"), ui.ledt_9_2);
	m_map.insert(QString("9_3"), ui.ledt_9_3);
	m_map.insert(QString("9_4"), ui.ledt_9_4);

	m_map.insert(QString("10_1"), ui.ledt_10_1);
	m_map.insert(QString("10_2"), ui.ledt_10_2);
	m_map.insert(QString("10_3"), ui.ledt_10_3);
	m_map.insert(QString("10_4"), ui.ledt_10_4);

	m_map.insert(QString("11_1"), ui.ledt_11_1);
	m_map.insert(QString("11_2"), ui.ledt_11_2);
	m_map.insert(QString("11_3"), ui.ledt_11_3);
	m_map.insert(QString("11_4"), ui.ledt_11_4);

	m_map.insert(QString("12_1"), ui.ledt_12_1);
	m_map.insert(QString("12_2"), ui.ledt_12_2);
	m_map.insert(QString("12_3"), ui.ledt_12_3);
	m_map.insert(QString("12_4"), ui.ledt_12_4);

	m_map.insert(QString("13_1"), ui.ledt_13_1);
	m_map.insert(QString("13_2"), ui.ledt_13_2);
	m_map.insert(QString("13_3"), ui.ledt_13_3);
	m_map.insert(QString("13_4"), ui.ledt_13_4);

	m_map.insert(QString("14_1"), ui.ledt_14_1);
	m_map.insert(QString("14_2"), ui.ledt_14_2);
	m_map.insert(QString("14_3"), ui.ledt_14_3);
	m_map.insert(QString("14_4"), ui.ledt_14_4);

	m_map.insert(QString("15_1"), ui.ledt_15_1);
	m_map.insert(QString("15_2"), ui.ledt_15_2);
	m_map.insert(QString("15_3"), ui.ledt_15_3);
	m_map.insert(QString("15_4"), ui.ledt_15_4);

	m_map.insert(QString("16_1"), ui.ledt_16_1);
	m_map.insert(QString("16_2"), ui.ledt_16_2);
	m_map.insert(QString("16_3"), ui.ledt_16_3);
	m_map.insert(QString("16_4"), ui.ledt_16_4);

	m_map.insert(QString("17_1"), ui.ledt_17_1);
	m_map.insert(QString("17_2"), ui.ledt_17_2);
	m_map.insert(QString("17_3"), ui.ledt_17_3);
	m_map.insert(QString("17_4"), ui.ledt_17_4);

	m_map.insert(QString("18_1"), ui.ledt_18_1);
	m_map.insert(QString("18_2"), ui.ledt_18_2);
	m_map.insert(QString("18_3"), ui.ledt_18_3);
	m_map.insert(QString("18_4"), ui.ledt_18_4);
	
	QMap<QString, QLineEdit*>::const_iterator itor;
	for (itor = m_map.constBegin(); itor != m_map.constEnd(); ++itor) 
	{
		QLineEdit *pEdit = itor.value();
		pEdit->setValidator(new QIntValidator(0, 100, this));
	}
		
}

bool Dlg_LightConfig::GetLightParam()
{
	MgrItsCtrl* pMgrItsCtrl = MgrData::getInstance()->GetMgrItsCtrl();
	if (pMgrItsCtrl != NULL)
	{
		u32 dIP = MgrData::getInstance()->GetIP();
		ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dIP);
		if (pCtrl != NULL)
		{
			bool bRet = pCtrl->GetIpcLightBrightness(NULL, (u32)(CObserver*)this);
			if (!bRet)
			{
			}
		}
	}
	return true;
}