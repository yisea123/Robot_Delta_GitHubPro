#include "circlemotioncode.h"

CircleMotionCode::CircleMotionCode()
{
    m_Type = MOTION_CIRCLE_TYPE;
}

QString CircleMotionCode::toDebugString()
{
    QString ss;
    ss = QString("CenterPoint: (%1, %2), Radius: %3\n").arg(m_CenterPoint.x).arg(m_CenterPoint.y).arg(m_dRadius);

    return BaseMotionCode::toDebugString() + ss;
}
