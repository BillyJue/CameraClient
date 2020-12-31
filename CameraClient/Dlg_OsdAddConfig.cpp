#include "Dlg_OsdAddConfig.h"
#include "MgrData.h"
#include <QListView>
#include "Dlg_OsdFontSetting.h"
#include <QTimer>
Dlg_OsdAddConfig::Dlg_OsdAddConfig(QWidget *parent)
	: MyWidget(parent), m_nOsdType(0), m_messageBox(nullptr)
{
	memset(&m_tData, 0, sizeof(m_tData));
	memset(&m_tFont, 0, sizeof(m_tFont));
	ui.setupUi(this);
	MgrData::getInstance()->GetMgrSubject()->Add(this);
	Init();
	//connect(ui.listWidget, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(Slot_ListWidgetItemClicked(QListWidgetItem*)));
	connect(this, SIGNAL(signal_osdInfo()), this, SLOT(Slot_OsdInfo()));
	connect(this, SIGNAL(signal_osdFontInfo()), this, SLOT(Slot_OsdFontInfo()));
	connect(ui.btn_fontSetting, SIGNAL(clicked()), this, SLOT(Slot_FontSettingClicked()));
	connect(ui.btn_defautOsd, SIGNAL(clicked()), this, SLOT(Slot_DefaultOsdClicked()));
	connect(ui.btn_save, SIGNAL(clicked()), this, SLOT(Slot_SaveClicked()));
	connect(ui.btn_fresh, SIGNAL(clicked()), this, SLOT(Slot_FreshClicked()));
	connect(ui.btn_clearFormat, SIGNAL(clicked()), this, SLOT(Slot_ClearFormatClicked()));
	connect(ui.btn_wordWrap, SIGNAL(clicked()), this, SLOT(Slot_WordWrapClicked()));

	connect(ui.btn_tab1, SIGNAL(clicked()), this, SLOT(Slot_TabCurrentIndexChanged()));
	connect(ui.btn_tab2, SIGNAL(clicked()), this, SLOT(Slot_TabCurrentIndexChanged()));
	connect(ui.btn_tab3, SIGNAL(clicked()), this, SLOT(Slot_TabCurrentIndexChanged()));
	connect(ui.btn_tab4, SIGNAL(clicked()), this, SLOT(Slot_TabCurrentIndexChanged()));
	connect(ui.btn_tab5, SIGNAL(clicked()), this, SLOT(Slot_TabCurrentIndexChanged()));
	connect(ui.btn_tab6, SIGNAL(clicked()), this, SLOT(Slot_TabCurrentIndexChanged()));

	
}

void Dlg_OsdAddConfig::OnSetTxtVisible()
{
	if (m_messageBox)
	{
		m_messageBox->close();
		SAFE_DELETE(m_messageBox);
	}
}

void Dlg_OsdAddConfig::InitData(bool is)
{
	if (is)
	{ 
		ui.btn_tab1->clicked(true);
	}
}

void Dlg_OsdAddConfig::Slot_ClearFormatClicked()
{
	QMap<QString, QCheckBox*>::Iterator iter;
	QString sTemp;
	for (iter = m_map.begin(); iter != m_map.end(); iter++)
	{
		sTemp = iter.key();
		m_map[sTemp]->setChecked(false);
	}
	ui.ledt_text->setText("");

}

void Dlg_OsdAddConfig::Slot_WordWrapClicked()
{
	QString sLine = ui.ledt_text->toPlainText();
	sLine += "\r";
	ui.ledt_text->setText(sLine);
}

void Dlg_OsdAddConfig::Slot_SaveClicked()
{
	UpdateData(false);
	SetParam();
	if (!m_messageBox)
	{
		QTimer::singleShot(1500, this, &Dlg_OsdAddConfig::OnSetTxtVisible);
		m_messageBox = new Dlg_MessageBox;
		m_messageBox->SetInfoText(GBUTF8("保存成功！"));
		m_messageBox->SetBtnNoVisible(false);
		m_messageBox->exec();
	}
}

void Dlg_OsdAddConfig::Slot_FreshClicked()
{
	GetParam();
}

void Dlg_OsdAddConfig::Slot_DefaultOsdClicked()
{
	ui.cmb_alignType->setCurrentIndex(0);
	ui.cmb_positonType->setCurrentIndex(0);
	ui.cmb_backgroundColor->setCurrentIndex(6);
	ui.cmb_symbol->setCurrentIndex(0);
	ui.cmb_timeFormat->setCurrentIndex(0);
	ui.chk_wordWrap->setChecked(false);
	QString str = GBUTF8("时间:D# E# 地点:A# 设备编号:G#\r车道号:C# 车牌号:I# H# ");
	ui.ledt_text->setText(str);
	ui.label_9->setText(str);
}

void Dlg_OsdAddConfig::Slot_FontSettingClicked()
{
	Dlg_OsdFontSetting dlg(&m_tFont, this);
	dlg.exec();
	SetOsdFontParam();
}

void Dlg_OsdAddConfig::Slot_OsdInfo()
{
	UpdateData();
	u32 dIP = MgrData::getInstance()->GetIP();
	ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dIP);
	if (pCtrl != NULL)
	{
		pCtrl->GetSysCfgOsdFont(&m_nOsdType, (u32)(CObserver*)this);
	}
}

void Dlg_OsdAddConfig::UpdateData(bool bUpdate /*= true*/)
{
	wchar_t wchars[256] = { 0 };
	char    chars[256] = { 0 };

	// 从数据更新到界面
	if (bUpdate)
	{
		ui.chk_enable->setChecked(m_tData.bEnable);
		ui.cmb_alignType->setCurrentIndex(m_tData.dwAlignType);
		ui.cmb_positonType->setCurrentIndex(m_tData.dwPosType);
		ui.cmb_backgroundColor->setCurrentIndex(m_tData.dwBKColor);
		
		QString sFormat = QString(m_tData.szDateTimeFormat);
		for (int j = 0; j < ui.cmb_timeFormat->count(); j++)
		{
			if (ui.cmb_timeFormat->itemText(j) == sFormat)
			{
				ui.cmb_timeFormat->setCurrentIndex(j);
				break;
			}
		}
		//cmb_symbol
		//通配符 m_tData.szWildcard[0]
		char2wchar(wchars, &m_tData.szWildcard[0]);
		for (int i = 0; i < ui.cmb_symbol->count(); i++)
		{
			if (ui.cmb_symbol->itemText(i) == QString::fromStdWString(wchars))
			{
				ui.cmb_symbol->setCurrentIndex(i);
				break;
			}
		}
		ui.chk_wordWrap->setChecked(m_tData.bAutoLinefeed);
		
		QString sTextData;
		for (int i = 0; i < m_tData.dwRowNumber; i++)
		{
			char2wchar(wchars, m_tData.szOsdFormat[i]);
			sTextData += QString::fromWCharArray(wchars);
			
			if (i + 1 != m_tData.dwRowNumber)
			{
				sTextData += "\\r";
			}
		}
		ui.ledt_text->setText("");
		ui.ledt_text->setText(sTextData);
		std::wstring str = sTextData.toStdWString();
		ParserStr(str, true);
		QString sExample = QString::fromStdWString(str);
		ui.label_9->setText(sExample);



	}
	// 从界面更新到数据
	else
	{
		char sy = m_tData.szWildcard[0];
		memset(&m_tData, 0, sizeof(m_tData));
		m_tData.szWildcard[0] = '#';
		if (ui.chk_enable->isChecked())
		{
			m_tData.bEnable = true;
			m_tData.dwAlignType = ui.cmb_alignType->currentIndex();
			m_tData.dwPosType = ui.cmb_positonType->currentIndex();
			m_tData.dwBKColor = ui.cmb_backgroundColor->currentIndex();

			QString sFormat = ui.cmb_timeFormat->currentText();
			QString sSysmbol = ui.cmb_symbol->currentText();
			wchar2char(m_tData.szDateTimeFormat, sFormat.toStdWString().c_str());
			//wchar2char(chars, sSysmbol.toStdWString().c_str());
			//m_tData.szWildcard[0] = chars[0];
			m_tData.bAutoLinefeed = ui.chk_wordWrap->isChecked();
			QString str = ui.ledt_text->toPlainText();
			int pos = 0, oldPos = 0, idx = 0;
			int size = str.length();
			pos = str.indexOf("\\r", pos);
			while (pos != -1)
			{
				wchar2char(m_tData.szOsdFormat[idx++], str.left(pos).toStdWString().c_str());
				str = str.right(str.length() - pos - 1);
				pos = str.indexOf("\\r", 0);
			}
			wchar2char(m_tData.szOsdFormat[idx], str.toStdWString().c_str());
			m_tData.dwRowNumber = idx + 1;
		}
	}
}

void Dlg_OsdAddConfig::Slot_OsdFontInfo()
{

}

void Dlg_OsdAddConfig::Slot_TabCurrentIndexChanged()
{
	memset(&m_tData, 0, sizeof(m_tData));
	int nIndex = 0;
	if (this->sender() == ui.btn_tab1)
	{
		m_nOsdType = 0;
	}
	else if (this->sender() == ui.btn_tab2)
	{
		m_nOsdType = 1;
	}
	else if (this->sender() == ui.btn_tab3)
	{
		m_nOsdType = 2;
	}
	else if (this->sender() == ui.btn_tab4)
	{
		m_nOsdType = 3;
	}
	else if (this->sender() == ui.btn_tab5)
	{
		m_nOsdType = 4;
	}
	else if (this->sender() == ui.btn_tab6)
	{
		m_nOsdType = 5;
	}
	
	GetParam();
}

Dlg_OsdAddConfig::~Dlg_OsdAddConfig()
{
	SAFE_DELETE(m_messageBox);
	MgrData::getInstance()->GetMgrSubject()->Del(this);
	if (m_signalMapper)
	{
		delete m_signalMapper;
		m_signalMapper = nullptr;
	}
}

void Dlg_OsdAddConfig::Init()
{
	ui.cmb_alignType->addItem(GBUTF8("左对齐"));
	ui.cmb_alignType->addItem(GBUTF8("右对齐"));
	ui.cmb_alignType->setView(new  QListView());
	
	ui.cmb_positonType->addItem(GBUTF8("图片底部"));
	ui.cmb_positonType->addItem(GBUTF8("图片左上方"));
	ui.cmb_positonType->addItem(GBUTF8("图片右下方"));
	ui.cmb_positonType->addItem(GBUTF8("图片顶部"));
	ui.cmb_positonType->setView(new  QListView());

	ui.cmb_backgroundColor->addItem(GBUTF8("白色"));
	ui.cmb_backgroundColor->addItem(GBUTF8("红色"));
	ui.cmb_backgroundColor->addItem(GBUTF8("黄色"));
	ui.cmb_backgroundColor->addItem(GBUTF8("绿色"));
	ui.cmb_backgroundColor->addItem(GBUTF8("蓝色"));
	ui.cmb_backgroundColor->addItem(GBUTF8("紫色"));
	ui.cmb_backgroundColor->addItem(GBUTF8("黑色"));
	ui.cmb_backgroundColor->addItem(GBUTF8("透明"));
	ui.cmb_backgroundColor->setView(new  QListView());

	ui.cmb_timeFormat->addItem(GBUTF8("YYYY-MM-DD hh:mm:ss"));
	ui.cmb_timeFormat->addItem(GBUTF8("YYYY/MM/DD hh:mm:ss"));
	ui.cmb_timeFormat->setView(new  QListView());

	m_map.insert("0", ui.chk_A);
	m_map.insert("1", ui.chk_B);
	m_map.insert("2", ui.chk_C);
	m_map.insert("3", ui.chk_D);
	m_map.insert("4", ui.chk_E);
	m_map.insert("5", ui.chk_F);
	m_map.insert("6", ui.chk_G);
	m_map.insert("7", ui.chk_H);
	m_map.insert("8", ui.chk_I);
	m_map.insert("9", ui.chk_J);
	m_map.insert("10", ui.chk_K);
	m_map.insert("11", ui.chk_L);
	m_map.insert("12", ui.chk_M);
	m_map.insert("13", ui.chk_N);
	m_map.insert("14", ui.chk_O);
	m_map.insert("15", ui.chk_P);
	m_map.insert("16", ui.chk_Q);
	m_map.insert("17", ui.chk_R);
	m_map.insert("18", ui.chk_S);
	m_map.insert("19", ui.chk_T);
	m_map.insert("20", ui.chk_U);
	m_map.insert("21", ui.chk_V);
	m_map.insert("22", ui.chk_W);
	m_map.insert("23", ui.chk_X);
	m_map.insert("24", ui.chk_Y);
	m_map.insert("25", ui.chk_Z);
	m_map.insert("26", ui.chk_a);
	m_map.insert("27", ui.chk_b);
	m_map.insert("28", ui.chk_c);
	m_map.insert("29", ui.chk_d);
	m_map.insert("30", ui.chk_e);
	ui.chk_A->setProperty("name", GBUTF8("地点:"));
	ui.chk_B->setProperty("name", GBUTF8("道路编号:"));
	ui.chk_C->setProperty("name", GBUTF8("车道:"));
	ui.chk_D->setProperty("name", GBUTF8("时间:"));
	ui.chk_E->setProperty("name", GBUTF8(""));
	ui.chk_F->setProperty("name", GBUTF8("设备名称:"));
	ui.chk_G->setProperty("name", GBUTF8("设备编号:"));
	ui.chk_H->setProperty("name", GBUTF8("车牌:"));
	ui.chk_I->setProperty("name", GBUTF8("车牌颜色:"));
	ui.chk_J->setProperty("name", GBUTF8("车身颜色:"));
	ui.chk_K->setProperty("name", GBUTF8("车标:"));
	ui.chk_L->setProperty("name", GBUTF8("车型:"));
	ui.chk_M->setProperty("name", GBUTF8("限速:"));
	ui.chk_N->setProperty("name", GBUTF8("车速:"));
	ui.chk_O->setProperty("name", GBUTF8("超速比:"));
	ui.chk_P->setProperty("name", GBUTF8("行驶方向:"));
	ui.chk_Q->setProperty("name", GBUTF8("违法类型:"));
	ui.chk_R->setProperty("name", GBUTF8("违法代码:"));
	ui.chk_S->setProperty("name", GBUTF8("防伪码:"));
	ui.chk_T->setProperty("name", GBUTF8("红灯时间:"));
	ui.chk_U->setProperty("name", GBUTF8("红灯开始时间:"));
	ui.chk_V->setProperty("name", GBUTF8("红灯结束时间:"));
	ui.chk_W->setProperty("name", GBUTF8("违停时间:"));
	ui.chk_X->setProperty("name", GBUTF8("车道方向:"));
	ui.chk_Y->setProperty("name", GBUTF8("危险品车辆:"));
	ui.chk_Z->setProperty("name", GBUTF8("号牌置信度:"));
	ui.chk_a->setProperty("name", GBUTF8("号牌类别:"));
	ui.chk_b->setProperty("name", GBUTF8("司乘人员信息:"));
	ui.chk_c->setProperty("name", GBUTF8("限行:"));
	ui.chk_d->setProperty("name", GBUTF8("电子标识:"));
	ui.chk_e->setProperty("name", GBUTF8("物理定位:"));


	/*m_map.insert("A", ui.chk_A);
	m_map.insert("B", ui.chk_B);
	m_map.insert("C", ui.chk_C);
	m_map.insert("D", ui.chk_D);
	m_map.insert("E", ui.chk_E);
	m_map.insert("F", ui.chk_F);
	m_map.insert("G", ui.chk_G);
	m_map.insert("H", ui.chk_H);
	m_map.insert("I", ui.chk_I);
	m_map.insert("J", ui.chk_J);
	m_map.insert("K", ui.chk_K);
	m_map.insert("L", ui.chk_L);
	m_map.insert("M", ui.chk_M);
	m_map.insert("N", ui.chk_N);
	m_map.insert("O", ui.chk_O);
	m_map.insert("P", ui.chk_P);
	m_map.insert("Q", ui.chk_Q);
	m_map.insert("R", ui.chk_R);
	m_map.insert("S", ui.chk_S);
	m_map.insert("T", ui.chk_T);
	m_map.insert("U", ui.chk_U);
	m_map.insert("V", ui.chk_V);
	m_map.insert("W", ui.chk_W);
	m_map.insert("X", ui.chk_X);
	m_map.insert("Y", ui.chk_Y);
	m_map.insert("Z", ui.chk_Z);
	m_map.insert("a", ui.chk_a);
	m_map.insert("b", ui.chk_b);
	m_map.insert("c", ui.chk_c);
	m_map.insert("d", ui.chk_d);
	m_map.insert("e", ui.chk_e);*/
	
	QMap<QString, QCheckBox*>::Iterator iter;
	QString sTemp;
	int nCount = 0;

	m_signalMapper = new QSignalMapper(this);
	for (iter = m_map.begin(); iter != m_map.end(); iter++)
	{
		sTemp = iter.key();
		m_signalMapper->setMapping(m_map[sTemp], m_map[sTemp]);
		connect(m_map[sTemp], SIGNAL(stateChanged(int)), m_signalMapper, SLOT(map()));
		nCount++;
	}
	connect(m_signalMapper, SIGNAL(mapped(QWidget *)),
		this, SLOT(Slot_CheckClicked(QWidget*)));
	//通配符隐藏
	ui.cmb_symbol->setVisible(false);
	ui.label_10->setVisible(false);

}

void Dlg_OsdAddConfig::Slot_CheckClicked(QWidget* pWidget)
{
	QCheckBox *pCheck = dynamic_cast<QCheckBox*>(pWidget);
	QString sText = ui.ledt_text->toPlainText();
	QString sName = pCheck->text();
	QString sChar = sName.left(1);
	
	QString sTextName = pCheck->property("name").toString();
	if (pCheck->isChecked())
	{
		if (sText.indexOf(sChar) == -1)
		{
			sText = QString("%1%2%3%4 ").arg(sText).arg(sTextName).arg(sChar).arg(m_tData.szWildcard[0]);
			ui.ledt_text->setText(sText);
		}
		
	}
	else
	{
		int nPos = sText.indexOf(sChar);
		if (nPos != -1)
		{
			QString sLeft = sText.left(nPos - sTextName.length());
			nPos = nPos + 2;
			QString sRight = sText.right(sText.length() - nPos - 1);
			QString sText = sLeft + sRight;
			ui.ledt_text->setText(sText);
		}
	}
	
	/*
	QString sText = ui.ledt_text->toPlainText();
	QCheckBox *pCheck = dynamic_cast<QCheckBox *>(this->sender());
	QString sName = pCheck->text();
	int nIndex = ui.ledt_text->property("index").toInt();
	char sChar;
	if (nIndex > 25)
	{
		sChar = 'a' + nIndex - 26;
	}
	else
	{
		sChar = 'A' + nIndex;
	}

	if (nStatus)
	{
		sText = sText + sName + QString("%1%2%3 ").arg(sName).arg(sChar).arg(m_tData.szWildcard[0]);
		ui.ledt_text->setText(sText);
	}
	else
	{
	 	int nPos = sText.indexOf(sName);
		if (nPos != -1)
		{
			QString sLeft = sText.left(nPos);
			nPos = nPos + sText.length() + 1;
			QString sRight = sText.right(sText.length() - nPos);
			QString sText = sLeft + sRight;
			ui.ledt_text->setText(sText);
		}
		
	}
	*/

}

void Dlg_OsdAddConfig::OnObserverNotify(LPARAM lHint, LPVOID pHint)
{
	if (lHint == NMC_GET_OSDINFO)
	{
		TOsdCfgRsp* p = (TOsdCfgRsp*)pHint;
		if (p->nRetCode)//nRetCode非0表示发生错误
		{
			return;
		}
		memcpy(&m_tData, &p->tOsdCfg, sizeof(m_tData));
		emit signal_osdInfo();
		//this->PostMessage(NMC_GET_OSDINFO);
	}
	else if (lHint == NMC_GET_OSDFONTINFO)
	{
		TOsdFontCfgRsp* p = (TOsdFontCfgRsp*)pHint;
		memcpy(&m_tFont, &p->tOsdFontCfg, sizeof(m_tFont));
		emit signal_osdFontInfo();
	}
	else if (lHint == NMC_CFG_SETSVRIP)
	{
		u32  dwSvrIP = *(u32*)pHint;
		//SetDevIP(dwSvrIP);
	}
	
}

bool Dlg_OsdAddConfig::GetParam()
{
	bool bRet = false;
	u32 dIP = MgrData::getInstance()->GetIP();
	ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dIP);
	if (pCtrl != NULL)
	{
		bRet = pCtrl->GetSysCfgOsd(&m_nOsdType, (u32)(CObserver*)this);
	}
		
	return true;
}

void Dlg_OsdAddConfig::ParserStr(std::wstring &str, bool updateChk)
{
	QString examStr[MAX_OSD_OPTION] = {
		GBUTF8("金山路"), //A:地点
		GBUTF8("123456"), //B:道路编号	
		GBUTF8("1"), //C:车道号		
		GBUTF8("2016-5-12 13:02:45"), //D:过车时间
		GBUTF8("681"),	//E:过车时间毫秒数	
		GBUTF8("IPC625"), //F:设备名称	
		GBUTF8("123456"), //G:设备编号	 
		GBUTF8("苏E 86U86"), //H:车牌	
		GBUTF8("蓝色"), //I:车牌颜色
		GBUTF8("白色"), //G:车身颜色    
		GBUTF8("雪佛兰"), //K:车标	   
		GBUTF8("小轿车"), //L:车型		
		GBUTF8("80km/h"), //M:限速		
		GBUTF8("60km/h"), //N:车速		
		GBUTF8("0"), //O:超速百分比		
		GBUTF8("由西向东"), //P:行驶方向		
		GBUTF8("闯红灯"), //Q:违法类型		
		GBUTF8("4236"),	 //R:违法代码
		GBUTF8("SGJSDFLGK3KD9"), //S:防伪码		
		GBUTF8("13"), //T:红灯时间		
		GBUTF8("2016-5-12 13:02:45"), //U:红灯开始时间		
		GBUTF8("2016-5-12 13:02:55"), //V:红灯结束时间
		GBUTF8("2016-5-12 13:02:45"), //W:违停时间	
		GBUTF8("由西向东"), //X:车道方向
		GBUTF8(""), //Y:危化品车辆
		GBUTF8("苏-99,E-99,8-99,6-99,U-99,8-99,6-99"), //Z:号牌置信度
		GBUTF8("小型汽车号牌"), //a:号牌类别
		GBUTF8(""), //b:司乘人员信息
	};
	

	QMap<QString, QCheckBox*>::Iterator iter;
	QString sTemp;
	for (iter = m_map.begin(); iter != m_map.end(); iter++)
	{
		sTemp = iter.key();
		m_map[sTemp]->setChecked(false);
	}

	wchar_t wz[8] = { 0 };
	char2wchar(wz, m_tData.szWildcard);
	int idx;
	int upper, lower;
	do
	{
		idx = str.find_first_of(wz[0]);
		if (idx == -1)
		{
			break;
		}
		upper = str[idx - 1] - 'A';
		lower = str[idx - 1] - 'a';
		if ((upper < 0 || upper >= 26) && (lower < 0 || lower >= 26))
		{
			break;
		}
		if (lower >= 0 && lower < 26)
		{
			upper = lower + 26;
		}
		str.replace(idx - 1, 2, examStr[upper].toStdWString().c_str());
		QString sKey = QString("%1").arg(upper);
		QCheckBox *pCheckBox = m_map[sKey];
		if (updateChk && pCheckBox)
		{
			pCheckBox->setChecked(true);
		}
		
	} while (1);
}


bool Dlg_OsdAddConfig::SetParam()
{
	bool bRet = false;
	u32 dIP = MgrData::getInstance()->GetIP();
	ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dIP);
	if (pCtrl != NULL)
	{
		m_tData.dwType = m_nOsdType;
		bRet = pCtrl->SetSysCfgOsd(&m_tData);
	}

	//SetOsdFontParam();

	return true;
}

bool Dlg_OsdAddConfig::SetOsdFontParam()
{
	u32 dIP = MgrData::getInstance()->GetIP();
	ItsCtrl* pCtrl = MgrData::getInstance()->GetMgrItsCtrl()->GetCtrl(dIP);
	if (pCtrl != NULL)
	{
		m_tFont.dwType = m_nOsdType;
		pCtrl->SetSysCfgOsdFont(&m_tFont);
	}

	return true;
}