#ifndef COORDINATEGCODE_H
#define COORDINATEGCODE_H

#include "BaseGCode.h"

class ActionReferenceGCode;



const QString  GCodeCoordinateString[] ={"GCODE_Coordinate_NULL",
                                      "GCODE_G92",
                                      "GCODE_G93",
                                      "GCODE_G98",
                                      "GCODE_G72"};

class CoordinateGcode : public BaseGCode
{
public:
    CoordinateGcode();
    ~CoordinateGcode();
    bool fillGCodeVarient(ParseGCodeInterVariableCache*  Cache, GCodeParseInfo& inf, ErrorInfo& e);
    bool genrateAction(EmulationInterVariableCache* Cache, QVector<BaseAction *>& actions,ErrorInfo& e);
    QString  toDebugString();
    QVector<Point>*  getCoordinateList();

    int getDP();
    int getDK();

public:
    GCodeCoordinateType      m_nCoordinateG;      /**< G�������� */
    Point                    m_WorkOriginPoint;    /**< �ӹ�ͼ��ԭ�����꣬��G72��� */

protected:

    Point                    m_MachineOriginPoint;  /**< ��еԭ�����꣬��G92��� */
    //Point                    m_WorkOriginPoint;    /**< �ӹ�ͼ��ԭ�����꣬��G72��� */
    Point                    m_OffestDistance;     /**< X��Y�������ƫ��ֵ����G93��� */
    QVector<Point>           m_pCoordinateList;   /**< ����ԭ���б���G98���� */
    int                      m_bIsCreateCoordList; /**< �Ƿ���G98�����˹���ԭ���б�=1��ʾ����������ô��б�ָ��������ϵ��=0�б���Ч */

    double                   m_dX;
    double                   m_dY;
    double                   m_dI;
    double                   m_dJ;
    int                      m_nP;
    int                      m_nK;

    ActionReferenceGCode*    m_pReferenceGCode;

private:
    void setWorkOriginPoint();
    void setOffestDistance();
    void setMachineOriginPoint();
    void setCoordinateList();
};

#endif // COORDINATEGCODE_H
