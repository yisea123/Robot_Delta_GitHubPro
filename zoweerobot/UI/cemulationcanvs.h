#ifndef CEMULATIONCANVS_H
#define CEMULATIONCANVS_H

#include <QWidget>
#include <QColor>
#include <QPainter>
#include <QMutex>
#include  "GlobalVariableType.h"

#define  SIGNAL_TRIGGER_TIME    300

class CEmulationCanvs : public QWidget
{
    Q_OBJECT
public:
    enum  viewStatus
    {
        STATUS_NONE,
        STATUS_DELAY_MOVE,
        STATUS_NO_DELAY_MOVE,
        STATUS_MOVE,
        STATUS_ZOOM,
        STATUS_ZOOM_OUT,
        STATUS_ZOOM_IN,
        STATUS_ZOOM_REGION,
        STATUS_RESTORE,
        STATUS_REGION_DRAW
    };

    explicit CEmulationCanvs(QWidget *parent = 0);
    void  enableDrawFunction();
    void  disableDrawFunction();
    void  clearGraphicals();
    void  clearCarrierGraphical();
    void  updataCarrierInfo(GraphicalInfo* info1, GraphicalInfo* info2, GraphicalInfo* info3, Point p, QString s);
    void  updataCarrierInfo(Point p, double r, double len, Point pt, QString s);
    void  updataCarrierInfo(Point p, double r, double len);
    int   getGraphicalsSize();
    void  addGraphicals(GraphicalInfo *Graphical);
    void  setSizeXY(double x, double y, double x0, double y0);
    void  init();
    CarrierGraphicalInfo*   getCarrierGraphicalsHandle();
    void setCanvasStatus(viewStatus vv, int arg1 = -1, int arg2 = -1);
    void setOrignalPoint(double x, double y);

    QMutex mutex;
private:


protected:
    void paintEvent(QPaintEvent *event);
    void resize(const QSize &);
    void mousePressEvent(QMouseEvent * e);//鼠标单击事件响应
    void mouseMoveEvent(QMouseEvent * e);
    void mouseReleaseEvent(QMouseEvent * e);
    void wheelEvent(QWheelEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

//    void mouseDoubleClickEvent( QMouseEvent * e );


private:
    void setRadio(double x, double y);
    void  changeGraphicalToCanvs();
    QPoint coordinateVChange(double x, double y);
    QPoint coordinateVChange(Point p);
    void  drawLine(Point p1, Point p2, Qt::PenStyle s = Qt::SolidLine, QColor c = Qt::red);
    void  drawRegion(QPoint pf, QPoint ps, QColor c);
    void  drawGraphical(GraphicalInfo *Graphical);
    void  drawCarrierGraphical();
signals:
    
public slots:
    void animateTimeout();
    void UpdataCanvas();

private:
    viewStatus  m_eStatus;
    int m_nLineWidth;
    int m_nCanvsHeight;
    int m_nCanvsWidth;
    double m_dSizeX;
    double m_dSizeY;
    double m_dCenterX;
    double m_dCenterY;
    double m_dRadio;
    int  m_OriginX;
    int  m_OriginY;
    int  m_MoveX;
    int  m_MoveY;
    float m_dZoomRadio;
    bool m_bDrawEnable;
    bool m_bisKeepStatus;
    int  m_nProcessValue;
    double m_dCursorRadio;

    QPoint  m_ZoomRegionFirstPoint;
    QPoint  m_ZoomRegionOldPoint;

    QVector<GraphicalInfo *>  m_Graphicals;
    CarrierGraphicalInfo*    m_CarrierGraphicals;



};

#endif // CEMULATIONCANVS_H
