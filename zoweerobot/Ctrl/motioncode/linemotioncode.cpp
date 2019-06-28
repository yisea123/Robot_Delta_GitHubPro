#include "linemotioncode.h"

LineMotionCode::LineMotionCode()
{
    m_Type = MOTION_LINE_TYPE;
}

QString LineMotionCode::toDebugString()
{
    return BaseMotionCode::toDebugString();
}
