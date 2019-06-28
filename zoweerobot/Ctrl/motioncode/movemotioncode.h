#ifndef MOVEMOTIONCODE_H
#define MOVEMOTIONCODE_H

#include "basemotioncode.h"

class MoveMotionCode : public BaseMotionCode
{
public:
    MoveMotionCode();
    virtual QString  toDebugString();

    double m_dMoveHeight;
    double m_dBurnHeight;
};

#endif // MOVEMOTIONCODE_H
