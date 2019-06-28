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
    GCodeMacroType    m_nMacroG;     /**< G�������� */
    int      m_nW;    /**< ��ָ��ӳ��򣩵ı�ţ�����������Ҫִ�еĺ�ָ�� */
    int      m_nQ;    /**< ָ�ʼִ�е�λ�ã�Q1���½ǡ�Q2���½ǡ�Q3���Ͻǡ�Q4���Ͻ� */
//    runG75();/**< ����ò�ִ��(X��������) */
//    runG76();/**< ����ò�ִ��(Y��������) */
};

#endif // MACRORUNGCODE_H
