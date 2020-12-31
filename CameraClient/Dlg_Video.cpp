#include "Dlg_Video.h"
#include <QPainter>
#include "MgrData.h"
#include <QtWinExtras/QtWin>
#include <QBitmap>
#include "global.h"
HDC g_hdc;
void __stdcall VideoDrawCB3(PLAYERHANDLE hPlayHandle, unsigned long dwPaintDC, unsigned long dwFrmID, unsigned long dwParam)
{
	Dlg_Video *pDlg = (Dlg_Video *)dwParam;
	HDC hdc = (HDC)dwPaintDC;
	g_hdc = hdc;
	if (IsWindow((HWND)pDlg->winId()))
	{
		//pDlg->update();
		pDlg->DrawData(hdc);
	}
	
	
}

void Dlg_Video::SetLoopDrawData(TIPC_SetVideoLoopReq *ptVideoLoopInfo)
{
	m_bDraw = true;
	int nType;
	for (int i = 0; i < VIDEO_LOOP_NUM_MAX; i++)
	{
		if (ptVideoLoopInfo->tVideoLoop[i].bEnable == false)
		{
			continue;
		}

		nType = ptVideoLoopInfo->tVideoLoop[i].dwLoopLabel;
		if (ptVideoLoopInfo->tVideoLoop[i].dwMethod > 100)
		{
			TECHELON_S trapezium;
			trapezium.X1 = ptVideoLoopInfo->tVideoLoop[i].X1;
			trapezium.Y1 = ptVideoLoopInfo->tVideoLoop[i].Y1;
			trapezium.W1 = ptVideoLoopInfo->tVideoLoop[i].W1;
			trapezium.X2 = ptVideoLoopInfo->tVideoLoop[i].X2;
			trapezium.Y2 = ptVideoLoopInfo->tVideoLoop[i].Y2;
			trapezium.W2 = ptVideoLoopInfo->tVideoLoop[i].W2;
			m_pixDraw->AddTrapezium(&trapezium, nType, ptVideoLoopInfo->tVideoLoop[i].dwLoopLabel);
		}
		else
		{
			TRECTANGLE_S area;
			area.X = ptVideoLoopInfo->tVideoLoop[i].dwStartX;
			area.Y = ptVideoLoopInfo->tVideoLoop[i].dwStartY;
			area.W = ptVideoLoopInfo->tVideoLoop[i].dwWidth;
			area.H = ptVideoLoopInfo->tVideoLoop[i].dwHeight;
			m_pixDraw->AddRect(&area, nType, ptVideoLoopInfo->tVideoLoop[i].dwLoopLabel);
		}

		m_pixDraw->SetSelType(nType);

		switch (ptVideoLoopInfo->tVideoLoop[i].dwMethod)
		{
		case 0:
			m_pixDraw->SetColorByType(nType, COLOR_YELLOW);
			break;
		case 1:
			m_pixDraw->SetColorByType(nType, COLOR_BLUE);
			break;
		case 2:
			m_pixDraw->SetColorByType(nType, COLOR_GREEN);
			break;
		case 3:
			m_pixDraw->SetColorByType(nType, COLOR_PURPLE);
			break;
		case 4:
			m_pixDraw->SetColorByType(nType, COLOR_BLACK);
			break;
		case 5:
			m_pixDraw->SetColorByType(nType, COLOR_RED);
			break;
		default:
			break;
		}
	}
}




#include <QMutex>
void Dlg_Video::DrawData(HDC hdc)
{
	if (IsWindow((HWND)this->winId()))
	{
		m_pixDraw->DrawGraph(hdc);
	}
	
}

void Dlg_Video::ClearDrawData()
{
	m_bDraw = false;
	m_pixDraw->OnRedrawAll();
}

void Dlg_Video::SetPixSize(int w, int  h)
{
	m_nSceneHeight = h;
	m_nSceneWidth = w;
}

void Dlg_Video::SetParkScenceDrawData(TIPC_illgParkPara *ptParkScence)
{
	m_pixDraw->Clear();
	m_pixDraw->SetPixSize(m_nSceneWidth, m_nSceneHeight);
	TRoiArea_S apArea;
	for (int i = 0; i < ptParkScence->RoiAreaNum; i++)
	{
		apArea.dwRoiLabel = i;
		apArea.wPointNum = ptParkScence->tRoiArea[i].pointNum;
		memcpy(apArea.tPoint, ptParkScence->tRoiArea[i].tPoint, sizeof(TPoint_S)*apArea.wPointNum);
		m_pixDraw->AddRgn(&apArea, i, apArea.dwRoiLabel);
		m_pixDraw->SetColorByType(i, COLOR_YELLOW);
	}
}

Dlg_Video::Dlg_Video(QWidget *parent)
	: QWidget(parent), m_bDraw(false)
{
	setPalette(QPalette(Qt::white));
	setAutoFillBackground(true);
	connect(this, SIGNAL(signal_dataChanged()), this, SLOT(Slot_DataChanged()));

	m_pixDraw = new Dlg_PictureDraw(nullptr);
	m_pixDraw->resize(360, 240);
	//m_pixDraw->SetPixSize(360, 240);
	m_pixDraw->setVisible(false);
}

void Dlg_Video::SetPicDrawSize(int w, int h)
{
	m_pixDraw->resize(w, h);
}

void Dlg_Video::SetScenceDrawData(TIPC_GetSceneCfgRsp *ptSeceneCfg)
{
	m_pixDraw->Clear();
	m_pixDraw->SetPixSize(ptSeceneCfg->tMaxParam.dwMaxWidth, ptSeceneCfg->tMaxParam.dwMaxHeight);

	TRuleLine_S* apLine[scene::EMCUSTOMTYPE_COUNT];
	memset(apLine, 0, sizeof(apLine));
	apLine[scene::EMLINETYPE_LANE_LEFT] = &(ptSeceneCfg->tSceneCfg.tRdLineL);
	apLine[scene::EMLINETYPE_LANE_RIGHT] = &(ptSeceneCfg->tSceneCfg.tRdLineR);
	apLine[scene::EMLINETYPE_LANE_12] = &(ptSeceneCfg->tSceneCfg.tRdLine12);
	apLine[scene::EMLINETYPE_LANE_23] = &(ptSeceneCfg->tSceneCfg.tRdLine23);
	apLine[scene::EMLINETYPE_LANE_34] = &(ptSeceneCfg->tSceneCfg.tRdLine34);
	apLine[scene::EMLINETYPE_TRIGGER] = &(ptSeceneCfg->tSceneCfg.tScanLine);
	apLine[scene::EMLINETYPE_JUDGE_LEFT] = &(ptSeceneCfg->tSceneCfg.tTurnLeftLine);
	apLine[scene::EMLINETYPE_JUDGE_ZX] = &(ptSeceneCfg->tSceneCfg.tStraightLine);
	apLine[scene::EMLINETYPE_JUDGE_RIGHT] = &(ptSeceneCfg->tSceneCfg.tTurnRightLine);
	apLine[scene::EMLINETYPE_STOP1] = &(ptSeceneCfg->tSceneCfg.tStopLine[0]);
	apLine[scene::EMLINETYPE_STOP2] = &(ptSeceneCfg->tSceneCfg.tStopLine[1]);
	apLine[scene::EMLINETYPE_STOP3] = &(ptSeceneCfg->tSceneCfg.tStopLine[2]);
	apLine[scene::EMLINETYPE_STOP4] = &(ptSeceneCfg->tSceneCfg.tStopLine[3]);
	// 自定义的规则线
	for (int i = 0; i < USER_RULE_LINE_NUM_MAX; i++)
	{
		if (ptSeceneCfg->tSceneCfg.tRuleLine[i].bEnable)
		{
			apLine[scene::EMCUSTOMTYPE_1 + i] = &(ptSeceneCfg->tSceneCfg.tRuleLine[i]);
		}
	}

	for (int i = 0; i < scene::EMCUSTOMTYPE_COUNT; i++)
	{
		if (apLine[i] != NULL)
		{
			m_pixDraw->AddLine(&(apLine[i]->tLine), i, apLine[i]->LineLabel);
			if (i <= scene::EMLINETYPE_LANE_34)
			{
				m_pixDraw->SetColorByType(i, COLOR_BLUE);
			}
			else if (i >= scene::EMLINETYPE_TRIGGER && i < scene::EMLINETYPE_COUNT)
			{
				m_pixDraw->SetColorByType(i, COLOR_RED);
			}
			else if (i >= scene::EMAREATYPE_TRACK && i < scene::EMAREATYPE_COUNT)
			{
				m_pixDraw->SetColorByType(i, COLOR_YELLOW);
			}
			else if (i >= scene::EMCUSTOMTYPE_1 && i < scene::EMCUSTOMTYPE_COUNT)
			{
				m_pixDraw->SetColorByType(i, COLOR_PURPLE);
			}
		}
	}

	TRoiArea_S* apArea[5];
	memset(apArea, 0, sizeof(apArea));
	apArea[0] = &(ptSeceneCfg->tSceneCfg.tTrackArea);
	for (int i = 0; i < 5; i++)
	{
		if (apArea[i] != NULL)
		{
			m_pixDraw->AddRgn(apArea[i], i + scene::EMAREATYPE_TRACK, apArea[i]->dwRoiLabel);
			m_pixDraw->SetColorByType(i + scene::EMAREATYPE_TRACK, COLOR_YELLOW);
		}
	}

	for (int i = 0; i < USER_RULE_REGION_MAX; i++)
	{
		if (ptSeceneCfg->tSceneCfg.tRuleRegion[i].bEnable)
		{
			m_pixDraw->AddTrapezium(&(ptSeceneCfg->tSceneCfg.tRuleRegion[i].echelonArea), scene::EMCUSTOMTYPE_AREA_1 + i, ptSeceneCfg->tSceneCfg.tRuleRegion[i].Label);
			m_pixDraw->SetColorByType(scene::EMCUSTOMTYPE_AREA_1 + i, COLOR_PURPLE);
		}
	}
	for (int i = 0; i < USER_RULE_REGION_MAX; i++)
	{
		ptSeceneCfg->tSceneCfg.tPolygon[i].enable = (ptSeceneCfg->tSceneCfg.tPolygon[i].area.pointNum > 0 ? true : false);
	}

	for (int i = 0; i < USER_RULE_REGION_MAX - 3; i++)
	{
		if (ptSeceneCfg->tSceneCfg.tPolygon[i].area.pointNum > 0)
		{
			m_pixDraw->AddPolygon(&(ptSeceneCfg->tSceneCfg.tPolygon[i].area), scene::EMCUSTOMTYPE_POLYGON_1 + i,
				ptSeceneCfg->tSceneCfg.tPolygon[i].Label);
			m_pixDraw->SetColorByType(scene::EMCUSTOMTYPE_POLYGON_1 + i, COLOR_PURPLE);
		}
	}

	//行人高度

	m_pixDraw->AddPolygon(&(ptSeceneCfg->tSceneCfg.tPolygon[3].area), scene::EMAREATYPE_PERSON_WAIT,
		ptSeceneCfg->tSceneCfg.tPolygon[3].Label);
	m_pixDraw->SetColorByType(scene::EMAREATYPE_PERSON_WAIT, COLOR_YELLOW);

	//行人检测区域2
	m_pixDraw->AddPolygon(&(ptSeceneCfg->tSceneCfg.tPolygon[2].area), scene::EMAREATYPE_PERSON,
		ptSeceneCfg->tSceneCfg.tPolygon[2].Label);
	m_pixDraw->SetColorByType(scene::EMAREATYPE_PERSON, COLOR_YELLOW);

	//预警区域
	m_pixDraw->AddPolygon(&(ptSeceneCfg->tSceneCfg.tPolygon[1].area), scene::EMAREATYPE_WARNING,
		ptSeceneCfg->tSceneCfg.tPolygon[1].Label);
	m_pixDraw->SetColorByType(scene::EMAREATYPE_WARNING, COLOR_YELLOW);

}



void Dlg_Video::paintEvent(QPaintEvent *event)
{
	
}

void Dlg_Video::Slot_DataChanged()
{
	
}

Dlg_Video::~Dlg_Video()
{
	SAFE_DELETE(m_pixDraw);
	ControlPlayVideo(false);
}

QPaintEngine * Dlg_Video::paintEngine() const
{
	return nullptr;
}

void Dlg_Video::ControlPlayVideo(bool is)
{
	// 获取本地软件配置参数
	TLocalCfgInfo			m_tLocalCfgInfo;
	MgrData::getInstance()->GetMgrLocalCfg()->GetLocalCfg(&m_tLocalCfgInfo);

	if (m_tLocalCfgInfo.ispic)
	{
	}
	else
	{
		if (is == 1)
		{
			MgrData::getInstance()->GetMgrVideo()->PlayVideo((unsigned long)this->winId());
			MgrData::getInstance()->GetMgrVideo()->RegisterDrawCB(VideoDrawCB3, (unsigned long)this);
		}
		else
		{
			MgrData::getInstance()->GetMgrVideo()->RegisterDrawCB(NULL, NULL);
			MgrData::getInstance()->GetMgrVideo()->StopPlayVideo((unsigned long)this->winId());
		}
	}
}


