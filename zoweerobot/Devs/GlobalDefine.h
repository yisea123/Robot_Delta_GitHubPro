#ifndef __GLOBALSTRUCT_H__
#define __GLOBALSTRUCT_H__
#include "error.h"
//#include <string>
//using std::string;

#ifndef U8
#define U8 unsigned char
#endif

#ifndef U16
#define U16 unsigned short
#endif

#ifndef U32
#define U32 unsigned int
#endif
//#define (unsigned short) U16
//#define unsigned int U32
//#define PROGRAM_USELVDS 
#define SIXAXIS

#define RELEASE_MODE		0	//正常运行发布版
#define PC_TEST_MODE		1	//PC裸机测试版
#define OFFLINE_TEST_MODE	2	//内部脱机测试版
//#define DSP_STATE_TEST			//测试DSP内部状态用
#define MaxServoNum			7	//最大从站数，用于内部脱机测试版
#define IOBoard_EN			1	// IO从站是否有效
#define DRIVER

#define DebugIfLostLine          0

#define PROGRAM_VERSION RELEASE_MODE//OFFLINE_TEST_MODE//

#define ARM_WAIT_DELAY 0xFFFFFFFF

//#ifndef TEST_ROBOT_CONTROL  // 测试专用
//#define TEST_ROBOT_CONTROL
//#endif
#define MAXTRAJPERIOD 1
#define Ct(n)    (((n) + INTERP_FIFO_LENTH+1) % (INTERP_FIFO_LENTH+1))


#define MaxCompensatePulse        (100*gs_State.Motor_Precision[i]/10000)

#define PeriodCompensatePulse        (2*gs_State.Motor_Precision[i]/10000)

/*#ifndef bool
#define bool unsigned char
#endif*/

#define TIME_AUTOVELCHANGE  100.0f//1200   // 再现速度从100%变化到0%所需要的时间, ms
#define TIME_AUTOVELSTOP    100.0f   // 250

#define ACC_AUTOVELCTRL    64
#define VEL_AUTOVELCTRL     3


#define MODE_AUTOVEL_INCRE  1
#define MODE_AUTOVEL_DECRE -1
#define MODE_AUTOVEL_STOP   0

#define MAX_INTRFZONE_NUM 6
#define MAX_USERFRAME_NUM 10

/* 循环数组的前一个元素下标 */
#define Array_PreID(CurrID, Array_Size) (((CurrID) > 0) ? ((CurrID) - 1) : ((Array_Size) - 1))

#define SetDSPStateLight(Index)   DSPStateLight|=(1<<(Index-1))
#define ClrDSPStateLight(Index)   DSPStateLight&=~(1<<(Index-1))
#define GetDSPStateLight(Index)  (DSPStateLight&(1<<(Index-1)))

#define LightProcess(index,period,light)   if(gs_timer.Count[index]++>period){gs_timer.Count[index]=0;\
                 if(GetDSPStateLight(light)) ClrDSPStateLight(light);\
		   else SetDSPStateLight(light);}


// [[ 插补参数
//#define INTERP_MOTION_JERK     (10000.0f)     // 加加速度
#define INTERP_JOINT_DEGREE        4     // 关节插补曲线Degree
#define INTERP_FIFO_LENTH          300//240//120//60//24//12// 6// 插补点队列长度
#define INTERP_JOINT_MAXVELPCT  (0.95f)     // 关节 插补最大速度百分比, 95%
#define INTERP_TEACH_MAXVELPCT  (0.10f)     // 示教最大速度百分比, 20%
#define INTERP_TEACH_MAXCVEL     (250.0f)     // 笛卡尔示教最大速度, mm/s
#define INTERP_TEACH_MAXGVEL      (10.0f)     // 笛卡尔示教最大姿态速度, deg/s
#define INTERP_ZONE_GRADE          4     // 过渡段最大等级
#define INTERP_ZONE_ACCPCT       1.0f     // 过渡段加速度百分比, 100%
#define INTERP_SINGULARITY_ZONE  5.0f     // 奇异区域大小, deg
#define JOINT_CART_TRANSF_EN	   0	 // 关节与笛卡尔过渡开关
// ]] 

// [[ 系统参数
#define MOF           7


#define INTERP_CNT_RATIO   2 //250us
#define INTERP_PULSE_RATIO   (26)//13*INTERP_CNT_RATIO
#define maxlenperperiod 0.1  //0.2/INTERP_CNT_RATIO
#if PROGRAM_VERSION==RELEASE_MODE||PROGRAM_VERSION==OFFLINE_TEST_MODE
#define INTERP_T	   0.00025f     // 插补周期,s
#define POS_T          0.00025f     // 位控周期,s
#else
#define INTERP_T       0.004f     // (test)插补周期,s
#define POS_T          0.004f     // (test)位控周期,s
#endif
#define RT_LITTLE    0.000001f    // 系统可识别的最小正量
#define SERR          0.00001f    // 误差允许范围
#define POS_SERR     0.001f// 0.00036f    // 关节位置误差允许范围, 1cts - 0.00036(减速比100)
#define POS_PRECISION       1    // 轴位置精度, 伺服相关
#ifndef PI
#define PI      3.1415926535898f  // 圆周率
#endif
#ifndef PI2
#define PI2     6.2831853071796f  // 2倍圆周率  
#endif
#define PI_RAD  0.0174532925199f  // 角度转换为弧度参数
#define PI_DEG 57.2957795130823f  // 弧度转换为角度参数
// ]]
#ifndef ERR_STOP_T
#define ERR_STOP_T      0.08f//  1.000 //秒s
#endif



// [[ 运动状态
#define RUN_MODE_TEACH     1     // 示教模式
#define RUN_MODE_AUTO      2     // 再现模式
#define RUN_MODE_ERR       0     // 错误模式
#define RUN_DIR_POS        1     // 正方向
#define RUN_DIR_STOP       0     // 无方向
#define RUN_DIR_NEG       -1     // 负方向
#define RUN_STATE_RUNNING  1     // 正在运动
#define RUN_STATE_STOP     0     // 停止
#define RUN_STOPMODE_HOLD  0     // 暂停
#define RUN_STOPMODE_STOP  1     // 停止
#define RUN_LIM_POS        1     // 正极限
#define RUN_LIM_NOT        0     // 无极限
#define RUN_LIM_NEG       -1     // 负极限
#define RUN_SIN_YES        1     // 奇异状态
#define RUN_SIN_NO         0     // 非奇异

#define TEACH_STOP 1
#define TEACH_RUN  2
// ]]

// [[ 坐标系
// [[ 示教坐标系 - 速度模式/位置模式
#define COORDINATE_JOINT_VEL   1    // 关节坐标系
#define COORDINATE_WORLD_VEL   2    // 世界坐标系
#define COORDINATE_TOOL_VEL    3    // 工具坐标系
#define COORDINATE_USER_VEL    4    // 用户坐标系

	#define COORDINATE_TABLE_VEL   5    // 转台坐标系
#define COORDINATE_JOINT_POS  11    // 关节坐标系
#define COORDINATE_WORLD_POS  12    // 世界坐标系
#define COORDINATE_TOOL_POS   13    // 工具坐标系
#define COORDINATE_USER_POS   14    // 用户坐标系
	#define COORDINATE_TABLE_POS  15    // 转台坐标系
// ]]

// [[ 轴状态
#define MOTOR_ENABLE       1     // 使能
#define MOTOR_DISABLE      0     // 非使能
// ]]


// [[ 程序相关
#define PROG_MODE_MOVE         1     // Move
#define PROG_MODE_IO           2     // IO
#define PROG_MODE_DELAY        3     // Delay
#define PROG_MODE_NOTHING      4     // 非程序行
#define PROG_MODE_WAIT      5    // Wait
#define PROG_MODE_WELD      10    // 焊接
#define PROG_MODE_PULSE    12
#define PROG_IOMODE_Ds   8
#define PROG_IOMODE_De   9

#define PROG_order_ARCON    1  
#define PROG_order_ARCOFF    2 
#define PROG_order_ARCCT    3 
#define PROG_order_ARCSET    4 
#define PROG_order_AWELD    5 
#define PROG_order_VWELD    6 
#define PROG_order_ARCSETAVP    11 
#define PROG_order_ARCSETACC    12
#define PROG_order_ARCSETVEL    13


#define PROG_DsorDe_DisDs      1    // 焊接
#define PROG_DsorDe_DisDe      2    // 焊接
#define PROG_DsorDe_DisDsSlow      3    // 焊接
#define PROG_DsorDe_DisDeSlow      4   // 焊接
#define PROG_DsorDe_TimeDs      8   // 焊接
#define PROG_DsorDe_TimeDe      9    // 焊接
#define PROG_DsorDe_TimeDsSlow     10    // 焊接
#define PROG_DsorDe_TimeDeSlow      11   // 焊接

#define PROG_INTERP_JOINT      1     // 关节插补
#define PROG_INTERP_LINEAR     2     // 直线插补
#define PROG_INTERP_ARC        3     // 圆弧插补
#define PROG_INTERP_CIRCLE     4     // 整圆插补
#define PROG_INTERP_PARABOLA   5     // 抛物线插补 
#define PROG_INTERP_QUINTIC    6     // 五次多项式插补

#define PROG_VELMODE_EULER     1  // 欧拉角速度模式
#define PROG_VELMODE_ANGLE     2  // 角速度模式

#define PROG_MARK_MIDLINE      0     // 中间行
#define PROG_MARK_FIRSTLINE    0     // 首行
#define PROG_MARK_LASTLINE     3     // 末行
#define PROG_MARK_SINGLELINE   1     // 单行
#define PROG_MARK_CONTINUE	   2     // 单行
#define PROG_COMM_CANREAD      2     // 程序可读
#define PROG_COMM_READOVER     3     // 读程序完成
#define PROG_RUN_NOTSTART      0     // 未运行
#define PROG_RUN_ON            1     // 正在运行
#define PROG_RUN_OVER          2     // 运动结束
#define PROG_RUN_STOP          3     // 运动停止
#define PROG_RUN_ERROR        -1     // 运动错误

#define PROG_PLAN_NODATA       1     // 无数据
#define PROG_PLAN_WAITING      2     // 等待
#define PROG_PLAN_PROCESSING   3     // 程序正在规划
#define PROG_PLAN_COMPLETED    4     // 程序规划完成

#define PROG_TRANSMODE_YES     1     // 过渡段
#define PROG_TRANSMODE_NO      0     // 非过渡段
// ]]

// [[状态常量宏定义
#define STATE_COMPLETED			 0		// 代码处理完成
#define STATE_BLOCKCOMPLETED	 1		// 代码行完成
#define STATE_FILECOMPLETED		 2		// 代码文件完成
#define STATE_PROCESSING		 3		// 代码正在处理
#define STATE_WAITING			 4		// 等待下载代码文件
#define STATE_FEEDHOLD			 5		// 暂停
#define STATE_RESET			     8		// 复位
#define STATE_WARNING            9      // 警告
#define STATE_NOHOME            10      // 非回零
#define STATE_DECPROCESS		11      // 
// ]]


// IO相关
#define AOUT_VOLT_MAX			10
#define AIN_VOLT_MAX			10
#define ANALOG_PREC				4096	// 10位精度


#define NMI  0
#define TIMER1  1
#define TIMER2  2
#define IDLE  3
#define ON   1
#define OFF  0

#ifndef IN
#define IN
#endif
#ifndef OUT
#define OUT
#endif

#define SYSTEM_OK    0   // 系统正常
#define SYSTEM_ERR  -1   // 系统有误

#ifndef SYSTEM_TRUE
#define SYSTEM_TRUE  1
#endif
#ifndef SYSTEM_FALSE
#define SYSTEM_FALSE 0
#endif
#define Square(n)   ((n) * (n))              // 平方
#define Cube(n)     ((n) * (n) * (n))        // 立方

#ifndef Max
#define Max(a,b)    (((a) > (b)) ? (a) : (b))
#endif
#define Max3(a,b,c) Max(Max(a,b), (c))
#ifndef Min
#define Min(a,b)    (((a) < (b)) ? (a) : (b))
#endif

#define Cmd_Mode  1
#define Cmd_Stop  2
#define Cmd_AlarmClear  3
#define Cmd_Enable  4
#define Cmd_Reset  5
#define Cmd_MachineLock  6
#define Cmd_PctAutoVel  7
#define Cmd_Hold  8
#define Cmd_DSPState  9
#define Cmd_InitOk  10
#define Cmd_EStop  11
#define Cmd_HOHE  12
#define Cmd_LASTER  13
#define Cmd_LAS_VEL_PWR  14
#define Cmd_GET_VESION  15
#define Cmd_GET_TIAOGAOLIST  16


// [[ 关节空间  param0-15
//U8 m_pJointControl;// 关节参数控制位     
#define pJointMaxAcc 0 // 关节最大允许加速度, deg/s*s param1-2
#define pJointMaxVel 6// 关节最大速度
#define pJointStopDec 12// 关节停止减速度, deg/s*s
#define pJointPLim    18// 关节正极限, deg
#define pJointNLim  24// 关节负极限, deg  param13-15
 //]]



// [[ 轴空间 ,电机数据
//U8 m_pMotorControl ; // 轴参数控制位       param16-31
#define pMotorRatio 30    // 轴减速比
#define pMotorAbsZero 36   // 轴绝对零点, 电机转角
#define pMotorPrec 42                 // 轴精度, cts/round
#define pMotorDir 48 
// ]]

#define pZoneLevel 50 

// [[ 笛卡尔空间 param32-63
//U8 m_pCarControl; // 笛卡尔参数控制位
#define pAccTime 64// 加速时间, ms
#define pCarMaxPVel 65 // 笛卡尔位置最大速度, mm/s
#define pCarMaxGVel 66 // 笛卡尔姿态最大速度, deg/s
#define pCarMaxPAcc 67 // 笛卡尔位置加速度
#define pCarMaxGAcc 68 // 笛卡尔姿态加速度
#define pCarStopPDec 69 // 笛卡尔位置停止减速度
#define pCarStopGDec 70 // 笛卡尔姿态停止减速度
//]]

#define pK1    72   //y1 PID开关
#define pKp1    73	//y1 P参数
#define pKi1    74	//y1 i参数
#define pKd1    75//y1 d参数
#define pK2    76	//y2 PID开关
#define pKp2    77	//y2 P参数
#define pKi2    78	//y2 i参数
#define pKd2    79//y2 d参数

#define pLineMaxLen    80	//直线长度分段长度
#define pLineMaxVel    81	//直线长度分段速度
#define pLineMaxPer   82	//直线长度分段时最大加速度比
#define pLineMinPer    83	//直线长度分段时最小加速度比
#define pArcRadMaxLen    84	//圆弧长度分段半径
#define pArcLenMaxLen    85	//圆弧长度分段长度
#define pArcMaxVel    86	//圆弧长度分段速度
#define pArcMaxPer    87	//圆弧长度分段时最大加速度比
#define pArcMinPer    88	//圆弧长度分段时最小加速度比
#define pArcError    89	//过渡圆弧最大误差

#define pLinkDimension   90

/*#define pPOSKP   96
#define pVELKP   102
#define pVELKI  108
#define pCURKP   114
#define pCURKI   120*/

#define pAIXS1PID   96
#define pAIXS2PID   102
#define pAIXS3PID  108
#define pAIXS4PID   114

#define pAIXS5PID   120
#define pAIXS6PID  126
#define pAIXS7PID   132

#define GoHomeOrder1   138   // 手动 回零顺序, 步骤1.  默认值 3. 先回Z轴
#define GoHomeOrder2   139   // 手动 回零顺序, 步骤2.  默认值 1.
#define GoHomeOrder3   140   // 手动 回零顺序, 步骤3.  默认值 2.
#define GoHomeOrder4   141   // 手动 回零顺序, 步骤4.  默认值 4.

//#define GoHomeOrder1   120   // 手动 回零顺序, 步骤1.  默认值 3. 先回Z轴
//#define GoHomeOrder2   121   // 手动 回零顺序, 步骤2.  默认值 1.
//#define GoHomeOrder3   122   // 手动 回零顺序, 步骤3.  默认值 2.
//#define GoHomeOrder4   123   // 手动 回零顺序, 步骤4.  默认值 4.

//#define pAIXS5PID   124
//#define pAIXS6PID  130
//#define pAIXS7PID   136

#define ep_CoorID   142
#define pJointMaxVel7   166
//============================================================

#define HOME_START     0
#define HOME_POSTIVE_VEL   1
#define HOME_NEGTIVE_VEL   2
#define HOME_POSTIVE_PULSE 3
#define HOME_NEGTIVE_PULSE 4
#define HOME_POSTIVE_STOP 5
#define HOME_NEGTIVE_STOP 6
#define HOME_NEGTIVE_VEL_2   7

#define VEL_OVER_TIME 3
#define PULSE_OVER_TIME 3000//30000脉冲=30mm

#define MAXFRAMELEN 0x100
#define MAXDATALEN  (MAXFRAMELEN-6)
#define TEACHFRAMELEN 20


#define FramequeueLen 20
#define HaveSendqueueLen 20
#define PulseFramequeueLen 100
#define FirstByte (0x55)
#define SecondByte (0xaa)
#define FrameHeadWord (0xaa55)

#define OK		1
#define FAIL	0

#ifndef ALARMINFORMSTRUCT
#define ALARMINFORMSTRUCT
typedef struct _alarmInformStruct
{
    int Err_Comm; // 通讯错误编号
    int Err_Plan; // 规划错误
    int Err_Run;  // 运行错误
    int Err_Lim;  // 极限
    int Alm_Ser;  // 伺服错误
    int OTC_CIA;  // 干涉区
}AlarmInformStruct;
#endif

typedef struct _velPctStruct
{
	bool Enable;
	
	double P1;    // 首点
	double P2;    // 末点
	double A1; // 加速段加速度
	double V1; // 初速度
	double T1; // 加速段时间
	double T2; // 总时间

	double time;  // 当前时间
	double vel;  // 当前速度
	double pos;  // 当前位置

}VelPctStruct;

typedef struct _velCtrlStruct
{
	int Mode; // 模式, 1加 | 0停止 | -1减

	double Pos; // 当前位置, [0,1]
	double Vel; // 当前速度, 
}VelCtrlStruct;

// 位姿矩阵结构
typedef struct _poseMatrix
{
	double R11;
	double R12;
	double R13;
	double R21;
	double R22;
	double R23;
	double R31;
	double R32;
	double R33;
	
	double X;
	double Y;
	double Z;
}PoseMatrix;


typedef struct _FrameBody
{

	U16 framehead;	
	U8  funcode;
	U8  length;
	U8  frameno;
	U8  trafficctrl;
	U8  databuf[MAXDATALEN];
}FrameBody;


typedef struct _FrameTrafficCtrl
{
	U8  sendframeno;//非周期性发送帧序号
	U8  recvframeno;//收到正确非周期性的帧序号
	U8  sendtrafficctrl;//周期性帧中要发送的流控制字节
	U8  recvtrafficctrl;//收到的周期性帧中的流控制定节	
	U8  succedrecvno;//收到周期性数据反馈的对方已收到的帧序号
	U8  sendstate;//发送非周期性数据流控制状态字
  volatile    U8  overtime;//超时
	U8  periodovertime;
	U16  programline;//位置/脉冲帧中的行号
	U16  currentpoint;//位置/脉冲帧中的点序号
	U16  periodctrlword;//要发送周期性帧中的控制字
	U16  periodstateword;//接收到的周期性帧中的状态字
	U8  control;//控制帧中的控制字
	U8  resendno;//重发送序号
	U8  state;//状态帧中的状态字
	U8  alarmno;//报警号
	U8    sendinput[8];
	U8    sendoutput[8];
	U8    recvinput[8];
	U8    recvoutput[8];
	float    realpos[8];//max 8 axis
	U32   decode_id;
	U32   statebit;
	float apos;
    float bpos;
    float cpos;
    float dpos;
	float epos;
       float fpos;
       float gpos;
	float reserve;
#ifdef  PROGRAM_USELVDS
	uint64_t pulsepos[8];// 4
	uint64_t realaxispos[8];// 4
#else
	U32 pulsepos[8];// 4
	U32 realaxispos[8];// 4
#endif
	U32  realaxis[16];// 8
	U32  plusepos[16];// 8
	AlarmInformStruct alarm;
	
}FrameTrafficCtrl,*pFrameTrafficCtrl;


typedef struct _Framequeue
{
	FrameBody*queue;
	U16 front;
	U16 rear;
	U16 maxsize;
}Framequeue;

typedef struct _Sendqueue
{
	FrameBody*queue;
	U16 front;
	U16 rear;
	U16 maxsize;
}Sendqueue;





// 轨迹点参数类
#ifndef _TRAJPOINTPARA_
#define _TRAJPOINTPARA_
typedef struct _trajPointPara
{
	double Acc; // 加速度
	double Vel; // 速度
	double Len; // 距离
}TrajPointPara;
#endif

// 轨迹输入数据类
typedef struct _spdPTrajInputData
{
	double Len;  // 轨迹段长度
	double Vs;   // 初始速度
	double V;    // 期望速度
	double Ve;   // 终点速度
	double Acc;  // 最大加速度
	double Jerk; // 最大加加速度
	double T_Acc;// 加速时间
}SpdPTrajInputData;

// 轨迹输出数据类
typedef struct _spdPTrajData
{
	double Len;  // 轨迹段长度
	double Vs;   // 初始速度
	double V;    // 期望速度
	double Ve;   // 终点速度

	double Jacc;
	double Jdec;

	double T1;   // 各段时间
	double T2;
	double T3;
	double T4;
	double T5;
	double T6;
	double T7;
	
	double T;    // 规划总时间

	double V1;   // 各段端点速度
	double V2;
	double V3;
	double V4;
	double V5;
	double V6;

	double L1;  // 各段端点位移
	double L2;
	double L3;
	double L4;
	double L5;
	double L6;
	double L7;
}SpdTrajStruct;


#ifndef JOINTPOSE
#define JOINTPOSE

typedef struct _jointPose
{
    double S;      // 轴S转角,deg
    double L;      // 轴L转角,deg
    double U;      // 轴U转角,deg
    double R;      // 轴R转角,deg
    //double B;      // 轴B转角,deg
    //double T;      // 轴T转角,deg
}JointPose;
#endif




#ifndef CARTESIANPOSE
#define CARTESIANPOSE

typedef struct _cartesianPose
{
    double X;     // 位置X,mm
    double Y;     // 位置Y,mm
    double Z;     // 位置Z,mm
    double W;     // 姿态W,deg
    double P;     // 姿态P,deg
    double R;     // 姿态R,deg
    double M;     // 姿态M,deg
}CartesianPose;
#endif




#ifndef INPUTAUTODATA
#define INPUTAUTODATA
// 系统输入再现数据结构
typedef struct _inputAutoData
{
    unsigned int decode_id;
    unsigned int Prog_SequNum;     // 当前程序段编号	//24
    unsigned int Prog_LineNum;     // 当前程序行编号     //28
    unsigned int Prog_Mark;        // 程序标识 0:非最后一行  1:单行或最后一行//32
    // ]]

    unsigned int  Mode;             // 程序行模式, 运动行-1 | IO-2 | Delay-3 | 非程序行-4 |Wait-5|焊接-10  //36

    // [[ 运动行信息//
    unsigned int  Interp_Mode;      // 插补方式 - MOVJ 1 | MOVL 2 | 圆弧 3 | 整圆 4   //40

    int Joint_VelPct;     // 关节插补速度百分比   //44
    int PathSN;			  // 圆弧时指定是第一段还是第二段，0--整段，1--第一段，2--第二段//48
	
    double JPos_Start[MOF]; // 起始点, 圆弧插补时用（明确赋值）  //空间变化
    double JPos_Mid[MOF];   // 中间点, 圆弧/抛物线插补时用  //空间变化
    double JPos_End[MOF];   // 目标点  //空间变化
    double Car_PVel;      // 笛卡尔插补位置速度, 0-默认//200
    double Car_GVel; 	  //208
    int Zone;             // 过渡等级, 0,1,2,3,4   //216

    // [[ IO行信息
    int IO_Mode;          // 1-输入 | 2-输出 3-模拟输入 4-模拟输出//244
    int IO_Name;          // IO编号//248
    int IO_Value;         // IO值, 1-true | 0-false//252
    // ]]
    int WaitTime;         // 等待时间, ms//256
}InputAutoData;
#endif

#ifndef INPUTTEACHDATA
#define INPUTTEACHDATA
// 输入示教数据结构定义
typedef struct _inputTeachData
{
    int Stop; 	// 示教状态 stop-1 run - 2
    int Coor; 	// 速度方式：关节--1、直角--2、工具--3、工件--4；定长方式：关节--11、直角--12、工具--13、工件--14
    int Aix;  	// 轴1-6
    int Vel;  	// 速度百分比
    int Dir;  	// 方向 -1/1 
    int Acc;  	// 加速度
	double Pos;	// 定长示教下位移长度，角度量单位：deg，平移量单位：mm
}InputTeachData;
#endif

// 示教结构定义
typedef struct _teachStruct
{
	//------ 输入 ------//
	double Interp_T;          // 插补周期
	int Tacc;                 // 加速周期数   - 关节 | 笛卡尔
	int Flag[MOF];       // 各轴运动状态 - JC
	int Dir[MOF];        // 各轴示教方向 - JC
	int Time[MOF];       // 加速周期计数 - JC,//从大往小减 +pyc
	double Vel[MOF];     // 各轴期望速度 - JC

	//---- 定长示教 ----//
	double T_Acc;
	double Total_T[MOF]; // 运动总时间
	double Start_Pos[MOF];  /*  开始位置	+pyc*/
	double Tar_Pos[MOF]; // 目标位置

	double Act_T[MOF];   // 当前时间

	//------ 计算 ------//
	double Jerk[MOF];    // 加加速度 - JC
	double Act_Acc[MOF]; // 加速度   - JC
	double Act_Vel[MOF]; // 各轴速度 - JC
	double Act_Pos[MOF]; // 各轴位置 - JC

	//------ 输出 ------//
	double O_Acc[MOF];   // 输出加速度 - JC
	double O_Vel[MOF];   // 输出速度   - JC
	double O_Pos[MOF];   // 输出位姿   - JC

	// 笛卡尔坐标系示教专用
	double Curr_Vel[MOF];// 当前速度 - C
	double Curr_Pos[MOF];// 当前位姿 - C
}TeachStruct;


#ifndef JOINTINTPARA
#define JOINTINTPARA
typedef struct _jointIntPara
{
    short int S;
    short int L;
    short int U;
    //short int R;
    //short int B;
    //short int T;
}JointIntPara;
#endif

#ifndef ALARMINFORMSTRUCT
#define ALARMINFORMSTRUCT
typedef struct _alarmInformStruct
{
    int Err_Comm; // 通讯错误编号
    int Err_Plan; // 规划错误
    int Err_Run;  // 运行错误
    int Err_Lim;  // 极限
    int Alm_Ser;  // 伺服错误
    int OTC_CIA;  // 干涉区
}AlarmInformStruct;
#endif

typedef struct _alarmQueue
{
    int Err_Num;                         //  ?μí3±¨?ˉêy
    int Err_Number[10];  // ?μí3±¨?ˉo?
}AlarmQueue;

#ifndef PROGRAMINFORMSTRUCT
#define PROGRAMINFORMSTRUCT
// 程序状态
typedef struct _programInformStruct
{
	int Prog_State; // 1. 运动：当前程序运动状态, 0无运行, 1正在运行, 2运动结束, 其它-运动错误
	                // 2. 规划：0正常, 其它-规划错误。行号和段号只在规划错误时有效
	int CurrLine_State;	//当前行状态, 0 无运行, 1 正在运行, 2 运动结束, 其它-运动错误
    // 程序名
    // 显示用到
    //char name[20];
	
    // 译码的部编号
    // 实时同步用到
    unsigned int decode_id ;
	int Prog_SequNum;// 所属程序段号
	int Prog_LineNum;// 所属程序行号
	int IfTransSeg;  // 是否是过渡段
}ProgramInformStruct;

#endif

#ifndef PROGRAMRUNSTRUCT
#define PROGRAMRUNSTRUCT
// 程序状态
typedef struct _programRunInformStruct
{
   	 // 实时同步用到
	double time;      //该轨迹已运行时间
	double totaltime;//该轨迹运行总时间
	double len; //该轨迹已运行长度
	double totallen;//该轨迹运行总长度
	
}ProgramRunInformStruct;

#endif

typedef struct  _SYSTEMSTATUS
{
	U16 pEStop;        
	U16 pMode;       
    U16 intCanUse; 
    U16 intMode; 
	U16 pStop;       
    U16 pReset; 
	U16 pEnable;   
    U16 pAbort;
	U16 pPctAutoVel;
    U16 pHold;
	U32 DSPStateLight;
}SYSTEMSTATUS;

#ifndef IOCARDCTRL
#define IOCARDCTRL
typedef struct _IoCardCtrl
{
	U32 DigitalSignal[2];			 
	U32 AnalogSignal[2];         
	U16 CardADC[4];
}IoCardCtrl;
#endif

#ifndef IOARMCTRL
#define IOARMCTRL
typedef struct _IoArmCtrl
{
	U32 DigitalSignal[2];	//64位IO		       
	U16 Control;//控制位，1-ARM IO指令有效 2-DSP读取后置为2
}IoArmCtrl;
#endif


// DSP定时器，单位-POS_T插补周期
typedef struct _dsp_Timer
{
	U16 Count[4];
	/*U16 NmiCount;
	U16 Timer1Count;
	U16 Timer2Count;
	U16 IdleCount;*/
	
}DSP_Timer;

#ifndef LINKDIMENSION
#define LINKDIMENSION
// 机器人连杆参数结构
typedef struct _linkDimension
{
	double A1;           // a1,mm
	double A2;           // a2,mm
	double A3;           // a3,mm
	double D3;           // d3,mm
	double D4;           // d4,mm
	double Df;           // df,mm
	
}LinkDimension;
#endif

#ifndef CARTESIANPOSE
#define CARTESIANPOSE
// 笛卡尔坐标系表示
typedef struct _cartesianPose
{
	double X;     // 位置X,mm
	double Y;     // 位置Y,mm
	double Z;     // 位置Z,mm
	double W;     // 姿态W,deg
	//double P;     // 姿态P,deg
	//double R;     // 姿态R,deg
}CartesianPose;
#endif

#ifndef JOINTPOSE
#define JOINTPOSE
// 关节坐标系表示
typedef struct _jointPose
{
	double S;      // 轴S转角,deg
	double L;      // 轴L转角,deg
	double U;      // 轴U转角,deg
	double R;      // 轴R转角,deg
	//double B;      // 轴B转角,deg
	//double T;      // 轴T转角,deg
}JointPose;
#endif



#ifndef PROGRAMINFORMSTRUCT
#define PROGRAMINFORMSTRUCT
// 程序状态
typedef struct _programInformStruct
{
	int Prog_State; // 1. 运动：当前程序运动状态, 0无运行, 1正在运行, 2运动结, 其它-运动错误
	                // 2. 规划：0正常, 其它-规划错误。行号和段号只在规划错误时有效
	int CurrLine_State;	//当前行状态, 0 无运行, 1 正在运行, 2 运动结束, 其它-运动错误
    // 程序名
    // 显示用到
    //char name[20];
	
    // 译码的部编号
    // 实时同步用到
    unsigned int decode_id;
	int Prog_SequNum;// 所属序段号
	int Prog_LineNum;// 所属程序行号
	int IfTransSeg;  // 是否是过渡段
}ProgramInformStruct;
#endif



typedef struct _globalState
{
/////////////////////////////////////////////////////////////////////// 参数设置
	int Acc_Time;              // 加速时间, ms

// [[ 笛卡尔
	double Car_MaxPVel;          // 笛卡尔位置最大速度, mm/s
	double Car_MaxGVel;          // 笛卡尔姿态最大速度, deg/s
	double Car_MaxPAcc;          // 笛卡尔位置加速度
	double Car_MaxGAcc;          // 笛卡尔姿态加速度
	double Car_StopPDec;         // 笛卡尔位置停止减速度
	double Car_StopGDec;         // 笛卡尔姿态停止减速度
	double INTERP_MOTION_JERK;
	double LineMaxLen;
	double LineMaxVel;
	double LineMaxPer;    
	double LineMinPer; 
	double ArcRadMaxLen; 
	double ArcLenMaxLen;
	double ArcMaxVel;
	double ArcMaxPer; 
	double ArcMinPer; 
	double ArcError;
	double T0;
	//LinkDimension Robot_Link;    // 连杆参数
	//CartesianPose Robot_Tool;    // 工具坐标系参数
	//CartesianPose Robot_User;    // 用户坐标系参数
// ]] 笛卡尔

// [[ 关节空间
	double Joint_MaxVel[MOF];    // 关节最大速度限制, mm/s*s
	double Joint_StopDec[MOF];   // 关节停止减速度, mm/s*s
	double Joint_PLim[MOF];      // 关节正极限, mm
	double Joint_NLim[MOF];      // 关节负极限, mm
	double Joint_MaxAcc[MOF];    // 关节最大加速度, mm/s*s
// ]] 关节空间

// [[ 轴空间
	double Motor_Ratio[MOF];     // 轴减速比
	double Motor_AbsZero[MOF];   // 轴绝对零点, 电机转角
	double Motor_Precision[MOF];    // 轴精度, cts/round
	int Motor_Dir[MOF];          // 轴方向, "1"-电机方向与关节方向一致, "-1"-不一致
// ]] 轴空间

/////////////////////////////////////////////////////////////////////// 反馈数据
	CartesianPose Car_ActPos; // 实际位姿
	double Joint_ActPos[MOF]; // 关节实际位置
	double Joint_ActVel[MOF]; // 关节实际速度
	double Motor_ActCurr[MOF];// 轴实际电流
	double Motor_ActPos[MOF]; // 轴实际转角
	double Joint_CurrentPos[MOF]; // 路径规划的当前位置
	double Joint_CalculPos[MOF];
	double Car_ActVel[MOF];   // 笛卡尔实际速度

	short Joint_Dir[MOF];     // 关节运动方向  +pyc:显示诊断用
	short Flag_PLim[MOF];     // 关节位置极限标志, 正极限(1)/非极限(0)/负极限(-1)
	short Joint_State[MOF];   // 关节运动状态  +pyc:显示诊断用
	short Flag_Enable[MOF];   // 使能标志  +pyc:显示诊断用
	short Flag_VLim[MOF];     // 关节速度极限标志  +pyc:显示诊断用
	//double Table_MaxVel;  //转台轴最大合成旋转速度,  deg/s
	//double Table_MaxAcc;  //转台轴最大合成最大加速度,  deg/s.s
	//TableConfigT Table_ConfigT;   //转台配置
	//int Table_Gt;

/////////////////////////////////////////////////////////////////////// 全局状态
// [[ 输入
	U16 Mode;           // 示教(1)/再现(2) - // ARM设置值：1 或 2
	U16 Stop;           // 停止(1)         - // ARM设置值：1
	U16 Hold;           // 暂停(1)/继续(0) - // ARM设置值：1 或 0
	U16 AlarmClear;          // 复位(1)         - // ARM设置值：1
	U16 EStop;          // 急停(1)         - // ARM设置值：1
	U16 Abort;          // 取消运动(1)     - // ARM设置值：1
	U16 MachineStop;    // 锁定(1)/解除(0) - // ARM设置值：1 或 0
	U16 Pct_AutoVel;    // 再现运动速度百分比, 可在运动过程中设置
// ]] 输入

// [[ 当前状态
	U16 MotionMode;     // 当前运动模式 - 示教(1)/再现(2)/错误(0)
	U16 JogCoordinate;  // 手动坐标系选择 - 关节(1)/基(2)/工具(3)/用户(4)
	U16 In_Motion;      // 正在运动标志(1)
	U16 Singularity;    // 奇异位形(1)
	U16 MotorEnable;    // 轴使能(1)
// ]] 当前状态
	/* DSPState[0]-停止状态，DSPState[1]-暂停状态, DSPState[2]-总线状态*/
	char DSPState[4];
// [[ 程序状态
	ProgramInformStruct Prog_Run;  // 程序当前运动状态, 在插补时更新
	ProgramInformStruct Prog_Plan; // 程序当前状态, 在规划错误时更新
// ]] 程序状态

// [[ 错误信息
	AlarmInformStruct Err;    // 错误信息, 在程序执行过程中更新
	VelPctStruct* pVelPct;
}GlobalState;

typedef struct STRC_DelayCtrl
{
	int Req;	//请求延时标志
	int DlyTime;//延时时间

}UD_DelayCtrl;

//IO闪灯内部参数
typedef struct STRC_DSPStateLightParam
{
	unsigned int TotleTimes;
	unsigned int BlinkTimes;
	unsigned int CurrTimes;
	unsigned int IsBlinkSrart;
}UD_DSPStateLightParam;

//焊接控制结构
/*typedef struct STRC_WeldControl
{

       int WaitTime;         // 等待时间, ms//256
	// Mode == 10 焊接
	// 1 ARCON
	// 2 ARCOF
	// 3 ARCCT 
	// 4 ARCSET
	// 5 AWELD
	// 6 VWELD
	int order ;      
	// 电流
	double AC ;
	// 电压
	double AVP ;
	// 时间
	double T ;
	// 速度
	double V ;
	// 1 ds
	// 2 es
	int  dis_type;    
	double dis;

    double  CurrentAvp;
    double  CurrentAcc;
   // double  Vel;
    U32 IO_Mode;          // 1-输入 | 2-输出
    U32 IO_Name;          // IO编号
    U32 IO_Value;         // IO值, 1-true | 0-false
}WeldControl;*/

//errorplan.h

// 轨迹初始参数
typedef struct _trajInitPara
{
	double Jerk; // 加加速度
	double Acc;  // 加速度
	double Vel;  // 速度
	double Len;  // 距离
}TrajInitPara;

// 错误停止插补点
#ifndef _TRAJPOINTPARA_
#define _TRAJPOINTPARA_
typedef struct _trajPointPara
{
	double Acc; // 加速度
	double Vel; // 速度
	double Len; // 距离
}TrajPointPara;
#endif

// 错误停止结构体
typedef struct _errStopTrajStruct
{
	int Stop;      // 停止标志
	
	double Interp_T; // 插补周期
	
	double Time;   // 停止时间
	double T[MOF];   // 
	double Pos[MOF]; // 初始位置
	double Vel[MOF]; // 初始速度
	double Acc[MOF]; // 加速度
	double Curr_T; // 当前时间
	
}ErrStopTrajStruct;

//interploate
#define PATH_BUF_MAX_LEN        34//17//6//5  // 路径缓冲区长度
#define TRAJECTORY_BUF_LEN_MIN  50//18//6  // 轨迹缓冲区最小长度
#define TRAJECTORY_BUF_LEN_MAX 60//36//14//12//10  // 轨迹缓冲区最大长度

#ifndef ABS
#define ABS(a)		((a)>=0?(a):(-(a)))
#endif
#define Count(n)    (((n) + PATH_BUF_MAX_LEN) % PATH_BUF_MAX_LEN) // 路径缓冲区计数器
#define Cnt(n)    (((n) + TRAJECTORY_BUF_LEN_MAX) % TRAJECTORY_BUF_LEN_MAX)
//------------------------------- 结构定义 ----------------------------------//
// MOVJ路径特征
typedef struct _jointPathFeature
{
	double JPos_Start[MOF];  // 起点
	double JPos_Mid1[MOF];
	double JPos_Mid2[MOF];
	double JPos_End[MOF];    // 末点
	double JVel_Cons[MOF];   // 匀速度

	double Time;
	double ld_acctime;
}JointPathFeature;

// 空间向量
typedef struct _vectorStruct
{
	double X;   // 向量x
	double Y;   // 向量y
	double Z;   // 向量z
	double Con; // 常量
}VectorStruct;

// 笛卡尔路径特征
typedef struct _cartesianPathFeature
{
	double JPos_Start[MOF];   // 起点关节角
	double JPos_Mid[MOF];     // 圆弧中间点关节角
	double JPos_End[MOF];     // 末点关节角

	CartesianPose CPos_Start; // 起点位姿
	CartesianPose CPos_Mid;   // 圆弧中间点位姿
	CartesianPose CPos_End;   // 末点位姿

	double Car_PAcc;          // 线加速度
	double Car_PVel;          // 期望线速度
	double PosLen;            // 路径长度, 直线长度或圆弧弧长
	//double Car_GAcc;          // 姿态加速度
	//double Car_GVel;          // 期望姿态速度
	//double GesAng;            // 姿态角
	bool Flag_Reverse;

	double Time;
	VectorStruct Line_Vector; // 直线单位向量, 常量Con = 1
	double Angle_Vector[2];  //旋转单位向量
	double Car_Vec;      //笛卡尔长度分量

	PoseMatrix   Arc_Matrix;  // 圆弧UVW新坐标系矩阵
	VectorStruct Arc_Centre;  // 圆心坐标及半径
	double       Arc_Radian;  // 圆弧的弧度
	double       Arc_MidLen;  // 圆弧中间点对应的弧长
	double       Arc_Start;  //
	double       Arc_End;  // 
	CartesianPose CPos_Zone;   //过渡起始位置
	VectorStruct  PVel_Zone;   //过渡起始速度
	double  zonelen;	//过渡长度
	int zone;//过渡等级

}CartesianPathFeature;

// 路径
typedef struct _interpPathStruct
{
	InputAutoData Input;

	JointPathFeature  JointPath;
	CartesianPathFeature CartePath;
}InterpPathStruct;


// 轨迹
typedef struct _InterpTrajectoryStruct
{
	// 程序名
    // 显示用到
    //char name[20];
	
    // 译码的部编号
    // 实时同步用到
    unsigned int decode_id ;

	//------------------- 段信息 --------------------//
	int Prog_SequNum;         // 当前程序段编号	
	int Prog_LineNum;         // 当前程序行编号
	int Prog_Mark;            // 程序标志, 1-start/2-end/0-mid	
	int Mode;                 // 程序行模式, 运动行-1 | IO-2 | Delay-3
	int Interp_Mode;          // 插补方式 - MOVJ 1 | MOVL 2 | MOVC 3 | MOVP 4
	int IfTransSeg;           // 是否是过渡段, 1是/0否

	//------------------ 运动参数 --------------------//
	double TotleTime;		  //当前行总时间
	double Time;              // 总时间

	double JPos_Start[MOF];   // 始点关节角
	double JPos_End[MOF];     // 末点关节角
	
// [[ 关节插补
	double JVel_Start[MOF];   // MOVJ 关节初速度
	double JVel_End[MOF];     // MOVJ 关节末速度
// ]]

// [[ 笛卡尔插补
	CartesianPose CPos_Start;   // 始点位姿
	CartesianPose CPos_End;     // 末点位姿
	PoseMatrix    Arc_Matrix;   // 圆弧UVW新坐标系矩阵
	VectorStruct  Arc_Centre;   // 圆心坐标及半径
	double        Arc_Len_Start;// 圆弧起始点弧长
	double   Arc_End;
	double   Arc_Start;
	SpdTrajStruct Traj_CPos;    // 位置轨迹, 非过渡段使用
	//SpdTrajStruct Traj_CGes;    // 姿态轨迹
	bool          Ges_Reverse;  // 姿态反向插补标志, 1反向 | 0正向
	double        CVel_Start[MOF];// 位置初速度, 过渡段使用
	double        CVel_End[MOF];  // 位置末速度, 过渡段使用

	double Car_Vec;      //笛卡尔长度分量

	VectorStruct Line_Vector; // 直线单位向量, 常量Con = 1
// ]]

	//------------------ 运动参数 --------------------//
	// [[ IO行信息
	int IO_Mode;          // 1-输入 | 2-输出
	int IO_Name;          // IO编号
	int IO_Value;         // IO值, 1-true | 0-false
	// ]]
	int WaitTime;         // 等待时间, ms	
}InterpTrajectoryStruct;


// 路径缓冲区
typedef struct _path_Buffer
{
	InterpPathStruct Data[PATH_BUF_MAX_LEN];   
	U16 Start;
	U16 End;
	U16 Now;
	
	U16 Num;
}Path_Buffer;


// 轨迹缓冲区
typedef struct _trajectory_Buffer
{
	InterpTrajectoryStruct Data[TRAJECTORY_BUF_LEN_MAX];
	U16 Start;
	U16 End;
}Trajectory_Buffer;


// 关节空间插补结构体
typedef struct _jointInterpStruct
{
	int Degree;     // 插补拟合曲线的最高次幂
	double Pos0[MOF]; // 第一点数据
	double Pos1[MOF]; // 第二点
	double Pos2[MOF]; // 第三点
	double Pos3[MOF]; // 第四点
	double Pos4[MOF]; // 第五点 
	int Num;        // 每周期插补点数
	int Count;      // 插补点计数
}JointInterpStruct;

typedef struct _Laseter
{	
	unsigned char cmd;
	unsigned char curlasterno;
	unsigned char pwrbyvel;
	unsigned char mode;
	unsigned short pwmHz;
	unsigned char per;
	unsigned char pwrper;
	unsigned char senddata[10];
	unsigned char onoff[4];
	unsigned char timcnt;
	unsigned char sendcnt;
}Laster,*pLaster;

typedef struct _LaseterList
{	
	unsigned char listno;
	unsigned char num;
	unsigned char type;
	unsigned char reserver;
	unsigned char  per[100];
}LasterList,*pLasterList;

typedef struct _Home
{
	
	int axis;
	int state[MOF];
	int ifhome[MOF];
	int cnt[MOF];
	int JogCoordinate[MOF];
}Home,*pHome;



typedef struct _elemType
{	
	char name[20];	//程序名
	U32 decode_id;	//译码ID
	U32	Mode;	//运动行-1 | IO-2 | Delay-3 | 非程序行-4(等待)
	U32 Prog_SequNum;// 所属程序段号
	U32 Prog_LineNum;// 所属程序行号
	U32 Prog_Mark;	 // 行标志位
	U32 IfTransSeg;  // 是否是过渡段
	U32	IfLastPoint; // 是否最后一点，0 非最后点，1 行最后点，2 段最后一点
	int Error;         // 点错误状态

	double Posval[MOF]; // 六轴位置+转台2轴位置
	double Spdval[MOF]; // 六轴速度+转台2轴位置

	U32 IO_Mode;          // 1-输入 | 2-输出
	U32 IO_Name;          // IO编号
	U32 IO_Value;         // IO值, 1-true | 0-false
	
	U32 WaitTime;         // 等待时间, ms

	// Mode == 10 焊接
	// 1 ARCON
	// 2 ARCOF
	// 3 ARCCT 
	// ----4 ARCSET----11,12,13
	// 5 AWELD
	// 6 VWELD

	// 11 ARCSET  av
	// 12 ARCSET  ac
	// 13 ARCSET  v
	int order ;      
	// 电流
	double AC ;
	// 电压
	double AVP ;
	// 时间
	double T ;
	// 速度
	double V ;
	double Percent;
	int IfDout;       //是否输出电压电流
	int IfAout;
	//add for return to arm
	double time;
	double totaltime;
	double len;
	double totallen;
}ElemType;

typedef struct _elemPiont
{	
	double X;
	double Y;
	U32 decode_id;	//译码ID
	U16 Error;   // 点错误状态
	U16 flag;//bit0-Prog_Mark bit1-IfTransSeg bit2-IfLastPoint bit3-IfDout
	U8  Mode;//运动行-1 | IO-2 | Delay-3 | 非程序行-4(等待)|5-wait io input
	U8 IO_Mode;          // 1-输入 | 2-输出
	U8 IO_Name;          // IO编号
	U8 IO_Value;         // IO值, 1-true | 0-false
	U32 WaitTime;         // 等待时间, ms
	U16 DoutSetBit;        //数字IO设置位
	U16 DoutClrBit;        //数字IO清零位
	U8 DOUT61VAL;
	U8 DOUT62VAL;
	U8 DOUT63VAL;
	U8 DOUT64VAL;
}ElemPoint;




typedef struct _queue
{
	ElemType *Queue;   // 指向存储队列的数组空间
	volatile U16 Front;// 队首指针（下标）,队首指针不指向元素
	volatile U16 Rear; // 队尾指针（下标）
    U16 MaxSize;       // Queue长度
}Queue;

// 轨迹插补状态
typedef struct _interpolationState
{
	bool Enable;    // 插补使能状态
	double Time;    // 当前插补时间
	InterpTrajectoryStruct Traj; // 当前插补轨迹段

	bool IfEndPoint;   // 末点标志
	ElemType EndPoint; // 末点
	
}InterpolationState;



#endif

