#ifndef ARCMOTIONCODE_H
#define ARCMOTIONCODE_H

#include "basemotioncode.h"

class ArcMotionCode : public BaseMotionCode
{
public:
    ArcMotionCode();
    virtual QString  toDebugString();

    RS_Vector m_CenterPoint;
    double    m_dRadius;
    double    m_dStartAngle;
    double    m_dEndAngle;
    bool      m_bIsReversed;//true: 顺时针 ; false: 逆时针
};

#endif // ARCMOTIONCODE_H
