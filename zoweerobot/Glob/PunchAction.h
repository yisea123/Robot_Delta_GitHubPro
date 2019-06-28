#ifndef PUNCHACTION_H
#define PUNCHACTION_H

//#include "BaseAction.h"
#include "MoveAction.h"

#define NOTPUNCH                0   //����ѹ
#define PUNCH_BACKTO_PREPUNCH   1   //��ѹ��ص�Ԥ��ѹλ��
#define PUNCH_BACKTO_ORIGNAL    2   //��ѹ���������/��ģλ
#define NOTPUNCH_BACKTO_ORIGNAL 3   //ֱ�Ӵӵ�ǰ��������/��ģλ

class PunchAction : public MoveAction
{
public:
    PunchAction();
    void  addPointList(QVector<Point>&  pp);
    int   m_nToolID;
    int   m_nPunchType;
};

#endif // PUNCHACTION_H
