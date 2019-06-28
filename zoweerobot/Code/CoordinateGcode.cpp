#include "CoordinateGcode.h"
#include "ActionReferenceGCode.h"

CoordinateGcode::CoordinateGcode()
{
    m_GCodeType = GCODE_COORDINATE_TYPE;
    m_nCoordinateG = GCODE_Coordinate_NULL;
    m_pReferenceGCode = NULL;

    m_dX = -9999.0;
    m_dY = -9999.0;
    m_dI = -9999.0;
    m_dJ = -9999.0;
    m_nP = -1;
    m_nK = -1;
}

CoordinateGcode::~CoordinateGcode()
{
    if(m_pReferenceGCode != NULL)
    {
        delete m_pReferenceGCode;
    }
}

bool CoordinateGcode::fillGCodeVarient(ParseGCodeInterVariableCache*  Cache, GCodeParseInfo& inf, ErrorInfo& e)
{
    QString ss;
    for(int i = 0; i < inf.m_info.size(); i++)
    {
        ss = inf.m_info.at(i).symbol;
        if(ss == "G" || ss == "g")
        {
            switch(inf.m_info.at(i).value.toInt())
            {
                case 92:
                {
                    m_nCoordinateG = GCODE_G92;
                    inf.m_info.removeAt(i--);
                }break;
                case 72:
                {
                    m_nCoordinateG = GCODE_G72;
                    inf.m_info.removeAt(i--);
                }break;
                case 98:
                {
                    m_nCoordinateG = GCODE_G98;
                    inf.m_info.removeAt(i--);
                }break;
                case 93:
                {
                    m_nCoordinateG = GCODE_G93;
                    inf.m_info.removeAt(i--);
                }break;
                case 90:
                case 91:
                {
                    m_pReferenceGCode = new ActionReferenceGCode;
                    m_pReferenceGCode->fillGCodeVarient(Cache, inf, e);
                }break;
            }
        }
        else if(ss == "X" || ss == "x")
        {
            m_dX = inf.m_info.at(i).value.toDouble();
            inf.m_info.removeAt(i--);
        }
        else if(ss == "Y" || ss == "y")
        {
            m_dY = inf.m_info.at(i).value.toDouble();
            inf.m_info.removeAt(i--);
        }
        else if(ss == "I" || ss == "i")
        {
            m_dI = inf.m_info.at(i).value.toDouble();
            inf.m_info.removeAt(i--);
        }
        else if(ss == "J" || ss == "j")
        {
            m_dJ = inf.m_info.at(i).value.toDouble();
            inf.m_info.removeAt(i--);
        }
        else if(ss == "P" || ss == "p")
        {
            m_nP = inf.m_info.at(i).value.toInt();
            inf.m_info.removeAt(i--);
        }
        else if(ss == "K" || ss == "k")
        {
            m_nK = inf.m_info.at(i).value.toInt();
            inf.m_info.removeAt(i--);
        }
    }

    if(m_nCoordinateG == GCODE_G92)
    {
        if(m_dX > -1000 && m_dY > -1000)
        {
            setMachineOriginPoint();
        }
        else
        {
            e.insertDebug(DEBUG_NG_COMAND_NO_SUPPORT_ERROR, "CoordinateGcode::fillGCodeVarient G92 erroe");
            return FALSE;
        }
    }
    else if(m_nCoordinateG == GCODE_G93)
    {
        if(m_dX > -1000 && m_dY > -1000)
        {
            setOffestDistance();
        }
        else
        {
            e.insertDebug(DEBUG_NG_COMAND_NO_SUPPORT_ERROR, "CoordinateGcode::fillGCodeVarient G93 erroe");
            return FALSE;
        }
    }
    else if(m_nCoordinateG == GCODE_G72)
    {
        if(m_dX > -1000 && m_dY > -1000)
        {
            setWorkOriginPoint();
        }
        else
        {
            e.insertDebug(DEBUG_NG_COMAND_NO_SUPPORT_ERROR, "CoordinateGcode::fillGCodeVarient G72 erroe");
            return FALSE;
        }
    }
    else if(m_nCoordinateG == GCODE_G98)
    {
        if(m_dX > -1000 && m_dY > -1000/* && m_dI > 0 && m_dJ > 0 && m_nP > 0 && m_nK >0*/)
        {
            setCoordinateList();
        }
        else
        {
            e.insertDebug(DEBUG_NG_COMAND_NO_SUPPORT_ERROR, "CoordinateGcode::fillGCodeVarient G98 erroe");
            return FALSE;
        }
    }
    else
    {
        e.insertDebug(DEBUG_NG_COMAND_NO_SUPPORT_ERROR, "CoordinateGcode::fillGCodeVarient error");
        return FALSE;
    }

    return  BaseGCode::fillGCodeVarient(Cache, inf, e);
}

bool CoordinateGcode::genrateAction(EmulationInterVariableCache* Cache, QVector<BaseAction *>& actions,ErrorInfo& e)
{
    switch(m_nCoordinateG)
    {
        case GCODE_G98:
        {
            if(m_pCoordinateList.size() > 0)
            {
                Cache->m_WorkCoordinateList.clear();
                for(int i = 0; i < m_pCoordinateList.size(); i++)
                {
                    Cache->m_WorkCoordinateList.push_back(m_pCoordinateList.at(i));
                }
                Cache->m_nWorkCoordinateP = m_nP;//前面已经加1，此处不用再加了
                Cache->m_nWorkCoordinateK = m_nK;//前面已经加1，此处不用再加了
                Cache->m_WorkCoordinate = Cache->m_WorkCoordinateList.at(0);
            }

        }break;
        case GCODE_G72:
        {
            Cache->m_ReferenceCoordinate = m_WorkOriginPoint;
        }break;
        case GCODE_G92:
        {
            Cache->m_MachineCoordinate = m_MachineOriginPoint;
        }break;
        default:
        {
        }break;
    }

    return true;
}

void CoordinateGcode::setWorkOriginPoint()
{
    m_WorkOriginPoint.x = m_dX;
    m_WorkOriginPoint.y = m_dY;
}
void CoordinateGcode::setOffestDistance()
{
    m_OffestDistance.x = m_dX;
    m_OffestDistance.y = m_dY;
}
void CoordinateGcode::setMachineOriginPoint()
{
    m_MachineOriginPoint.x = -m_dX;
    m_MachineOriginPoint.y = -m_dY;
}
void CoordinateGcode::setCoordinateList()
{
    Point pp;
    if(m_nP < 0)
    {
        m_nP = 0;
    }
    if(m_nK < 0)
    {
        m_nK = 0;
    }
    if(m_dI < 0.0)
    {
        m_dI = 0.0;
    }
    if(m_dJ < 0.0)
    {
        m_dJ = 0.0;
    }

    m_nP++;
    m_nK++;

    for(int i = 0; i < m_nK; i++)
    {

        for(int j = 0; j < m_nP; j++)
        {
            pp.x = m_dX + i * m_dI;
            pp.y = m_dY + j * m_dJ;
            m_pCoordinateList.push_back(pp);
        }
    }
}

QVector<Point>*  CoordinateGcode::getCoordinateList()
{
    return &m_pCoordinateList;
}

int  CoordinateGcode::getDP()
{
    return m_nP;
}
int  CoordinateGcode::getDK()
{
    return m_nK;
}

QString  CoordinateGcode::toDebugString()
{
    QString  ss;

    ss += "坐标系代码：\n";
    ss += "坐标系类型：";
    ss += GCodeCoordinateString[m_nCoordinateG];

    ss += ",X: ";
    ss += ss.number(m_dX);
    ss += ",Y: ";
    ss += ss.number(m_dY);

    ss += ",I: ";
    if(m_dI > -1.0)
    {
       ss += ss.number(m_dI);
    }
    else
    {
        ss += "NULL";
    }
    ss += ",J: ";
    if(m_dJ > -1.0)
    {
       ss += ss.number(m_dJ);
    }
    else
    {
        ss += "NULL";
    }
    ss += ",P: ";
    if(m_nP >= 0)
    {
       ss += ss.number(m_nP);
    }
    else
    {
        ss += "NULL";
    }
    ss += ",K: ";
    if(m_nK >= 0)
    {
       ss += ss.number(m_nK);
    }
    else
    {
        ss += "NULL";
    }
    ss += "\n";

    if(m_nCoordinateG == GCODE_G92)
    {
        ss += "机械原点坐标：";
        ss += "X: ";
        ss += ss.number(m_MachineOriginPoint.x);
        ss += ",Y: ";
        ss += ss.number(m_MachineOriginPoint.y);
        ss += "\n";
    }
    else if(m_nCoordinateG == GCODE_G93)
    {
        ss += "坐标偏移值：";
        ss += ",X: ";
        ss += ss.number(m_OffestDistance.x);
        ss += ",Y: ";
        ss += ss.number(m_OffestDistance.y);

        if(m_pReferenceGCode == NULL)
        {
            ss += "没跟G90、G91";
        }
        else
        {
            ss += m_pReferenceGCode->toDebugString();
        }
        ss += "\n";
    }
    else if(m_nCoordinateG == GCODE_G72)
    {
        ss += "加工图形原点坐标：";
        ss += "X: ";
        ss += ss.number(m_WorkOriginPoint.x);
        ss += "Y: ";
        ss += ss.number(m_WorkOriginPoint.y);
        ss += "\n";
    }
    else if(m_nCoordinateG == GCODE_G98)
    {
        ss += "工件原点列表：";
        for(int i = 0; i < m_pCoordinateList.size(); i++)
        {
            ss += "(";
            ss += ss.number(m_pCoordinateList.at(i).x);
            ss += ",";
            ss += ss.number(m_pCoordinateList.at(i).y);
            ss += ") ";
        }
        ss += "\n";

    }
    return  BaseGCode::toDebugString() + ss;
}
