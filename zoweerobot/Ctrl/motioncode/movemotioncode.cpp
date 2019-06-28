#include "movemotioncode.h"

MoveMotionCode::MoveMotionCode()
{
    m_Type = MOTION_MOVE_TYPE;

    m_dMoveHeight = 10.0;
    m_dBurnHeight = 1.0;
}

QString MoveMotionCode::toDebugString()
{
    return BaseMotionCode::toDebugString();
}
