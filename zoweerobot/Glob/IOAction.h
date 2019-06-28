#ifndef IOACTION_H
#define IOACTION_H

#include "BaseAction.h"
typedef  enum _IoActionTypes
{
    IO_OUT,
    IO_IN,
    IO_NET
}IoActionTypes;

class IOAction : public BaseAction
{
public:
    IOAction();
    void  addPointList(QVector<Point>&  pp);
    IoActionTypes m_nIoType;
    int m_nIONum;
    bool m_bIoSts;

    int m_nCheckIONum;
    bool m_bCheckIoSts;
    int waitTime;
};

#endif // IOACTION_H
