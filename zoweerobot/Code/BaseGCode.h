/**
Copyright (c)
@file BaseGCode.h
@author zhou
@version 1.0
@date 2018/07/10
@brief BaseGCode类型的定义。
*/

#ifndef BASEGCODE_H
#define BASEGCODE_H

#include "ComonTypes.h"
#include "GlobalVariableType.h"


class BaseGCode;
class BaseAction;

const QString  GCodeString[] ={"GCODE_REFERENCE_TYPE",
                                      "GCODE_MOVE_TYPE",
                                      "GCODE_PUNCH_TYPE",
                                      "GCODE_M_TYPE",
                                      "GCODE_COORDINATE_TYPE",
                                      "GCODE_PARAMETER_TYPE",
                                      "GCODE_MACRO_TYPE",
                                      "GCODE_MACRO_RUN_TYPE",
                                      "GCODE_CALL_TYPE",
                                      "GCODE_PCALL_TYPE",
                                      "GCODE_MCALL_TYPE",
                                      "GCODE_RPT_TYPE",
                                      "GCODE_IF_TYPE",
                                      "GCODE_GOTO_TYPE"};



class BaseGCode
{
public:
    BaseGCode();

    virtual bool fillGCodeVarient(ParseGCodeInterVariableCache*  Cache, GCodeParseInfo& inf, ErrorInfo& e);
    virtual bool genrateAction(EmulationInterVariableCache* Cache, QVector<BaseAction *>& actions,ErrorInfo& e);
    virtual QString  toDebugString();

    void setLineSerialNum(int i)
    {
        m_LineSerialNum = i;
    }
    void setNGSerialNum(int i)
    {
        m_NGSerialNum = i;
    }

    GCodeAllTypes    m_GCodeType;       /**< GCode类型 */

protected:
    int              m_LineSerialNum;   /**< 记录G代码的行号，例如 N0010 m_LineSerialNum=10 */
    int              m_NGSerialNum;


};

#endif // BASEGCODE_H
