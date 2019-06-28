#include "waitiomotioncode.h"

WaitIoMotionCode::WaitIoMotionCode()
{
    m_Type = MOTION_WAIT_TYPE;
    m_nName = 0;
    m_bValue = false;
    m_nWaitTime = 0;
}

QString WaitIoMotionCode::toDebugString()
{
    QString ss;
    ss = QString("IO: %1, value: %2, waitTime：%3\n").arg(m_nName).arg(m_bValue).arg(m_nWaitTime);

    return BaseMotionCode::toDebugString() + ss;
}
