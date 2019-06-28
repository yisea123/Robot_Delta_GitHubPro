/************************************************************
  Copyright (C),
  文件名：SystemProcessInfo.cpp
  作者：zhou    版本：V1.0    创建日期：2018/07/10
  模块描述：SystemProcessInfo类的实现
  历史修改记录：<author>    <time>    <version >    <desc>
                  zhou    2018/07/10     V1.0      添加注释
*************************************************************/

#include "SystemProcessInfo.h"
#include <QDebug>
#include <QWidget>

#include "ComonTypes.h"

#include "BaseModule.h"
#include "BaseDevice.h"


SystemProcessInfo::SystemProcessInfo(QObject *parent) :
    QObject(parent)
{
    qDebug() << "startup system debuger";
}

/*************************************************
  函数名称：printfMesssage(QString s)
  函数功能：s  需要打印的字符串
  输入参数：
  输出参数：
  返 回 值：
  备    注：
**************************************************/
void SystemProcessInfo::printfMesssage(QString s)
{
    qDebug() << s;
}

/*************************************************
  函数名称：printfWidgetInfo(InfoUI *w)
  函数功能：打印UI的信息
  输入参数：w  需要打印UI的INfoUI指针
  输出参数：
  返 回 值：
  备    注：
**************************************************/
void SystemProcessInfo::printfWidgetInfo(InfoUI *w)
{
    QString s = QString("Widget Name: %1, isActive: %2, user: ").arg(w->sName).arg(w->isActive);
    foreach(int i, w->nModulesUser)
    {
        s = s + ModulesName[i] + " ";
    }

    qDebug() << s << endl;
}

QString SystemProcessInfo::toString(InfoUI *w)
{
    QString s = QString("Widget Name: %1, isActive: %2, user: ").arg(w->sName).arg(w->isActive);
    foreach(int i, w->nModulesUser)
    {
        s = s + ModulesName[i] + " ";
    }

    return s;
}

/*************************************************
  函数名称：printfModule(BaseModule *m)
  函数功能：打印Module信息
  输入参数：m  需要打印的Module指针
  输出参数：
  返 回 值：
  备    注：
**************************************************/
void SystemProcessInfo::printfModule(BaseModule *m)
{
    QString s = QString("ID: %2, Module Name: %1, Device: %3").arg(m->getModuleInfo()->name).arg(m->getModuleInfo()->nID).arg(m->getDevice()->getDeviceName());

    qDebug() << s;
    qDebug() << "Module UI:";

    QList<InfoUI *>* uis = m->getUIs();

    for(int i = 0; i < uis->size(); i++)
    {
        s = QString("%1: %2").arg(i).arg(uis->at(i)->sName);
        qDebug() << s;
    }
}

QString SystemProcessInfo::toString(BaseModule *m)
{
    QString s = QString("ID: %2, Module Name: %1, Device: %3\n").arg(m->getModuleInfo()->name).arg(m->getModuleInfo()->nID).arg(m->getDevice()->getDeviceName());

    s += "Module UI:";

    QList<InfoUI *>* uis = m->getUIs();

    for(int i = 0; i < uis->size(); i++)
    {
        s += QString("%1: %2").arg(i).arg(uis->at(i)->sName);
    }
    return s;
}
