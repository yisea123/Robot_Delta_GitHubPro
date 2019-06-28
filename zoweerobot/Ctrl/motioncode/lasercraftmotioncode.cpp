#include "lasercraftmotioncode.h"

LaserCraftMotionCode::LaserCraftMotionCode()
{
    m_Type = MOTION_LASER_CARFT_TYPE;
    m_LaserCraftMode = LCMC_TABLE;
    m_nTableNum = 0;
    m_nLaserPower = 0;
}

QString LaserCraftMotionCode::toDebugString()
{
    QString ss;
    ss = QString("Mod: %1, tableNum: %2, power: %3\n").arg(m_LaserCraftMode).arg(m_nTableNum).arg(m_nLaserPower);

    return BaseMotionCode::toDebugString() + ss;
}
