#include "ActionMGCode.h"
#include "MGAction.h"
#include "IOAction.h"


ActionMGCode::ActionMGCode()
{
    m_GCodeType = GCODE_M_TYPE;
    m_nM = -1;
}

bool ActionMGCode::fillGCodeVarient(ParseGCodeInterVariableCache*  Cache, GCodeParseInfo& inf, ErrorInfo& e)
{
    QString s;
    for(int i = 0; i < inf.m_info.size(); i++)
    {
        s = inf.m_info.at(i).symbol;
        if(s == "M" || s == "m")
        {
            m_nM = inf.m_info.at(i).value.toInt();
        }
    }

    return  BaseGCode::fillGCodeVarient(Cache, inf, e);
}

bool ActionMGCode::genrateAction(EmulationInterVariableCache* Cache, QVector<BaseAction *>& actions, ErrorInfo& e)
{
    switch (m_nM)
    {
    case 0:     //����ֹͣ
        mAction(Cache, actions, m_nM, "stop");
        break;
    case 1:     //ѡ��ֹͣ
        mAction(Cache, actions, m_nM, "pause");
        break;
    case 2:     //�������
        mAction(Cache, actions, m_nM, "stop");
        break;
    case 13:    //��ѹģʽ��λ
        break;
    case 30:    //�������
        mAction(Cache, actions, m_nM, "stop");
        break;
    case 70:    //��ǯ�ɿ�
        ioAction(Cache, actions, IO_JIAQIAN_PLC_M, false, IO_JIAQIAN_SONGJIN_SIGNAL_PLC_M);
        break;
    case 71:    //��ǯ�н�
        ioAction(Cache, actions, IO_JIAQIAN_PLC_M, true, IO_JIAQIAN_SONGJIN_SIGNAL_PLC_M);
        break;
    case 690:   //����ʹ��
        break;
    case 691:   //�����ر�
        break;
    case 692:   //��ͷ����
        break;
    case 698:   //����ģʽ��λ
        break;
    default:
        break;
    }

    return true;
}

bool ActionMGCode::mAction(EmulationInterVariableCache* Cache, QVector<BaseAction *>& actions, int mNum, QString s)
{
    MGAction* action = new MGAction;
    action->m_nMGCode = mNum;
    action->m_MCodeType = s;
    action->m_nLineSerialNum = m_LineSerialNum;

    actions.push_back(action);
    return true;
}

bool ActionMGCode::ioAction(EmulationInterVariableCache* Cache, QVector<BaseAction *>& actions, int ioNum, bool sts)
{
    IOAction *action = new IOAction;
    action->m_nIONum = ioNum;
    action->m_bIoSts = sts;
    action->m_nLineSerialNum = m_LineSerialNum;

    actions.push_back(action);
    return true;
}

bool ActionMGCode::ioAction(EmulationInterVariableCache* Cache, QVector<BaseAction *>& actions, int ioNum, bool sts, int checkIONum)
{
    IOAction *action = new IOAction;
    action->m_nIONum = ioNum;
    action->m_bIoSts = sts;
    action->m_nLineSerialNum = m_LineSerialNum;
    action->m_nCheckIONum = checkIONum;

    actions.push_back(action);
    return true;
}
