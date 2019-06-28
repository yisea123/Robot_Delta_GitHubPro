#include "MacroGCode.h"

MacroGCode::MacroGCode()
{
    m_GCodeType = GCODE_MACRO_TYPE;
}

MacroGCode::~MacroGCode()
{
    for(int i = 0; i < m_GCodeList.size(); i++)
    {
        delete m_GCodeList.at(i);
    }
    m_GCodeList.clear();
}

bool MacroGCode::fillGCodeVarient(ParseGCodeInterVariableCache*  Cache, GCodeParseInfo& inf, ErrorInfo& e)
{
    QString  s;

    for(int i = 0; i < inf.m_info.size(); i++)
    {
        s = inf.m_info.at(i).symbol;

        if(s == "U" || s == "u")
        {
            m_nMacroNum = inf.m_info.at(i).value.toInt();
        }
    }

    return BaseGCode::fillGCodeVarient(Cache, inf, e);
}

bool MacroGCode::genrateAction(EmulationInterVariableCache* Cache, QVector<BaseAction *>& actions,ErrorInfo& e)
{
    bool flag = false;
    for(int i = 0; i < Cache->m_MacroGCode.size(); i++)
    {
        if(m_nMacroNum == Cache->m_MacroGCode.at(i)->getMacroNum())
        {
            flag = true;
            break;
        }
    }

    if(flag)
    {
        for(int i = 0; i < m_GCodeList.size(); i++)
        {
            m_GCodeList.at(i)->genrateAction(Cache, actions, e);
        }
    }
    else
    {
        Cache->m_MacroGCode.push_back(this);
    }

    return true;
}

QString  MacroGCode::toDebugString()
{
    QString  ss;

    ss += "ºê´úÂë£º\n";
    ss += "×Ó³ÌÐò¶Î:";
    ss += QString::number(m_nMacroNum);
    ss += "\n";
    ss += "{ \n";

    for(int i = 0; i < m_GCodeList.size(); i++)
    {
        ss += "¶Î";
        ss += QString::number(i);
        ss += ":\n";
        ss += m_GCodeList.at(i)->toDebugString();
    }
    ss += "}";

    return BaseGCode::toDebugString() + ss;
}


