/**
Copyright (c)
@file ActionReferenceGCode.h
@author zhou
@version 1.0
@date 2018/07/10
@brief ActionReferenceGCode类型的定义。
*/

#ifndef ACTIONREFERENCEGCODE_H
#define ACTIONREFERENCEGCODE_H

#include "BaseGCode.h"

const QString  GCodeReferenceString[] ={"GCODE_REFERENCE_NULL",
                                      "GCODE_G90",
                                      "GCODE_G91"};

class ActionReferenceGCode : public BaseGCode
{
public:
    ActionReferenceGCode();
    bool fillGCodeVarient(ParseGCodeInterVariableCache*  Cache, GCodeParseInfo& inf, ErrorInfo& e);
    bool genrateAction(EmulationInterVariableCache* Cache, QVector<BaseAction *>& actions,ErrorInfo& e);
    QString  toDebugString();
    bool  isReferencePositionIAssign();
    bool  isReferencePositionJAssign();
    GCodePosition  getReferencePosition();
    double getReferencePositionX();
    double getReferencePositionY();
    bool  isAbsCoordinate();

public:


protected:
    GCodeCoordinateReferenceType   m_CoordinateReference;     /**< 坐标参考类 */
    GCodePosition                  m_ReferencePosition;      /**< 参考位置信息 */
    bool    m_bIsAbsCoordinate;

 //   bool  m_isCoordinateReferenceAssign;    /**< 坐标参考类是否被赋值 */
    bool  m_isReferencePositionIAssign;     /**< 坐标参考类I是否被赋值 */
    bool  m_isReferencePositionJAssign;     /**< 坐标参考类J是否被赋值 */
};

#endif // ACTIONREFERENCEGCODE_H
