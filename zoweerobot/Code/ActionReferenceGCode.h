/**
Copyright (c)
@file ActionReferenceGCode.h
@author zhou
@version 1.0
@date 2018/07/10
@brief ActionReferenceGCode���͵Ķ��塣
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
    GCodeCoordinateReferenceType   m_CoordinateReference;     /**< ����ο��� */
    GCodePosition                  m_ReferencePosition;      /**< �ο�λ����Ϣ */
    bool    m_bIsAbsCoordinate;

 //   bool  m_isCoordinateReferenceAssign;    /**< ����ο����Ƿ񱻸�ֵ */
    bool  m_isReferencePositionIAssign;     /**< ����ο���I�Ƿ񱻸�ֵ */
    bool  m_isReferencePositionJAssign;     /**< ����ο���J�Ƿ񱻸�ֵ */
};

#endif // ACTIONREFERENCEGCODE_H
