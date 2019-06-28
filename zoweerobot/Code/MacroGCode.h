#ifndef MACROGCODE_H
#define MACROGCODE_H

#include "BaseGCode.h"
#include "GlobalVariableType.h"
#include <QList>


class MacroGCode : public BaseGCode
{
public:
    MacroGCode();
    ~MacroGCode();
    bool fillGCodeVarient(ParseGCodeInterVariableCache*  Cache, GCodeParseInfo& inf, ErrorInfo& e);
    bool genrateAction(EmulationInterVariableCache* Cache, QVector<BaseAction *>& actions,ErrorInfo& e);
    QString  toDebugString();
    QList<BaseGCode *>* getGCodeList()
    {
        return  &m_GCodeList;
    }

    int getMacroNum()
    {
        return m_nMacroNum;
    }

protected:
    int                 m_nMacroNum;    /**< ��¼��ָ��ӳ��򣩵ı�� */
    QList<BaseGCode *>  m_GCodeList;     /**< ����G������ŵ����ݽṹ */
};

#endif // MACROGCODE_H
