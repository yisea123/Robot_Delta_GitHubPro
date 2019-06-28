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
    GCodeCoordinateType      m_nCoordinateG;      /**< G代码类型 */
    Point                    m_WorkOriginPoint;    /**< 加工图形原点坐标，由G72填充 */

protected:

    Point                    m_MachineOriginPoint;  /**< 机械原点坐标，由G92填充 */
    //Point                    m_WorkOriginPoint;    /**< 加工图形原点坐标，由G72填充 */
    Point                    m_OffestDistance;     /**< X、Y轴的坐标偏移值，由G93填充 */
    QVector<Point>           m_pCoordinateList;   /**< 工件原点列表，由G98创建 */
    int                      m_bIsCreateCoordList; /**< 是否由G98创建了工件原点列表，=1表示后续代码采用此列表指定的坐标系，=0列表无效 */

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
