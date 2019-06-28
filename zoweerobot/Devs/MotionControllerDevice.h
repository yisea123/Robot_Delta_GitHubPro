/**
Copyright (c)
@file MotionControllerDevice.h
@author zhou
@version 1.0
@date 2018/07/10
@brief MotionControllerDevice���͵Ķ��塣
*/

#ifndef MOTIONCONTROLLERDEVICE_H
#define MOTIONCONTROLLERDEVICE_H

#include <QObject>
#include "BaseDevice.h"
#include "TCPSocket.h"

#include "GlobalVariableType.h"
#include "EmulationControllerDevice.h"
#include "cemulationcanvs.h"

#include "movemotioncode.h"
#include "linemotioncode.h"
#include "arcmotioncode.h"
#include "circlemotioncode.h"
#include "iomotioncode.h"
#include "delaymotioncode.h"
#include "lasercraftmotioncode.h"
#include "waitiomotioncode.h"

#include "GlobalDefine.h"
#include "IOAction.h"
#include "DelayAction.h"

//#define _CNC_PARSE_ACTION_RECORD_DEBUG
//#define _CNC_NET_FIFO_RECORD_DEBUG
//#define   PC_SEND_COMMAND_PORT     60123
//#define   PC_RECV_COMMAND_PORT     60124

//#define   PC_SEND_COMMAND_PORT     7580
//#define   PC_RECV_COMMAND_PORT     7575


//#define   CONTROLLER_SERVER_IP     "192.168.1.42"   //"172.168.1.123"//


#define BIG_ENDIAN 0
#define LITTLE_ENDIAN 1
#define ENDIANNESS BIG_ENDIAN

#define FUNCTION_CODE   2
#define FUNCTION_PARM_1 3
#define FUNCTION_PARM_2 4
#define FUNCTION_PARM_3 5
#define FUNCTION_PARM_4 6

#define  MSG_FUNCTION_CLEAR_FIFO     1
#define  MSG_FUNCTION_ADD_FIFO       2
#define  MSG_FUNCTION_SET_PARAM      10
#define  MSG_FUNCTION_SEND_NOTIFY    20
#define  MSG_FUNCTION_SEND_COMMAND   21
#define  MSG_FUNCTION_ASK_CURPOINT   22
#define  MSG_FUNCTION_GET_NOTIFY     30
#define  MSG_FUNCTION_GET_CURPOINT   31
#define  MSG_FUNCTION_GET_HEARTBEAT  32

#define  MSG_NORMAL_BOOT             1

typedef enum  _SystemState
{
    SYS_STETE_NORMAL,
    SYS_STATE_EMERGSTOP_ASK,    /**< ��ͣ���� */
    SYS_STATE_AXISALARM,        /**< �ᱨ�� */
    SYS_STATE_LIMIT,            /**< ��λ���� */
    SYS_STATE_IDENTIFY_DONE,    /**< ��ʶ��� */
    SYS_STATE_EMERGSTOP_DONE,   /**< ��ͣ��� */
    SYS_STATE_GOHOME_DONE,      /**< ��ԭ����� */
    SYS_STATE_DI_CHANGE         /**< DI�����仯 */
}SystemState;

class MotionControllerModule;
class SystemSchedule;
class CAutoMachiningStateUi;
class CManualMachiningStateUi;
class CCommunicationSettingUi;
class CNetSocket;
class CNetCtrl;


class MotionControllerDevice : public EmulationControllerDevice
{
    Q_OBJECT
public:
	
    MotionControllerDevice(QString s, BaseModule* m, SystemParameter* para);
    ~MotionControllerDevice();
	
    int startDevice(ErrorInfo& e);
    int pauseDevice(ErrorInfo& e);
    int openDevice(ErrorInfo& e);
    int stopDevice(ErrorInfo& e);
    int restartDevice(ErrorInfo& e);

    void setMachiningStateUi(CAutoMachiningStateUi* ui);
    void setManualMachiningStateUi(CManualMachiningStateUi* ui);
    void setCommunicationSettingUI(CCommunicationSettingUi* ui);

//    bool readDevice(char *recv, int& size);

    int creatTCPSocket();
	int creatUdPSocket();
	int SendNetReqByTCP(int num);
    int RecvNetDataByTCP(double *pos);
    int SaveXYZRPos(char *data,int num);
    int SendPTPByUDP();
    bool reConnectNet();
    bool closeNet();

    int runOneActions(const BaseAction* action, ProcessInterVariableCache* cache, SystemParameter* para, ErrorInfo& e);

bool getBattery();
bool setBattery();
bool setAxisNo();
bool getAxisNo();
int PutTeachPoint(unsigned short start,unsigned short end,unsigned char *paramdata);//(unsigned char start,unsigned char end,unsigned char *paramdata);
int PutTeachPointText(unsigned short start,unsigned short end,unsigned char *paramdata);
int PutSaveTeachPointText();
int PutSaveTeachPoint();
bool saveTeachPoint();
bool getTeachPoint();
int saveUserFile(int openflag,unsigned short start,unsigned short end);
    int fillOneFIFO(int* axisData, ErrorInfo& e);    /**< ���һ���λ�˶����ݵ�FIFO */
    int fillOneFIFO(int movetype,double* axisData, ErrorInfo& e);//�˿�1
    int fillOneFIFO(MoveAction * action,double* axisData, ErrorInfo& e);
    int fillOneFIFO(IOAction * action,ErrorInfo& e);
    int fillOneFIFO(DelayAction * action,ErrorInfo& e);
    int backHome();
    int movetoteachpoint(int num);
    bool setParam(char* paramValue, int paramNum, ErrorInfo& e);    /**< �������� */
    //bool sendMsg(char param1, ErrorInfo& e);    /**< �´��¼�֪ͨ */
    bool getMontionCount(ErrorInfo& e);    /**< ѯ������ɵ�λ�˶��� */
    bool getMsg(ErrorInfo& e);    /**< �ϴ��¼�֪ͨ */
    bool getMotionState(ErrorInfo& e);    /**< �ϴ���λ�˶�״̬ */
    bool getHeartBeat(ErrorInfo& e);    /**< �ϴ�����״̬ */
    bool getSts(int *sts, ErrorInfo& e);//��ѯ����������״̬
    bool clearSts(ErrorInfo& e);

    bool tcpSent();    /**< �������� */
    bool tcpReceive(ErrorInfo& e);    /**< �������� */
    bool tcpSentAndReceive(ErrorInfo& e);    /**< �������� */
    bool tcpMonitor(ErrorInfo& e);    /**< �������ݡ��������� */

    bool normalTurnOn(ErrorInfo& e);    /**< ��������֪ͨ */
    bool normalTurnOff(ErrorInfo& e);    /**< �����ػ�֪ͨ */
    bool emergencyStop(ErrorInfo& e);    /**< ��֪ͣͨ */
    bool clearEmergencyStop(ErrorInfo& e);    /**< �����֪ͣͨ */
    bool setServoEnable(bool sts, ErrorInfo& e);    /**< �ŷ�ʹ������ */

    bool emulationMotion();

    int getCurrentRunActionIndex();

    bool manualMove(int axis, double dis, ErrorInfo& e);
    bool manualMove(ErrorInfo& e, double x, double y, double z, double w, double p,double r);

    void initMotionControllerDevice();

    void setPunchLockSts(bool bSts);

    bool isTcpSendComConnect();

signals:
    void NetLedCtrl(bool);

public slots:
    void tcpMonitorData();

    void OnRecvSocketDisconnected();
    void OnSentSocketDisconnected();
    void OnRecvSocketError(QAbstractSocket::SocketError error);
    void OnSentSocketError(QAbstractSocket::SocketError error);

    bool clearFIFO(ErrorInfo& e);    /**< 清除FIFO */
    bool fillOneFIFO(MoveMotionCode* data, bool isEnd = false);    /**< 填充一组点位运动数据到FIFO */
    bool fillOneFIFO(LineMotionCode* data, bool isEnd = false, double vel = 100.0);    /**< 填充一组点位运动数据到FIFO */
    bool fillOneFIFO(ArcMotionCode* data, bool isEnd = false, double vel = 100.0);    /**< 填充一组点位运动数据到FIFO */
    bool fillOneFIFO(CircleMotionCode* data, bool isEnd = false, double vel = 100.0);    /**< 填充一组点位运动数据到FIFO */
    bool fillOneFIFO(IoMotionCode* data, bool isEnd = false);    /**< 填充一组点位运动数据到FIFO */
    bool fillOneFIFO(WaitIoMotionCode* data, bool isEnd = false);    /**< 填充一组点位运动数据到FIFO */
    bool fillOneFIFO(DelayMotionCode* data, bool isEnd = false);    /**< 填充一组点位运动数据到FIFO */
    bool fillOneFIFO(LaserCraftMotionCode* data, bool isEnd = false);
    bool runOneMotionCode(BaseMotionCode *data);
    bool addDelayFIFO(int id, int time);
    bool addMoveFIFO(int id, double x, double y);
    bool addMotionEndCode(int id);
    bool setParam(char* paramValue, int paramNum);    /**< 参数设置 */
    bool setParam(char start, char* paramValue, int paramNum);    /**< 工艺参数设置 */
    bool setMotionParam1(SystemParameter* para);//0~18
    bool setMotionParam2(SystemParameter* para);//22~37
    bool setMotionParamCtl(SystemParameter* para);//40~47
    bool sendMsg(char param1, ErrorInfo& e);    /**< 下传事件通知 */

    //手动运动指令
    bool startJogMotion(int axis, int dir, int vRate);    /**< 启动单轴定速运动 */
    bool stopJogMotion(int axis);    /**< 停止单轴定速运动 */
    bool startMotion(int axis, double dis, int vRate);    /**< 启动单轴定长运动 */
    bool startWorldMotion(int axis, double dis, int vRate);    /**< 启动单轴定长运动 */
    bool stopMotion(int axis);    /**< 停止单轴定长运动 */
    bool startMove(int axis, double dis, int run,int vRate);
    bool startWorldMove(int axis, double dis, int run,int vRate);
    bool isFifoNotFull();
    //控制帧--总操作
    bool setMotionMode(int mode);    /**< 设置运动模式，1--手动、2--程序运行（自动） */
    bool clearAlarm();    /**< 清除报警 */
    bool setOrian(int zeroaxis);
    bool getParamer();
    bool setParamer();
    bool setLinkParamer();
    bool setTools(int toolindex,int toolsetstep);
    bool setcurTool(int toolindex);
    bool setZeroCalibration(int step);
    bool axisEnable(bool isEnable);    /**< 使能 */
    bool getVersion();
    bool controllerReset();    /**< 复位 */
    bool emsStop(bool isStop);    /**< 急停 */
    bool getCtlVersion();    /**< 获取控制器版本号 */

    //控制帧--自动加工
    bool machineLock(bool isLock);    /**< 机械锁定（空走） */

    //非周期帧
    bool motionSpeedRate(int rate);    /**< 速度百分比(0~100) */
    bool holdMotion(bool ishold);    /**< 暂停/继续运动 */
    bool stopFifoMotion();    /**< 停止运动 */
    bool setOutputIo(int io, bool value);

    int PutAutoDataFrame(InputAutoData * autodata);
    int PutTeachDataFrame(InputTeachData *teachdata);
    int PutPeriodFrame(unsigned char *control,unsigned char *outputio);
    int PutControlFrame(unsigned char control,unsigned char value);
    int PutControlFrame(unsigned char control,unsigned char value,unsigned char value1);
    int PutSetParamFrame(unsigned char ifupdown,unsigned char start,unsigned char end,unsigned char *paramdata);	
    void printFifoData(InputAutoData * autodata);
    void onNetCtrlTimeout();
private:
    int initDevice(ErrorInfo& e);
    void fillIntParmToRequestFrame(QVector<unsigned char> &dataRequest, int intParm);
    void fillShortParmToRequestFrame(QVector<unsigned char> &dataRequest, short shortParm);
    int  charToInt(unsigned char c0, unsigned char c1, unsigned char c2, unsigned char c3);
    short charToShort(unsigned char c0, unsigned char c1);

    int  pointToPoint(MoveAction * action, ProcessInterVariableCache* cache, SystemParameter* para, ErrorInfo& e);
    int  LineMove(MoveAction * action, ProcessInterVariableCache* cache, SystemParameter* para, ErrorInfo& e);
	int OutIO(IOAction * action, ProcessInterVariableCache* cache, SystemParameter* para, ErrorInfo& e);
	int WaitIN(IOAction * action, ProcessInterVariableCache* cache, SystemParameter* para, ErrorInfo& e);
	int Delay(DelayAction * action, ProcessInterVariableCache* cache, SystemParameter* para, ErrorInfo& e);



    bool ctrlMonitorData();

    //void initPulseEquivalent(SystemParameter* para);
    void initRequestFrame();    /**< ��ʼ������֡ */
    void initRespondFrame_monitor();

    bool isPointInClampProtectArea(double x, double y, ProcessInterVariableCache* cache);
    
public:
	CNetSocket* m_socket;
	CNetCtrl* m_netctrl;
	SystemParameter* m_pSystemParm;
	QFile *m_userfile;
private:
    QVector<unsigned char> m_dataRequest;    /**< ����֡ */
    QVector<unsigned char> m_dataResponse;   /**< ��Ӧ֡ */
    QVector<unsigned char> m_dataRequest_monitor;    /**< ����֡ */
    QVector<unsigned char> m_dataResponse_monitor;   /**< ��Ӧ֡ */

    int m_nFIFOSpace;   /**< FIFOʣ��ռ� */

    char m_requestFunc;    /**< ����֡������ */
    char m_requestParam1;  /**< ����֡���ܲ���1 */
    char m_requestParam2;  /**< ����֡���ܲ���2 */
    char m_requestParam3;  /**< ����֡���ܲ���3 */
    char m_requestParam4;  /**< ����֡���ܲ���4 */

    char m_responseFunc;    /**< ��Ӧ֡��Ӧ�� */
    char m_responseParam1;  /**< ��Ӧ֡���ܲ���1 */
    char m_responseParam2;  /**< ��Ӧ֡���ܲ���2 */
    char m_responseParam3;  /**< ��Ӧ֡���ܲ���3 */
    char m_responseParam4;  /**< ��Ӧ֡���ܲ���4 */

    short m_nDI;    /**< ����IO״̬ */
    short m_nDO;    /**< ���IO���� */
    int m_nMotionCount;    /**< ����ɵ�λ�˶����� */
    char m_nHeartBreatCount;/**< ���������� */

    SystemState m_sysState;
    NAxis m_axisAlarm;    /**< �ᱨ������0ֵ��ʾ���� */
    NAxis m_axisLimit;    /**< ����λ������С��0��ʾ����λ����������0��ʾ����λ���� */

    int  m_nSts;

    RS_Vector m_OldPos;
    RS_Vector m_CurPos;
    unsigned int m_nMontionCtlState;
    unsigned int m_nOldMontionCtlSts;
    unsigned int m_nCurCtlRunId;
    unsigned int m_nOldCtlRunId;

    unsigned char m_cCurAlarmNum;
     union U64
    {
        unsigned char bit[8];
        unsigned long data;
    }m_ContrlBit, m_OutPutIo;

    unsigned char m_curRecvInputIo[8];
    unsigned char m_curRecvOutputIo[8];
    unsigned char m_oldRecvInputIo[8];
    unsigned char m_oldRecvOutputIo[8];

    double m_dCurY1Pos;
    double m_dOldY1Pos;
    double m_dCurY2Pos;
    double m_dOldY2Pos;
    double m_dCurDelYPos;
    double m_dOldDelYPos;
    double m_dCurVel;
    double m_dOldVel;
//    NAxis m_oldPositon;
//    NAxis m_newPosition;
//    NAxis m_curPosition;
    
private:
    TCPSocket    *m_PCSendCommandSocket;
    TCPSocket    *m_PCRecvCommandSocket;
    unsigned char         m_recvControllerData[TCP_BUFFER_SIZE];
    int          m_nRecvControllerDataNum;
    unsigned char         m_sendControllerData[TCP_BUFFER_SIZE];
    int          m_nSendControllerDataNum;

    unsigned char  m_sendMsgControllerData[TCP_BUFFER_SIZE];
    int            m_nSendMsgControllerDataNum;
    unsigned char  m_recvMsgControllerData[TCP_BUFFER_SIZE];
    int            m_nRecvMsgControllerDataNum;

//    float        m_dCurrentXPos;
//    float        m_dCurrentYPos;
//    ToolInfo*    m_currentTool;
//    int          m_toolNum;

    DAxis        m_currentPos;
    NAxis        m_nCurrentPosition;
    ToolInfo*    m_currentTool;
//double       m_currentToolAngle;    int          m_toolNum;

    int          m_currentRunActionIndex;   /**< ��ɵ�λ��������ֵ */
    int          m_currentSendActionIndex;  /**< �·���λ��������ֵ */
    bool         m_bisFinshOneAction;   /**< �����һ���λ��������Ϊtrue */

    //NAxis        m_pulseEquivalent;    /**< ���嵱������λpluse/mm */

    CAutoMachiningStateUi*  m_pMachiningUI;
    //CMachiningSimulationUi* m_pSimulationUI;
    CManualMachiningStateUi* m_pManualMachiningUI;
    CCommunicationSettingUi*  m_pCommunicationSettingUI;

    
    QTimer *m_NetCtrlTimer;

    bool m_bIsPunchLock;
    bool m_bisNetConnect;
    int  ifgetnetvalue;
    double scarapos[4];
    QMutex  mutex;
};

#endif // MOTIONCONTROLLERDEVICE_H
