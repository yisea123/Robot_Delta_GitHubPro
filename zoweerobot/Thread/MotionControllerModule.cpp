/************************************************************
  Copyright (C),
  文件名：ParseGCode.cpp
  作者：zhou    版本：V1.0    创建日期：2018/07/10
  模块描述：ParseGCode类的实现
  历史修改记录：<author>    <time>    <version >    <desc>
                  zhou    2018/07/10     V1.0      添加注释
*************************************************************/

#include "MotionControllerModule.h"
#include "ComonTypes.h"
#include "SystemSchedule.h"
#include "mainwindow.h"
#include "cautomachiningstateui.h"

#include "BaseDevice.h"
#include "MotionControllerDevice.h"

#include "MotionControllerModule.h"
//#include "EmulationControllerModule.h"

#include "BaseGCode.h"
#include "ParseGCodeModel.h"
#include "ProcEmulationModel.h"

//#include "PLCControllerModule.h"
#include "NetCtrl.h"
#include "NetSocket.h"

#include <QList>

/*************************************************
  函数名称：MotionControllerModule()
  函数功能：MotionControllerModule构造函数
  输入参数：
  输出参数：
  返 回 值：
  备    注：
**************************************************/
MotionControllerModule::MotionControllerModule()
{
    m_ModuleInfo = ModulesInfo[MOTION_CONTROLLER_ID];
}

/*************************************************
  函数名称：MotionControllerModule(QList<InfoUI *>& u, SystemSchedule* ss)
  函数功能：MotionControllerModule构造函数
  输入参数：u UI链表
           ss  SystemScheduler指针
  输出参数：
  返 回 值：
  备    注：
**************************************************/
MotionControllerModule::MotionControllerModule(QList<InfoUI *>& u, SystemSchedule* ss)
{
    m_ModuleInfo = ModulesInfo[MOTION_CONTROLLER_ID];
    m_pScheduler = ss;
    m_bisEmulation = false;

    m_errInfo.setModuleInfo(m_ModuleInfo);
    m_errInfo.e_type = DEBUG_NO_ERROR;
    m_pEmulationModelHandler = NULL;
    m_pDevice=nullptr;
    initMotionControllerModule();

    m_pProcessCacheHandler = NULL;
    m_pProcessCacheHandler = new ProcessInterVariableCache;
    m_pProcessCacheHandler->m_clampPos = m_pScheduler->getSystemParameterHandler()->m_clampPosition;

    m_MotionTimer = NULL;
     
//    m_MotionTimer = new QTimer(this);
//    connect(m_MotionTimer, SIGNAL(timeout()), this, SLOT(doMotion()), Qt::DirectConnection);
//    m_MotionTimer->start(100);
}

/*************************************************
  函数名称：~MotionControllerModule()
  函数功能：MotionControllerModule析构函数
  输入参数：
  输出参数：
  返 回 值：
  备    注：
**************************************************/
MotionControllerModule::~MotionControllerModule()
{
    clearGCodeDataList();
    delete m_pProcessCacheHandler;
}

/*************************************************
  函数名称：initMotionControllerModule()
  函数功能：MotionControllerModule成员变量初始化
  输入参数：
  输出参数：
  返 回 值：
  备    注：在执行系统复位时需要调用
**************************************************/
void MotionControllerModule::initMotionControllerModule()
{
    m_nSpeedLevel = EMULATION_SPEED_MIDDLE;
    m_WorkStatus = MOTION_WORK_NORMAL_BOOT;

    m_GCodeRunSerialNum = 0;

    m_nMotionControllerRunNum = 0;
    m_nLastPartGCodeNum = 0;
    m_bisStopMotion = false;
    m_bisPauseMotion = false;
    m_bisInPauseWaittingStatus = false;
    m_bisCycleRun = false;
    //m_argFromUI = "";

    m_ManualStatus = MANUAL_IDLE;
    m_StopStatus = STOP_DONE;
    m_StartStatus = START_PRER;//SETTING_PARM;

    m_bIsJogMotionStart = false;
    m_bIsJogMotionStop = false;
    m_bIsStepRun = false;
    m_bIsBreakPointRun = false;
    m_nBreakPoint = 0;
    m_bIsM01Effect = false;
    m_nDelayTimeSurplus = 0;
    m_bIsServoEnable = false;

    //m_SetParmStatus = ALL_CARFT_PARM;
    m_SetParmStatus = 127;//0111 1111B  所有参数设置

    m_nClampAutoSearchStep = CAS_PRE_WORK;
    m_bIsprevInClampSignalArea = false;

    m_bIsEmsPress = false;
    m_bIsOutEmsPress = false;
}

/*************************************************
  函数名称：initModule(QList<InfoUI *>& u)
  函数功能：Module初始化 加载UI 加载设备
  输入参数：u UI链表
  输出参数：
  返 回 值：
  备    注：
**************************************************/
int MotionControllerModule::initModule(QList<InfoUI *>& u)
{
    findUIResource(u, m_errInfo);
    connectUIResource();
    ReadErrorInformation();
//    addDevice(m_errInfo);
    //m_pPlcControllerModuleHandle = (PLCControllerModule*)(m_pScheduler->getModulesList()->at(PLC_CONTROLLER_ID));

    start();

    /*m_ModuleTimer = new QTimer(this);
    connect(m_ModuleTimer, SIGNAL(timeout()), this, SLOT(runTimer()));
    m_ModuleTimer->start(100);*/
    return 0;
}

// »ñÈ¡µ±Ç°×´Ì¬
MotionControllerModule::_MotionWorkStatus MotionControllerModule::GetCurWorkStatus(void)
{
    return m_WorkStatus;
}

// ÅÐ¶Ï ÊÖ¶¯ÊÇ·ñ´¦ÓÚ¿ÕÏÐ×´Ì¬
bool MotionControllerModule::IsManualIdleStatus(void)
{
    if(m_ManualStatus == MANUAL_IDLE){
        return true;
    }
    return false;
}

/*************************************************
  函数名称：connectUIResource()
  函数功能：连接UI
  输入参数：
  输出参数：
  返 回 值：
  备    注：
**************************************************/
bool MotionControllerModule::connectUIResource()
{
//    for(int i = 0; i < m_UIs.size(); i++)
//    {
//        if(m_UIs.at(i)->nUISerial == UI_PROGRAM_EMULATION_SELECTION)
//        {
//            (m_UIs.at(i)->pUi)->connect(m);
//        }
//    }
    return true;
}

/*************************************************
  函数名称：findUIResource(QList<InfoUI *>& u, ErrorInfo& e)
  函数功能：在系统的所有注册的UI下，寻找MotionControllerModule使用的UI
  输入参数：u UI链表
           e  错误信息
  输出参数：
  返 回 值：
  备    注：
**************************************************/
int MotionControllerModule::findUIResource(QList<InfoUI *>& u, ErrorInfo& e)
{
    for(int i = 0; i < u.size(); i++)
    {
        for(int j = 0; j < u.at(i)->nModulesUser.size(); j++)
        {
            if(u.at(i)->nModulesUser[j] == MOTION_CONTROLLER_ID)   /* 匹配该Module */
               m_UIs.append(u.at(i));
        }
    }

    if(m_UIs.size() == 0)
    {
        e.insertDebug(DEBUG_WARNING, "MotionControllerModule initModule find no ui");
    }
    return 0;
}

bool MotionControllerModule::ReadErrorInformation()
{
	QFile file("errorinformation.txt");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return 0;

    int  i = 0;


    while(!file.atEnd())
    {
       if(i<256)
       	{
		ErrorInformation[i]= file.readLine();
		//ErrorInformation[i].remove('\n');
		i++;
       	}
	else
	{
	break;
	}
}
	file.close();
    return true;
}

/*************************************************
  函数名称：getEmulationCanvs()
  函数功能：获取自动加工界面仿真画布指针
  输入参数：
  输出参数：
  返 回 值：
  备    注：
**************************************************/
CEmulationCanvs*  MotionControllerModule::getEmulationCanvs()
{
    for(int i = 0; i < m_UIs.size(); i++)
    {
        if(m_UIs.at(i)->nUISerial == UI_AUTO_MACHINING_STATE)
        {
            return  ((CAutoMachiningStateUi *)(m_UIs.at(i)->pUi))->getCanvsHandle();
        }
    }

    return NULL;
}

/*************************************************
  函数名称：getMachiningStateUI()
  函数功能：获取自动加工界面指针
  输入参数：
  输出参数：
  返 回 值：
  备    注：
**************************************************/
CAutoMachiningStateUi*  MotionControllerModule::getMachiningStateUI()
{
    for(int i = 0; i < m_UIs.size(); i++)
    {
        if(m_UIs.at(i)->nUISerial == UI_AUTO_MACHINING_STATE)
        {
            return  (CAutoMachiningStateUi *)(m_UIs.at(i)->pUi);
        }
    }

    return NULL;
}

/*************************************************
  函数名称：getSimulationStateUI()
  函数功能：获取仿真界面指针
  输入参数：
  输出参数：
  返 回 值：
  备    注：
**************************************************/
/*CMachiningSimulationUi*  MotionControllerModule::getSimulationStateUI()
{
    for(int i = 0; i < m_UIs.size(); i++)
    {
        if(m_UIs.at(i)->nUISerial == UI_MACHINING_SIMULATION)
        {
            return  (CMachiningSimulationUi *)(m_UIs.at(i)->pUi);
        }
    }

    return NULL;
}*/

/*************************************************
  函数名称：getManualMachiningStateUI()
  函数功能：获取手动加工界面指针
  输入参数：
  输出参数：
  返 回 值：
  备    注：
**************************************************/
/*CManualMachiningStateUi*  MotionControllerModule::getManualMachiningStateUI()
{
    for(int i = 0; i < m_UIs.size(); i++)
    {
        if(m_UIs.at(i)->nUISerial == UI_MANUAL_MACHINING_STATE)
        {
            return  (CManualMachiningStateUi *)(m_UIs.at(i)->pUi);
        }
    }

    return NULL;
}*/

/*************************************************
  函数名称：getManualMachiningStateUI()
  函数功能：获取外部设备通讯界面指针
  输入参数：
  输出参数：
  返 回 值：
  备    注：
**************************************************/
/*CCommunicationSettingUi*  MotionControllerModule::getCommunicationSettingUi()
{
    for(int i = 0; i < m_UIs.size(); i++)
    {
        if(m_UIs.at(i)->nUISerial == UI_COMMUNICATION_SETTINGS)
        {
            return  (CCommunicationSettingUi *)(m_UIs.at(i)->pUi);
        }
    }

    return NULL;
}*/

/*************************************************
  函数名称：addDevice(ErrorInfo& e)
  函数功能：加载设备
  输入参数：
  输出参数：e  错误信息
  返 回 值：
  备    注：
**************************************************/
int MotionControllerModule::addDevice(ErrorInfo& e)
{
    m_pDevice = new MotionControllerDevice(m_ModuleInfo.name, this, m_pScheduler->getSystemParameterHandler());

    e.e_type = DEBUG_NO_ERROR;
    ((MotionControllerDevice *)m_pDevice)->setCanvsHandle(getEmulationCanvs());
    ((MotionControllerDevice *)m_pDevice)->setMachiningStateUi(getMachiningStateUI());
    /*((MotionControllerDevice *)m_pDevice)->setSimulationStateUi(getSimulationStateUI());
    ((MotionControllerDevice *)m_pDevice)->setManualMachiningStateUi(getManualMachiningStateUI());
    ((MotionControllerDevice *)m_pDevice)->setCommunicationSettingUI(getCommunicationSettingUi());*/

//    connect(((MotionControllerDevice *)m_pDevice), SIGNAL(monitorControllerValue(unsigned char*, int)),
//            this, SLOT(monitorControllerValue(unsigned char*, int)), Qt::DirectConnection);

    //connect(((MotionControllerDevice *)m_pDevice), SIGNAL(NetLedCtrl(bool)),
    //        getCommunicationSettingUi(), SLOT(ctrlNetConnectLed(bool)), Qt::QueuedConnection);

//    connect(getCommunicationSettingUi(), SIGNAL(reConnectNet()),
//            ((MotionControllerDevice *)m_pDevice), SLOT(reConnectNet()), Qt::DirectConnection);


    return m_pDevice->openDevice(e);
}

/*************************************************
  函数名称：clearGCodeDataList()
  函数功能：清除m_GCodeDataList链表
  输入参数：
  输出参数：
  返 回 值：
  备    注：
**************************************************/
void MotionControllerModule::clearGCodeDataList()
{
    for(int i = 0; i < m_GCodeDataList.size(); i++)
    {
        delete m_GCodeDataList.at(i);
    }

    m_GCodeDataList.clear();
}

/*************************************************
  函数名称：parseNGFileModel()
  函数功能：槽函数   相应”解析G代码按钮“
  输入参数：
  输出参数：
  返 回 值：
  备    注：
**************************************************/
void MotionControllerModule::parseNGFileModel()
{
    ParseGCodeModel* pp = pp->loadParseController(this, m_errInfo);

    if(pp != NULL)
    {
        clearGCodeDataList();

        m_errInfo.e_type = DEBUG_NO_ERROR;

        /* 方式1：在槽函数中执行全部解析G代码 */
        if(!pp->startupParseController(this, &m_GCodeDataList, m_errInfo))
        {
            clearGCodeDataList();
            /* emit错误信号给system，system相应的槽接受，所有的信息在m_errInfo*/
            m_pScheduler->recvErrMsgFromAllModules(m_errInfo);
            m_pScheduler->writeMsgToStatusBar(m_errInfo.toString(), STATUSBAR_ERROR_INFO);
        }

        pp->unloadParseController(this, m_errInfo);
    }
    else
    {
        m_errInfo.insertDebug(DEBUG_CREAT_ERROR, "parseNGFileModel loadParseController error\n");
        m_pScheduler->recvErrMsgFromAllModules(m_errInfo);
    }


    /* 方式2：在线程（MotionControllerModule::run()）中执行全部解析G代码 */
//    pp->moveToThread(this);
//    start();

}

/*************************************************
  函数名称：procEmulationModel()
  函数功能：启动仿真
  输入参数：
  输出参数：
  返 回 值：
  备    注：
**************************************************/
void MotionControllerModule::procEmulationModel()
{
    
}

/*************************************************
  函数名称：setEmulationStatus(CAutoMachiningStateUi::MotionRunStatue s)
  函数功能：设置自动加工界面的运行状态
  输入参数：s：自动加工界面的运行状态
  输出参数：
  返 回 值：
  备    注：
**************************************************/
void MotionControllerModule::setEmulationStatus(CAutoMachiningStateUi::MotionRunStatue s)
{
    for(int i = 0; i < m_UIs.size(); i++)
    {
        if(m_UIs.at(i)->nUISerial == UI_AUTO_MACHINING_STATE)
        {
            ((CAutoMachiningStateUi *)(m_UIs.at(i)->pUi))->setIsEmulationRunning(s);
        }
    }
}

/*************************************************
  函数名称：setProcessBarValue(int s)
  函数功能：设置自动加工界面的加工进度
  输入参数：s：自动加工界面的加工进度
  输出参数：
  返 回 值：
  备    注：
**************************************************/
void MotionControllerModule::setProcessBarValue(int s)
{
    for(int i = 0; i < m_UIs.size(); i++)
    {
        if(m_UIs.at(i)->nUISerial == UI_AUTO_MACHINING_STATE)
        {
            ((CAutoMachiningStateUi *)(m_UIs.at(i)->pUi))->setProcessBarValue(s);
        }
    }
}

/*************************************************
  函数名称：pauseEmulation()
  函数功能：暂停仿真
  输入参数：
  输出参数：
  返 回 值：
  备    注：
**************************************************/
void MotionControllerModule::pauseEmulation()
{
    if(m_bisEmulation)
    {
        if(m_ModuleTimer->isActive())
        {
            m_ModuleTimer->stop();
        }
        setEmulationStatus(CAutoMachiningStateUi::EMULATION_PAUSE);
    }
}

/*************************************************
  函数名称：pauseBreakPointEmulation()
  函数功能：暂停仿真
  输入参数：
  输出参数：
  返 回 值：
  备    注：
**************************************************/
void MotionControllerModule::pauseBreakPointEmulation()
{
    if(m_bisEmulation)
    {
        if(m_ModuleTimer->isActive())
        {
            m_ModuleTimer->stop();
        }
        setEmulationStatus(CAutoMachiningStateUi::EMULATION_BREAKPOINT_PAUSE);
    }
}

/*************************************************
  函数名称：continueEmulation()
  函数功能：仿真继续
  输入参数：
  输出参数：
  返 回 值：
  备    注：
**************************************************/
void MotionControllerModule::continueEmulation()
{
    if(m_bisEmulation)
    {
        if(!m_ModuleTimer->isActive())
        {
            m_ModuleTimer->start();
        }
        setEmulationStatus(CAutoMachiningStateUi::EMULATION_RUN);
    }
}

/*************************************************
  函数名称：changeEmulationSpeed(int v)
  函数功能：设置仿真速度
  输入参数：
  输出参数：
  返 回 值：
  备    注：
**************************************************/
void MotionControllerModule::changeEmulationSpeed(int v)
{
    /*if(m_bisEmulation)
    {
        if(m_ModuleTimer->isActive())
        {
            m_ModuleTimer->stop();
        }
        m_ModuleTimer->start(EmulationSpeedArray[v]);
        //m_nSpeedLevel = v;
    }
    m_nSpeedLevel = v;*/
}

/*************************************************
  函数名称：restartEmulation()
  函数功能：仿真重启
  输入参数：
  输出参数：
  返 回 值：
  备    注：
**************************************************/
void MotionControllerModule::restartEmulation()
{
    /*if(m_pEmulationModelHandler != NULL)
    {
        m_pEmulationModelHandler->restartemulateMotion(m_errInfo);
        setEmulationStatus(CAutoMachiningStateUi::MOTION_EMULATION_IDLE);
    }
    m_ModuleTimer->stop();
    m_bisEmulation = false;
    m_pProcessCacheHandler->m_bisEmulation = false;*/
}

/*************************************************
  函数名称：runToBreakpointEmulation()
  函数功能：仿真断点运行
  输入参数：
  输出参数：
  返 回 值：
  备    注：
**************************************************/
void MotionControllerModule::runToBreakpointEmulation()
{
   /* CAutoMachiningStateUi *pEmulationUi = (CAutoMachiningStateUi *)getUIHandlerByID(UI_AUTO_MACHINING_STATE);
    int nBreakpoint = pEmulationUi->getBreakpoint();

    procEmulationModel();
    m_pEmulationModelHandler->setBreakpointLine(nBreakpoint);*/
}

/*************************************************
  函数名称：absorbMotionModule(QString msg, QString arg)
  函数功能：槽函数  处理来自emitMotionModuleSignal(QString, QString)的信号
  输入参数：
  输出参数：
  返 回 值：
  备    注：
**************************************************/
void MotionControllerModule::absorbMotionModule(QString msg, QString arg)
{
    qDebug()<<"absorbMotionModule start";

    {
        //系统除急停状态之外能执行的消息//
        if (m_WorkStatus != MOTION_EMERGENCY_STOP_WAITTING)
        {
           /* if (msg == "system reset")
            {
                //各种清理工作
                systemReset();
                m_pScheduler->recvRunInfoFromAllModules("系统复位");
            }
            else if (msg == "clear error")
            {
                m_WorkStatus = MOTION_CLEAR_ERROR;//MOTION_WORK_IDLE;
                m_StopStatus = STOP_DONE;
                m_StartStatus = START_PRER;//SETTING_PARM;
                m_ManualStatus == MANUAL_IDLE;
                m_pProcessCacheHandler->m_bIsManualMotion = true;
                //setEmulationStatus(CAutoMachiningStateUi::MOTION_EMULATION_IDLE);

                m_nMotionControllerRunNum = ((MotionControllerDevice *)m_pDevice)->getCurrentRunActionIndex();// + 1;
                m_nLastPartGCodeNum = m_nMotionControllerRunNum;
                m_pProcessCacheHandler->m_nLastPartGCodeNum = m_nLastPartGCodeNum;
                m_bisStopMotion = false;
                setEmulationStatus(CAutoMachiningStateUi::MOTION_EMULATION_IDLE);
                m_pProcessCacheHandler->m_newPoint =  m_pProcessCacheHandler->m_curPoint;
                m_pProcessCacheHandler->m_newPosition = m_pProcessCacheHandler->m_curPosition;
                m_nDelayTimeSurplus = 0;

                restartEmulation();

                setProcessBarValue(0);
                m_pScheduler->recvRunInfoFromAllModules("清除错误");
                m_pScheduler->writeMsgToStatusBar("");
            }
            else*/
            if (msg == "cycle run")
            {
                if (arg == "cycle")
                {
                     if ((m_WorkStatus != MOTION_WORK_NORMAL_START)
			&&(m_WorkStatus != MOTION_WORK_NORMAL_PROCESS)
			&&(m_WorkStatus != MOTION_WORK_NORMAL_END)
			&&(m_WorkStatus != MOTION_WORK_ERROR_WAITTING))
                     {
                    		m_bisCycleRun = true;
		      		if (isServoEnable()&&(2==isServoMode()))
		                {
		                	  qDebug()<<"procMotionModel start";
		                    procMotionModel();
			  		qDebug()<<"procMotionModel end";
		                    //m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("加工启动"));
		                    setProcessBarValue(0);
		                    //m_pScheduler->writeMsgToStatusBar(QString::fromLocal8Bit("加工启动"));
		                    m_bisInPauseWaittingStatus = false;
				     m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("循环加工启动"));
		                }
                    		
				 else
				{
				        QMessageBox::information(NULL, QString::fromLocal8Bit("警告"), QString::fromLocal8Bit("未使能或未处于自动模式"));
				        
				}
                    		//m_pScheduler->writeMsgToStatusBar(QString::fromLocal8Bit("循环加工启动"), STATUSBAR_STATUS_ON_INFO);
                     }
			else
			{
			        QMessageBox::information(NULL, QString::fromLocal8Bit("警告"), QString::fromLocal8Bit("程序运行中，请先停止！"));
			        qDebug()<<"系统状态"<<m_WorkStatus;
			}
					 
                }
                else if(arg == "ok"){
                    m_bisCycleRun = true;                    	   		     	
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("Ñ­»·¼Ó¹¤¿ªÆô"));
                }
                else  // cancel
                {                  
                    m_bisCycleRun = false;                    	   		     	
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("Ñ­»·¼Ó¹¤È¡Ïû"));
                    //m_pScheduler->writeMsgToStatusBar(QString::fromLocal8Bit("Ñ­»·¼Ó¹¤È¡Ïû"), STATUSBAR_STATUS_OFF_INFO);
                }
            }
            else if (msg == "getPIDParamer")
            {             

                      	{
                           // if (arg == "15")
                            {
                            	  
                                m_nPidAxis=(arg.toInt()/10)-1;
				  m_nPidCmd=((m_nPidAxis>=0)&&(m_nPidAxis<MOF))?2:0;
				   m_nPidLen=3;//(arg.toInt()%10)
                            }
                      	}
                        
            }
            else if (msg == "setPIDParamer")
            {             

                      	{
                           // if (arg == "15")
                            {
                            	  
                                m_nPidAxis=(arg.toInt()/10)-1;
				  m_nPidCmd=((m_nPidAxis>=0)&&(m_nPidAxis<MOF))?1:0;
				   m_nPidLen=3;//(arg.toInt()%10)
                            }
                      	}
                        
            }
	     else if (msg == "setCanDebug")
            {             

                      	{
                           // if (arg == "15")
                            {
                            	  
                                m_nCanDebug=1;
				   m_nCanDebuglevel=(arg.toInt()%10);
                            }
                      	}
                        
            }
            else
            {
            		qDebug()<<"m_WorkStatus"<<m_WorkStatus;
                //系统正常状态下能执行的消息//
                //if (m_WorkStatus != MOTION_WORK_ERROR_WAITTING)
		 if ((m_WorkStatus != MOTION_WORK_NORMAL_START)
			&&(m_WorkStatus != MOTION_WORK_NORMAL_PROCESS)
			&&(m_WorkStatus != MOTION_WORK_NORMAL_END)
			&&(m_WorkStatus != MOTION_WORK_ERROR_WAITTING))
                  
                {
                    if (msg == "start motion")
                    {                                          
                            if (isReadyToStartMotion())//安全监测
                            {
                                if (arg == "cycle"){  // Ñ­»·
                                    m_bisCycleRun = true;
                                }else{
                                    m_bisCycleRun = false;
                                }
                                if (isServoEnable()&&(2==isServoMode()))
                                {
                                	  qDebug()<<"procMotionModel start";
                                    procMotionModel();
					  qDebug()<<"procMotionModel end";
                                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("加工启动"));
                                    setProcessBarValue(0);
                                    //m_pScheduler->writeMsgToStatusBar(QString::fromLocal8Bit("加工启动"));
				      //m_pScheduler->writeMsgToStatusBar(QString::fromLocal8Bit(""));
                                    m_bisInPauseWaittingStatus = false;
                                }
				   else
					{
					        QMessageBox::information(NULL, QString::fromLocal8Bit("警告"), QString::fromLocal8Bit("未使能或未处于自动模式"));
					
					}
                            }
                        
                    }
                   
                   
                    
                    else if(msg == "cycle motion")
                    {
                        m_nLastPartGCodeNum = m_nMotionControllerRunNum;
                        m_GCodeRunSerialNum = 0;
                        m_pProcessCacheHandler->m_nMCodeCnt = 0;
                        m_pProcessCacheHandler->m_nLastPartGCodeNum = m_nLastPartGCodeNum;
                        m_pProcessCacheHandler->m_nlastRecordRunCnt = 0;

                        //((MotionControllerDevice *)m_pDevice)->clearCanvsData();
                        //((MotionControllerDevice *)m_pDevice)->drawBackground(m_pEmulationModelHandler->getRawMaterial(), m_GCodeActions, m_pEmulationModelHandler->m_EmulationCache, m_errInfo);
                        m_WorkStatus = MOTION_WORK_NORMAL_PROCESS;
                        m_nDelayTimeSurplus = 0;
                    }
                 
                }
                else
                {
                    QMessageBox::information(NULL, QString::fromLocal8Bit("警告"), QString::fromLocal8Bit("程序运行中，请先停止！"));
                }
            }
        }
        else
        {
            QMessageBox::information(NULL, QString::fromLocal8Bit("警告"), QString::fromLocal8Bit("系统处于急停状态中，请先消除急停！"));
        }
    }
}

/*************************************************
  函数名称：stepEmulation()
  函数功能：单步仿真
  输入参数：
  输出参数：
  返 回 值：
  备    注：
**************************************************/
void MotionControllerModule::stepEmulation()
{
    /*if(!m_bisEmulation)
    {
        procEmulationModel();
    }

    int nextPoint;
    if(m_pEmulationModelHandler->stepEmulateMotion(nextPoint, m_errInfo))
    {
        m_pEmulationModelHandler->setBreakpointLine(nextPoint);
        continueEmulation();
    }*/
}

/*************************************************
  函数名称：absorbManualMotionModule(QString msg, QString arg)
  函数功能：槽函数  处理来自emitMotionModuleSignal(QString, QString)的信号
  输入参数：
  输出参数：
  返 回 值：
  备    注：响应手动功能
**************************************************/
void MotionControllerModule::absorbManualMotionModule(QString msg, QString arg)
{
    if (m_WorkStatus == MOTION_WORK_MANUAL_SET||m_WorkStatus == MOTION_WORK_IDLE || m_WorkStatus == MOTION_WORK_MANUAL_MOTION || m_WorkStatus == MOTION_WORK_JOG_MOTION)
    {
       
        if (msg == "xPMotion")
        {
            if (isServoEnable())// && isAxisXYEnableMove(!m_bIsDebugMode)
            {
                if (m_ManualStatus == MANUAL_IDLE)
                {
                    m_nMoveAxis = AXIS_X;
                    m_dMoveDis = arg.toDouble();
                    m_ManualStatus = MANUAL_START;
                    m_WorkStatus = MOTION_WORK_MANUAL_MOTION;
                    //m_pScheduler->recvRunInfoFromAllModules("X杞寸Щ鍔 + arg + "mm");
			return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("璇风瓑寰呯Щ鍔ㄦ寚浠ゅ畬鎴怺 杩愯鐘舵€侊細%1锛屾寚浠わ細%2锛屽唴瀹癸細%3 ]").arg(m_ManualStatus).arg(msg).arg(arg));
                    return;
                }
            }
        }
	  if (msg == "xPMotion_world")
        {
            if (isServoEnable())// && isAxisXYEnableMove(!m_bIsDebugMode)
            {
                if (m_ManualStatus == MANUAL_IDLE)
                {
                    m_nMoveAxis = AXIS_X;
                    m_dMoveDis = arg.toDouble();
                    m_ManualStatus = MANUAL_WORLD_START;
                    m_WorkStatus = MOTION_WORK_MANUAL_MOTION;
                    //m_pScheduler->recvRunInfoFromAllModules("X杞寸Щ鍔 + arg + "mm");
			return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("璇风瓑寰呯Щ鍔ㄦ寚浠ゅ畬鎴怺 杩愯鐘舵€侊細%1锛屾寚浠わ細%2锛屽唴瀹癸細%3 ]").arg(m_ManualStatus).arg(msg).arg(arg));
                    return;
                }
            }
        }
        else if (msg == "yPMotion")
        {
            if (isServoEnable())// && isAxisXYEnableMove(!m_bIsDebugMode)
            {
                if (m_ManualStatus == MANUAL_IDLE)
                {
                    m_nMoveAxis = AXIS_Y;
                    m_dMoveDis = arg.toDouble();
                    m_ManualStatus = MANUAL_START;
                    m_WorkStatus = MOTION_WORK_MANUAL_MOTION;
                    //m_pScheduler->recvRunInfoFromAllModules("Y杞寸Щ鍔 + arg + "mm");
                    return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("璇风瓑寰呯Щ鍔ㄦ寚浠ゅ畬鎴怺 杩愯鐘舵€侊細%1锛屾寚浠わ細%2锛屽唴瀹癸細%3 ]").arg(m_ManualStatus).arg(msg).arg(arg));
                    return;
                }
            }
        }
	  else if (msg == "yPMotion_world")
        {
            if (isServoEnable())// && isAxisXYEnableMove(!m_bIsDebugMode)
            {
                if (m_ManualStatus == MANUAL_IDLE)
                {
                    m_nMoveAxis = AXIS_Y;
                    m_dMoveDis = arg.toDouble();
                    m_ManualStatus = MANUAL_WORLD_START;
                    m_WorkStatus = MOTION_WORK_MANUAL_MOTION;
                    //m_pScheduler->recvRunInfoFromAllModules("X杞寸Щ鍔 + arg + "mm");
			return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("璇风瓑寰呯Щ鍔ㄦ寚浠ゅ畬鎴怺 杩愯鐘舵€侊細%1锛屾寚浠わ細%2锛屽唴瀹癸細%3 ]").arg(m_ManualStatus).arg(msg).arg(arg));
                    return;
                }
            }
        }
        else if (msg == "zPMotion")
        {
            if (isServoEnable())// && isAxisXYEnableMove(!m_bIsDebugMode)
            {
                if (m_ManualStatus == MANUAL_IDLE)
                {
                    m_nMoveAxis = AXIS_Z;
                    m_dMoveDis = arg.toDouble();
                    m_ManualStatus = MANUAL_START;
                    m_WorkStatus = MOTION_WORK_MANUAL_MOTION;
                    //m_pScheduler->recvRunInfoFromAllModules("Z杞寸Щ鍔 + arg + "mm");
                    return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("璇风瓑寰呯Щ鍔ㄦ寚浠ゅ畬鎴怺 杩愯鐘舵€侊細%1锛屾寚浠わ細%2锛屽唴瀹癸細%3 ]").arg(m_ManualStatus).arg(msg).arg(arg));
                    return;
                }
            }
        }
	  else if (msg == "zPMotion_world")
        {
            if (isServoEnable())// && isAxisXYEnableMove(!m_bIsDebugMode)
            {
                if (m_ManualStatus == MANUAL_IDLE)
                {
                    m_nMoveAxis = AXIS_Z;
                    m_dMoveDis = arg.toDouble();
                    m_ManualStatus = MANUAL_WORLD_START;
                    m_WorkStatus = MOTION_WORK_MANUAL_MOTION;
                    //m_pScheduler->recvRunInfoFromAllModules("X杞寸Щ鍔 + arg + "mm");
			return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("璇风瓑寰呯Щ鍔ㄦ寚浠ゅ畬鎴怺 杩愯鐘舵€侊細%1锛屾寚浠わ細%2锛屽唴瀹癸細%3 ]").arg(m_ManualStatus).arg(msg).arg(arg));
                    return;
                }
            }
        }
        else if (msg == "rPMotion")
        {
            if (isServoEnable())// && isAxisXYEnableMove(!m_bIsDebugMode)
            {
                if (m_ManualStatus == MANUAL_IDLE)
                {
                    m_nMoveAxis = AXIS_R;
                    m_dMoveDis = arg.toDouble();
                    m_ManualStatus = MANUAL_START;
                    m_WorkStatus = MOTION_WORK_MANUAL_MOTION;
                    //m_pScheduler->recvRunInfoFromAllModules("R杞寸Щ鍔 + arg + "mm");
                    return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("璇风瓑寰呯Щ鍔ㄦ寚浠ゅ畬鎴怺 杩愯鐘舵€侊細%1锛屾寚浠わ細%2锛屽唴瀹癸細%3 ]").arg(m_ManualStatus).arg(msg).arg(arg));
                    return;
                }
            }
        }
	 else if (msg == "rPMotion_world")
        {
            if (isServoEnable())// && isAxisXYEnableMove(!m_bIsDebugMode)
            {
                if (m_ManualStatus == MANUAL_IDLE)
                {
                    m_nMoveAxis = AXIS_R;
                    m_dMoveDis = arg.toDouble();
                    m_ManualStatus = MANUAL_WORLD_START;
                    m_WorkStatus = MOTION_WORK_MANUAL_MOTION;
                    //m_pScheduler->recvRunInfoFromAllModules("X杞寸Щ鍔 + arg + "mm");
			return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("璇风瓑寰呯Щ鍔ㄦ寚浠ゅ畬鎴怺 杩愯鐘舵€侊細%1锛屾寚浠わ細%2锛屽唴瀹癸細%3 ]").arg(m_ManualStatus).arg(msg).arg(arg));
                    return;
                }
            }
        }

	 else if (msg == "wPMotion")
        {
            if (isServoEnable())// && isAxisXYEnableMove(!m_bIsDebugMode)
            {
                if (m_ManualStatus == MANUAL_IDLE)
                {
                    m_nMoveAxis = AXIS_W;
                    m_dMoveDis = arg.toDouble();
                    m_ManualStatus = MANUAL_START;
                    m_WorkStatus = MOTION_WORK_MANUAL_MOTION;
                    //m_pScheduler->recvRunInfoFromAllModules("R杞寸Щ鍔 + arg + "mm");
                    return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("璇风瓑寰呯Щ鍔ㄦ寚浠ゅ畬鎴怺 杩愯鐘舵€侊細%1锛屾寚浠わ細%2锛屽唴瀹癸細%3 ]").arg(m_ManualStatus).arg(msg).arg(arg));
                    return;
                }
            }
        }
	 else if (msg == "wPMotion_world")
        {
            if (isServoEnable())// && isAxisXYEnableMove(!m_bIsDebugMode)
            {
                if (m_ManualStatus == MANUAL_IDLE)
                {
                    m_nMoveAxis = AXIS_W;
                    m_dMoveDis = arg.toDouble();
                    m_ManualStatus = MANUAL_WORLD_START;
                    m_WorkStatus = MOTION_WORK_MANUAL_MOTION;
                    //m_pScheduler->recvRunInfoFromAllModules("X杞寸Щ鍔 + arg + "mm");
			return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("璇风瓑寰呯Щ鍔ㄦ寚浠ゅ畬鎴怺 杩愯鐘舵€侊細%1锛屾寚浠わ細%2锛屽唴瀹癸細%3 ]").arg(m_ManualStatus).arg(msg).arg(arg));
                    return;
                }
            }
        }

	 else if (msg == "pPMotion")
        {
            if (isServoEnable())// && isAxisXYEnableMove(!m_bIsDebugMode)
            {
                if (m_ManualStatus == MANUAL_IDLE)
                {
                    m_nMoveAxis = AXIS_P;
                    m_dMoveDis = arg.toDouble();
                    m_ManualStatus = MANUAL_START;
                    m_WorkStatus = MOTION_WORK_MANUAL_MOTION;
                    //m_pScheduler->recvRunInfoFromAllModules("R杞寸Щ鍔 + arg + "mm");
                    return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("璇风瓑寰呯Щ鍔ㄦ寚浠ゅ畬鎴怺 杩愯鐘舵€侊細%1锛屾寚浠わ細%2锛屽唴瀹癸細%3 ]").arg(m_ManualStatus).arg(msg).arg(arg));
                    return;
                }
            }
        }
	 else if (msg == "pPMotion_world")
        {
            if (isServoEnable())// && isAxisXYEnableMove(!m_bIsDebugMode)
            {
                if (m_ManualStatus == MANUAL_IDLE)
                {
                    m_nMoveAxis = AXIS_P;
                    m_dMoveDis = arg.toDouble();
                    m_ManualStatus = MANUAL_WORLD_START;
                    m_WorkStatus = MOTION_WORK_MANUAL_MOTION;
                    //m_pScheduler->recvRunInfoFromAllModules("X杞寸Щ鍔 + arg + "mm");
			return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("璇风瓑寰呯Щ鍔ㄦ寚浠ゅ畬鎴怺 杩愯鐘舵€侊細%1锛屾寚浠わ細%2锛屽唴瀹癸細%3 ]").arg(m_ManualStatus).arg(msg).arg(arg));
                    return;
                }
            }
        }
      else if (msg == "mPMotion")
         {
             if (isServoEnable())// && isAxisXYEnableMove(!m_bIsDebugMode)
             {
                 if (m_ManualStatus == MANUAL_IDLE)
                 {
                     m_nMoveAxis = AXIS_M;
                     m_dMoveDis = arg.toDouble();
                     m_ManualStatus = MANUAL_START;
                     m_WorkStatus = MOTION_WORK_MANUAL_MOTION;
                     //m_pScheduler->recvRunInfoFromAllModules("R杞寸Щ鍔 + arg + "mm");
                     return;
                 }
                 else
                 {
                     m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("璇风瓑寰呯Щ鍔ㄦ寚浠ゅ畬鎴怺 杩愯鐘舵€侊細%1锛屾寚浠わ細%2锛屽唴瀹癸細%3 ]").arg(m_ManualStatus).arg(msg).arg(arg));
                     return;
                 }
             }
         }
      else if (msg == "mPMotion_world")
         {
             if (isServoEnable())// && isAxisXYEnableMove(!m_bIsDebugMode)
             {
                 if (m_ManualStatus == MANUAL_IDLE)
                 {
                     m_nMoveAxis = AXIS_M;
                     m_dMoveDis = arg.toDouble();
                     m_ManualStatus = MANUAL_WORLD_START;
                     m_WorkStatus = MOTION_WORK_MANUAL_MOTION;
                     //m_pScheduler->recvRunInfoFromAllModules("X杞寸Щ鍔 + arg + "mm");
             return;
                 }
                 else
                 {
                     m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("璇风瓑寰呯Щ鍔ㄦ寚浠ゅ畬鎴怺 杩愯鐘舵€侊細%1锛屾寚浠わ細%2锛屽唴瀹癸細%3 ]").arg(m_ManualStatus).arg(msg).arg(arg));
                     return;
                 }
             }
         }
	  else if (msg == "xNMotion")
        {
            if (isServoEnable())// && isAxisXYEnableMove(!m_bIsDebugMode)
            {
                if (m_ManualStatus == MANUAL_IDLE)
                {
                    m_nMoveAxis = AXIS_X;
                    m_dMoveDis = -arg.toDouble();
                    m_ManualStatus = MANUAL_START;
                    m_WorkStatus = MOTION_WORK_MANUAL_MOTION;
                    //m_pScheduler->recvRunInfoFromAllModules("X杞寸Щ鍔 + arg + "mm");
                    return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("璇风瓑寰呯Щ鍔ㄦ寚浠ゅ畬鎴怺 杩愯鐘舵€侊細%1锛屾寚浠わ細%2锛屽唴瀹癸細%3 ]").arg(m_ManualStatus).arg(msg).arg(arg));
                    return;
                }
            }
        }
	  else if (msg == "xNMotion_world")
        {
            if (isServoEnable())// && isAxisXYEnableMove(!m_bIsDebugMode)
            {
                if (m_ManualStatus == MANUAL_IDLE)
                {
                    m_nMoveAxis = AXIS_X;
                    m_dMoveDis = -arg.toDouble();
                    m_ManualStatus = MANUAL_WORLD_START;
                    m_WorkStatus = MOTION_WORK_MANUAL_MOTION;
                    //m_pScheduler->recvRunInfoFromAllModules("X杞寸Щ鍔 + arg + "mm");
			return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("璇风瓑寰呯Щ鍔ㄦ寚浠ゅ畬鎴怺 杩愯鐘舵€侊細%1锛屾寚浠わ細%2锛屽唴瀹癸細%3 ]").arg(m_ManualStatus).arg(msg).arg(arg));
                    return;
                }
            }
        }
        else if (msg == "yNMotion")
        {
            if (isServoEnable())// && isAxisXYEnableMove(!m_bIsDebugMode)
            {
                if (m_ManualStatus == MANUAL_IDLE)
                {
                    m_nMoveAxis = AXIS_Y;
                    m_dMoveDis = -arg.toDouble();
                    m_ManualStatus = MANUAL_START;
                    m_WorkStatus = MOTION_WORK_MANUAL_MOTION;
                    //m_pScheduler->recvRunInfoFromAllModules("Y杞寸Щ鍔 + arg + "mm");
                    return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("璇风瓑寰呯Щ鍔ㄦ寚浠ゅ畬鎴怺 杩愯鐘舵€侊細%1锛屾寚浠わ細%2锛屽唴瀹癸細%3 ]").arg(m_ManualStatus).arg(msg).arg(arg));
                    return;
                }
            }
        }
	  else if (msg == "yNMotion_world")
        {
            if (isServoEnable())// && isAxisXYEnableMove(!m_bIsDebugMode)
            {
                if (m_ManualStatus == MANUAL_IDLE)
                {
                    m_nMoveAxis = AXIS_Y;
                    m_dMoveDis = -arg.toDouble();
                    m_ManualStatus = MANUAL_WORLD_START;
                    m_WorkStatus = MOTION_WORK_MANUAL_MOTION;
                    //m_pScheduler->recvRunInfoFromAllModules("X杞寸Щ鍔 + arg + "mm");
			return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("璇风瓑寰呯Щ鍔ㄦ寚浠ゅ畬鎴怺 杩愯鐘舵€侊細%1锛屾寚浠わ細%2锛屽唴瀹癸細%3 ]").arg(m_ManualStatus).arg(msg).arg(arg));
                    return;
                }
            }
        }
        else if (msg == "zNMotion")
        {
            if (isServoEnable())// && isAxisXYEnableMove(!m_bIsDebugMode)
            {
                if (m_ManualStatus == MANUAL_IDLE)
                {
                    m_nMoveAxis = AXIS_Z;
                    m_dMoveDis = -arg.toDouble();
                    m_ManualStatus = MANUAL_START;
                    m_WorkStatus = MOTION_WORK_MANUAL_MOTION;
                    //m_pScheduler->recvRunInfoFromAllModules("Z杞寸Щ鍔 + arg + "mm");
                    return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("璇风瓑寰呯Щ鍔ㄦ寚浠ゅ畬鎴怺 杩愯鐘舵€侊細%1锛屾寚浠わ細%2锛屽唴瀹癸細%3 ]").arg(m_ManualStatus).arg(msg).arg(arg));
                    return;
                }
            }
        }
	 else if (msg == "zNMotion_world")
        {
            if (isServoEnable())// && isAxisXYEnableMove(!m_bIsDebugMode)
            {
                if (m_ManualStatus == MANUAL_IDLE)
                {
                    m_nMoveAxis = AXIS_Z;
                    m_dMoveDis = -arg.toDouble();
                    m_ManualStatus = MANUAL_WORLD_START;
                    m_WorkStatus = MOTION_WORK_MANUAL_MOTION;
                    //m_pScheduler->recvRunInfoFromAllModules("X杞寸Щ鍔 + arg + "mm");
			return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("璇风瓑寰呯Щ鍔ㄦ寚浠ゅ畬鎴怺 杩愯鐘舵€侊細%1锛屾寚浠わ細%2锛屽唴瀹癸細%3 ]").arg(m_ManualStatus).arg(msg).arg(arg));
                    return;
                }
            }
        }
        else if (msg == "rNMotion")
        {
            if (isServoEnable())// && isAxisXYEnableMove(!m_bIsDebugMode)
            {
                if (m_ManualStatus == MANUAL_IDLE)
                {
                    m_nMoveAxis = AXIS_R;
                    m_dMoveDis = -arg.toDouble();
                    m_ManualStatus = MANUAL_START;
                    m_WorkStatus = MOTION_WORK_MANUAL_MOTION;
                    //m_pScheduler->recvRunInfoFromAllModules("R杞寸Щ鍔 + arg + "mm");
                    return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("璇风瓑寰呯Щ鍔ㄦ寚浠ゅ畬鎴怺 杩愯鐘舵€侊細%1锛屾寚浠わ細%2锛屽唴瀹癸細%3 ]").arg(m_ManualStatus).arg(msg).arg(arg));
                    return;
                }
            }
        }
	 else if (msg == "rNMotion_world")
        {
            if (isServoEnable())// && isAxisXYEnableMove(!m_bIsDebugMode)
            {
                if (m_ManualStatus == MANUAL_IDLE)
                {
                    m_nMoveAxis = AXIS_R;
                    m_dMoveDis = -arg.toDouble();
                    m_ManualStatus = MANUAL_WORLD_START;
                    m_WorkStatus = MOTION_WORK_MANUAL_MOTION;
                    //m_pScheduler->recvRunInfoFromAllModules("X杞寸Щ鍔 + arg + "mm");
			return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("璇风瓑寰呯Щ鍔ㄦ寚浠ゅ畬鎴怺 杩愯鐘舵€侊細%1锛屾寚浠わ細%2锛屽唴瀹癸細%3 ]").arg(m_ManualStatus).arg(msg).arg(arg));
                    return;
                }
            }
        }
	 else if (msg == "wNMotion")
        {
            if (isServoEnable())// && isAxisXYEnableMove(!m_bIsDebugMode)
            {
                if (m_ManualStatus == MANUAL_IDLE)
                {
                    m_nMoveAxis = AXIS_W;
                    m_dMoveDis = -arg.toDouble();
                    m_ManualStatus = MANUAL_START;
                    m_WorkStatus = MOTION_WORK_MANUAL_MOTION;
                    //m_pScheduler->recvRunInfoFromAllModules("R杞寸Щ鍔 + arg + "mm");
                    return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("璇风瓑寰呯Щ鍔ㄦ寚浠ゅ畬鎴怺 杩愯鐘舵€侊細%1锛屾寚浠わ細%2锛屽唴瀹癸細%3 ]").arg(m_ManualStatus).arg(msg).arg(arg));
                    return;
                }
            }
        }
	 else if (msg == "wNMotion_world")
        {
            if (isServoEnable())// && isAxisXYEnableMove(!m_bIsDebugMode)
            {
                if (m_ManualStatus == MANUAL_IDLE)
                {
                    m_nMoveAxis = AXIS_W;
                    m_dMoveDis = -arg.toDouble();
                    m_ManualStatus = MANUAL_WORLD_START;
                    m_WorkStatus = MOTION_WORK_MANUAL_MOTION;
                    //m_pScheduler->recvRunInfoFromAllModules("X杞寸Щ鍔 + arg + "mm");
			return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("璇风瓑寰呯Щ鍔ㄦ寚浠ゅ畬鎴怺 杩愯鐘舵€侊細%1锛屾寚浠わ細%2锛屽唴瀹癸細%3 ]").arg(m_ManualStatus).arg(msg).arg(arg));
                    return;
                }
            }
        }
	 else if (msg == "pNMotion")
        {
            if (isServoEnable())// && isAxisXYEnableMove(!m_bIsDebugMode)
            {
                if (m_ManualStatus == MANUAL_IDLE)
                {
                    m_nMoveAxis = AXIS_P;
                    m_dMoveDis = -arg.toDouble();
                    m_ManualStatus = MANUAL_START;
                    m_WorkStatus = MOTION_WORK_MANUAL_MOTION;
                    //m_pScheduler->recvRunInfoFromAllModules("R杞寸Щ鍔 + arg + "mm");
                    return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("璇风瓑寰呯Щ鍔ㄦ寚浠ゅ畬鎴怺 杩愯鐘舵€侊細%1锛屾寚浠わ細%2锛屽唴瀹癸細%3 ]").arg(m_ManualStatus).arg(msg).arg(arg));
                    return;
                }
            }
        }
	 else if (msg == "pNMotion_world")
        {
            if (isServoEnable())// && isAxisXYEnableMove(!m_bIsDebugMode)
            {
                if (m_ManualStatus == MANUAL_IDLE)
                {
                    m_nMoveAxis = AXIS_P;
                    m_dMoveDis = -arg.toDouble();
                    m_ManualStatus = MANUAL_WORLD_START;
                    m_WorkStatus = MOTION_WORK_MANUAL_MOTION;
                    //m_pScheduler->recvRunInfoFromAllModules("X杞寸Щ鍔 + arg + "mm");
			return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("璇风瓑寰呯Щ鍔ㄦ寚浠ゅ畬鎴怺 杩愯鐘舵€侊細%1锛屾寚浠わ細%2锛屽唴瀹癸細%3 ]").arg(m_ManualStatus).arg(msg).arg(arg));
                    return;
                }
            }
        }
      else if (msg == "mNMotion")
         {
             if (isServoEnable())// && isAxisXYEnableMove(!m_bIsDebugMode)
             {
                 if (m_ManualStatus == MANUAL_IDLE)
                 {
                     m_nMoveAxis = AXIS_M;
                     m_dMoveDis = -arg.toDouble();
                     m_ManualStatus = MANUAL_START;
                     m_WorkStatus = MOTION_WORK_MANUAL_MOTION;
                     //m_pScheduler->recvRunInfoFromAllModules("R杞寸Щ鍔 + arg + "mm");
                     return;
                 }
                 else
                 {
                     m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("璇风瓑寰呯Щ鍔ㄦ寚浠ゅ畬鎴怺 杩愯鐘舵€侊細%1锛屾寚浠わ細%2锛屽唴瀹癸細%3 ]").arg(m_ManualStatus).arg(msg).arg(arg));
                     return;
                 }
             }
         }
      else if (msg == "mNMotion_world")
         {
             if (isServoEnable())// && isAxisXYEnableMove(!m_bIsDebugMode)
             {
                 if (m_ManualStatus == MANUAL_IDLE)
                 {
                     m_nMoveAxis = AXIS_M;
                     m_dMoveDis = -arg.toDouble();
                     m_ManualStatus = MANUAL_WORLD_START;
                     m_WorkStatus = MOTION_WORK_MANUAL_MOTION;
                     //m_pScheduler->recvRunInfoFromAllModules("X杞寸Щ鍔 + arg + "mm");
             return;
                 }
                 else
                 {
                     m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("璇风瓑寰呯Щ鍔ㄦ寚浠ゅ畬鎴怺 杩愯鐘舵€侊細%1锛屾寚浠わ細%2锛屽唴瀹癸細%3 ]").arg(m_ManualStatus).arg(msg).arg(arg));
                     return;
                 }
             }
         }
	 else if ((msg == "xPMove")||(msg == "xPMove_world")||(msg == "xNMove")||(msg == "xNMove_world"))
        {
            if (isServoEnable())// && isAxisXYEnableMove(!m_bIsDebugMode)
            {
                if (m_ManualStatus == MANUAL_IDLE)
                {
                    m_nMoveAxis = AXIS_X;
			if((msg == "xPMove")||(msg == "xPMove_world"))
			{
				m_dMoveDis=1;
			}
			else if((msg == "xNMove")||(msg == "xNMove_world"))
			{
				m_dMoveDis=-1;
			}
			if(arg=="start")
			{
                    	m_dMoverun = 1;
			}
			else
			{
				m_dMoverun = 0;
			}
			if((msg == "xPMove")||(msg == "xNMove"))
			{
                    	m_ManualStatus = MANUAL_MOVE_START;
			}			
			else if((msg == "xPMove_world")||(msg == "xNMove_world"))
			{
				m_ManualStatus = MANUAL_WORLDMOVE_START;
			}
                    m_WorkStatus = MOTION_WORK_MANUAL_MOTION;
                    //m_pScheduler->recvRunInfoFromAllModules("X杞寸Щ鍔 + arg + "mm");
			return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("璇风瓑寰呯Щ鍔ㄦ寚浠ゅ畬鎴怺 杩愯鐘舵€侊細%1锛屾寚浠わ細%2锛屽唴瀹癸細%3 ]").arg(m_ManualStatus).arg(msg).arg(arg));
                    return;
                }
            }
        }
        else if ((msg == "yPMove")||(msg == "yPMove_world")||(msg == "yNMove")||(msg == "yNMove_world"))
        {
            if (isServoEnable())// && isAxisXYEnableMove(!m_bIsDebugMode)
            {
                if (m_ManualStatus == MANUAL_IDLE)
                {
                    m_nMoveAxis = AXIS_Y;
			if((msg == "yPMove")||(msg == "yPMove_world"))
			{
				m_dMoveDis=1;
			}
			else if((msg == "yNMove")||(msg == "yNMove_world"))
			{
				m_dMoveDis=-1;
			}
			if(arg=="start")
			{
                    	m_dMoverun = 1;
			}
			else
			{
				m_dMoverun = 0;
			}
			if((msg == "yPMove")||(msg == "yNMove"))
			{
                    	m_ManualStatus = MANUAL_MOVE_START;
			}			
			else if((msg == "yPMove_world")||(msg == "yNMove_world"))
			{
				m_ManualStatus = MANUAL_WORLDMOVE_START;
			}
                    m_WorkStatus = MOTION_WORK_MANUAL_MOTION;
                    //m_pScheduler->recvRunInfoFromAllModules("X杞寸Щ鍔 + arg + "mm");
			return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("璇风瓑寰呯Щ鍔ㄦ寚浠ゅ畬鎴怺 杩愯鐘舵€侊細%1锛屾寚浠わ細%2锛屽唴瀹癸細%3 ]").arg(m_ManualStatus).arg(msg).arg(arg));
                    return;
                }
            }
        }
	  else if ((msg == "zPMove")||(msg == "zPMove_world")||(msg == "zNMove")||(msg == "zNMove_world"))
        {
            if (isServoEnable())// && isAxisXYEnableMove(!m_bIsDebugMode)
            {
                if (m_ManualStatus == MANUAL_IDLE)
                {
                    m_nMoveAxis = AXIS_Z;
			if((msg == "zPMove")||(msg == "zPMove_world"))
			{
				m_dMoveDis=1;
			}
			else if((msg == "zNMove")||(msg == "zNMove_world"))
			{
				m_dMoveDis=-1;
			}
			if(arg=="start")
			{
                    	m_dMoverun = 1;
			}
			else
			{
				m_dMoverun = 0;
			}
			if((msg == "zPMove")||(msg == "zNMove"))
			{
                    	m_ManualStatus = MANUAL_MOVE_START;
			}			
			else if((msg == "zPMove_world")||(msg == "zNMove_world"))
			{
				m_ManualStatus = MANUAL_WORLDMOVE_START;
			}
                    m_WorkStatus = MOTION_WORK_MANUAL_MOTION;
                    //m_pScheduler->recvRunInfoFromAllModules("X杞寸Щ鍔 + arg + "mm");
			return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("璇风瓑寰呯Щ鍔ㄦ寚浠ゅ畬鎴怺 杩愯鐘舵€侊細%1锛屾寚浠わ細%2锛屽唴瀹癸細%3 ]").arg(m_ManualStatus).arg(msg).arg(arg));
                    return;
                }
            }
        }
	  else if ((msg == "rPMove")||(msg == "rPMove_world")||(msg == "rNMove")||(msg == "rNMove_world"))
        {
            if (isServoEnable())// && isAxisXYEnableMove(!m_bIsDebugMode)
            {
                if (m_ManualStatus == MANUAL_IDLE)
                {
                    m_nMoveAxis = AXIS_R;
			if((msg == "rPMove")||(msg == "rPMove_world"))
			{
				m_dMoveDis=1;
			}
			else if((msg == "rNMove")||(msg == "rNMove_world"))
			{
				m_dMoveDis=-1;
			}
			if(arg=="start")
			{
                    	m_dMoverun = 1;
			}
			else
			{
				m_dMoverun = 0;
			}
			if((msg == "rPMove")||(msg == "rNMove"))
			{
                    	m_ManualStatus = MANUAL_MOVE_START;
			}			
			else if((msg == "rPMove_world")||(msg == "rNMove_world"))
			{
				m_ManualStatus = MANUAL_WORLDMOVE_START;
			}
                    m_WorkStatus = MOTION_WORK_MANUAL_MOTION;
                    //m_pScheduler->recvRunInfoFromAllModules("X杞寸Щ鍔 + arg + "mm");
			return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("璇风瓑寰呯Щ鍔ㄦ寚浠ゅ畬鎴怺 杩愯鐘舵€侊細%1锛屾寚浠わ細%2锛屽唴瀹癸細%3 ]").arg(m_ManualStatus).arg(msg).arg(arg));
                    return;
                }
            }
        }

	   else if ((msg == "wPMove")||(msg == "wPMove_world")||(msg == "wNMove")||(msg == "wNMove_world"))
        {
            if (isServoEnable())// && isAxisXYEnableMove(!m_bIsDebugMode)
            {
                if (m_ManualStatus == MANUAL_IDLE)
                {
                    m_nMoveAxis = AXIS_W;
			if((msg == "wPMove")||(msg == "wPMove_world"))
			{
				m_dMoveDis=1;
			}
			else if((msg == "wNMove")||(msg == "wNMove_world"))
			{
				m_dMoveDis=-1;
			}
			if(arg=="start")
			{
                    	m_dMoverun = 1;
			}
			else
			{
				m_dMoverun = 0;
			}
			if((msg == "wPMove")||(msg == "wNMove"))
			{
                    	m_ManualStatus = MANUAL_MOVE_START;
			}			
			else if((msg == "wPMove_world")||(msg == "wNMove_world"))
			{
				m_ManualStatus = MANUAL_WORLDMOVE_START;
			}
                    m_WorkStatus = MOTION_WORK_MANUAL_MOTION;
                    //m_pScheduler->recvRunInfoFromAllModules("X杞寸Щ鍔 + arg + "mm");
			return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("璇风瓑寰呯Щ鍔ㄦ寚浠ゅ畬鎴怺 杩愯鐘舵€侊細%1锛屾寚浠わ細%2锛屽唴瀹癸細%3 ]").arg(m_ManualStatus).arg(msg).arg(arg));
                    return;
                }
            }
        }
	    else if ((msg == "pPMove")||(msg == "pPMove_world")||(msg == "pNMove")||(msg == "pNMove_world"))
        {
            if (isServoEnable())// && isAxisXYEnableMove(!m_bIsDebugMode)
            {
                if (m_ManualStatus == MANUAL_IDLE)
                {
                    m_nMoveAxis = AXIS_P;
			if((msg == "pPMove")||(msg == "pPMove_world"))
			{
				m_dMoveDis=1;
			}
			else if((msg == "pNMove")||(msg == "pNMove_world"))
			{
				m_dMoveDis=-1;
			}
			if(arg=="start")
			{
                    	m_dMoverun = 1;
			}
			else
			{
				m_dMoverun = 0;
			}
			if((msg == "pPMove")||(msg == "pNMove"))
			{
                    	m_ManualStatus = MANUAL_MOVE_START;
			}			
			else if((msg == "pPMove_world")||(msg == "pNMove_world"))
			{
				m_ManualStatus = MANUAL_WORLDMOVE_START;
			}
                    m_WorkStatus = MOTION_WORK_MANUAL_MOTION;
                    //m_pScheduler->recvRunInfoFromAllModules("X杞寸Щ鍔 + arg + "mm");
			return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("璇风瓑寰呯Щ鍔ㄦ寚浠ゅ畬鎴怺 杩愯鐘舵€侊細%1锛屾寚浠わ細%2锛屽唴瀹癸細%3 ]").arg(m_ManualStatus).arg(msg).arg(arg));
                    return;
                }
            }
        }
      else if ((msg == "mPMove")||(msg == "mPMove_world")||(msg == "mNMove")||(msg == "mNMove_world"))
      {
          if (isServoEnable())// && isAxisXYEnableMove(!m_bIsDebugMode)
          {
              if (m_ManualStatus == MANUAL_IDLE)
              {
                  m_nMoveAxis = AXIS_M;
          if((msg == "mPMove")||(msg == "mPMove_world"))
          {
              m_dMoveDis=1;
          }
          else if((msg == "mNMove")||(msg == "mNMove_world"))
          {
              m_dMoveDis=-1;
          }
          if(arg=="start")
          {
                      m_dMoverun = 1;
          }
          else
          {
              m_dMoverun = 0;
          }
          if((msg == "mPMove")||(msg == "mNMove"))
          {
                      m_ManualStatus = MANUAL_MOVE_START;
          }
          else if((msg == "mPMove_world")||(msg == "mNMove_world"))
          {
              m_ManualStatus = MANUAL_WORLDMOVE_START;
          }
                  m_WorkStatus = MOTION_WORK_MANUAL_MOTION;
                  //m_pScheduler->recvRunInfoFromAllModules("X杞寸Щ鍔 + arg + "mm");
          return;
              }
              else
              {
                  m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("璇风瓑寰呯Щ鍔ㄦ寚浠ゅ畬鎴怺 杩愯鐘舵€侊細%1锛屾寚浠わ細%2锛屽唴瀹癸細%3 ]").arg(m_ManualStatus).arg(msg).arg(arg));
                  return;
              }
          }
      }
	  else if(msg=="setParamer")
	  {
	        if(arg=="all")
	  	{
                if (m_ManualStatus == MANUAL_IDLE)
                {
                   
                    m_CtrlStatus = SETTING_PARAMER;
                    m_WorkStatus = MOTION_WORK_MANUAL_SET;
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("设置参数"));
                    return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("条件不满足，设置失败"));
                    return;
                }
            }
            else if(arg=="linkparam")
	  	{
                if (m_ManualStatus == MANUAL_IDLE)
                {
                   
                    m_CtrlStatus = SETTING_LINKPARAMER;
                    m_WorkStatus = MOTION_WORK_MANUAL_SET;
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("设置连杆参数"));
                    return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("条件不满足，设置连杆失败"));
                    return;
                }
            }
	  }
	 	
	  else if(msg == "clear")
	 {
          
            {
                if (m_ManualStatus == MANUAL_IDLE)
                {
                   
                    m_CtrlStatus = SETTING_CLEAR;
                    m_WorkStatus = MOTION_WORK_MANUAL_SET;
                   
		     m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("清除错误"));
                    m_pScheduler->writeMsgToStatusBar("");	
                    return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("条件不满足，清除失败"));
                    return;
                }
            }

                	
        }
	   else if(msg == "setorian")
	 {
             
            {
                if (m_ManualStatus == MANUAL_IDLE)
                {
                   
                    m_CtrlStatus = SETTING_ORIAN;
		     if(arg=="yes")
		     {
		          m_nZeroaxis=1;//arg.toInt();
		     }
		     else
		     {
		          m_nZeroaxis=arg.toInt();
		     }
                    m_WorkStatus = MOTION_WORK_MANUAL_SET;
                    m_pScheduler->recvRunInfoFromAllModules("Set Orian");
                    return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("Set Orian"));
                    return;
                }
            }
        }
	 else if(msg == "mode")
	 {
          
            {
                if (m_ManualStatus == MANUAL_IDLE)
                {
                    m_mode=arg.toInt();
                    m_CtrlStatus = SETTING_MODE;
                    m_WorkStatus = MOTION_WORK_MANUAL_SET;
                    m_pScheduler->recvRunInfoFromAllModules("SetMode");
                    return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("SetMode"));
                    return;
                }
            }
        }
	 
	 else if(msg == "setToolstep1")
	 {
          
            {
                if (m_ManualStatus == MANUAL_IDLE)
                {
                    m_toolindex=arg.toInt();
			m_toolsetstep=1;
                    m_CtrlStatus = SETTING_TOOLS;
                    m_WorkStatus = MOTION_WORK_MANUAL_SET;
                    m_pScheduler->recvRunInfoFromAllModules("setToolstep1");
                    return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("setToolstep1"));
                    return;
                }
            }
        }
	  else if(msg == "setToolstep2")
	 {
          
            {
                if (m_ManualStatus == MANUAL_IDLE)
                {
                    m_toolindex=arg.toInt();
			m_toolsetstep=2;
                    m_CtrlStatus = SETTING_TOOLS;
                    m_WorkStatus = MOTION_WORK_MANUAL_SET;
                    m_pScheduler->recvRunInfoFromAllModules("setToolstep2");
                    return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("setToolstep2"));
                    return;
                }
            }
        }
	  else if(msg == "setZerostep")
	 {
          
            {
                if (m_ManualStatus == MANUAL_IDLE)
                {
                    
			m_zerosetstep=arg.toInt();
                    m_CtrlStatus = SETTING_ZERO_CALIBRATION;
                    m_WorkStatus = MOTION_WORK_MANUAL_SET;
                    m_pScheduler->recvRunInfoFromAllModules("setZeroStep");
                    return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("setZeroStep"));
                    return;
                }
            }
        }
	
         else if(msg == "setcurTool")
	 {
          
            {
                if (m_ManualStatus == MANUAL_IDLE)
                {
                    m_toolindex=arg.toInt();
			//m_toolsetstep=2;
                    m_CtrlStatus = SETTING_CURTOOL;
                    m_WorkStatus = MOTION_WORK_MANUAL_SET;
                    m_pScheduler->recvRunInfoFromAllModules("setCurTool");
                    return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("setCurTool"));
                    return;
                }
            }
        }
	else if(msg == "backzero")
	 {
	     
            if (isServoEnable())
            {
            	if ((m_WorkStatus != MOTION_WORK_NORMAL_START)
			&&(m_WorkStatus != MOTION_WORK_NORMAL_PROCESS)
			&&(m_WorkStatus != MOTION_WORK_NORMAL_END)
			&&(m_WorkStatus != MOTION_WORK_ERROR_WAITTING))
            	{
	                if (m_ManualStatus == MANUAL_IDLE)
	                {
	                    //m_toolindex=arg.toInt();
				//m_toolsetstep=2;
	                    m_ManualStatus = MANUAL_BACKZERO;
	                    m_WorkStatus = MOTION_WORK_MANUAL_MOTION;
	                    ((MotionControllerDevice *)m_pDevice)->InitGoHome();
	                    m_pScheduler->recvRunInfoFromAllModules("back to zero");
	                    return;
	                }
	                else
	                {
	                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("back to zero"));
	                    return;
	                }
            	}
		 else
		{
		        QMessageBox::information(NULL, QString::fromLocal8Bit("警告"), QString::fromLocal8Bit("程序运行中，请先停止！"));
		        qDebug()<<"系统状态"<<m_WorkStatus;
		}
            }
        }
	else if(msg == "GetVersion")
	 {
          
            {
                if (m_ManualStatus == MANUAL_IDLE)
                {
                    
                    m_ManualStatus = MANUAL_GETVERSION;
                    m_WorkStatus = MOTION_WORK_MANUAL_MOTION;
                    m_pScheduler->recvRunInfoFromAllModules("get version");
                    return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("get version"));
                    return;
                }
            }
        }     
        else if (msg == "servoEnable")
        {
            m_WorkStatus = MOTION_WORK_SERVO_ENABLE;
            if (arg == "yes")
            {
                m_bIsServoEnable = true;
                m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("伺服启动"));
                //m_pScheduler->writeMsgToStatusBar(QString::fromLocal8Bit("伺服启动"));
            }
            else
            {
                m_bIsServoEnable = false;
                m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("伺服关闭"));
                //m_pScheduler->writeMsgToStatusBar(QString::fromLocal8Bit("伺服关闭"));
            }
        }
	else if(msg == "GetBattery")
	{
          
            {
                if (m_ManualStatus == MANUAL_IDLE)
                {
                    
                    m_ManualStatus = MANUAL_GETBATTERY;
                    m_WorkStatus = MOTION_WORK_MANUAL_MOTION;
                    m_pScheduler->recvRunInfoFromAllModules("get Battery");
                    return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("get Battery"));
                    return;
                }
            }
        }
	else if(msg == "SetBattery")
	 {
          
            {
                if (m_ManualStatus == MANUAL_IDLE)
                {
                    
                    m_ManualStatus = MANUAL_SETBATTERY;
                    m_WorkStatus = MOTION_WORK_MANUAL_MOTION;
                    m_pScheduler->recvRunInfoFromAllModules("set Battery");
                    return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("set Battery"));
                    return;
                }
            }
        }
	else if(msg == "reset")
	 {
	      
	        {
		    if ((m_WorkStatus != MOTION_WORK_NORMAL_START)
				&&(m_WorkStatus != MOTION_WORK_NORMAL_PROCESS)
				&&(m_WorkStatus != MOTION_WORK_NORMAL_END)
				&&(m_WorkStatus != MOTION_WORK_ERROR_WAITTING))
		    {
		            if (m_ManualStatus == MANUAL_IDLE)
		            {
		               
		                m_CtrlStatus = SETTING_RESET;
		                m_WorkStatus = MOTION_WORK_MANUAL_SET;
		                m_pScheduler->recvRunInfoFromAllModules("Reset");
		                return;
		            }
		            else
		            {
		                m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("RESET"));
		                return;
		            }
		    }
		    else
			{
			        QMessageBox::information(NULL, QString::fromLocal8Bit("警告"), QString::fromLocal8Bit("程序运行中，请先停止！"));
			        qDebug()<<"系统状态"<<m_WorkStatus;
			}
	        }
	    }
	    else if(msg == "saveteachpoint")
		{
	          
	            {
	                if (m_ManualStatus == MANUAL_IDLE)
	                {
	                    m_startteachpoint=0;
	                    m_ManualStatus = MANUAL_SAVETEACHPOINT;
	                    m_WorkStatus = MOTION_WORK_MANUAL_MOTION;
	                    m_pScheduler->recvRunInfoFromAllModules("saveteachpoint");
	                    return;
	                }
	                else
	                {
	                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("saveteachpoint failed"));
	                    return;
	                }
	            }
	        }
		else if(msg == "getteachpoint")
		{
	          
	            {
	                if (m_ManualStatus == MANUAL_IDLE)
	                {
	                    
	                    m_ManualStatus = MANUAL_GETTEACHPOINT;
	                    m_WorkStatus = MOTION_WORK_MANUAL_MOTION;
	                    m_pScheduler->recvRunInfoFromAllModules("getteachpoint");
	                    return;
	                }
	                else
	                {
	                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("getteachpoint failed"));
	                    return;
	                }
	            }
	        }
		else if(msg == "movetopoint")
		 {
		     
	            if (isServoEnable())
	            {
	            	if ((m_WorkStatus != MOTION_WORK_NORMAL_START)
				&&(m_WorkStatus != MOTION_WORK_NORMAL_PROCESS)
				&&(m_WorkStatus != MOTION_WORK_NORMAL_END)
				&&(m_WorkStatus != MOTION_WORK_ERROR_WAITTING))
	            	{
		                if (m_ManualStatus == MANUAL_IDLE)
		                {
		                    m_movepointnum=arg.toInt();
				      if((m_pScheduler->m_pSystemParameter->teachpoint[m_movepointnum].index<1)
					  ||(m_pScheduler->m_pSystemParameter->teachpoint[m_movepointnum].index>1024))	  
					{
					        QMessageBox::information(NULL, QString::fromLocal8Bit("警告"), QString::fromLocal8Bit("请先添加示教点！"));
					        qDebug()<<"示教点序号"<<m_pScheduler->m_pSystemParameter->teachpoint[m_movepointnum].index;
						return ;
					}
				     (m_pScheduler->m_pSystemParameter->sys_ctrl.sendinput[0])&=~0x2;
		                    m_ManualStatus = MANUAL_MOVETOPOINT;
		                    m_WorkStatus = MOTION_WORK_MANUAL_MOTION;
		                    m_pScheduler->recvRunInfoFromAllModules("move to teachpoint");
		                    return;
		                }
		                else
		              {
				        QMessageBox::information(NULL, QString::fromLocal8Bit("警告"), QString::fromLocal8Bit("程序运行中，请先停止！"));
				        qDebug()<<"手动状态"<<m_ManualStatus;
				}
	            	}
			 else
			{
			        QMessageBox::information(NULL, QString::fromLocal8Bit("警告"), QString::fromLocal8Bit("程序运行中，请先停止！"));
			        qDebug()<<"系统状态"<<m_WorkStatus;
			}
	            }
	        }
	    	else if(msg == "downuserfile")
		{
	          
	            {
	                if (m_ManualStatus == MANUAL_IDLE)
	                {
	                    m_userfileblock=0;
	                    m_ManualStatus = MANUAL_DOWNUSERFILE;
	                    m_WorkStatus = MOTION_WORK_MANUAL_MOTION;
	                    m_pScheduler->recvRunInfoFromAllModules("downuserfile");
	                    return;
	                }
	                else
	                {
	                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("downuserfile failed"));
	                    return;
	                }
	            }
	        }
		else if(msg == "setaxisno")          
            {
                if (m_ManualStatus == MANUAL_IDLE)
                {
                    
                    m_ManualStatus = MANUAL_SETAXISNO;
                    m_WorkStatus = MOTION_WORK_MANUAL_MOTION;
                    m_pScheduler->recvRunInfoFromAllModules("set axisno");
                    return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("set axisno"));
                    return;
                }
            }
	    else if(msg == "getaxisno")          
            {
                if (m_ManualStatus == MANUAL_IDLE)
                {
                    
                    m_ManualStatus = MANUAL_GETAXISNO;
                    m_WorkStatus = MOTION_WORK_MANUAL_MOTION;
                    m_pScheduler->recvRunInfoFromAllModules("get axisno");
                    return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("get axisno"));
                    return;
                }
            }
	    else if(msg == "getsysparam")          
            {
                if (m_ManualStatus == MANUAL_IDLE)
                {
                    
                    m_ManualStatus = MANUAL_GETSYSPARAM;
                    m_WorkStatus = MOTION_WORK_MANUAL_MOTION;
                    m_pScheduler->recvRunInfoFromAllModules("get sysparam");
                    return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("get sysparam"));
                    return;
                }
            }
        
    }
   else
    {
        QMessageBox::information(NULL, QString::fromLocal8Bit("警告"), QString::fromLocal8Bit("系统状态不满足执行该指令！"));
        qDebug()<<"系统状态"<<m_WorkStatus;
    }

}

/*************************************************
  函数名称：runTimer()
  函数功能：运行仿真 定时器
  输入参数：
  输出参数：
  返 回 值：
  备    注：
**************************************************/
void MotionControllerModule::runTimer()
{
   
}

/*************************************************
  函数名称：procMotionModel()
  函数功能：正常加工前的准备工作
  输入参数：
  输出参数：
  返 回 值：
  备    注：
**************************************************/
void MotionControllerModule::procMotionModel()
{
    if(m_pEmulationModelHandler != NULL)
    {
        m_pEmulationModelHandler->unloadProcEmulationController(this, m_errInfo);
    }
    m_pEmulationModelHandler = ProcEmulationModel::loadProcEmulationController(this, m_errInfo);
     qDebug()<<"m_pEmulationModelHandler";
    m_errInfo.e_type = DEBUG_NO_ERROR;

    if(m_pEmulationModelHandler != NULL)
    {
        clearGCodeDataList();
        qDebug()<<"clearGCodeDataList end";
        m_errInfo.e_type = DEBUG_NO_ERROR;
        //m_pDevice->startDevice(m_errInfo);
	 qDebug()<<"startupProcEmulationController";
        //((MotionControllerDevice *)m_pDevice)->clearCanvsData();

        if(m_pEmulationModelHandler->startupProcEmulationController(this, &m_GCodeDataList, m_pProcessCacheHandler, m_errInfo))
        {
            m_GCodeActions = m_pEmulationModelHandler->getBaseActionHandle();
            m_WorkStatus = MOTION_WORK_NORMAL_START;//MOTION_WORK_NORMAL_PROCESS;
            //m_GCodeRunSerialNum = 0;//刚开始加工文件时初始化该值,由于MOTION_WORK_NORMAL_START状态同时用于暂停和暂停恢复，故将初始化放在此处
            setEmulationStatus(CAutoMachiningStateUi::MOTION_EMULATION_RUN);
        }
        else
        {
            clearGCodeDataList();
            //m_pEmulationModelHandler->unloadProcEmulationController(this, m_errInfo);
            m_pScheduler->recvErrMsgFromAllModules(m_errInfo);
            m_pScheduler->writeMsgToStatusBar(m_errInfo.toString(), STATUSBAR_ERROR_INFO);
        }
    }
    else
    {
        m_errInfo.insertDebug(DEBUG_CREAT_ERROR, "parseNGFileModel loadParseController error\n");
        m_pScheduler->recvErrMsgFromAllModules(m_errInfo);
    }
}

/*************************************************
  函数名称：run()
  函数功能：正常加工线程运行
  输入参数：
  输出参数：
  返 回 值：
  备    注：
**************************************************/
void MotionControllerModule::run()
{
    bool flag = true;
    addDevice(m_errInfo);

    if(((MotionControllerDevice *)m_pDevice)->creatTCPSocket() != 0)
    {
        m_errInfo.insertDebug(DEBUG_SERIOUS_ERROR, "TCP Socket 创建不成功，请检查网络连接并重新启动！");
        m_pScheduler->recvErrMsgFromAllModules(m_errInfo);

        //m_WorkStatus = MOTION_NO_CONNECT_NET;
        //m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "not connect");
    }
	if(((MotionControllerDevice *)m_pDevice)->creatUdPSocket() != 0)
    {
        m_errInfo.insertDebug(DEBUG_SERIOUS_ERROR, "Socket 创建不成功，请检查网络连接并重新启动！");
        m_pScheduler->recvErrMsgFromAllModules(m_errInfo);

        m_WorkStatus = MOTION_NO_CONNECT_NET;
        m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "not connect");
    }

    m_MotionTimer = new QTimer(this);
    connect(m_MotionTimer, SIGNAL(timeout()), this, SLOT(doMotion()), Qt::DirectConnection);
    m_MotionTimer->start(100);

    m_NetCtrlTimer = new QTimer(this);
    connect(m_NetCtrlTimer, SIGNAL(timeout()), this, SLOT(onnetctrltimeout()), Qt::DirectConnection);
    m_NetCtrlTimer->start(20);

    if(flag)
    {
        exec();
    }
}

/*************************************************
  函数名称：doMotion()
  函数功能：主工艺流程运行（正常加工线程）
  输入参数：
  输出参数：
  返 回 值：
  备    注：
**************************************************/
void MotionControllerModule::doMotion()
{
    //qDebug()<<"d st"<<m_WorkStatus;
    //qDebug()<<"m_WorkStatus = "<<m_WorkStatus;
    int sts = 1;
    int i, homeStep=0;
    switch (m_WorkStatus)
    {
    case MOTION_WORK_NORMAL_BOOT:
        //if(((MotionControllerDevice *)m_pDevice)->sendMsg(MSG_NORMAL_BOOT, m_errInfo))
        /*if(((MotionControllerDevice *)m_pDevice)->normalTurnOn(m_errInfo))
        {
            m_pScheduler->recvRunInfoFromAllModules("发送正常开机通知");
            m_WorkStatus = MOTION_WORK_SETTING_CRAFTPARM;//MOTION_WORK_IDLE;
        }
        else
        {
            if(m_errInfo.e_type != DEBUG_NO_ERROR)
            {
                m_WorkStatus = MOTION_WORK_ERROR_WAITTING;
                m_pScheduler->recvErrMsgFromAllModules(m_errInfo);
                m_pScheduler->writeMsgToStatusBar(m_errInfo.toString());
            }
        }*/
        m_WorkStatus=MOTION_WORK_IDLE;
        break;
    case MOTION_WORK_MANUAL_SET:
        switch (m_CtrlStatus)
        {
        case SETTING_RESET:
            qDebug()<<"reset start";
            if (((MotionControllerDevice *)m_pDevice)->controllerReset())
            {
                m_ManualStatus = MANUAL_IDLE;
                m_WorkStatus= MOTION_WORK_IDLE;
                m_pProcessCacheHandler->m_bIsManualMotion = true;
            }
            else
            {

            }
            break;

        case SETTING_CLEAR:
            qDebug()<<"clear Alarm";
            if (((MotionControllerDevice *)m_pDevice)->clearAlarm())
            {
                m_ManualStatus = MANUAL_IDLE;
                m_WorkStatus= MOTION_WORK_IDLE;
                m_pProcessCacheHandler->m_bIsManualMotion = true;
            }
            else
            {

            }
            break;

        case SETTING_ORIAN:
            qDebug()<<"set orian";
            if (((MotionControllerDevice *)m_pDevice)->setOrian())
            {
                m_ManualStatus = MANUAL_IDLE;
                m_WorkStatus= MOTION_WORK_IDLE;
                m_pProcessCacheHandler->m_bIsManualMotion = true;
            }
            else
            {

            }
            break;

        case SETTING_MODE:
            qDebug()<<"set mode";
            if (((MotionControllerDevice *)m_pDevice)->setMotionMode(m_mode))
            {
                m_ManualStatus = MANUAL_IDLE;
                m_WorkStatus= MOTION_WORK_IDLE;
                m_pProcessCacheHandler->m_bIsManualMotion = true;
            }
            else
            {

            }
            break;

        case SETTING_PARAMER:
            qDebug()<<"SET PARAM";
            if (((MotionControllerDevice *)m_pDevice)->setParamer())
            {
                m_ManualStatus = MANUAL_IDLE;
                m_WorkStatus= MOTION_WORK_IDLE;
                m_pProcessCacheHandler->m_bIsManualMotion = true;
            }
            else
            {

            }
            break;

        case SETTING_LINKPARAMER:
            qDebug()<<"SET LINKPARAM";
            if (((MotionControllerDevice *)m_pDevice)->setLinkParamer())
            {
                m_ManualStatus = MANUAL_IDLE;
                m_WorkStatus= MOTION_WORK_IDLE;
                m_pProcessCacheHandler->m_bIsManualMotion = true;
            }
            else
            {

            }
            break;

        case SETTING_TOOLS:
            qDebug()<<"set tools";
            if (((MotionControllerDevice *)m_pDevice)->setTools(m_toolindex,m_toolsetstep))
            {
                m_ManualStatus = MANUAL_IDLE;
                m_WorkStatus= MOTION_WORK_IDLE;
                m_pProcessCacheHandler->m_bIsManualMotion = true;
            }
            else
            {

            }
            break;

        case SETTING_CURTOOL:
            qDebug()<<"set tools";
            if (((MotionControllerDevice *)m_pDevice)->setcurTool(m_toolindex))
            {
                m_ManualStatus = MANUAL_IDLE;
                m_WorkStatus= MOTION_WORK_IDLE;
                m_pProcessCacheHandler->m_bIsManualMotion = true;
            }
            else
            {

            }
            break;

        case SETTING_ZERO_CALIBRATION:
            qDebug()<<"set zero calibration";
            if (((MotionControllerDevice *)m_pDevice)->setZeroCalibration(m_zerosetstep))
            {
                m_ManualStatus = MANUAL_IDLE;
                m_WorkStatus= MOTION_WORK_IDLE;
                m_pProcessCacheHandler->m_bIsManualMotion = true;
            }
            else
            {

            }
            break;

        default:break;
        }
        break;

    case MOTION_WORK_NORMAL_START:
        if (((MotionControllerDevice *)m_pDevice)->setMotionMode(2))//鏀瑰彉杩愬姩鐘舵€?
        {
            //m_MotionMode = Mode_Auto;
            m_WorkStatus = MOTION_WORK_NORMAL_PROCESS;
            
            //m_StartStatus = SETTING_PARM;
            m_GCodeRunSerialNum = 0;
            nCurrentLineSerialNum=0;
            m_nDelayTime=0;
            m_nDelayTime=0;
            qDebug()<<"control="<<m_nMotionControllerRunNum<<" last="<<m_nLastPartGCodeNum;

            m_pProcessCacheHandler->m_bIsManualMotion = false;
            m_pScheduler->recvRunInfoFromAllModules("启动FIFO运动成功");
        }

        break;
    case MOTION_WORK_NORMAL_PROCESS:
    {
        if(m_GCodeRunSerialNum < m_GCodeActions->size())
        {

            if (m_GCodeRunSerialNum < 0)
            {
                m_errInfo.insertDebug(DEBUG_SERIOUS_ERROR, "action 运行计数错误！");
                m_pScheduler->recvErrMsgFromAllModules(m_errInfo);
                m_pScheduler->writeMsgToStatusBar(m_errInfo.toString());
                m_WorkStatus = MOTION_WORK_ERROR_WAITTING;
                break;
            }
            nCurrentLineSerialNum = m_GCodeActions->at(m_GCodeRunSerialNum)->m_nLineSerialNum;
            qDebug()<<"cur"<<nCurrentLineSerialNum<<"now"<<m_GCodeRunSerialNum;
            runOneActions();
            
            
            //if((((MotionControllerDevice *)m_pDevice)->m_netctrl->gb_ctrl.sendinput[0])&0x2)
            if(m_pScheduler->m_pSystemParameter->sys_ctrl.sendinput[0]&0x2)
            {
                m_GCodeRunSerialNum=0;
                nCurrentLineSerialNum=0;
                m_WorkStatus = MOTION_WORK_IDLE;
                m_bisStopMotion = true;
                m_bisCycleRun=0;
                qDebug()<<"stop the motion";

                //m_nLastPartGCodeNum = m_nMotionControllerRunNum;
                //m_pProcessCacheHandler->m_nLastPartGCodeNum = m_nLastPartGCodeNum;
                m_pProcessCacheHandler->m_bIsManualMotion = true;
            }
        }
        else
        {

            if (m_GCodeActions->isEmpty())
            {
                m_WorkStatus = MOTION_WORK_IDLE;
                m_bisStopMotion = true;
                m_bisCycleRun=0;
                m_GCodeRunSerialNum=0;
                nCurrentLineSerialNum=0;
                qDebug()<<"stop the motion";

                //m_nLastPartGCodeNum = m_nMotionControllerRunNum;
                //m_pProcessCacheHandler->m_nLastPartGCodeNum = m_nLastPartGCodeNum;
                m_pProcessCacheHandler->m_bIsManualMotion = true;
                m_pScheduler->recvRunInfoFromAllModules("鍔犲伐瀹屾垚");
            }
            else
            {

                if (m_pScheduler->m_pSystemParameter->sys_ctrl.decode_id>= nCurrentLineSerialNum)//(m_nCurCtlRunId >= m_MotionCodeList.last()->m_nCodeId)//鎺у埗鍣ㄥ弽棣堝畬鎴?
                {
                    qDebug()<<"decode id"<<m_pScheduler->m_pSystemParameter->sys_ctrl.decode_id<<"cur"<<nCurrentLineSerialNum;
                    if((m_pScheduler->m_pSystemParameter->sys_ctrl.statebit& (1 << CTL_STS_IN_MOTION)) == 0)
                    {
                        if(m_GCodeActions->at(m_GCodeActions->size()-1)->m_type==ACTION_DELAY_TYPE)
                        {
                            if(((DelayAction *)m_GCodeActions->at(m_GCodeActions->size()-1))->m_nDelayTime)
                            {
                                m_nDelayTime+=100;
                                if(m_nDelayTime>((DelayAction *)m_GCodeActions->at(m_GCodeActions->size()-1))->m_nDelayTime)
                                {
                                    m_WorkStatus = MOTION_WORK_NORMAL_END;
                                    m_nDelayTime=0;
                                }
                            }
                            else
                            {
                                m_WorkStatus = MOTION_WORK_NORMAL_END;
                            }
                        }
                        else
                        {
                            m_WorkStatus = MOTION_WORK_NORMAL_END;
                        }
                        //m_nMotionCodeRunId = 0;
                        //motionDone();
                        //m_pScheduler->recvRunInfoFromAllModules("鍔犲伐瀹屾垚");
                    }

                }
                //if((((MotionControllerDevice *)m_pDevice)->m_netctrl->gb_ctrl.sendinput[0])&0x2)
                if(m_pScheduler->m_pSystemParameter->sys_ctrl.sendinput[0]&0x2)
                {
                    m_GCodeRunSerialNum=0;
                    nCurrentLineSerialNum=0;
                    m_WorkStatus = MOTION_WORK_IDLE;
                    m_bisStopMotion = true;
                    m_bisCycleRun=0;
                    qDebug()<<"stop the motion";

                    //m_nLastPartGCodeNum = m_nMotionControllerRunNum;
                    //m_pProcessCacheHandler->m_nLastPartGCodeNum = m_nLastPartGCodeNum;
                    m_pProcessCacheHandler->m_bIsManualMotion = true;
                }
            }
        }
    }
        break;
    case MOTION_WORK_NORMAL_END:
    {
        //int nLastLineSerialNum = m_GCodeActions->at(m_GCodeActions->size()-1)->m_nLineSerialNum;
        m_nDelayTime=0;
        if (m_bisCycleRun)//寰幆鍔犲伐
        {
            m_GCodeRunSerialNum=0;
            nCurrentLineSerialNum=0;
            m_WorkStatus = MOTION_WORK_NORMAL_PROCESS;
            //m_pScheduler->recvRunInfoFromAllModules(tr("寰幆鍔犲伐绗?1娆″畬鎴愶紝鍏?2娆?).arg(m_nCurCircleRunNum).arg(m_nCricleRunCnt));
            //if((((MotionControllerDevice *)m_pDevice)->m_netctrl->gb_ctrl.sendinput[0])&0x2)
            if(m_pScheduler->m_pSystemParameter->sys_ctrl.sendinput[0]&0x2)
            {
                m_WorkStatus = MOTION_WORK_IDLE;
                m_bisStopMotion = true;
                m_bisCycleRun=0;
                qDebug()<<"stop the motion";

                //m_nLastPartGCodeNum = m_nMotionControllerRunNum;
                //m_pProcessCacheHandler->m_nLastPartGCodeNum = m_nLastPartGCodeNum;
                m_pProcessCacheHandler->m_bIsManualMotion = true;
            }
        }
        else
        {
            /*if(m_pScheduler->m_pSystemParameter->sys_ctrl.decode_id>= nLastLineSerialNum)// (m_nMotionCodeRunId == m_nMotionCodeLastId + 1)
            {
                qDebug()<<"End Motion"<<endl;


            }
            else*/
            {
                qDebug()<<"m_nMontionCtlState & (1 << CTL_STS_IN_MOTION)) == 0:"<<((m_pScheduler->m_pSystemParameter->sys_ctrl.statebit & (1 << CTL_STS_IN_MOTION)) == 0);
                if((m_pScheduler->m_pSystemParameter->sys_ctrl.statebit & (1 << CTL_STS_IN_MOTION)) == 0 )
                {
                    m_GCodeRunSerialNum=0;
                    nCurrentLineSerialNum=0;
                    m_WorkStatus = MOTION_WORK_IDLE;
                    m_bisStopMotion = true;
                    m_bisCycleRun=0;
                    qDebug()<<"stop the motion";

                    //m_nLastPartGCodeNum = m_nMotionControllerRunNum;
                    //m_pProcessCacheHandler->m_nLastPartGCodeNum = m_nLastPartGCodeNum;
                    m_pProcessCacheHandler->m_bIsManualMotion = true;
                }
                else  if(m_pScheduler->m_pSystemParameter->sys_ctrl.sendinput[0]&0x2)//if((((MotionControllerDevice *)m_pDevice)->m_netctrl->gb_ctrl.sendinput[0])&0x2)
                {
                    m_GCodeRunSerialNum=0;
                    nCurrentLineSerialNum=0;
                    m_WorkStatus = MOTION_WORK_IDLE;
                    m_bisStopMotion = true;
                    m_bisCycleRun=0;
                    qDebug()<<"stop the motion";

                    //m_nLastPartGCodeNum = m_nMotionControllerRunNum;
                    //m_pProcessCacheHandler->m_nLastPartGCodeNum = m_nLastPartGCodeNum;
                    m_pProcessCacheHandler->m_bIsManualMotion = true;
                }

            }
        }
    }
        break;

    case MOTION_WORK_ERROR_WAITTING	:
        if(m_pScheduler->m_pSystemParameter->sys_ctrl.sendinput[0]&0x2)//if((((MotionControllerDevice *)m_pDevice)->m_netctrl->gb_ctrl.sendinput[0])&0x2)
        {
            m_GCodeRunSerialNum=0;
            nCurrentLineSerialNum=0;
            m_WorkStatus = MOTION_WORK_IDLE;
            m_bisStopMotion = true;
            m_bisCycleRun=0;
            qDebug()<<"stop the motion";

            //m_nLastPartGCodeNum = m_nMotionControllerRunNum;
            //m_pProcessCacheHandler->m_nLastPartGCodeNum = m_nLastPartGCodeNum;
            m_pProcessCacheHandler->m_bIsManualMotion = true;
        }
        break;
    case MOTION_WORK_STEPRUN_WAITTING:
        //qDebug()<<"step waitting";
        break;


    case MOTION_WORK_MANUAL_MOTION:
        switch (m_ManualStatus)
        {
        case MANUAL_START:
            qDebug()<<"manual start";
            if (((MotionControllerDevice *)m_pDevice)->startMotion(m_nMoveAxis, m_dMoveDis,m_pScheduler->m_pSystemParameter->sysvel))
            {
                m_ManualStatus = MANUAL_IDLE;
                //m_WorkStatus = MOTION_WORK_IDLE;
                m_pProcessCacheHandler->m_bIsManualMotion = true;
            }
            else
            {

            }
            break;
        case MANUAL_WORLD_START:
            qDebug()<<"manual start";
            if (((MotionControllerDevice *)m_pDevice)->startWorldMotion(m_nMoveAxis, m_dMoveDis,m_pScheduler->m_pSystemParameter->sysvel))
            {
                m_ManualStatus = MANUAL_IDLE;
                //m_WorkStatus = MOTION_WORK_IDLE;
                m_pProcessCacheHandler->m_bIsManualMotion = true;
            }
            else
            {

            }
            break;

        case MANUAL_MOVE_START:
            qDebug()<<"manual start";
            if (((MotionControllerDevice *)m_pDevice)->startMove(m_nMoveAxis, m_dMoveDis,m_dMoverun,m_pScheduler->m_pSystemParameter->sysvel))
            {
                m_ManualStatus = MANUAL_IDLE;
                //m_WorkStatus = MOTION_WORK_IDLE;
                m_pProcessCacheHandler->m_bIsManualMotion = true;
            }
            else
            {

            }
            break;

        case MANUAL_WORLDMOVE_START:
            qDebug()<<"manual start";
            if (((MotionControllerDevice *)m_pDevice)->startWorldMove(m_nMoveAxis, m_dMoveDis,m_dMoverun,m_pScheduler->m_pSystemParameter->sysvel))
            {
                m_ManualStatus = MANUAL_IDLE;
                //m_WorkStatus = MOTION_WORK_IDLE;
                m_pProcessCacheHandler->m_bIsManualMotion = true;
            }
            else
            {

            }
            break;


        case MANUAL_MOVSTOP:
            qDebug()<<"manual start";
            if (((MotionControllerDevice *)m_pDevice)->startMotion(m_nMoveAxis, m_dMoveDis,m_pScheduler->m_pSystemParameter->sysvel))
            {
                m_ManualStatus = MANUAL_IDLE;
                //m_WorkStatus = MOTION_WORK_IDLE;
                m_pProcessCacheHandler->m_bIsManualMotion = true;
            }
            else
            {

            }
            break;
        case MANUAL_BACKZERO:
            qDebug()<<"manual backzero";
            if (((MotionControllerDevice *)m_pDevice)->axisEnable(1))
            {
                if (((MotionControllerDevice *)m_pDevice)->setMotionMode(2))
                {
                    for(i=0;i<4;i++){
                        if (((MotionControllerDevice *)m_pDevice)->backHome(homeStep) == 1){ // ³É¹¦
                            if(homeStep>=4){  // Íê³É
                                m_ManualStatus = MANUAL_IDLE;
                                m_pProcessCacheHandler->m_bIsManualMotion = true;
                                break;
                            }
                        }else{  // »º³åÇø ÒÑÂú
                            break;
                        }
                    }
                }
            }
            break;
        case MANUAL_MOVETOPOINT:
            qDebug()<<"manual move to point"<<m_movepointnum;
            if (((MotionControllerDevice *)m_pDevice)->axisEnable(1))
            {
                if (((MotionControllerDevice *)m_pDevice)->setMotionMode(2))
                {

                    if (((MotionControllerDevice *)m_pDevice)->movetoteachpoint(m_movepointnum))
                    {
                        m_ManualStatus = MANUAL_IDLE;
                        //m_WorkStatus = MOTION_WORK_IDLE;
                        m_pProcessCacheHandler->m_bIsManualMotion = true;
                    }
                    else
                    {

                    }
                }
            }
            break;
        case MANUAL_GETVERSION:
            qDebug()<<"manual getversion";
            if (((MotionControllerDevice *)m_pDevice)->getVersion())
            {

                m_ManualStatus = MANUAL_IDLE;
                //m_WorkStatus = MOTION_WORK_IDLE;
                m_pProcessCacheHandler->m_bIsManualMotion = true;

            }
            break;
        case MANUAL_GETBATTERY:
            qDebug()<<"manual GETBATTERY";
            if (((MotionControllerDevice *)m_pDevice)->getBattery())
            {

                m_ManualStatus = MANUAL_IDLE;
                //m_WorkStatus = MOTION_WORK_IDLE;
                m_pProcessCacheHandler->m_bIsManualMotion = true;

            }
            break;
        case MANUAL_SETBATTERY:
            qDebug()<<"manual SETBATTERY";
            if (((MotionControllerDevice *)m_pDevice)->setBattery())
            {

                m_ManualStatus = MANUAL_IDLE;
                //m_WorkStatus = MOTION_WORK_IDLE;
                m_pProcessCacheHandler->m_bIsManualMotion = true;

            }
            break;
        case MANUAL_SAVETEACHPOINT:
            qDebug()<<"manual SAVETEACHPOINT";  // ±£´æÊ¾½Ìµã
            if(!getSystemScheduleHandle()->NetIsConnect()){ // ÍøÂç¶Ï¿ªÊ±
                m_ManualStatus = MANUAL_IDLE;
                m_pProcessCacheHandler->m_bIsManualMotion = true;
                break;
            }
            if (saveTeachPoint())
            {

                m_ManualStatus = MANUAL_SAVETEACHPOINTTEXT;
                //m_ManualStatus = MANUAL_IDLE;
                //m_pProcessCacheHandler->m_bIsManualMotion = true;

            }
            break;
        case MANUAL_SAVETEACHPOINTTEXT:
            qDebug()<<"manual SAVETEACHPOINTTEXT";  // ±£´æ×¢ÊÍ
            if(!getSystemScheduleHandle()->NetIsConnect()){ // ÍøÂç¶Ï¿ªÊ±
                m_ManualStatus = MANUAL_IDLE;
                m_pProcessCacheHandler->m_bIsManualMotion = true;
                break;
            }
            if (saveTeachPointText())
            {

                m_ManualStatus = MANUAL_IDLE;

                m_pProcessCacheHandler->m_bIsManualMotion = true;

            }
            break;
        case MANUAL_GETTEACHPOINT:
            qDebug()<<"manual SAVETEACHPOINT";
            if (((MotionControllerDevice *)m_pDevice)->getTeachPoint())
            {

                m_ManualStatus = MANUAL_IDLE;
                //m_WorkStatus = MOTION_WORK_IDLE;
                m_pProcessCacheHandler->m_bIsManualMotion = true;

            }
            break;
        case MANUAL_DOWNUSERFILE:
            qDebug()<<"manual DOWNUSERFILE";
            //if (((MotionControllerDevice *)m_pDevice)->saveUserFile())
            if (downUserFile())
            {

                m_ManualStatus = MANUAL_IDLE;
                //m_WorkStatus = MOTION_WORK_IDLE;
                m_pProcessCacheHandler->m_bIsManualMotion = true;

            }
            break;

        case MANUAL_SETAXISNO:
            qDebug()<<"manual SETAXISNO";
            if (((MotionControllerDevice *)m_pDevice)->setAxisNo())
                //if (downUserFile())
            {

                m_ManualStatus = MANUAL_IDLE;
                //m_WorkStatus = MOTION_WORK_IDLE;
                m_pProcessCacheHandler->m_bIsManualMotion = true;

            }
            break;
        case MANUAL_GETAXISNO:
            qDebug()<<"manual SETAXISNO";
            if (((MotionControllerDevice *)m_pDevice)->getAxisNo())
                //if (downUserFile())
            {

                m_ManualStatus = MANUAL_IDLE;
                //m_WorkStatus = MOTION_WORK_IDLE;
                m_pProcessCacheHandler->m_bIsManualMotion = true;

            }
            break;

        case MANUAL_GETSYSPARAM:
            qDebug()<<"manual GETSYSPARAM";
            if (((MotionControllerDevice *)m_pDevice)->getParamer())
                //if (downUserFile())
            {

                m_ManualStatus = MANUAL_IDLE;
                //m_WorkStatus = MOTION_WORK_IDLE;
                m_pProcessCacheHandler->m_bIsManualMotion = true;

            }
            break;
        case MANUAL_RUN:
            if (m_nMoveAxis != AXIS_ALL)
            {
                if (((MotionControllerDevice *)m_pDevice)->manualMove(m_nMoveAxis , m_dMoveDis, m_errInfo))
                {
                    m_ManualStatus = MANUAL_WAIT;
                }
                else
                {
                    if(m_errInfo.e_type != DEBUG_NO_ERROR)
                    {
                        m_WorkStatus = MOTION_WORK_ERROR_WAITTING;
                        m_pScheduler->recvErrMsgFromAllModules(m_errInfo);
                        m_pScheduler->writeMsgToStatusBar(m_errInfo.toString());
                    }
                }
            }
            else
            {
                if (((MotionControllerDevice *)m_pDevice)->manualMove(m_errInfo, m_axisMoveDis.x, m_axisMoveDis.y, m_axisMoveDis.z, m_axisMoveDis.w, m_axisMoveDis.p, m_axisMoveDis.r))
                {
                    m_ManualStatus = MANUAL_WAIT;
                }
                else
                {
                    if(m_errInfo.e_type != DEBUG_NO_ERROR)
                    {
                        m_WorkStatus = MOTION_WORK_ERROR_WAITTING;
                        m_pScheduler->recvErrMsgFromAllModules(m_errInfo);
                        m_pScheduler->writeMsgToStatusBar(m_errInfo.toString());
                    }
                }
            }
            break;
        case MANUAL_WAIT:
            if(((MotionControllerDevice *)m_pDevice)->getSts(&sts, m_errInfo))
            {
                qDebug()<<"manualsts="<<sts;
                if ((sts & 0x02) == 0)
                {
                    m_ManualStatus = MANUAL_IDLE;
                    m_WorkStatus = MOTION_WORK_IDLE;
                    //m_nManualCodeRunNum++;
                    //m_pProcessCacheHandler->m_nManualCodeRunNum++;
                    //m_pProcessCacheHandler->m_bIsManualMotion = false;

                    //                    m_nLastPartGCodeNum = m_nMotionControllerRunNum;
                    //                    m_pProcessCacheHandler->m_nLastPartGCodeNum = m_nLastPartGCodeNum;
                    m_nLastPartGCodeNum++;
                    m_pProcessCacheHandler->m_nLastPartGCodeNum++;
                    qDebug()<<"Done";
                }
            }
            else
            {
                if(m_errInfo.e_type != DEBUG_NO_ERROR)
                {
                    m_WorkStatus = MOTION_WORK_ERROR_WAITTING;
                    m_pScheduler->recvErrMsgFromAllModules(m_errInfo);
                    m_pScheduler->writeMsgToStatusBar(m_errInfo.toString());
                }
            }
            break;
        default:
            break;
        }//end of switch (m_ManualStatus){}
        break;
    case MOTION_WORK_SETTING_PARM:
        //if (settingParmToController())
    {
        m_WorkStatus = MOTION_WORK_IDLE;
    }
        /*else
        {
            m_WorkStatus = MOTION_WORK_ERROR_WAITTING;
        }*/
        break;
    case MOTION_WORK_SETTING_CRAFTPARM:
        //if (settingCraftParmToController())
    {
        m_WorkStatus = MOTION_WORK_IDLE;//MOTION_WORK_SETTING_PARM;
    }
        /*else
        {
            m_WorkStatus = MOTION_WORK_ERROR_WAITTING;
        }*/
        break;

    case MOTION_WORK_EMERGENCY_STOP:
        if(((MotionControllerDevice *)m_pDevice)->emergencyStop(m_errInfo))
        {
            m_pScheduler->recvRunInfoFromAllModules("紧急停止指令发送成功");
            m_WorkStatus = MOTION_EMERGENCY_STOP_WAITTING;//MOTION_WORK_IDLE;
        }
        else
        {
            m_pScheduler->recvRunInfoFromAllModules("紧急停止指令发送失败");
            if(m_errInfo.e_type != DEBUG_NO_ERROR)
            {
                m_pScheduler->recvErrMsgFromAllModules(m_errInfo);
                m_WorkStatus = MOTION_WORK_ERROR_WAITTING;
                m_pScheduler->writeMsgToStatusBar(m_errInfo.toString());
            }
        }
        break;
    case MOTION_RE_CONNECT_NET:
    {
        if(((MotionControllerDevice *)m_pDevice)->reConnectNet())
        {
            m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "reconnect success");
        }
        else
        {
            m_pScheduler->recvRunInfoFromAllModules("网络重连不成功");
            //m_pScheduler->writeMsgToStatusBar("网络重连不成功");//domotion()需要在主线程才能不会报错！（这里暂时将run()创建的线程移动到主线程）
            m_WorkStatus = MOTION_NO_CONNECT_NET;
        }
    }break;
    case MOTION_CLOSE_CONNECT_NET:
        if (((MotionControllerDevice *)m_pDevice)->closeNet())
        {
            //m_pScheduler->recvRunInfoFromAllModules("断开网络连接成功");
            //m_pScheduler->writeMsgToStatusBar("断开网络连接成功");
            m_WorkStatus = MOTION_NO_CONNECT_NET;
        }
        else
        {
            m_pScheduler->recvRunInfoFromAllModules("断开网络连接失败");
            //m_pScheduler->writeMsgToStatusBar("断开网络连接失败");
            m_WorkStatus = MOTION_WORK_IDLE;
        }
        break;
    case MOTION_NO_CONNECT_NET:
        break;

    case MOTION_WORK_SERVO_ENABLE:
        if (((MotionControllerDevice *)m_pDevice)->axisEnable(m_bIsServoEnable))
        {
            m_WorkStatus = MOTION_WORK_IDLE;
            qDebug()<<"enable"<<m_bIsServoEnable;
            //m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("浼烘湇鍚姩/鍏抽棴鎸囦护鍙戦€佹垚鍔));
        }
        break;

    case MOTION_CLEAR_ERROR:
        if(((MotionControllerDevice *)m_pDevice)->clearSts(m_errInfo))
        {
            m_pScheduler->recvRunInfoFromAllModules("清除控制器状态成功");
            m_WorkStatus = MOTION_WORK_IDLE;
        }
        else
        {
            m_pScheduler->recvRunInfoFromAllModules("清除控制器状态失败");
            if(m_errInfo.e_type != DEBUG_NO_ERROR)
            {
                m_pScheduler->recvErrMsgFromAllModules(m_errInfo);
                m_WorkStatus = MOTION_WORK_ERROR_WAITTING;
                m_pScheduler->writeMsgToStatusBar(m_errInfo.toString());
            }
        }
        break;
    case MOTION_EMERGENCY_STOP_CLEAR:
        if(((MotionControllerDevice *)m_pDevice)->clearEmergencyStop(m_errInfo))
        {
            m_pScheduler->recvRunInfoFromAllModules("清除控制器急停状态成功");
            m_WorkStatus = MOTION_WORK_NORMAL_BOOT;
        }
        else
        {
            m_pScheduler->recvRunInfoFromAllModules("清除控制器急停状态失败");
            if(m_errInfo.e_type != DEBUG_NO_ERROR)
            {
                m_pScheduler->recvErrMsgFromAllModules(m_errInfo);
                m_WorkStatus = MOTION_WORK_ERROR_WAITTING;
                m_pScheduler->writeMsgToStatusBar(m_errInfo.toString());
            }
        }
        break;
    default:
        break;
    }
    if(m_nPidCmd)
    {
        ((MotionControllerDevice *)m_pDevice)->PIDParam(m_nPidCmd,m_nPidAxis,m_nPidLen);
        m_nPidCmd=0;
    }
    if(m_nCanDebug)
    {
        ((MotionControllerDevice *)m_pDevice)->setCanDebug(m_nCanDebuglevel);
        m_nCanDebug=0;
    }
    //qDebug()<<"d en"<<m_WorkStatus;
    feedbackSystemState();
    //((MotionControllerDevice *)m_pDevice)->m_netctrl->ontimeout();
}

void MotionControllerModule::onnetctrltimeout()
{
	((MotionControllerDevice *)m_pDevice)->m_netctrl->ontimeout();
}

/*************************************************
  函数名称：systemReset()
  函数功能：系统复位
  输入参数：
  输出参数：
  返 回 值：
  备    注：
**************************************************/
void MotionControllerModule::systemReset()
{
    initMotionControllerModule();
    m_pProcessCacheHandler->initProcessCache();
    clearGCodeDataList();
    ((MotionControllerDevice *)m_pDevice)->initMotionControllerDevice();
    setEmulationStatus(CAutoMachiningStateUi::MOTION_EMULATION_IDLE);

    restartEmulation();
    setProcessBarValue(0);
    m_errInfo.e_type = DEBUG_NO_ERROR;
}

// ±£´æÊ¾½Ìµã µ½ÏÂÎ»»ú
int MotionControllerModule::saveTeachPoint()
{
    int cnt;
    
     //static int overcnt=0;
     if(m_startteachpoint<((TEACHPIONTNUM/6)*6))
     {
           for(;m_startteachpoint<((TEACHPIONTNUM/6)*6);)
           {
		    if (((MotionControllerDevice *)m_pDevice)->PutTeachPoint(m_startteachpoint, m_startteachpoint+5, (unsigned char*)&m_pScheduler->getSystemParameterHandler()->teachpoint[m_startteachpoint]))
		    {
		        m_startteachpoint+=6;
		        cnt = (double)m_startteachpoint/TEACHPIONTNUM*10;
		        getSystemParameterHandle()->TeachSaveCnt = cnt<1?1:cnt;
			 /*if(overcnt++>20)
			 {
			 	overcnt=0;
			 	break;
			 }*/
		    }
		    else 
		    {
		    	 return false;
		    }
           }
     } 
     else if((m_startteachpoint>=((TEACHPIONTNUM/6)*6))&&(m_startteachpoint<TEACHPIONTNUM))
     {
            if (((MotionControllerDevice *)m_pDevice)->PutTeachPoint(((TEACHPIONTNUM/6)*6), TEACHPIONTNUM-1, (unsigned char*)&m_pScheduler->getSystemParameterHandler()->teachpoint[((TEACHPIONTNUM/6)*6)]))
	    {
	        m_startteachpoint=TEACHPIONTNUM;
	    }		
     }
     else if(m_startteachpoint==TEACHPIONTNUM)
     {
            if (((MotionControllerDevice *)m_pDevice)->PutSaveTeachPoint())
	    {
	        //m_startteachpoint+=1;
		 m_startteachpoint=0;//
		 return true;
	    }
     }
     else
     {
     	   m_startteachpoint=TEACHPIONTNUM;
     }
     return false;
}

// ±£´æÊ¾½Ìµã×¢ÊÍ µ½ÏÂÎ»»ú
int MotionControllerModule::saveTeachPointText()
{
    int cnt;
    
     //static int overcnt=0;
     if(m_startteachpoint<(TEACHPIONTNUM))
     {
           for(;m_startteachpoint<(TEACHPIONTNUM);)
           {
		    if (((MotionControllerDevice *)m_pDevice)->PutTeachPointText(m_startteachpoint, m_startteachpoint, (unsigned char*)&m_pScheduler->getSystemParameterHandler()->teachpointtext[m_startteachpoint][0]))
		    {
		        m_startteachpoint+=1;
			cnt = 10+(double)m_startteachpoint/TEACHPIONTNUM*90;
		        getSystemParameterHandle()->TeachSaveCnt = cnt;
			 /*if(overcnt++>20)
			 {
			 	overcnt=0;
			 	break;
			 }*/
		    }
		    else 
		    {
		    	 return false;
		    }
           }
     } 
     /*else if((m_startteachpoint>=((TEACHPIONTNUM/6)*6))&&(m_startteachpoint<TEACHPIONTNUM))
     {
            if (((MotionControllerDevice *)m_pDevice)->PutTeachPoint(((TEACHPIONTNUM/6)*6), TEACHPIONTNUM-1, (unsigned char*)&m_pScheduler->getSystemParameterHandler()->teachpoint[((TEACHPIONTNUM/6)*6)]))
	    {
	        m_startteachpoint=TEACHPIONTNUM;
	    }		
     }*/
     else if(m_startteachpoint>=TEACHPIONTNUM)
     {
            if (((MotionControllerDevice *)m_pDevice)->PutSaveTeachPointText())
	    {
	        //m_startteachpoint+=1;
	        m_startteachpoint=0;
		getSystemParameterHandle()->TeachSaveCnt = 100;
		 return true;
	    }
     }
     else
     {
     	   m_startteachpoint=TEACHPIONTNUM;
     }
     return false;
}


int MotionControllerModule::downUserFile()
{
    
     //static int overcnt=0;
     int ret=0;
	 ret=((MotionControllerDevice *)m_pDevice)->saveUserFile(0, m_userfileblock,0);
   
		    if (3==ret)
		    {
		         m_userfileblock=0;
			 return true;
		    }
		    else  if (2==ret)
		    {
		         m_userfileblock++;
			 return false;
		    }
		    else  if (1==ret)
		    {
		         m_userfileblock++;
			 return false;
		    }
		    else 
		    {
		    	 return false;
		    }
     
     return false;
}

/*************************************************
  函数名称：runOneActions()
  函数功能：执行一个action
  输入参数：
  输出参数：
  返 回 值：
  备    注：
**************************************************/
bool MotionControllerModule::runOneActions()
{
    static bool runIOWait = true;
    switch (m_GCodeActions->at(m_GCodeRunSerialNum)->m_type)
    {
    case ACTION_MG_TYPE:
        qDebug()<<"mRun"<<m_nMotionControllerRunNum<<"M"<<m_pProcessCacheHandler->m_nMCodeCnt<<"cur"<<m_GCodeRunSerialNum<<"last"<<m_nLastPartGCodeNum;
        if (m_nMotionControllerRunNum + m_pProcessCacheHandler->m_nMCodeCnt - m_nLastPartGCodeNum == m_GCodeRunSerialNum)//等待运行到当前位置
        {
            int nM = ((MGAction *)m_GCodeActions->at(m_GCodeRunSerialNum))->m_nMGCode;
            m_GCodeRunSerialNum++;
            m_pProcessCacheHandler->m_nMCodeCnt++;
            switch (nM)
            {
            case 0://程序停止
                m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "pause emulation");
                m_pScheduler->recvRunInfoFromAllModules("M00 程序停止");
                //m_pScheduler->writeMsgToStatusBar("M00 程序停止");
                break;
            case 1://选择停止
                if (m_bIsM01Effect)
                {
                    m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "pause emulation");
                    m_pScheduler->recvRunInfoFromAllModules("M01 选择停止");
                    //m_pScheduler->writeMsgToStatusBar("M01 选择停止");
                }
                break;
            case 2://程序结束
            case 30://程序结束
                m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "stop emulation");
                m_pScheduler->recvRunInfoFromAllModules("M02/M30 程序结束");
                //m_pScheduler->writeMsgToStatusBar("M02/M30 程序结束");
                break;
            case 13://冲压模式复位
                break;
            default:
                break;
            }
            //m_WorkStatus = MOTION_WORK_ERROR_WAITTING;
        }
        break;
   /* case ACTION_IO_TYPE:
        qDebug()<<"IO re"<<m_nMotionControllerRunNum<<" m"<<m_pProcessCacheHandler->m_nMCodeCnt<<" L"<<m_nLastPartGCodeNum<<" w"<<m_GCodeRunSerialNum;
        if (m_nMotionControllerRunNum + m_pProcessCacheHandler->m_nMCodeCnt - m_nLastPartGCodeNum == m_GCodeRunSerialNum)//等待运行到当前位置
        {
            if (runIOWait)
            {
                qDebug()<<"***run io "<<((IOAction *)m_GCodeActions->at(m_GCodeRunSerialNum))->m_nIONum<<((IOAction *)m_GCodeActions->at(m_GCodeRunSerialNum))->m_bIoSts;
                //m_pPlcControllerModuleHandle->setPlcIOStatus(((IOAction *)m_GCodeActions->at(m_GCodeRunSerialNum))->m_nIONum, ((IOAction *)m_GCodeActions->at(m_GCodeRunSerialNum))->m_bIoSts);
                runIOWait = false;
            }
            //判断IO是否执行到位（这里需要讨论是否需要延时等待）
            //if (m_pPlcControllerModuleHandle->getInputIOStatus(((IOAction *)m_GCodeActions->at(m_GCodeRunSerialNum))->m_nCheckIONum) == ((IOAction *)m_GCodeActions->at(m_GCodeRunSerialNum))->m_bCheckIoSts)
            if (1)
            {
                m_GCodeRunSerialNum++;
                m_pProcessCacheHandler->m_nMCodeCnt++;
                runIOWait = true;
                qDebug()<<"waitting io "<<((IOAction *)m_GCodeActions->at(m_GCodeRunSerialNum))->m_nIONum;
            }
        }
        break;
    case ACTION_DELAY_TYPE:
        qDebug()<<"delay"<<m_nDelayTimeSurplus;
        qDebug()<<"De re"<<m_nMotionControllerRunNum<<" m"<<m_pProcessCacheHandler->m_nMCodeCnt<<" L"<<m_nLastPartGCodeNum<<" w"<<m_GCodeRunSerialNum;
        if (m_nDelayTimeSurplus <= 0)//初始化赋值
        {
            m_nDelayTimeSurplus = (((DelayAction *)m_GCodeActions->at(m_GCodeRunSerialNum))->m_nDelayTime);
        }

        if (m_nMotionControllerRunNum + m_pProcessCacheHandler->m_nMCodeCnt - m_nLastPartGCodeNum == m_GCodeRunSerialNum)//等待运行到当前位置
        {
            m_nDelayTimeSurplus -= 100;
            if (m_nDelayTimeSurplus <= 0)//延时时间到，重回加工
            {
                m_GCodeRunSerialNum++;
                m_pProcessCacheHandler->m_nMCodeCnt++;
            }
        }
        break;
    case ACTION_TOOL_CHANGE_TYPE:
        qDebug()<<"TC re"<<m_nMotionControllerRunNum<<" m"<<m_pProcessCacheHandler->m_nMCodeCnt<<" L"<<m_nLastPartGCodeNum<<" w"<<m_GCodeRunSerialNum;
        if (m_nMotionControllerRunNum + m_pProcessCacheHandler->m_nMCodeCnt - m_nLastPartGCodeNum == m_GCodeRunSerialNum)//等待运行到当前位置
        {
           // if (m_pPlcControllerModuleHandle->getInputIOStatus(IO_CANTOOLCHANGE_SIGNAL_PLC_M))
            {
                int temp = ((MotionControllerDevice *)m_pDevice)->runOneActions(m_GCodeActions->at(m_GCodeRunSerialNum), m_pProcessCacheHandler, m_pScheduler->getSystemParameterHandler(), m_errInfo);
                if (temp > 0)
                {
                    m_GCodeRunSerialNum++;
                }
                else if (temp == 0)//缓冲区满，重新发送，还原累计坐标数据
                {
                    //m_pScheduler->recvErrMsgFromAllModules(m_errInfo);
                    if (m_GCodeActions->at(m_GCodeRunSerialNum)->m_type == ACTION_MOVE_TYPE || m_GCodeActions->at(m_GCodeRunSerialNum)->m_type == ACTION_PUNCH_TYPE)
                    {
                        m_pProcessCacheHandler->m_newPoint = m_pProcessCacheHandler->m_oldPoint;
                        m_pProcessCacheHandler->m_newPosition = m_pProcessCacheHandler->m_oldPosition;
                    }
                }
                else
                {
                    m_WorkStatus = MOTION_WORK_ERROR_WAITTING;
                    if(m_errInfo.e_type != DEBUG_NO_ERROR)
                    {
                        m_pScheduler->recvErrMsgFromAllModules(m_errInfo);
                        m_pScheduler->writeMsgToStatusBar(m_errInfo.toString());
                    }
                    return false;
                }
            }
        }
        break;
    case ACTION_TOOL_ROTATE_TYPE:
        qDebug()<<"TR re"<<m_nMotionControllerRunNum<<" m"<<m_pProcessCacheHandler->m_nMCodeCnt<<" L"<<m_nLastPartGCodeNum<<" w"<<m_GCodeRunSerialNum;
        if (m_nMotionControllerRunNum + m_pProcessCacheHandler->m_nMCodeCnt - m_nLastPartGCodeNum == m_GCodeRunSerialNum)//等待运行到当前位置
        {
           // if (m_pPlcControllerModuleHandle->getInputIOStatus(IO_ISROTATETOOL_SIGNAL_PLC_M))//判断是否是转模
            {
                int temp = ((MotionControllerDevice *)m_pDevice)->runOneActions(m_GCodeActions->at(m_GCodeRunSerialNum), m_pProcessCacheHandler, m_pScheduler->getSystemParameterHandler(), m_errInfo);
                if (temp > 0)
                {
                    m_GCodeRunSerialNum++;
                }
                else if (temp == 0)//缓冲区满，重新发送，还原累计坐标数据
                {
                    //m_pScheduler->recvErrMsgFromAllModules(m_errInfo);
                    if (m_GCodeActions->at(m_GCodeRunSerialNum)->m_type == ACTION_MOVE_TYPE || m_GCodeActions->at(m_GCodeRunSerialNum)->m_type == ACTION_PUNCH_TYPE)
                    {
                        m_pProcessCacheHandler->m_newPoint = m_pProcessCacheHandler->m_oldPoint;
                        m_pProcessCacheHandler->m_newPosition = m_pProcessCacheHandler->m_oldPosition;
                    }
                }
                else
                {
                    m_WorkStatus = MOTION_WORK_ERROR_WAITTING;
                    if(m_errInfo.e_type != DEBUG_NO_ERROR)
                    {
                        m_pScheduler->recvErrMsgFromAllModules(m_errInfo);
                        m_pScheduler->writeMsgToStatusBar(m_errInfo.toString());
                    }
                    return false;
                }
            }
       
        }
        break;*/
    case ACTION_IO_TYPE:
    {
        //qDebug()<<"Ac re"<<m_nMotionControllerRunNum<<" m"<<m_pProcessCacheHandler->m_nMCodeCnt<<" L"<<m_nLastPartGCodeNum<<" w"<<m_GCodeRunSerialNum;
	if((((IOAction *)m_GCodeActions->at(m_GCodeRunSerialNum))->m_nIoType == IO_OUT )
    &&(((IOAction *)m_GCodeActions->at(m_GCodeRunSerialNum))->m_nIONum == 255 ))
	{
	      // if(1)
	       	{
	       	       //发送数据请求
	       	       
		((MotionControllerDevice *)m_pDevice)->SendNetReqByTCP(m_GCodeRunSerialNum);
	       	m_GCodeRunSerialNum++;		  
	       qDebug()<<"m_GCodeRunSerialNum"<<m_GCodeRunSerialNum;
		}
		return true;
	}
	

	int temp = ((MotionControllerDevice *)m_pDevice)->runOneActions((IOAction *)m_GCodeActions->at(m_GCodeRunSerialNum), m_pProcessCacheHandler, m_pScheduler->getSystemParameterHandler(), m_errInfo);
	

	if (temp > 0)
        {
            m_GCodeRunSerialNum++;
	     qDebug()<<"m_GCodeRunSerialNum"<<m_GCodeRunSerialNum;
        }
        else if (temp == 0)//缓冲区满，重新发送，还原累计坐标数据
        {
            //m_pScheduler->recvErrMsgFromAllModules(m_errInfo);
           // if ((IOAction *)m_GCodeActions->at(m_GCodeRunSerialNum)->m_type == ACTION_MOVE_TYPE || m_GCodeActions->at(m_GCodeRunSerialNum)->m_type == ACTION_PUNCH_TYPE)
            {
            	 qDebug()<<"temp RETURN 0";
              
            }
    
        }
        else
        {
        	qDebug()<<"temp RETURN -1";
            m_WorkStatus = MOTION_WORK_ERROR_WAITTING;
            if(m_errInfo.e_type != DEBUG_NO_ERROR)
            {
                m_pScheduler->recvErrMsgFromAllModules(m_errInfo);
                m_pScheduler->writeMsgToStatusBar(m_errInfo.toString());
            }
            return false;
        }
    	}
	break;
    case ACTION_DELAY_TYPE:
   {
       int temp = ((MotionControllerDevice *)m_pDevice)->runOneActions((DelayAction *)m_GCodeActions->at(m_GCodeRunSerialNum), m_pProcessCacheHandler, m_pScheduler->getSystemParameterHandler(), m_errInfo);
	
	if (temp > 0)
        {
            m_GCodeRunSerialNum++;
	     qDebug()<<"m_GCodeRunSerialNum"<<m_GCodeRunSerialNum;
        }
        else if (temp == 0)//缓冲区满，重新发送，还原累计坐标数据
        {
            //m_pScheduler->recvErrMsgFromAllModules(m_errInfo);
            //if ((DelayAction *)m_GCodeActions->at(m_GCodeRunSerialNum)->m_type == ACTION_DELAY_TYPE || m_GCodeActions->at(m_GCodeRunSerialNum)->m_type == ACTION_PUNCH_TYPE)
            {
            	 qDebug()<<"temp RETURN 0";
               
            }
    
        }
        else
        {
        	qDebug()<<"temp RETURN -1";
            m_WorkStatus = MOTION_WORK_ERROR_WAITTING;
            if(m_errInfo.e_type != DEBUG_NO_ERROR)
            {
                m_pScheduler->recvErrMsgFromAllModules(m_errInfo);
                m_pScheduler->writeMsgToStatusBar(m_errInfo.toString());
            }
            return false;
        }
    	}   
	break;
    default:
    {
	
	if((m_GCodeActions->at(m_GCodeRunSerialNum)->m_type == ACTION_MOVE_TYPE)
		&&(m_GCodeActions->at(m_GCodeRunSerialNum)->m_delaytime>0))	
	{
	       if(((MotionControllerDevice *)m_pDevice)->RecvNetDataByTCP(&scarapos[0]))
	       	{
	       	       //等待数据
	       	      	
			//((MoveAction *)m_GCodeActions->at(m_GCodeRunSerialNum))->m_endPoint=Point(scarapos[0],scarapos[1],scarapos[2],scarapos[3]);
		
	       	}
		else 
		{
			(m_GCodeActions->at(m_GCodeRunSerialNum)->m_delaytime)-=100;
			return false;			
		}
		
	}
        //qDebug()<<"Ac re"<<m_nMotionControllerRunNum<<" m"<<m_pProcessCacheHandler->m_nMCodeCnt<<" L"<<m_nLastPartGCodeNum<<" w"<<m_GCodeRunSerialNum;
        int temp = ((MotionControllerDevice *)m_pDevice)->runOneActions(m_GCodeActions->at(m_GCodeRunSerialNum), m_pProcessCacheHandler, m_pScheduler->getSystemParameterHandler(), m_errInfo);
	if (temp > 0)
        {
            m_GCodeRunSerialNum++;
	     qDebug()<<"m_GCodeRunSerialNum"<<m_GCodeRunSerialNum;
        }
        else if (temp == 0)//缓冲区满，重新发送，还原累计坐标数据
        {
            //m_pScheduler->recvErrMsgFromAllModules(m_errInfo);
            if (m_GCodeActions->at(m_GCodeRunSerialNum)->m_type == ACTION_MOVE_TYPE || m_GCodeActions->at(m_GCodeRunSerialNum)->m_type == ACTION_PUNCH_TYPE)
            {
            	 qDebug()<<"temp RETURN 0";
                m_pProcessCacheHandler->m_newPoint = m_pProcessCacheHandler->m_oldPoint;
                m_pProcessCacheHandler->m_newPosition = m_pProcessCacheHandler->m_oldPosition;
            }
     	     /*if(!m_bisCycleRun)
            {
                m_WorkStatus = MOTION_WORK_IDLE;
                m_bisStopMotion = true;
                m_pProcessCacheHandler->m_bIsManualMotion = true;
            }*/
        }
        else
        {
        	qDebug()<<"temp RETURN -1";
            m_WorkStatus = MOTION_WORK_ERROR_WAITTING;
            if(m_errInfo.e_type != DEBUG_NO_ERROR)
            {
                m_pScheduler->recvErrMsgFromAllModules(m_errInfo);
                m_pScheduler->writeMsgToStatusBar(m_errInfo.toString());
            }
            return false;
        }
        }break;
    }

    return true;
}

void MotionControllerModule::feedbackSystemState()
{
    //return;
    static int timeCnt = 10;

    if (timeCnt > 0)
    {
        if (timeCnt%4 == 0 && m_nClampAutoSearchStep == CAS_PRE_WORK)//夹钳搜索期间不获取状态
        {
            if(((MotionControllerDevice *)m_pDevice)->getSts(&(m_pProcessCacheHandler->m_nMotionControllerSts), m_errInfo))
            {
                qDebug()<<"@@@@@@@@sts="<<m_pProcessCacheHandler->m_nMotionControllerSts<<"#########";
                if ((m_pProcessCacheHandler->m_nMotionControllerSts & 0xffc00000) != 0)//触发限位
                {
                    //qDebug()<<"清除限位";
                    m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "limit trigger");
                    ((MotionControllerDevice *)m_pDevice)->clearSts(m_errInfo);//清除状态
                }
            }
        }
        timeCnt--;
    }
    else //1s反馈一次
    {
//        qDebug()<<"feedback"<<((MotionControllerDevice *)m_pDevice)->m_netctrl->gb_ctrl.decode_id;//m_nMotionControllerRunNum;
        timeCnt = 10;
        //自动运行    
    }
      if(((MotionControllerDevice *)m_pDevice)->m_netctrl->m_bIsCtlAlarm)
    {
	    //m_errInfo.insertDebug(DEBUG_WARNING,QString("MotionCotrol: alarmno=%1!").arg(((MotionControllerDevice *)m_pDevice)->m_netctrl->gb_ctrl.alarmno));
	     m_errInfo.insertDebug(DEBUG_WARNING,QString::fromLocal8Bit(ErrorInformation[((MotionControllerDevice *)m_pDevice)->m_netctrl->gb_ctrl.alarmno]));
	    
	    m_pScheduler->recvErrMsgFromAllModules(m_errInfo);
	    m_pScheduler->writeMsgToStatusBar(m_errInfo.toString());
	    ((MotionControllerDevice *)m_pDevice)->m_netctrl->m_bIsCtlAlarm=0;
    }
}

bool MotionControllerModule::isAxisVInRightPosition(double angle)
{
    for (int i = 0; i < m_pScheduler->getSystemParameterHandler()->m_mouldsParm.size(); i++)
    {
        if (fabs(angle - m_pScheduler->getSystemParameterHandler()->m_mouldsParm.at(i).m_dPosAngle) < 0.1)//允许误差0.1度
        {
            return true;
        }
    }
    return false;
}

bool MotionControllerModule::isReadyToStartMotion()
{
 
    return true;
}

bool MotionControllerModule::isServoEnable()
{

     if((m_pScheduler->m_pSystemParameter->sys_ctrl.statebit & (1 << CTL_STS_ENABLE)) == 1 )
     {
	   if (!m_bIsServoEnable)//if ((m_pProcessCacheHandler->m_nMotionControllerSts & checkflag) != 0)//浼烘湇鍚姩
	    {
	    	m_bIsServoEnable=1;
	        
	    }
	    
	   return true;
      }
      else
     {
        /* if (isShowMessageBox)
         {
             m_pScheduler->closeCtlMonitor();
             QMessageBox::warning(NULL, "璀﹀憡", "鐢垫満鏈娇鑳, QMessageBox::Yes, QMessageBox::Yes);
         }*/
         m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("电机未使能，请先上使能！"));
	 QMessageBox::warning(NULL, QString::fromLocal8Bit("警告"), QString::fromLocal8Bit("电机未使能，请先上使能！"));
         return false;
     }
}

/*bool MotionControllerModule::isServoEnable()
{

     
	   if (m_bIsServoEnable)//if ((m_pProcessCacheHandler->m_nMotionControllerSts & checkflag) != 0)//浼烘湇鍚姩
	    {
	    	
	        return true;
	    }
	    else
	    {
	       
	        m_pScheduler->recvRunInfoFromAllModules("鐢垫満鏈娇鑳斤紝涓嶈兘鎵ц");
	        return false;
	    }
      	
}*/

int MotionControllerModule::isServoMode()
{
   //int mode=(m_pScheduler->m_pSystemParameter->sys_ctrl.statebit&0x60)>>5;
   int mode=(m_pScheduler->m_pSystemParameter->sys_ctrl.statebit&0xc0000)>>18;
 
    return mode;

    
}

bool MotionControllerModule::isDingWeiXiaoEnable()
{
    /*
    if (!m_pPlcControllerModuleHandle->getInputIOStatus(IO_DINGWEIXIAO_SIGNAL_PLC_M))
    {
        QMessageBox::information(NULL, tr("警告"), tr("定位销未到位！"));
        return false;
    }*/
    return true;
}

bool MotionControllerModule::isAxisTEnableMove()
{
     return true;
    /*if (m_pPlcControllerModuleHandle->getInputIOStatus(IO_CANTOOLCHANGE_SIGNAL_PLC_M))//T轴安全监测
    {
        if (m_pProcessCacheHandler->m_curPoint.v < 1)
        {
            if (!m_pPlcControllerModuleHandle->getInputIOStatus(IO_SAFTDOOROPEN_SIGNAL_PLC_M))
            {
                return true;
            }
            else
            {
                QMessageBox::information(NULL, tr("警告"), tr("安全门没有关闭"));
                return false;
            }
        }
        else
        {
            QMessageBox::information(NULL, tr("警告"), tr("V轴没有在上死点！"));
            return false;
        }
    }
    else
    {
        QMessageBox::information(NULL, tr("警告"), tr("定位销未到位！"));
        return false;
    }*/
}








