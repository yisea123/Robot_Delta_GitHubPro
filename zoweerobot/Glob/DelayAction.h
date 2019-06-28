#ifndef DELAYACTION_H
#define DELAYACTION_H

#include "BaseAction.h"

class DelayAction : public BaseAction
{
public:
    DelayAction();

    void  addPointList(QVector<Point>&  pp);

    int  m_nDelayTime;//ms
};

#endif // DELAYACTION_H
