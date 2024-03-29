/**
Copyright (c)
@file MotionControllerDevice.h
@author zhou
@version 1.0
@date 2018/07/10
@brief MotionControllerDevice类型的定义。
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
    SYS_STATE_EMERGSTOP_ASK,    /**< 急停请求 */
    SYS_STATE_AXISALARM,        /**< 轴报警 */
    SYS_STATE_LIMIT,            /**< 限位报警 */
    SYS_STATE_IDENTIFY_DONE,    /**< 辨识完成 */
    SYS_STATE_EMERGSTOP_DONE,   /**< 急停完成 */
    SYS_STATE_GOHOME_DONE,      /**< 回原点完成 */
    SYS_STATE_DI_CHANGE         /**< DI发生变化 */
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
    int fillOneFIFO(int* axisData, ErrorInfo& e);    /**< 填充一组点位运动数据到FIFO */
    int fillOneFIFO(int movetype,double* axisData, ErrorInfo& e);//端口1
    int fillOneFIFO(MoveAction * action,double* axisData, ErrorInfo& e);
    int fillOneFIFO(IOAction * action,ErrorInfo& e);
    int fillOneFIFO(DelayAction * action,ErrorInfo& e);
    int backHome();
    int movetoteachpoint(int num);
    bool setParam(char* paramValue, int paramNum, ErrorInfo& e);    /**< 参数设置 */
    //bool sendMsg(char param1, ErrorInfo& e);    /**< 下传事件通知 */
    bool getMontionCount(ErrorInfo& e);    /**< 询问已完成点位运动数 */
    bool getMsg(ErrorInfo& e);    /**< 上传事件通知 */
    bool getMotionState(ErrorInfo& e);    /**< 上传点位运动状态 */
    bool getHeartBeat(ErrorInfo& e);    /**< 上传心跳状态 */
    bool getSts(int *sts, ErrorInfo& e);//查询控制器运行状态
    bool clearSts(ErrorInfo& e);

    bool tcpSent();    /**< 发送数据 */
    bool tcpReceive(ErrorInfo& e);    /**< 接收数据 */
    bool tcpSentAndReceive(ErrorInfo& e);    /**< 接收数据 */
    bool tcpMonitor(ErrorInfo& e);    /**< 监视数据、如心跳等 */

    bool normalTurnOn(ErrorInfo& e);    /**< 正常开机通知 */
    bool normalTurnOff(ErrorInfo& e);    /**< 正常关机通知 */
    bool emergencyStop(ErrorInfo& e);    /**< 急停通知 */
    bool clearEmergencyStop(ErrorInfo& e);    /**< 清除急停通知 */
    bool setServoEnable(bool sts, ErrorInfo& e);    /**< 伺服使能命令 */

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

    bool clearFIFO(ErrorInfo& e);    /**< 娓呴櫎FIFO */
    bool fillOneFIFO(MoveMotionCode* data, bool isEnd = false);    /**< 濉厖涓�缁勭偣浣嶈繍鍔ㄦ暟鎹埌FIFO */
    bool fillOneFIFO(LineMotionCode* data, bool isEnd = false, double vel = 100.0);    /**< 濉厖涓�缁勭偣浣嶈繍鍔ㄦ暟鎹埌FIFO */
    bool fillOneFIFO(ArcMotionCode* data, bool isEnd = false, double vel = 100.0);    /**< 濉厖涓�缁勭偣浣嶈繍鍔ㄦ暟鎹埌FIFO */
    bool fillOneFIFO(CircleMotionCode* data, bool isEnd = false, double vel = 100.0);    /**< 濉厖涓�缁勭偣浣嶈繍鍔ㄦ暟鎹埌FIFO */
    bool fillOneFIFO(IoMotionCode* data, bool isEnd = false);    /**< 濉厖涓�缁勭偣浣嶈繍鍔ㄦ暟鎹埌FIFO */
    bool fillOneFIFO(WaitIoMotionCode* data, bool isEnd = false);    /**< 濉厖涓�缁勭偣浣嶈繍鍔ㄦ暟鎹埌FIFO */
    bool fillOneFIFO(DelayMotionCode* data, bool isEnd = false);    /**< 濉厖涓�缁勭偣浣嶈繍鍔ㄦ暟鎹埌FIFO */
    bool fillOneFIFO(LaserCraftMotionCode* data, bool isEnd = false);
    bool runOneMotionCode(BaseMotionCode *data);
    bool addDelayFIFO(int id, int time);
    bool addMoveFIFO(int id, double x, double y);
    bool addMotionEndCode(int id);
    bool setParam(char* paramValue, int paramNum);    /**< 鍙傛暟璁剧疆 */
    bool setParam(char start, char* paramValue, int paramNum);    /**< 宸ヨ壓鍙傛暟璁剧疆 */
    bool setMotionParam1(SystemParameter* para);//0~18
    bool setMotionParam2(SystemParameter* para);//22~37
    bool setMotionParamCtl(SystemParameter* para);//40~47
    bool sendMsg(char param1, ErrorInfo& e);    /**< 涓嬩紶浜嬩欢閫氱煡 */

    //鎵嬪姩杩愬姩鎸囦护
    bool startJogMotion(int axis, int dir, int vRate);    /**< 鍚姩鍗曡酱瀹氶�熻繍鍔� */
    bool stopJogMotion(int axis);    /**< 鍋滄鍗曡酱瀹氶�熻繍鍔� */
    bool startMotion(int axis, double dis, int vRate);    /**< 鍚姩鍗曡酱瀹氶暱杩愬姩 */
    bool startWorldMotion(int axis, double dis, int vRate);    /**< 鍚姩鍗曡酱瀹氶暱杩愬姩 */
    bool stopMotion(int axis);    /**< 鍋滄鍗曡酱瀹氶暱杩愬姩 */
    bool startMove(int axis, double dis, int run,int vRate);
    bool startWorldMove(int axis, double dis, int run,int vRate);
    bool isFifoNotFull();
    //鎺у埗甯�--鎬绘搷浣�
    bool setMotionMode(int mode);    /**< 璁剧疆杩愬姩妯″紡锛�1--鎵嬪姩銆�2--绋嬪簭杩愯锛堣嚜鍔級 */
    bool clearAlarm();    /**< 娓呴櫎鎶ヨ */
    bool setOrian(int zeroaxis);
    bool getParamer();
    bool setParamer();
    bool setLinkParamer();
    bool setTools(int toolindex,int toolsetstep);
    bool setcurTool(int toolindex);
    bool setZeroCalibration(int step);
    bool axisEnable(bool isEnable);    /**< 浣胯兘 */
    bool getVersion();
    bool controllerReset();    /**< 澶嶄綅 */
    bool emsStop(bool isStop);    /**< 鎬ュ仠 */
    bool getCtlVersion();    /**< 鑾峰彇鎺у埗鍣ㄧ増鏈彿 */

    //鎺у埗甯�--鑷姩鍔犲伐
    bool machineLock(bool isLock);    /**< 鏈烘閿佸畾锛堢┖璧帮級 */

    //闈炲懆鏈熷抚
    bool motionSpeedRate(int rate);    /**< 閫熷害鐧惧垎姣�(0~100) */
    bool holdMotion(bool ishold);    /**< 鏆傚仠/缁х画杩愬姩 */
    bool stopFifoMotion();    /**< 鍋滄杩愬姩 */
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
    void initRequestFrame();    /**< 初始化请求帧 */
    void initRespondFrame_monitor();

    bool isPointInClampProtectArea(double x, double y, ProcessInterVariableCache* cache);
    
public:
	CNetSocket* m_socket;
	CNetCtrl* m_netctrl;
	SystemParameter* m_pSystemParm;
	QFile *m_userfile;
private:
    QVector<unsigned char> m_dataRequest;    /**< 请求帧 */
    QVector<unsigned char> m_dataResponse;   /**< 响应帧 */
    QVector<unsigned char> m_dataRequest_monitor;    /**< 请求帧 */
    QVector<unsigned char> m_dataResponse_monitor;   /**< 响应帧 */

    int m_nFIFOSpace;   /**< FIFO剩余空间 */

    char m_requestFunc;    /**< 请求帧功能码 */
    char m_requestParam1;  /**< 请求帧功能参数1 */
    char m_requestParam2;  /**< 请求帧功能参数2 */
    char m_requestParam3;  /**< 请求帧功能参数3 */
    char m_requestParam4;  /**< 请求帧功能参数4 */

    char m_responseFunc;    /**< 响应帧响应码 */
    char m_responseParam1;  /**< 响应帧功能参数1 */
    char m_responseParam2;  /**< 响应帧功能参数2 */
    char m_responseParam3;  /**< 响应帧功能参数3 */
    char m_responseParam4;  /**< 响应帧功能参数4 */

    short m_nDI;    /**< 输入IO状态 */
    short m_nDO;    /**< 输出IO设置 */
    int m_nMotionCount;    /**< 已完成点位运动计数 */
    char m_nHeartBreatCount;/**< 心跳数计数 */

    SystemState m_sysState;
    NAxis m_axisAlarm;    /**< 轴报警，非0值表示报警 */
    NAxis m_axisLimit;    /**< 轴限位报警，小于0表示负限位触发、大于0表示正限位触发 */

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

    int          m_currentRunActionIndex;   /**< 完成点位数的索引值 */
    int          m_currentSendActionIndex;  /**< 下发点位数的索引值 */
    bool         m_bisFinshOneAction;   /**< 完成了一组点位数，设置为true */

    //NAxis        m_pulseEquivalent;    /**< 脉冲当量，单位pluse/mm */

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
