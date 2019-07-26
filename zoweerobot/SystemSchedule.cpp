/************************************************************
  Copyright (C),
  文件名：SystemSchedule.cpp
  作者：zhou    版本：V1.0    创建日期：2018/07/10
  模块描述：SystemSchedule类的实现
  历史修改记录：<author>    <time>    <version >    <desc>
                  zhou    2018/07/10     V1.0      添加注释
*************************************************************/

#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "SystemSchedule.h"
#include "ComonTypes.h"

#include "BaseModule.h"
#include "MotionControllerModule.h"
//#include "EmulationControllerModule.h"
#include "XmlControllerModule.h"
//#include "KeyControllerModule.h"
//#include "PLCControllerModule.h"

#include "GlobalVariableType.h"

#include "cautomachiningstateui.h"
#include "cteachpointui.h"
//#include "ui_cteachpointui.h"
#include "cmanualmachiningstateui.h"
//#include "ciostateui.h"
//#include "cmachiningsimulationui.h"
#include "csystempreferencesui.h"
#include "ctoolcoorsetui.h"
#include "cjointparamsetui.h"
#include "cstructparam.h"
#include "cusercoorsetui.h"
#include "cpidparamui.h"
#include "CFrmSetZero.h"
#include "CFrmUserLevel.h"
#include "MotionControllerDevice.h"
#include "NetCtrl.h"
/*#include "caxespreferencesui.h"
#include "ctoolpreferencesui.h"
#include "cwarningmsgdiagnosisui.h"
#include "cwarningrecordui.h"
#include "csysteminformationui.h"
#include "ccommunicationsettingui.h"
#include "cadvancedsystemsettingsui.h"
#include "ccraftpreferencesui.h"*/


#include <QFileDialog>
#include <QMessageBox>


//SystemSchedule::SystemSchedule(MainWindow *w, QObject *parent) :
//    QObject(parent),
//    m_MainWidget(w)
//{
//    initEnvironment();
//    registerAllUI();
//    registerAllModules();
////    connectModulesAndUIResource();

//}

SystemSchedule::SystemSchedule(MainWindow *w, UserParameter parm, QObject *parent) :
    QObject(parent),
    m_MainWidget(w)
{
    initEnvironment(parm);
    registerAllUI(parm);
    registerAllModules();
//    connectModulesAndUIResource();

    //运行日志记录
    initRunInfoFile();
    writeRunInfoToFile("正常开机");
}

/*************************************************
  函数名称：initEnvironment()
  函数功能：初始化环境，包括全局变量
  输入参数：
  输出参数：
  返 回 值：
  备    注：
**************************************************/
void SystemSchedule::initEnvironment(UserParameter parm)
{
    /**<  读取XML，初始化全局变量 */

    m_pSystemParameter = new SystemParameter;
    m_pSystemParameter->setUserParm(parm);
    m_pSystemParameter->sysvel=2;
    ReadUserInformation();
    ReadSystemParamInformation();
    ReadAxisParamInformation();
    ReadPidParaGrp("PidParaGrp.dat");
}

bool SystemSchedule::ReadUserInformation()
{
	 int  i = 0;

	QFile file("userinformation.dat");

    if (!file.open(QIODevice::ReadOnly))
        return 0;

   
    //for(i=0;i<21;i++)
    {
        file.read((char *)&m_pSystemParameter->m_userParam[0],21*sizeof(CartesianPose));
     
    }
	file.close();
    return true;
}

bool SystemSchedule::WriteUserInformation()
{
	int  i = 0;
	QFile file("userinformation.dat");

    if (!file.open(QIODevice::WriteOnly))
        return 0;

    


    //for(i=0;i<21;i++)
    {
        file.write((char *)&m_pSystemParameter->m_userParam[0],21*sizeof(CartesianPose));
      
    }
	file.close();
    return true;
}

bool SystemSchedule::ReadSystemParamInformation(const QString &filename)
{
    int  i = 0;
    
    QFile file(filename);
    
    if (!file.open(QIODevice::ReadOnly))
        return false;    
    
    file.read((char *)m_pSystemParameter->SystemParam,256*sizeof(float));
        
    file.close();
    
    return true;
}

bool SystemSchedule::ReadSystemParamInformation()
{
    return ReadSystemParamInformation("SystemParam.dat");
}

bool SystemSchedule::WriteSystemParamInformation()
{
	int  i = 0;
	QFile file("SystemParam.dat");

    if (!file.open(QIODevice::WriteOnly))
        return 0;

    


    //for(i=0;i<21;i++)
    {
        file.write((char *)&m_pSystemParameter->SystemParam[0],256*sizeof(float));
      
    }
	file.close();


    return true;
}

bool SystemSchedule::ReadAxisParamInformation(const QString &filename)
{
	 int  i = 0;

	QFile file(filename);

    if (!file.open(QIODevice::ReadOnly))
        return false;

   
    //for(i=0;i<21;i++)
    {
        file.read((char *)&m_pSystemParameter->axisno[0],17*sizeof(int));
     
    }
	file.close();
    return true;
}

bool SystemSchedule::ReadAxisParamInformation()
{
	return ReadAxisParamInformation("AxisParam.dat");
}

bool SystemSchedule::WriteAxisParamInformation()
{
	int  i = 0;
	

	QFile file("AxisParam.dat");

    if (!file.open(QIODevice::WriteOnly))
        return 0;

    


    //for(i=0;i<21;i++)
    {
        file.write((char *)&m_pSystemParameter->axisno[0],17*sizeof(int));
    }
	file.close();
    return true;
}

bool SystemSchedule::ReadPidParaGrp(const QString &filename)
{
    int  i = 0;
    
    QFile file(filename);

    memset(m_pSystemParameter->PidParaGrpName,0,sizeof(m_pSystemParameter->PidParaGrpName));
    for(i=0;i<5;i++){
        memcpy(&m_pSystemParameter->PidParaGrp[i],&m_pSystemParameter->SystemParam[pAIXS1PID],sizeof(m_pSystemParameter->PidParaGrp[0]));
    }
    
    if (!file.open(QIODevice::ReadOnly)){        
        return false;    
    }

    file.read((char *)m_pSystemParameter->PidParaGrpName,sizeof(m_pSystemParameter->PidParaGrpName));
    file.read((char *)m_pSystemParameter->PidParaGrp,sizeof(m_pSystemParameter->PidParaGrp));
    
    file.close();
    
    return true;
}

bool SystemSchedule::SavePidParaGrp(const QString &filename)
{
    int  i = 0;
    QFile file(filename);
    
    if (!file.open(QIODevice::WriteOnly))
        return 0;
    
    file.write((char *)&m_pSystemParameter->PidParaGrpName,sizeof(m_pSystemParameter->PidParaGrpName));
    file.write((char *)&m_pSystemParameter->PidParaGrp,sizeof(m_pSystemParameter->PidParaGrp));
    file.close();
    
    return true;
}


/*************************************************
  函数名称：registerAllUI()
  函数功能：注册所有的UI
  输入参数：
  输出参数：
  返 回 值：
  备    注：
**************************************************/
void SystemSchedule::registerAllUI(UserParameter user)
{
//    registerGCodeEditUI(m_MainWidget);
    registerAutoMachiningStateUI();
    registerTeachPointStateUI();
    registerManualMachiningStateUI();
    //registerIOStateUI();
    //registerMachiningSimulationUI();
    registerSystemPreferencesUI();
    
    registerJointParamSetUI();
    registerStructParamSetUI();
    registerPIDParamSetUI();
    registerZeroParamSetUI();
    registerToolCoorSetUI();
    registerUserCoorSetUI();
    registerUserLevelSetUI();
    
    //registerUserCoorSetUI();
    /*registerAxesPreferencesUI();
    registerCraftPreferencesUI();
    registerToolPreferencesUI();
    registerWarningMsgDiagnosisUI();
    registerWarningRecordUI();
    registerSystemInformationUI();
    registerCommunicationsSettingsUI();
    registerAdvancedSystemSettingsUI();*/

    m_MainWidget->createDisplayUIs(m_Widgets);


    m_MainWidget->showDisplayUIs(m_Widgets, user);

    m_MainWidget->goToUiPage(UI_PROGRAM_ID);
#ifdef _CNC_SYSTEM_PROCESS_DEBUG      /* 在QT的控制台显示系统加载的所有UI的信息 */
    m_systemInfo.printfMesssage("UI imformation");

    foreach(InfoUI *w, m_Widgets)
    {
        m_systemInfo.printfWidgetInfo(w);
    }   
#else
    showSystemRunInfo("UI imformation");

    foreach(InfoUI *w, m_Widgets)
    {
        showSystemRunInfo(m_systemInfo.toString(w));
    }
#endif
}



void SystemSchedule::registerUI(QWidget *w, QString name, int nType, int nSerial, QVector<int> &nModulesUser)
{
    InfoUI *ui = new InfoUI;

//    ui->bShow = true;

    ui->sName = name;
    ui->pUi = w;
    ui->pParentUi = m_MainWidget;
    ui->isActive = FALSE;
    ui->nType = nType;
    ui->nUISerial = nSerial;
    ui->nModulesUser = nModulesUser;

    m_Widgets.append(ui);
}

void SystemSchedule::registerAutoMachiningStateUI()
{
    QVector<int> nModulesUser;
 
    nModulesUser.push_back(EMULATION_CONTROLLER_ID);
    nModulesUser.push_back(MOTION_CONTROLLER_ID);
    nModulesUser.push_back(PLC_CONTROLLER_ID);

    registerUI(new CAutoMachiningStateUi(m_MainWidget, this), QString::fromLocal8Bit("自动"),
               UI_PROGRAM_ID, UI_AUTO_MACHINING_STATE, nModulesUser);
}

void SystemSchedule::registerTeachPointStateUI()
{
    QVector<int> nModulesUser;

    nModulesUser.push_back(MOTION_CONTROLLER_ID);
    nModulesUser.push_back(PLC_CONTROLLER_ID);
    nModulesUser.push_back(XML_CONTROLLER_ID);//用于设置原点
    registerUI(new cteachpointui(m_MainWidget, this),  QString::fromLocal8Bit("示教"),
               UI_PROGRAM_ID, UI_MANUAL_MACHINING_STATE, nModulesUser);
}
void SystemSchedule::registerManualMachiningStateUI()
{
    QVector<int> nModulesUser;

    nModulesUser.push_back(MOTION_CONTROLLER_ID);
    nModulesUser.push_back(PLC_CONTROLLER_ID);
    nModulesUser.push_back(XML_CONTROLLER_ID);//用于设置原点
    registerUI(new CManualMachiningStateUi(m_MainWidget, this),  QString::fromLocal8Bit("I/O"),
               UI_PROGRAM_ID, UI_IO_STATE, nModulesUser);
}


/*
void SystemSchedule::registerIOStateUI()
{
    QVector<int> nModulesUser;
    nModulesUser.push_back(PLC_CONTROLLER_ID);
    nModulesUser.push_back(KEY_CONTROLLER_ID);
    registerUI(new CIOStateUi(m_MainWidget, this),  QString::fromLocal8Bit("I/O状态显示"),
               UI_PROGRAM_ID, UI_IO_STATE, nModulesUser);
}

void SystemSchedule::registerMachiningSimulationUI()
{
    QVector<int> nModulesUser;
    nModulesUser.push_back(EMULATION_CONTROLLER_ID);
    nModulesUser.push_back(MOTION_CONTROLLER_ID);
    registerUI(new CMachiningSimulationUi(m_MainWidget, this),  QString::fromLocal8Bit("加工图形仿真"),
               UI_PROGRAM_ID, UI_MACHINING_SIMULATION, nModulesUser);

//    connect(((CProgramEmulationSelection *)(m_Widgets.at(UI_PROGRAM_EMULATION_SELECTION)->pUi))->getScanFileBtn(),SIGNAL(clicked()),this,SLOT(selectNGFile()));
}
*/
void SystemSchedule::registerSystemPreferencesUI()
{
    QVector<int> nModulesUser;
    nModulesUser.push_back(XML_CONTROLLER_ID);
    nModulesUser.push_back(MOTION_CONTROLLER_ID);
    registerUI(new CSystemPreferencesUi(m_MainWidget, this),  QString::fromLocal8Bit("系统"),
               UI_STATE_ID, UI_SYSTEM_PREFERENCES, nModulesUser);
}
void SystemSchedule::registerJointParamSetUI()
{
    QVector<int> nModulesUser;
    nModulesUser.push_back(MOTION_CONTROLLER_ID);
    registerUI(new cjointparamsetui(m_MainWidget, this),  QString::fromLocal8Bit("运动"),
               UI_SETTING_ID, UI_TOOL_PREFERENCES, nModulesUser);
}
void SystemSchedule::registerStructParamSetUI()
{
    QVector<int> nModulesUser;
    nModulesUser.push_back(MOTION_CONTROLLER_ID);
    registerUI(new cstructparam(m_MainWidget, this),  QString::fromLocal8Bit("结构"),
               UI_SETTING_ID, UI_TOOL_STRUCT_SETTINGS, nModulesUser);
}
void SystemSchedule::registerPIDParamSetUI()
{
    QVector<int> nModulesUser;
    nModulesUser.push_back(MOTION_CONTROLLER_ID);
    registerUI(new cpidparamui(m_MainWidget, this),  QString::fromLocal8Bit("PID "),
               UI_SETTING_ID, UI_WARNING_MSG_DIAGNOSIS, nModulesUser);
}
void SystemSchedule::registerZeroParamSetUI()
{
    QVector<int> nModulesUser;
    nModulesUser.push_back(MOTION_CONTROLLER_ID);
    registerUI(new CFrmSetZero(m_MainWidget, this),  QString::fromLocal8Bit("零点"),
               UI_SETTING_ID, UI_ZERO_SETTINGS, nModulesUser);
}

void SystemSchedule::registerToolCoorSetUI()
{
    QVector<int> nModulesUser;
    nModulesUser.push_back(MOTION_CONTROLLER_ID);
    registerUI(new CToolCoorSetUi(m_MainWidget, this),  QString::fromLocal8Bit("工具"),
               UI_SETTING_ID, UI_WARNING_RECORD, nModulesUser);
}
void SystemSchedule::registerUserCoorSetUI()
{
    QVector<int> nModulesUser;
    nModulesUser.push_back(MOTION_CONTROLLER_ID);
    registerUI(new cusercoorsetui(m_MainWidget, this),  QString::fromLocal8Bit("用户"),
               UI_SETTING_ID, UI_SYSTEM_INFORMATION, nModulesUser);
}

void SystemSchedule::registerUserLevelSetUI()
{
    QVector<int> nModulesUser;
    nModulesUser.push_back(MOTION_CONTROLLER_ID);
    registerUI(new CFrmUserLevel(m_MainWidget, this),  QString::fromLocal8Bit("权限"),
               UI_SETTING_ID, UI_USERRIGHT_SETTINGS, nModulesUser);
}

/*
void SystemSchedule::registerAxesPreferencesUI()
{
    QVector<int> nModulesUser;
    nModulesUser.push_back(XML_CONTROLLER_ID);
    nModulesUser.push_back(MOTION_CONTROLLER_ID);
    registerUI(new CAxesPreferencesUi(m_MainWidget, this),  QString::fromLocal8Bit("运动轴参数设置"),
               UI_STATE_ID, UI_AXES_PREFERENCES, nModulesUser);
}

void SystemSchedule::registerCraftPreferencesUI()
{
    QVector<int> nModulesUser;
    nModulesUser.push_back(XML_CONTROLLER_ID);
    nModulesUser.push_back(MOTION_CONTROLLER_ID);
    registerUI(new CCraftPreferencesUi(m_MainWidget, this),  QString::fromLocal8Bit("工艺参数设置"),
               UI_STATE_ID, UI_CRAFT_PREFERENCES, nModulesUser);
}

void SystemSchedule::registerToolPreferencesUI()
{
    QVector<int> nModulesUser;
    nModulesUser.push_back(XML_CONTROLLER_ID);
    nModulesUser.push_back(MOTION_CONTROLLER_ID);
    registerUI(new CToolPreferencesUi(m_MainWidget, this),  QString::fromLocal8Bit("模具库参数设置"),
               UI_STATE_ID, UI_TOOL_PREFERENCES, nModulesUser);
}

void SystemSchedule::registerWarningMsgDiagnosisUI()
{
    QVector<int> nModulesUser;
    nModulesUser.push_back(MOTION_CONTROLLER_ID);
    registerUI(m_pWarningMsgDiagnosisUi = new CWarningMsgDiagnosisUi(m_MainWidget, this),  QString::fromLocal8Bit("警告信息诊断"),
               UI_SETTING_ID, UI_WARNING_MSG_DIAGNOSIS, nModulesUser);
}

void SystemSchedule::registerWarningRecordUI()
{
    QVector<int> nModulesUser;
    registerUI(new CWarningRecordUi(m_MainWidget),  QString::fromLocal8Bit("历史警告信息记录"),
               UI_SETTING_ID, UI_WARNING_RECORD, nModulesUser);
}

void SystemSchedule::registerSystemInformationUI()
{
    QVector<int> nModulesUser;
    registerUI(new CSystemInformationUi(m_MainWidget, this),  QString::fromLocal8Bit("软件信息"),
               UI_WARNING_ID, UI_SYSTEM_INFORMATION, nModulesUser);
}

void SystemSchedule::registerCommunicationsSettingsUI()
{
    QVector<int> nModulesUser;
    nModulesUser.push_back(XML_CONTROLLER_ID);
    nModulesUser.push_back(MOTION_CONTROLLER_ID);
    registerUI(new CCommunicationSettingUi(m_MainWidget, this),  QString::fromLocal8Bit("外部设备通信设置"),
               UI_WARNING_ID, UI_COMMUNICATION_SETTINGS, nModulesUser);
}

void SystemSchedule::registerAdvancedSystemSettingsUI()
{
    QVector<int> nModulesUser;
    registerUI(new CAdvancedSystemSettingsUi(m_MainWidget, this),  QString::fromLocal8Bit("高级设置"),
               UI_WARNING_ID, UI_ADVANCED_SYSTEM_SETTINGS, nModulesUser);
}


*/

/*************************************************
  函数名称：registerAllModules()
  函数功能：注册所有UI
  输入参数：
  输出参数：
  返 回 值：
  备    注：
**************************************************/
void SystemSchedule::registerAllModules()
{
    addAllModules();

    for(int i=0;i < m_Modules.size(); i++)
    {
        if(m_Modules.at(i)->initModule(m_Widgets) != 0)
        {
            qDebug() << m_Modules.at(i)->getErrorInfo().toString();
        }
    }

    startAllModules();


#ifdef _CNC_SYSTEM_PROCESS_DEBUG     /* 在QT的控制台显示系统运行的信息 */
    m_systemInfo.printfMesssage("Modules imformation");

    foreach(BaseModule *m, m_Modules)
    {
        m_systemInfo.printfModule(m);
    }
#else
    showSystemRunInfo("Modules imformation");

   /* foreach(BaseModule *m, m_Modules)
    {
        showSystemRunInfo(m_systemInfo.toString(m));
    }*/
#endif
}

/*************************************************
  函数名称：startAllModules()
  函数功能：为每个Module启动定时器
  输入参数：
  输出参数：
  返 回 值：
  备    注：
**************************************************/
void SystemSchedule::startAllModules()
{
//    for(int i = 0; i < m_Modules.size(); i++)
//    {
//        QThread *pThread = new QThread;
//        m_Modules.at(i)->moveToThread(pThread);
//        pThread->start();
//        m_threads.append(pThread);
//    }
//    for(int i = 0; i < m_Modules.size(); i++)
//    {
//        m_Modules.at(i)->start();
//    }
}

/*************************************************
  函数名称：addAllModules()
  函数功能：加载所有Module
  输入参数：
  输出参数：
  返 回 值：
  备    注：
**************************************************/
void SystemSchedule::addAllModules()
{
    addXmlControllerModule();
    addMotionControllerModule();
    /*addEmulationControllerModule();
    addKeyControllerModule();
    addPLCControllerModule();*/
}

/*************************************************
  函数名称：addMotionControllerModule()
  函数功能：加载MotionController
  输入参数：
  输出参数：
  返 回 值：
  备    注：
**************************************************/
int SystemSchedule::addMotionControllerModule()
{
    MotionControllerModule* m = new MotionControllerModule(m_Widgets, this);

    if(m->getErrorType() >= DEBUG_PARAMETER_ERROR)
    {
        /**<  错误代码处理 */
        int errorType = m->getErrorType();
        delete m;
        return errorType;
    }

    m_Modules.append(m);

    connect(this, SIGNAL(emitMotionModuleSignal(QString, QString)), m, SLOT(absorbMotionModule(QString, QString)));
    connect(this, SIGNAL(emitManualMotionModuleSignal(QString, QString)), m, SLOT(absorbManualMotionModule(QString, QString)));
    return 0;
}




int SystemSchedule::addXmlControllerModule()
{
    XmlControllerModule* m = new XmlControllerModule(m_Widgets, this);

    if(m->getErrorType() >= DEBUG_PARAMETER_ERROR)
    {
        /**<  错误代码处理 */
        int errorType = m->getErrorType();
        delete m;
        return errorType;
    }

    m_Modules.append(m);
    
    connect(this, SIGNAL(emitXMLModuleSignal(QString, QString)),m, SLOT(absorbXMLModule(QString, QString)));

    return 0;
}


void SystemSchedule::showSystemRunInfo(QString s)//运行日志
{
    //m_pWarningMsgDiagnosisUi->recordSystemRunInfo(s);
}

void SystemSchedule::showSystemErrorInfo(QString s)//错误信息
{
    //m_pWarningMsgDiagnosisUi->recordSystemErrorInfo(s);
}




void SystemSchedule::recvErrMsgFromAllModules(ErrorInfo  e)
{
    QDateTime current_date_time = QDateTime::currentDateTime();
    QString  current_date = current_date_time.toString("yyyy-MM-dd hh:mm:ss ");

    m_systemInfo.printfMesssage(e.e_str);
    showSystemErrorInfo(current_date + e.toString());//写入到警告页面

    writeErrInfoToFile(current_date + e.toString());//写入到文件
}

void SystemSchedule::recvRunInfoFromAllModules(QString  s)
{
    QDateTime current_date_time = QDateTime::currentDateTime();
    QString  current_date = current_date_time.toString("yyyy-MM-dd hh:mm:ss ");

    m_systemInfo.printfMesssage(s);
    showSystemRunInfo(current_date + s);//写入到警告页面

    writeRunInfoToFile(current_date + s);//写入到文件
}

void SystemSchedule::setNGFilePath(QString  s)
{
    m_filePath = s;
}

QString SystemSchedule::getNGFilePath()
{
    return m_filePath;
}

void SystemSchedule::setParmFilePath(QString  s)
{
    m_sParmFilePath = s;
}

//QString SystemSchedule::getParmFilePath()
//{
//    return m_filePath;
//}

/*************************************************
  函数名称：clearFileContextList()
  函数功能：清除m_fileContexts链表
  输入参数：
  输出参数：
  返 回 值：
  备    注：
**************************************************/
void  SystemSchedule::clearFileContextList()
{
    m_fileContexts.clear();
}

QList<ContextRecord>*  SystemSchedule::getNGFileContexts()
{
    return  &m_fileContexts;
}

SystemParameter*   SystemSchedule::getSystemParameterHandler()
{
    return  m_pSystemParameter;
}



void SystemSchedule::recvMsgFromWindows(int moduleId, QString msg, QString arg)
{
    switch(moduleId)
    {
    case XML_CONTROLLER_ID:
        emit emitXMLModuleSignal(msg, arg);
        break;
    case EMULATION_CONTROLLER_ID:
        emit emitEmulationModuleSignal(msg, arg);
        break;
    case MOTION_CONTROLLER_ID:
        if (msg.left(6) != "manual")
        {
        	qDebug()<<msg<<arg;
            emit emitMotionModuleSignal(msg, arg);
        }
        else
        {
            QString tempMsg = msg.section('/', -1);//取"manual/"后面的内容
            //qDebug()<<"msg!!!"<<tempMsg;
            emit emitManualMotionModuleSignal(tempMsg, arg);
        }
        break;
    case PLC_CONTROLLER_ID:
            emit emitPLCModuleSignal(msg, arg);
        break;
    case KEY_CONTROLLER_ID:
            emit emitKeyModuleSignal(msg, arg);
        break;
    default:
        break;
    }
}

void SystemSchedule::writeMsgToStatusBar(QString msg, int level)
{
    //return;
    m_MainWidget->writeMsgToStatusBar(msg, level);
}

void SystemSchedule::writeMsgToStatusBar(QString msg)
{
    //return;
    m_MainWidget->writeMsgToStatusBar(msg);
}

void SystemSchedule::reShowAllUIs(UserParameter user)
{
    m_MainWidget->showDisplayUIs(m_Widgets, user);
}

void SystemSchedule::writeErrInfoToFile(QString errInfo)
{
    //return;
    QFile historyErrFile("historyErrorInfo.txt");
    historyErrFile.open(QIODevice::WriteOnly | QIODevice::Append);

    QTextStream in(&historyErrFile);
    in << errInfo << "\r\n";

    //in.flush();
    historyErrFile.close();
}


void SystemSchedule::writeRunInfoToFile(QString runInfo)
{
    //return;
    QFile runInfoFile("runInfo.txt");
    runInfoFile.open(QIODevice::WriteOnly | QIODevice::Append);

    QTextStream in(&runInfoFile);
    in << runInfo << "\r\n";

    runInfoFile.close();
}

void SystemSchedule::initRunInfoFile()
{
    QFile runInfoFile("runInfo.txt");
    runInfoFile.open(QIODevice::WriteOnly);
    runInfoFile.close();
}

bool SystemSchedule::NetIsConnect(void)
{
    MotionControllerDevice *pDev=NULL;
    //MotionControllerModule *pModule=NULL;

    //pModule = (MotionControllerModule *)getModulesList()->at(1);
    pDev = (MotionControllerDevice *)(getModulesList()->at(1)->getDevice());
    if(pDev->m_netctrl==NULL){
        return false;
    }
    if(!pDev->m_netctrl->NetIsConn()){
        return false;
    }
    return true;
}
 // 获取当前操作模式(0:无 1:手动 2:自动)
int  SystemSchedule::GetCurOptMode(void)
{
    if(0x80000==(m_pSystemParameter->sys_ctrl.statebit&0xc0000)){  // 当前模式
        return 2;  // 自动
    }else if(0x40000==(m_pSystemParameter->sys_ctrl.statebit&0xc0000)){
        return 1;  // 手动
    }
    return 0; // 无
}


//获取使能状态(0:关 1:开)
bool SystemSchedule::GetServoState(void)
{
    if((m_pSystemParameter->sys_ctrl.statebit&0x1)==1){
        return true;
    }
    return false;
}

// 设置使能开关(false:关 true:开)
void SystemSchedule::SetServoState(bool bIsOn)
{
    bool bState;
    
    bState = GetServoState();
    if(!bState && bIsOn){
        recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/servoEnable", "yes");
    }else if(bState && !bIsOn){
        recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/servoEnable", "no");
    }
}
void SetServoEnabel(bool enable)
{

}
