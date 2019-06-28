/************************************************************
  Copyright (C),
  �ļ�����SystemProcessInfo.cpp
  ���ߣ�zhou    �汾��V1.0    �������ڣ�2018/07/10
  ģ��������SystemProcessInfo���ʵ��
  ��ʷ�޸ļ�¼��<author>    <time>    <version >    <desc>
                  zhou    2018/07/10     V1.0      ���ע��
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
  �������ƣ�printfMesssage(QString s)
  �������ܣ�s  ��Ҫ��ӡ���ַ���
  ���������
  ���������
  �� �� ֵ��
  ��    ע��
**************************************************/
void SystemProcessInfo::printfMesssage(QString s)
{
    qDebug() << s;
}

/*************************************************
  �������ƣ�printfWidgetInfo(InfoUI *w)
  �������ܣ���ӡUI����Ϣ
  ���������w  ��Ҫ��ӡUI��INfoUIָ��
  ���������
  �� �� ֵ��
  ��    ע��
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
  �������ƣ�printfModule(BaseModule *m)
  �������ܣ���ӡModule��Ϣ
  ���������m  ��Ҫ��ӡ��Moduleָ��
  ���������
  �� �� ֵ��
  ��    ע��
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
