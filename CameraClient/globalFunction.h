#pragma once
#include "stdafx.h"
#include <string.h>
#include "ivhs_struct.h"
#include "define.h"
#include <QRect>
#define SAFE_DELETE(obj) \
	if(obj) \
				{       \
	  delete obj; \
	  obj = nullptr; \
			}  

bool wchar2char(char* psz, const wchar_t* pwsz);
bool char2wchar(wchar_t* pwsz, const char* psz);
std::wstring char2wstring(const char* psz);


//bool	DrawStr(HDC hdc, wchar_t* szDraw, int *outWidth = NULL, int x = 0, int y = 0, int nFontSize = 32, wchar_t szFontName[16] = TEXT("宋体"), int maxwidth = 0);
//BOOL	SaveBitmapToFile(HBITMAP hBitmap, LPCTSTR lpszFileName);
//int		BmpToY(char *pFilePath, char *pBufY, u32 *pWidth, u32 *pHeight);
namespace Vechile
{
	std::wstring GetLaneNum(TVechileData* pData);
	std::wstring GetPlateNum(TVechileData* pData);
	std::wstring GetPlateColor(TVechileData* pData);
	std::wstring GetPlateColor(Uint32 dwType);
	QString GetPlateColor2(TVechileData* pData);
	QString GetPlateColor2(Uint32 dwType);
	QString	GetPlateTextColor(TVechileData* pData);
	QString	GetPlateTextColor(Uint32 dwType);
	std::wstring GetVehSpeed(TVechileData* pData);
	std::wstring GetVehiclecColor(TVechileData* pData);
	std::wstring GetVehiclecColor(Uint32 dwType);
	DWORD GetVehiclecColor2(TVechileData* pData);
	std::wstring GetVehiclecLogo(TVechileData* pData);
	std::wstring GetVehiclecType(TVechileData* pData);
	std::wstring GetVehiclecType(Uint32 dwType);
	std::wstring GetIllegalType(TVechileData* pData);
	std::wstring GetThroughTime(TVechileData* pData);
	std::wstring GetLocation(TVechileData* pData);
}



namespace trag
{

	// 检查点是否越界，越界后修正
	void LinePointOverCheck(int& x1, int& y1, int x2, int y2, QRect rcArea);

	// 检查直线中的点是否越界，越界后修正
	void LinePointOverCheckEx(POINT& pt1, POINT pt2, QRect rcArea);


	// 判断点是否在多边形中
	bool ptInPolygonf(float ax[], float ay[], int nNum, float x, float y);
	bool ptInPolygon(int ax[], int ay[], int nNum, int x, int y);

	// 找出多边形的区域的rect
	RECT GetPolygonRect(int ax[], int ay[], int nNum);

}
