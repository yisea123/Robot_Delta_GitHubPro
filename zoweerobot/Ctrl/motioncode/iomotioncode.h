#ifndef IOMOTIONCODE_H
#define IOMOTIONCODE_H

#include "basemotioncode.h"

class IoMotionCode : public BaseMotionCode
{
public:
    IoMotionCode();
    virtual QString  toDebugString();

    int  m_nName;
    bool m_bValue;
};

#endif // IOMOTIONCODE_H
