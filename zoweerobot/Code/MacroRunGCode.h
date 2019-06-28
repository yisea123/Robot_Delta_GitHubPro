#ifndef MACRORUNGCODE_H
#define MACRORUNGCODE_H

#include "BaseGCode.h"



const QString  GCodeMacroRunString[] ={"GCODE_MACRO_RUN_NULL",
                                  "GCODE_G75",
                                  "GCODE_G76"};


#define  MACRO_DIRECT_LEFT_BOTTOM       1
#define  MACRO_DIRECT_RIGHT_BOTTOM      2
#define  MACRO_DIRECT_LEFT_TOP          3
#define  MACRO_DIRECT_RIGHT_TOP         4

class MacroRunGCode : public BaseGCode
{
public:
    MacroRunGCode();
    bool fillGCodeVarient(ParseGCodeInterVariableCache*  Cache, GCodeParseInfo& inf, ErrorInfo& e);
    bool genrateAction(EmulationInterVariableCache* Cache, QVector<BaseAction *>& actions,ErrorInfo& e);
    QString  toDebugString();

protected:
    GCodeMacroType    m_nMacroG;     /**< G代码类型 */
    int      m_nW;    /**< 宏指令（子程序）的编号，用于索引需要执行的宏指令 */
    int      m_nQ;    /**< 指令开始执行的位置：Q1左下角、Q2右下角、Q3左上角、Q4右上角 */
//    runG75();/**< 宏调用并执行(X方向优先) */
//    runG76();/**< 宏调用并执行(Y方向优先) */
};

#endif // MACRORUNGCODE_H
