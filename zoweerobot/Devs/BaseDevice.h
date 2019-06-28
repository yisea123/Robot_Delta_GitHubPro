/**
Copyright (c)
@file BaseDevice.h
@author zhou
@version 1.0
@date 2018/07/10
@brief BaseDevice类型的定义。
*/

#ifndef BASEDEVICE_H
#define BASEDEVICE_H

#include <QObject>
#include "ComonTypes.h"

class BaseModule;

class BaseDevice : public QObject
{
    Q_OBJECT
public:
    explicit BaseDevice(QObject *parent = 0);

    virtual int startDevice(ErrorInfo& e);
    virtual int pauseDevice(ErrorInfo& e);
    virtual int openDevice(ErrorInfo& e);
    virtual int stopDevice(ErrorInfo& e);
    virtual int restartDevice(ErrorInfo& e);

    QString getDeviceName();


protected:
    QString     m_sDeviceName;    /**< 设备名称 */
    BaseModule* m_pModuleHandle;  /**< 设备所属的Module句柄 */
    WorkStatus  m_workStatus;     /**< 设备工作状态 */

private:
    virtual int initDevice(ErrorInfo& e);


};

#endif // BASEDEVICE_H
