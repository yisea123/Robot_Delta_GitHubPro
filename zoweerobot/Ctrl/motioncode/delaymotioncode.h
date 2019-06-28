#ifndef DELAYMOTIONCODE_H
#define DELAYMOTIONCODE_H

#include "basemotioncode.h"

class DelayMotionCode : public BaseMotionCode
{
public:
    DelayMotionCode();
    virtual QString  toDebugString();

    int m_nTime;//ms
};

#endif // DELAYMOTIONCODE_H
