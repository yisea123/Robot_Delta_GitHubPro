/**
Copyright (c)
@file BaseModule.h
@author zhou
@version 1.0
@date 2018/07/10
@brief BaseModule���͵Ķ��塣
*/

#ifndef BASEMODULE_H
#define BASEMODULE_H

#include <QObject>
#include <QThread>
#include <QWidget>
#include "ComonTypes.h"

class BaseDevice;
class SystemSchedule;
class SystemParameter;

class BaseModule : public QThread
{
    Q_OBJECT
public:
    explicit BaseModule();
    virtual ~BaseModule();

    virtual int initModule(QList<InfoUI *>& u);
    int   getErrorType();
    ModuleInfo* getModuleInfo();
    BaseDevice* getDevice();
    QList<InfoUI *>* getUIs();
    InfoUI*    getUIbyNames(QString s, ErrorInfo& e);
    SystemSchedule*  getSystemScheduleHandle();
    SystemParameter* getSystemParameterHandle();
    ErrorInfo& getErrorInfo();
    QWidget*  getUIHandlerByID(int id);

protected:
    virtual  void run();


protected:
    ModuleInfo           m_ModuleInfo;       /**< ģ�����Ϣ */
    QList<InfoUI *>      m_UIs;              /**< ģ����Ҫ�õ���UI���� */
    BaseDevice*          m_pDevice;          /**< �豸��һ��Moduleֻ��һ���豸 */
    ErrorInfo            m_errInfo;           /**< ÿһ��Module����һ��������Ϣ */
    SystemSchedule*      m_pScheduler;         /**< ϵͳ�ľ�� */

    QTimer*              m_ModuleTimer;

signals:
    
public slots:
    virtual void parseNGFileModel();
    virtual void runTimer();

    virtual void absorbXMLModule(QString, QString);
    virtual void absorbPLCModule(QString, QString);
    virtual void absorbMotionModule(QString, QString);
    virtual void absorbEmulationModule(QString, QString);
    virtual void absorbManualMotionModule(QString, QString);
    virtual void absorbKeyModule(QString, QString);


};

#endif // BASEMODULE_H
