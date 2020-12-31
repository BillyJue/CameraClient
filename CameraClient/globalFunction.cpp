#include "globalFunction.h"
#include <string.h>

/*
// DrawStr(hDC, TEXT("你好吗122jjjkklKG"),100, 100, 32);
bool	DrawStr(HDC hdc, wchar_t* szDraw, int *outWidth, int x, int y, int nFontSize, wchar_t szFontName[16], int maxwidth)
{
	HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);//CDC::GetCurrentFont(hdc);
	LOGFONT LogFont = { 0 };
	::GetObject(hFont, sizeof(LOGFONT), &LogFont);
	::DeleteObject(hFont);

	wcscpy(LogFont.lfFaceName, szFontName);
	LogFont.lfHeight = -nFontSize; // 字体大小 
	hFont = CreateFontIndirect(&LogFont);
	HFONT hOldFont = (HFONT)::SelectObject(hdc, hFont);

	int nLen = wcslen(szDraw);
	int nCW = 0;
	int nPos = x;
	int nFontSize21 = nFontSize / 2;
	for (int i = 0; i < nLen; i++)
	{
		bool bWByte = true;

		// 不规则字体适用
		SIZE sz;
		GetTextExtentPoint32(hdc, (wchar_t*)(szDraw + i), 1, &sz);
		float fScale = ((float)sz.cx) / ((float)sz.cy);
		fScale = (((int)(fScale * 100)) % 100);
		if (fabs(fScale) > 30 && fabs(fScale) < 70)
		{
			bWByte = false;
		}

		int nFontWidth = nFontSize;
		if (!bWByte) nFontWidth = nFontSize21;

		nCW += nFontWidth;
		if (maxwidth > 0 && nCW > maxwidth)
			break;
		TextOut(hdc, nPos, y, (wchar_t*)(szDraw + i), 1);
		nPos = nPos + nFontWidth;

	}

	if (outWidth)
	{
		*outWidth = nCW;
	}

	::SelectObject(hdc, hOldFont);
	::DeleteObject(hFont);
	return true;
}

BOOL SaveBitmapToFile(HBITMAP hBitmap, LPCTSTR lpszFileName)
{
	CLSID pngClsid;

	Bitmap bmp(hBitmap, NULL);

	// 获取 BMP 文件的编码方式 ( 如果希望获取 JPEG 的编码方式，

	// 那么参数一要设置为： _TEXT("image/jpeg") ，其他支持的图片格式类似 )
	int nResult = GetEncoderClsid(_TEXT("image/bmp"), &pngClsid);

	if (nResult >= 0)
	{
		// 保存所截取的屏幕图片
		bmp.Save(lpszFileName, &pngClsid);
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

int BmpToY(char *pFilePath, char *pBufY, u32 *pWidth, u32 *pHeight)
{
	BITMAPFILEHEADER bmp_header = { 0 };
	BITMAPINFOHEADER bmp_info = { 0 };
	FILE *pFile = fopen(pFilePath, "rb+");
	if (pFile == NULL)
	{
		return -1;
	}

	fread(&bmp_header.bfType, sizeof(bmp_header.bfType), 1, pFile);
	fread(&bmp_header.bfSize, sizeof(bmp_header.bfSize), 1, pFile);
	fread(&bmp_header.bfReserved1, sizeof(bmp_header.bfReserved1), 1, pFile);
	fread(&bmp_header.bfReserved2, sizeof(bmp_header.bfReserved2), 1, pFile);
	fread(&bmp_header.bfOffBits, sizeof(bmp_header.bfOffBits), 1, pFile);
	fread(&bmp_info, sizeof(BITMAPINFOHEADER), 1, pFile);

	*pWidth = bmp_info.biWidth;
	*pHeight = bmp_info.biHeight;

	int realWidth = (((bmp_info.biWidth * bmp_info.biBitCount) + 31) >> 5) << 2;

	char *pBufBmp = (char *)malloc(bmp_info.biHeight * realWidth);
	fseek(pFile, bmp_header.bfOffBits, SEEK_SET);
	fread(pBufBmp, 1, bmp_info.biHeight * realWidth, pFile);
	fclose(pFile);

	switch (bmp_info.biBitCount)
	{
	case 1:
	{
		int n = 0;
		for (int i = bmp_info.biHeight - 1; i >= 0; i--)
		{
			for (int j = 0; j < bmp_info.biWidth; j++)
			{
				BYTE mixIndex = 0;
				int k = i*realWidth + j / 8;
				mixIndex = pBufBmp[k];
				switch (j % 8)
				{
				case 7:
					mixIndex = mixIndex << 7;
					mixIndex = mixIndex >> 7;
					break;
				case 6:
					mixIndex = mixIndex << 6;
					mixIndex = mixIndex >> 7;
					break;
				case 5:
					mixIndex = mixIndex << 5;
					mixIndex = mixIndex >> 7;
					break;
				case 4:
					mixIndex = mixIndex << 4;
					mixIndex = mixIndex >> 7;
					break;
				case 3:
					mixIndex = mixIndex << 3;
					mixIndex = mixIndex >> 7;
					break;
				case 2:
					mixIndex = mixIndex << 2;
					mixIndex = mixIndex >> 7;
					break;
				case 1:
					mixIndex = mixIndex << 1;
					mixIndex = mixIndex >> 7;
					break;
				case 0:
					mixIndex = mixIndex >> 7;
					break;
				}

				if (mixIndex)
				{
					pBufY[n] = 255;
				}
				else
				{
					pBufY[n] = 0;
				}
				n++;
			}
		}
		n = 0;
	}
	break;
	default:
		break;
	}

	free(pBufBmp);

	return 0;
}
*/



bool wchar2char(char* psz, const wchar_t* pwsz)
{
	int len = WideCharToMultiByte(CP_ACP, 0, pwsz, -1, NULL, 0, NULL, NULL);
	if (!psz)
	{
		return false;
	}
	memset(psz, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, pwsz, -1, psz, len, NULL, NULL);

	return true;
}

bool char2wchar(wchar_t* pwsz, const char* psz)
{
	int len = MultiByteToWideChar(CP_ACP, 0, psz, -1, NULL, 0);
	if (!pwsz)
	{
		return false;
	}
	memset(pwsz, 0, len + 1);
	MultiByteToWideChar(CP_ACP, 0, psz, -1, pwsz, len);

	return true;
}

/*
bool CStdString2char(char* psz, CStdString* pwsz, int nMax)
{
	int len = WideCharToMultiByte(CP_ACP, 0, pwsz->GetData(), -1, NULL, 0, NULL, NULL);
	if (!psz)
	{
		return false;
	}
	memset(psz, 0, len + 1);
	if (len > nMax) len = nMax;
	WideCharToMultiByte(CP_ACP, 0, pwsz->GetData(), -1, psz, len, NULL, NULL);

	return true;
}

std::string CStdString2string(CStdString* pwsz)
{
	std::string str;
	int len = WideCharToMultiByte(CP_ACP, 0, pwsz->GetData(), -1, NULL, 0, NULL, NULL);

	char* psz = new char[len + 1];
	memset(psz, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, pwsz->GetData(), -1, psz, len, NULL, NULL);

	str = psz;
	delete psz;

	return str;
}
*/

std::wstring char2wstring(const char* psz)
{
	int len = MultiByteToWideChar(CP_ACP, 0, psz, -1, NULL, 0);
	wchar_t * pwsz = new wchar_t[len + 1];
	memset(pwsz, 0, len + 1);
	MultiByteToWideChar(CP_ACP, 0, psz, -1, pwsz, len);

	std::wstring ws = pwsz;
	delete[] pwsz;

	return ws;
}

// 解析过车信息为字符串，用于窗口显示
namespace Vechile
{
	std::wstring		GetLaneNum(TVechileData* pData)
	{
		if (pData == NULL) return std::wstring(TEXT(""));

		wchar_t ws[129];
		wsprintf(ws, TEXT("%d"), pData->tInfo.dwLaneNum);

		return std::wstring(ws);
	}

	std::wstring GetPlateNum(TVechileData* pData)
	{
		if (pData == NULL) return std::wstring(TEXT(""));

		return char2wstring(pData->tInfo.achFrontPlateNum);
	}

	std::wstring GetVehSpeed(TVechileData* pData)
	{
		if (pData == NULL) return std::wstring(TEXT("0 km/h"));

		wchar_t ws[129];
		wsprintf(ws, TEXT("%d km/h"), pData->tInfo.dwVehicleSpeed);
		return  std::wstring(ws);
	}

	std::wstring GetPlateColor(TVechileData* pData)
	{
		if (pData == NULL) return std::wstring(TEXT(""));

		return GetPlateColor(pData->tInfo.dwFrontPlateColor);
	}

	std::wstring GetPlateColor(Uint32 dwType)
	{

		switch (dwType)
		{
		case PLATE_COLOR_BLACK: // 黑
		{
			return std::wstring(TEXT("0xFF000000"));
		}
		break;
		case PLATE_COLOR_BLUE:
		{
			return std::wstring(TEXT("0xFF0000FF"));
		}
		break;
		case PLATE_COLOR_WHITE:
		{
			return std::wstring(TEXT("0xFFFFFFFF"));
		}
		break;
		case PLATE_COLOR_YELLOW:
		{
			return std::wstring(TEXT("0xFFFFFF00"));
		}
		break;
		case PLATE_COLOR_GREEN:
		{
			return std::wstring(TEXT("0xFF00FF00"));
		}
		break;
		default:
			return std::wstring(TEXT("0x00000000")); // 未知颜色
		}

		return std::wstring(TEXT("0x00000000"));
	}

	QString GetPlateColor2(Uint32 dwType)
	{
		//Uint32 dwType = pData->tInfo.dwFrontPlateColor;
		switch (dwType)
		{
		case PLATE_COLOR_BLACK: // 黑
		{
			return "black"; //0xFF000000;
		}
			break;
		case PLATE_COLOR_BLUE:
		{
			return "blue";//0xFF0000FF
		}
			break;
		case PLATE_COLOR_WHITE:
		{
			return "white";//0xFFFFFFFF;
		}
			break;
		case PLATE_COLOR_YELLOW:
		{
			return "yellow";//0xFFFFFF00;
		}
			break;
		case PLATE_COLOR_GREEN:
		{
			return "green";//0xFF00FF00;
		}
			break;
		default:
			return "0"; // 未知颜色
		}

		return "0";
	}

	QString GetPlateColor2(TVechileData* pData)
	{
		Uint32 dwType = pData->tInfo.dwFrontPlateColor;
		switch (dwType)
		{
		case PLATE_COLOR_BLACK: // 黑
		{
			return "black"; //0xFF000000;
		}
			break;
		case PLATE_COLOR_BLUE:
		{
			return "blue";//0xFF0000FF
		}
			break;
		case PLATE_COLOR_WHITE:
		{
			return "white";//0xFFFFFFFF;
		}
			break;
		case PLATE_COLOR_YELLOW:
		{
			return "yellow";//0xFFFFFF00;
		}
			break;
		case PLATE_COLOR_GREEN:
		{
			return "green";//0xFF00FF00;
		}
			break;
		default:
			return "0"; // 未知颜色
		}

		return "0";
	}

	QString GetPlateTextColor(TVechileData* pData)
	{
		if (pData == NULL) return 0;

		return GetPlateTextColor(pData->tInfo.dwFrontPlateColor);
	}

	QString GetPlateTextColor(Uint32 dwType)
	{

		switch (dwType)
		{
		case PLATE_COLOR_BLACK: // 黑
		{
			return "white";//0xFFFFFFFF
		}
		break;
		case PLATE_COLOR_BLUE:
		{
			return "white";//0xFFFFFFFF;
		}
		break;
		case PLATE_COLOR_WHITE:
		{
			return "red";
		}
		break;
		case PLATE_COLOR_YELLOW:
		{
			return "red";//0xFF000000
		}
		break;
		case PLATE_COLOR_GREEN:
		{
			return "red";
		}
		break;
		default:
			break;
		}

		return 0;
	}

	DWORD GetVehiclecColor2(TVechileData* pData)
	{
		if (pData == NULL)
		{
			return 0x00000000;
		}
		switch (pData->tInfo.nVehiclecColor)
		{
		case VEHICLE_COLOR_WHITE: // 黑
		{
			return 0xFFFFFFFF;
		}
			break;
		case VEHICLE_COLOR_GRAY:
		{
			return 0xFFC0C0C0;
		}
			break;
		case VEHICLE_COLOR_YELLOW:
		{
			return 0xFFFFFF00;
		}
			break;
		case VEHICLE_COLOR_RED:
		{
			return 0xFFFF0000;
		}
			break;
		case VEHICLE_COLOR_purple:
		{
			return 0xFFA020F0;
		}
			break;
		case VEHICLE_COLOR_GREEN:
		{
			return 0xFF00FF00;
		}
			break;
		case VEHICLE_COLOR_BLUE:
		{
			return 0xFF0000FF;
		}
			break;
		case VEHICLE_COLOR_BROWN:
		{
			return 0xFF802A2A;
		}
			break;
		case VEHICLE_COLOR_BLACK:
		{
			return 0xFF000000;
		}
			break;
		case VEHICLE_COLOR_ORANGE:
		{
			return 0xFFFF8000;
		}
			break;
		case VEHICLE_COLOR_INDIGO:
		{
			return 0xFF00FFFF;
		}
			break;
		case VEHICLE_COLOR_SILVERY:
		{
			return 0xFFC0C0C0;
		}
			break;
		case VEHICLE_COLOR_SILVERY_WHITE:
		{
			return 0xFFC0C0C0;
		}
			break;
		default:
			return 0x00000000; // 未知颜色
		}

		return 0x00000000;
	}

	std::wstring GetVehiclecColor(TVechileData* pData)
	{
		if (pData == NULL) return std::wstring(TEXT(""));

		return GetVehiclecColor(pData->tInfo.nVehiclecColor);
	}

	std::wstring GetVehiclecColor(Uint32 dwType)
	{
		switch (dwType)
		{
		case VEHICLE_COLOR_WHITE: // 黑
		{
			return std::wstring(TEXT("0xFFFFFFFF"));
		}
		break;
		case VEHICLE_COLOR_GRAY:
		{
			return std::wstring(TEXT("0xFFC0C0C0"));
		}
		break;
		case VEHICLE_COLOR_YELLOW:
		{
			return std::wstring(TEXT("0xFFFFFF00"));
		}
		break;
		case VEHICLE_COLOR_RED:
		{
			return std::wstring(TEXT("0xFFFF0000"));
		}
		break;
		case VEHICLE_COLOR_purple:
		{
			return std::wstring(TEXT("0xFFA020F0"));
		}
		break;
		case VEHICLE_COLOR_GREEN:
		{
			return std::wstring(TEXT("0xFF00FF00"));
		}
		break;
		case VEHICLE_COLOR_BLUE:
		{
			return std::wstring(TEXT("0xFF0000FF"));
		}
		break;
		case VEHICLE_COLOR_BROWN:
		{
			return std::wstring(TEXT("0xFF802A2A"));
		}
		break;
		case VEHICLE_COLOR_BLACK:
		{
			return std::wstring(TEXT("0xFF000000"));
		}
		break;
		case VEHICLE_COLOR_ORANGE:
		{
			return std::wstring(TEXT("0xFFFF8000"));
		}
		break;
		case VEHICLE_COLOR_INDIGO:
		{
			return std::wstring(TEXT("0xFF00FFFF"));
		}
		break;
		case VEHICLE_COLOR_SILVERY:
		{
			return std::wstring(TEXT("0xFFC0C0C0"));
		}
		break;
		case VEHICLE_COLOR_SILVERY_WHITE:
		{
			return std::wstring(TEXT("0xFFC0C0C0"));
		}
		break;
		default:
			return std::wstring(TEXT("0x00000000")); // 未知颜色
		}

		return std::wstring(TEXT("0x00000000"));
	}

	std::wstring GetVehiclecLogo(TVechileData* pData)
	{
		return char2wstring(pData->tInfo.achVehiclelogo);
	}

	std::wstring GetVehiclecType(TVechileData* pData)
	{
		if (pData == NULL) return std::wstring(TEXT(""));

		return GetVehiclecType(pData->tInfo.dwVehiclesubclass);
	}

	std::wstring GetVehiclecType(Uint32 dwType)
	{
		switch (dwType)
		{
		case EM_VEHICLE_SUB_CLASS_CAR:
		{
			return std::wstring(TEXT("轿车"));
		}
		break;
		case EM_VEHICLE_SUB_CLASS_BUS:
		{
			return std::wstring(TEXT("客车"));
		}
		break;
		case EM_VEHICLE_SUB_CLASS_VAN:
		{
			return std::wstring(TEXT("面包车"));
		}
		break;
		case EM_VEHICLE_SUB_CLASS_HEAVY_TRUCK:
		{
			return std::wstring(TEXT("重型货车"));
		}
		break;
		case EM_VEHICLE_SUB_CLASS_LIGHT_TRUCK:
		{
			return std::wstring(TEXT("轻微型货车"));
		}
		break;
		case EM_VEHICLE_SUB_CLASS_MEDIUM_BUS:
		{
			return std::wstring(TEXT("中型客车"));
		}
		break;
		case EM_VEHICLE_SUB_CLASS_SUV:
		{
			return std::wstring(TEXT("SUV"));
		}
		break;
		case EM_VEHICLE_SUB_CLASS_MPV:
		{
			return std::wstring(TEXT("MPV"));
		}
		break;
		case EM_VEHICLE_SUB_CLASS_PICKUP:
		{
			return std::wstring(TEXT("皮卡"));
		}
		break;
		case EM_VEHICLE_SUB_CLASS_XIAO_TRUCK:
		{
			return std::wstring(TEXT("小型货车"));
		}
		break;
		case EM_VEHICLE_SUB_CLASS_ZHONG_TRUCK:
		{
			return std::wstring(TEXT("中型货车"));
		}
		break;
		case EM_VEHICLE_SUB_CLASS_BUS_XIAO:
		{
			return std::wstring(TEXT("小客车"));
		}
		break;
		case EM_VEHICLE_SUB_CLASS_XIANG_TRUCK:
		{
			return std::wstring(TEXT("箱式货车"));
		}
		break;
		default:
		{
			return std::wstring(TEXT("未识别"));
		}

		}
	}

	std::wstring GetIllegalType(TVechileData* pData)
	{
		if (pData == NULL) return std::wstring(TEXT(""));

		wchar_t ws[129];
		wchar_t ws2[129];
		memset(ws, 0, 129);
		memset(ws2, 0, 129);
		std::string str(pData->tInfo.achIllegalType);

		if (pData->tInfo.dwIllegalState == 0)
		{
			wsprintf(ws, TEXT("%s"), TEXT("卡口事件"));
			return std::wstring(ws);
		}

		// 只显示第一个违法类型
		if (!str.empty())
		{
			int nFind = str.find(',');
			if (nFind == -1)
			{
				nFind = str.size();
			}

			std::string str1 = str.substr(0, nFind);
			str.erase(0, nFind);
			Uint32 code = atoi(str1.c_str());
			switch (code)
			{
			case ILLEGAL_TYPE_OVER_SPEED:
				wsprintf(ws2, TEXT("%s"), TEXT("超速"));
				break;
			case ILLEGAL_TYPE_RETROGRADE:
				wsprintf(ws2, TEXT("%s"), TEXT("逆行"));
				break;
			case ILLEGAL_TYPE_NON_PRESCRIBED_LANE:
				wsprintf(ws2, TEXT("%s"), TEXT("不按车道行驶"));
				break;
			case ILLEGAL_TYPE_ZYFJDCCD:
				wsprintf(ws2, TEXT("%s"), TEXT("机动车占用非机动车道"));
				break;
			case ILLEGAL_TYPE_ZYYJCD:
				wsprintf(ws2, TEXT("%s"), TEXT("占用应急车道"));
				break;
			case ILLEGAL_TYPE_ZYGJCD:
				wsprintf(ws2, TEXT("%s"), TEXT("占用公交车道"));
				break;
			case ILLEGAL_TYPE_ZYXXCCD:
				wsprintf(ws2, TEXT("%s"), TEXT("大型车占用小型车道"));
				break;
			case ILLEGAL_TYPE_JZHCTX:
				wsprintf(ws2, TEXT("%s"), TEXT("禁止货运车通行"));
				break;
			case ILLEGAL_TYPE_SOLIDLINE:
				wsprintf(ws2, TEXT("%s"), TEXT("压实线"));
				break;
			case ILLEGAL_TYPE_SOLID_LINE:
				wsprintf(ws2, TEXT("%s"), TEXT("实线变道"));
				break;
			case ILLEGAL_TYPE_THROUGHT_RED_RIGHT:
				wsprintf(ws2, TEXT("%s"), TEXT("闯红灯"));
				break;
			case ILLEGAL_TYPE_NOT_CORRECT_LANE:
				wsprintf(ws2, TEXT("%s"), TEXT("不按导向车道行驶"));
				break;
			case ILLEGAL_TYPE_LINE_STOP:
				wsprintf(ws2, TEXT("%s"), TEXT("越线停车"));
				break;
			case ILLEGAL_TYPE_crossing_retention:
				wsprintf(ws2, TEXT("%s"), TEXT("路口滞留"));
				break;
			case ILLEGAL_TYPE_SAFETY_BELT:
				wsprintf(ws2, TEXT("%s"), TEXT("主驾未系安全带"));
				break;
			case ILLEGAL_TYPE_SEC_SAFETY_BELT:
				wsprintf(ws2, TEXT("%s"), TEXT("副驾未系安全带"));
				break;
			case ILLEGAL_TYPE_MAKE_PHONE:
				wsprintf(ws2, TEXT("%s"), TEXT("主驾开车打电话"));
				break;
			case ILLEGAL_TYPE_TRAFFIC_SIGNS:
				wsprintf(ws2, TEXT("%s"), TEXT("禁止左转"));
				break;
			case ILLEGAL_TYPE_DWXZ:
				wsprintf(ws2, TEXT("%s"), TEXT("大弯小转"));
				break;
			case ILLEGAL_TYPE_STOP:
				wsprintf(ws2, TEXT("%s"), TEXT("违法停车"));
				break;
			case ILLEGAL_TYPE_ILLEGA_TURN:
				wsprintf(ws2, TEXT("%s"), TEXT("违法掉头"));
				break;
			case ILLEGAL_TYPE_pedestrian_crossing_stop:
				wsprintf(ws2, TEXT("%s"), TEXT("黄网格停车"));
				break;
			case ILLEGAL_TYPE_NO_comity_pedestrian:
				wsprintf(ws2, TEXT("%s"), TEXT("机动车未礼让行人"));
				break;
			case ILLEGAL_TYPE_VEH_BACK_OFF:
				wsprintf(ws2, TEXT("%s"), TEXT("快速路违法倒车"));
				break;
			case ILLEGAL_TYPE_swerve_No_comity_straight:
				wsprintf(ws2, TEXT("%s"), TEXT("转弯未让直行"));
				break;
			case ILLEGAL_TYPE_pedestrian_run_red_light:
				wsprintf(ws2, TEXT("%s"), TEXT("非机动车闯红灯"));
				break;
			case ILLEGAL_TYPE_MUCK_CARS:
				wsprintf(ws2, TEXT("%s"), TEXT("渣土车不按规定行驶"));
				break;
			case ILLEGAL_TYPE_NO_ENOUGH_DISTANCE:
				wsprintf(ws2, TEXT("%s"), TEXT("未保持行车安全距离"));
				break;
			case ILLEGAL_TYPE_No_Car_Horns:
				wsprintf(ws2, TEXT("%s"), TEXT("违章鸣笛"));
				break;
			case ILLEGAL_TYPE_DRIVING_LIMIT:
				wsprintf(ws2, TEXT("%s"), TEXT("车辆限行"));
				break;
			case ILLEGAL_TYPE_FJDCZYJDCD:
				wsprintf(ws2, TEXT("%s"), TEXT("非机动车占用机动车道"));
				break;
			case ILLEGAL_TYPE_FJDCZR:
				wsprintf(ws2, TEXT("%s"), TEXT("非机动车载人"));
				break;
			case ILLEGAL_TYPE_FJDJZSJ:
				wsprintf(ws2, TEXT("%s"), TEXT("非机动车加装伞具"));
				break;
			default:
				wsprintf(ws2, TEXT("%d"), code);
				break;
			}

			if (wcslen(ws) > 0)
			{
				wsprintf(ws, TEXT("%s/%s"), ws, ws2);
			}
			else
			{
				wsprintf(ws, TEXT("%s"), ws2);
			}
		}

		return std::wstring(ws);
	}

	std::wstring GetThroughTime(TVechileData* pData)
	{
		if (pData == NULL) return std::wstring(TEXT(""));

		struct timeval tv;
		tv.tv_sec = pData->tInfo.qwThroughTime / 1000;
		tv.tv_usec = pData->tInfo.qwThroughTime % 1000 * 1000;

		wchar_t ws[129];
		time_t ttime = tv.tv_sec;
		if (ttime < 0)
		{
			ttime = 0;
		}
		struct tm *pTmp = localtime(&ttime);

		wsprintf(ws, TEXT("%d-%d-%d %d:%d:%d"), pTmp->tm_year + 1900, pTmp->tm_mon + 1, pTmp->tm_mday, pTmp->tm_hour, pTmp->tm_min, pTmp->tm_sec);

		return std::wstring(ws);
	}

	std::wstring GetLocation(TVechileData* pData)
	{
		if (pData == NULL) return std::wstring(TEXT(""));

		return std::wstring(char2wstring(pData->tInfo.achPointNum));
	}
}



namespace trag
{

	// 检查点是否越界，越界后修正
	void LinePointOverCheck(int& x1, int& y1, int x2, int y2, QRect rcArea)
	{
		if (x1 < rcArea.left())
		{
			if (x1 == x2 || x1 == 0 || x2 == 0) return;
			if (y1 == 0 || y1 == y2)
			{
				x1 = rcArea.left();
				return;
			}

			// k = (y - b) / x;
			// 推导出 b1=(y2x1-y1x2)/(x1-x2)
			int b1 = (y2 * x1 - y1 * x2) / (x1 - x2);
			double k1 = (y1 - b1) / (double)x1;

			// 交点: K1X+B1=K2X+B2
			// 推导出 x=(b2-b1)/(k1-k2)
			int xJD = rcArea.left();
			// Y=K1X+B1
			int yJD = (int)(k1*xJD + b1);
			yJD = yJD < rcArea.top() ? rcArea.top() : yJD;
			yJD = yJD > rcArea.bottom() ? rcArea.bottom() : yJD;

			x1 = xJD;
			y1 = yJD;
		}
		else if (x1 > rcArea.right())
		{
			if (x1 == x2 || x1 == 0 || x2 == 0) return;
			if (y1 == 0 || y1 == y2)
			{
				x1 = rcArea.right();
				return;
			}

			// k = (y - b) / x;
			// 推导出 b1=(y2x1-y1x2)/(x1-x2)
			int b1 = (y2 * x1 - y1 * x2) / (x2 - x1);
			double k1 = (y1 - b1) / (double)x1;

			//
			int xJD = rcArea.right();
			int yJD = (int)(k1*xJD + b1);
			yJD = yJD < rcArea.top() ? rcArea.top() : yJD;
			yJD = yJD > rcArea.bottom() ? rcArea.bottom() : yJD;

			x1 = xJD;
			y1 = yJD;
		}
		else if (y1 < rcArea.top())
		{
			if (y1 == y2 || y1 == 0 || y2 == 0)			return;
			if (x1 == 0 || x1 == x2)
			{
				y1 = rcArea.top();
				return;
			}

			// k = (y - b) / x;
			// 推导出 b1=(y2x1-y1x2)/(x1-x2)
			int b1 = (x2 * y1 - x1 * y2) / (y1 - y2);
			double k1 = (x1 - b1) / (double)y1;

			// k = (y - b) / x;
			int yJD = rcArea.top();
			int xJD = (int)(k1*yJD + b1);
			xJD = xJD < rcArea.left() ? rcArea.left() : xJD;
			xJD = xJD > rcArea.right() ? rcArea.right() : xJD;

			x1 = xJD;
			y1 = yJD;
		}
		else if (y1 > rcArea.bottom())
		{
			if (y1 == y2 || y1 == 0 || y2 == 0)			return;
			if (x1 == 0 || x1 == x2)
			{
				y1 = rcArea.bottom();
				return;
			}

			// k = (y - b) / x;
			// 推导出 b1=(y2x1-y1x2)/(x1-x2)
			int b1 = (x2 * y1 - x1 * y2) / (y1 - y2);
			double k1 = (x1 - b1) / (double)y1;

			// k = (y - b) / x;
			int yJD = rcArea.bottom();
			int xJD = (int)(k1*yJD + b1);
			xJD = xJD < rcArea.left() ? rcArea.left() : xJD;
			xJD = xJD > rcArea.right() ? rcArea.right() : xJD;

			// 		wchar_t sz[125];
			// 		wsprintf(sz, TEXT("xJD = %d    k1 = %d    b1 = %d    x1 = %d   y1 = %d\n"), xJD, k1, b1, x1, y1);
			// 		TRACE(sz);

			x1 = xJD;
			y1 = yJD;
		}

	}

	// 检查直线中的点是否越界，越界后修正
	void LinePointOverCheckEx(POINT& pt1, POINT pt2, QRect rcArea)
	{
		//pubtrag::LinePointOverCheck((int)pt1.x, (int)pt1.y, (int)pt2.x, (int)pt2.y, rcArea);
	}


	bool ptInPolygonf(float ax[], float ay[], int nNum, float x, float y)
	{
		if (nNum < 3) return false;

		int   i, j = nNum - 1;
		bool  bIn = false;

		for (i = 0; i < nNum; i++)
		{
			if ((ay[i] < y && ay[j] >= y || ay[j] < y && ay[i] >= y)
				&& (ax[i] <= x || ax[j] <= x)
				)
			{
				if (ax[i] + (y - ay[i]) / (ay[j] - ay[i]) * (ax[j] - ax[i]) < x)
				{
					bIn = !bIn;
				}
			}
			j = i;
		}

		return bIn;
	}

	bool ptInPolygon(int ax[], int ay[], int nNum, int x, int y)
	{
		float afx[12], afy[12], fx, fy;

		if (nNum > 12)
		{
			return false;
		}

		for (int i = 0; i < nNum; i++)
		{
			afx[i] = (float)ax[i];
			afy[i] = (float)ay[i];
		}

		fx = (float)x;
		fy = (float)y;

		return ptInPolygonf(afx, afy, nNum, fx, fy);
	}

	// 找出多边形的区域的rect
	RECT GetPolygonRect(int ax[], int ay[], int nNum)
	{
		RECT rc;
		memset(&rc, 0, sizeof(RECT));
		if (nNum < 3) return rc;

		rc.left = ax[0]; rc.right = ax[0];
		rc.top = ay[0]; rc.bottom = ay[0];

		for (int i = 0; i < nNum; i++)
		{
			if (ax[i] < rc.left) rc.left = ax[i];
			if (ax[i] > rc.right) rc.right = ax[i];
			if (ay[i] < rc.top) rc.top = ay[i];
			if (ay[i] > rc.bottom) rc.bottom = ay[i];
		}

		return rc;
	}


}





