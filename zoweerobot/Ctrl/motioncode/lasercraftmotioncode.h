#ifndef LASERCRAFTMOTIONCODE_H
#define LASERCRAFTMOTIONCODE_H

#include "basemotioncode.h"

class LaserCraftMotionCode : public BaseMotionCode
{
public:
    LaserCraftMotionCode();
    virtual QString  toDebugString();

    enum LaserCraftMode{LCMC_MAP_ON, LCMC_MAP_OFF, LCMC_TABLE, LCMC_LASER_ON, LCMC_LASER_OFF, LCMC_POWER_SET} m_LaserCraftMode;
    int  m_nTableNum;
    int  m_nLaserPower;
};

#endif // LASERCRAFTMOTIONCODE_H
