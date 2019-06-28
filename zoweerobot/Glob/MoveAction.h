#ifndef MOVEACTION_H
#define MOVEACTION_H

#include "BaseAction.h"
#include "GlobalVariableType.h"

/** @brief 运动指令类型 */
typedef  enum _MoveActionTypes
{
    MOVE_POINT,
    MOVE_LINE,
    MOVE_C_ARC,
    MOVE_CC_ARC
}MoveActionTypes;

class MoveAction : public BaseAction
{
public:
    MoveAction();

    void  addPointList(QVector<Point>&  pp);

    NAxis pointToPoint(ProcessInterVariableCache* cache, SystemParameter* para);

public:
    MoveActionTypes m_moveType;    /**< 运动类型 */
    Point  m_startPoint;    /**< 起点 */
    Point  m_endPoint;    /**< 终点 */
    Point  m_centerPoint;    /**< 圆弧中心点 */
    double m_dRadius;    /**< 半径 */
    double m_dFeedrate;    /**< 运动速度 */
};

#endif // MOVEACTION_H
