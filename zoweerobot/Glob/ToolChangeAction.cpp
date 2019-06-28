#include "ToolChangeAction.h"

ToolChangeAction::ToolChangeAction()
{
    m_type = ACTION_TOOL_CHANGE_TYPE;
    m_dAngle = 0.0;
    m_newTool = NULL;
    m_dToolChangeAngle = 0.0;
}

void  ToolChangeAction::addPointList(QVector<Point>&  pp)
{

}
