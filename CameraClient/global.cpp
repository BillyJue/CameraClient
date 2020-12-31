#include "global.h"
using namespace std;
// 解析视频线圈信息为字符串，用于窗口显示
namespace videoloop
{
	std::wstring GetListValue(TIPC_VidLoopCfg* pData, int em)
	{
		if (pData == NULL)
		{
			return std::wstring(TEXT(""));
		}

		wchar_t sz[129];
		wchar_t ws[129];
		memset(sz, 0, 129);
		memset(ws, 0, 129 * sizeof(wchar_t));

		switch (em)
		{
		case EVIDEOLOOP_SN:
		{
			wsprintf(ws, TEXT("%d"), pData->dwLoopLabel);
		}
			break;
		case EVIDEOLOOP_ENABLED:
		{
			if (pData->bEnable)
			{
				wsprintf(ws, TEXT("%s"), TEXT("是"));
			}
			else
			{
				wsprintf(ws, TEXT("%s"), TEXT("否"));
			}
		}
			break;
		case EVIDEOLOOP_STARTX:
		{
			wsprintf(ws, TEXT("%d"), pData->dwStartX);
		}
			break;
		case EVIDEOLOOP_STARTY:
		{
			wsprintf(ws, TEXT("%d"), pData->dwStartY);
		}
			break;
		case EVIDEOLOOP_WIDTH:
		{
			wsprintf(ws, TEXT("%d"), pData->dwWidth);
		}
			break;
		case EVIDEOLOOP_HEIGHT:
		{
			wsprintf(ws, TEXT("%d"), pData->dwHeight);
		}
			break;
		case EVIDEOLOOP_STARTX1:
		{
			wsprintf(ws, TEXT("%d"), pData->X1);
		}
			break;
		case EVIDEOLOOP_STARTY1:
		{
			wsprintf(ws, TEXT("%d"), pData->Y1);
		}
			break;
		case EVIDEOLOOP_WIDTH1:
		{
			wsprintf(ws, TEXT("%d"), pData->W1);
		}
			break;
		case EVIDEOLOOP_STARTX2:
		{
			wsprintf(ws, TEXT("%d"), pData->X2);
		}
			break;
		case EVIDEOLOOP_STARTY2:
		{
			wsprintf(ws, TEXT("%d"), pData->Y2);
		}
			break;
		case EVIDEOLOOP_WIDTH2:
		{
			wsprintf(ws, TEXT("%d"), pData->W2);
		}
			break;
		case EVIDEOLOOP_DIR:
		{
			wsprintf(ws, TEXT("%s"), DirectionMode(pData->dwDirection).c_str());
		}
			break;
		case EVIDEOLOOP_ANGLE:
		{
			wsprintf(ws, TEXT("%s"), AnalyAngleMode(pData->wPlateAngle).c_str());
		}
			break;
		case EVIDEOLOOP_TIME:
		{
			wsprintf(ws, TEXT("%d"), pData->dwPresentTime);
		}
			break;
		case EVIDEOLOOP_MODE:
		{
			wsprintf(ws, TEXT("%s"), AnalyChkType(pData->dwMethod).c_str());
		}
			break;
		case EVIDEOLOOP_TARGET_WIDHT:
		{
			wsprintf(ws, TEXT("%d"), pData->dwTargetWidth);
		}
			break;
		case EVIDEOLOOP_TARGET_HEIGTH:
		{
			wsprintf(ws, TEXT("%d"), pData->dwTargetHeight);
		}
			break;
		case EVIDEOLOOP_SENSITIVE:
		{
			wsprintf(ws, TEXT("%d"), pData->dwSensitive);
		}
			break;
		case EVIDEOLOOP_FILTERON:
		{
			wsprintf(ws, TEXT("%d"), pData->dwFilterOnTimes);
		}
			break;
		case EVIDEOLOOP_FILTEROFF:
		{
			wsprintf(ws, TEXT("%d"), pData->dwFilterOffTimes);
		}
			break;
		default:
			break;
		}
		return std::wstring(ws);
	}

	std::wstring  DirectionMode(u32 dwDir)
	{
		std::wstring Mode1[11] =
		{
			TEXT("车头"), TEXT("车尾"), TEXT("右转"), TEXT("直行"),
			TEXT("左转"), TEXT("右转三色"), TEXT("直行三色"), TEXT("左转三色"),
			TEXT("水平方向"), TEXT("垂直方向"), TEXT("任意方向")
		};

		int i = GetDirectionModeIndex(dwDir);

		return Mode1[i];
	}

	int GetDirectionModeIndex(u32 dwDir)
	{
		int i = 0;
		switch (dwDir)
		{
		case DETECT_HEAD_VIEW:
			i = 0;
			break;
		case DETECT_TAIL_VIEW:
			i = 1;
			break;
		case REDLAMP_PHASE_TURNRIGHT:
			i = 2;
			break;
		case REDLAMP_PHASE_STRA_AHEAD:
			i = 3;
			break;
		case REDLAMP_PHASE_TURNLEFT:
			i = 4;
			break;
		case REDLAMP_PHASE_THREECOLOR_TURNRIGHT:
			i = 5;
			break;
		case REDLAMP_PHASE_THREECOLOR_STRA_AHEAD:
			i = 6;
			break;
		case REDLAMP_PHASE_THREECOLOR_TURNLEFT:
			i = 7;
			break;
		case PEDES_MOVE_HORIZONTAL:
			i = 8;
			break;
		case PEDES_MOVE_VERTICAL:
			i = 9;
			break;
		case PEDES_MOVE_ANYDIR:
			i = 10;
			break;
		default:
			i = 0;
			break;
		}

		return i;
	}

	int GetDirectionModeValue(u32 index)
	{
		int i = 0;
		switch (index)
		{
		case 0:
			i = DETECT_HEAD_VIEW;
			break;
		case 1:
			i = DETECT_TAIL_VIEW;
			break;
		case 2:
			i = REDLAMP_PHASE_TURNRIGHT;
			break;
		case 3:
			i = REDLAMP_PHASE_STRA_AHEAD;
			break;
		case 4:
			i = REDLAMP_PHASE_TURNLEFT;
			break;
		case 5:
			i = REDLAMP_PHASE_THREECOLOR_TURNRIGHT;
			break;
		case 6:
			i = REDLAMP_PHASE_THREECOLOR_STRA_AHEAD;
			break;
		case 7:
			i = REDLAMP_PHASE_THREECOLOR_TURNLEFT;
			break;
		case 8:
			i = PEDES_MOVE_HORIZONTAL;
			break;
		case 9:
			i = PEDES_MOVE_VERTICAL;
			break;
		case 10:
			i = PEDES_MOVE_ANYDIR;
			break;
		default:
			i = 0;
			break;
		}

		return i;
	}

	std::wstring		AnalyAngleMode(u32 dwMode)
	{
		std::wstring Mode1[7] =
		{
			TEXT("龙门架(C杆)"), TEXT("A杆摄像左牌照右"), TEXT("A杆摄像右牌照左"), TEXT("角度方向不定"),
			TEXT("A杆摄像左牌照右严重变形"), TEXT("A杆摄像右牌照左严重变形"), TEXT("角度方向不定严重变形")
		};

		int index = GetAngleModeIndex(dwMode);
		return Mode1[index];
	}

	int GetAngleModeIndex(u32 dwMode)
	{
		int i = 0;
		switch (dwMode)
		{
		case PLATE_ANGLE_C:
			i = 0;
			break;
		case PLATE_ANGLE_A_R:
			i = 1;
			break;
		case PLATE_ANGLE_A_L:
			i = 2;
			break;
		case PLATE_ANGLE_UN:
			i = 3;
			break;
		case PLATE_ANGLE_A_R_R:
			i = 4;
			break;
		case PLATE_ANGLE_A_L_L:
			i = 5;
			break;
		case PLATE_ANGLE_A_UN_UN:
			i = 6;
			break;
		default:
			i = 0;
			break;
		}
		return i;
	}

	int GetAngleModeValue(u32 index)
	{
		int i = 0;
		switch (index)
		{
		case 0:
			i = PLATE_ANGLE_C;
			break;
		case 1:
			i = PLATE_ANGLE_A_R;
			break;
		case 2:
			i = PLATE_ANGLE_A_L;
			break;
		case 3:
			i = PLATE_ANGLE_UN;
			break;
		case 4:
			i = PLATE_ANGLE_A_R_R;
			break;
		case 5:
			i = PLATE_ANGLE_A_L_L;
			break;
		case 6:
			i = PLATE_ANGLE_A_UN_UN;
			break;
		default:
			i = PLATE_ANGLE_C;
			break;
		}
		return i;
	}

	std::wstring AnalyChkType(u32 dwType)
	{
		std::wstring Mode2[10] =
		{
			TEXT("运动检测"), TEXT("号牌检测"), TEXT("尾灯检测"), TEXT("头灯检测"),
			TEXT("简单运动检测"), TEXT("红绿灯检测"), TEXT("滞留检测"), TEXT("车头检测器"),
			TEXT("车尾检测器"), TEXT("行人检测器")
		};

		int index = GetMethodIndex(dwType);

		return Mode2[index];
	}

	int GetMethodIndex(u32 method)
	{
		int i = 0;
		switch (method)
		{
		case DETECT_METHOD_GRAY:
		case DETECT_METHOD_GRAY_ENHANCE:
			i = 0;
			break;
		case DETECT_METHOD_PLATE:
		case DETECT_METHOD_PLATE_ENHANCE:
			i = 1;
			break;
		case DETECT_METHOD_TAILLAMP:
		case DETECT_METHOD_TAILLAMP_ENHANCE:
			i = 2;
			break;
		case DETECT_METHOD_HEADLAMP:
		case DETECT_METHOD_HEADLAMP_ENHANCE:
			i = 3;
			break;
		case DETECT_METHOD_GRAY_SIMPLE:
		case DETECT_METHOD_GRAY_SIMPLE_ENHANCE:
			i = 4;
			break;
		case DETECT_METHOD_REDLAMP:
			i = 5;
			break;
		case DETECT_METHOD_RETENTION:
			i = 6;
			break;
		case DETECT_METHOD_VEH_HEAD:
			i = 7;
			break;
		case DETECT_METHOD_VEH_TAIL:
			i = 8;
			break;
		case DETECT_METHOD_PEDES:
			i = 9;
			break;
		default:
			i = 0;
			break;
		}
		return i;
	}

	int GetMethodValue(u32 index, bool bTrapezium)
	{
		int method;
		switch (index)
		{
		case 0:
			if (bTrapezium)
			{
				method = DETECT_METHOD_GRAY_ENHANCE;
			}
			else
			{
				method = DETECT_METHOD_GRAY;
			}
			break;
		case 1:
			if (bTrapezium)
			{
				method = DETECT_METHOD_PLATE_ENHANCE;
			}
			else
			{
				method = DETECT_METHOD_PLATE;
			}
			break;
		case 2:
			if (bTrapezium)
			{
				method = DETECT_METHOD_TAILLAMP_ENHANCE;
			}
			else
			{
				method = DETECT_METHOD_TAILLAMP;
			}
			break;
		case 3:
			if (bTrapezium)
			{
				method = DETECT_METHOD_HEADLAMP;
			}
			else
			{
				method = DETECT_METHOD_HEADLAMP_ENHANCE;
			}
			break;
		case 4:
			if (bTrapezium)
			{
				method = DETECT_METHOD_GRAY_SIMPLE_ENHANCE;
			}
			else
			{
				method = DETECT_METHOD_GRAY_SIMPLE;
			}
			break;
		case 5:
			method = DETECT_METHOD_REDLAMP;
			break;
		case 6:
			method = DETECT_METHOD_RETENTION;
			break;
		case 7:
			method = DETECT_METHOD_VEH_HEAD;
			break;
		case 8:
			method = DETECT_METHOD_VEH_TAIL;
			break;
		case 9:
			method = DETECT_METHOD_PEDES;
			break;
		default:
			method = DETECT_METHOD_PLATE;
			break;
		}
		return method;
	}
};

namespace coilloop
{
	std::wstring GetListValue(TIPC_CoilLoopCfg* pData, int em)
	{
		if (pData == NULL)
		{
			return std::wstring(TEXT(""));
		}

		wchar_t sz[129];
		wchar_t ws[129];
		memset(sz, 0, 129);
		memset(ws, 0, 129 * sizeof(wchar_t));

		switch (em)
		{
		case 0: //编号
			wsprintf(ws, TEXT("%d"), pData->dwCoilLabel);
			break;
		case 1: //使能
			if (pData->bEnable)
			{
				wsprintf(ws, TEXT("%s"), TEXT("是"));
			}
			else
			{
				wsprintf(ws, TEXT("%s"), TEXT("否"));
			}
			break;
		case 2: // 串口通道号
			if (pData->dwSerialChnId >= 0 && pData->dwSerialChnId < 8)
			{
				std::wstring Mode[8] =
				{
					TEXT("串口1"), TEXT("串口2"), TEXT("串口3"), TEXT("串口4"),
					TEXT("IO1"), TEXT("IO2"), TEXT("IO3"), TEXT("IO4")
				};
				return Mode[pData->dwSerialChnId];
			}
			wsprintf(ws, TEXT("%d"), pData->dwSerialChnId);
			break;
		case 3: //线圈通道号
			if (pData->dwCoilChnId >= 0 && pData->dwCoilChnId < 9)
			{
				std::wstring Mode2[9] =
				{
					TEXT("通道1"), TEXT("通道2"), TEXT("通道3"), TEXT("通道4"),
					TEXT("通道5"), TEXT("通道6"), TEXT("通道7"), TEXT("通道8"),
					TEXT("通道9")
				};
				return Mode2[pData->dwCoilChnId];
			}
			wsprintf(ws, TEXT("%d"), pData->dwCoilChnId);
			break;
		case 4: //方向
			if (pData->dwDirection - 1 >= 0 && pData->dwDirection - 1 < 2)
			{
				std::wstring Mode3[2] =
				{
					TEXT("车头"), TEXT("车尾")
				};
				return Mode3[pData->dwDirection - 1];
			}
			wsprintf(ws, TEXT("%d"), pData->dwDirection);
			break;
		case 5: //线圈间距
			wsprintf(ws, TEXT("%d"), pData->dwV_distance);
			break;
		case 6: //速度修正系数
			char buf[24];
			sprintf(buf, "%f", pData->fCorrectRate);
			MultiByteToWideChar(CP_ACP, 0, buf, -1, ws, 128);
			break;
		case 7: //关联IO输出
			if (pData->dwIOLabel >= 0 && pData->dwIOLabel < 2)
			{
				std::wstring Mode4[4] =
				{
					TEXT("IO1"), TEXT("IO2"), TEXT("IO3"), TEXT("IO4")
				};
				return Mode4[pData->dwIOLabel];
			}
			wsprintf(ws, TEXT("%d"), pData->dwIOLabel);
			break;
		case 8: //对应车道
		{
			std::wstring Mode5[5] =
			{
				TEXT("0"), TEXT("车道1"), TEXT("车道2"), TEXT("车道3"), TEXT("车道4")
			};
			if (pData->dwPhyLaneNo > 4 || pData->dwPhyLaneNo < 1)
			{
				return Mode5[0];
			}
			else
			{
				return Mode5[pData->dwPhyLaneNo];
			}
		}
			break;
		case 9: //线圈类型
		{
			std::wstring Mode6[3] =
			{
				TEXT("前线圈"), TEXT("中线圈"), TEXT("后线圈")
			};
			if (pData->dwCoilType > 2)
			{
				return Mode6[0];
			}
			else
			{
				return Mode6[pData->dwCoilType];
			}
		}
			break;
		default:
			break;
		}
		return std::wstring(ws);
	}
};

namespace redarloop
{
	std::wstring		GetListValue(TIPC_RadarLoopCfg* pData, int em)
	{
		if (pData == NULL)
		{
			return std::wstring(TEXT(""));
		}

		wchar_t sz[129];
		wchar_t ws[129];
		memset(sz, 0, 129);
		memset(ws, 0, 129 * sizeof(wchar_t));

		switch (em)
		{
		case EREDARLOOP_SN: //编号
			wsprintf(ws, TEXT("%d"), pData->dwLoopLabel);
			break;
		case EREDARLOOP_ENABLED: //使能
			if (pData->bEnable)
			{
				wsprintf(ws, TEXT("%s"), TEXT("是"));
			}
			else
			{
				wsprintf(ws, TEXT("%s"), TEXT("否"));
			}
			break;
		case EREDARLOOP_SERIAL: // 串口通道号
			if (pData->dwSerialChnId >= 0 && pData->dwSerialChnId < 4)
			{
				std::wstring Mode[4] =
				{
					TEXT("串口1"), TEXT("串口2"), TEXT("串口3"), TEXT("串口4")
				};
				return Mode[pData->dwSerialChnId];
			}
			wsprintf(ws, TEXT("%d"), pData->dwSerialChnId);
			break;
		case EREDARLOOP_TYPE: //雷达类型
			if (pData->dwRadarType >= 0 && pData->dwRadarType < 3)
			{
				std::wstring Mode2[3] =
				{
					TEXT("单目标雷达"), TEXT("多目标雷达"), TEXT("声呐雷达")
				};
				return Mode2[pData->dwRadarType];
			}
			wsprintf(ws, TEXT("%d"), pData->dwRadarType);
			break;
			//case EREDARLOOP_MODE: //工作模式
			//	if (pData->dwRadarMode >= 0 && pData->dwRadarMode < 2)
			//	{
			//		wstring Mode3[2]=
			//		{
			//			TEXT("触发模式"), TEXT("连续模式")
			//		};
			//		return Mode3[pData->dwRadarMode];
			//	}
			//	wsprintf(ws, TEXT("%d"), pData->dwRadarMode);
			//	break;
		case EREDARLOOP_DISTANCE: //雷达和相机距离
		{
			char buf[24];
			sprintf(buf, "%f", pData->fH_distance);
			MultiByteToWideChar(CP_ACP, 0, buf, -1, ws, 128);
		}
			break;
		case EREDARLOOP_SPEED: //速度修正系数
		{
			char buf[24];
			sprintf(buf, "%f", pData->fCorrectRate);
			MultiByteToWideChar(CP_ACP, 0, buf, -1, ws, 128);
		}
			break;
		case EREDARLOOP_LANE: //对应车道
		{
			std::wstring Mode5[5] =
			{
				TEXT("0"), TEXT("车道1"), TEXT("车道2"), TEXT("车道3"), TEXT("车道4")
			};
			if (pData->dwPhyLaneNo > 4 || pData->dwPhyLaneNo < 1)
			{
				return Mode5[0];
			}
			else
			{
				return Mode5[pData->dwPhyLaneNo];
			}
		}
			break;
		default:
			break;
		}
		return std::wstring(ws);
	}
};

namespace RedLampSig
{
	std::wstring		GetListValue(TIPC_RedLampSigLoop* pData, int em)
	{
		if (pData == NULL)
		{
			return std::wstring(TEXT(""));
		}

		wchar_t sz[129];
		wchar_t ws[129];
		memset(sz, 0, 129);
		memset(ws, 0, 129 * sizeof(wchar_t));

		switch (em)
		{
		case 0: //编号
			wsprintf(ws, TEXT("%d"), pData->dwLoopLabel);
			break;
		case 1: //使能
			if (pData->bEnable)
			{
				wsprintf(ws, TEXT("%s"), TEXT("是"));
			}
			else
			{
				wsprintf(ws, TEXT("%s"), TEXT("否"));
			}
			break;
		case 2: // 串口通道号
			if (pData->dwSerialChnId >= 0 && pData->dwSerialChnId < 4)
			{
				std::wstring Mode[4] =
				{
					TEXT("串口1"), TEXT("串口2"), TEXT("串口3"), TEXT("串口4")
				};
				return Mode[pData->dwSerialChnId];
			}
			wsprintf(ws, TEXT("%d"), pData->dwSerialChnId);
			break;
		case 3: // 通道号
			if (pData->dwChnId >= 0 && pData->dwChnId < 16)
			{
				std::wstring Mode[16] =
				{
					TEXT("通道1"), TEXT("通道2"), TEXT("通道3"), TEXT("通道4"),
					TEXT("通道5"), TEXT("通道6"), TEXT("通道7"), TEXT("通道8"),
					TEXT("通道9"), TEXT("通道10"), TEXT("通道11"), TEXT("通道12"),
					TEXT("通道13"), TEXT("通道14"), TEXT("通道15"), TEXT("通道16"),
				};
				return Mode[pData->dwChnId];
			}
			wsprintf(ws, TEXT("%d"), pData->dwChnId);
			break;
		case 4: // 方向
			if (pData->dwDetPhase >= 0 && pData->dwDetPhase < 4)
			{
				std::wstring Mode[4] =
				{
					TEXT("无方向"), TEXT("左转红灯"), TEXT("直行红灯"), TEXT("右转红灯")
				};
				return Mode[pData->dwDetPhase];
			}
			wsprintf(ws, TEXT("%d"), pData->dwDetPhase);
			break;
		default:
			break;
		}

		return std::wstring(ws);
	}
};


namespace redincrease
{
	std::wstring		GetListValue(TIPC_RedSigEnhance* pData, int em)
	{
		if (pData == NULL)
		{
			return std::wstring(TEXT(""));
		}

		wchar_t sz[129];
		wchar_t ws[129];
		memset(sz, 0, 129);
		memset(ws, 0, 129 * sizeof(wchar_t));

		switch (em)
		{
		case 0:  // 使能
			if (pData->bEnable)
			{
				wsprintf(ws, TEXT("%s"), TEXT("是"));
			}
			else
			{
				wsprintf(ws, TEXT("%s"), TEXT("否"));
			}
			break;
		case 1:  // x
			wsprintf(ws, TEXT("%d"), pData->dwStartX);
			break;
		case 2:  // y
			wsprintf(ws, TEXT("%d"), pData->dwStartY);
			break;
		case 3:  // w
			wsprintf(ws, TEXT("%d"), pData->dwWidth);
			break;
		case 4:  // h
			wsprintf(ws, TEXT("%d"), pData->dwHeight);
			break;
		case 5:  // 最小像素
			wsprintf(ws, TEXT("%d"), pData->dwPixMin);
			break;
		case 6:  // 白天增强等级
			//if (pData->dwLevelDay >= 0 && pData->dwLevelDay < 3)
			//{
			//	wstring Mode1[3]=
			//	{
			//		TEXT("1"), TEXT("2"),TEXT("3")
			//	};
			//	return Mode1[pData->dwLevelDay];
			//}
			wsprintf(ws, TEXT("%d"), pData->dwLevelDay);
			break;
		case 7:  // 夜晚增强等级
			//if (pData->dwLevelNight >= 0 && pData->dwLevelNight < 3)
			//{
			//	wstring Mode1[3]=
			//	{
			//		TEXT("1"), TEXT("2"),TEXT("3")
			//	};
			//	return Mode1[pData->dwLevelNight];
			//}
			wsprintf(ws, TEXT("%d"), pData->dwLevelNight);
			break;
		}
		return std::wstring(ws);
	}
};

namespace recogarea
{
	std::wstring		GetListValue(TRecogAreaParam* pData, int em)
	{
		if (pData == NULL)
		{
			return std::wstring(TEXT(""));
		}

		wchar_t sz[129];
		wchar_t ws[129];
		memset(sz, 0, 129);
		memset(ws, 0, 129 * sizeof(wchar_t));

		switch (em)
		{
		case 0: // 编号
			wsprintf(ws, TEXT("%d"), pData->dwLabel);
			break;
		case 1: // 使能
			if (pData->bEnable)
			{
				wsprintf(ws, TEXT("%s"), TEXT("是"));
			}
			else
			{
				wsprintf(ws, TEXT("%s"), TEXT("否"));
			}
			break;
		case 2: // X1
			wsprintf(ws, TEXT("%d"), pData->X1);
			break;
		case 3: // Y1
			wsprintf(ws, TEXT("%d"), pData->Y1);
			break;
		case 4: // W1
			wsprintf(ws, TEXT("%d"), pData->W1);
			break;
		case 5: // X2
			wsprintf(ws, TEXT("%d"), pData->X2);
			break;
		case 6: // Y2
			wsprintf(ws, TEXT("%d"), pData->Y2);
			break;
		case 7: // W2
			wsprintf(ws, TEXT("%d"), pData->W2);
			break;
		case 8: // 号牌宽度
			wsprintf(ws, TEXT("%d"), pData->dwPlateWidth);
			break;
		case 9: // 号牌角度
		{
			switch (pData->dwPlateAngle)
			{
			case 0:
				wsprintf(ws, TEXT("龙门架(C杆)"));
				break;
			case 1:
				wsprintf(ws, TEXT("A杆摄像左牌照右"));
				break;
			case 2:
				wsprintf(ws, TEXT("A杆摄像右牌照左"));
				break;
			case 3:
				wsprintf(ws, TEXT("角度方向不定"));
				break;
			case 4:
				wsprintf(ws, TEXT("A杆摄像左牌照右严重变形"));
				break;
			case 5:
				wsprintf(ws, TEXT("A杆摄像右牌照左严重变形"));
				break;
			case 6:
				wsprintf(ws, TEXT("角度方向不定严重变形"));
				break;
			default:
				wsprintf(ws, TEXT("龙门架(C杆)"));
				break;
			}
		}
			break;
		case 10: // 车牌方向
		{
			switch (pData->dwShootRear)
			{
			case 0:
				wsprintf(ws, TEXT("车头"));
				break;
			case 1:
				wsprintf(ws, TEXT("车尾"));
				break;
			default:
				wsprintf(ws, TEXT("车头"));
				break;
			}
		}
			break;
		default:
			break;
		}
		return std::wstring(ws);
	}
};


namespace data2
{
	void Line2DrawData(TLine_S* pLine, tagDrawData* pData)
	{
		if (pData == NULL || pLine == NULL) return;

		pData->nDrawMode = EMDRAWMODE_LINE;
		pData->nPtNum = 2;
		pData->x[0] = pLine->X1;
		pData->x[1] = pLine->X2;
		pData->y[0] = pLine->Y1;
		pData->y[1] = pLine->Y2;
	}

	void DrawData2Line(TLine_S* pLine, tagDrawData* pData)
	{
		if (pData == NULL || pLine == NULL) return;

		memset(pLine, 0, sizeof(TLine_S));
		if (pData->nDrawMode == EMDRAWMODE_LINE)
		{
			pLine->X1 = pData->x[0];
			pLine->X2 = pData->x[1];
			pLine->Y1 = pData->y[0];
			pLine->Y2 = pData->y[1];
		}
	}


	void Rect2DrawData(TRECTANGLE_S* pRect, tagDrawData* pData)
	{
		if (pData == NULL || pRect == NULL) return;

		pData->nDrawMode = EMDRAWMODE_RECT;
		pData->nPtNum = 4;
		pData->x[0] = pRect->X;
		pData->y[0] = pRect->Y;
		pData->x[1] = pRect->X + pRect->W;
		pData->y[1] = pRect->Y;
		pData->x[2] = pRect->X + pRect->W;
		pData->y[2] = pRect->Y + pRect->H;
		pData->x[3] = pRect->X;
		pData->y[3] = pRect->Y + pRect->H;
	}

	void DrawData2Rect(TRECTANGLE_S* pRect, tagDrawData* pData)
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

	void Rect2Rgn4(TRECTANGLE_S* pRect, TRoiArea_S* pRgn)
	{
		if (pRect == NULL || pRgn == NULL) return;

		memset(pRgn->tPoint, 0, sizeof(pRgn->tPoint));
		pRgn->wPointNum = 4;
		pRgn->tPoint[0].X = pRect->X;
		pRgn->tPoint[0].Y = pRect->Y;
		pRgn->tPoint[1].X = pRect->X + pRect->W;
		pRgn->tPoint[1].Y = pRect->Y;
		pRgn->tPoint[2].X = pRect->X + pRect->W;
		pRgn->tPoint[2].Y = pRect->Y + pRect->H;
		pRgn->tPoint[3].X = pRect->X;
		pRgn->tPoint[3].Y = pRect->Y + pRect->H;
	}
	void Rgn42Rect(TRECTANGLE_S* pRect, TRoiArea_S* pRgn)
	{
		if (pRect == NULL || pRgn == NULL) return;

		memset(pRect, 0, sizeof(TRECTANGLE_S));
		pRect->X = pRgn->tPoint[0].X;
		pRect->Y = pRgn->tPoint[0].Y;
		pRect->W = pRgn->tPoint[2].X - pRgn->tPoint[0].X;
		pRect->H = pRgn->tPoint[2].Y - pRgn->tPoint[0].Y;
	}

	void DrawData2Rgn(TRoiArea_S* pRgn, tagDrawData* pData)
	{
		if (pData == NULL || pRgn == NULL) return;

		memset(pRgn, 0, sizeof(TRoiArea_S));
		pRgn->bEnable = true;
		pRgn->wPointNum = pData->nPtNum;
		for (int i = 0; i < pData->nPtNum; i++)
		{
			if (i > 8) break;
			pRgn->tPoint[i].X = pData->x[i];
			pRgn->tPoint[i].Y = pData->y[i];
		}
	}

	void Rgn2DrawData(TRoiArea_S* pRgn, tagDrawData* pData)
	{
		if (pData == NULL || pRgn == NULL) return;

		pData->Empty();
		pData->nDrawMode = EMDRAWMODE_RGN;
		pData->nPtNum = pRgn->wPointNum;
		for (int i = 0; i < pRgn->wPointNum; i++)
		{
			if (i > 8) break;
			pData->x[i] = pRgn->tPoint[i].X;
			pData->y[i] = pRgn->tPoint[i].Y;
		}
	}


	void DrawData2Rgn4(TRoiArea_S* pRgn, tagDrawData* pData)
	{
		if (pData == NULL || pRgn == NULL) return;

		memset(pRgn, 0, sizeof(TRoiArea_S));
		pRgn->bEnable = true;
		pRgn->wPointNum = pData->nPtNum;
		for (int i = 0; i < pData->nPtNum; i++)
		{
			if (i > 4) break;
			pRgn->tPoint[i].X = pData->x[i];
			pRgn->tPoint[i].Y = pData->y[i];
		}
	}

	void Rgn42DrawData(TRoiArea_S* pRgn, tagDrawData* pData)
	{
		if (pData == NULL || pRgn == NULL) return;

		pData->Empty();
		pData->nDrawMode = EMDRAWMODE_RGN_4;
		pData->nPtNum = pRgn->wPointNum;
		for (int i = 0; i < pRgn->wPointNum; i++)
		{
			if (i > 4) break;
			pData->x[i] = pRgn->tPoint[i].X;
			pData->y[i] = pRgn->tPoint[i].Y;
		}
	}

}

int GetSpecifyBitValue(Uint32 dwSrc, int nIndex)
{
	Uint32 dw2 = ((int)pow(2.0, nIndex));
	dwSrc = dwSrc & dw2;
	dwSrc = dwSrc >> nIndex;

	return dwSrc;
}

u32 SetSpecifyBitValue(Uint32& dwSrcOut, Uint32 dwValue, int nIndex)
{
	Uint32 dw2 = ((int)pow(2.0, nIndex));

	if (dwValue == 0)
	{
		dw2 = ~dw2;
		dwSrcOut = dwSrcOut & dw2;
	}
	else
	{
		dwSrcOut = dwSrcOut | dw2;
	}

	return dwSrcOut;
}

// 解析逻辑车道信息为字符串，用于窗口显示
namespace logiclane
{
	wstring		GetListValue(TIPC_LogicLane* pData, int em)
	{
		if (pData == NULL)
		{
			return wstring(TEXT(""));
		}

		wchar_t sz[129];
		wchar_t ws[129];
		memset(sz, 0, 129);
		memset(ws, 0, 129 * sizeof(wchar_t));
		//wsprintf(ws2, TEXT("%s"),TEXT("卡口事件"));

		switch (em)
		{
		case EMLOGICLANE_SN:
		{
			wsprintf(ws, TEXT("%d"), pData->dwLgRdId);
		}
			break;
		case EMLOGICLANE_ENABLED:
		{
			if (pData->bEnable)
			{
				wsprintf(ws, TEXT("%s"), TEXT("是"));
			}
			else
			{
				wsprintf(ws, TEXT("%s"), TEXT("否"));
			}
		}
			break;
		case EMLOGICLANE_CHKTYPE:
		{
			if (pData->dwLgRdType == 0 && pData->dwExtLgRdType == 0)
			{
				return TEXT("未识别");
			}
			int index = AnalyCheckType2Index(pData->dwLgRdType);
			if (index == MAX_LOGICLANE_TYPE_NUM)
			{
				index = AnalyCheckExtType2Index(pData->dwExtLgRdType);
			}
			wsprintf(ws, TEXT("%s"), AnalyChkType(index).c_str());
		}
			break;
		case EMLOGICLANE_SENSITIVITY:
		{
			wsprintf(ws, TEXT("%d"), (int)pData->dwSensitivity);
		}
			break;
		case EMLOGICLANE_IDENTID:
		{
			wsprintf(ws, TEXT("%d"), pData->dwRecAreaID);
		}
			break;
		case EMLOGICLANE_LANEID:
		{
			wsprintf(ws, TEXT("%d"), pData->dwLaneId);
		}
			break;
		case EMLOGICLANE_FILLLIGHT_DAY:
		{
			u32 temp = pData->dwDaySnapLight & 0xFF;
			wsprintf(ws, TEXT("%s"), AnalyLightMode(temp).c_str());
		}
			break;
		case EMLOGICLANE_FILLLIGHT_NIGHT:
		{
			u32 temp = pData->dwNightSnapLight & 0xFF;
			wsprintf(ws, TEXT("%s"), AnalyLightMode(temp).c_str());
		}
			break;
		case EMLOGICLANE_EXPOSURE_PEOPLE:
		{
			if (pData->bFaceMode)
			{
				wsprintf(ws, TEXT("%s"), TEXT("是"));
			}
			else
			{
				wsprintf(ws, TEXT("%s"), TEXT("否"));
			}
		}
			break;

		case EMLOGICLANE_ILLEGALCODE:
		{
			wsprintf(ws, TEXT("%d"), pData->dwWzCode);
		}
			break;

		}

		return wstring(ws);
	}

	// 解析逻辑车道类型
	int AnalyCheckType2Index(u32 dwMode)
	{
		int nIndex = 0;
		switch (dwMode)
		{
		case LOGIC_ROAD_TYPE_KK:
			nIndex = 0; break;
		case LOGIC_ROAD_TYPE_CS:
			nIndex = 1; break;
		case LOGIC_ROAD_TYPE_ZD:
			nIndex = 2; break;
		case LOGIC_ROAD_TYPE_BD:
			nIndex = 3; break;
		case LOGIC_ROAD_TYPE_HD:
			nIndex = 4; break;
		case LOGIC_ROAD_TYPE_DX:
			nIndex = 5; break;
		case LOGIC_ROAD_TYPE_JZ:
			nIndex = 6; break;
		case LOGIC_ROAD_TYPE_NX_2:
			nIndex = 7; break;
		case LOGIC_ROAD_TYPE_YX_2:
			nIndex = 8; break;
		case LOGIC_ROAD_TYPE_WFDT:
			nIndex = 9; break;
		case LOGIC_ROAD_TYPE_LINE_STOP:
			nIndex = 10; break;
		case LOGIC_ROAD_TYPE_crossing_retention:
			nIndex = 11; break;
		case LOGIC_ROAD_TYPE_pedestrian_crossing_stop:
			nIndex = 12; break;
		case LOGIC_ROAD_TYPE_NO_comity_pedestrian:
			nIndex = 13; break;
		case LOGIC_ROAD_TYPE_VEH_BACK_OFF:
			nIndex = 14; break;
		case LOGIC_ROAD_TYPE_swerve_No_comity_straight:
			nIndex = 15; break;
		case LOGIC_ROAD_TYPE_pedestrian_run_red_light:
			nIndex = 16; break;
		case LOGIC_ROAD_TYPE_VEH_DISTANCE:
			nIndex = 23; break;
		case LOGIC_ROAD_TYPE_PERSON_ILLEGAL:
			nIndex = 24; break;
		case LOGIC_ROAD_TYPE_LIMIT_DRIVING:
			nIndex = 25; break;
		case LOGIC_ROAD_TYPE_CAR_HORNS:
			nIndex = 26; break;
		case LOGIC_ROAD_TYPE_VEH_OVERHEIGHT:
			nIndex = 27; break;
		default:
			nIndex = MAX_LOGICLANE_TYPE_NUM; break;
		}

		return nIndex;
	}

	int AnalyCheckExtType2Index(u32 dwMode)
	{
		int nIndex = 0;
		switch (dwMode)
		{
		case EXT_LOGIC_ROAD_TYPE_ZYFJDCD:
			nIndex = 17; break;
		case EXT_LOGIC_ROAD_TYPE_ZYYJCD:
			nIndex = 18; break;
		case EXT_LOGIC_ROAD_TYPE_ZYGJCD:
			nIndex = 19; break;
		case EXT_LOGIC_ROAD_TYPE_ZYXXCCD:
			nIndex = 20; break;
		case EXT_LOGIC_ROAD_TYPE_JZHCTX:
			nIndex = 21; break;
		case EXT_LOGIC_ROAD_DWXZ:
			nIndex = 22; break;
		case EXT_LOGIC_ROAD_TYPE_FJDCZYJDCD:
			nIndex = 27; break;
		case EXT_LOGIC_ROAD_TYPE_FJDCZR:
			nIndex = 28; break;
		case EXT_LOGIC_ROAD_TYPE_FJDJZSJ:
			nIndex = 29; break;
		case EXT_LOGIC_ROAD_TYPE_YXYJ:
			nIndex = 30; break;
		default:
			nIndex = MAX_LOGICLANE_TYPE_NUM; break;
		}

		return nIndex;
	}

	wstring	AnalyChkType(u32 dwType)
	{
		wstring awsRaodType[MAX_LOGICLANE_TYPE_NUM + 2] =
		{
			TEXT("卡口"), TEXT("超速"), TEXT("不按规定行驶"), TEXT("实线变道"), TEXT("闯红灯"),
			TEXT("不按导向行驶"), TEXT("禁止左转"), TEXT("逆行"), TEXT("压实线"), TEXT("违法掉头"),
			TEXT("越线停车"), TEXT("路口滞留"), TEXT("黄网格停车"), TEXT("机动车未礼让行人"),
			TEXT("快速路违法倒车"), TEXT("转弯未让直行"), TEXT("行人闯红灯"), TEXT("机动车占用非机动车道"),
			TEXT("占用应急车道"), TEXT("占用公交车道"), TEXT("大型车占用小型车车道"), TEXT("禁止货车车辆通行"),
			TEXT("大弯小转"), TEXT("未保持行车安全距离"), TEXT("司乘违法"), TEXT("车辆限行"), TEXT("违章鸣笛"),
			TEXT("超高"), TEXT("非机动车占用机动车道"), TEXT("非机动车载人"), TEXT("非机动车加装伞具"), TEXT("越线预警"),
			TEXT("未识别")
		};

		return awsRaodType[dwType];
	}

	u32	AnalyIndex2CheckType(int nIndex)
	{
		u32 dwMode = 0;
		switch (nIndex)
		{
		case 0:
			dwMode = LOGIC_ROAD_TYPE_KK; break;
		case 1:
			dwMode = LOGIC_ROAD_TYPE_CS; break;
		case 2:
			dwMode = LOGIC_ROAD_TYPE_ZD; break;
		case 3:
			dwMode = LOGIC_ROAD_TYPE_BD; break;
		case 4:
			dwMode = LOGIC_ROAD_TYPE_HD; break;
		case 5:
			dwMode = LOGIC_ROAD_TYPE_DX; break;
		case 6:
			dwMode = LOGIC_ROAD_TYPE_JZ; break;
		case 7:
			dwMode = LOGIC_ROAD_TYPE_NX_2; break;
		case 8:
			dwMode = LOGIC_ROAD_TYPE_YX_2; break;
		case 9:
			dwMode = LOGIC_ROAD_TYPE_WFDT; break;
		case 10:
			dwMode = LOGIC_ROAD_TYPE_LINE_STOP; break;
		case 11:
			dwMode = LOGIC_ROAD_TYPE_crossing_retention; break;
		case 12:
			dwMode = LOGIC_ROAD_TYPE_pedestrian_crossing_stop; break;
		case 13:
			dwMode = LOGIC_ROAD_TYPE_NO_comity_pedestrian; break;
		case 14:
			dwMode = LOGIC_ROAD_TYPE_VEH_BACK_OFF; break;
		case 15:
			dwMode = LOGIC_ROAD_TYPE_swerve_No_comity_straight; break;
		case 16:
			dwMode = LOGIC_ROAD_TYPE_pedestrian_run_red_light; break;
		case 23:
			dwMode = LOGIC_ROAD_TYPE_VEH_DISTANCE; break;
		case 24:
			dwMode = LOGIC_ROAD_TYPE_PERSON_ILLEGAL; break;
		case 25:
			dwMode = LOGIC_ROAD_TYPE_LIMIT_DRIVING; break;
		case 26:
			dwMode = LOGIC_ROAD_TYPE_CAR_HORNS; break;
		case 27:
			dwMode = LOGIC_ROAD_TYPE_VEH_OVERHEIGHT; break;
		}

		return dwMode;
	}

	u32	AnalyIndex2CheckExtType(int nIndex)
	{
		u32 dwMode = 0;
		switch (nIndex)
		{
		case 17:
			dwMode = EXT_LOGIC_ROAD_TYPE_ZYFJDCD; break;
		case 18:
			dwMode = EXT_LOGIC_ROAD_TYPE_ZYYJCD; break;
		case 19:
			dwMode = EXT_LOGIC_ROAD_TYPE_ZYGJCD; break;
		case 20:
			dwMode = EXT_LOGIC_ROAD_TYPE_ZYXXCCD; break;
		case 21:
			dwMode = EXT_LOGIC_ROAD_TYPE_JZHCTX; break;
		case 22:
			dwMode = EXT_LOGIC_ROAD_DWXZ; break;
		case 28:
			dwMode = EXT_LOGIC_ROAD_TYPE_FJDCZYJDCD; break;
		case 29:
			dwMode = EXT_LOGIC_ROAD_TYPE_FJDCZR; break;
		case 30:
			dwMode = EXT_LOGIC_ROAD_TYPE_FJDJZSJ; break;
		case 31:
			dwMode = EXT_LOGIC_ROAD_TYPE_YXYJ; break;
		}

		return dwMode;
	}

	wstring		AnalyLightMode(u32 dwMode)
	{
		int nIndex = AnalyLightMode2Index(dwMode);
		wstring awsLightMode[9] =
		{
			TEXT("视频流截图"), TEXT("闪光灯"), TEXT("LED增亮补光"), TEXT("红外闪光灯"), TEXT("红外LED灯"), TEXT("闪光灯+LED增亮补光"), TEXT("闪光灯+红外LED增亮")
			, TEXT("红外闪光灯+红外LED增亮"), TEXT("红外闪光灯+LED增亮补光")
		};
		return awsLightMode[nIndex];
	}

	int			AnalyLightMode2Index(u32 dwMode)
	{
		int nIndex = 0;
		switch (dwMode)
		{
		case NONE_LIGHT_MODE:
			nIndex = 0; break;
		case FLASH_LIGHT_MODE:
			nIndex = 1; break;
		case LED_LIGHT_MODE:
			nIndex = 2; break;
		case FLASH_IR_LIGHT_MODE:
			nIndex = 3; break;
		case LED_IR_LIGHT_MODE:
			nIndex = 4; break;
		case FLASH_LED_LIGHT_MODE:
			nIndex = 5; break;
		case FLASH_IR_LED_MODE:
			nIndex = 6; break;
		case IR_FLASH_IR_LED_MODE:
			nIndex = 7; break;
		case IR_FLASH_LED_MODE:
			nIndex = 8; break;
		default:
			nIndex = 0; break;
		}
		return nIndex;
	}
	u32			AnalyIndex2LightMode(int nIndex)
	{
		u32 dwMode = 0;
		switch (nIndex)
		{
		case 0:
			dwMode = NONE_LIGHT_MODE; break;
		case 1:
			dwMode = FLASH_LIGHT_MODE; break;
		case 2:
			dwMode = LED_LIGHT_MODE; break;
		case 3:
			dwMode = FLASH_IR_LIGHT_MODE; break;
		case 4:
			dwMode = LED_IR_LIGHT_MODE; break;
		case 5:
			dwMode = FLASH_LED_LIGHT_MODE; break;
		case 6:
			dwMode = FLASH_IR_LED_MODE; break;
		case 7:
			dwMode = IR_FLASH_IR_LED_MODE; break;
		case 8:
			dwMode = IR_FLASH_LED_MODE; break;
		}

		return dwMode;
	}

	//////////////////////////////////////////////////////////////////////////
	//

	bool		SetEdtValue(void* pData, QLineEdit* pEdt, int em)
	{
		if (pEdt && pData)
		{
			int number;
			wchar_t ws[129];
			switch (em)
			{
			case EMLOGICLANEDETAILS_EDT_DECTOR1:
			{
				number = ((TIPC_LogicLane*)pData)->dwDectorLabel[0];
				number = number % 100;
				wsprintf(ws, TEXT("%d"), number);
			}
				break;
			case EMLOGICLANEDETAILS_EDT_DECTOR2:
			{
				number = ((TIPC_LogicLane*)pData)->dwDectorLabel[1];
				number = number % 100;
				wsprintf(ws, TEXT("%d"), number);
			}
				break;
			case EMLOGICLANEDETAILS_EDT_DECTOR3:
			{
				number = ((TIPC_LogicLane*)pData)->dwDectorLabel[2];
				number = number % 100;
				wsprintf(ws, TEXT("%d"), number);
			}
				break;
			case EMLOGICLANEDETAILS_EDT_DECTOR4:
			{
				number = ((TIPC_LogicLane*)pData)->dwDectorLabel[3];
				number = number % 100;
				wsprintf(ws, TEXT("%d"), number);
			}
				break;
			case EMLOGICLANEDETAILS_EDT_SNAPNUM2:
			{
				number = ((TIPC_LogicLane*)pData)->dwNeedCnts;
				number = number / 100;
				wsprintf(ws, TEXT("%d"), number);
			}
				break;
			case EMLOGICLANEDETAILS_EDT_MINSNAPTIME:
			{
				wsprintf(ws, TEXT("%d"), ((TIPC_LogicLane*)pData)->dwMinInterval);
			}
				break;
			case EMLOGICLANEDETAILS_EDT_SPEEDTIME12:
			{
				// 使用了联合体
				if (((TIPC_LogicLane*)pData)->dwSnapCtrlMode != enum_snap_ctrl_mode_by_time)
					return true;
				wsprintf(ws, TEXT("%d"), ((TIPC_LogicLane*)pData)->ShareData1.dwTime);
			}
				break;
			case EMLOGICLANEDETAILS_EDT_SPEEDTIME23:
			{
				// 使用了联合体
				if (((TIPC_LogicLane*)pData)->dwSnapCtrlMode != enum_snap_ctrl_mode_by_time)
					return true;
				wsprintf(ws, TEXT("%d"), ((TIPC_LogicLane*)pData)->ShareData2.dwTime);
			}
				break;
			case EMLOGICLANEDETAILS_EDT_SPEEDRANG12:
			{
				// 使用了联合体
				if (((TIPC_LogicLane*)pData)->dwSnapCtrlMode != enum_snap_ctrl_mode_by_distance)
					return true;
				wsprintf(ws, TEXT("%d"), ((TIPC_LogicLane*)pData)->ShareData1.dwDistance);
			}
				break;
			case EMLOGICLANEDETAILS_EDT_SPEEDRANG23:
			{
				// 使用了联合体
				if (((TIPC_LogicLane*)pData)->dwSnapCtrlMode != enum_snap_ctrl_mode_by_distance)
					return true;
				wsprintf(ws, TEXT("%d"), ((TIPC_LogicLane*)pData)->ShareData2.dwDistance);
			}
				break;
			case EMLOGICLANEDETAILS_EDT_CHILDPLATE_WIDHT:
			{
				wsprintf(ws, TEXT("%d"), ((TIPC_LogicLane*)pData)->dwLprPicW);
			}
				break;
			case EMLOGICLANEDETAILS_EDT_CHILDPLATE_HEIGHT:
			{
				wsprintf(ws, TEXT("%d"), ((TIPC_LogicLane*)pData)->dwLprPicH);
			}
				break;
			case EMLOGICLANEDETAILS_EDT_CHILDVEHICLE_WIDHT:
			{
				wsprintf(ws, TEXT("%d"), ((TIPC_LogicLane*)pData)->dwVehPicW);
			}
				break;
			case EMLOGICLANEDETAILS_EDT_CHILDVEHICLE_HEIGHT:
			{
				wsprintf(ws, TEXT("%d"), ((TIPC_LogicLane*)pData)->dwVehPicH);
			}
				break;
			case EMLOGICLANEDETAILS_EDT_CHILDPOEPLE_WIDHT:
			{
				wsprintf(ws, TEXT("%d"), ((TIPC_LogicLane*)pData)->dwFacePicW & 0xffff);
			}
				break;
			case EMLOGICLANEDETAILS_EDT_CHILDPOEPLE_HEIGHT:
			{
				wsprintf(ws, TEXT("%d"), ((TIPC_LogicLane*)pData)->dwFacePicH & 0xffff);
			}
				break;
			case EMLOGICLANEDETAILS_EDT_CHILDPOEPLE_WIDHT_2:
			{
				wsprintf(ws, TEXT("%d"), ((TIPC_LogicLane*)pData)->dwFacePicW >> 16);
			}
				break;
			case EMLOGICLANEDETAILS_EDT_CHILDPOEPLE_HEIGHT_2:
			{
				wsprintf(ws, TEXT("%d"), ((TIPC_LogicLane*)pData)->dwFacePicH >> 16);
			}
				break;
			case EMLOGICLANEDETAILS_EDT_RATELIMIT_LITTLE:
			{
				wsprintf(ws, TEXT("%d"), ((TIPC_LogicLane*)pData)->dwCarLmSpd);
			}
				break;
			case EMLOGICLANEDETAILS_EDT_RATELIMIT_BIG:
			{
				wsprintf(ws, TEXT("%d"), ((TIPC_LogicLane*)pData)->dwTruckLmSpd);
			}
				break;
			case EMLOGICLANEDETAILS_EDT_SNAPLIMIT_LITTLE:
			{
				wsprintf(ws, TEXT("%d"), ((TIPC_LogicLane*)pData)->dwCarSnapSpd);
			}
				break;
			case EMLOGICLANEDETAILS_EDT_SNAPLIMIT_BIG:
			{
				wsprintf(ws, TEXT("%d"), ((TIPC_LogicLane*)pData)->dwTruckSnapSpd);
			}
				break;
			case EMLOGICLANEDETAILS_EDT_RATEOFFSET:
			{
				wsprintf(ws, TEXT("%d"), ((TIPC_LogicLane*)pData)->dwSpdOffset);
			}
				break;
			case EMLOGICLANEDETAILS_EDT_MINREDLIGHTTIME:
			{
				wsprintf(ws, TEXT("%d"), ((TIPC_LogicLane*)pData)->dwMinRedLight);
			}
				break;
			case EMLOGICLANEDETAILS_EDT_MAXREDLIGHTTIME:
			{
				wsprintf(ws, TEXT("%d"), ((TIPC_LogicLane*)pData)->dwMaxRedLight);
			}
				break;
			case EMLOGICLANEDETAILS_EDT_GZZXD:
			{
				wsprintf(ws, TEXT("%d"), ((TIPC_LogicLane*)pData)->dwConfidenceLevel);
			}
				break;
			default:
				break;
			}

			pEdt->setText(QString::fromWCharArray(ws));
			return true;
		}


		return false;
	}
	bool		GetEdtValue(void* pData, QLineEdit* pEdt, int em)
	{
		if (pEdt)
		{
			int number;
			QString str = pEdt->text();
			switch (em)
			{
			case EMLOGICLANEDETAILS_EDT_DECTOR1:
			{
				number = ((TIPC_LogicLane*)pData)->dwDectorLabel[0];
				number = number - (number % 100);
				number = number + str.toInt();
				((TIPC_LogicLane*)pData)->dwDectorLabel[0] = number;
			}
				break;
			case EMLOGICLANEDETAILS_EDT_DECTOR2:
			{
				number = ((TIPC_LogicLane*)pData)->dwDectorLabel[1];
				number = number - (number % 100);
				number = number + str.toInt();
				((TIPC_LogicLane*)pData)->dwDectorLabel[1] = number;
			}
				break;
			case EMLOGICLANEDETAILS_EDT_DECTOR3:
			{
				number = ((TIPC_LogicLane*)pData)->dwDectorLabel[2];
				number = number - (number % 100);
				number = number + str.toInt();
				((TIPC_LogicLane*)pData)->dwDectorLabel[2] = number;
			}
				break;
			case EMLOGICLANEDETAILS_EDT_DECTOR4:
			{
				number = ((TIPC_LogicLane*)pData)->dwDectorLabel[3];
				number = number - (number % 100);
				number = number + str.toInt();
				((TIPC_LogicLane*)pData)->dwDectorLabel[3] = number;
			}
				break;
			case EMLOGICLANEDETAILS_EDT_SNAPNUM2:
			{
				int temp = str.toInt();
				number = ((TIPC_LogicLane*)pData)->dwNeedCnts;
				number = number % 100;
				((TIPC_LogicLane*)pData)->dwNeedCnts = number + temp * 100;
			}
				break;
			case EMLOGICLANEDETAILS_EDT_MINSNAPTIME:
			{
				((TIPC_LogicLane*)pData)->dwMinInterval = str.toInt();
			}
				break;
			case EMLOGICLANEDETAILS_EDT_SPEEDTIME12:
			{
				// 使用了联合体
				if (((TIPC_LogicLane*)pData)->dwSnapCtrlMode != enum_snap_ctrl_mode_by_time)
					return true;
				((TIPC_LogicLane*)pData)->ShareData1.dwTime = str.toInt();
			}
				break;
			case EMLOGICLANEDETAILS_EDT_SPEEDTIME23:
			{
				// 使用了联合体
				if (((TIPC_LogicLane*)pData)->dwSnapCtrlMode != enum_snap_ctrl_mode_by_time)
					return true;
				((TIPC_LogicLane*)pData)->ShareData2.dwTime = str.toInt();
			}
				break;
			case EMLOGICLANEDETAILS_EDT_SPEEDRANG12:
			{
				// 使用了联合体
				if (((TIPC_LogicLane*)pData)->dwSnapCtrlMode != enum_snap_ctrl_mode_by_distance)
					return true;
				((TIPC_LogicLane*)pData)->ShareData1.dwDistance = str.toInt();
			}
				break;
			case EMLOGICLANEDETAILS_EDT_SPEEDRANG23:
			{
				// 使用了联合体
				if (((TIPC_LogicLane*)pData)->dwSnapCtrlMode != enum_snap_ctrl_mode_by_distance)
					return true;
				((TIPC_LogicLane*)pData)->ShareData2.dwDistance = str.toInt();
			}
				break;
			case EMLOGICLANEDETAILS_EDT_CHILDPLATE_WIDHT:
			{
				((TIPC_LogicLane*)pData)->dwLprPicW = str.toInt();
			}
				break;
			case EMLOGICLANEDETAILS_EDT_CHILDPLATE_HEIGHT:
			{
				((TIPC_LogicLane*)pData)->dwLprPicH = str.toInt();
			}
				break;
			case EMLOGICLANEDETAILS_EDT_CHILDVEHICLE_WIDHT:
			{
				((TIPC_LogicLane*)pData)->dwVehPicW = str.toInt();
			}
				break;
			case EMLOGICLANEDETAILS_EDT_CHILDVEHICLE_HEIGHT:
			{
				((TIPC_LogicLane*)pData)->dwVehPicH = str.toInt();
			}
				break;
			case EMLOGICLANEDETAILS_EDT_CHILDPOEPLE_WIDHT:
			{
				Uint32 nValue = str.toInt();
				Uint32 nTemp = 0xffff0000 & (((TIPC_LogicLane*)pData)->dwFacePicW);
				nTemp += nValue;
				((TIPC_LogicLane*)pData)->dwFacePicW = nTemp;
			}
				break;
			case EMLOGICLANEDETAILS_EDT_CHILDPOEPLE_HEIGHT:
			{
				Uint32 nValue = str.toInt();
				Uint32 nTemp = 0xffff0000 & (((TIPC_LogicLane*)pData)->dwFacePicH);
				nTemp += nValue;
				((TIPC_LogicLane*)pData)->dwFacePicH = nTemp;
			}
				break;
			case EMLOGICLANEDETAILS_EDT_CHILDPOEPLE_WIDHT_2:
			{
				Uint32 nValue = str.toInt();
				Uint32 nTemp = 0xffff & (((TIPC_LogicLane*)pData)->dwFacePicW);
				int n1 = nValue << 16;
				nTemp += n1;
				((TIPC_LogicLane*)pData)->dwFacePicW = nTemp;
			}
				break;
			case EMLOGICLANEDETAILS_EDT_CHILDPOEPLE_HEIGHT_2:
			{
				Uint32 nValue = str.toInt();
				Uint32 nTemp = 0xffff & (((TIPC_LogicLane*)pData)->dwFacePicH);
				int n1 = nValue << 16;
				nTemp += n1;
				((TIPC_LogicLane*)pData)->dwFacePicH = nTemp;
			}
				break;
			case EMLOGICLANEDETAILS_EDT_RATELIMIT_LITTLE:
			{
				((TIPC_LogicLane*)pData)->dwCarLmSpd = str.toInt();
			}
				break;
			case EMLOGICLANEDETAILS_EDT_RATELIMIT_BIG:
			{
				((TIPC_LogicLane*)pData)->dwTruckLmSpd = str.toInt();
			}
				break;
			case EMLOGICLANEDETAILS_EDT_SNAPLIMIT_LITTLE:
			{
				((TIPC_LogicLane*)pData)->dwCarSnapSpd = str.toInt();
			}
				break;
			case EMLOGICLANEDETAILS_EDT_SNAPLIMIT_BIG:
			{
				((TIPC_LogicLane*)pData)->dwTruckSnapSpd = str.toInt();
			}
				break;
			case EMLOGICLANEDETAILS_EDT_RATEOFFSET:
			{
				((TIPC_LogicLane*)pData)->dwSpdOffset = str.toInt();
			}
				break;
			case EMLOGICLANEDETAILS_EDT_MINREDLIGHTTIME:
			{
				((TIPC_LogicLane*)pData)->dwMinRedLight = str.toInt();
			}
				break;
			case EMLOGICLANEDETAILS_EDT_MAXREDLIGHTTIME:
			{
				((TIPC_LogicLane*)pData)->dwMaxRedLight = str.toInt();
			}
				break;
			case EMLOGICLANEDETAILS_EDT_GZZXD:
			{
				((TIPC_LogicLane*)pData)->dwConfidenceLevel = str.toInt();
			}
				break;
			default:
				break;
			}

			return true;
		}

		return false;
	}

	bool			SetCmbValue(void* pData, QComboBox* pCmb, int em)
	{
		if (pCmb && pData)
		{
			int number;
			int nIndex = -1;
			switch (em)
			{
			case EMLOGICLANEDETAILS_CMB_SNAPNUM:
			{
				number = ((TIPC_LogicLane*)pData)->dwNeedCnts;
				number = number % 100;
				nIndex = number - 1;
			}
				break;
			case EMLOGICLANEDETAILS_CMB_SNAPCTRL:
			{
				nIndex = AnalySnapCtrlMode2Index(((TIPC_LogicLane*)pData)->dwSnapCtrlMode);
			}
				break;
			case EMLOGICLANEDETAILS_CMB_ZFPP:
			{
				nIndex = ((TIPC_LogicLane*)pData)->dwMatchCnt;
			}
				break;
			case EMLOGICLANEDETAILS_CMB_JX:
			{
				nIndex = ((TIPC_LogicLane*)pData)->bPolarity;
			}
				break;
			case EMLOGICLANEDETAILS_CMB_FILTERDIR:
			{
				nIndex = ((TIPC_LogicLane*)pData)->dwFilterDir;
			}
				break;
			case EMLOGICLANEDETAILS_CMB_PIC2DAY:
			{
				u32 temp = ((TIPC_LogicLane*)pData)->dwDaySnapLight;
				temp = (temp & 0xFF00) >> 8;
				nIndex = AnalyLightMode2Index(temp);
			}
				break;
			case EMLOGICLANEDETAILS_CMB_PIC2NIGHT:
			{
				u32 temp = ((TIPC_LogicLane*)pData)->dwNightSnapLight;
				temp = (temp & 0xFF00) >> 8;
				nIndex = AnalyLightMode2Index(temp);
			}
				break;
			case EMLOGICLANEDETAILS_CMB_PIC3DAY:
			{
				u32 temp = ((TIPC_LogicLane*)pData)->dwDaySnapLight;
				temp = (temp & 0xFF0000) >> 16;
				nIndex = AnalyLightMode2Index(temp);
			}
				break;
			case EMLOGICLANEDETAILS_CMB_PIC3NIGHT:
			{
				u32 temp = ((TIPC_LogicLane*)pData)->dwNightSnapLight;
				temp = (temp & 0xFF0000) >> 16;
				nIndex = AnalyLightMode2Index(temp);
			}
				break;
			case EMLOGICLANEDETAILS_CMB_PIC4DAY:
			{
				u32 temp = ((TIPC_LogicLane*)pData)->dwDaySnapLight;
				temp = (temp & 0xFF000000) >> 24;
				nIndex = AnalyLightMode2Index(temp);
			}
				break;
			case EMLOGICLANEDETAILS_CMB_PIC4NIGHT:
			{
				u32 temp = ((TIPC_LogicLane*)pData)->dwNightSnapLight;
				temp = (temp & 0xFF000000) >> 24;
				nIndex = AnalyLightMode2Index(temp);
			}
				break;
			}

			if (nIndex != -1)
			{
				if (pCmb->currentIndex() != nIndex)
					pCmb->setCurrentIndex(nIndex);
				return true;
			}
		}

		return false;
	}
	bool		GetCmbValue(void* pData, QComboBox* pCmb, int em)
	{
		if (pCmb)
		{
			int nSel = pCmb->currentIndex();
			switch (em)
			{
			case EMLOGICLANEDETAILS_CMB_SNAPNUM:
			{
				int number = ((TIPC_LogicLane*)pData)->dwNeedCnts;
				number = number - (number % 100);
				((TIPC_LogicLane*)pData)->dwNeedCnts = number + nSel + 1;
			}
				break;
			case EMLOGICLANEDETAILS_CMB_SNAPCTRL:
			{
				((TIPC_LogicLane*)pData)->dwSnapCtrlMode = AnalyIndex2SnapCtrlMode(nSel);
			}
				break;
			case EMLOGICLANEDETAILS_CMB_ZFPP:
			{
				((TIPC_LogicLane*)pData)->dwMatchCnt = nSel;
			}
				break;
			case EMLOGICLANEDETAILS_CMB_JX:
			{
				((TIPC_LogicLane*)pData)->bPolarity = nSel;
			}
				break;
			case EMLOGICLANEDETAILS_CMB_FILTERDIR:
			{
				((TIPC_LogicLane*)pData)->dwFilterDir = nSel;
			}
				break;
			case EMLOGICLANEDETAILS_CMB_PIC2DAY:
			{
				u32 temp = AnalyIndex2LightMode(nSel);
				u32 temp2 = ((TIPC_LogicLane*)pData)->dwDaySnapLight;
				temp2 &= 0xFFFF00FF;
				temp2 += (temp << 8);
				((TIPC_LogicLane*)pData)->dwDaySnapLight = temp2;
			}
				break;
			case EMLOGICLANEDETAILS_CMB_PIC2NIGHT:
			{
				u32 temp = AnalyIndex2LightMode(nSel);
				u32 temp2 = ((TIPC_LogicLane*)pData)->dwNightSnapLight;
				temp2 &= 0xFFFF00FF;
				temp2 += (temp << 8);
				((TIPC_LogicLane*)pData)->dwNightSnapLight = temp2;
			}
				break;
			case EMLOGICLANEDETAILS_CMB_PIC3DAY:
			{
				u32 temp = AnalyIndex2LightMode(nSel);
				u32 temp2 = ((TIPC_LogicLane*)pData)->dwDaySnapLight;
				temp2 &= 0xFF00FFFF;
				temp2 += (temp << 16);
				((TIPC_LogicLane*)pData)->dwDaySnapLight = temp2;
			}
				break;
			case EMLOGICLANEDETAILS_CMB_PIC3NIGHT:
			{
				u32 temp = AnalyIndex2LightMode(nSel);
				u32 temp2 = ((TIPC_LogicLane*)pData)->dwNightSnapLight;
				temp2 &= 0xFF00FFFF;
				temp2 += (temp << 16);
				((TIPC_LogicLane*)pData)->dwNightSnapLight = temp2;
			}
				break;
			case EMLOGICLANEDETAILS_CMB_PIC4DAY:
			{
				u32 temp = AnalyIndex2LightMode(nSel);
				u32 temp2 = ((TIPC_LogicLane*)pData)->dwDaySnapLight;
				temp2 &= 0x00FFFFFF;
				temp2 += (temp << 24);
				((TIPC_LogicLane*)pData)->dwDaySnapLight = temp2;
			}
				break;
			case EMLOGICLANEDETAILS_CMB_PIC4NIGHT:
			{
				u32 temp = AnalyIndex2LightMode(nSel);
				u32 temp2 = ((TIPC_LogicLane*)pData)->dwNightSnapLight;
				temp2 &= 0x00FFFFFF;
				temp2 += (temp << 24);
				((TIPC_LogicLane*)pData)->dwNightSnapLight = temp2;
			}
				break;
			}
		}

		return true;
	}

	bool		SetChkValue(void* pData, QCheckBox* pChkBtn, int em)
	{

		if (pChkBtn && pData)
		{
			int nState = -1;
			switch (em)
			{
			case EMLOGICLANEDETAILS_CHK_DECTOR1:
			{
				int number;
				number = ((TIPC_LogicLane*)pData)->dwDectorLabel[0];
				number = number / 100;
				nState = number;
			}
				break;
			case EMLOGICLANEDETAILS_CHK_DECTOR2:
			{
				int number;
				number = ((TIPC_LogicLane*)pData)->dwDectorLabel[1];
				number = number / 100;
				nState = number;
			}
				break;
			case EMLOGICLANEDETAILS_CHK_DECTOR3:
			{
				int number;
				number = ((TIPC_LogicLane*)pData)->dwDectorLabel[2];
				number = number / 100;
				nState = number;
			}
				break;
			case EMLOGICLANEDETAILS_CHK_DECTOR4:
			{
				int number;
				number = ((TIPC_LogicLane*)pData)->dwDectorLabel[3];
				number = number / 100;
				nState = number;
			}
				break;
			case EMLOGICLANEDETAILS_CHK_CHILD_PLATE:
			{
				nState = (int)((TIPC_LogicLane*)pData)->bLprSubPic;
			}
				break;
			case EMLOGICLANEDETAILS_CHK_CHILD_VEHICLE:
			{
				nState = (int)((TIPC_LogicLane*)pData)->bVehSubPic;
			}
				break;
			case EMLOGICLANEDETAILS_CHK_CHILD_POEPLE:
			{
				nState = (int)((TIPC_LogicLane*)pData)->bFaceSubPic;
			}
				break;
			case EMLOGICLANEDETAILS_CHK_ADD_FACE:
			{
				nState = (int)((TIPC_LogicLane*)pData)->bAddFaceSubPic;
			}
				break;
			case EMLOGICLANEDETAILS_CHKREDLIGHT_LEFT:
			{
				nState = GetSpecifyBitValue(((TIPC_LogicLane*)pData)->dwRedLightPhase, 4);
			}
				break;
			case EMLOGICLANEDETAILS_CHKREDLIGHT_LINE:
			{
				nState = GetSpecifyBitValue(((TIPC_LogicLane*)pData)->dwRedLightPhase, 3);
			}
				break;
			case EMLOGICLANEDETAILS_CHKREDLIGHT_RIGHT:
			{
				nState = GetSpecifyBitValue(((TIPC_LogicLane*)pData)->dwRedLightPhase, 2);
			}
				break;
			case EMLOGICLANEDETAILS_CHK_HPGL_YELLOW:
			{
				nState = GetSpecifyBitValue(((TIPC_LogicLane*)pData)->dwFilterLpr, 0);
			}
				break;
			case EMLOGICLANEDETAILS_CHK_HPGL_WHITE:
			{
				nState = GetSpecifyBitValue(((TIPC_LogicLane*)pData)->dwFilterLpr, 1);
			}
				break;
			case EMLOGICLANEDETAILS_CHK_HPGL_BLUE:
			{
				nState = GetSpecifyBitValue(((TIPC_LogicLane*)pData)->dwFilterLpr, 2);
			}
				break;
			case EMLOGICLANEDETAILS_CHK_HPGL_BLACK:
			{
				nState = GetSpecifyBitValue(((TIPC_LogicLane*)pData)->dwFilterLpr, 3);
			}
				break;
			case EMLOGICLANEDETAILS_CHK_HPGL_GREEN:
			{
				nState = GetSpecifyBitValue(((TIPC_LogicLane*)pData)->dwFilterLpr, 4);
			}
				break;
			case EMLOGICLANEDETAILS_CHK_HPGL_YELLOW_GREEN:
			{
				nState = GetSpecifyBitValue(((TIPC_LogicLane*)pData)->dwFilterLpr, 5);
			}
				break;
			case EMLOGICLANEDETAILS_CHK_HPGL_NO:
			{
				nState = GetSpecifyBitValue(((TIPC_LogicLane*)pData)->dwFilterLpr, 6);
			}
				break;
			case EMLOGICLANEDETAILS_CHK_VEH_0:
			{
				nState = GetSpecifyBitValue(((TIPC_LogicLane*)pData)->dwSnapVehType, 0);
			}
				break;
			case EMLOGICLANEDETAILS_CHK_VEH_1:
			{
				nState = GetSpecifyBitValue(((TIPC_LogicLane*)pData)->dwSnapVehType, 1);
			}
				break;
			case EMLOGICLANEDETAILS_CHK_VEH_2:
			{
				nState = GetSpecifyBitValue(((TIPC_LogicLane*)pData)->dwSnapVehType, 2);
			}
				break;
			case EMLOGICLANEDETAILS_CHK_VEH_3:
			{
				nState = GetSpecifyBitValue(((TIPC_LogicLane*)pData)->dwSnapVehType, 3);
			}
				break;
			case EMLOGICLANEDETAILS_CHK_VEH_4:
			{
				nState = GetSpecifyBitValue(((TIPC_LogicLane*)pData)->dwSnapVehType, 4);
			}
				break;
			case EMLOGICLANEDETAILS_CHK_VEH_5:
			{
				nState = GetSpecifyBitValue(((TIPC_LogicLane*)pData)->dwSnapVehType, 5);
			}
				break;
			case EMLOGICLANEDETAILS_CHK_VEH_6:
			{
				nState = GetSpecifyBitValue(((TIPC_LogicLane*)pData)->dwSnapVehType, 6);
			}
				break;
			case EMLOGICLANEDETAILS_CHK_VEH_7:
			{
				nState = GetSpecifyBitValue(((TIPC_LogicLane*)pData)->dwSnapVehType, 7);
			}
				break;
			case EMLOGICLANEDETAILS_CHK_VEH_8:
			{
				nState = GetSpecifyBitValue(((TIPC_LogicLane*)pData)->dwSnapVehType, 8);
			}
				break;
			case EMLOGICLANEDETAILS_CHK_SPEED_CHECK:
			{
				nState = (int)((TIPC_LogicLane*)pData)->bUseVidSpdChk;
			}
				break;
			case EMLOGICLANEDETAILS_CHK_MERGE_SELECT_DEST:
			{
				nState = (int)((TIPC_LogicLane*)pData)->bShowTarget;
			}
			}

			if (nState >= 0)
			{
				pChkBtn->setChecked(nState == 1);
				return true;
			}
		}

		return false;

	}

	bool		GetChkValue(void* pData, QCheckBox* pChkBtn, int em)
	{
		if (pChkBtn)
		{
			bool bSel = pChkBtn->isChecked();
			switch (em)
			{
			case EMLOGICLANEDETAILS_CHK_DECTOR1:
			{
				int number;
				number = ((TIPC_LogicLane*)pData)->dwDectorLabel[0];
				number = number % 100;
				if (bSel)
				{
					number = number + 100;
				}
				((TIPC_LogicLane*)pData)->dwDectorLabel[0] = number;
			}
				break;
			case EMLOGICLANEDETAILS_CHK_DECTOR2:
			{
				int number;
				number = ((TIPC_LogicLane*)pData)->dwDectorLabel[1];
				number = number % 100;
				if (bSel)
				{
					number = number + 100;
				}
				((TIPC_LogicLane*)pData)->dwDectorLabel[1] = number;
			}
				break;
			case EMLOGICLANEDETAILS_CHK_DECTOR3:
			{
				int number;
				number = ((TIPC_LogicLane*)pData)->dwDectorLabel[2];
				number = number % 100;
				if (bSel)
				{
					number = number + 100;
				}
				((TIPC_LogicLane*)pData)->dwDectorLabel[2] = number;
			}
				break;
			case EMLOGICLANEDETAILS_CHK_DECTOR4:
			{
				int number;
				number = ((TIPC_LogicLane*)pData)->dwDectorLabel[3];
				number = number % 100;
				if (bSel)
				{
					number = number + 100;
				}
				((TIPC_LogicLane*)pData)->dwDectorLabel[3] = number;
			}
				break;
			case EMLOGICLANEDETAILS_CHK_CHILD_PLATE:
			{
				((TIPC_LogicLane*)pData)->bLprSubPic = bSel;
			}
				break;
			case EMLOGICLANEDETAILS_CHK_CHILD_VEHICLE:
			{
				((TIPC_LogicLane*)pData)->bVehSubPic = bSel;
			}
				break;
			case EMLOGICLANEDETAILS_CHK_CHILD_POEPLE:
			{
				((TIPC_LogicLane*)pData)->bFaceSubPic = bSel;
			}
				break;
			case EMLOGICLANEDETAILS_CHK_ADD_FACE:
			{
				((TIPC_LogicLane*)pData)->bAddFaceSubPic = bSel;
			}
				break;
			case EMLOGICLANEDETAILS_CHKREDLIGHT_LEFT:
			{
				SetSpecifyBitValue(((TIPC_LogicLane*)pData)->dwRedLightPhase, bSel, 4);
			}
				break;
			case EMLOGICLANEDETAILS_CHKREDLIGHT_LINE:
			{
				SetSpecifyBitValue(((TIPC_LogicLane*)pData)->dwRedLightPhase, bSel, 3);
			}
				break;
			case EMLOGICLANEDETAILS_CHKREDLIGHT_RIGHT:
			{
				SetSpecifyBitValue(((TIPC_LogicLane*)pData)->dwRedLightPhase, bSel, 2);
			}
				break;
			case EMLOGICLANEDETAILS_CHK_HPGL_YELLOW:
			{
				SetSpecifyBitValue(((TIPC_LogicLane*)pData)->dwFilterLpr, bSel, 0);
			}
				break;
			case EMLOGICLANEDETAILS_CHK_HPGL_WHITE:
			{
				SetSpecifyBitValue(((TIPC_LogicLane*)pData)->dwFilterLpr, bSel, 1);
			}
				break;
			case EMLOGICLANEDETAILS_CHK_HPGL_BLUE:
			{
				SetSpecifyBitValue(((TIPC_LogicLane*)pData)->dwFilterLpr, bSel, 2);
			}
				break;
			case EMLOGICLANEDETAILS_CHK_HPGL_BLACK:
			{
				SetSpecifyBitValue(((TIPC_LogicLane*)pData)->dwFilterLpr, bSel, 3);
			}
				break;
			case EMLOGICLANEDETAILS_CHK_HPGL_GREEN:
			{
				SetSpecifyBitValue(((TIPC_LogicLane*)pData)->dwFilterLpr, bSel, 4);
			}
				break;
			case EMLOGICLANEDETAILS_CHK_HPGL_YELLOW_GREEN:
			{
				SetSpecifyBitValue(((TIPC_LogicLane*)pData)->dwFilterLpr, bSel, 5);
			}
				break;
			case EMLOGICLANEDETAILS_CHK_HPGL_NO:
			{
				SetSpecifyBitValue(((TIPC_LogicLane*)pData)->dwFilterLpr, bSel, 6);
			}
				break;
			case EMLOGICLANEDETAILS_CHK_VEH_0:
			{
				SetSpecifyBitValue(((TIPC_LogicLane*)pData)->dwSnapVehType, bSel, 0);
			}
				break;
			case EMLOGICLANEDETAILS_CHK_VEH_1:
			{
				SetSpecifyBitValue(((TIPC_LogicLane*)pData)->dwSnapVehType, bSel, 1);
			}
				break;
			case EMLOGICLANEDETAILS_CHK_VEH_2:
			{
				SetSpecifyBitValue(((TIPC_LogicLane*)pData)->dwSnapVehType, bSel, 2);
			}
				break;
			case EMLOGICLANEDETAILS_CHK_VEH_3:
			{
				SetSpecifyBitValue(((TIPC_LogicLane*)pData)->dwSnapVehType, bSel, 3);
			}
				break;
			case EMLOGICLANEDETAILS_CHK_VEH_4:
			{
				SetSpecifyBitValue(((TIPC_LogicLane*)pData)->dwSnapVehType, bSel, 4);
			}
				break;
			case EMLOGICLANEDETAILS_CHK_VEH_5:
			{
				SetSpecifyBitValue(((TIPC_LogicLane*)pData)->dwSnapVehType, bSel, 5);
			}
				break;
			case EMLOGICLANEDETAILS_CHK_VEH_6:
			{
				SetSpecifyBitValue(((TIPC_LogicLane*)pData)->dwSnapVehType, bSel, 6);
			}
				break;
			case EMLOGICLANEDETAILS_CHK_VEH_7:
			{
				SetSpecifyBitValue(((TIPC_LogicLane*)pData)->dwSnapVehType, bSel, 7);
			}
				break;
			case EMLOGICLANEDETAILS_CHK_VEH_8:
			{
				SetSpecifyBitValue(((TIPC_LogicLane*)pData)->dwSnapVehType, bSel, 8);
			}
				break;
			case EMLOGICLANEDETAILS_CHK_SPEED_CHECK:
			{
				((TIPC_LogicLane*)pData)->bUseVidSpdChk = bSel;
			}
				break;
			case EMLOGICLANEDETAILS_CHK_MERGE_SELECT_DEST:
			{
				((TIPC_LogicLane*)pData)->bShowTarget = bSel;
				break;
			}
			}
		}

		return true;
	}

	int			AnalySnapCtrlMode2Index(u32 dwMode)
	{
		int nIndex = 0;
		switch (dwMode)
		{
		case enum_snap_ctrl_mode_by_time:
			nIndex = 0; break;
		case enum_snap_ctrl_mode_by_distance:
			nIndex = 1; break;
		default:
			nIndex = 0; break;
		}
		return nIndex;
	}
	u32			AnalyIndex2SnapCtrlMode(int nIndex)
	{
		u32 dwMode = enum_snap_ctrl_mode_by_time;
		switch (nIndex)
		{
		case 0:
			dwMode = enum_snap_ctrl_mode_by_time; break;
		case 1:
			dwMode = enum_snap_ctrl_mode_by_distance; break;
		}

		return dwMode;
	}

	// 解析红灯相位模式
	int			AnalyRedLightPhase2Index(u32 dwMode)
	{
		int nIndex = 0;
		switch (dwMode)
		{
		case (0x0001 << 0) :
			nIndex = 0; break;
		case (0x0001 << 1) :
			nIndex = 1; break;
		case (0x0001 << 2) :
			nIndex = 2; break;
		default:
			nIndex = 0; break;
		}
		return nIndex;
	}
	u32			AnalyIndex2RedLightPhase(int nIndex)
	{
		u32 dwMode = 0x0001 << 0;
		switch (nIndex)
		{
		case 0:
			dwMode = 0x0001 << 0; break;
		case 1:
			dwMode = 0x0001 << 1; break;
		case 2:
			dwMode = 0x0001 << 2; break;
		}

		return dwMode;
	}

};

