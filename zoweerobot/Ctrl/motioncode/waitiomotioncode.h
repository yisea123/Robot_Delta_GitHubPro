#ifndef WAITIOMOTIONCODE_H
#define WAITIOMOTIONCODE_H

#include "basemotioncode.h"

class WaitIoMotionCode : public BaseMotionCode
{
public:
    WaitIoMotionCode();
    virtual QString  toDebugString();

    int  m_nName;
    bool m_bValue;
    int  m_nWaitTime;
};

#endif // WAITIOMOTIONCODE_H
