#ifndef BASEACTION_H
#define BASEACTION_H

#include <QVector>
#include "GlobalVariableType.h"

//#include "ComonTypes.h"



typedef enum _Actiontype
{
    ACTION_MOVE_TYPE,
    ACTION_PUNCH_TYPE,
    ACTION_TOOL_CHANGE_TYPE,
    ACTION_TOOL_ROTATE_TYPE,
    ACTION_MG_TYPE,
    ACTION_IO_TYPE,
    ACTION_DELAY_TYPE
}Actiontype;


class BaseAction : public QObject
{
public:
    BaseAction();
    virtual void  addPointList(QVector<Point>&  pp);

public:
    DAxis nAxisToDAxis(NAxis naxis, SystemParameter* para);
    NAxis dAxisToNAxis(DAxis naxis, SystemParameter* para);

public:

    Actiontype  m_type;

    Point  m_basePoint;
    ToolInfo*  m_tool;
    double m_dAngle;
    int m_toolSerialNum;
    int    m_nLineSerialNum;

    DAxis m_oldPoint;
    DAxis m_newPoint;
//    DAxis m_curPoint;
    NAxis m_oldPosition;
    NAxis m_newPosition;
    int m_delaytime;
	int m_hand;
//    NAxis m_curPosition;

    //bool  m_isPunchAction;
};

#endif // BASEACTION_H
