#include "MoveAction.h"
#include <QDebug>

MoveAction::MoveAction()
{
    m_type = ACTION_MOVE_TYPE;

    m_moveType = MOVE_POINT;
}

void  MoveAction::addPointList(QVector<Point>&  pp)
{
    pp.push_back(m_startPoint);
    pp.push_back(m_endPoint);
}

NAxis MoveAction::pointToPoint(ProcessInterVariableCache* cache, SystemParameter* para)
{
    cache->m_oldPoint = cache->m_newPoint;
    cache->m_oldPosition = cache->m_newPosition;

    m_oldPoint = cache->m_oldPoint;
    m_oldPosition = cache->m_oldPosition;

    //m_newPoint = m_oldPoint;
    m_newPoint.x = m_endPoint.x;
    m_newPoint.y = m_endPoint.y;
    m_newPoint.z= m_endPoint.z;
    m_newPoint.w = m_endPoint.w;
    m_newPoint.p= m_endPoint.p;
    m_newPoint.r = m_endPoint.r;
    //m_newPoint.c = m_dAngle;
    //m_newPoint.t = m_oldPoint.t;
//    m_newPoint.v = m_oldPoint.v;

    m_newPosition = dAxisToNAxis(m_newPoint, para);

    NAxis deltaD = m_newPosition - m_oldPosition;

    cache->m_newPoint = m_newPoint;
    cache->m_newPosition = m_newPosition;

    return deltaD;
}
