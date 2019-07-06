/**
Copyright (c)
@file SystemSchedule.h
@author zhou
@version 1.0
@date 2018/07/10
@brief SystemSchedule���͵Ķ��塣
*/

#ifndef CSYSTEMSCHEDULE_H
#define CSYSTEMSCHEDULE_H

#include <QObject>
#include <QList>
#include <QThread>

#include "ComonTypes.h"
#include "SystemProcessInfo.h"
//#include "KeyControllerModule.h"
#include "GlobalDefine.h"
#include <QMessageBox>


class QWidget;
class MainWindow;

class BaseModule;
class MotionControllerModule;
class SystemProcessInfo;
class SystemParameter;
class UserParameter;
class CWarningMsgDiagnosisUi;

class SystemRunPram
{
public:
    SystemRunPram()
    {         
    }

	//float SystemParam[256];

};

class SystemSchedule : public QObject
{
    Q_OBJECT
public:
//    explicit SystemSchedule(MainWindow *w, QObject *parent = 0);
    explicit SystemSchedule(MainWindow *w, UserParameter parm, QObject *parent = 0);

    QList<BaseModule *>* getModulesList()
    {
        return &m_Modules;
    }

    MainWindow* getMainWidget()
    {
        return m_MainWidget;
    }

    void  setNGFilePath(QString  s);
    QString getNGFilePath();
    void  clearFileContextList();
    QList<ContextRecord>*  getNGFileContexts();
    SystemParameter*       getSystemParameterHandler();
    //void recvMsgFromWindows(QString msg);
    void recvMsgFromWindows(int moduleId, QString msg, QString arg = "");
    

    void setParmFilePath(QString s);   /*< ���ò����ļ�·�� */
    //QString getParmFilePath();    /*< ��ȡ�����ļ�·�� */

    void recvErrMsgFromAllModules(ErrorInfo  e);
    void recvRunInfoFromAllModules(QString  s);

    void writeMsgToStatusBar(QString msg, int level);
    void writeMsgToStatusBar(QString msg);

    void reShowAllUIs(UserParameter user);
    bool ReadUserInformation();
    bool WriteUserInformation();
    bool ReadSystemParamInformation(const QString &filename);
    bool ReadSystemParamInformation();    
    bool WriteSystemParamInformation();

    bool ReadAxisParamInformation(const QString &filename);
    bool ReadAxisParamInformation();
    bool WriteAxisParamInformation();

    bool ReadPidParaGrp(const QString &filename);
    bool SavePidParaGrp(const QString &filename);

    bool NetIsConnect(void);
    int  GetCurOptMode(void);  // ��ȡ��ǰ����ģʽ(0:�� 1:�ֶ� 2:�Զ�)
    bool GetServoState(void); //��ȡʹ��״̬(0:�� 1:��)
    void SetServoState(bool bIsOn); // ����ʹ�ܿ���(false:�� true:��)

private:
    void registerAllUI(UserParameter user);
//    void registerGCodeEditUI(QWidget *w);
    void registerUI(QWidget *w, QString name, int nType, int nSerial, QVector<int> &nModulesUser);
    void registerAutoMachiningStateUI();
    void registerTeachPointStateUI();
    void registerManualMachiningStateUI();
    void registerIOStateUI();
    void registerMachiningSimulationUI();
    void registerSystemPreferencesUI();
    void registerToolCoorSetUI();
    void registerJointParamSetUI();
    void registerStructParamSetUI();
    void registerPIDParamSetUI();
    void registerZeroParamSetUI();
    void registerUserCoorSetUI();
    void registerUserLevelSetUI();
    void registerAxesPreferencesUI();
    void registerCraftPreferencesUI();
    void registerToolPreferencesUI();
    void registerWarningMsgDiagnosisUI();
    void registerWarningRecordUI();
    void registerSystemInformationUI();
    void registerCommunicationsSettingsUI();
    void registerAdvancedSystemSettingsUI();

    void registerAllModules();
    void addAllModules();
    int  addMotionControllerModule();
   // int  addEmulationControllerModule();
    int  addXmlControllerModule();
   // int  addKeyControllerModule();
   // int  addPLCControllerModule();
    void startAllModules();
    void initEnvironment(UserParameter parm);
    
    void showSystemRunInfo(QString s);
    void showSystemErrorInfo(QString s);

    void writeErrInfoToFile(QString errInfo);
    void writeRunInfoToFile(QString runInfo);
    void initRunInfoFile();

public:

	
	SystemParameter*      m_pSystemParameter;
	SystemRunPram         m_systemRunParm;
private:
    QList<InfoUI *>       m_Widgets;      /**< ����UI�ļ������� */
    MainWindow*           m_MainWidget;   /**< MainWindow���� */
    QList<BaseModule *>   m_Modules;      /**< ����Module�ļ������� */
    SystemProcessInfo     m_systemInfo;   /**< system����ʱ�ĵ�����Ϣ */

    QList<ContextRecord>  m_fileContexts;      /**< G�ļ�����QString�б��� */
    QString               m_filePath;        /**< �ļ�·�� */
    //SystemParameter*      m_pSystemParameter;

    QString               m_sParmFilePath;        /**< �����ļ�·�� */

    //UserParameter m_curUserInfo;

private:
    
    CWarningMsgDiagnosisUi*    m_pWarningMsgDiagnosisUi;

signals:
//    void startEmulationSignal();
//    void continueEmulationSignal();
//    void stepEmulationSignal();
//    void runToBreakpointEmulationSignal();

//    void saveXMLDocSignal();
//    void readXMLDocSignal();
//    void writeXMLParmSignal(QString s);
//    void readXMLParmSignal(QString s);
//    void recoverXMLParmSignal();

    void emitXMLModuleSignal(QString, QString);
    void emitPLCModuleSignal(QString, QString);
    void emitMotionModuleSignal(QString, QString);
    void emitEmulationModuleSignal(QString, QString);
    void emitManualMotionModuleSignal(QString, QString);
    void emitKeyModuleSignal(QString, QString);
};

#endif // CSYSTEMSCHEDULE_H
