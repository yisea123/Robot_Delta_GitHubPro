/**
Copyright (c)
@file BaseModule.h
@author zhou
@version 1.0
@date 2018/07/10
@brief BaseModule类型的定义。
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
    ModuleInfo           m_ModuleInfo;       /**< 模块的信息 */
    QList<InfoUI *>      m_UIs;              /**< 模块需要用到的UI链表 */
    BaseDevice*          m_pDevice;          /**< 设备，一个Module只有一个设备 */
    ErrorInfo            m_errInfo;           /**< 每一个Module都有一个错误信息 */
    SystemSchedule*      m_pScheduler;         /**< 系统的句柄 */

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
