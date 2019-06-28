#ifndef ACTIONMGCODE_H
#define ACTIONMGCODE_H

#include "BaseGCode.h"

class ActionMGCode : public BaseGCode
{
public:
    ActionMGCode();
    bool fillGCodeVarient(ParseGCodeInterVariableCache*  Cache, GCodeParseInfo& inf, ErrorInfo& e);
    bool genrateAction(EmulationInterVariableCache* Cache, QVector<BaseAction *>& actions,ErrorInfo& e);
protected:
    bool mAction(EmulationInterVariableCache* Cache, QVector<BaseAction *>& actions, int mNum, QString s);
    bool ioAction(EmulationInterVariableCache* Cache, QVector<BaseAction *>& actions, int ioNum, bool sts);
    bool ioAction(EmulationInterVariableCache* Cache, QVector<BaseAction *>& actions, int ioNum, bool sts, int checkIONum);

    int   m_nM;
};

#endif // ACTIONMGCODE_H
