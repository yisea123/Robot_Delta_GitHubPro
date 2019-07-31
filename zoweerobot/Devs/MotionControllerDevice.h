/**
Copyright (c)
@file MotionControllerDevice.h
@author zhou
@version 1.0
@date 2018/07/10
@brief MotionControllerDevice¿‡–Õµƒ∂®“Â°£
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
    SYS_STATE_EMERGSTOP_ASK,    /**< º±Õ£«Î«Û */
    SYS_STATE_AXISALARM,        /**< ÷·±®æØ */
    SYS_STATE_LIMIT,            /**< œﬁŒª±®æØ */
    SYS_STATE_IDENTIFY_DONE,    /**< ±Ê ∂ÕÍ≥… */
    SYS_STATE_EMERGSTOP_DONE,   /**< º±Õ£ÕÍ≥… */
    SYS_STATE_GOHOME_DONE,      /**< ªÿ‘≠µ„ÕÍ≥… */
    SYS_STATE_DI_CHANGE         /**< DI∑¢…˙±‰ªØ */
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
    int fillOneFIFO(int* axisData, ErrorInfo& e);    /**< ÃÓ≥‰“ª◊Èµ„Œª‘À∂Ø ˝æ›µΩFIFO */
    int fillOneFIFO(int movetype,double* axisData, ErrorInfo& e);//∂Àø⁄1
    int fillOneFIFO(MoveAction * action,double* axisData, ErrorInfo& e);
    int fillOneFIFO(IOAction * action,ErrorInfo& e);
    int fillOneFIFO(DelayAction * action,ErrorInfo& e);

    void InitGoHome();
    int backHome(int &step);
    int movetoteachpoint(int num);
    bool setParam(char* paramValue, int paramNum, ErrorInfo& e);    /**< ≤Œ ˝…Ë÷√ */
    //bool sendMsg(char param1, ErrorInfo& e);    /**< œ¬¥´ ¬º˛Õ®÷™ */
    bool getMontionCount(ErrorInfo& e);    /**< —ØŒ “—ÕÍ≥…µ„Œª‘À∂Ø ˝ */
    bool getMsg(ErrorInfo& e);    /**< …œ¥´ ¬º˛Õ®÷™ */
    bool getMotionState(ErrorInfo& e);    /**< …œ¥´µ„Œª‘À∂Ø◊¥Ã¨ */
    bool getHeartBeat(ErrorInfo& e);    /**< …œ¥´–ƒÃ¯◊¥Ã¨ */
    bool getSts(int *sts, ErrorInfo& e);//≤È—Øøÿ÷∆∆˜‘À––◊¥Ã¨
    bool clearSts(ErrorInfo& e);

    bool tcpSent();    /**< ∑¢ÀÕ ˝æ› */
    bool tcpReceive(ErrorInfo& e);    /**< Ω” ’ ˝æ› */
    bool tcpSentAndReceive(ErrorInfo& e);    /**< Ω” ’ ˝æ› */
    bool tcpMonitor(ErrorInfo& e);    /**< º‡ ” ˝æ›°¢»Á–ƒÃ¯µ» */

    bool normalTurnOn(ErrorInfo& e);    /**< ’˝≥£ø™ª˙Õ®÷™ */
    bool normalTurnOff(ErrorInfo& e);    /**< ’˝≥£πÿª˙Õ®÷™ */
    bool emergencyStop(ErrorInfo& e);    /**< º±Õ£Õ®÷™ */
    bool clearEmergencyStop(ErrorInfo& e);    /**< «Â≥˝º±Õ£Õ®÷™ */
    bool setServoEnable(bool sts, ErrorInfo& e);    /**< À≈∑˛ πƒ‹√¸¡Ó */

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

    bool clearFIFO(ErrorInfo& e);    /**< Ê∏ÖÈô§FIFO */
    bool fillOneFIFO(MoveMotionCode* data, bool isEnd = false);    /**< Â°´ÂÖÖ‰∏ÄÁªÑÁÇπ‰ΩçËøêÂä®Êï∞ÊçÆÂà∞FIFO */
    bool fillOneFIFO(LineMotionCode* data, bool isEnd = false, double vel = 100.0);    /**< Â°´ÂÖÖ‰∏ÄÁªÑÁÇπ‰ΩçËøêÂä®Êï∞ÊçÆÂà∞FIFO */
    bool fillOneFIFO(ArcMotionCode* data, bool isEnd = false, double vel = 100.0);    /**< Â°´ÂÖÖ‰∏ÄÁªÑÁÇπ‰ΩçËøêÂä®Êï∞ÊçÆÂà∞FIFO */
    bool fillOneFIFO(CircleMotionCode* data, bool isEnd = false, double vel = 100.0);    /**< Â°´ÂÖÖ‰∏ÄÁªÑÁÇπ‰ΩçËøêÂä®Êï∞ÊçÆÂà∞FIFO */
    bool fillOneFIFO(IoMotionCode* data, bool isEnd = false);    /**< Â°´ÂÖÖ‰∏ÄÁªÑÁÇπ‰ΩçËøêÂä®Êï∞ÊçÆÂà∞FIFO */
    bool fillOneFIFO(WaitIoMotionCode* data, bool isEnd = false);    /**< Â°´ÂÖÖ‰∏ÄÁªÑÁÇπ‰ΩçËøêÂä®Êï∞ÊçÆÂà∞FIFO */
    bool fillOneFIFO(DelayMotionCode* data, bool isEnd = false);    /**< Â°´ÂÖÖ‰∏ÄÁªÑÁÇπ‰ΩçËøêÂä®Êï∞ÊçÆÂà∞FIFO */
    bool fillOneFIFO(LaserCraftMotionCode* data, bool isEnd = false);
    bool runOneMotionCode(BaseMotionCode *data);
    bool addDelayFIFO(int id, int time);
    bool addMoveFIFO(int id, double x, double y);
    bool addMotionEndCode(int id);
    bool setParam(char* paramValue, int paramNum);    /**< ÂèÇÊï∞ËÆæÁΩÆ */
    bool setParam(char start, char* paramValue, int paramNum);    /**< Â∑•Ëâ∫ÂèÇÊï∞ËÆæÁΩÆ */
    bool setMotionParam1(SystemParameter* para);//0~18
    bool setMotionParam2(SystemParameter* para);//22~37
    bool setMotionParamCtl(SystemParameter* para);//40~47
    bool sendMsg(char param1, ErrorInfo& e);    /**< ‰∏ã‰º†‰∫ã‰ª∂ÈÄöÁü• */

    //ÊâãÂä®ËøêÂä®Êåá‰ª§
    bool startJogMotion(int axis, int dir, int vRate);    /**< ÂêØÂä®ÂçïËΩ¥ÂÆöÈÄüËøêÂä® */
    bool stopJogMotion(int axis);    /**< ÂÅúÊ≠¢ÂçïËΩ¥ÂÆöÈÄüËøêÂä® */
    bool startMotion(int axis, double dis, int vRate);    /**< ÂêØÂä®ÂçïËΩ¥ÂÆöÈïøËøêÂä® */
    bool startWorldMotion(int axis, double dis, int vRate);    /**< ÂêØÂä®ÂçïËΩ¥ÂÆöÈïøËøêÂä® */
    bool stopMotion(int axis);    /**< ÂÅúÊ≠¢ÂçïËΩ¥ÂÆöÈïøËøêÂä® */
    bool startMove(int axis, double dis, int run,int vRate);
    bool startWorldMove(int axis, double dis, int run,int vRate);
    bool isFifoNotFull();
    //ÊéßÂà∂Â∏ß--ÊÄªÊìç‰Ωú
    bool setMotionMode(int mode);    /**< ËÆæÁΩÆËøêÂä®Ê®°ÂºèÔºå1--ÊâãÂä®„ÄÅ2--Á®ãÂ∫èËøêË°åÔºàËá™Âä®Ôºâ */
    bool clearAlarm();    /**< Ê∏ÖÈô§Êä•Ë≠¶ */
    bool setOrian(int axis);
    bool getParamer();
    bool setParamer();
    bool setLinkParamer();
    bool setTools(int toolindex,int toolsetstep);
    bool setcurTool(int toolindex);
    bool setZeroCalibration(int step);
    bool PIDParam(int cmd,int axis,int len);
    bool setCanDebug(int level);
    bool axisEnable(bool isEnable);    /**< ‰ΩøËÉΩ */
    bool getVersion();
    bool controllerReset();    /**< Â§ç‰Ωç */
    bool emsStop(bool isStop);    /**< ÊÄ•ÂÅú */
    bool getCtlVersion();    /**< Ëé∑ÂèñÊéßÂà∂Âô®ÁâàÊú¨Âè∑ */

    //ÊéßÂà∂Â∏ß--Ëá™Âä®Âä†Â∑•
    bool machineLock(bool isLock);    /**< Êú∫Ê¢∞ÈîÅÂÆöÔºàÁ©∫Ëµ∞Ôºâ */

    //ÈùûÂë®ÊúüÂ∏ß
    bool motionSpeedRate(int rate);    /**< ÈÄüÂ∫¶ÁôæÂàÜÊØî(0~100) */
    bool holdMotion(bool ishold);    /**< ÊöÇÂÅú/ÁªßÁª≠ËøêÂä® */
    bool stopFifoMotion();    /**< ÂÅúÊ≠¢ËøêÂä® */
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
    void initRequestFrame();    /**< ≥ı ºªØ«Î«Û÷° */
    void initRespondFrame_monitor();

    bool isPointInClampProtectArea(double x, double y, ProcessInterVariableCache* cache);
    
public:
	CNetSocket* m_socket;
	CNetSocket* m_socket2;
	CNetCtrl* m_netctrl;
	SystemParameter* m_pSystemParm;
	QFile *m_userfile;
private:
    QVector<unsigned char> m_dataRequest;    /**< «Î«Û÷° */
    QVector<unsigned char> m_dataResponse;   /**< œÏ”¶÷° */
    QVector<unsigned char> m_dataRequest_monitor;    /**< «Î«Û÷° */
    QVector<unsigned char> m_dataResponse_monitor;   /**< œÏ”¶÷° */

    int m_nFIFOSpace;   /**< FIFO £”‡ø’º‰ */

    char m_requestFunc;    /**< «Î«Û÷°π¶ƒ‹¬Î */
    char m_requestParam1;  /**< «Î«Û÷°π¶ƒ‹≤Œ ˝1 */
    char m_requestParam2;  /**< «Î«Û÷°π¶ƒ‹≤Œ ˝2 */
    char m_requestParam3;  /**< «Î«Û÷°π¶ƒ‹≤Œ ˝3 */
    char m_requestParam4;  /**< «Î«Û÷°π¶ƒ‹≤Œ ˝4 */

    char m_responseFunc;    /**< œÏ”¶÷°œÏ”¶¬Î */
    char m_responseParam1;  /**< œÏ”¶÷°π¶ƒ‹≤Œ ˝1 */
    char m_responseParam2;  /**< œÏ”¶÷°π¶ƒ‹≤Œ ˝2 */
    char m_responseParam3;  /**< œÏ”¶÷°π¶ƒ‹≤Œ ˝3 */
    char m_responseParam4;  /**< œÏ”¶÷°π¶ƒ‹≤Œ ˝4 */

    short m_nDI;    /**<  ‰»ÎIO◊¥Ã¨ */
    short m_nDO;    /**<  ‰≥ˆIO…Ë÷√ */
    int m_nMotionCount;    /**< “—ÕÍ≥…µ„Œª‘À∂Øº∆ ˝ */
    char m_nHeartBreatCount;/**< –ƒÃ¯ ˝º∆ ˝ */

    SystemState m_sysState;
    NAxis m_axisAlarm;    /**< ÷·±®æØ£¨∑«0÷µ±Ì æ±®æØ */
    NAxis m_axisLimit;    /**< ÷·œﬁŒª±®æØ£¨–°”⁄0±Ì æ∏∫œﬁŒª¥•∑¢°¢¥Û”⁄0±Ì æ’˝œﬁŒª¥•∑¢ */

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

    int          m_currentRunActionIndex;   /**< ÕÍ≥…µ„Œª ˝µƒÀ˜“˝÷µ */
    int          m_currentSendActionIndex;  /**< œ¬∑¢µ„Œª ˝µƒÀ˜“˝÷µ */
    bool         m_bisFinshOneAction;   /**< ÕÍ≥…¡À“ª◊Èµ„Œª ˝£¨…Ë÷√Œ™true */

    //NAxis        m_pulseEquivalent;    /**< ¬ˆ≥Âµ±¡ø£¨µ•Œªpluse/mm */

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
    int GoHomeStep; // ªÿ¡„≤Ω ˝
};

#endif // MOTIONCONTROLLERDEVICE_H
