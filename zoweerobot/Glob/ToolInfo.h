#ifndef TOOLINFO_H
#define TOOLINFO_H

#include "GlobalVariableType.h"
#include <QVector>

class Point;

#define   TOOL_TYPE_NUM                10

#define   TOOL_TYPE_SQUARE_SHAPE       0
#define   TOOL_TYPE_RECTANGULAR_SHAPE  1
#define   TOOL_TYPE_ROUND_SHAPE        2
#define   TOOL_TYPE_OB_SHAPE        3
#define   TOOL_TYPE_WD_SHAPE        4
#define   TOOL_TYPE_SD_SHAPE        5
#define   TOOL_TYPE_TR_SHAPE        6
#define   TOOL_TYPE_MJ_SHAPE        7
#define   TOOL_TYPE_RR_SHAPE        8
#define   TOOL_TYPE_CR_SHAPE        9

const  QString  ToolNames[] = {"SQ", "RE", "RO", "OB", "WD", "SD", "TR", "MJ", "RR", "CR"};

class ToolInfo
{
public:
    ToolInfo();
    ToolInfo(QString tool);
    void setFrameNum(int i);
    void setSerialNum(int i);
    int  getSerialNum();
    void settoolXY(double x, double y);
    void setTrC(double c);
    void setMJD(double d);
    void setCRA(double a);
    void setCRB(double b);
    void setCRR(double r);
    void setRadius(double r);
    void setAngle(double a);
    void setName(QString s);
    void setToolType(QString s);
    void setRotaryFlag(bool b);
    bool isRotaryTool();   /**< 刀具是否可旋转 */

    double getToolAngle();   /**< 获取刀具角度 */

    bool breakupGraph();
    bool breakupGraph(QVector<Point>& ps, Point p, double angle = 0);

    void setPosAngle(double t);
    double getPosAngle();

private:
    bool  breakupSquareGraph();     //SQ
    bool  breakupRectangularGraph();//RE
    bool  breakupRoundGraph();      //RO
    bool  breakupOBGraph();         //OB
    bool  breakupWDGraph();         //WD
    bool  breakupSDGraph();         //SD
    bool  breakupTRGraph();         //TR
    bool  breakupMJGraph();         //MJ
    bool  breakupRRGraph();         //RR
    bool  breakupCRGraph();         //CR

    bool  breakupSquareGraph(QVector<Point>& ps, Point p, double angle = 0);
    bool  breakupRectangularGraph(QVector<Point>& ps, Point p, double angle = 0);
    bool  breakupRoundGraph(QVector<Point>& ps, Point p, double angle = 0);
    bool  breakupOBGraph(QVector<Point>& ps, Point p, double angle = 0);
    bool  breakupWDGraph(QVector<Point>& ps, Point p, double angle = 0);
    bool  breakupSDGraph(QVector<Point>& ps, Point p, double angle = 0);
    bool  breakupTRGraph(QVector<Point>& ps, Point p, double angle = 0);
    bool  breakupMJGraph(QVector<Point>& ps, Point p, double angle = 0);
    bool  breakupRRGraph(QVector<Point>& ps, Point p, double angle = 0);
    bool  breakupCRGraph(QVector<Point>& ps, Point p, double angle = 0);

    bool  getDrawPointList(QVector<Point>& ps, Point p, double ang);


public:
    QString  m_name;
    int      m_toolSerialNum;

private:

    int      m_nFrameNum;
    int      m_toolType;
    //int      m_toolSerialNum;
    double   m_toolX;
    double   m_toolY;
    double   m_radius;
    double   m_angle;
    double   m_trC;
    double   m_MJd;
    double   m_CRa;
    double   m_CRb;
    double   m_CRr;

    bool     m_bIsRotary;

    QVector<Point>  m_PointList;
    double  m_dPosAngle;
};

#endif // TOOLINFO_H
