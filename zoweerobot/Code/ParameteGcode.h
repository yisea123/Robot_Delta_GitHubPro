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
    int     m_nG;           /**< G�������� */
    double  m_dA;           /**< ��ĺ�ȣ�0.3~6.0mm */
    int     m_nB;           /**< ��ģ�0��̼�֣�1������֣�2���� */
};

#endif // PARAMETEGCODE_H
