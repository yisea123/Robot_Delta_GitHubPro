#ifndef MGACTION_H
#define MGACTION_H

#include "BaseAction.h"

class MGAction : public BaseAction
{
public:
    MGAction();

    void  addPointList(QVector<Point>&  pp);

    int m_nMGCode;
    QString m_MCodeType;
};

#endif // MGACTION_H
