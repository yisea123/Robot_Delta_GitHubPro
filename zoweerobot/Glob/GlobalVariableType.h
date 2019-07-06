﻿#ifndef GCODEEXECUTECOMONTYPE_H
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

//输出IO
#define CTL_IO_SOLENOID     1 //继电器
#define CTL_IO_PWM          2 //PWM开关
#define CTL_IO_AIRBLOW      3 //吹气1
#define CTL_IO_ZAXIS_EMS    4 //急停（调高器）
#define CTL_IO_ZAXIS_MIDDLE 5 //回中信号（调高器）
#define CTL_IO_ZAXIS_UP     6 //快速上抬（调高器）
#define CTL_IO_ZAXIS_FOLLOW 7 //切割跟踪（调高器）

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

//#define CTL_IO_SP_LASER     101//打开激光/光闸（在控制帧设置）

//输入IO
#define CTL_IO_IN_ALARM       1 //报警（调高器）
#define CTL_IO_IN_ALARM_ERR   2 //报警出错（激光器）
#define CTL_IO_IN_ISREADY     3 //准备好（激光器）
#define CTL_IO_IN_PUNCH_DONE  4 //穿孔到位（调高器）
#define CTL_IO_IN_STOP_DONE   5 //停靠到位（调高器）
#define CTL_IO_IN_CUT_DONE    7 //切割到位（调高器）

#define CTL_IO_IN_CUT_DONE_OFF true
#define CTL_IO_IN_CUT_DONE_ON  false
#define TEACHPIONTNUM 1024
#define TEACHPIONTSIZE 56//(sizeof(TeachPoint))//40
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
    Point(double x,double y,double z,double w,double p,double r)
    {
        this->x = x;
        this->y = y;
	 this->z = z;
	 this->w = w;
	 this->p = p;
        this->r = r;
    }

    Point operator + (const Point &point)
    {
        return Point((this->x + point.x),(this->y + point.y),(this->z + point.z),(this->w + point.w),(this->p + point.p),(this->r + point.r));
    }

    Point operator - (const Point &point)
    {
        return Point((this->x - point.x),(this->y - point.y),(this->z - point.z),(this->w - point.w),(this->p - point.p),(this->r - point.r));
    }

    Point operator * (double a)
    {
        return Point((this->x * a),(this->y * a),(this->z * a),(this->w * a),(this->p * a),(this->r * a));
    }

    double  x;
    double  y;
    double  z;
    double  w;
    double  p;
    double  r;
};

/** @brief �������̵���ʱ�����洢�� */
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

    bool  m_isSelectTool;     /**< G�����Ƿ����õ��� */
    int   m_ToolLastSerialNum;
    Point m_LastPosition;
    double m_dLastToolAngle;         /**< ���߽Ƕ� */
    bool  m_isSelectCoordinateReference;    /**< G�����Ƿ���������ο� */

    Point m_WorkCoordinate;      /**< ��ǰ��������ϵ */
    Point m_MachineCoordinate;      /**< ��ǰ��������ϵ */
    bool  m_bIsAbsCoordinate;    /**< �Ƿ���þ�������ϵ */

    int   m_gVariableP[200];    /**< ȫ�ֱ��� P100-P299�Ļ��� */

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
        m_ReferenceCoordinate = Point(0, 0,0,0,0,0);
        m_nWorkCoordinateP = -1;
        m_nWorkCoordinateK = -1;
        m_bIsAbsCoordinate = ABSOLUTE_COORDINATE_REFERENCE;

        m_WorkCoordinate = Point(0, 0,0,0,0,0);
        m_MachineCoordinate = Point(0, 0,0,0,0,0);
        //m_UserCoordinate = Point(0, 0);
    }

    bool  m_bIsToolDeclaratic;   /**< �����Ƿ����� */
    int   m_nToolNum;            /**< ��ǰ���߱�� */
    double m_dToolAngle;         /**< ���߽Ƕ� */

    Point m_LastPosition;
    double m_dModulesAngle;
    bool  m_isSelectCoordinateReference;    /**< G�����Ƿ���������ο� */
    Point m_ReferenceCoordinate;      /**< ��ǰ��������ϵ */
    Point m_WorkCoordinate;      /**< ��ǰ��������ϵ */
    Point m_MachineCoordinate;
    //Point m_UserCoordinate;
    QVector<Point>  m_WorkCoordinateList;  /**< G98��������ϵ */
    int   m_nWorkCoordinateP;
    int   m_nWorkCoordinateK;
    bool  m_bIsAbsCoordinate;    /**< �Ƿ���þ�������ϵ */
    QVector<MacroGCode *>  m_MacroGCode;

    GCodeMoveType   m_MoveType;

    int   m_gVariableP[200];    /**< ȫ�ֱ��� P100-P299�Ļ��� */

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
    DAxis(double x,double y, double z, double w, double p, double r)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
        this->p = p;
        this->r = r;
    }

    DAxis operator + (const DAxis &point)
    {
        return DAxis((this->x + point.x),(this->y + point.y),(this->z + point.z),(this->w + point.w),(this->p + point.p),(this->r + point.r));
    }

    DAxis operator - (const DAxis &point)
    {
        return DAxis((this->x - point.x),(this->y - point.y),(this->z - point.z),(this->w - point.w),(this->p - point.p),(this->r - point.r));
    }

    DAxis operator * (double a)
    {
        return DAxis((this->x * a),(this->y * a),(this->z * a),(this->w * a),(this->p * a),(this->r * a));
    }

    double  x;
    double  y;
    double  z;
    double  w;
    double  p;
    double  r;
};


class NAxis
{
public:
    NAxis()
    {}
    NAxis(int x,int y, int z, int w, int p, int r)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
        this->p = p;
        this->r = r;
    }

    NAxis operator + (const NAxis &point)
    {
        return NAxis((this->x + point.x),(this->y + point.y),(this->z + point.z),(this->w + point.w),(this->p + point.p),(this->r + point.r));
    }		

    NAxis operator - (const NAxis &point)
    {
        return NAxis((this->x - point.x),(this->y - point.y),(this->z - point.z),(this->w - point.w),(this->p - point.p),(this->r - point.r));
    }

    NAxis operator * (int a)
    {
        return NAxis((this->x * a),(this->y * a),(this->z * a),(this->w * a),(this->p * a),(this->r * a));
    }

    int  x;
    int  y;
    int  z;
    int  w;
    int  p;
    int  r;
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

    /** @brief ��е���� */
    double m_dMachineSizeX;    /**< �����ߴ�X����λmm */
    double m_dMachineSizeY;    /**< �����ߴ�Y����λmm */
    double m_dGripper1OffsetX;    /**< ��ǯƫ��X����λmm */
    double m_dGripper1OffsetY;    /**< ��ǯƫ��Y����λmm */
    //double m_dGripper2OffsetX;    /**< ��ǯ2ƫ��X����λmm */
    //double m_dGripper2OffsetY;    /**< ��ǯ2ƫ��Y����λmm */
    Point  m_unloadPosition;    /**< ж��λ�ã���λmm */
    double m_dReverseGapT1;    /**< T1 �����϶����λmm */
    double m_dFrontGapT1;    /**< T1 �����϶����λmm */
    double m_dAngleOffsetT1;    /**< T1 �Ƕ�ƫ�ã���λmm */
    double m_dReverseGapT16;    /**< T16 �����϶����λmm */
    double m_dFrontGapT16;    /**< T16 �����϶����λmm */
    double m_dAngleOffsetT16;    /**< T16 �Ƕ�ƫ�ã���λmm */
    //Axis   m_axisReverseGap;    /**< �˶��ᷴ���϶ */

    /** @brief �ӹ����� */
    double m_dPartSizeX;    /**< ����ߴ�X����λmm */
    double m_dPartSizeY;    /**< ����ߴ�Y����λmm */
    double m_dPartThickness;    /**< �����ȣ���λmm */
    //DAxis   m_axisSoftwarePositiveLimit;    /**< ����λ+����λmm */
    //DAxis   m_axisSoftwareNegativeLimit;    /**< ����λ-����λmm */
    DAxis   m_machineOriginalPosition;    /**< �����������꣬��λmm */

    /** @brief �������� */
    double m_dScrewleverOilTime;    /**< ˿����ʱ�䣬��λs */
    double m_dScrewleverOilInterval;    /**< ˿���󻬼������λs */
    double m_dConnectingrodOilTime;    /**< ������ʱ�䣬��λs */
    double m_dConnectingrodOilInterval;    /**< �����󻬼������λs */
    int    m_nProcessPartNum;    /**< ��ӹ�������� */
    bool   m_bIsProcessDoneDown;    /**< ������ͣ����־ */
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

    /** @brief �ٶȲ��� */
    double m_dCWMaxAcc;    /**< �����ת���ٶȣ���λmm/s^2 */
    double m_dCCWMaxAcc;    /**< ���ת���ٶȣ���λmm/s^2 */
    double m_dCWMaxVel;    /**< �����ת�ٶȣ���λmm/s */
    double m_dCCWMaxVel;    /**< ���ת�ٶȣ���λmm/s */
    double m_dPTPVel;    /**< �㶯�ٶȣ���λmm/s */
    double m_dToRefPointVel;    /**< �����ο����ٶȣ���λmm/s */
    double m_dToHomeVel;    /**< �����ٶȣ���λmm/s */
    //double m_dAccTime;    /**< ���ٶȼ���ʱ�䣬��λs */

    /** @brief ���嵱������ */
    //double m_dGearRatio;    /**< ���ٱ� */
    //double m_dScrewleverPitch;    /**< ˿���ݾ࣬��λmm */
    int    m_nPulseEquivalent;    /**< ���嵱������λpluse/mm */

    /** @brief ���ٱ� */
    int m_nRatioNumerator;    /**< ���ٱȷ��� */
    int m_nRatioDenominator;    /**< ���ٱȷ�ĸ */

    /** @brief ��λ */
    double   m_axisSoftwarePositiveLimit;    /**< ����λ������λmm */
    double   m_axisSoftwareNegativeLimit;    /**< ����λ������λmm */

    /** @brief �����϶ */
    int   m_axisReverseGap;    /**< �˶��ᷴ���϶����λpluse */
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

    int     m_nFrameNum;    /**< ģ�߼ܱ�� */
    int     m_nNum;    /**< ģ��ID */
    QString m_sType;    /**< ģ������ */
    double  m_dLength;    /**< ģ�߳��ȣ���λmm */
    double  m_dWidth;    /**< ģ�߿�ȣ���λmm */
    double  m_dAngle;    /**< ģ�߳�ʼ�Ƕȣ���λ�� */
    double  m_dDiameter;  /**< ģ��ֱ������λmm */
    bool    m_bIsRotary;    /**< ģ�߿���ת��־����λmm */
    double  m_dPosAngle;    /**< ģ���ڷֶ��̵�λ�ã���λ�� */

    double m_dR;  /**< ģ�߲�������λmm */
    double m_dA;  /**< ģ�߲�������λmm */
    double m_dB;  /**< ģ�߲�������λmm */
    double m_dC;  /**< ģ�߲�������λmm */
    double m_dD;  /**< ģ�߲�������λmm */

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
    QString   m_netIP;    /**< IP��ַ */
    QString   m_subNetMask;    /**< ���� */
    QString   m_GateWay;    /**< ���� */
    int       m_netPort1;    /**< �˿ں�1 */
    int       m_netPort2;    /**< �˿ں�2 */
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
    QString   m_userName;    /**< �û��� */
    QString   m_password;    /**< ���� */
    UserPermission  m_permission;    /**< �û�Ȩ�� */

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

    /** @brief ��ѹ���ղ��� */
    int   m_nPunchSpeed[5];   /**< �ٶ� */
    int   m_nPunchPlus[5]; /**< ���� */
    int   m_nPunchAcc[5];   /**< ���ٶ� */

    /** @brief �����˶����� */
    int   m_nSpeed[AXIS_ALL];   /**< �ٶ� */
    int   m_nAcc[AXIS_ALL];   /**< ���ٶ� */
    int   m_nJ[AXIS_ALL];   /**< �Ӽ��ٶ� */

    /** @brief XY�������������� */
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
	 sys_ctrl.sendinput[1]=2;  // �ٶȰٷֱ�
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
    SpeedParameter  m_speed;    /**< �ٶȲ��� */

    int m_nVersion;    /**< ����汾 */
    int m_nMachineType;    /**< �����ͺ� */

    /** @brief ϵͳ���� */
    MachineParemeter m_machineParm;

    MotionParameter  m_MotionParm;

    /** @brief �˶������ */
    AxisParemeter m_axisParm[MOF];

    /** @brief ģ�߲��� */
    QVector<MouldsParameter> m_mouldsParm;

    /** @brief ���ղ��� */
    CraftParemeter m_craftParm;

    /** @brief ��ǯλ�� */
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
	int TeachSaveCnt; // ����״̬ 0:���ڱ����У� 1-100:�������
	int axisno[16];
	char PidParaGrpName[5][100]; // Pid������ ע��
	float PidParaGrp[5][6*MOF]; // 5�� Pid����.
	int getaxinofinished;
	int getsysparamfinished;
	int getpidparamfinished[MOF];
	QString               m_userfilePath;
	bool m_bIsCollectMotionData; // �ɼ��˶�����
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

    DAxis m_oldPoint;      /**< ǰһ���ߵ��յ����꣬��λ��mm */
    NAxis m_oldPosition;    /**< ǰһ���ߵ��յ����꣬��λ��pulse */
    int   m_nOldToolNum;   /**< ǰһ���ߵĵ��߱�� */

    DAxis m_newPoint;      /**< ��һ���ߵ��յ����꣬��λ��mm */
    NAxis m_newPosition;   /**< ��һ���ߵ��յ����꣬��λ��pulse */
    int   m_nNewToolNum;   /**< ��һ���ߵĵ��߱�� */

    DAxis m_curPoint;      /**< �����������ĵ�ǰ���꣬��λ��mm */
    NAxis m_curPosition;   /**< �����������ĵ�ǰ���꣬��λ��pulse */
    int   m_nCurToolNum;   /**< �����������ĵ�ǰ���߱�� */

    DAxis m_pausePoint;

    int  m_currentRunActionIndex;
    int  m_currentSendActionIndex;

    int  m_nLastPartGCodeNum;
    //int  m_nManualCodeRunNum;

    bool m_bIsManualMotion;
    bool m_bisEmulation;

    int  m_nlastRecordRunCnt;
    int  m_nMCodeCnt;

    int  m_nMotionControllerSts;   /**< ������������ϵͳ״̬ */

    QVector<Point> m_clampPos;
};



#endif // GCODEEXECUTECOMONTYPE_H
