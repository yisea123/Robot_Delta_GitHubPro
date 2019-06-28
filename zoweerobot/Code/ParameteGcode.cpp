#include "ParameteGcode.h"
#include "BaseGCode.h"

ParameteGcode::ParameteGcode()
{
    m_GCodeType = GCODE_PARAMETER_TYPE;

}

bool ParameteGcode::fillGCodeVarient(ParseGCodeInterVariableCache*  Cache, GCodeParseInfo& inf, ErrorInfo& e)
{
    QString s;
    for(int i = 0; i < inf.m_info.size(); i++)
    {
        s = inf.m_info.at(i).symbol;
        if(s == "G" || s == "g")
        {
            m_nG = inf.m_info.at(i).value.toInt();
            inf.m_info.removeAt(i--);
        }
        else if(s == "A" || s == "a")
        {
            m_dA = inf.m_info.at(i).value.toDouble();
            inf.m_info.removeAt(i--);
        }
        else if(s == "B" || s == "b")
        {
            m_nB = inf.m_info.at(i).value.toInt();
            inf.m_info.removeAt(i--);
        }

    }

    return BaseGCode::fillGCodeVarient(Cache, inf, e);
}
