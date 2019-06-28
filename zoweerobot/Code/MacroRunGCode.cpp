#include "MacroRunGCode.h"
#include "MacroGCode.h"

MacroRunGCode::MacroRunGCode()
{
    m_GCodeType = GCODE_MACRO_RUN_TYPE;
    m_nMacroG = GCODE_Macro_NULL;

    m_nW = -1;
}

bool MacroRunGCode::fillGCodeVarient(ParseGCodeInterVariableCache*  Cache, GCodeParseInfo& inf, ErrorInfo& e)
{
    QString ss;
    for(int i = 0; i < inf.m_info.size(); i++)
    {
        ss = inf.m_info.at(i).symbol;
        if(ss == "G" || ss == "g")
        {
            switch(inf.m_info.at(i).value.toInt())
            {
                case 75:
                {
                    m_nMacroG = GCODE_G75;
                    inf.m_info.removeAt(i--);
                }break;
                case 76:
                {
                    m_nMacroG = GCODE_G76;
                    inf.m_info.removeAt(i--);
                }break;
            }
        }
        if(ss == "W" || ss == "w")
        {
            m_nW = inf.m_info.at(i).value.toInt();
            inf.m_info.removeAt(i--);
        }
        if(ss == "Q" || ss == "q")
        {
            m_nQ = inf.m_info.at(i).value.toInt();
            inf.m_info.removeAt(i--);
        }
    }

    return  BaseGCode::fillGCodeVarient(Cache, inf, e);
}

bool MacroRunGCode::genrateAction(EmulationInterVariableCache* Cache, QVector<BaseAction *>& actions,ErrorInfo& e)
{
    if(m_nW > Cache->m_MacroGCode.size())
    {
        e.insertDebug(DEBUG_SERIOUS_ERROR, "MacroRunGCode::genrateAction m_nW overflow!");
        return DONE_FAIL;
    }
    switch(m_nMacroG)
    {
    case GCODE_G75:
        {
            if(m_nQ == MACRO_DIRECT_LEFT_BOTTOM)
            {
                for(int i = 0; i < Cache->m_nWorkCoordinateK; i++)
                {
                    for(int j = 0; j < Cache->m_nWorkCoordinateP; j++)
                    {
                        if(Cache->m_WorkCoordinateList.size() > 0)
                        {
                            if (i%2 == 0)
                            {
                                Cache->m_WorkCoordinate = Cache->m_WorkCoordinateList[i + j * Cache->m_nWorkCoordinateP];
                            }
                            else
                            {
                                Cache->m_WorkCoordinate = Cache->m_WorkCoordinateList[i + (Cache->m_nWorkCoordinateP -1 - j) * Cache->m_nWorkCoordinateP];
                            }

                            if(m_nW <= Cache->m_MacroGCode.size())
                            {
                                Cache->m_MacroGCode.at(m_nW - 1)->genrateAction(Cache, actions, e);
                            }
                            else
                            {
                                return DONE_FAIL;
                            }

                        }
                        else
                        {
                            return DONE_FAIL;
                        }
                    }
                }
            }
            else if(m_nQ == MACRO_DIRECT_RIGHT_BOTTOM)
            {
                for(int i = 0; i < Cache->m_nWorkCoordinateK; i++)
                {
                    for(int j = 0; j < Cache->m_nWorkCoordinateP; j++)
                    {
                        if(Cache->m_WorkCoordinateList.size() > 0)
                        {
                            if (i%2 == 0)
                            {
                                Cache->m_WorkCoordinate = Cache->m_WorkCoordinateList[i + (Cache->m_nWorkCoordinateP -1 - j) * Cache->m_nWorkCoordinateP];
                            }
                            else
                            {
                                Cache->m_WorkCoordinate = Cache->m_WorkCoordinateList[i + j * Cache->m_nWorkCoordinateP];
                            }

                            if(m_nW <= Cache->m_MacroGCode.size())
                            {
                                Cache->m_MacroGCode.at(m_nW - 1)->genrateAction(Cache, actions, e);
                            }
                            else
                            {
                                return DONE_FAIL;
                            }

                        }
                        else
                        {
                            return DONE_FAIL;
                        }
                    }
                }
            }
            else if(m_nQ == MACRO_DIRECT_LEFT_TOP)
            {
                for(int i = (Cache->m_nWorkCoordinateK - 1); i >= 0; i--)
                {
                    for(int j = 0; j < Cache->m_nWorkCoordinateP; j++)
                    {
                        if(Cache->m_WorkCoordinateList.size() > 0)
                        {
                            if (i%2 == 0)
                            {
                                Cache->m_WorkCoordinate = Cache->m_WorkCoordinateList[i + j * Cache->m_nWorkCoordinateP];
                            }
                            else
                            {
                                Cache->m_WorkCoordinate = Cache->m_WorkCoordinateList[i + (Cache->m_nWorkCoordinateP -1 - j) * Cache->m_nWorkCoordinateP];
                            }

                            if(m_nW <= Cache->m_MacroGCode.size())
                            {
                                Cache->m_MacroGCode.at(m_nW - 1)->genrateAction(Cache, actions, e);
                            }
                            else
                            {
                                return DONE_FAIL;
                            }

                        }
                        else
                        {
                            return DONE_FAIL;
                        }
                    }
                }
            }
            else if(m_nQ == MACRO_DIRECT_RIGHT_TOP)
            {
                for(int i = (Cache->m_nWorkCoordinateK - 1); i >= 0; i--)
                {
                    for(int j = 0; j < Cache->m_nWorkCoordinateP; j++)
                    {
                        if(Cache->m_WorkCoordinateList.size() > 0)
                        {
                            if (i%2 == 0)
                            {
                                Cache->m_WorkCoordinate = Cache->m_WorkCoordinateList[i + (Cache->m_nWorkCoordinateP -1 - j) * Cache->m_nWorkCoordinateP];
                            }
                            else
                            {
                                Cache->m_WorkCoordinate = Cache->m_WorkCoordinateList[i + j * Cache->m_nWorkCoordinateP];
                            }

                            if(m_nW <= Cache->m_MacroGCode.size())
                            {
                                Cache->m_MacroGCode.at(m_nW - 1)->genrateAction(Cache, actions, e);
                            }
                            else
                            {
                                return DONE_FAIL;
                            }

                        }
                        else
                        {
                            return DONE_FAIL;
                        }
                    }
                }
            }
        }break;
    case GCODE_G76:
        {
        if(m_nQ == MACRO_DIRECT_LEFT_BOTTOM)
        {
            for(int i = 0; i < Cache->m_nWorkCoordinateP; i++)
            {
                for(int j = 0; j < Cache->m_nWorkCoordinateK; j++)
                {
                    if(Cache->m_WorkCoordinateList.size() > 0)
                    {
                        if (i%2 == 0)
                        {
                            Cache->m_WorkCoordinate = Cache->m_WorkCoordinateList[i * Cache->m_nWorkCoordinateP + j];
                        }
                        else
                        {
                            Cache->m_WorkCoordinate = Cache->m_WorkCoordinateList[i * Cache->m_nWorkCoordinateP + (Cache->m_nWorkCoordinateK -1 - j)];
                        }

                        if(m_nW <= Cache->m_MacroGCode.size())
                        {
                            Cache->m_MacroGCode.at(m_nW - 1)->genrateAction(Cache, actions, e);
                        }
                        else
                        {
                            return DONE_FAIL;
                        }

                    }
                    else
                    {
                        return DONE_FAIL;
                    }
                }
            }
        }
        else if(m_nQ == MACRO_DIRECT_RIGHT_BOTTOM)
        {
            for(int i = 0; i < Cache->m_nWorkCoordinateP; i++)
            {
                for(int j = 0; j < Cache->m_nWorkCoordinateK; j++)
                {
                    if(Cache->m_WorkCoordinateList.size() > 0)
                    {
                        if (i%2 == 0)
                        {
                            Cache->m_WorkCoordinate = Cache->m_WorkCoordinateList[i * Cache->m_nWorkCoordinateP + j];
                        }
                        else
                        {
                            Cache->m_WorkCoordinate = Cache->m_WorkCoordinateList[i * Cache->m_nWorkCoordinateP + (Cache->m_nWorkCoordinateK -1 - j)];
                        }

                        if(m_nW <= Cache->m_MacroGCode.size())
                        {
                            Cache->m_MacroGCode.at(m_nW - 1)->genrateAction(Cache, actions, e);
                        }
                        else
                        {
                            return DONE_FAIL;
                        }

                    }
                    else
                    {
                        return DONE_FAIL;
                    }
                }
            }
        }
        else if(m_nQ == MACRO_DIRECT_LEFT_TOP)
        {
            for(int i = (Cache->m_nWorkCoordinateP - 1); i >= 0; i--)
            {
                for(int j = 0; j < Cache->m_nWorkCoordinateK; j++)
                {
                    if(Cache->m_WorkCoordinateList.size() > 0)
                    {
                        if (i%2 == 0)
                        {
                            Cache->m_WorkCoordinate = Cache->m_WorkCoordinateList[i * Cache->m_nWorkCoordinateP + (Cache->m_nWorkCoordinateK -1 - j)];
                        }
                        else
                        {
                            Cache->m_WorkCoordinate = Cache->m_WorkCoordinateList[i * Cache->m_nWorkCoordinateP + j];
                        }

                        if(m_nW <= Cache->m_MacroGCode.size())
                        {
                            Cache->m_MacroGCode.at(m_nW - 1)->genrateAction(Cache, actions, e);
                        }
                        else
                        {
                            return DONE_FAIL;
                        }

                    }
                    else
                    {
                        return DONE_FAIL;
                    }
                }
            }
        }
        else if(m_nQ == MACRO_DIRECT_RIGHT_TOP)
        {
            for(int i = (Cache->m_nWorkCoordinateP - 1); i >= 0; i--)
            {
                for(int j = 0; j < Cache->m_nWorkCoordinateK; j++)
                {
                    if(Cache->m_WorkCoordinateList.size() > 0)
                    {
                        if (i%2 == 0)
                        {
                            Cache->m_WorkCoordinate = Cache->m_WorkCoordinateList[i * Cache->m_nWorkCoordinateP + (Cache->m_nWorkCoordinateK -1 - j)];
                        }
                        else
                        {
                            Cache->m_WorkCoordinate = Cache->m_WorkCoordinateList[i * Cache->m_nWorkCoordinateP + j];
                        }

                        if(m_nW <= Cache->m_MacroGCode.size())
                        {
                            Cache->m_MacroGCode.at(m_nW - 1)->genrateAction(Cache, actions, e);
                        }
                        else
                        {
                            return DONE_FAIL;
                        }

                    }
                    else
                    {
                        return DONE_FAIL;
                    }
                }
            }
        }
        }break;
    }
    return  DONE_SUCCESS;
}

QString  MacroRunGCode::toDebugString()
{
    QString  ss;

    ss += "宏运动：\n";
    ss += "宏运动类型：";
    ss += GCodeMacroRunString[m_nMacroG];
    ss += "，";
    ss += "W：";
    ss += ss.number(m_nW);
    ss += "，";
    ss += "Q：";
    ss += ss.number(m_nQ);
    ss += "\n";

    return BaseGCode::toDebugString() + ss;
}



