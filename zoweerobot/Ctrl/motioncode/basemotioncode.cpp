#include "basemotioncode.h"

BaseMotionCode::BaseMotionCode(QObject *parent) : QObject(parent)
{
    m_Type = MOTION_MOVE_TYPE;
    m_StartPoint = RS_Vector(0.0, 0.0, 0.0);
    m_EndPoint = RS_Vector(0.0, 0.0, 0.0);
    m_dLength = 0.0;
    m_nCrossLevel = 0;

    m_nCodeId = 0;
    m_nEntityId = 0;
    m_nContourId = 0;
    m_sLayerName = "0";
}

QString BaseMotionCode::toDebugString()
{
    QString sType[8] = {"MOTION_MOVE_TYPE",
                        "MOTION_LINE_TYPE",
                        "MOTION_ARC_TYPE",
                        "MOTION_CIRCLE_TYPE",
                        "MOTION_IO_TYPE",
                        "MOTION_DELAY_TYPE",
                        "MOTION_LASER_CARFT_TYPE",
                        "MOTION_WAIT_TYPE"};
    QString ss;
    ss = QString("ID: %1, Type: %2, EntityId: %3, ContourId: %4, Layer: %5\n").arg(m_nCodeId).arg(sType[m_Type]).arg(m_nEntityId).arg(m_nContourId).arg(m_sLayerName);
    ss += QString("StartPoint: (%1, %2), EndPoint: (%3, %4), CrossLevel: %5\n").arg(m_StartPoint.x).arg(m_StartPoint.y).arg(m_EndPoint.x).arg(m_EndPoint.y).arg(m_nCrossLevel);

    return ss;
}
