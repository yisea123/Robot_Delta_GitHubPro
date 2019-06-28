#ifndef PARAMETEGCODE_H
#define PARAMETEGCODE_H

#include "BaseGCode.h"

#define  MATERIAL_CARBON_STEEL_TYPE            0
#define  MATERIAL_STAINLESS_STEEL_TYPE         1
#define  MATERIAL_ALUMINUM_TYPE                2

class ParameteGcode : public BaseGCode
{
public:
    ParameteGcode();
    bool fillGCodeVarient(ParseGCodeInterVariableCache*  Cache, GCodeParseInfo& inf, ErrorInfo& e);

protected:
    int     m_nG;           /**< G´úÂëÀàÐÍ */
    double  m_dA;           /**< °å²Äºñ¶È£¬0.3~6.0mm */
    int     m_nB;           /**< °å²Ä£¬0£ºÌ¼¸Ö£»1£º²»Ðâ¸Ö£»2£ºÂÁ */
};

#endif // PARAMETEGCODE_H
