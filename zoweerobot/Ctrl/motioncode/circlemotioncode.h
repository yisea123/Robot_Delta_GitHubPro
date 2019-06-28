#ifndef CIRCLEMOTIONCODE_H
#define CIRCLEMOTIONCODE_H

#include "arcmotioncode.h"

class CircleMotionCode : public ArcMotionCode
{
public:
    CircleMotionCode();

    virtual QString  toDebugString();

};

#endif // CIRCLEMOTIONCODE_H
