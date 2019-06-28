#ifndef BASEMOTIONCODE_H
#define BASEMOTIONCODE_H

#include <QObject>
//#include "rs_vector.h"

class RS_Vector {
public:
	RS_Vector()=default;
	RS_Vector(double vx, double vy, double vz)
	{
		x=vx;
		y=vy;
		z=vz;
	}

public:
	double x=0.;
	double y=0.;
	double z=0.;
	bool valid=false;
};

class BaseMotionCode : public QObject
{
    Q_OBJECT
public:
    typedef enum _MotionCodeType
    {
        MOTION_MOVE_TYPE,
        MOTION_LINE_TYPE,
        MOTION_ARC_TYPE,
        MOTION_CIRCLE_TYPE,
        MOTION_IO_TYPE,
        MOTION_DELAY_TYPE,
        MOTION_LASER_CARFT_TYPE,
        MOTION_WAIT_TYPE
    }MotionCodeType;

    explicit BaseMotionCode(QObject *parent = 0);
    virtual QString  toDebugString();

    MotionCodeType m_Type;
    RS_Vector m_StartPoint;
    RS_Vector m_EndPoint;
    double m_dLength;
    unsigned int m_nCrossLevel;

    unsigned int m_nCodeId;
    unsigned int m_nEntityId;
    unsigned int m_nContourId;
    QString m_sLayerName;

};

#endif // BASEMOTIONCODE_H
