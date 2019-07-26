#ifndef GCODEEXECUTECOMONTYPE_H
#define GCODEEXECUTECOMONTYPE_H

#include <QString>
#include <QVector>
#include <QColor>
#include <QPoint>
#include <QVariant>
class MacroGCode;
class ToolInfo;

#include "math.h"

#include  "GCodeComonType.h"
#include  "ToolInfo.h"
#include "GlobalDefine.h"

#define CoordFormat "%9.3f"

#define TRUE 1
#define FALSE 0
#define  PI    3.1415926535897932384626

#define  AXIS_ALL 5
#define  AXIS_X  0
#define  AXIS_Y  1
#define  AXIS_Z  2

#define  AXIS_W  3
#define  AXIS_P  4
#define  AXIS_R  5
#define  AXIS_M  6

#define  AXIS_T  2
#define  AXIS_C  3
#define  AXIS_V  4

#define  AXIS_A  AXIS_T
#define  AXIS_B  AXIS_C
//#define  AXIS_Z  AXIS_V

#define  AXIS_1  AXIS_X
#define  AXIS_2  AXIS_Y
#define  AXIS_3  AXIS_Z
#define  AXIS_4  AXIS_R
#define  AXIS_5  AXIS_W
#define  AXIS_6  AXIS_P
#define  AXIS_7  AXIS_M
#define CTL_STS_ENABLE       0//(1<<0)
#define CTL_STS_IDLE_LED     1//(1<<1)
#define CTL_STS_MOTION_LED   2//(1<<2)
#define CTL_STS_PRF_LED      3//(1<<3)
#define CTL_STS_NET_LED      4//(1<<4)
#define CTL_STS_MODE         5//(1<<5)
#define CTL_STS_IN_MOTION    7//(1<<7)
#define CTL_STS_PROG_STS     8//(1<<8)
#define CTL_STS_HOLD         9//(1<<9)
#define CTL_STS_PSTOP        10//(1<<10)
#define CTL_STS_ERR_LIM      11//(1<<11)
#define CTL_STS_ERR_COMM     12//(1<<12)
#define CTL_STS_ERR_RUN      13//(1<<13)
#define CTL_STS_ERR_PLAN     15//(1<<15)
#define CTL_STS_ESTOP        16//(1<<16)
#define CTL_STS_STOP_STATE   17//(1<<17)
#define CTL_STS_MOTION_MODE  18//(1<<18)
#define CTL_STS_OTHER_MODE   20//(1<<20)

//è¾“å‡ºIO
#define CTL_IO_SOLENOID     1 //ç»§ç”µå™¨
#define CTL_IO_PWM          2 //PWMå¼€å…³
#define CTL_IO_AIRBLOW      3 //å¹æ°”1
#define CTL_IO_ZAXIS_EMS    4 //æ€¥åœï¼ˆè°ƒé«˜å™¨ï¼‰
#define CTL_IO_ZAXIS_MIDDLE 5 //å›žä¸­ä¿¡å·ï¼ˆè°ƒé«˜å™¨ï¼‰
#define CTL_IO_ZAXIS_UP     6 //å¿«é€Ÿä¸ŠæŠ¬ï¼ˆè°ƒé«˜å™¨ï¼‰
#define CTL_IO_ZAXIS_FOLLOW 7 //åˆ‡å‰²è·Ÿè¸ªï¼ˆè°ƒé«˜å™¨ï¼‰

#define CTL_IO_PWM_ON            true
#define CTL_IO_PWM_OFF           false
#define CTL_IO_AIRBLOW_ON        true
#define CTL_IO_AIRBLOW_OFF       false
#define CTL_IO_ZAXIS_EMS_ON      true
#define CTL_IO_ZAXIS_EMS_OFF     false
#define CTL_IO_ZAXIS_MIDDLE_ON   true
#define CTL_IO_ZAXIS_MIDDLE_OFF  false
#define CTL_IO_ZAXIS_UP_ON       true
#define CTL_IO_ZAXIS_UP_OFF      false
#define CTL_IO_ZAXIS_FOLLOW_ON   false
#define CTL_IO_ZAXIS_FOLLOW_OFF  true

//#define CTL_IO_SP_LASER     101//æ‰“å¼€æ¿€å…‰/å…‰é—¸ï¼ˆåœ¨æŽ§åˆ¶å¸§è®¾ç½®ï¼‰

//è¾“å…¥IO
#define CTL_IO_IN_ALARM       1 //æŠ¥è­¦ï¼ˆè°ƒé«˜å™¨ï¼‰
#define CTL_IO_IN_ALARM_ERR   2 //æŠ¥è­¦å‡ºé”™ï¼ˆæ¿€å…‰å™¨ï¼‰
#define CTL_IO_IN_ISREADY     3 //å‡†å¤‡å¥½ï¼ˆæ¿€å…‰å™¨ï¼‰
#define CTL_IO_IN_PUNCH_DONE  4 //ç©¿å­”åˆ°ä½ï¼ˆè°ƒé«˜å™¨ï¼‰
#define CTL_IO_IN_STOP_DONE   5 //åœé åˆ°ä½ï¼ˆè°ƒé«˜å™¨ï¼‰
#define CTL_IO_IN_CUT_DONE    7 //åˆ‡å‰²åˆ°ä½ï¼ˆè°ƒé«˜å™¨ï¼‰

#define CTL_IO_IN_CUT_DONE_OFF true
#define CTL_IO_IN_CUT_DONE_ON  false
#define TEACHPIONTNUM 1024
#define TEACHPIONTSIZE 64//56//(sizeof(TeachPoint))//40
typedef  struct  _Material
{
    QString  name;

    double  size_x;
    double  size_y;
    double  size_h;
}Material;

typedef  struct  _HeadToolInfo
{
    QString  name;

    int    toolSerialNum;
    int    toolMagazineSerialNum;

    int    paraRecordNum;
    double para[5];


}HeadToolInfo;

typedef  struct  _TeachPoint
{
    double    pos[MOF];
    unsigned short  index;
    unsigned char    toolindex;//0~20
    unsigned char    hand;//0-All 1-left  2-hand
    unsigned char    zone;
    unsigned char    reserved1;
    unsigned char    reserved[4];
}TeachPoint;

typedef  struct  _TeachList
{
    U16  size;
    U16  index[256];
}TeachList;

class Point
{
public:
    Point()
    {}
    Point(double x,double y,double z,double w,double p,double r,double m = 0)
    {
        this->x = x;
        this->y = y;
	 this->z = z;
	 this->w = w;
	 this->p = p;
        this->r = r;
	this->m = m;
    }

    Point operator + (const Point &point)
    {
        return Point((this->x + point.x),(this->y + point.y),(this->z + point.z),(this->w + point.w),(this->p + point.p),(this->r + point.r),(this->m + point.m));
    }

    Point operator - (const Point &point)
    {
        return Point((this->x - point.x),(this->y - point.y),(this->z - point.z),(this->w - point.w),(this->p - point.p),(this->r - point.r),(this->m - point.m));
    }

    Point operator * (double a)
    {
        return Point((this->x * a),(this->y * a),(this->z * a),(this->w * a),(this->p * a),(this->r * a),(this->m * a));
    }

    double  x;
    double  y;
    double  z;
    double  w;
    double  p;
    double  r;
	double  m;
};

/** @brief ½âÎö¹ý³ÌµÄÁÙÊ±²ÎÊý´æ´¢Àà */
class ParseGCodeInterVariableCache
{
public:
    ParseGCodeInterVariableCache()
    {
        m_isSelectTool = FALSE;
        m_isSelectCoordinateReference = TRUE;
        m_ToolLastSerialNum = 0;//-1;
        m_LastPosition.x = 0.0;
        m_LastPosition.y = 0.0;
	 m_LastPosition.z= 0.0;
        m_LastPosition.w = 0.0;
	 m_LastPosition.p= 0.0;
        m_LastPosition.r = 0.0;
	m_LastPosition.m = 0.0;
        m_dLastToolAngle = 250;//0.0;
	 m_hand=0;
        m_WorkCoordinate = Point(0, 0,0,0,0,0);
        m_MachineCoordinate = Point(0, 0 ,0,0,0,0);

        m_bIsAbsCoordinate = true;
        m_parts.name = "";
        m_parts.size_h = 0;
        m_parts.size_x = 0;//800;
        m_parts.size_y = 0;//600;

        m_RawMaterial.name = "";
        m_RawMaterial.size_h = 0;
        m_RawMaterial.size_x = 0;//800;
        m_RawMaterial.size_y = 0;//600;
        memset(&m_userParam[0],0,sizeof(m_userParam));
    }

    bool  m_isSelectTool;     /**< G´úÂëÊÇ·ñÉèÖÃµ¶¾ß */
    int   m_ToolLastSerialNum;
    Point m_LastPosition;
    double m_dLastToolAngle;         /**< µ¶¾ß½Ç¶È */
    bool  m_isSelectCoordinateReference;    /**< G´úÂëÊÇ·ñÉèÖÃ×ø±ê²Î¿¼ */

    Point m_WorkCoordinate;      /**< µ±Ç°¹¤¼þ×ø±êÏµ */
    Point m_MachineCoordinate;      /**< µ±Ç°¹¤¼þ×ø±êÏµ */
    bool  m_bIsAbsCoordinate;    /**< ÊÇ·ñ²ÉÓÃ¾ø¶Ô×ø±êÏµ */

    int   m_gVariableP[200];    /**< È«¾Ö±äÁ¿ P100-P299µÄ»º³å */

    Material  m_parts;
    Material  m_RawMaterial;
    CartesianPose m_userParam[21];
	int m_hand;

    QVector<HeadToolInfo>   m_ToolsInfo;
};

class EmulationInterVariableCache
{
public:
    EmulationInterVariableCache()
    {
        m_bIsToolDeclaratic = FALSE;
        m_isSelectCoordinateReference = TRUE;
        m_MoveType = GCODE_MOVE_NULL;
        m_nToolNum = 0;
        m_dToolAngle = 0;
        m_dModulesAngle = 0;
        m_LastPosition.x = 0.0;
        m_LastPosition.y = 0.0;
	  m_LastPosition.z = 0.0;
        m_LastPosition.w = 0.0;
	  m_LastPosition.p = 0.0;
        m_LastPosition.r = 0.0;
		m_LastPosition.m = 0.0;
        m_ReferenceCoordinate = Point(0, 0,0,0,0,0);
        m_nWorkCoordinateP = -1;
        m_nWorkCoordinateK = -1;
        m_bIsAbsCoordinate = ABSOLUTE_COORDINATE_REFERENCE;

        m_WorkCoordinate = Point(0, 0,0,0,0,0);
        m_MachineCoordinate = Point(0, 0,0,0,0,0);
        //m_UserCoordinate = Point(0, 0);
    }

    bool  m_bIsToolDeclaratic;   /**< µ¶¾ßÊÇ·ñ±»ÉùÃ÷ */
    int   m_nToolNum;            /**< µ±Ç°µ¶¾ß±àºÅ */
    double m_dToolAngle;         /**< µ¶¾ß½Ç¶È */

    Point m_LastPosition;
    double m_dModulesAngle;
    bool  m_isSelectCoordinateReference;    /**< G´úÂëÊÇ·ñÉèÖÃ×ø±ê²Î¿¼ */
    Point m_ReferenceCoordinate;      /**< µ±Ç°¹¤¼þ×ø±êÏµ */
    Point m_WorkCoordinate;      /**< µ±Ç°¹¤¼þ×ø±êÏµ */
    Point m_MachineCoordinate;
    //Point m_UserCoordinate;
    QVector<Point>  m_WorkCoordinateList;  /**< G98¹¤¼þ×ø±êÏµ */
    int   m_nWorkCoordinateP;
    int   m_nWorkCoordinateK;
    bool  m_bIsAbsCoordinate;    /**< ÊÇ·ñ²ÉÓÃ¾ø¶Ô×ø±êÏµ */
    QVector<MacroGCode *>  m_MacroGCode;

    GCodeMoveType   m_MoveType;

    int   m_gVariableP[200];    /**< È«¾Ö±äÁ¿ P100-P299µÄ»º³å */

    Material  m_parts;
    Material  m_RawMaterial;

    QVector<ToolInfo *>   m_ToolsInfo;
	
    CartesianPose m_userParam[21];
};


class  GraphicalInfo
{
public:
    GraphicalInfo()
    {
        isClose = true;
        style = Qt::SolidLine;
        color = Qt::red;
    }

    QVector<Point>   PointLists;
    QVector<QPoint>  CanvsPoints;
    bool             isClose;
    Qt::PenStyle     style;
    QColor           color;
};

class  CarrierGraphicalInfo
{
public:
    CarrierGraphicalInfo()
    {
        textSize = 10;
        textcolor = Qt::white;
        GraphicalInfo* info1 = new GraphicalInfo;
        info1->color = Qt::white;
        info1->style = Qt::SolidLine;
        info1->isClose = true;
        GraphicalInfo* info2 = new GraphicalInfo;
        info2->color = Qt::white;
        info2->style = Qt::SolidLine;
        info2->isClose = true;
        GraphicalInfo* info3 = new GraphicalInfo;
        info3->color = Qt::white;
        info3->style = Qt::SolidLine;
        info3->isClose = true;
        Graphicals.push_back(info1);
        Graphicals.push_back(info2);
        Graphicals.push_back(info3);
    }

    QVector<GraphicalInfo *>   Graphicals;

    QColor           textcolor;
    Point            textPos;
    int              textSize;
    QString          text;

};



class  SpeedParameter
{
public:
    SpeedParameter()
    {
        m_dV = 5.0;
        m_dA = 1000.0;
    }
    double m_dV;
    double m_dA;
};

class DAxis
{
public:
    DAxis()
    {}
    DAxis(double x,double y, double z, double w, double p, double r, double m = 0)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
        this->p = p;
        this->r = r;
	this->m = m;
    }

    DAxis operator + (const DAxis &point)
    {
        return DAxis((this->x + point.x),(this->y + point.y),(this->z + point.z),(this->w + point.w),(this->p + point.p),(this->r + point.r),(this->m + point.m));
    }

    DAxis operator - (const DAxis &point)
    {
        return DAxis((this->x - point.x),(this->y - point.y),(this->z - point.z),(this->w - point.w),(this->p - point.p),(this->r - point.r),(this->m - point.m));
    }

    DAxis operator * (double a)
    {
        return DAxis((this->x * a),(this->y * a),(this->z * a),(this->w * a),(this->p * a),(this->r * a),(this->m * a));
    }

    double  x;
    double  y;
    double  z;
    double  w;
    double  p;
    double  r;
	double  m;
};


class NAxis
{
public:
    NAxis()
    {}
    NAxis(int x,int y, int z, int w, int p, int r, int m = 0)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
        this->p = p;
        this->r = r;
		this->m = m;
    }

    NAxis operator + (const NAxis &point)
    {
        return NAxis((this->x + point.x),(this->y + point.y),(this->z + point.z),(this->w + point.w),(this->p + point.p),(this->r + point.r),(this->m + point.m));
    }		

    NAxis operator - (const NAxis &point)
    {
        return NAxis((this->x - point.x),(this->y - point.y),(this->z - point.z),(this->w - point.w),(this->p - point.p),(this->r - point.r),(this->m - point.m));
    }

    NAxis operator * (int a)
    {
        return NAxis((this->x * a),(this->y * a),(this->z * a),(this->w * a),(this->p * a),(this->r * a),(this->m * a));
    }

    int  x;
    int  y;
    int  z;
    int  w;
    int  p;
    int  r;
	int m;
};

class  MachineParemeter
{
public:
    MachineParemeter()
    {
        m_dMachineSizeX = 1000;
        m_dMachineSizeY = 1000;
        m_dGripper1OffsetX = 0;
        m_dGripper1OffsetY = 100;
        //m_dGripper2OffsetX = 0;
        //m_dGripper2OffsetY = 200;
        m_unloadPosition = Point(0,0,0,0,0,0);
        m_dReverseGapT1 = 0;
        m_dFrontGapT1 = 0;
        m_dAngleOffsetT1 = 0;
        m_dReverseGapT16 = 0;
        m_dFrontGapT16 = 0;
        m_dAngleOffsetT16 = 0;

        m_dPartSizeX = 1000;
        m_dPartSizeY = 1000;
        m_dPartThickness = 1.0;
        //m_axisSoftwarePositiveLimit = DAxis(1100.0, 1100.0, 500.0, 0.0, 0.0);
        //m_axisSoftwareNegativeLimit = DAxis(-100.0, -100.0, -100.0, 0.0, 0.0);
        m_machineOriginalPosition = DAxis(0.0, 0.0, 0.0, 0.0, 0.0 , 0.0);

        m_dScrewleverOilTime = 1;
        m_dScrewleverOilInterval = 5000;
        m_dConnectingrodOilTime = 1;
        m_dConnectingrodOilInterval = 5000;
        m_nProcessPartNum = 1;
        m_bIsProcessDoneDown = false;
    }

    /** @brief »úÐµ²ÎÊý */
    double m_dMachineSizeX;    /**< »ú´²³ß´çX£¬µ¥Î»mm */
    double m_dMachineSizeY;    /**< »ú´²³ß´çY£¬µ¥Î»mm */
    double m_dGripper1OffsetX;    /**< ¼ÐÇ¯Æ«ÖÃX£¬µ¥Î»mm */
    double m_dGripper1OffsetY;    /**< ¼ÐÇ¯Æ«ÖÃY£¬µ¥Î»mm */
    //double m_dGripper2OffsetX;    /**< ¼ÐÇ¯2Æ«ÖÃX£¬µ¥Î»mm */
    //double m_dGripper2OffsetY;    /**< ¼ÐÇ¯2Æ«ÖÃY£¬µ¥Î»mm */
    Point  m_unloadPosition;    /**< Ð¶ÁÏÎ»ÖÃ£¬µ¥Î»mm */
    double m_dReverseGapT1;    /**< T1 ·´Ïò¼äÏ¶£¬µ¥Î»mm */
    double m_dFrontGapT1;    /**< T1 ÕýÏò¼äÏ¶£¬µ¥Î»mm */
    double m_dAngleOffsetT1;    /**< T1 ½Ç¶ÈÆ«ÖÃ£¬µ¥Î»mm */
    double m_dReverseGapT16;    /**< T16 ·´Ïò¼äÏ¶£¬µ¥Î»mm */
    double m_dFrontGapT16;    /**< T16 ÕýÏò¼äÏ¶£¬µ¥Î»mm */
    double m_dAngleOffsetT16;    /**< T16 ½Ç¶ÈÆ«ÖÃ£¬µ¥Î»mm */
    //Axis   m_axisReverseGap;    /**< ÔË¶¯Öá·´Ïò¼äÏ¶ */

    /** @brief ¼Ó¹¤²ÎÊý */
    double m_dPartSizeX;    /**< Áã¼þ³ß´çX£¬µ¥Î»mm */
    double m_dPartSizeY;    /**< Áã¼þ³ß´çY£¬µ¥Î»mm */
    double m_dPartThickness;    /**< Áã¼þºñ¶È£¬µ¥Î»mm */
    //DAxis   m_axisSoftwarePositiveLimit;    /**< ÈíÏÞÎ»+£¬µ¥Î»mm */
    //DAxis   m_axisSoftwareNegativeLimit;    /**< ÈíÏÞÎ»-£¬µ¥Î»mm */
    DAxis   m_machineOriginalPosition;    /**< »úÆ÷»ØÁã×ø±ê£¬µ¥Î»mm */

    /** @brief Éú²ú²ÎÊý */
    double m_dScrewleverOilTime;    /**< Ë¿¸ËÈó»¬Ê±¼ä£¬µ¥Î»s */
    double m_dScrewleverOilInterval;    /**< Ë¿¸ËÈó»¬¼ä¸ô£¬µ¥Î»s */
    double m_dConnectingrodOilTime;    /**< Á¬¸ËÈó»¬Ê±¼ä£¬µ¥Î»s */
    double m_dConnectingrodOilInterval;    /**< Á¬¸ËÈó»¬¼ä¸ô£¬µ¥Î»s */
    int    m_nProcessPartNum;    /**< Äâ¼Ó¹¤Áã¼þÊýÁ¿ */
    bool   m_bIsProcessDoneDown;    /**< Áã¼þÍê³ÉÍ£»ú±êÖ¾ */
};


class MotionParameter
{
public:
    MotionParameter()
    {
        for (int i = 0; i < 3; i++)
        {
            m_dAxisMaxAcc[i] = 6000;
            m_dAxisMaxVel[i] = 2000;
            m_dAxisMaxDec[i] = 10000;
            m_dAxisPLimit[i] = 3000;
            m_dAxisNLimit[i] = -100;
            m_dAxisRadio[i] = 35.232;
        }
        m_dAxisPLimit[0] = 1500;

        m_nDirX = 1;
        m_nDirY = 1;
        m_dAccuracy = 17616;
        m_dAccTime = 400;
        m_dMaxVel = 1500;
        m_dMaxAcc = 6000;
        m_dMaxDec = 10000;

        for (int i = 0; i < 2; i++)
        {
            m_nK[i] = 0;
            m_dKp[i] = 0;
            m_dKi[i] = 0;
            m_dKd[i] = 0;
        }

        m_dpLineMaxLen = 0;
        m_dpLineMaxVel = 0;
        m_dpLineMaxPer = 0;
        m_dpLineMinPer = 0;
        m_dpArcRadMaxLen = 0;
        m_dpArcLenMaxLen = 0;
        m_dpArcMaxVel = 0;
        m_dpArcMaxPer = 0;
        m_dpArcMinPer = 0;
        m_dpArcError = 0;
    }

    double m_dAxisMaxAcc[3];
    double m_dAxisMaxVel[3];
    double m_dAxisMaxDec[3];
    double m_dAxisPLimit[3];
    double m_dAxisNLimit[3];
    double m_dAxisRadio[3];
    int    m_nDirX;
    int    m_nDirY;
    double m_dAccuracy;
    double m_dAccTime;
    double m_dMaxVel;
    double m_dMaxAcc;
    double m_dMaxDec;

    int    m_nK[2];
    double m_dKp[2];
    double m_dKi[2];
    double m_dKd[2];

    double m_dpLineMaxLen;
    double m_dpLineMaxVel;
    double m_dpLineMaxPer;
    double m_dpLineMinPer;
    double m_dpArcRadMaxLen;
    double m_dpArcLenMaxLen;
    double m_dpArcMaxVel;
    double m_dpArcMaxPer;
    double m_dpArcMinPer;
    double m_dpArcError;
};


class  AxisParemeter
{
public:
    AxisParemeter()
    {
        m_dCWMaxAcc = 1000;
        m_dCCWMaxAcc = 1000;
        m_dCWMaxVel = 1000;
        m_dCWMaxAcc = 1000;
        m_dCCWMaxVel = 1000;
        m_dPTPVel = 1000;
        m_dToRefPointVel = 100;
        m_dToHomeVel = 1;
        //m_dAccTime = 10;

        //m_dGearRatio = 100;
        //m_dScrewleverPitch = 2;
        m_nPulseEquivalent = 1000;

        m_nRatioNumerator = 1;
        m_nRatioDenominator = 1;

        m_axisSoftwarePositiveLimit = 1100;
        m_axisSoftwareNegativeLimit = -100;

        m_axisReverseGap = 0;

        //m_nVec = 10;
        //m_nPulse = 100;
        //m_nDir = 1;
    }

    /** @brief ËÙ¶È²ÎÊý */
    double m_dCWMaxAcc;    /**< ×î´óÕý×ª¼ÓËÙ¶È£¬µ¥Î»mm/s^2 */
    double m_dCCWMaxAcc;    /**< ×î´ó·´×ª¼ÓËÙ¶È£¬µ¥Î»mm/s^2 */
    double m_dCWMaxVel;    /**< ×î´óÕý×ªËÙ¶È£¬µ¥Î»mm/s */
    double m_dCCWMaxVel;    /**< ×î´ó·´×ªËÙ¶È£¬µ¥Î»mm/s */
    double m_dPTPVel;    /**< µã¶¯ËÙ¶È£¬µ¥Î»mm/s */
    double m_dToRefPointVel;    /**< ËÑË÷²Î¿¼µãËÙ¶È£¬µ¥Î»mm/s */
    double m_dToHomeVel;    /**< »ØÁãËÙ¶È£¬µ¥Î»mm/s */
    //double m_dAccTime;    /**< ¼ÓËÙ¶È¼ÓËÙÊ±¼ä£¬µ¥Î»s */

    /** @brief Âö³åµ±Á¿²ÎÊý */
    //double m_dGearRatio;    /**< ¼õËÙ±È */
    //double m_dScrewleverPitch;    /**< Ë¿¸ËÂÝ¾à£¬µ¥Î»mm */
    int    m_nPulseEquivalent;    /**< Âö³åµ±Á¿£¬µ¥Î»pluse/mm */

    /** @brief ¼õËÙ±È */
    int m_nRatioNumerator;    /**< ¼õËÙ±È·Ö×Ó */
    int m_nRatioDenominator;    /**< ¼õËÙ±È·ÖÄ¸ */

    /** @brief ÏÞÎ» */
    double   m_axisSoftwarePositiveLimit;    /**< ÈíÏÞÎ»Õý£¬µ¥Î»mm */
    double   m_axisSoftwareNegativeLimit;    /**< ÈíÏÞÎ»¸º£¬µ¥Î»mm */

    /** @brief ·´Ïò¼äÏ¶ */
    int   m_axisReverseGap;    /**< ÔË¶¯Öá·´Ïò¼äÏ¶£¬µ¥Î»pluse */
};

class  MouldsParameter
{
public:
    MouldsParameter()
    {
        m_nFrameNum = 0;
        m_nNum = 0;
        m_sType = "";
        m_dLength = 0;
        m_dWidth = 0;
        m_dDiameter = 0;
        m_dAngle = 0;
        m_bIsRotary = false;
        m_dPosAngle = 0;

        m_sDimensions = "";
        m_sTypeName = "";

        m_dR = 0;
        m_dA = 0;
        m_dB = 0;
        m_dC = 0;
        m_dD = 0;
    }

    int     m_nFrameNum;    /**< Ä£¾ß¼Ü±àºÅ */
    int     m_nNum;    /**< Ä£¾ßID */
    QString m_sType;    /**< Ä£¾ßÀàÐÍ */
    double  m_dLength;    /**< Ä£¾ß³¤¶È£¬µ¥Î»mm */
    double  m_dWidth;    /**< Ä£¾ß¿í¶È£¬µ¥Î»mm */
    double  m_dAngle;    /**< Ä£¾ß³õÊ¼½Ç¶È£¬µ¥Î»¡ã */
    double  m_dDiameter;  /**< Ä£¾ßÖ±¾¶£¬µ¥Î»mm */
    bool    m_bIsRotary;    /**< Ä£¾ß¿ÉÐý×ª±êÖ¾£¬µ¥Î»mm */
    double  m_dPosAngle;    /**< Ä£¾ßÔÚ·Ö¶ÈÅÌµÄÎ»ÖÃ£¬µ¥Î»¡ã */

    double m_dR;  /**< Ä£¾ß²ÎÊý£¬µ¥Î»mm */
    double m_dA;  /**< Ä£¾ß²ÎÊý£¬µ¥Î»mm */
    double m_dB;  /**< Ä£¾ß²ÎÊý£¬µ¥Î»mm */
    double m_dC;  /**< Ä£¾ß²ÎÊý£¬µ¥Î»mm */
    double m_dD;  /**< Ä£¾ß²ÎÊý£¬µ¥Î»mm */

    QString m_sDimensions;
    QString m_sTypeName;
};

class   SerialCommunicationParameter
{
public:
    SerialCommunicationParameter()
    {
        m_portSerialNum = "COM1";
        m_baud = 9600;
        m_dataBits = 8;
        m_stopBits = 1;
        m_ParityCheck = "NONE";
    }
    QString   m_portSerialNum;
    int       m_baud;
    int       m_dataBits;
    int       m_stopBits;
    QString   m_ParityCheck;
};

class   NetCommunicationParameter
{
public:
    NetCommunicationParameter()
    {
        m_netIP = "192.168.1.41";
        m_subNetMask = "255.255.255.0";
        m_GateWay = "192.168.1.1";
        m_netPort1 = 7580;
        m_netPort2 = 7575;
    }
    QString   m_netIP;    /**< IPµØÖ· */
    QString   m_subNetMask;    /**< ÑÚÂë */
    QString   m_GateWay;    /**< Íø¹Ø */
    int       m_netPort1;    /**< ¶Ë¿ÚºÅ1 */
    int       m_netPort2;    /**< ¶Ë¿ÚºÅ2 */
};

typedef enum _UsersPermission
{
    ORDINARY_USER,
    ADVANCED_USER,
    SUPER_USER
}UserPermission;

class   UserParameter
{
public:
    UserParameter()
    {
        m_userName = "";
        m_password = "";
        m_permission = ORDINARY_USER;
    }
    QString   m_userName;    /**< ÓÃ»§Ãû */
    QString   m_password;    /**< ÃÜÂë */
    UserPermission  m_permission;    /**< ÓÃ»§È¨ÏÞ */

//    UserParameter& operator=(UserParameter& u)
//    {
//        m_userName = u.m_userName;
//        m_password = u.m_password;
//        m_permission = u.m_permission;
//        return  this;
//    }
};

class  CraftParemeter
{
public:
    CraftParemeter()
    {
        for (int i = 0; i < 5; i++)
        {
            m_nPunchSpeed[i] = 100;
            m_nPunchPlus[i] = 10000;
            m_nPunchAcc[i] = 1000;
        }

        for (int i = 0; i < AXIS_ALL; i++)
        {
            m_nSpeed[i] = 100;
            m_nAcc[i] = 1000;
            m_nJ[i] = 10000;
        }

        for (int i = 0; i < 2; i++)
        {
            m_nLimitT0[i] = 0;
            m_nLimitS1[i] = 0;
            m_nLimitS2[i] = 0;
            m_nLimitS3[i] = 0;
            m_nLimitT1[i] = 0;
            m_nLimitT2[i] = 0;
            m_nLimitP1[i] = 0;
            m_nLimitP2[i] = 0;
        }

        for (int i = 2; i < 4; i++)
        {
            m_nLimitT0[i] = 0;
            m_nLimitS1[i] = 0;
            m_nLimitS2[i] = 0;
            m_nLimitS3[i] = 0;
        }
    }

    /** @brief ³åÑ¹¹¤ÒÕ²ÎÊý */
    int   m_nPunchSpeed[5];   /**< ËÙ¶È */
    int   m_nPunchPlus[5]; /**< Âö³å */
    int   m_nPunchAcc[5];   /**< ¼ÓËÙ¶È */

    /** @brief ¸÷ÖáÔË¶¯²ÎÊý */
    int   m_nSpeed[AXIS_ALL];   /**< ËÙ¶È */
    int   m_nAcc[AXIS_ALL];   /**< ¼ÓËÙ¶È */
    int   m_nJ[AXIS_ALL];   /**< ¼Ó¼ÓËÙ¶È */

    /** @brief XYÖáÏÞÖÆÌõ¼þ²ÎÊý */
    double   m_nLimitT0[4];   /**< T0 */
    double   m_nLimitS1[4];   /**< S1 */
    double   m_nLimitS2[4];   /**< S2 */
    double   m_nLimitS3[4];   /**< S3 */
    int m_nLimitT1[2];
    int m_nLimitT2[2];
    int m_nLimitP1[2];
    int m_nLimitP2[2];

};

class  SystemCtrl
{
public:
       int ctrlmode;
	int servoenable;

};


class  SystemParameter
{
public:
    SystemParameter()
    {
        m_nVersion = 1;
        m_nMachineType = 0;
//        MouldsParameter p;
//        m_mouldsParm.push_back(p);
        m_MachineCoordinate = Point(0, 0,0,0,0,0);
        m_clampPosition.append(Point(0, 0,0,0,0,0));

        memset(SystemParam,0,sizeof(SystemParam));
        memset(axisno,0,sizeof(axisno));
	 memset(&sys_ctrl,0,sizeof(sys_ctrl));        
	 sys_ctrl.sendinput[1]=2;  // ËÙ¶È°Ù·Ö±È
	 memset(m_current,0,sizeof(m_current));
	 m_currentTool=0;
	 teachreadfinished=0;
	 TeachSaveCnt = 0;
	 getaxinofinished=0;
	 getsysparamfinished=0;
	 memset(teachpoint,0,sizeof(teachpoint));
	 memset(teachpointtext,0,sizeof(teachpointtext));
	 memset(coor_car_pos,0,sizeof(coor_car_pos));
	 memset(coor_joint_pos,0,sizeof(coor_joint_pos));

	 memset(m_cVersion,0,sizeof(m_cVersion));
	 memset(m_battery,0,sizeof(m_battery));

	 memset(ToolCalResult,0,sizeof(ToolCalResult));
	 memset(zeropos,0,sizeof(zeropos));
	 memset(&Robot_Link,0,sizeof(Robot_Link));
       memset(m_userParam,0,sizeof(m_userParam));
       m_bIsCollectMotionData = false;
    }

    float SystemParam[256];
    
    FrameTrafficCtrl sys_ctrl;
    SystemCtrl m_sysctrl;
    SpeedParameter  m_speed;    /**< ËÙ¶È²ÎÊý */

    int m_nVersion;    /**< Èí¼þ°æ±¾ */
    int m_nMachineType;    /**< »úÆ÷ÐÍºÅ */

    /** @brief ÏµÍ³²ÎÊý */
    MachineParemeter m_machineParm;

    MotionParameter  m_MotionParm;

    /** @brief ÔË¶¯Öá²ÎÊý */
    AxisParemeter m_axisParm[MOF];

    /** @brief Ä£¾ß²ÎÊý */
    QVector<MouldsParameter> m_mouldsParm;

    /** @brief ¹¤ÒÕ²ÎÊý */
    CraftParemeter m_craftParm;

    /** @brief ¼ÐÇ¯Î»ÖÃ */
    QVector<Point> m_clampPosition;

    SerialCommunicationParameter  m_serialPLCParm;
    SerialCommunicationParameter  m_serialKeyParm;
    NetCommunicationParameter     m_netConnectionParm;
    int sysvel;//
    float coor_car_pos[8];
     float coor_joint_pos[8];
	 uint64_t realaxispos[8];
	 unsigned int realaxis[16];
	 unsigned int plusepos[16];
	 unsigned int zeropos[2*MOF];
	 LinkDimension Robot_Link;
	 CartesianPose ToolCalResult[21];
	 CartesianPose m_userParam[21];
	 char m_cVersion[13];
	 char m_battery[8];
	 char m_setbattery[8];
	 unsigned int m_current[MOF];
	 int m_currentTool;
     TeachPoint teachpoint[TEACHPIONTNUM];
	char teachpointtext[TEACHPIONTNUM][128];
	int teachreadfinished;
	int TeachSaveCnt; // ±£´æ×´Ì¬ 0:²»ÔÚ±£´æÖÐ£¬ 1-100:±£´æ½ø¶È
	int axisno[17];
	char PidParaGrpName[5][100]; // Pid²ÎÊý×é ×¢ÊÍ
	float PidParaGrp[5][6*MOF]; // 5×é Pid²ÎÊý.
	int getaxinofinished;
	int getsysparamfinished;
	int getpidparamfinished[MOF];
	QString               m_userfilePath;
	bool m_bIsCollectMotionData; // ²É¼¯ÔË¶¯Êý¾Ý
    void setUserParm(UserParameter u)
    {
        m_User = u;
	 
    }
    
    void setMachineCoordinate(Point point)
    {
        m_MachineCoordinate = point;
    }

    Point getMachineCoordinate()
    {
//        m_MachineCoordinate.x = m_machineParm.m_machineOriginalPosition.x;
//        m_MachineCoordinate.y = m_machineParm.m_machineOriginalPosition.y;
        return m_MachineCoordinate;
    }

    UserPermission getUserPermission()
    {
        return m_User.m_permission;
    }

    UserParameter getUserInfo()
    {
        return m_User;
    }

    int getMouldsFrameNum(double angle)
    {
//        for (int i = 0; i < m_mouldsParm.size(); i++)
//        {
//            if (fabs(angle - m_mouldsParm.at(i).m_dPosAngle) < 0.01)
//            {
//                return i;
//            }
//        }
//        return 0;

        while (angle >= 360)
        {
            angle -= 360;
        }
        while (angle < 0)
        {
            angle += 360;
        }

        for (int i = 0; i < m_mouldsParm.size(); i++)
        {
            if (fabs(angle - m_mouldsParm.at(i).m_dPosAngle) < 0.05)
            {
                return i;
            }
            else if (angle < m_mouldsParm.at(i).m_dPosAngle)
            {
                return i-1;
            }
        }
        return 0;
    }

    double getMouldsAngle(int frameNum)
    {
        if (frameNum >= 0)
        {
            if (frameNum < m_mouldsParm.size())
            {
                return m_mouldsParm.at(frameNum).m_dPosAngle;
            }
            else if (frameNum == m_mouldsParm.size())
            {
                return m_mouldsParm.at(0).m_dPosAngle;
            }
        }
        else
        {
            if (frameNum == -1)
            {
                return m_mouldsParm.at(m_mouldsParm.size() - 1).m_dPosAngle;
            }
            else
                return 0;
        }
    }

private:
    UserParameter   m_User;
    Point m_MachineCoordinate;
    //Point m_WorkCoordinate;
};

class ProcessInterVariableCache
{
public:
    ProcessInterVariableCache()
    {
        initProcessCache();
    }

    void initProcessCache()
    {
        m_oldPoint = DAxis(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
        m_oldPosition = NAxis(0, 0, 0, 0, 0, 0);
        m_nOldToolNum = 0;

        m_newPoint = DAxis(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
        m_newPosition = NAxis(0, 0, 0, 0, 0, 0);
        m_nNewToolNum = 0;

        m_curPoint = DAxis(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
        m_curPosition = NAxis(0, 0, 0, 0, 0, 0);
        m_nCurToolNum = 0;

        m_pausePoint = DAxis(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);

        m_nLastPartGCodeNum = 0;
        //m_nManualCodeRunNum = 0;

        m_bIsManualMotion = true;//false;
        m_bisEmulation = false;

        m_nlastRecordRunCnt = 0;
        m_nMCodeCnt = 0;

        m_clampPos.append(Point(0, 0,0,0,0,0));

        m_nMotionControllerSts= 0;
    }

    DAxis m_oldPoint;      /**< Ç°Ò»¶ÎÏßµÄÖÕµã×ø±ê£¬µ¥Î»£ºmm */
    NAxis m_oldPosition;    /**< Ç°Ò»¶ÎÏßµÄÖÕµã×ø±ê£¬µ¥Î»£ºpulse */
    int   m_nOldToolNum;   /**< Ç°Ò»¶ÎÏßµÄµ¶¾ß±àºÅ */

    DAxis m_newPoint;      /**< ÏÖÒ»¶ÎÏßµÄÖÕµã×ø±ê£¬µ¥Î»£ºmm */
    NAxis m_newPosition;   /**< ÏÖÒ»¶ÎÏßµÄÖÕµã×ø±ê£¬µ¥Î»£ºpulse */
    int   m_nNewToolNum;   /**< ÏÖÒ»¶ÎÏßµÄµ¶¾ß±àºÅ */

    DAxis m_curPoint;      /**< ¿ØÖÆÆ÷·´À¡µÄµ±Ç°×ø±ê£¬µ¥Î»£ºmm */
    NAxis m_curPosition;   /**< ¿ØÖÆÆ÷·´À¡µÄµ±Ç°×ø±ê£¬µ¥Î»£ºpulse */
    int   m_nCurToolNum;   /**< ¿ØÖÆÆ÷·´À¡µÄµ±Ç°µ¶¾ß±àºÅ */

    DAxis m_pausePoint;

    int  m_currentRunActionIndex;
    int  m_currentSendActionIndex;

    int  m_nLastPartGCodeNum;
    //int  m_nManualCodeRunNum;

    bool m_bIsManualMotion;
    bool m_bisEmulation;

    int  m_nlastRecordRunCnt;
    int  m_nMCodeCnt;

    int  m_nMotionControllerSts;   /**< ¿ØÖÆÆ÷·´À¡µÄÏµÍ³×´Ì¬ */

    QVector<Point> m_clampPos;
};



#endif // GCODEEXECUTECOMONTYPE_H
