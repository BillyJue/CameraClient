#include "stdafx.h"
#include <string.h>
#include "ivhs_struct.h"
#include "define.h"
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
using namespace std;


// 解析视频线圈道信息为字符串，用于窗口显示
namespace videoloop
{
	enum EVIDEOLOOP
	{
		// 编号
		EVIDEOLOOP_SN = 0,
		// 使能
		EVIDEOLOOP_ENABLED,
		// 起点X
		EVIDEOLOOP_STARTX,
		// 起点Y
		EVIDEOLOOP_STARTY,
		// 宽度
		EVIDEOLOOP_WIDTH,
		// 高度
		EVIDEOLOOP_HEIGHT,
		// 起点X1
		EVIDEOLOOP_STARTX1,
		// 起点Y1
		EVIDEOLOOP_STARTY1,
		// 宽度1
		EVIDEOLOOP_WIDTH1,
		// 起点X2
		EVIDEOLOOP_STARTX2,
		// 起点Y2
		EVIDEOLOOP_STARTY2,
		// 宽度2
		EVIDEOLOOP_WIDTH2,
		// 检测方式
		EVIDEOLOOP_MODE,
		// 方向
		EVIDEOLOOP_DIR,
		// 号牌角度
		EVIDEOLOOP_ANGLE,
		// 存在时间
		EVIDEOLOOP_TIME,
		// 目标高度
		EVIDEOLOOP_TARGET_WIDHT,
		// 目标宽度
		EVIDEOLOOP_TARGET_HEIGTH,
		// 灵敏度
		EVIDEOLOOP_SENSITIVE,
		// 进入滤波
		EVIDEOLOOP_FILTERON,
		// 离开滤波
		EVIDEOLOOP_FILTEROFF,
		EVIDEOLOOP_COUNT
	};
	std::wstring		GetListValue(TIPC_VidLoopCfg* pData, int em);

	std::wstring    DirectionMode(u32 dwDir);
	int GetDirectionModeIndex(u32 dwDir);
	int GetDirectionModeValue(u32 index);
	// 解析号牌角度模式
	std::wstring		AnalyAngleMode(u32 dwMode);
	int GetAngleModeIndex(u32 dwMode);
	int GetAngleModeValue(u32 index);
	// 解析检测类型
	std::wstring		AnalyChkType(u32 dwType);
	int GetMethodIndex(u32 method);
	int GetMethodValue(u32 index, bool bTrapezium);
};

// 线圈检测器
namespace coilloop
{
	std::wstring		GetListValue(TIPC_CoilLoopCfg* pData, int em);
};

// 雷达检测器
namespace redarloop
{
	enum EREDARLOOP
	{
		// 编号
		EREDARLOOP_SN = 0,
		// 使能
		EREDARLOOP_ENABLED,
		// 串口通道号
		EREDARLOOP_SERIAL,
		// 雷达类型
		EREDARLOOP_TYPE,
		// 工作模式
		//EREDARLOOP_MODE,
		// 雷达与相机距离
		EREDARLOOP_DISTANCE,
		// 速度修正系数
		EREDARLOOP_SPEED,
		// 对应车道
		EREDARLOOP_LANE,

		EREDARLOOP_COUNT
	};
	std::wstring		GetListValue(TIPC_RadarLoopCfg* pData, int em);
};

// 红灯信号检测器
namespace RedLampSig
{
	std::wstring		GetListValue(TIPC_RedLampSigLoop* pData, int em);
};

// 识别区域
namespace recogarea
{
	/*识别区域*/
	typedef struct
	{
		/*编号*/
		Uint32  dwLabel;
		/*使能*/
		Bool    bEnable;
		/*区域*/
		Int32	X1;
		Int32	Y1;
		Uint32	W1;
		Int32	X2;
		Int32	Y2;
		Uint32	W2;
		/*号牌宽度*/
		Uint32  dwPlateWidth;
		/*号牌角度*/
		Uint32  dwPlateAngle;
		/*号牌视角:  1 为抓拍车辆头部，2 为抓拍车辆尾部;*/
		Uint32  dwShootRear;
	}TRecogAreaParam;

	std::wstring		GetListValue(TRecogAreaParam* pData, int em);
};

// 红灯增强
namespace redincrease
{
	std::wstring		GetListValue(TIPC_RedSigEnhance* pData, int em);
}


namespace data2
{
	void Line2DrawData(TLine_S* pLine, tagDrawData* pData);

	void DrawData2Line(TLine_S* pLine, tagDrawData* pData);

	void Rect2DrawData(TRECTANGLE_S* pRect, tagDrawData* pData);

	void DrawData2Rect(TRECTANGLE_S* pRect, tagDrawData* pData);

	void Rect2Rgn4(TRECTANGLE_S* pRect, TRoiArea_S* pRgn);

	void Rgn42Rect(TRECTANGLE_S* pRect, TRoiArea_S* pRgn);

	void DrawData2Rgn(TRoiArea_S* pRgn, tagDrawData* pData);

	void Rgn2DrawData(TRoiArea_S* pRgn, tagDrawData* pData);


	void DrawData2Rgn4(TRoiArea_S* pRgn, tagDrawData* pData);

	void Rgn42DrawData(TRoiArea_S* pRgn, tagDrawData* pData);

}

namespace scene
{
	// 区域绘制线条和区域类型及转换
	// 线条类型
	enum EMLINETYPE
	{
		// 车道左边线
		EMLINETYPE_LANE_LEFT = 0,
		// 车道右边线RIGHT
		EMLINETYPE_LANE_RIGHT,
		// 车道12分割线
		EMLINETYPE_LANE_12,
		// 车道23分割线
		EMLINETYPE_LANE_23,
		// 车道34分割线
		EMLINETYPE_LANE_34,
		// 检测触发线
		EMLINETYPE_TRIGGER,
		// 左转判定线
		EMLINETYPE_JUDGE_LEFT,
		// 直行判定线
		EMLINETYPE_JUDGE_ZX,
		// 右转判定线
		EMLINETYPE_JUDGE_RIGHT,
		// 停车线1
		EMLINETYPE_STOP1,
		// 停车线2
		EMLINETYPE_STOP2,
		// 停车线3
		EMLINETYPE_STOP3,
		// 停车线4
		EMLINETYPE_STOP4,
		// 
		EMLINETYPE_COUNT
	};

	// 区域类型
	enum EMAREATYPE
	{
		EMAREATYPE_TRACK = EMLINETYPE_COUNT,

		EMAREATYPE_PERSON_WAIT,

		EMAREATYPE_PERSON,

		EMAREATYPE_WARNING,
		// 区域1
		EMAREATYPE_AREA1,
		// 区域2
		EMAREATYPE_AREA2,
		// 区域3
		EMAREATYPE_AREA3,
		// 区域4
		EMAREATYPE_AREA4,

		// 
		EMAREATYPE_COUNT
	};

	// 自定义规则线类型
	enum EMCUSTOMTYPE
	{
		EMCUSTOMTYPE_1 = EMAREATYPE_COUNT,
		// 
		EMCUSTOMTYPE_2,
		// 
		EMCUSTOMTYPE_3,
		// 
		EMCUSTOMTYPE_4,
		// 
		EMCUSTOMTYPE_5,
		// 
		EMCUSTOMTYPE_6,
		// 
		EMCUSTOMTYPE_7,
		// 
		EMCUSTOMTYPE_8,
		// 
		EMCUSTOMTYPE_9,
		// 
		EMCUSTOMTYPE_10,
		// 
		EMCUSTOMTYPE_11,
		// 
		EMCUSTOMTYPE_12,
		// 
		EMCUSTOMTYPE_13,
		// 
		EMCUSTOMTYPE_14,
		// 
		EMCUSTOMTYPE_15,
		// 
		EMCUSTOMTYPE_16,

		// 
		EMCUSTOMTYPE_COUNT
	};

	// 自定义区域类型
	enum EMCUSTOMTYPE_AREA
	{
		// 
		EMCUSTOMTYPE_AREA_1 = EMCUSTOMTYPE_COUNT,
		// 
		EMCUSTOMTYPE_AREA_2,
		// 
		EMCUSTOMTYPE_AREA_3,
		// 
		EMCUSTOMTYPE_AREA_4,

		// 
		EMCUSTOMTYPE_AREA_COUNT,
	};

	// 自定义多边形区域类型
	enum EMCUSTOMTYPE_POLYGON
	{
		// 
		EMCUSTOMTYPE_POLYGON_1 = EMCUSTOMTYPE_AREA_COUNT,
		// 
		EMCUSTOMTYPE_POLYGON_2,
		// 
		EMCUSTOMTYPE_POLYGON_3,
		// 
		EMCUSTOMTYPE_POLYGON_4,

		// 
		EMCUSTOMTYPE_POLYGON_COUNT,
	};

}

// 解析逻辑车道信息为字符串，用于窗口显示
#define MAX_LOGICLANE_TYPE_NUM 31
namespace logiclane
{
	enum EMLOGICLANE
	{
		// 编号
		EMLOGICLANE_SN = 0,
		// 使能
		EMLOGICLANE_ENABLED,
		// 检测类型
		EMLOGICLANE_CHKTYPE,
		// 灵敏度
		EMLOGICLANE_SENSITIVITY,
		// 识别区域
		EMLOGICLANE_IDENTID,
		// 车道ID
		EMLOGICLANE_LANEID,
		// 白天补光
		EMLOGICLANE_FILLLIGHT_DAY,
		// 夜间补光
		EMLOGICLANE_FILLLIGHT_NIGHT,
		// 人像曝光
		EMLOGICLANE_EXPOSURE_PEOPLE,
		// 违章代码
		EMLOGICLANE_ILLEGALCODE,
		// 详细
		EMLOGICLANE_DETAILS,
		// 
		EMLOGICLANE_COUNT
	};
	wstring		GetListValue(TIPC_LogicLane* pData, int em);

	// 解析灯光模式
	wstring		AnalyLightMode(u32 dwMode);
	int			AnalyLightMode2Index(u32 dwMode);
	u32			AnalyIndex2LightMode(int nIndex);
	// 解析检测类型
	wstring		AnalyChkType(u32 dwType);
	int			AnalyCheckType2Index(u32 dwMode);
	int			AnalyCheckExtType2Index(u32 dwMode);
	u32			AnalyIndex2CheckType(int nIndex);
	u32			AnalyIndex2CheckExtType(int nIndex);

	enum EMLOGICLANEDETAILS
	{
		// 检测器1
		EMLOGICLANEDETAILS_EDT_DECTOR1 = 0,
		// 检测器2
		EMLOGICLANEDETAILS_EDT_DECTOR2,
		// 检测器3
		EMLOGICLANEDETAILS_EDT_DECTOR3,
		// 检测器4
		EMLOGICLANEDETAILS_EDT_DECTOR4,
		// 辅助检测器1
		EMLOGICLANEDETAILS_CHK_DECTOR1,
		// 辅助检测器2
		EMLOGICLANEDETAILS_CHK_DECTOR2,
		// 辅助检测器3
		EMLOGICLANEDETAILS_CHK_DECTOR3,
		// 辅助检测器4
		EMLOGICLANEDETAILS_CHK_DECTOR4,
		// 图片2白天补光方式
		EMLOGICLANEDETAILS_CMB_PIC2DAY,
		// 图片2夜晚补光方式
		EMLOGICLANEDETAILS_CMB_PIC2NIGHT,
		// 图片3白天补光方式
		EMLOGICLANEDETAILS_CMB_PIC3DAY,
		// 图片3夜晚补光方式
		EMLOGICLANEDETAILS_CMB_PIC3NIGHT,
		// 图片4白天补光方式
		EMLOGICLANEDETAILS_CMB_PIC4DAY,
		// 图片4夜晚补光方式
		EMLOGICLANEDETAILS_CMB_PIC4NIGHT,
		// 抓拍张数
		EMLOGICLANEDETAILS_CMB_SNAPNUM,
		// 辅助抓拍张数
		EMLOGICLANEDETAILS_EDT_SNAPNUM2,
		// 最小触发抓拍间隔时间
		EMLOGICLANEDETAILS_EDT_MINSNAPTIME,
		// 抓拍间隔控制
		EMLOGICLANEDETAILS_CMB_SNAPCTRL,
		// 1|2间隔时间(MS)
		EMLOGICLANEDETAILS_EDT_SPEEDTIME12,
		// 2|3间隔时间(MS):
		EMLOGICLANEDETAILS_EDT_SPEEDTIME23,
		// 1|2间隔距离(MS)
		EMLOGICLANEDETAILS_EDT_SPEEDRANG12,
		// 2|3间隔距离(MS):
		EMLOGICLANEDETAILS_EDT_SPEEDRANG23,
		// 抓拍车牌子图:
		EMLOGICLANEDETAILS_CHK_CHILD_PLATE,
		// 宽
		EMLOGICLANEDETAILS_EDT_CHILDPLATE_WIDHT,
		// 高
		EMLOGICLANEDETAILS_EDT_CHILDPLATE_HEIGHT,
		// 抓拍车辆子图:
		EMLOGICLANEDETAILS_CHK_CHILD_VEHICLE,
		// 宽
		EMLOGICLANEDETAILS_EDT_CHILDVEHICLE_WIDHT,
		// 高
		EMLOGICLANEDETAILS_EDT_CHILDVEHICLE_HEIGHT,
		// 抓拍人脸子图:
		EMLOGICLANEDETAILS_CHK_CHILD_POEPLE,
		// 宽
		EMLOGICLANEDETAILS_EDT_CHILDPOEPLE_WIDHT,
		// 高
		EMLOGICLANEDETAILS_EDT_CHILDPOEPLE_HEIGHT,

		EMLOGICLANEDETAILS_EDT_CHILDPOEPLE_WIDHT_2,//贴图
		// 高
		EMLOGICLANEDETAILS_EDT_CHILDPOEPLE_HEIGHT_2,
		// 叠加人脸子图
		EMLOGICLANEDETAILS_CHK_ADD_FACE,
		// 测试校验
		EMLOGICLANEDETAILS_CHK_SPEED_CHECK,
		// 小车限速(MS/H)
		EMLOGICLANEDETAILS_EDT_RATELIMIT_LITTLE,
		// 大车限速(MS/H)
		EMLOGICLANEDETAILS_EDT_RATELIMIT_BIG,
		// 小车起拍速度速(MS/H)
		EMLOGICLANEDETAILS_EDT_SNAPLIMIT_LITTLE,
		// 大车起拍速度速
		EMLOGICLANEDETAILS_EDT_SNAPLIMIT_BIG,
		// 速度偏差限幅
		EMLOGICLANEDETAILS_EDT_RATEOFFSET,
		// 关联红灯相位
		EMLOGICLANEDETAILS_CHKREDLIGHT_LEFT,
		// 关联红灯相位
		EMLOGICLANEDETAILS_CHKREDLIGHT_LINE,
		// 关联红灯相位
		EMLOGICLANEDETAILS_CHKREDLIGHT_RIGHT,
		// 最小红灯时间
		EMLOGICLANEDETAILS_EDT_MINREDLIGHTTIME,
		// 最大红灯时间
		EMLOGICLANEDETAILS_EDT_MAXREDLIGHTTIME,
		// 跟踪置信度
		EMLOGICLANEDETAILS_EDT_GZZXD,
		// 匹配字符数
		EMLOGICLANEDETAILS_CMB_ZFPP,
		// 号牌过滤
		EMLOGICLANEDETAILS_CHK_HPGL_YELLOW,
		EMLOGICLANEDETAILS_CHK_HPGL_WHITE,
		EMLOGICLANEDETAILS_CHK_HPGL_BLUE,
		EMLOGICLANEDETAILS_CHK_HPGL_BLACK,
		EMLOGICLANEDETAILS_CHK_HPGL_GREEN,
		EMLOGICLANEDETAILS_CHK_HPGL_YELLOW_GREEN,
		EMLOGICLANEDETAILS_CHK_HPGL_NO,
		// 抓拍车辆
		EMLOGICLANEDETAILS_CHK_VEH_0,
		EMLOGICLANEDETAILS_CHK_VEH_1,
		EMLOGICLANEDETAILS_CHK_VEH_2,
		EMLOGICLANEDETAILS_CHK_VEH_3,
		EMLOGICLANEDETAILS_CHK_VEH_4,
		EMLOGICLANEDETAILS_CHK_VEH_5,
		EMLOGICLANEDETAILS_CHK_VEH_6,
		EMLOGICLANEDETAILS_CHK_VEH_7,
		EMLOGICLANEDETAILS_CHK_VEH_8,
		// 方向过滤
		EMLOGICLANEDETAILS_CMB_FILTERDIR,

		// 红灯极性
		EMLOGICLANEDETAILS_CMB_JX,
		// 
		EMLOGICLANEDETAILS_CHK_MERGE_LEFT_TOP, //头像叠加方向  左上
		EMLOGICLANEDETAILS_CHK_MERGE_LEFT_BOM,	//左下
		EMLOGICLANEDETAILS_CHK_MERGE_RIGHT_TOP, //右上
		EMLOGICLANEDETAILS_CHK_MERGE_RIGHT_BOM,//右下

		EMLOGICLANEDETAILS_CHK_MERGE_SELECT_DEST,//框选目标

		EMLOGICLANEDETAILS_COUNT
	};

	bool		SetEdtValue(void* pDataIn, QLineEdit* pEdt, int em);
	bool		GetEdtValue(void* pDataOut, QLineEdit* pEdt, int em);

	bool		SetChkValue(void* pDataIn, QCheckBox* pEdt, int em);
	bool		GetChkValue(void* pDataOut, QCheckBox* pEdt, int em);

	bool		SetCmbValue(void* pDataIn, QComboBox* pEdt, int em);
	bool		GetCmbValue(void* pDataOut, QComboBox* pEdt, int em);

	// 解析抓拍间隔控制
	int			AnalySnapCtrlMode2Index(u32 dwMode);
	u32			AnalyIndex2SnapCtrlMode(int nIndex);
	// 解析红灯相位模式
	int			AnalyRedLightPhase2Index(u32 dwMode);
	u32			AnalyIndex2RedLightPhase(int nIndex);





};
