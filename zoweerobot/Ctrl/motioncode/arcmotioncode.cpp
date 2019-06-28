#include "arcmotioncode.h"

ArcMotionCode::ArcMotionCode()
{
    m_Type = MOTION_ARC_TYPE;
    m_CenterPoint = RS_Vector(0.0, 0.0, 0.0);
    m_dRadius = 0.0;
    m_dStartAngle = 0.0;
    m_dEndAngle = 0.0;
    m_bIsReversed = false;//逆时针
}

QString ArcMotionCode::toDebugString()
{
    QString ss;
    ss = QString("CenterPoint: (%1, %2), Radius: %3, a1: %4, a2: %5\n").arg(m_CenterPoint.x).arg(m_CenterPoint.y).arg(m_dRadius).arg(m_dStartAngle).arg(m_dEndAngle);

    return BaseMotionCode::toDebugString() + ss;
}
