#ifndef MOVEACTION_H
#define MOVEACTION_H

#include "BaseAction.h"
#include "GlobalVariableType.h"

/** @brief �˶�ָ������ */
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
    MoveActionTypes m_moveType;    /**< �˶����� */
    Point  m_startPoint;    /**< ��� */
    Point  m_endPoint;    /**< �յ� */
    Point  m_centerPoint;    /**< Բ�����ĵ� */
    double m_dRadius;    /**< �뾶 */
    double m_dFeedrate;    /**< �˶��ٶ� */
};

#endif // MOVEACTION_H
