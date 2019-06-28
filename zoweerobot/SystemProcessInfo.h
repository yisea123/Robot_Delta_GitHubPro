/**
Copyright (c)
@file SystemProcessInfo.h
@author zhou
@version 1.0
@date 2018/07/10
@brief SystemProcessInfo类型的定义。
*/

#ifndef SYSTEMPROCESSINFO_H
#define SYSTEMPROCESSINFO_H

#include <QObject>
#include "ComonTypes.h"

class BaseModule;


class SystemProcessInfo : public QObject
{
    Q_OBJECT
public:
    explicit SystemProcessInfo(QObject *parent = 0);

    static void printfMesssage(QString s);

    void printfWidgetInfo(InfoUI *w);
    void printfModule(BaseModule *m);
    QString toString(BaseModule *m);
    QString toString(InfoUI *w);
    
signals:

public slots:
    
};


#endif // SYSTEMPROCESSINFO_H
