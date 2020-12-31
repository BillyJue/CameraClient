#include "Dlg_PlatComponent.h"
#include <QPushButton>
#include <QListView>
Dlg_PlatComponent::Dlg_PlatComponent(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	connect(ui.btn_expand_2, SIGNAL(clicked()), this, SIGNAL(signal_btnClicked()));
	connect(ui.btn_clear, SIGNAL(clicked()), this, SLOT(Slot_BtnClearClicked()));
	connect(ui.btn_add_2, SIGNAL(clicked()), this, SLOT(Slot_AddClicked()));
	connect(ui.chk_carInfo, SIGNAL(stateChanged(int)), this, SLOT(Slot_CarInfoClicked(int)));
	connect(ui.cmb_type_2, SIGNAL(currentIndexChanged(int)), this, SLOT(Slot_CmbIndexChanged(int)));
	Init();
	
}

void Dlg_PlatComponent::Slot_CmbIndexChanged(int index)
{
	switch (index)
	{
	case PROTOCOL_TYPE_KEDA:
		ui.ledt_port_2->setText("20000");
		break;
	case PROTOCOL_TYPE_YUSHI:
		ui.ledt_port_2->setText("5196");
		break;
	case PROTOCOL_TYPE_FTP:
	case PROTOCOL_TYPE_ZHONGKE:
	case PROTOCOL_TYPE_YANGZHOU:
	case PROTOCOL_TYPE_SHAPINGBA:
	case PROTOCOL_TYPE_TIANJIN:
	case PROTOCOL_TYPE_SHENYANG:
		ui.ledt_port_2->setText("21");
		break;
	case PROTOCOL_TYPE_HAIKANG:
		ui.ledt_port_2->setText("8123");
		break;
	default:
		ui.ledt_port_2->setText("0");
		break;
	}
}

void Dlg_PlatComponent::Slot_CarInfoClicked(int nStatus)
{
	if (nStatus)
	{
		ui.chk_kakPic->setChecked(false);
		ui.chk_kakMergePic->setChecked(false);
		ui.chk_illegalPic->setChecked(false);
		ui.chk_illegalMeragePic->setChecked(false);
		ui.chk_faceSpecial->setChecked(false);
		ui.chk_carSpecial->setChecked(false);
		ui.chk_numberSpecial->setChecked(false);
		ui.chk_illegalVideo->setChecked(false);
		ui.chk_eventPic->setChecked(false);
	}
}

void Dlg_PlatComponent::Slot_CheckClicked(QWidget *pWidget)
{
	QCheckBox *pCheck = dynamic_cast<QCheckBox*>(pWidget);
	QString sRule = GetEditRule();
	QString sIndex = pCheck->property("SEQ").toString();
	int nSpit = ui.cmb_spit->currentIndex();
	QString sSpit = m_spit[nSpit];
	if (pCheck->isChecked())
	{
		sRule = QString("%1%2%3").arg(sRule).arg(sSpit).arg(sIndex);
	}
	else
	{
		int nPos = sRule.indexOf(sIndex);
		QString sLeft = sRule.left(nPos - 1);
		QString sRight = sRule.right(sRule.length() - nPos - 1);
		sRule = QString("%1%2").arg(sLeft).arg(sRight);
	}
	SetEditRule(sRule);
}

void Dlg_PlatComponent::Init()
{
	m_ledt_uuid = new QLineEdit;
	m_ledt_usr = new QLineEdit;
	m_ledt_pwd = new QLineEdit;
	m_ledt_overSavePath = new QLineEdit;
	m_ledt_illegalSavePath = new QLineEdit;
	m_ledt_nameRule = new QLineEdit;
	m_ledt_self = new QLineEdit;
	ui.gridLayout_3->addWidget(m_ledt_uuid, 5, 2, 1, 2);
	ui.gridLayout_3->addWidget(m_ledt_usr, 6, 2, 1, 2);
	ui.gridLayout_3->addWidget(m_ledt_pwd, 7, 2, 1, 2);
	ui.gridLayout_3->addWidget(m_ledt_overSavePath, 8, 2, 1, 2);
	ui.gridLayout_3->addWidget(m_ledt_illegalSavePath, 9, 2, 1, 2);
	ui.gridLayout_3->addWidget(m_ledt_nameRule, 10, 2, 1, 3);
	ui.gridLayout_3->addWidget(m_ledt_self, 11, 2, 1, 2);

	QStringList cmbList;
	cmbList << GBUTF8("科达平台") << GBUTF8("宇视平台") << GBUTF8("苏州世跃平台") << GBUTF8("海康平台")
		<< GBUTF8("峰火平台") << GBUTF8("公安视图库") << GBUTF8("重庆银江") << GBUTF8("本能平台")
		<< GBUTF8("重庆天网") << GBUTF8("重庆企裕") << GBUTF8("朗新平台") << GBUTF8("扬州平台")
		<< GBUTF8("中科平台") << GBUTF8("天津交管平台")<<GBUTF8("沈阳支队")<<GBUTF8("黄浦平台");

	ui.cmb_type_2->addItems(cmbList);
	ui.cmb_type_2->setView(new QListView);
	QStringList spitList;
	spitList << "_" << "#" << "+" << "-" << "*";
	ui.cmb_spit->addItems(spitList);
	ui.cmb_spit->setView(new QListView);
	m_spit.insert(0, "_");
	m_spit.insert(1, "#");
	m_spit.insert(2, "+");
	m_spit.insert(3, "-");
	m_spit.insert(4, "*");

	mapTypeIdx[ui.chk_kakPic] = EM_DATA_TYPE_KAKOU_ORIGINAL_PIC;
	mapTypeIdx[ui.chk_kakMergePic] = EM_DATA_TYPE_KAKOU_MERGE_PIC;
	mapTypeIdx[ui.chk_illegalPic] = EM_DATA_TYPE_ILLEGAL_ORIGINAL_PIC;
	mapTypeIdx[ui.chk_illegalMeragePic] = EM_DATA_TYPE_ILLEGAL_MERGE_PIC;
	mapTypeIdx[ui.chk_faceSpecial] = EM_DATA_TYPE_FACE_FEATURE;
	mapTypeIdx[ui.chk_carSpecial] = EM_DATA_TYPE_VEHICLE_FEATURE;
	mapTypeIdx[ui.chk_numberSpecial] = EM_DATA_TYPE_PLATE_FEATURE;
	mapTypeIdx[ui.chk_illegalVideo] = EM_DATA_TYPE_ILLEGAL_VIDEO;
	mapTypeIdx[ui.chk_carInfo] = EM_DATA_TYPE_VEHICLE_INFO;
	mapTypeIdx[ui.chk_snap] = EM_DATA_TYPE_EMPTY_PIC;
	mapTypeIdx[ui.chk_eventPic] = EM_DATA_TYPE_EVENT_PIC;


	m_map.insert("A", ui.chk_time);
	m_map.insert("B", ui.chk_cardNumber);
	m_map.insert("C", ui.chk_NumberColor);
	m_map.insert("D", ui.chk_cardSpeed);
	m_map.insert("E", ui.chk_cardColor);
	m_map.insert("F", ui.chk_cardLogo);
	m_map.insert("G", ui.chk_laneNumber);
	m_map.insert("H", ui.chk_deviceNumber);
	m_map.insert("I", ui.chk_checkCode);
	m_map.insert("J", ui.chk_cardType);
	m_map.insert("K", ui.chk_position);
	m_map.insert("L", ui.chk_illegalCode);
	m_map.insert("M", ui.chk_limitValue);
	m_map.insert("N", ui.chk_illegalType);
	m_map.insert("O", ui.chk_roadCode);
	m_map.insert("P", ui.chk_direction);
	m_map.insert("Q", ui.chk_picType);
	m_map.insert("R", ui.chk_picSum);
	m_map.insert("S", ui.chk_curPicNum);
	m_map.insert("T", ui.chk_numberType);
	m_map.insert("U", ui.chk_directionCode);
	m_map.insert("V", ui.chk_cardNumberPos);

	m_signalMapper = new QSignalMapper(this);

	QMap<QString, QCheckBox*>::const_iterator iter = m_map.constBegin();
	char ch = 'A';
	while (iter != m_map.constEnd())
	{
		QCheckBox *pCheck = iter.value();
		connect(pCheck, SIGNAL(stateChanged(int)), m_signalMapper, SLOT(map()));
		pCheck->setProperty("SEQ", QString(ch));
		m_signalMapper->setMapping(pCheck, pCheck);
		ch += 1;
		++iter;
	}
	connect(m_signalMapper, SIGNAL(mapped(QWidget *)),
		this, SLOT(Slot_CheckClicked(QWidget*)));

}

Dlg_PlatComponent::~Dlg_PlatComponent()
{
	DELETE_PTR(m_ledt_uuid);
	DELETE_PTR(m_ledt_usr);
	DELETE_PTR(m_ledt_pwd);
	DELETE_PTR(m_ledt_overSavePath);
	DELETE_PTR(m_ledt_illegalSavePath);
	DELETE_PTR(m_ledt_nameRule);
	DELETE_PTR(m_ledt_self);
}

void Dlg_PlatComponent::Slot_BtnClearClicked()
{
	m_ledt_nameRule->setText("");
}

void Dlg_PlatComponent::Slot_AddClicked()
{
	QString sSelf = m_ledt_self->text();
	if (sSelf.isEmpty())
	{
		return;
	}
	QString sNameRule = m_ledt_nameRule->text();
	sNameRule = QString("%1\\%2\\").arg(sNameRule).arg(sSelf);
	m_ledt_nameRule->setText(sNameRule);
}


void Dlg_PlatComponent::SetCheckEnable(bool is)
{
	ui.chk_enable->setChecked(is);
}

bool Dlg_PlatComponent::GetCheckEnable()
{
	return ui.chk_enable->isChecked();
}

void Dlg_PlatComponent::SetEditIP(QString sIP)
{
	ui.ledt_ip_2->setText(sIP);
}

QString Dlg_PlatComponent::GetEditIP()
{
	return ui.ledt_ip_2->text();
}

void Dlg_PlatComponent::SetEditPort(QString sPort)
{
	ui.ledt_port_2->setText(sPort);
}

QString Dlg_PlatComponent::GetEditPort()
{
	return ui.ledt_port_2->text();
}

void Dlg_PlatComponent::SetUUID(QString sId)
{
	m_ledt_uuid->setText(sId);
}

QString Dlg_PlatComponent::GetUUID()
{
	return m_ledt_uuid->text();
}

void Dlg_PlatComponent::SetEditUsrName(QString sName)
{
	m_ledt_usr->setText(sName);
}

QString Dlg_PlatComponent::GetEditUsrName()
{
	return m_ledt_usr->text();
}

void Dlg_PlatComponent::SetEditPassword(QString sPwd)
{
	m_ledt_pwd->setText(sPwd);
}

QString Dlg_PlatComponent::GetEditPassword()
{
	return m_ledt_pwd->text();
}

void Dlg_PlatComponent::SetEditOverSavePath(QString sPath)
{
	m_ledt_overSavePath->setText(sPath);
}

QString Dlg_PlatComponent::GetEditOverSavePath()
{
	return m_ledt_overSavePath->text();
}

unsigned int Dlg_PlatComponent::GetReported()
{
	m_dwDataType = 0;
	QMap<QCheckBox*, int>::const_iterator iter = mapTypeIdx.constBegin();
	while (iter != mapTypeIdx.constEnd())
	{
		if (iter.key()->isChecked())
		{
			m_dwDataType += iter.value();
		}
		++iter;
	}
	return m_dwDataType;
}

void Dlg_PlatComponent::SetReportedChecked(unsigned int dwDataType)
{
	m_dwDataType = dwDataType;
	QMap<QCheckBox*, int>::const_iterator iter = mapTypeIdx.constBegin();
	while (iter != mapTypeIdx.constEnd())
	{
		if (dwDataType & iter.value())
		{
			iter.key()->setChecked(true);
		}
		else
		{
			iter.key()->setChecked(false);
		}
		
		++iter;
	}

}

void Dlg_PlatComponent::SetProCmbIndex(int nIndex)
{
	ui.cmb_type_2->setCurrentIndex(nIndex);
}

int Dlg_PlatComponent::GetProCmbIndex()
{
	return ui.cmb_type_2->currentIndex();
}

void Dlg_PlatComponent::SetEditRule(QString sRule)
{
	m_spit.insert(0, "_");
	m_spit.insert(1, "#");
	m_spit.insert(2, "+");
	m_spit.insert(3, "-");
	m_spit.insert(4, "*");

	QStringList sList;
	for (int i = 0; i < 5; i++)
	{
		sList = sRule.split(m_spit[i]);
		if (sList.size() > 1)
		{
			break;
		}
	}
	int nLen = sList.size();
	for (int j = 0; j < nLen; j++)
	{
		QString sTemp = sList.at(j);
		if (sTemp.isEmpty() || (sTemp.length() > 1))
		{
			continue;
		}
		m_map[sTemp]->setChecked(true);
	}
	m_ledt_nameRule->setText(sRule);
}

QString Dlg_PlatComponent::GetEditRule()
{
	return m_ledt_nameRule->text();
}

void Dlg_PlatComponent::SetEditIllegalPath(QString sRule)
{
	m_ledt_illegalSavePath->setText(sRule);
}

QString Dlg_PlatComponent::GetEditIllegalPath()
{
	return m_ledt_illegalSavePath->text();
}

void Dlg_PlatComponent::SetEditSelf(QString sRule)
{
	m_ledt_self->setText(sRule);
}

QString Dlg_PlatComponent::GetEditSelf()
{
	return m_ledt_self->text();
}

