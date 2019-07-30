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

#define RELEASE_MODE		0	//�������з�����
#define PC_TEST_MODE		1	//PC������԰�
#define OFFLINE_TEST_MODE	2	//�ڲ��ѻ����԰�
//#define DSP_STATE_TEST			//����DSP�ڲ�״̬��
#define MaxServoNum			7	//����վ���������ڲ��ѻ����԰�
#define IOBoard_EN			1	// IO��վ�Ƿ���Ч
#define DRIVER

#define DebugIfLostLine          0

#define PROGRAM_VERSION RELEASE_MODE//OFFLINE_TEST_MODE//

#define ARM_WAIT_DELAY 0xFFFFFFFF

//#ifndef TEST_ROBOT_CONTROL  // ����ר��
//#define TEST_ROBOT_CONTROL
//#endif
#define MAXTRAJPERIOD 1
#define Ct(n)    (((n) + INTERP_FIFO_LENTH+1) % (INTERP_FIFO_LENTH+1))


#define MaxCompensatePulse        (100*gs_State.Motor_Precision[i]/10000)

#define PeriodCompensatePulse        (2*gs_State.Motor_Precision[i]/10000)

/*#ifndef bool
#define bool unsigned char
#endif*/

#define TIME_AUTOVELCHANGE  100.0f//1200   // �����ٶȴ�100%�仯��0%����Ҫ��ʱ��, ms
#define TIME_AUTOVELSTOP    100.0f   // 250

#define ACC_AUTOVELCTRL    64
#define VEL_AUTOVELCTRL     3


#define MODE_AUTOVEL_INCRE  1
#define MODE_AUTOVEL_DECRE -1
#define MODE_AUTOVEL_STOP   0

#define MAX_INTRFZONE_NUM 6
#define MAX_USERFRAME_NUM 10

/* ѭ�������ǰһ��Ԫ���±� */
#define Array_PreID(CurrID, Array_Size) (((CurrID) > 0) ? ((CurrID) - 1) : ((Array_Size) - 1))

#define SetDSPStateLight(Index)   DSPStateLight|=(1<<(Index-1))
#define ClrDSPStateLight(Index)   DSPStateLight&=~(1<<(Index-1))
#define GetDSPStateLight(Index)  (DSPStateLight&(1<<(Index-1)))

#define LightProcess(index,period,light)   if(gs_timer.Count[index]++>period){gs_timer.Count[index]=0;\
                 if(GetDSPStateLight(light)) ClrDSPStateLight(light);\
		   else SetDSPStateLight(light);}


// [[ �岹����
//#define INTERP_MOTION_JERK     (10000.0f)     // �Ӽ��ٶ�
#define INTERP_JOINT_DEGREE        4     // �ؽڲ岹����Degree
#define INTERP_FIFO_LENTH          300//240//120//60//24//12// 6// �岹����г���
#define INTERP_JOINT_MAXVELPCT  (0.95f)     // �ؽ� �岹����ٶȰٷֱ�, 95%
#define INTERP_TEACH_MAXVELPCT  (0.10f)     // ʾ������ٶȰٷֱ�, 20%
#define INTERP_TEACH_MAXCVEL     (250.0f)     // �ѿ���ʾ������ٶ�, mm/s
#define INTERP_TEACH_MAXGVEL      (10.0f)     // �ѿ���ʾ�������̬�ٶ�, deg/s
#define INTERP_ZONE_GRADE          4     // ���ɶ����ȼ�
#define INTERP_ZONE_ACCPCT       1.0f     // ���ɶμ��ٶȰٷֱ�, 100%
#define INTERP_SINGULARITY_ZONE  5.0f     // ���������С, deg
#define JOINT_CART_TRANSF_EN	   0	 // �ؽ���ѿ������ɿ���
// ]] 

// [[ ϵͳ����
#define MOF           7


#define INTERP_CNT_RATIO   2 //250us
#define INTERP_PULSE_RATIO   (26)//13*INTERP_CNT_RATIO
#define maxlenperperiod 0.1  //0.2/INTERP_CNT_RATIO
#if PROGRAM_VERSION==RELEASE_MODE||PROGRAM_VERSION==OFFLINE_TEST_MODE
#define INTERP_T	   0.00025f     // �岹����,s
#define POS_T          0.00025f     // λ������,s
#else
#define INTERP_T       0.004f     // (test)�岹����,s
#define POS_T          0.004f     // (test)λ������,s
#endif
#define RT_LITTLE    0.000001f    // ϵͳ��ʶ�����С����
#define SERR          0.00001f    // �������Χ
#define POS_SERR     0.001f// 0.00036f    // �ؽ�λ���������Χ, 1cts - 0.00036(���ٱ�100)
#define POS_PRECISION       1    // ��λ�þ���, �ŷ����
#ifndef PI
#define PI      3.1415926535898f  // Բ����
#endif
#ifndef PI2
#define PI2     6.2831853071796f  // 2��Բ����  
#endif
#define PI_RAD  0.0174532925199f  // �Ƕ�ת��Ϊ���Ȳ���
#define PI_DEG 57.2957795130823f  // ����ת��Ϊ�ǶȲ���
// ]]
#ifndef ERR_STOP_T
#define ERR_STOP_T      0.08f//  1.000 //��s
#endif



// [[ �˶�״̬
#define RUN_MODE_TEACH     1     // ʾ��ģʽ
#define RUN_MODE_AUTO      2     // ����ģʽ
#define RUN_MODE_ERR       0     // ����ģʽ
#define RUN_DIR_POS        1     // ������
#define RUN_DIR_STOP       0     // �޷���
#define RUN_DIR_NEG       -1     // ������
#define RUN_STATE_RUNNING  1     // �����˶�
#define RUN_STATE_STOP     0     // ֹͣ
#define RUN_STOPMODE_HOLD  0     // ��ͣ
#define RUN_STOPMODE_STOP  1     // ֹͣ
#define RUN_LIM_POS        1     // ������
#define RUN_LIM_NOT        0     // �޼���
#define RUN_LIM_NEG       -1     // ������
#define RUN_SIN_YES        1     // ����״̬
#define RUN_SIN_NO         0     // ������

#define TEACH_STOP 1
#define TEACH_RUN  2
// ]]

// [[ ����ϵ
// [[ ʾ������ϵ - �ٶ�ģʽ/λ��ģʽ
#define COORDINATE_JOINT_VEL   1    // �ؽ�����ϵ
#define COORDINATE_WORLD_VEL   2    // ��������ϵ
#define COORDINATE_TOOL_VEL    3    // ��������ϵ
#define COORDINATE_USER_VEL    4    // �û�����ϵ

	#define COORDINATE_TABLE_VEL   5    // ת̨����ϵ
#define COORDINATE_JOINT_POS  11    // �ؽ�����ϵ
#define COORDINATE_WORLD_POS  12    // ��������ϵ
#define COORDINATE_TOOL_POS   13    // ��������ϵ
#define COORDINATE_USER_POS   14    // �û�����ϵ
	#define COORDINATE_TABLE_POS  15    // ת̨����ϵ
// ]]

// [[ ��״̬
#define MOTOR_ENABLE       1     // ʹ��
#define MOTOR_DISABLE      0     // ��ʹ��
// ]]


// [[ �������
#define PROG_MODE_MOVE         1     // Move
#define PROG_MODE_IO           2     // IO
#define PROG_MODE_DELAY        3     // Delay
#define PROG_MODE_NOTHING      4     // �ǳ�����
#define PROG_MODE_WAIT      5    // Wait
#define PROG_MODE_WELD      10    // ����
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


#define PROG_DsorDe_DisDs      1    // ����
#define PROG_DsorDe_DisDe      2    // ����
#define PROG_DsorDe_DisDsSlow      3    // ����
#define PROG_DsorDe_DisDeSlow      4   // ����
#define PROG_DsorDe_TimeDs      8   // ����
#define PROG_DsorDe_TimeDe      9    // ����
#define PROG_DsorDe_TimeDsSlow     10    // ����
#define PROG_DsorDe_TimeDeSlow      11   // ����

#define PROG_INTERP_JOINT      1     // �ؽڲ岹
#define PROG_INTERP_LINEAR     2     // ֱ�߲岹
#define PROG_INTERP_ARC        3     // Բ���岹
#define PROG_INTERP_CIRCLE     4     // ��Բ�岹
#define PROG_INTERP_PARABOLA   5     // �����߲岹 
#define PROG_INTERP_QUINTIC    6     // ��ζ���ʽ�岹

#define PROG_VELMODE_EULER     1  // ŷ�����ٶ�ģʽ
#define PROG_VELMODE_ANGLE     2  // ���ٶ�ģʽ

#define PROG_MARK_MIDLINE      0     // �м���
#define PROG_MARK_FIRSTLINE    0     // ����
#define PROG_MARK_LASTLINE     3     // ĩ��
#define PROG_MARK_SINGLELINE   1     // ����
#define PROG_MARK_CONTINUE	   2     // ����
#define PROG_COMM_CANREAD      2     // ����ɶ�
#define PROG_COMM_READOVER     3     // ���������
#define PROG_RUN_NOTSTART      0     // δ����
#define PROG_RUN_ON            1     // ��������
#define PROG_RUN_OVER          2     // �˶�����
#define PROG_RUN_STOP          3     // �˶�ֹͣ
#define PROG_RUN_ERROR        -1     // �˶�����

#define PROG_PLAN_NODATA       1     // ������
#define PROG_PLAN_WAITING      2     // �ȴ�
#define PROG_PLAN_PROCESSING   3     // �������ڹ滮
#define PROG_PLAN_COMPLETED    4     // ����滮���

#define PROG_TRANSMODE_YES     1     // ���ɶ�
#define PROG_TRANSMODE_NO      0     // �ǹ��ɶ�
// ]]

// [[״̬�����궨��
#define STATE_COMPLETED			 0		// ���봦�����
#define STATE_BLOCKCOMPLETED	 1		// ���������
#define STATE_FILECOMPLETED		 2		// �����ļ����
#define STATE_PROCESSING		 3		// �������ڴ���
#define STATE_WAITING			 4		// �ȴ����ش����ļ�
#define STATE_FEEDHOLD			 5		// ��ͣ
#define STATE_RESET			     8		// ��λ
#define STATE_WARNING            9      // ����
#define STATE_NOHOME            10      // �ǻ���
#define STATE_DECPROCESS		11      // 
// ]]


// IO���
#define AOUT_VOLT_MAX			10
#define AIN_VOLT_MAX			10
#define ANALOG_PREC				4096	// 10λ����


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

#define SYSTEM_OK    0   // ϵͳ����
#define SYSTEM_ERR  -1   // ϵͳ����

#ifndef SYSTEM_TRUE
#define SYSTEM_TRUE  1
#endif
#ifndef SYSTEM_FALSE
#define SYSTEM_FALSE 0
#endif
#define Square(n)   ((n) * (n))              // ƽ��
#define Cube(n)     ((n) * (n) * (n))        // ����

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


// [[ �ؽڿռ�  param0-15
//U8 m_pJointControl;// �ؽڲ�������λ     
#define pJointMaxAcc 0 // �ؽ����������ٶ�, deg/s*s param1-2
#define pJointMaxVel 6// �ؽ�����ٶ�
#define pJointStopDec 12// �ؽ�ֹͣ���ٶ�, deg/s*s
#define pJointPLim    18// �ؽ�������, deg
#define pJointNLim  24// �ؽڸ�����, deg  param13-15
 //]]



// [[ ��ռ� ,�������
//U8 m_pMotorControl ; // ���������λ       param16-31
#define pMotorRatio 30    // ����ٱ�
#define pMotorAbsZero 36   // ��������, ���ת��
#define pMotorPrec 42                 // �ᾫ��, cts/round
#define pMotorDir 48 
// ]]

#define pZoneLevel 50 

// [[ �ѿ����ռ� param32-63
//U8 m_pCarControl; // �ѿ�����������λ
#define pAccTime 64// ����ʱ��, ms
#define pCarMaxPVel 65 // �ѿ���λ������ٶ�, mm/s
#define pCarMaxGVel 66 // �ѿ�����̬����ٶ�, deg/s
#define pCarMaxPAcc 67 // �ѿ���λ�ü��ٶ�
#define pCarMaxGAcc 68 // �ѿ�����̬���ٶ�
#define pCarStopPDec 69 // �ѿ���λ��ֹͣ���ٶ�
#define pCarStopGDec 70 // �ѿ�����ֹ̬ͣ���ٶ�
//]]

#define pK1    72   //y1 PID����
#define pKp1    73	//y1 P����
#define pKi1    74	//y1 i����
#define pKd1    75//y1 d����
#define pK2    76	//y2 PID����
#define pKp2    77	//y2 P����
#define pKi2    78	//y2 i����
#define pKd2    79//y2 d����

#define pLineMaxLen    80	//ֱ�߳��ȷֶγ���
#define pLineMaxVel    81	//ֱ�߳��ȷֶ��ٶ�
#define pLineMaxPer   82	//ֱ�߳��ȷֶ�ʱ�����ٶȱ�
#define pLineMinPer    83	//ֱ�߳��ȷֶ�ʱ��С���ٶȱ�
#define pArcRadMaxLen    84	//Բ�����ȷֶΰ뾶
#define pArcLenMaxLen    85	//Բ�����ȷֶγ���
#define pArcMaxVel    86	//Բ�����ȷֶ��ٶ�
#define pArcMaxPer    87	//Բ�����ȷֶ�ʱ�����ٶȱ�
#define pArcMinPer    88	//Բ�����ȷֶ�ʱ��С���ٶȱ�
#define pArcError    89	//����Բ��������

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

#define GoHomeOrder1   138   // �ֶ� ����˳��, ����1.  Ĭ��ֵ 3. �Ȼ�Z��
#define GoHomeOrder2   139   // �ֶ� ����˳��, ����2.  Ĭ��ֵ 1.
#define GoHomeOrder3   140   // �ֶ� ����˳��, ����3.  Ĭ��ֵ 2.
#define GoHomeOrder4   141   // �ֶ� ����˳��, ����4.  Ĭ��ֵ 4.

//#define GoHomeOrder1   120   // �ֶ� ����˳��, ����1.  Ĭ��ֵ 3. �Ȼ�Z��
//#define GoHomeOrder2   121   // �ֶ� ����˳��, ����2.  Ĭ��ֵ 1.
//#define GoHomeOrder3   122   // �ֶ� ����˳��, ����3.  Ĭ��ֵ 2.
//#define GoHomeOrder4   123   // �ֶ� ����˳��, ����4.  Ĭ��ֵ 4.

//#define pAIXS5PID   124
//#define pAIXS6PID  130
//#define pAIXS7PID   136

#define ep_CoorID   142
#define pJointMaxAcc7 166 // �ؽ����������ٶ�, deg/s*s param1-2
#define pJointMaxVel7 167// �ؽ�����ٶ�
#define pJointStopDec7 168// �ؽ�ֹͣ���ٶ�, deg/s*s
#define pJointPLim7    169// �ؽ�������, deg
#define pJointNLim7  170// �ؽڸ�����, deg  param13-15
#define pMotorRatio7 171    // ����ٱ�
#define pMotorAbsZero7 172   // ��������, ���ת��
#define pMotorPrec7 173                 // �ᾫ��, cts/round
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
#define PULSE_OVER_TIME 3000//30000����=30mm

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
    int Err_Comm; // ͨѶ������
    int Err_Plan; // �滮����
    int Err_Run;  // ���д���
    int Err_Lim;  // ����
    int Alm_Ser;  // �ŷ�����
    int OTC_CIA;  // ������
}AlarmInformStruct;
#endif

typedef struct _velPctStruct
{
	bool Enable;
	
	double P1;    // �׵�
	double P2;    // ĩ��
	double A1; // ���ٶμ��ٶ�
	double V1; // ���ٶ�
	double T1; // ���ٶ�ʱ��
	double T2; // ��ʱ��

	double time;  // ��ǰʱ��
	double vel;  // ��ǰ�ٶ�
	double pos;  // ��ǰλ��

}VelPctStruct;

typedef struct _velCtrlStruct
{
	int Mode; // ģʽ, 1�� | 0ֹͣ | -1��

	double Pos; // ��ǰλ��, [0,1]
	double Vel; // ��ǰ�ٶ�, 
}VelCtrlStruct;

// λ�˾���ṹ
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
	U8  sendframeno;//�������Է���֡���
	U8  recvframeno;//�յ���ȷ�������Ե�֡���
	U8  sendtrafficctrl;//������֡��Ҫ���͵��������ֽ�
	U8  recvtrafficctrl;//�յ���������֡�е������ƶ���	
	U8  succedrecvno;//�յ����������ݷ����ĶԷ����յ���֡���
	U8  sendstate;//���ͷ�����������������״̬��
  volatile    U8  overtime;//��ʱ
	U8  periodovertime;
	U16  programline;//λ��/����֡�е��к�
	U16  currentpoint;//λ��/����֡�еĵ����
	U16  periodctrlword;//Ҫ����������֡�еĿ�����
	U16  periodstateword;//���յ���������֡�е�״̬��
	U8  control;//����֡�еĿ�����
	U8  resendno;//�ط������
	U8  state;//״̬֡�е�״̬��
	U8  alarmno;//������
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





// �켣�������
#ifndef _TRAJPOINTPARA_
#define _TRAJPOINTPARA_
typedef struct _trajPointPara
{
	double Acc; // ���ٶ�
	double Vel; // �ٶ�
	double Len; // ����
}TrajPointPara;
#endif

// �켣����������
typedef struct _spdPTrajInputData
{
	double Len;  // �켣�γ���
	double Vs;   // ��ʼ�ٶ�
	double V;    // �����ٶ�
	double Ve;   // �յ��ٶ�
	double Acc;  // �����ٶ�
	double Jerk; // ���Ӽ��ٶ�
	double T_Acc;// ����ʱ��
}SpdPTrajInputData;

// �켣���������
typedef struct _spdPTrajData
{
	double Len;  // �켣�γ���
	double Vs;   // ��ʼ�ٶ�
	double V;    // �����ٶ�
	double Ve;   // �յ��ٶ�

	double Jacc;
	double Jdec;

	double T1;   // ����ʱ��
	double T2;
	double T3;
	double T4;
	double T5;
	double T6;
	double T7;
	
	double T;    // �滮��ʱ��

	double V1;   // ���ζ˵��ٶ�
	double V2;
	double V3;
	double V4;
	double V5;
	double V6;

	double L1;  // ���ζ˵�λ��
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
    double S;      // ��Sת��,deg
    double L;      // ��Lת��,deg
    double U;      // ��Uת��,deg
    double R;      // ��Rת��,deg
    //double B;      // ��Bת��,deg
    //double T;      // ��Tת��,deg
}JointPose;
#endif




#ifndef CARTESIANPOSE
#define CARTESIANPOSE

typedef struct _cartesianPose
{
    double X;     // λ��X,mm
    double Y;     // λ��Y,mm
    double Z;     // λ��Z,mm
    double W;     // ��̬W,deg
    double P;     // ��̬P,deg
    double R;     // ��̬R,deg
    double M;     // ��̬M,deg
}CartesianPose;
#endif




#ifndef INPUTAUTODATA
#define INPUTAUTODATA
// ϵͳ�����������ݽṹ
typedef struct _inputAutoData
{
    unsigned int decode_id;
    unsigned int Prog_SequNum;     // ��ǰ����α��	//24
    unsigned int Prog_LineNum;     // ��ǰ�����б��     //28
    unsigned int Prog_Mark;        // �����ʶ 0:�����һ��  1:���л����һ��//32
    // ]]

    unsigned int  Mode;             // ������ģʽ, �˶���-1 | IO-2 | Delay-3 | �ǳ�����-4 |Wait-5|����-10  //36

    // [[ �˶�����Ϣ//
    unsigned int  Interp_Mode;      // �岹��ʽ - MOVJ 1 | MOVL 2 | Բ�� 3 | ��Բ 4   //40

    int Joint_VelPct;     // �ؽڲ岹�ٶȰٷֱ�   //44
    int PathSN;			  // Բ��ʱָ���ǵ�һ�λ��ǵڶ��Σ�0--���Σ�1--��һ�Σ�2--�ڶ���//48
	
    double JPos_Start[MOF]; // ��ʼ��, Բ���岹ʱ�ã���ȷ��ֵ��  //�ռ�仯
    double JPos_Mid[MOF];   // �м��, Բ��/�����߲岹ʱ��  //�ռ�仯
    double JPos_End[MOF];   // Ŀ���  //�ռ�仯
    double Car_PVel;      // �ѿ����岹λ���ٶ�, 0-Ĭ��//200
    double Car_GVel; 	  //208
    int Zone;             // ���ɵȼ�, 0,1,2,3,4   //216

    // [[ IO����Ϣ
    int IO_Mode;          // 1-���� | 2-��� 3-ģ������ 4-ģ�����//244
    int IO_Name;          // IO���//248
    int IO_Value;         // IOֵ, 1-true | 0-false//252
    // ]]
    int WaitTime;         // �ȴ�ʱ��, ms//256
}InputAutoData;
#endif

#ifndef INPUTTEACHDATA
#define INPUTTEACHDATA
// ����ʾ�����ݽṹ����
typedef struct _inputTeachData
{
    int Stop; 	// ʾ��״̬ stop-1 run - 2
    int Coor; 	// �ٶȷ�ʽ���ؽ�--1��ֱ��--2������--3������--4��������ʽ���ؽ�--11��ֱ��--12������--13������--14
    int Aix;  	// ��1-6
    int Vel;  	// �ٶȰٷֱ�
    int Dir;  	// ���� -1/1 
    int Acc;  	// ���ٶ�
	double Pos;	// ����ʾ����λ�Ƴ��ȣ��Ƕ�����λ��deg��ƽ������λ��mm
}InputTeachData;
#endif

// ʾ�̽ṹ����
typedef struct _teachStruct
{
	//------ ���� ------//
	double Interp_T;          // �岹����
	int Tacc;                 // ����������   - �ؽ� | �ѿ���
	int Flag[MOF];       // �����˶�״̬ - JC
	int Dir[MOF];        // ����ʾ�̷��� - JC
	int Time[MOF];       // �������ڼ��� - JC,//�Ӵ���С�� +pyc
	double Vel[MOF];     // ���������ٶ� - JC

	//---- ����ʾ�� ----//
	double T_Acc;
	double Total_T[MOF]; // �˶���ʱ��
	double Start_Pos[MOF];  /*  ��ʼλ��	+pyc*/
	double Tar_Pos[MOF]; // Ŀ��λ��

	double Act_T[MOF];   // ��ǰʱ��

	//------ ���� ------//
	double Jerk[MOF];    // �Ӽ��ٶ� - JC
	double Act_Acc[MOF]; // ���ٶ�   - JC
	double Act_Vel[MOF]; // �����ٶ� - JC
	double Act_Pos[MOF]; // ����λ�� - JC

	//------ ��� ------//
	double O_Acc[MOF];   // ������ٶ� - JC
	double O_Vel[MOF];   // ����ٶ�   - JC
	double O_Pos[MOF];   // ���λ��   - JC

	// �ѿ�������ϵʾ��ר��
	double Curr_Vel[MOF];// ��ǰ�ٶ� - C
	double Curr_Pos[MOF];// ��ǰλ�� - C
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
    int Err_Comm; // ͨѶ������
    int Err_Plan; // �滮����
    int Err_Run;  // ���д���
    int Err_Lim;  // ����
    int Alm_Ser;  // �ŷ�����
    int OTC_CIA;  // ������
}AlarmInformStruct;
#endif

typedef struct _alarmQueue
{
    int Err_Num;                         //  ?�̨�3����?����y
    int Err_Number[10];  // ?�̨�3����?��o?
}AlarmQueue;

#ifndef PROGRAMINFORMSTRUCT
#define PROGRAMINFORMSTRUCT
// ����״̬
typedef struct _programInformStruct
{
	int Prog_State; // 1. �˶�����ǰ�����˶�״̬, 0������, 1��������, 2�˶�����, ����-�˶�����
	                // 2. �滮��0����, ����-�滮�����кźͶκ�ֻ�ڹ滮����ʱ��Ч
	int CurrLine_State;	//��ǰ��״̬, 0 ������, 1 ��������, 2 �˶�����, ����-�˶�����
    // ������
    // ��ʾ�õ�
    //char name[20];
	
    // ����Ĳ����
    // ʵʱͬ���õ�
    unsigned int decode_id ;
	int Prog_SequNum;// ��������κ�
	int Prog_LineNum;// ���������к�
	int IfTransSeg;  // �Ƿ��ǹ��ɶ�
}ProgramInformStruct;

#endif

#ifndef PROGRAMRUNSTRUCT
#define PROGRAMRUNSTRUCT
// ����״̬
typedef struct _programRunInformStruct
{
   	 // ʵʱͬ���õ�
	double time;      //�ù켣������ʱ��
	double totaltime;//�ù켣������ʱ��
	double len; //�ù켣�����г���
	double totallen;//�ù켣�����ܳ���
	
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
	U32 DigitalSignal[2];	//64λIO		       
	U16 Control;//����λ��1-ARM IOָ����Ч 2-DSP��ȡ����Ϊ2
}IoArmCtrl;
#endif


// DSP��ʱ������λ-POS_T�岹����
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
// ���������˲����ṹ
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
// �ѿ�������ϵ��ʾ
typedef struct _cartesianPose
{
	double X;     // λ��X,mm
	double Y;     // λ��Y,mm
	double Z;     // λ��Z,mm
	double W;     // ��̬W,deg
	//double P;     // ��̬P,deg
	//double R;     // ��̬R,deg
}CartesianPose;
#endif

#ifndef JOINTPOSE
#define JOINTPOSE
// �ؽ�����ϵ��ʾ
typedef struct _jointPose
{
	double S;      // ��Sת��,deg
	double L;      // ��Lת��,deg
	double U;      // ��Uת��,deg
	double R;      // ��Rת��,deg
	//double B;      // ��Bת��,deg
	//double T;      // ��Tת��,deg
}JointPose;
#endif



#ifndef PROGRAMINFORMSTRUCT
#define PROGRAMINFORMSTRUCT
// ����״̬
typedef struct _programInformStruct
{
	int Prog_State; // 1. �˶�����ǰ�����˶�״̬, 0������, 1��������, 2�˶���, ����-�˶�����
	                // 2. �滮��0����, ����-�滮�����кźͶκ�ֻ�ڹ滮����ʱ��Ч
	int CurrLine_State;	//��ǰ��״̬, 0 ������, 1 ��������, 2 �˶�����, ����-�˶�����
    // ������
    // ��ʾ�õ�
    //char name[20];
	
    // ����Ĳ����
    // ʵʱͬ���õ�
    unsigned int decode_id;
	int Prog_SequNum;// ������κ�
	int Prog_LineNum;// ���������к�
	int IfTransSeg;  // �Ƿ��ǹ��ɶ�
}ProgramInformStruct;
#endif



typedef struct _globalState
{
/////////////////////////////////////////////////////////////////////// ��������
	int Acc_Time;              // ����ʱ��, ms

// [[ �ѿ���
	double Car_MaxPVel;          // �ѿ���λ������ٶ�, mm/s
	double Car_MaxGVel;          // �ѿ�����̬����ٶ�, deg/s
	double Car_MaxPAcc;          // �ѿ���λ�ü��ٶ�
	double Car_MaxGAcc;          // �ѿ�����̬���ٶ�
	double Car_StopPDec;         // �ѿ���λ��ֹͣ���ٶ�
	double Car_StopGDec;         // �ѿ�����ֹ̬ͣ���ٶ�
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
	//LinkDimension Robot_Link;    // ���˲���
	//CartesianPose Robot_Tool;    // ��������ϵ����
	//CartesianPose Robot_User;    // �û�����ϵ����
// ]] �ѿ���

// [[ �ؽڿռ�
	double Joint_MaxVel[MOF];    // �ؽ�����ٶ�����, mm/s*s
	double Joint_StopDec[MOF];   // �ؽ�ֹͣ���ٶ�, mm/s*s
	double Joint_PLim[MOF];      // �ؽ�������, mm
	double Joint_NLim[MOF];      // �ؽڸ�����, mm
	double Joint_MaxAcc[MOF];    // �ؽ������ٶ�, mm/s*s
// ]] �ؽڿռ�

// [[ ��ռ�
	double Motor_Ratio[MOF];     // ����ٱ�
	double Motor_AbsZero[MOF];   // ��������, ���ת��
	double Motor_Precision[MOF];    // �ᾫ��, cts/round
	int Motor_Dir[MOF];          // �᷽��, "1"-���������ؽڷ���һ��, "-1"-��һ��
// ]] ��ռ�

/////////////////////////////////////////////////////////////////////// ��������
	CartesianPose Car_ActPos; // ʵ��λ��
	double Joint_ActPos[MOF]; // �ؽ�ʵ��λ��
	double Joint_ActVel[MOF]; // �ؽ�ʵ���ٶ�
	double Motor_ActCurr[MOF];// ��ʵ�ʵ���
	double Motor_ActPos[MOF]; // ��ʵ��ת��
	double Joint_CurrentPos[MOF]; // ·���滮�ĵ�ǰλ��
	double Joint_CalculPos[MOF];
	double Car_ActVel[MOF];   // �ѿ���ʵ���ٶ�

	short Joint_Dir[MOF];     // �ؽ��˶�����  +pyc:��ʾ�����
	short Flag_PLim[MOF];     // �ؽ�λ�ü��ޱ�־, ������(1)/�Ǽ���(0)/������(-1)
	short Joint_State[MOF];   // �ؽ��˶�״̬  +pyc:��ʾ�����
	short Flag_Enable[MOF];   // ʹ�ܱ�־  +pyc:��ʾ�����
	short Flag_VLim[MOF];     // �ؽ��ٶȼ��ޱ�־  +pyc:��ʾ�����
	//double Table_MaxVel;  //ת̨�����ϳ���ת�ٶ�,  deg/s
	//double Table_MaxAcc;  //ת̨�����ϳ������ٶ�,  deg/s.s
	//TableConfigT Table_ConfigT;   //ת̨����
	//int Table_Gt;

/////////////////////////////////////////////////////////////////////// ȫ��״̬
// [[ ����
	U16 Mode;           // ʾ��(1)/����(2) - // ARM����ֵ��1 �� 2
	U16 Stop;           // ֹͣ(1)         - // ARM����ֵ��1
	U16 Hold;           // ��ͣ(1)/����(0) - // ARM����ֵ��1 �� 0
	U16 AlarmClear;          // ��λ(1)         - // ARM����ֵ��1
	U16 EStop;          // ��ͣ(1)         - // ARM����ֵ��1
	U16 Abort;          // ȡ���˶�(1)     - // ARM����ֵ��1
	U16 MachineStop;    // ����(1)/���(0) - // ARM����ֵ��1 �� 0
	U16 Pct_AutoVel;    // �����˶��ٶȰٷֱ�, �����˶�����������
// ]] ����

// [[ ��ǰ״̬
	U16 MotionMode;     // ��ǰ�˶�ģʽ - ʾ��(1)/����(2)/����(0)
	U16 JogCoordinate;  // �ֶ�����ϵѡ�� - �ؽ�(1)/��(2)/����(3)/�û�(4)
	U16 In_Motion;      // �����˶���־(1)
	U16 Singularity;    // ����λ��(1)
	U16 MotorEnable;    // ��ʹ��(1)
// ]] ��ǰ״̬
	/* DSPState[0]-ֹͣ״̬��DSPState[1]-��ͣ״̬, DSPState[2]-����״̬*/
	char DSPState[4];
// [[ ����״̬
	ProgramInformStruct Prog_Run;  // ����ǰ�˶�״̬, �ڲ岹ʱ����
	ProgramInformStruct Prog_Plan; // ����ǰ״̬, �ڹ滮����ʱ����
// ]] ����״̬

// [[ ������Ϣ
	AlarmInformStruct Err;    // ������Ϣ, �ڳ���ִ�й����и���
	VelPctStruct* pVelPct;
}GlobalState;

typedef struct STRC_DelayCtrl
{
	int Req;	//������ʱ��־
	int DlyTime;//��ʱʱ��

}UD_DelayCtrl;

//IO�����ڲ�����
typedef struct STRC_DSPStateLightParam
{
	unsigned int TotleTimes;
	unsigned int BlinkTimes;
	unsigned int CurrTimes;
	unsigned int IsBlinkSrart;
}UD_DSPStateLightParam;

//���ӿ��ƽṹ
/*typedef struct STRC_WeldControl
{

       int WaitTime;         // �ȴ�ʱ��, ms//256
	// Mode == 10 ����
	// 1 ARCON
	// 2 ARCOF
	// 3 ARCCT 
	// 4 ARCSET
	// 5 AWELD
	// 6 VWELD
	int order ;      
	// ����
	double AC ;
	// ��ѹ
	double AVP ;
	// ʱ��
	double T ;
	// �ٶ�
	double V ;
	// 1 ds
	// 2 es
	int  dis_type;    
	double dis;

    double  CurrentAvp;
    double  CurrentAcc;
   // double  Vel;
    U32 IO_Mode;          // 1-���� | 2-���
    U32 IO_Name;          // IO���
    U32 IO_Value;         // IOֵ, 1-true | 0-false
}WeldControl;*/

//errorplan.h

// �켣��ʼ����
typedef struct _trajInitPara
{
	double Jerk; // �Ӽ��ٶ�
	double Acc;  // ���ٶ�
	double Vel;  // �ٶ�
	double Len;  // ����
}TrajInitPara;

// ����ֹͣ�岹��
#ifndef _TRAJPOINTPARA_
#define _TRAJPOINTPARA_
typedef struct _trajPointPara
{
	double Acc; // ���ٶ�
	double Vel; // �ٶ�
	double Len; // ����
}TrajPointPara;
#endif

// ����ֹͣ�ṹ��
typedef struct _errStopTrajStruct
{
	int Stop;      // ֹͣ��־
	
	double Interp_T; // �岹����
	
	double Time;   // ֹͣʱ��
	double T[MOF];   // 
	double Pos[MOF]; // ��ʼλ��
	double Vel[MOF]; // ��ʼ�ٶ�
	double Acc[MOF]; // ���ٶ�
	double Curr_T; // ��ǰʱ��
	
}ErrStopTrajStruct;

//interploate
#define PATH_BUF_MAX_LEN        34//17//6//5  // ·������������
#define TRAJECTORY_BUF_LEN_MIN  50//18//6  // �켣��������С����
#define TRAJECTORY_BUF_LEN_MAX 60//36//14//12//10  // �켣��������󳤶�

#ifndef ABS
#define ABS(a)		((a)>=0?(a):(-(a)))
#endif
#define Count(n)    (((n) + PATH_BUF_MAX_LEN) % PATH_BUF_MAX_LEN) // ·��������������
#define Cnt(n)    (((n) + TRAJECTORY_BUF_LEN_MAX) % TRAJECTORY_BUF_LEN_MAX)
//------------------------------- �ṹ���� ----------------------------------//
// MOVJ·������
typedef struct _jointPathFeature
{
	double JPos_Start[MOF];  // ���
	double JPos_Mid1[MOF];
	double JPos_Mid2[MOF];
	double JPos_End[MOF];    // ĩ��
	double JVel_Cons[MOF];   // ���ٶ�

	double Time;
	double ld_acctime;
}JointPathFeature;

// �ռ�����
typedef struct _vectorStruct
{
	double X;   // ����x
	double Y;   // ����y
	double Z;   // ����z
	double Con; // ����
}VectorStruct;

// �ѿ���·������
typedef struct _cartesianPathFeature
{
	double JPos_Start[MOF];   // ���ؽڽ�
	double JPos_Mid[MOF];     // Բ���м��ؽڽ�
	double JPos_End[MOF];     // ĩ��ؽڽ�

	CartesianPose CPos_Start; // ���λ��
	CartesianPose CPos_Mid;   // Բ���м��λ��
	CartesianPose CPos_End;   // ĩ��λ��

	double Car_PAcc;          // �߼��ٶ�
	double Car_PVel;          // �������ٶ�
	double PosLen;            // ·������, ֱ�߳��Ȼ�Բ������
	//double Car_GAcc;          // ��̬���ٶ�
	//double Car_GVel;          // ������̬�ٶ�
	//double GesAng;            // ��̬��
	bool Flag_Reverse;

	double Time;
	VectorStruct Line_Vector; // ֱ�ߵ�λ����, ����Con = 1
	double Angle_Vector[2];  //��ת��λ����
	double Car_Vec;      //�ѿ������ȷ���

	PoseMatrix   Arc_Matrix;  // Բ��UVW������ϵ����
	VectorStruct Arc_Centre;  // Բ�����꼰�뾶
	double       Arc_Radian;  // Բ���Ļ���
	double       Arc_MidLen;  // Բ���м���Ӧ�Ļ���
	double       Arc_Start;  //
	double       Arc_End;  // 
	CartesianPose CPos_Zone;   //������ʼλ��
	VectorStruct  PVel_Zone;   //������ʼ�ٶ�
	double  zonelen;	//���ɳ���
	int zone;//���ɵȼ�

}CartesianPathFeature;

// ·��
typedef struct _interpPathStruct
{
	InputAutoData Input;

	JointPathFeature  JointPath;
	CartesianPathFeature CartePath;
}InterpPathStruct;


// �켣
typedef struct _InterpTrajectoryStruct
{
	// ������
    // ��ʾ�õ�
    //char name[20];
	
    // ����Ĳ����
    // ʵʱͬ���õ�
    unsigned int decode_id ;

	//------------------- ����Ϣ --------------------//
	int Prog_SequNum;         // ��ǰ����α��	
	int Prog_LineNum;         // ��ǰ�����б��
	int Prog_Mark;            // �����־, 1-start/2-end/0-mid	
	int Mode;                 // ������ģʽ, �˶���-1 | IO-2 | Delay-3
	int Interp_Mode;          // �岹��ʽ - MOVJ 1 | MOVL 2 | MOVC 3 | MOVP 4
	int IfTransSeg;           // �Ƿ��ǹ��ɶ�, 1��/0��

	//------------------ �˶����� --------------------//
	double TotleTime;		  //��ǰ����ʱ��
	double Time;              // ��ʱ��

	double JPos_Start[MOF];   // ʼ��ؽڽ�
	double JPos_End[MOF];     // ĩ��ؽڽ�
	
// [[ �ؽڲ岹
	double JVel_Start[MOF];   // MOVJ �ؽڳ��ٶ�
	double JVel_End[MOF];     // MOVJ �ؽ�ĩ�ٶ�
// ]]

// [[ �ѿ����岹
	CartesianPose CPos_Start;   // ʼ��λ��
	CartesianPose CPos_End;     // ĩ��λ��
	PoseMatrix    Arc_Matrix;   // Բ��UVW������ϵ����
	VectorStruct  Arc_Centre;   // Բ�����꼰�뾶
	double        Arc_Len_Start;// Բ����ʼ�㻡��
	double   Arc_End;
	double   Arc_Start;
	SpdTrajStruct Traj_CPos;    // λ�ù켣, �ǹ��ɶ�ʹ��
	//SpdTrajStruct Traj_CGes;    // ��̬�켣
	bool          Ges_Reverse;  // ��̬����岹��־, 1���� | 0����
	double        CVel_Start[MOF];// λ�ó��ٶ�, ���ɶ�ʹ��
	double        CVel_End[MOF];  // λ��ĩ�ٶ�, ���ɶ�ʹ��

	double Car_Vec;      //�ѿ������ȷ���

	VectorStruct Line_Vector; // ֱ�ߵ�λ����, ����Con = 1
// ]]

	//------------------ �˶����� --------------------//
	// [[ IO����Ϣ
	int IO_Mode;          // 1-���� | 2-���
	int IO_Name;          // IO���
	int IO_Value;         // IOֵ, 1-true | 0-false
	// ]]
	int WaitTime;         // �ȴ�ʱ��, ms	
}InterpTrajectoryStruct;


// ·��������
typedef struct _path_Buffer
{
	InterpPathStruct Data[PATH_BUF_MAX_LEN];   
	U16 Start;
	U16 End;
	U16 Now;
	
	U16 Num;
}Path_Buffer;


// �켣������
typedef struct _trajectory_Buffer
{
	InterpTrajectoryStruct Data[TRAJECTORY_BUF_LEN_MAX];
	U16 Start;
	U16 End;
}Trajectory_Buffer;


// �ؽڿռ�岹�ṹ��
typedef struct _jointInterpStruct
{
	int Degree;     // �岹������ߵ���ߴ���
	double Pos0[MOF]; // ��һ������
	double Pos1[MOF]; // �ڶ���
	double Pos2[MOF]; // ������
	double Pos3[MOF]; // ���ĵ�
	double Pos4[MOF]; // ����� 
	int Num;        // ÿ���ڲ岹����
	int Count;      // �岹�����
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
	char name[20];	//������
	U32 decode_id;	//����ID
	U32	Mode;	//�˶���-1 | IO-2 | Delay-3 | �ǳ�����-4(�ȴ�)
	U32 Prog_SequNum;// ��������κ�
	U32 Prog_LineNum;// ���������к�
	U32 Prog_Mark;	 // �б�־λ
	U32 IfTransSeg;  // �Ƿ��ǹ��ɶ�
	U32	IfLastPoint; // �Ƿ����һ�㣬0 �����㣬1 �����㣬2 �����һ��
	int Error;         // �����״̬

	double Posval[MOF]; // ����λ��+ת̨2��λ��
	double Spdval[MOF]; // �����ٶ�+ת̨2��λ��

	U32 IO_Mode;          // 1-���� | 2-���
	U32 IO_Name;          // IO���
	U32 IO_Value;         // IOֵ, 1-true | 0-false
	
	U32 WaitTime;         // �ȴ�ʱ��, ms

	// Mode == 10 ����
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
	// ����
	double AC ;
	// ��ѹ
	double AVP ;
	// ʱ��
	double T ;
	// �ٶ�
	double V ;
	double Percent;
	int IfDout;       //�Ƿ������ѹ����
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
	U32 decode_id;	//����ID
	U16 Error;   // �����״̬
	U16 flag;//bit0-Prog_Mark bit1-IfTransSeg bit2-IfLastPoint bit3-IfDout
	U8  Mode;//�˶���-1 | IO-2 | Delay-3 | �ǳ�����-4(�ȴ�)|5-wait io input
	U8 IO_Mode;          // 1-���� | 2-���
	U8 IO_Name;          // IO���
	U8 IO_Value;         // IOֵ, 1-true | 0-false
	U32 WaitTime;         // �ȴ�ʱ��, ms
	U16 DoutSetBit;        //����IO����λ
	U16 DoutClrBit;        //����IO����λ
	U8 DOUT61VAL;
	U8 DOUT62VAL;
	U8 DOUT63VAL;
	U8 DOUT64VAL;
}ElemPoint;




typedef struct _queue
{
	ElemType *Queue;   // ָ��洢���е�����ռ�
	volatile U16 Front;// ����ָ�루�±꣩,����ָ�벻ָ��Ԫ��
	volatile U16 Rear; // ��βָ�루�±꣩
    U16 MaxSize;       // Queue����
}Queue;

// �켣�岹״̬
typedef struct _interpolationState
{
	bool Enable;    // �岹ʹ��״̬
	double Time;    // ��ǰ�岹ʱ��
	InterpTrajectoryStruct Traj; // ��ǰ�岹�켣��

	bool IfEndPoint;   // ĩ���־
	ElemType EndPoint; // ĩ��
	
}InterpolationState;



#endif

