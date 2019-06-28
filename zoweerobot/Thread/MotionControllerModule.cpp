/************************************************************
  Copyright (C),
  �ļ�����ParseGCode.cpp
  ���ߣ�zhou    �汾��V1.0    �������ڣ�2018/07/10
  ģ��������ParseGCode���ʵ��
  ��ʷ�޸ļ�¼��<author>    <time>    <version >    <desc>
                  zhou    2018/07/10     V1.0      ���ע��
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
  �������ƣ�MotionControllerModule()
  �������ܣ�MotionControllerModule���캯��
  ���������
  ���������
  �� �� ֵ��
  ��    ע��
**************************************************/
MotionControllerModule::MotionControllerModule()
{
    m_ModuleInfo = ModulesInfo[MOTION_CONTROLLER_ID];
}

/*************************************************
  �������ƣ�MotionControllerModule(QList<InfoUI *>& u, SystemSchedule* ss)
  �������ܣ�MotionControllerModule���캯��
  ���������u UI����
           ss  SystemSchedulerָ��
  ���������
  �� �� ֵ��
  ��    ע��
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
  �������ƣ�~MotionControllerModule()
  �������ܣ�MotionControllerModule��������
  ���������
  ���������
  �� �� ֵ��
  ��    ע��
**************************************************/
MotionControllerModule::~MotionControllerModule()
{
    clearGCodeDataList();
    delete m_pProcessCacheHandler;
}

/*************************************************
  �������ƣ�initMotionControllerModule()
  �������ܣ�MotionControllerModule��Ա������ʼ��
  ���������
  ���������
  �� �� ֵ��
  ��    ע����ִ��ϵͳ��λʱ��Ҫ����
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
    m_SetParmStatus = 127;//0111 1111B  ���в�������

    m_nClampAutoSearchStep = CAS_PRE_WORK;
    m_bIsprevInClampSignalArea = false;

    m_bIsEmsPress = false;
    m_bIsOutEmsPress = false;
}

/*************************************************
  �������ƣ�initModule(QList<InfoUI *>& u)
  �������ܣ�Module��ʼ�� ����UI �����豸
  ���������u UI����
  ���������
  �� �� ֵ��
  ��    ע��
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

/*************************************************
  �������ƣ�connectUIResource()
  �������ܣ�����UI
  ���������
  ���������
  �� �� ֵ��
  ��    ע��
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
  �������ƣ�findUIResource(QList<InfoUI *>& u, ErrorInfo& e)
  �������ܣ���ϵͳ������ע���UI�£�Ѱ��MotionControllerModuleʹ�õ�UI
  ���������u UI����
           e  ������Ϣ
  ���������
  �� �� ֵ��
  ��    ע��
**************************************************/
int MotionControllerModule::findUIResource(QList<InfoUI *>& u, ErrorInfo& e)
{
    for(int i = 0; i < u.size(); i++)
    {
        for(int j = 0; j < u.at(i)->nModulesUser.size(); j++)
        {
            if(u.at(i)->nModulesUser[j] == MOTION_CONTROLLER_ID)   /* ƥ���Module */
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
  �������ƣ�getEmulationCanvs()
  �������ܣ���ȡ�Զ��ӹ�������滭��ָ��
  ���������
  ���������
  �� �� ֵ��
  ��    ע��
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
  �������ƣ�getMachiningStateUI()
  �������ܣ���ȡ�Զ��ӹ�����ָ��
  ���������
  ���������
  �� �� ֵ��
  ��    ע��
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
  �������ƣ�getSimulationStateUI()
  �������ܣ���ȡ�������ָ��
  ���������
  ���������
  �� �� ֵ��
  ��    ע��
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
  �������ƣ�getManualMachiningStateUI()
  �������ܣ���ȡ�ֶ��ӹ�����ָ��
  ���������
  ���������
  �� �� ֵ��
  ��    ע��
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
  �������ƣ�getManualMachiningStateUI()
  �������ܣ���ȡ�ⲿ�豸ͨѶ����ָ��
  ���������
  ���������
  �� �� ֵ��
  ��    ע��
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
  �������ƣ�addDevice(ErrorInfo& e)
  �������ܣ������豸
  ���������
  ���������e  ������Ϣ
  �� �� ֵ��
  ��    ע��
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
  �������ƣ�clearGCodeDataList()
  �������ܣ����m_GCodeDataList����
  ���������
  ���������
  �� �� ֵ��
  ��    ע��
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
  �������ƣ�parseNGFileModel()
  �������ܣ��ۺ���   ��Ӧ������G���밴ť��
  ���������
  ���������
  �� �� ֵ��
  ��    ע��
**************************************************/
void MotionControllerModule::parseNGFileModel()
{
    ParseGCodeModel* pp = pp->loadParseController(this, m_errInfo);

    if(pp != NULL)
    {
        clearGCodeDataList();

        m_errInfo.e_type = DEBUG_NO_ERROR;

        /* ��ʽ1���ڲۺ�����ִ��ȫ������G���� */
        if(!pp->startupParseController(this, &m_GCodeDataList, m_errInfo))
        {
            clearGCodeDataList();
            /* emit�����źŸ�system��system��Ӧ�Ĳ۽��ܣ����е���Ϣ��m_errInfo*/
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


    /* ��ʽ2�����̣߳�MotionControllerModule::run()����ִ��ȫ������G���� */
//    pp->moveToThread(this);
//    start();

}

/*************************************************
  �������ƣ�procEmulationModel()
  �������ܣ���������
  ���������
  ���������
  �� �� ֵ��
  ��    ע��
**************************************************/
void MotionControllerModule::procEmulationModel()
{
    
}

/*************************************************
  �������ƣ�setEmulationStatus(CAutoMachiningStateUi::MotionRunStatue s)
  �������ܣ������Զ��ӹ����������״̬
  ���������s���Զ��ӹ����������״̬
  ���������
  �� �� ֵ��
  ��    ע��
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
  �������ƣ�setProcessBarValue(int s)
  �������ܣ������Զ��ӹ�����ļӹ�����
  ���������s���Զ��ӹ�����ļӹ�����
  ���������
  �� �� ֵ��
  ��    ע��
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
  �������ƣ�pauseEmulation()
  �������ܣ���ͣ����
  ���������
  ���������
  �� �� ֵ��
  ��    ע��
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
  �������ƣ�pauseBreakPointEmulation()
  �������ܣ���ͣ����
  ���������
  ���������
  �� �� ֵ��
  ��    ע��
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
  �������ƣ�continueEmulation()
  �������ܣ��������
  ���������
  ���������
  �� �� ֵ��
  ��    ע��
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
  �������ƣ�changeEmulationSpeed(int v)
  �������ܣ����÷����ٶ�
  ���������
  ���������
  �� �� ֵ��
  ��    ע��
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
  �������ƣ�restartEmulation()
  �������ܣ���������
  ���������
  ���������
  �� �� ֵ��
  ��    ע��
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
  �������ƣ�runToBreakpointEmulation()
  �������ܣ�����ϵ�����
  ���������
  ���������
  �� �� ֵ��
  ��    ע��
**************************************************/
void MotionControllerModule::runToBreakpointEmulation()
{
   /* CAutoMachiningStateUi *pEmulationUi = (CAutoMachiningStateUi *)getUIHandlerByID(UI_AUTO_MACHINING_STATE);
    int nBreakpoint = pEmulationUi->getBreakpoint();

    procEmulationModel();
    m_pEmulationModelHandler->setBreakpointLine(nBreakpoint);*/
}

/*************************************************
  �������ƣ�absorbMotionModule(QString msg, QString arg)
  �������ܣ��ۺ���  ��������emitMotionModuleSignal(QString, QString)���ź�
  ���������
  ���������
  �� �� ֵ��
  ��    ע��
**************************************************/
void MotionControllerModule::absorbMotionModule(QString msg, QString arg)
{
    qDebug()<<"absorbMotionModule start";

    {
        //ϵͳ����ͣ״̬֮����ִ�е���Ϣ//
        if (m_WorkStatus != MOTION_EMERGENCY_STOP_WAITTING)
        {
           /* if (msg == "system reset")
            {
                //����������
                systemReset();
                m_pScheduler->recvRunInfoFromAllModules("ϵͳ��λ");
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
                m_pScheduler->recvRunInfoFromAllModules("�������");
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
		                    //m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("�ӹ�����"));
		                    setProcessBarValue(0);
		                    //m_pScheduler->writeMsgToStatusBar(QString::fromLocal8Bit("�ӹ�����"));
		                    m_bisInPauseWaittingStatus = false;
				     m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("ѭ���ӹ�����"));
		                }
                    		
				 else
				{
				        QMessageBox::information(NULL, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("δʹ�ܻ�δ�����Զ�ģʽ"));
				        
				}
                    		//m_pScheduler->writeMsgToStatusBar(QString::fromLocal8Bit("ѭ���ӹ�����"), STATUSBAR_STATUS_ON_INFO);
                     }
			else
			{
			        QMessageBox::information(NULL, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("���������У�����ֹͣ��"));
			        qDebug()<<"ϵͳ״̬"<<m_WorkStatus;
			}
					 
                }
                else
                {
                  
                    m_bisCycleRun = false;
                    	   		     	
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("ѭ���ӹ�ȡ��"));
                    //m_pScheduler->writeMsgToStatusBar(QString::fromLocal8Bit("ѭ���ӹ�ȡ��"), STATUSBAR_STATUS_OFF_INFO);
                }
            }
            
           
            else
            {
            		qDebug()<<"m_WorkStatus"<<m_WorkStatus;
                //ϵͳ����״̬����ִ�е���Ϣ//
                //if (m_WorkStatus != MOTION_WORK_ERROR_WAITTING)
		 if ((m_WorkStatus != MOTION_WORK_NORMAL_START)
			&&(m_WorkStatus != MOTION_WORK_NORMAL_PROCESS)
			&&(m_WorkStatus != MOTION_WORK_NORMAL_END)
			&&(m_WorkStatus != MOTION_WORK_ERROR_WAITTING))
                  
                {
                    if (msg == "start motion")
                    {                                          
                            if (isReadyToStartMotion())//��ȫ���
                            {
                                if (isServoEnable()&&(2==isServoMode()))
                                {
                                	  qDebug()<<"procMotionModel start";
                                    procMotionModel();
					  qDebug()<<"procMotionModel end";
                                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("�ӹ�����"));
                                    setProcessBarValue(0);
                                    //m_pScheduler->writeMsgToStatusBar(QString::fromLocal8Bit("�ӹ�����"));
				      //m_pScheduler->writeMsgToStatusBar(QString::fromLocal8Bit(""));
                                    m_bisInPauseWaittingStatus = false;
                                }
				   else
					{
					        QMessageBox::information(NULL, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("δʹ�ܻ�δ�����Զ�ģʽ"));
					
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
                    QMessageBox::information(NULL, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("���������У�����ֹͣ��"));
                }
            }
        }
        else
        {
            QMessageBox::information(NULL, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("ϵͳ���ڼ�ͣ״̬�У�����������ͣ��"));
        }
    }
}

/*************************************************
  �������ƣ�stepEmulation()
  �������ܣ���������
  ���������
  ���������
  �� �� ֵ��
  ��    ע��
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
  �������ƣ�absorbManualMotionModule(QString msg, QString arg)
  �������ܣ��ۺ���  ��������emitMotionModuleSignal(QString, QString)���ź�
  ���������
  ���������
  �� �� ֵ��
  ��    ע����Ӧ�ֶ�����
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
                    //m_pScheduler->recvRunInfoFromAllModules("X轴移� + arg + "mm");
			return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("请等待移动指令完成[ 运行状态：%1，指令：%2，内容：%3 ]").arg(m_ManualStatus).arg(msg).arg(arg));
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
                    //m_pScheduler->recvRunInfoFromAllModules("X轴移� + arg + "mm");
			return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("请等待移动指令完成[ 运行状态：%1，指令：%2，内容：%3 ]").arg(m_ManualStatus).arg(msg).arg(arg));
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
                    //m_pScheduler->recvRunInfoFromAllModules("Y轴移� + arg + "mm");
                    return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("请等待移动指令完成[ 运行状态：%1，指令：%2，内容：%3 ]").arg(m_ManualStatus).arg(msg).arg(arg));
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
                    //m_pScheduler->recvRunInfoFromAllModules("X轴移� + arg + "mm");
			return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("请等待移动指令完成[ 运行状态：%1，指令：%2，内容：%3 ]").arg(m_ManualStatus).arg(msg).arg(arg));
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
                    //m_pScheduler->recvRunInfoFromAllModules("Z轴移� + arg + "mm");
                    return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("请等待移动指令完成[ 运行状态：%1，指令：%2，内容：%3 ]").arg(m_ManualStatus).arg(msg).arg(arg));
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
                    //m_pScheduler->recvRunInfoFromAllModules("X轴移� + arg + "mm");
			return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("请等待移动指令完成[ 运行状态：%1，指令：%2，内容：%3 ]").arg(m_ManualStatus).arg(msg).arg(arg));
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
                    //m_pScheduler->recvRunInfoFromAllModules("R轴移� + arg + "mm");
                    return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("请等待移动指令完成[ 运行状态：%1，指令：%2，内容：%3 ]").arg(m_ManualStatus).arg(msg).arg(arg));
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
                    //m_pScheduler->recvRunInfoFromAllModules("X轴移� + arg + "mm");
			return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("请等待移动指令完成[ 运行状态：%1，指令：%2，内容：%3 ]").arg(m_ManualStatus).arg(msg).arg(arg));
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
                    //m_pScheduler->recvRunInfoFromAllModules("R轴移� + arg + "mm");
                    return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("请等待移动指令完成[ 运行状态：%1，指令：%2，内容：%3 ]").arg(m_ManualStatus).arg(msg).arg(arg));
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
                    //m_pScheduler->recvRunInfoFromAllModules("X轴移� + arg + "mm");
			return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("请等待移动指令完成[ 运行状态：%1，指令：%2，内容：%3 ]").arg(m_ManualStatus).arg(msg).arg(arg));
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
                    //m_pScheduler->recvRunInfoFromAllModules("R轴移� + arg + "mm");
                    return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("请等待移动指令完成[ 运行状态：%1，指令：%2，内容：%3 ]").arg(m_ManualStatus).arg(msg).arg(arg));
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
                    //m_pScheduler->recvRunInfoFromAllModules("X轴移� + arg + "mm");
			return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("请等待移动指令完成[ 运行状态：%1，指令：%2，内容：%3 ]").arg(m_ManualStatus).arg(msg).arg(arg));
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
                    //m_pScheduler->recvRunInfoFromAllModules("X轴移� + arg + "mm");
                    return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("请等待移动指令完成[ 运行状态：%1，指令：%2，内容：%3 ]").arg(m_ManualStatus).arg(msg).arg(arg));
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
                    //m_pScheduler->recvRunInfoFromAllModules("X轴移� + arg + "mm");
			return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("请等待移动指令完成[ 运行状态：%1，指令：%2，内容：%3 ]").arg(m_ManualStatus).arg(msg).arg(arg));
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
                    //m_pScheduler->recvRunInfoFromAllModules("Y轴移� + arg + "mm");
                    return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("请等待移动指令完成[ 运行状态：%1，指令：%2，内容：%3 ]").arg(m_ManualStatus).arg(msg).arg(arg));
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
                    //m_pScheduler->recvRunInfoFromAllModules("X轴移� + arg + "mm");
			return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("请等待移动指令完成[ 运行状态：%1，指令：%2，内容：%3 ]").arg(m_ManualStatus).arg(msg).arg(arg));
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
                    //m_pScheduler->recvRunInfoFromAllModules("Z轴移� + arg + "mm");
                    return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("请等待移动指令完成[ 运行状态：%1，指令：%2，内容：%3 ]").arg(m_ManualStatus).arg(msg).arg(arg));
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
                    //m_pScheduler->recvRunInfoFromAllModules("X轴移� + arg + "mm");
			return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("请等待移动指令完成[ 运行状态：%1，指令：%2，内容：%3 ]").arg(m_ManualStatus).arg(msg).arg(arg));
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
                    //m_pScheduler->recvRunInfoFromAllModules("R轴移� + arg + "mm");
                    return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("请等待移动指令完成[ 运行状态：%1，指令：%2，内容：%3 ]").arg(m_ManualStatus).arg(msg).arg(arg));
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
                    //m_pScheduler->recvRunInfoFromAllModules("X轴移� + arg + "mm");
			return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("请等待移动指令完成[ 运行状态：%1，指令：%2，内容：%3 ]").arg(m_ManualStatus).arg(msg).arg(arg));
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
                    //m_pScheduler->recvRunInfoFromAllModules("R轴移� + arg + "mm");
                    return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("请等待移动指令完成[ 运行状态：%1，指令：%2，内容：%3 ]").arg(m_ManualStatus).arg(msg).arg(arg));
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
                    //m_pScheduler->recvRunInfoFromAllModules("X轴移� + arg + "mm");
			return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("请等待移动指令完成[ 运行状态：%1，指令：%2，内容：%3 ]").arg(m_ManualStatus).arg(msg).arg(arg));
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
                    //m_pScheduler->recvRunInfoFromAllModules("R轴移� + arg + "mm");
                    return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("请等待移动指令完成[ 运行状态：%1，指令：%2，内容：%3 ]").arg(m_ManualStatus).arg(msg).arg(arg));
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
                    //m_pScheduler->recvRunInfoFromAllModules("X轴移� + arg + "mm");
			return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("请等待移动指令完成[ 运行状态：%1，指令：%2，内容：%3 ]").arg(m_ManualStatus).arg(msg).arg(arg));
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
                    //m_pScheduler->recvRunInfoFromAllModules("X轴移� + arg + "mm");
			return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("请等待移动指令完成[ 运行状态：%1，指令：%2，内容：%3 ]").arg(m_ManualStatus).arg(msg).arg(arg));
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
                    //m_pScheduler->recvRunInfoFromAllModules("X轴移� + arg + "mm");
			return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("请等待移动指令完成[ 运行状态：%1，指令：%2，内容：%3 ]").arg(m_ManualStatus).arg(msg).arg(arg));
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
                    //m_pScheduler->recvRunInfoFromAllModules("X轴移� + arg + "mm");
			return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("请等待移动指令完成[ 运行状态：%1，指令：%2，内容：%3 ]").arg(m_ManualStatus).arg(msg).arg(arg));
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
                    //m_pScheduler->recvRunInfoFromAllModules("X轴移� + arg + "mm");
			return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("请等待移动指令完成[ 运行状态：%1，指令：%2，内容：%3 ]").arg(m_ManualStatus).arg(msg).arg(arg));
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
                    //m_pScheduler->recvRunInfoFromAllModules("X轴移� + arg + "mm");
			return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("请等待移动指令完成[ 运行状态：%1，指令：%2，内容：%3 ]").arg(m_ManualStatus).arg(msg).arg(arg));
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
                    //m_pScheduler->recvRunInfoFromAllModules("X轴移� + arg + "mm");
			return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("请等待移动指令完成[ 运行状态：%1，指令：%2，内容：%3 ]").arg(m_ManualStatus).arg(msg).arg(arg));
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
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("���ò���"));
                    return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("���������㣬����ʧ��"));
                    return;
                }
            }
            else if(arg=="linkparam")
	  	{
                if (m_ManualStatus == MANUAL_IDLE)
                {
                   
                    m_CtrlStatus = SETTING_LINKPARAMER;
                    m_WorkStatus = MOTION_WORK_MANUAL_SET;
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("�������˲���"));
                    return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("���������㣬��������ʧ��"));
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
                   
		     m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("�������"));
                    m_pScheduler->writeMsgToStatusBar("");	
                    return;
                }
                else
                {
                    m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("���������㣬���ʧ��"));
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
		        QMessageBox::information(NULL, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("���������У�����ֹͣ��"));
		        qDebug()<<"ϵͳ״̬"<<m_WorkStatus;
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
                m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("�ŷ�����"));
                //m_pScheduler->writeMsgToStatusBar(QString::fromLocal8Bit("�ŷ�����"));
            }
            else
            {
                m_bIsServoEnable = false;
                m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("�ŷ��ر�"));
                //m_pScheduler->writeMsgToStatusBar(QString::fromLocal8Bit("�ŷ��ر�"));
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
			        QMessageBox::information(NULL, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("���������У�����ֹͣ��"));
			        qDebug()<<"ϵͳ״̬"<<m_WorkStatus;
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
					        QMessageBox::information(NULL, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("�������ʾ�̵㣡"));
					        qDebug()<<"ʾ�̵����"<<m_pScheduler->m_pSystemParameter->teachpoint[m_movepointnum].index;
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
				        QMessageBox::information(NULL, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("���������У�����ֹͣ��"));
				        qDebug()<<"�ֶ�״̬"<<m_ManualStatus;
				}
	            	}
			 else
			{
			        QMessageBox::information(NULL, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("���������У�����ֹͣ��"));
			        qDebug()<<"ϵͳ״̬"<<m_WorkStatus;
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
        QMessageBox::information(NULL, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("ϵͳ״̬������ִ�и�ָ�"));
        qDebug()<<"ϵͳ״̬"<<m_WorkStatus;
    }

}

/*************************************************
  �������ƣ�runTimer()
  �������ܣ����з��� ��ʱ��
  ���������
  ���������
  �� �� ֵ��
  ��    ע��
**************************************************/
void MotionControllerModule::runTimer()
{
   
}

/*************************************************
  �������ƣ�procMotionModel()
  �������ܣ������ӹ�ǰ��׼������
  ���������
  ���������
  �� �� ֵ��
  ��    ע��
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
            //m_GCodeRunSerialNum = 0;//�տ�ʼ�ӹ��ļ�ʱ��ʼ����ֵ,����MOTION_WORK_NORMAL_START״̬ͬʱ������ͣ����ͣ�ָ����ʽ���ʼ�����ڴ˴�
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
  �������ƣ�run()
  �������ܣ������ӹ��߳�����
  ���������
  ���������
  �� �� ֵ��
  ��    ע��
**************************************************/
void MotionControllerModule::run()
{
    bool flag = true;
    addDevice(m_errInfo);

    if(((MotionControllerDevice *)m_pDevice)->creatTCPSocket() != 0)
    {
        m_errInfo.insertDebug(DEBUG_SERIOUS_ERROR, "TCP Socket �������ɹ��������������Ӳ�����������");
        m_pScheduler->recvErrMsgFromAllModules(m_errInfo);

        //m_WorkStatus = MOTION_NO_CONNECT_NET;
        //m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "not connect");
    }
	if(((MotionControllerDevice *)m_pDevice)->creatUdPSocket() != 0)
    {
        m_errInfo.insertDebug(DEBUG_SERIOUS_ERROR, "Socket �������ɹ��������������Ӳ�����������");
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
  �������ƣ�doMotion()
  �������ܣ��������������У������ӹ��̣߳�
  ���������
  ���������
  �� �� ֵ��
  ��    ע��
**************************************************/
void MotionControllerModule::doMotion()
{
    //qDebug()<<"d st"<<m_WorkStatus;
    //qDebug()<<"m_WorkStatus = "<<m_WorkStatus;
    int sts = 1;
    switch (m_WorkStatus)
    {
    case MOTION_WORK_NORMAL_BOOT:
        //if(((MotionControllerDevice *)m_pDevice)->sendMsg(MSG_NORMAL_BOOT, m_errInfo))
        /*if(((MotionControllerDevice *)m_pDevice)->normalTurnOn(m_errInfo))
        {
            m_pScheduler->recvRunInfoFromAllModules("������������֪ͨ");
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
	     if (((MotionControllerDevice *)m_pDevice)->setOrian(m_nZeroaxis))
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
        if (((MotionControllerDevice *)m_pDevice)->setMotionMode(2))//改变运动状态
        {
            //m_MotionMode = Mode_Auto;
            m_WorkStatus = MOTION_WORK_NORMAL_PROCESS;
            
                //m_StartStatus = SETTING_PARM;
            m_GCodeRunSerialNum = 0;
	    nCurrentLineSerialNum=0;
		m_nDelayTime=0;
            qDebug()<<"control="<<m_nMotionControllerRunNum<<" last="<<m_nLastPartGCodeNum;

            m_pProcessCacheHandler->m_bIsManualMotion = false;
            m_pScheduler->recvRunInfoFromAllModules("����FIFO�˶��ɹ�");
        }
	
        break;
    case MOTION_WORK_NORMAL_PROCESS:
    {
        if(m_GCodeRunSerialNum < m_GCodeActions->size())
        {

	    if (m_GCodeRunSerialNum < 0)
            {
                m_errInfo.insertDebug(DEBUG_SERIOUS_ERROR, "action ���м�������");
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
                m_pScheduler->recvRunInfoFromAllModules("加工完成");
            }
            else 
            {
            	    
		    if (m_pScheduler->m_pSystemParameter->sys_ctrl.decode_id>= nCurrentLineSerialNum)//(m_nCurCtlRunId >= m_MotionCodeList.last()->m_nCodeId)//控制器反馈完成
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
	                    //m_pScheduler->recvRunInfoFromAllModules("加工完成");
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
        if (m_bisCycleRun)//循环加工
        {
                    m_GCodeRunSerialNum=0;
			nCurrentLineSerialNum=0;
                    m_WorkStatus = MOTION_WORK_NORMAL_PROCESS;
                    //m_pScheduler->recvRunInfoFromAllModules(tr("循环加工第%1次完成，共%2次").arg(m_nCurCircleRunNum).arg(m_nCricleRunCnt));
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
					
			     if (((MotionControllerDevice *)m_pDevice)->backHome())
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
	    qDebug()<<"manual SAVETEACHPOINT";
           if (saveTeachPoint())
           {
           	   	    
		                m_ManualStatus = MANUAL_SAVETEACHPOINTTEXT;
				  //m_ManualStatus = MANUAL_IDLE;
		                //m_pProcessCacheHandler->m_bIsManualMotion = true;
		   
           }
            break;
	case MANUAL_SAVETEACHPOINTTEXT:
	    qDebug()<<"manual SAVETEACHPOINTTEXT";
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
            m_pScheduler->recvRunInfoFromAllModules("����ָֹͣ��ͳɹ�");
            m_WorkStatus = MOTION_EMERGENCY_STOP_WAITTING;//MOTION_WORK_IDLE;
        }
        else
        {
            m_pScheduler->recvRunInfoFromAllModules("����ָֹͣ���ʧ��");
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
                m_pScheduler->recvRunInfoFromAllModules("�����������ɹ�");
                //m_pScheduler->writeMsgToStatusBar("�����������ɹ�");//domotion()��Ҫ�����̲߳��ܲ��ᱨ����������ʱ��run()�������߳��ƶ������̣߳�
                m_WorkStatus = MOTION_NO_CONNECT_NET;
            }
        }break;
    case MOTION_CLOSE_CONNECT_NET:
        if (((MotionControllerDevice *)m_pDevice)->closeNet())
        {
            //m_pScheduler->recvRunInfoFromAllModules("�Ͽ��������ӳɹ�");
            //m_pScheduler->writeMsgToStatusBar("�Ͽ��������ӳɹ�");
            m_WorkStatus = MOTION_NO_CONNECT_NET;
        }
        else
        {
            m_pScheduler->recvRunInfoFromAllModules("�Ͽ���������ʧ��");
            //m_pScheduler->writeMsgToStatusBar("�Ͽ���������ʧ��");
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
            //m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("伺服启动/关闭指令发送成�));
        }
        break;
    
    case MOTION_CLEAR_ERROR:
        if(((MotionControllerDevice *)m_pDevice)->clearSts(m_errInfo))
        {
            m_pScheduler->recvRunInfoFromAllModules("���������״̬�ɹ�");
            m_WorkStatus = MOTION_WORK_IDLE;
        }
        else
        {
            m_pScheduler->recvRunInfoFromAllModules("���������״̬ʧ��");
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
            m_pScheduler->recvRunInfoFromAllModules("�����������ͣ״̬�ɹ�");
            m_WorkStatus = MOTION_WORK_NORMAL_BOOT;
        }
        else
        {
            m_pScheduler->recvRunInfoFromAllModules("�����������ͣ״̬ʧ��");
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
    //qDebug()<<"d en"<<m_WorkStatus;
    feedbackSystemState();
    //((MotionControllerDevice *)m_pDevice)->m_netctrl->ontimeout();
}

void MotionControllerModule::onnetctrltimeout()
{
	((MotionControllerDevice *)m_pDevice)->m_netctrl->ontimeout();
}

/*************************************************
  �������ƣ�systemReset()
  �������ܣ�ϵͳ��λ
  ���������
  ���������
  �� �� ֵ��
  ��    ע��
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

int MotionControllerModule::saveTeachPoint()
{
    int cnt;
     //static int overcnt=0;
     if(m_startteachpoint<TEACHPIONTNUM)
     {
           for(;m_startteachpoint<TEACHPIONTNUM;)
           {
		    if (((MotionControllerDevice *)m_pDevice)->PutTeachPoint(m_startteachpoint, m_startteachpoint+3, (unsigned char*)&m_pScheduler->getSystemParameterHandler()->teachpoint[m_startteachpoint]))
		    {
		        m_startteachpoint+=4;
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
     /*else if(m_startteachpoint>=TEACHPIONTNUM)
     {
            //if (((MotionControllerDevice *)m_pDevice)->PutTeachPoint(((TEACHPIONTNUM/6)*6), TEACHPIONTNUM-1, (unsigned char*)&m_pScheduler->getSystemParameterHandler()->teachpoint[((TEACHPIONTNUM/6)*6)]))
	    {
	        m_startteachpoint=TEACHPIONTNUM;
	    }		
     }*/
     else if(m_startteachpoint>=TEACHPIONTNUM)
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
  �������ƣ�runOneActions()
  �������ܣ�ִ��һ��action
  ���������
  ���������
  �� �� ֵ��
  ��    ע��
**************************************************/
bool MotionControllerModule::runOneActions()
{
    static bool runIOWait = true;
    switch (m_GCodeActions->at(m_GCodeRunSerialNum)->m_type)
    {
    case ACTION_MG_TYPE:
        qDebug()<<"mRun"<<m_nMotionControllerRunNum<<"M"<<m_pProcessCacheHandler->m_nMCodeCnt<<"cur"<<m_GCodeRunSerialNum<<"last"<<m_nLastPartGCodeNum;
        if (m_nMotionControllerRunNum + m_pProcessCacheHandler->m_nMCodeCnt - m_nLastPartGCodeNum == m_GCodeRunSerialNum)//�ȴ����е���ǰλ��
        {
            int nM = ((MGAction *)m_GCodeActions->at(m_GCodeRunSerialNum))->m_nMGCode;
            m_GCodeRunSerialNum++;
            m_pProcessCacheHandler->m_nMCodeCnt++;
            switch (nM)
            {
            case 0://����ֹͣ
                m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "pause emulation");
                m_pScheduler->recvRunInfoFromAllModules("M00 ����ֹͣ");
                //m_pScheduler->writeMsgToStatusBar("M00 ����ֹͣ");
                break;
            case 1://ѡ��ֹͣ
                if (m_bIsM01Effect)
                {
                    m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "pause emulation");
                    m_pScheduler->recvRunInfoFromAllModules("M01 ѡ��ֹͣ");
                    //m_pScheduler->writeMsgToStatusBar("M01 ѡ��ֹͣ");
                }
                break;
            case 2://�������
            case 30://�������
                m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "stop emulation");
                m_pScheduler->recvRunInfoFromAllModules("M02/M30 �������");
                //m_pScheduler->writeMsgToStatusBar("M02/M30 �������");
                break;
            case 13://��ѹģʽ��λ
                break;
            default:
                break;
            }
            //m_WorkStatus = MOTION_WORK_ERROR_WAITTING;
        }
        break;
   /* case ACTION_IO_TYPE:
        qDebug()<<"IO re"<<m_nMotionControllerRunNum<<" m"<<m_pProcessCacheHandler->m_nMCodeCnt<<" L"<<m_nLastPartGCodeNum<<" w"<<m_GCodeRunSerialNum;
        if (m_nMotionControllerRunNum + m_pProcessCacheHandler->m_nMCodeCnt - m_nLastPartGCodeNum == m_GCodeRunSerialNum)//�ȴ����е���ǰλ��
        {
            if (runIOWait)
            {
                qDebug()<<"***run io "<<((IOAction *)m_GCodeActions->at(m_GCodeRunSerialNum))->m_nIONum<<((IOAction *)m_GCodeActions->at(m_GCodeRunSerialNum))->m_bIoSts;
                //m_pPlcControllerModuleHandle->setPlcIOStatus(((IOAction *)m_GCodeActions->at(m_GCodeRunSerialNum))->m_nIONum, ((IOAction *)m_GCodeActions->at(m_GCodeRunSerialNum))->m_bIoSts);
                runIOWait = false;
            }
            //�ж�IO�Ƿ�ִ�е�λ��������Ҫ�����Ƿ���Ҫ��ʱ�ȴ���
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
        if (m_nDelayTimeSurplus <= 0)//��ʼ����ֵ
        {
            m_nDelayTimeSurplus = (((DelayAction *)m_GCodeActions->at(m_GCodeRunSerialNum))->m_nDelayTime);
        }

        if (m_nMotionControllerRunNum + m_pProcessCacheHandler->m_nMCodeCnt - m_nLastPartGCodeNum == m_GCodeRunSerialNum)//�ȴ����е���ǰλ��
        {
            m_nDelayTimeSurplus -= 100;
            if (m_nDelayTimeSurplus <= 0)//��ʱʱ�䵽���ػؼӹ�
            {
                m_GCodeRunSerialNum++;
                m_pProcessCacheHandler->m_nMCodeCnt++;
            }
        }
        break;
    case ACTION_TOOL_CHANGE_TYPE:
        qDebug()<<"TC re"<<m_nMotionControllerRunNum<<" m"<<m_pProcessCacheHandler->m_nMCodeCnt<<" L"<<m_nLastPartGCodeNum<<" w"<<m_GCodeRunSerialNum;
        if (m_nMotionControllerRunNum + m_pProcessCacheHandler->m_nMCodeCnt - m_nLastPartGCodeNum == m_GCodeRunSerialNum)//�ȴ����е���ǰλ��
        {
           // if (m_pPlcControllerModuleHandle->getInputIOStatus(IO_CANTOOLCHANGE_SIGNAL_PLC_M))
            {
                int temp = ((MotionControllerDevice *)m_pDevice)->runOneActions(m_GCodeActions->at(m_GCodeRunSerialNum), m_pProcessCacheHandler, m_pScheduler->getSystemParameterHandler(), m_errInfo);
                if (temp > 0)
                {
                    m_GCodeRunSerialNum++;
                }
                else if (temp == 0)//�������������·��ͣ���ԭ�ۼ���������
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
        if (m_nMotionControllerRunNum + m_pProcessCacheHandler->m_nMCodeCnt - m_nLastPartGCodeNum == m_GCodeRunSerialNum)//�ȴ����е���ǰλ��
        {
           // if (m_pPlcControllerModuleHandle->getInputIOStatus(IO_ISROTATETOOL_SIGNAL_PLC_M))//�ж��Ƿ���תģ
            {
                int temp = ((MotionControllerDevice *)m_pDevice)->runOneActions(m_GCodeActions->at(m_GCodeRunSerialNum), m_pProcessCacheHandler, m_pScheduler->getSystemParameterHandler(), m_errInfo);
                if (temp > 0)
                {
                    m_GCodeRunSerialNum++;
                }
                else if (temp == 0)//�������������·��ͣ���ԭ�ۼ���������
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
	       	       //������������
	       	       
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
        else if (temp == 0)//�������������·��ͣ���ԭ�ۼ���������
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
        else if (temp == 0)//�������������·��ͣ���ԭ�ۼ���������
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
	       	       //�ȴ�����
	       	      	
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
        else if (temp == 0)//�������������·��ͣ���ԭ�ۼ���������
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
        if (timeCnt%4 == 0 && m_nClampAutoSearchStep == CAS_PRE_WORK)//��ǯ�����ڼ䲻��ȡ״̬
        {
            if(((MotionControllerDevice *)m_pDevice)->getSts(&(m_pProcessCacheHandler->m_nMotionControllerSts), m_errInfo))
            {
                qDebug()<<"@@@@@@@@sts="<<m_pProcessCacheHandler->m_nMotionControllerSts<<"#########";
                if ((m_pProcessCacheHandler->m_nMotionControllerSts & 0xffc00000) != 0)//������λ
                {
                    //qDebug()<<"�����λ";
                    m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "limit trigger");
                    ((MotionControllerDevice *)m_pDevice)->clearSts(m_errInfo);//���״̬
                }
            }
        }
        timeCnt--;
    }
    else //1s����һ��
    {
        qDebug()<<"feedback"<<((MotionControllerDevice *)m_pDevice)->m_netctrl->gb_ctrl.decode_id;//m_nMotionControllerRunNum;
        timeCnt = 10;
        //�Զ�����    
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
        if (fabs(angle - m_pScheduler->getSystemParameterHandler()->m_mouldsParm.at(i).m_dPosAngle) < 0.1)//�������0.1��
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
	   if (!m_bIsServoEnable)//if ((m_pProcessCacheHandler->m_nMotionControllerSts & checkflag) != 0)//伺服启动
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
             QMessageBox::warning(NULL, "警告", "电机未使�, QMessageBox::Yes, QMessageBox::Yes);
         }*/
         m_pScheduler->recvRunInfoFromAllModules(QString::fromLocal8Bit("���δʹ�ܣ�������ʹ�ܣ�"));
	 QMessageBox::warning(NULL, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("���δʹ�ܣ�������ʹ�ܣ�"));
         return false;
     }
}

/*bool MotionControllerModule::isServoEnable()
{

     
	   if (m_bIsServoEnable)//if ((m_pProcessCacheHandler->m_nMotionControllerSts & checkflag) != 0)//伺服启动
	    {
	    	
	        return true;
	    }
	    else
	    {
	       
	        m_pScheduler->recvRunInfoFromAllModules("电机未使能，不能执行");
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
        QMessageBox::information(NULL, tr("����"), tr("��λ��δ��λ��"));
        return false;
    }*/
    return true;
}

bool MotionControllerModule::isAxisTEnableMove()
{
     return true;
    /*if (m_pPlcControllerModuleHandle->getInputIOStatus(IO_CANTOOLCHANGE_SIGNAL_PLC_M))//T�ᰲȫ���
    {
        if (m_pProcessCacheHandler->m_curPoint.v < 1)
        {
            if (!m_pPlcControllerModuleHandle->getInputIOStatus(IO_SAFTDOOROPEN_SIGNAL_PLC_M))
            {
                return true;
            }
            else
            {
                QMessageBox::information(NULL, tr("����"), tr("��ȫ��û�йر�"));
                return false;
            }
        }
        else
        {
            QMessageBox::information(NULL, tr("����"), tr("V��û���������㣡"));
            return false;
        }
    }
    else
    {
        QMessageBox::information(NULL, tr("����"), tr("��λ��δ��λ��"));
        return false;
    }*/
}








