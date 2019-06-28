#ifndef PUNCHACTION_H
#define PUNCHACTION_H

//#include "BaseAction.h"
#include "MoveAction.h"

#define NOTPUNCH                0   //不冲压
#define PUNCH_BACKTO_PREPUNCH   1   //冲压后回到预冲压位置
#define PUNCH_BACKTO_ORIGNAL    2   //冲压后回上死点/换模位
#define NOTPUNCH_BACKTO_ORIGNAL 3   //直接从当前回上死点/换模位

class PunchAction : public MoveAction
{
public:
    PunchAction();
    void  addPointList(QVector<Point>&  pp);
    int   m_nToolID;
    int   m_nPunchType;
};

#endif // PUNCHACTION_H
