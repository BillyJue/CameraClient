#include "stdafx.h"
#include <string.h>
#include "ivhs_struct.h"
#include "define.h"
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
using namespace std;


// ������Ƶ��Ȧ����ϢΪ�ַ��������ڴ�����ʾ
namespace videoloop
{
	enum EVIDEOLOOP
	{
		// ���
		EVIDEOLOOP_SN = 0,
		// ʹ��
		EVIDEOLOOP_ENABLED,
		// ���X
		EVIDEOLOOP_STARTX,
		// ���Y
		EVIDEOLOOP_STARTY,
		// ���
		EVIDEOLOOP_WIDTH,
		// �߶�
		EVIDEOLOOP_HEIGHT,
		// ���X1
		EVIDEOLOOP_STARTX1,
		// ���Y1
		EVIDEOLOOP_STARTY1,
		// ���1
		EVIDEOLOOP_WIDTH1,
		// ���X2
		EVIDEOLOOP_STARTX2,
		// ���Y2
		EVIDEOLOOP_STARTY2,
		// ���2
		EVIDEOLOOP_WIDTH2,
		// ��ⷽʽ
		EVIDEOLOOP_MODE,
		// ����
		EVIDEOLOOP_DIR,
		// ���ƽǶ�
		EVIDEOLOOP_ANGLE,
		// ����ʱ��
		EVIDEOLOOP_TIME,
		// Ŀ��߶�
		EVIDEOLOOP_TARGET_WIDHT,
		// Ŀ����
		EVIDEOLOOP_TARGET_HEIGTH,
		// ������
		EVIDEOLOOP_SENSITIVE,
		// �����˲�
		EVIDEOLOOP_FILTERON,
		// �뿪�˲�
		EVIDEOLOOP_FILTEROFF,
		EVIDEOLOOP_COUNT
	};
	std::wstring		GetListValue(TIPC_VidLoopCfg* pData, int em);

	std::wstring    DirectionMode(u32 dwDir);
	int GetDirectionModeIndex(u32 dwDir);
	int GetDirectionModeValue(u32 index);
	// �������ƽǶ�ģʽ
	std::wstring		AnalyAngleMode(u32 dwMode);
	int GetAngleModeIndex(u32 dwMode);
	int GetAngleModeValue(u32 index);
	// �����������
	std::wstring		AnalyChkType(u32 dwType);
	int GetMethodIndex(u32 method);
	int GetMethodValue(u32 index, bool bTrapezium);
};

// ��Ȧ�����
namespace coilloop
{
	std::wstring		GetListValue(TIPC_CoilLoopCfg* pData, int em);
};

// �״�����
namespace redarloop
{
	enum EREDARLOOP
	{
		// ���
		EREDARLOOP_SN = 0,
		// ʹ��
		EREDARLOOP_ENABLED,
		// ����ͨ����
		EREDARLOOP_SERIAL,
		// �״�����
		EREDARLOOP_TYPE,
		// ����ģʽ
		//EREDARLOOP_MODE,
		// �״����������
		EREDARLOOP_DISTANCE,
		// �ٶ�����ϵ��
		EREDARLOOP_SPEED,
		// ��Ӧ����
		EREDARLOOP_LANE,

		EREDARLOOP_COUNT
	};
	std::wstring		GetListValue(TIPC_RadarLoopCfg* pData, int em);
};

// ����źż����
namespace RedLampSig
{
	std::wstring		GetListValue(TIPC_RedLampSigLoop* pData, int em);
};

// ʶ������
namespace recogarea
{
	/*ʶ������*/
	typedef struct
	{
		/*���*/
		Uint32  dwLabel;
		/*ʹ��*/
		Bool    bEnable;
		/*����*/
		Int32	X1;
		Int32	Y1;
		Uint32	W1;
		Int32	X2;
		Int32	Y2;
		Uint32	W2;
		/*���ƿ��*/
		Uint32  dwPlateWidth;
		/*���ƽǶ�*/
		Uint32  dwPlateAngle;
		/*�����ӽ�:  1 Ϊץ�ĳ���ͷ����2 Ϊץ�ĳ���β��;*/
		Uint32  dwShootRear;
	}TRecogAreaParam;

	std::wstring		GetListValue(TRecogAreaParam* pData, int em);
};

// �����ǿ
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
	// ��������������������ͼ�ת��
	// ��������
	enum EMLINETYPE
	{
		// ���������
		EMLINETYPE_LANE_LEFT = 0,
		// �����ұ���RIGHT
		EMLINETYPE_LANE_RIGHT,
		// ����12�ָ���
		EMLINETYPE_LANE_12,
		// ����23�ָ���
		EMLINETYPE_LANE_23,
		// ����34�ָ���
		EMLINETYPE_LANE_34,
		// ��ⴥ����
		EMLINETYPE_TRIGGER,
		// ��ת�ж���
		EMLINETYPE_JUDGE_LEFT,
		// ֱ���ж���
		EMLINETYPE_JUDGE_ZX,
		// ��ת�ж���
		EMLINETYPE_JUDGE_RIGHT,
		// ͣ����1
		EMLINETYPE_STOP1,
		// ͣ����2
		EMLINETYPE_STOP2,
		// ͣ����3
		EMLINETYPE_STOP3,
		// ͣ����4
		EMLINETYPE_STOP4,
		// 
		EMLINETYPE_COUNT
	};

	// ��������
	enum EMAREATYPE
	{
		EMAREATYPE_TRACK = EMLINETYPE_COUNT,

		EMAREATYPE_PERSON_WAIT,

		EMAREATYPE_PERSON,

		EMAREATYPE_WARNING,
		// ����1
		EMAREATYPE_AREA1,
		// ����2
		EMAREATYPE_AREA2,
		// ����3
		EMAREATYPE_AREA3,
		// ����4
		EMAREATYPE_AREA4,

		// 
		EMAREATYPE_COUNT
	};

	// �Զ������������
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

	// �Զ�����������
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

	// �Զ���������������
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

// �����߼�������ϢΪ�ַ��������ڴ�����ʾ
#define MAX_LOGICLANE_TYPE_NUM 31
namespace logiclane
{
	enum EMLOGICLANE
	{
		// ���
		EMLOGICLANE_SN = 0,
		// ʹ��
		EMLOGICLANE_ENABLED,
		// �������
		EMLOGICLANE_CHKTYPE,
		// ������
		EMLOGICLANE_SENSITIVITY,
		// ʶ������
		EMLOGICLANE_IDENTID,
		// ����ID
		EMLOGICLANE_LANEID,
		// ���첹��
		EMLOGICLANE_FILLLIGHT_DAY,
		// ҹ�䲹��
		EMLOGICLANE_FILLLIGHT_NIGHT,
		// �����ع�
		EMLOGICLANE_EXPOSURE_PEOPLE,
		// Υ�´���
		EMLOGICLANE_ILLEGALCODE,
		// ��ϸ
		EMLOGICLANE_DETAILS,
		// 
		EMLOGICLANE_COUNT
	};
	wstring		GetListValue(TIPC_LogicLane* pData, int em);

	// �����ƹ�ģʽ
	wstring		AnalyLightMode(u32 dwMode);
	int			AnalyLightMode2Index(u32 dwMode);
	u32			AnalyIndex2LightMode(int nIndex);
	// �����������
	wstring		AnalyChkType(u32 dwType);
	int			AnalyCheckType2Index(u32 dwMode);
	int			AnalyCheckExtType2Index(u32 dwMode);
	u32			AnalyIndex2CheckType(int nIndex);
	u32			AnalyIndex2CheckExtType(int nIndex);

	enum EMLOGICLANEDETAILS
	{
		// �����1
		EMLOGICLANEDETAILS_EDT_DECTOR1 = 0,
		// �����2
		EMLOGICLANEDETAILS_EDT_DECTOR2,
		// �����3
		EMLOGICLANEDETAILS_EDT_DECTOR3,
		// �����4
		EMLOGICLANEDETAILS_EDT_DECTOR4,
		// ���������1
		EMLOGICLANEDETAILS_CHK_DECTOR1,
		// ���������2
		EMLOGICLANEDETAILS_CHK_DECTOR2,
		// ���������3
		EMLOGICLANEDETAILS_CHK_DECTOR3,
		// ���������4
		EMLOGICLANEDETAILS_CHK_DECTOR4,
		// ͼƬ2���첹�ⷽʽ
		EMLOGICLANEDETAILS_CMB_PIC2DAY,
		// ͼƬ2ҹ���ⷽʽ
		EMLOGICLANEDETAILS_CMB_PIC2NIGHT,
		// ͼƬ3���첹�ⷽʽ
		EMLOGICLANEDETAILS_CMB_PIC3DAY,
		// ͼƬ3ҹ���ⷽʽ
		EMLOGICLANEDETAILS_CMB_PIC3NIGHT,
		// ͼƬ4���첹�ⷽʽ
		EMLOGICLANEDETAILS_CMB_PIC4DAY,
		// ͼƬ4ҹ���ⷽʽ
		EMLOGICLANEDETAILS_CMB_PIC4NIGHT,
		// ץ������
		EMLOGICLANEDETAILS_CMB_SNAPNUM,
		// ����ץ������
		EMLOGICLANEDETAILS_EDT_SNAPNUM2,
		// ��С����ץ�ļ��ʱ��
		EMLOGICLANEDETAILS_EDT_MINSNAPTIME,
		// ץ�ļ������
		EMLOGICLANEDETAILS_CMB_SNAPCTRL,
		// 1|2���ʱ��(MS)
		EMLOGICLANEDETAILS_EDT_SPEEDTIME12,
		// 2|3���ʱ��(MS):
		EMLOGICLANEDETAILS_EDT_SPEEDTIME23,
		// 1|2�������(MS)
		EMLOGICLANEDETAILS_EDT_SPEEDRANG12,
		// 2|3�������(MS):
		EMLOGICLANEDETAILS_EDT_SPEEDRANG23,
		// ץ�ĳ�����ͼ:
		EMLOGICLANEDETAILS_CHK_CHILD_PLATE,
		// ��
		EMLOGICLANEDETAILS_EDT_CHILDPLATE_WIDHT,
		// ��
		EMLOGICLANEDETAILS_EDT_CHILDPLATE_HEIGHT,
		// ץ�ĳ�����ͼ:
		EMLOGICLANEDETAILS_CHK_CHILD_VEHICLE,
		// ��
		EMLOGICLANEDETAILS_EDT_CHILDVEHICLE_WIDHT,
		// ��
		EMLOGICLANEDETAILS_EDT_CHILDVEHICLE_HEIGHT,
		// ץ��������ͼ:
		EMLOGICLANEDETAILS_CHK_CHILD_POEPLE,
		// ��
		EMLOGICLANEDETAILS_EDT_CHILDPOEPLE_WIDHT,
		// ��
		EMLOGICLANEDETAILS_EDT_CHILDPOEPLE_HEIGHT,

		EMLOGICLANEDETAILS_EDT_CHILDPOEPLE_WIDHT_2,//��ͼ
		// ��
		EMLOGICLANEDETAILS_EDT_CHILDPOEPLE_HEIGHT_2,
		// ����������ͼ
		EMLOGICLANEDETAILS_CHK_ADD_FACE,
		// ����У��
		EMLOGICLANEDETAILS_CHK_SPEED_CHECK,
		// С������(MS/H)
		EMLOGICLANEDETAILS_EDT_RATELIMIT_LITTLE,
		// ������(MS/H)
		EMLOGICLANEDETAILS_EDT_RATELIMIT_BIG,
		// С�������ٶ���(MS/H)
		EMLOGICLANEDETAILS_EDT_SNAPLIMIT_LITTLE,
		// �������ٶ���
		EMLOGICLANEDETAILS_EDT_SNAPLIMIT_BIG,
		// �ٶ�ƫ���޷�
		EMLOGICLANEDETAILS_EDT_RATEOFFSET,
		// ���������λ
		EMLOGICLANEDETAILS_CHKREDLIGHT_LEFT,
		// ���������λ
		EMLOGICLANEDETAILS_CHKREDLIGHT_LINE,
		// ���������λ
		EMLOGICLANEDETAILS_CHKREDLIGHT_RIGHT,
		// ��С���ʱ��
		EMLOGICLANEDETAILS_EDT_MINREDLIGHTTIME,
		// �����ʱ��
		EMLOGICLANEDETAILS_EDT_MAXREDLIGHTTIME,
		// �������Ŷ�
		EMLOGICLANEDETAILS_EDT_GZZXD,
		// ƥ���ַ���
		EMLOGICLANEDETAILS_CMB_ZFPP,
		// ���ƹ���
		EMLOGICLANEDETAILS_CHK_HPGL_YELLOW,
		EMLOGICLANEDETAILS_CHK_HPGL_WHITE,
		EMLOGICLANEDETAILS_CHK_HPGL_BLUE,
		EMLOGICLANEDETAILS_CHK_HPGL_BLACK,
		EMLOGICLANEDETAILS_CHK_HPGL_GREEN,
		EMLOGICLANEDETAILS_CHK_HPGL_YELLOW_GREEN,
		EMLOGICLANEDETAILS_CHK_HPGL_NO,
		// ץ�ĳ���
		EMLOGICLANEDETAILS_CHK_VEH_0,
		EMLOGICLANEDETAILS_CHK_VEH_1,
		EMLOGICLANEDETAILS_CHK_VEH_2,
		EMLOGICLANEDETAILS_CHK_VEH_3,
		EMLOGICLANEDETAILS_CHK_VEH_4,
		EMLOGICLANEDETAILS_CHK_VEH_5,
		EMLOGICLANEDETAILS_CHK_VEH_6,
		EMLOGICLANEDETAILS_CHK_VEH_7,
		EMLOGICLANEDETAILS_CHK_VEH_8,
		// �������
		EMLOGICLANEDETAILS_CMB_FILTERDIR,

		// ��Ƽ���
		EMLOGICLANEDETAILS_CMB_JX,
		// 
		EMLOGICLANEDETAILS_CHK_MERGE_LEFT_TOP, //ͷ����ӷ���  ����
		EMLOGICLANEDETAILS_CHK_MERGE_LEFT_BOM,	//����
		EMLOGICLANEDETAILS_CHK_MERGE_RIGHT_TOP, //����
		EMLOGICLANEDETAILS_CHK_MERGE_RIGHT_BOM,//����

		EMLOGICLANEDETAILS_CHK_MERGE_SELECT_DEST,//��ѡĿ��

		EMLOGICLANEDETAILS_COUNT
	};

	bool		SetEdtValue(void* pDataIn, QLineEdit* pEdt, int em);
	bool		GetEdtValue(void* pDataOut, QLineEdit* pEdt, int em);

	bool		SetChkValue(void* pDataIn, QCheckBox* pEdt, int em);
	bool		GetChkValue(void* pDataOut, QCheckBox* pEdt, int em);

	bool		SetCmbValue(void* pDataIn, QComboBox* pEdt, int em);
	bool		GetCmbValue(void* pDataOut, QComboBox* pEdt, int em);

	// ����ץ�ļ������
	int			AnalySnapCtrlMode2Index(u32 dwMode);
	u32			AnalyIndex2SnapCtrlMode(int nIndex);
	// ���������λģʽ
	int			AnalyRedLightPhase2Index(u32 dwMode);
	u32			AnalyIndex2RedLightPhase(int nIndex);





};
