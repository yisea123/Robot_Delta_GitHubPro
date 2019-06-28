#include "delaymotioncode.h"

DelayMotionCode::DelayMotionCode()
{
    m_Type = MOTION_DELAY_TYPE;
    m_nTime = 0;
}

QString DelayMotionCode::toDebugString()
{
    QString ss;
    ss = QString("DelayTime: %1\n").arg(m_nTime);

    return BaseMotionCode::toDebugString() + ss;
}
