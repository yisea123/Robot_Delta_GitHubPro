#include "iomotioncode.h"

IoMotionCode::IoMotionCode()
{
    m_Type = MOTION_IO_TYPE;
    m_nName = 0;
    m_bValue = false;
}

QString IoMotionCode::toDebugString()
{
    QString ss;
    ss = QString("IO: %1, value: %2\n").arg(m_nName).arg(m_bValue);

    return BaseMotionCode::toDebugString() + ss;
}
