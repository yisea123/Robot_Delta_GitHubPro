#ifndef CHANGETOOLACTION_H
#define CHANGETOOLACTION_H

#include "BaseAction.h"
#include "MoveAction.h"

class ToolChangeAction : public MoveAction
{
public:
    ToolChangeAction();
    void  addPointList(QVector<Point>&  pp);

//    ToolInfo*  m_oldTool;
    ToolInfo*  m_newTool;
    int        m_nToolID;
    double     m_dToolChangeAngle;

};

#endif // CHANGETOOLACTION_H
