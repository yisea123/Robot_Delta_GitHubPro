#ifndef TOOLROTATEACTION_H
#define TOOLROTATEACTION_H

#include "BaseAction.h"
#include "MoveAction.h"

class ToolRotateAction : public MoveAction
{
public:
    ToolRotateAction();
    void  addPointList(QVector<Point>&  pp);

    //double  m_dRotateAngle;
    //int     m_nToolID;
};

#endif // TOOLROTATEACTION_H
