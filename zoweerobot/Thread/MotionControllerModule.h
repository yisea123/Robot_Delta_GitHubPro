/**
Copyright (c)
@file MotionControllerModule.h
@author zhou
@version 1.0
@date 2018/07/10
@brief MotionControllerModule类型的定义。
*/

#ifndef MOTIONCONTROLLERMODULE_H
#define MOTIONCONTROLLERMODULE_H


#include "BaseModule.h"
#include "cautomachiningstateui.h"
//#include "cmachiningsimulationui.h"
//#include "cmanualmachiningstateui.h"
//#include "ccommunicationsettingui.h"
#include <QString>
#include <QPushButton>
#include <QList>
#define  EMULATION_SPEED_LOWEST       0
#define  EMULATION_SPEED_LOWER        1
#define  EMULATION_SPEED_MIDDLE       2
#define  EMULATION_SPEED_HIGHER       3
#define  EMULATION_SPEED_HIGHTEST     4

class  BaseGCode;
class  SystemSchedule;
class  ProcEmulationModel;
class  BaseAction;
class  MotionControllerDevice;
//class  CAutoMachiningStateUi;
//class CMachiningSimulationUi;
//class CManualMachiningStateUi;
//class CCommunicationSettingUi;
class PLCControllerModule;



class MotionControllerModule : public BaseModule
{
    Q_OBJECT
public:
    enum  _MotionWorkStatus
    {
        MOTION_WORK_NORMAL_BOOT,
        MOTION_WORK_IDLE,
        MOTION_WORK_NORMAL_START,   /* 开始准备 */
        MOTION_WORK_NORMAL_PROCESS,   /* 加工中 */
        MOTION_WORK_NORMAL_PAUSE,   /* 暂停 */
        MOTION_WORK_RESTORE_MOTION,   /* 暂停恢复 */
        MOTION_WORK_NORMAL_STOP,   /* 停止 */
        MOTION_WORK_CLEAR_FIFO,   /* 清除缓冲区 */
        MOTION_WORK_SHUTDOWN,    /* 关机 */
        MOTION_WORK_EMERGENCY_STOP,   /* 急停 */
//        MOTION_WORK_READY_FOR_FEEDER,  /* 上料准备 */
//        MOTION_WORK_REFUELING,     /* 换料中 */
        MOTION_WORK_MANUAL_MOTION,     /* 手动 */
        MOTION_WORK_STEPRUN_WAITTING,
        MOTION_WORK_ERROR_WAITTING,
        MOTION_WORK_SETTING_PARM,
        MOTION_WORK_SETTING_CRAFTPARM,
        MOTION_WORK_HOME_ACTION,
        MOTION_NO_CONNECT_NET,
        MOTION_RE_CONNECT_NET,
        MOTION_WORK_JOG_MOTION,     /* 手动 */
        MOTION_WORK_SERVO_ENABLE,
        MOTION_WORK_CLAMP_AUTOSEARCH,
        MOTION_CLOSE_CONNECT_NET,
        MOTION_CLEAR_ERROR,
        MOTION_EMERGENCY_STOP_WAITTING,
        MOTION_EMERGENCY_STOP_CLEAR,
        MOTION_WORK_NORMAL_END,
        MOTION_WORK_MANUAL_SET
        
    };


    MotionControllerModule();
    MotionControllerModule(QList<InfoUI *>& u, SystemSchedule* ss);
    ~MotionControllerModule();

    void run();
    int  initModule(QList<InfoUI *>& u);
    void monitorControllerValue(unsigned char* recv, int size);

    MotionControllerModule::_MotionWorkStatus GetCurWorkStatus(void);
    bool IsManualIdleStatus(void);
    //enum ManualStatus{MANUAL_START, MANUAL_RUN, MANUAL_WAIT, MANUAL_IDLE,MANUAL_MOVSTOP,MANUAL_WORLD_START,MANUAL_MOVE_START,MANUAL_MOVE_STOP,MANUAL_WORLDMOVE_START,MANUAL_WORLDMOVE_STOP,MANUAL_BACKZERO,MANUAL_GETVERSION,MANUAL_GETBATTERY,MANUAL_SETBATTERY,MANUAL_SAVETEACHPOINT,MANUAL_SAVETEACHPOINTTEXT,MANUAL_GETTEACHPOINT,MANUAL_MOVETOPOINT,MANUAL_DOWNUSERFILE,MANUAL_SETAXISNO,MANUAL_GETAXISNO,MANUAL_GETSYSPARAM} m_ManualStatus;

private:
    void initMotionControllerModule();
    int  findUIResource(QList<InfoUI *>& u, ErrorInfo& e);
    bool connectUIResource();
    bool ReadErrorInformation();
    int  addDevice(ErrorInfo& e);
    CEmulationCanvs* getEmulationCanvs();
    CAutoMachiningStateUi*  getMachiningStateUI();
    /*CMachiningSimulationUi*  getSimulationStateUI();
    CManualMachiningStateUi*  getManualMachiningStateUI();
    CCommunicationSettingUi*  getCommunicationSettingUi();*/
    void setEmulationStatus(CAutoMachiningStateUi::MotionRunStatue s);
    void setProcessBarValue(int s);

    void clearGCodeDataList();
    void stepEmulation();
    int saveTeachPoint();
    int saveTeachPointText();
	int downUserFile();
private:
    QVector<BaseGCode *>     m_GCodeDataList;     /**< 解析G代码后存放的数据结构 */
    QVector<BaseAction *>*     m_GCodeActions;
    ProcEmulationModel* m_pEmulationModelHandler;
    PLCControllerModule* m_pPlcControllerModuleHandle;

    ProcessInterVariableCache *m_pProcessCacheHandler;
    bool m_bisEmulation;
    int  m_nSpeedLevel;
    _MotionWorkStatus    m_WorkStatus;
    QTimer *m_MotionTimer;
	QTimer *m_NetCtrlTimer;
    int       m_GCodeRunSerialNum;
    int       nCurrentLineSerialNum;
    int m_nMotionControllerRunNum;
    int m_nDelayTime;
    //int m_nManualCodeRunNum;
    int m_nLastPartGCodeNum;
    bool m_bisStopMotion;
    bool m_bisPauseMotion;
    bool m_bisCycleRun;
    bool m_bisInPauseWaittingStatus;

    enum ManualStatus{MANUAL_START, MANUAL_RUN, MANUAL_WAIT, MANUAL_IDLE,MANUAL_MOVSTOP,MANUAL_WORLD_START,MANUAL_MOVE_START,MANUAL_MOVE_STOP,MANUAL_WORLDMOVE_START,MANUAL_WORLDMOVE_STOP,MANUAL_BACKZERO,MANUAL_GETVERSION,MANUAL_GETBATTERY,MANUAL_SETBATTERY,MANUAL_SAVETEACHPOINT,MANUAL_SAVETEACHPOINTTEXT,MANUAL_GETTEACHPOINT,MANUAL_MOVETOPOINT,MANUAL_DOWNUSERFILE,MANUAL_SETAXISNO,MANUAL_GETAXISNO,MANUAL_GETSYSPARAM} m_ManualStatus;
    enum StopStatus{STOP_START, WAITTING_STOP, CLEAR_FIFO, STOP_DONE} m_StopStatus;
    enum StartStatus{SETTING_PARM, SETTING_CRAFT_PARM, START_PRER} m_StartStatus;
    enum CtrlStatus{SETTING_RESET, SETTING_MODE, SETTING_CLEAR, SETTING_ORIAN,SETTING_TOOLS,SETTING_CURTOOL,SETTING_PARAMER,SETTING_LINKPARAMER,SETTING_ZERO_CALIBRATION} m_CtrlStatus;
    //enum SetParmStatus{SETTING_SYS_PARM, SETTING_CRAFT_PARM} m_SetParmStatus;
    int m_nZeroaxis;
    int m_nMoveAxis;
    double m_dMoveDis;
    int m_dMoverun;
    DAxis  m_axisMoveDis;
    int  m_mode;
    int  m_toolindex;
    int  m_toolsetstep;
    int  m_zerosetstep;
    int m_startteachpoint;
    int m_movepointnum;
	int m_userfileblock;
	QFile *m_userfile;

	int m_nPidCmd;
	int m_nPidAxis;
	int m_nPidLen;
	int m_nCanDebug;
	int m_nCanDebuglevel;
    //QString m_argFromUI;
    bool m_bIsJogMotionStart;
    bool m_bIsJogMotionStop;
    bool m_bIsStepRun;
    bool m_bIsBreakPointRun;
    int  m_nBreakPoint;
    bool m_bIsM01Effect;
    int  m_nDelayTimeSurplus;
    bool m_bIsServoEnable;

    //enum SetParmStatus{ALL_CARFT_PARM, PUNCHCARFT_PARM, AXIS_PARM, XYLIMIT_PARM, LIMIT_PARM, AXISSPEED_PARM, HOMESPEED_PARM} m_SetParmStatus;
    char m_SetParmStatus;

    //int  m_nClampAutoSearchStep;//夹钳搜索步骤
    enum ClampAutoSearchStep{CAS_PRE_WORK, CAS_X_MOVE, CAS_SEARCH_CLAMP, CAS_WAIT_STOP, CAS_RECORD_POS, CAS_CLEAR_LIMIT, CAS_X_MOVE_ZERO_PRE, CAS_X_MOVE_ZERO, CAS_WAIT_DONE} m_nClampAutoSearchStep;//夹钳搜索步骤
    bool m_bIsprevInClampSignalArea;//上一个夹钳信号检测值

    bool m_bIsEmsPress;
    bool m_bIsOutEmsPress;
    QByteArray ErrorInformation[256];
    double scarapos[MOF];
signals:
//    void continueEmulationSignal();

public slots:
    void runTimer();
    void absorbMotionModule(QString msg, QString arg);
    void absorbManualMotionModule(QString msg, QString arg);
    void doMotion();
	void onnetctrltimeout();

private:
    void procEmulationModel();
    void parseNGFileModel();
    void pauseEmulation();
    void pauseBreakPointEmulation();
    void runToBreakpointEmulation();
    void continueEmulation();
    void procMotionModel();
    void changeEmulationSpeed(int v);
    void restartEmulation();

    void systemReset();
    bool runOneActions();

    void feedbackSystemState();//反馈系统状态给plc
    bool isAxisVInRightPosition(double angle);
    bool isReadyToStartMotion();
    bool isServoEnable();
    bool isDingWeiXiaoEnable();
    bool isAxisTEnableMove();
    int isServoMode();
    friend  class MotionControllerDevice;
};


#endif // MOTIONCONTROLLERMODULE_H
