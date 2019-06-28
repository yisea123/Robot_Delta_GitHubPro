#ifndef LINEMOTIONCODE_H
#define LINEMOTIONCODE_H

#include "basemotioncode.h"

class LineMotionCode : public BaseMotionCode
{
public:
    LineMotionCode();
    virtual QString  toDebugString();
};

#endif // LINEMOTIONCODE_H
