#include "cemulationcanvs.h"
#include <QCursor>
#include <QTimer>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QDebug>
#include <QTimer>

#include "EmulationControllerDevice.h"



CEmulationCanvs::CEmulationCanvs(QWidget *parent) :
    QWidget(parent)
{
    m_nLineWidth = 2;
    m_nCanvsWidth = -1;
    m_nCanvsHeight = -1;
    m_MoveX = 0;
    m_MoveY = 0;
    m_dZoomRadio = 1.0;
    m_nProcessValue = 0;
    m_dCursorRadio = 5.0;

    m_bDrawEnable = false;
    m_bisKeepStatus = false;
    m_eStatus = STATUS_NONE;
    m_CarrierGraphicals = new CarrierGraphicalInfo;

    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(UpdataCanvas()));
    timer->start(400);

}

void  CEmulationCanvs::enableDrawFunction()
{
    m_bDrawEnable = true;
}

void  CEmulationCanvs::disableDrawFunction()
{
    m_bDrawEnable = false;
}

void CEmulationCanvs::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.fillRect(0, 0, rect().width(), rect().height(), Qt::black);

    if(m_bDrawEnable)
    {
        if((rect().width() != m_nCanvsWidth) || (rect().height() != m_nCanvsHeight))
        {
            setRadio(m_dSizeX,m_dSizeY);
            setOrignalPoint(m_dCenterX, m_dCenterY);
          //  changeGraphicalToCanvs();
        }


        for(int i = 0; i < m_Graphicals.size(); i++)
        {
            drawGraphical(m_Graphicals[i]);
        }

        drawCarrierGraphical();
 //       disableDrawFunction();

        if(m_eStatus == STATUS_REGION_DRAW)
        {
            drawRegion(m_ZoomRegionFirstPoint, m_ZoomRegionOldPoint, Qt::green);
        }
    }

}

void CEmulationCanvs::mousePressEvent(QMouseEvent * e)//鼠标单击事件响应
{
    if(e->button() == Qt::LeftButton)
    {
        m_MoveX = e->pos().x();
        m_MoveY = e->pos().y();
        if(m_eStatus == STATUS_NO_DELAY_MOVE)
        {
            m_eStatus = STATUS_MOVE;
        }
        else if(m_eStatus == STATUS_NONE)
        {
            QTimer::singleShot(SIGNAL_TRIGGER_TIME, this, SLOT(animateTimeout()) );
            m_eStatus = STATUS_DELAY_MOVE;
        }
        else if(m_eStatus == STATUS_ZOOM_REGION)
        {           
            m_ZoomRegionFirstPoint = e->pos();
            m_ZoomRegionOldPoint = m_ZoomRegionFirstPoint;
            m_eStatus = STATUS_REGION_DRAW;
           // drawRegion(m_ZoomRegionFirstPoint, m_ZoomRegionOldPoint, Qt::green);
        }

    }
}
void CEmulationCanvs::animateTimeout()
{
    if(STATUS_DELAY_MOVE == m_eStatus)
    {
        QCursor Cursor(Qt::SizeAllCursor);
        setCursor(Cursor);
        m_eStatus = STATUS_MOVE;
    }
}

void CEmulationCanvs::UpdataCanvas()
{
    mutex.lock();
    update();
    mutex.unlock();

}

void CEmulationCanvs::mouseMoveEvent(QMouseEvent * e)
{
    if(STATUS_MOVE == m_eStatus)
    {
        m_OriginX += e->pos().x() - m_MoveX;
        m_OriginY += e->pos().y() - m_MoveY;

        m_MoveX = e->pos().x();
        m_MoveY = e->pos().y();

        update();
    }
    else if(m_eStatus == STATUS_REGION_DRAW)
    {
        m_MoveX = e->pos().x();
        m_MoveY = e->pos().y();
        QPoint pn(m_MoveX, m_MoveY);
//        drawRegion(m_ZoomRegionFirstPoint, m_ZoomRegionOldPoint, Qt::green);
//        drawRegion(m_ZoomRegionFirstPoint, pn, Qt::green);
        m_ZoomRegionOldPoint = pn;

        update();
    }
}
void CEmulationCanvs::mouseReleaseEvent(QMouseEvent * e)
{
    if(m_bisKeepStatus)
    {
        if(m_eStatus == STATUS_MOVE)
        {
            m_eStatus = STATUS_NO_DELAY_MOVE;
        }
        else if(m_eStatus == STATUS_REGION_DRAW)
        {
            m_eStatus = STATUS_ZOOM_REGION;
            m_ZoomRegionOldPoint = e->pos();

            QPoint pp = (m_ZoomRegionOldPoint + m_ZoomRegionFirstPoint) * 0.5;
            int size_X = abs(m_ZoomRegionFirstPoint.x() - m_ZoomRegionOldPoint.x());
            int size_Y = abs(m_ZoomRegionFirstPoint.y() - m_ZoomRegionOldPoint.y());

            if((size_X > 0) && (size_Y > 0))
            {
                double dd1 = ((double)m_nCanvsHeight / (double)size_Y);
                double dd2 = ((double)m_nCanvsWidth / (double)size_X);
                double dd = (dd1 > dd2) ? dd2 : dd1;
                m_dZoomRadio *= dd;
                m_OriginX = (int)(((double)(m_OriginX - pp.x())) * dd + pp.x());
                m_OriginY = (int)(((double)(m_OriginY - pp.y())) * dd + pp.y());
            }
        }
    }
    else
    {
        m_eStatus = STATUS_NONE;
        QCursor Cursor(Qt::ArrowCursor);
        setCursor(Cursor);
    }
    update();
}

void CEmulationCanvs::wheelEvent(QWheelEvent *event)
{
    if(m_bisKeepStatus)
    {
        QPoint pp = event->pos();
        if(m_eStatus == STATUS_ZOOM_OUT)
        {
            m_dZoomRadio *= 1.01;
            m_OriginX = (int)(((double)(m_OriginX - pp.x())) * 1.01 + pp.x());
            m_OriginY = (int)(((double)(m_OriginY - pp.y())) * 1.01 + pp.y());
        }
        else if(m_eStatus == STATUS_ZOOM_IN)
        {
            m_dZoomRadio /= 1.01;
            m_OriginX = (int)(((double)(m_OriginX - pp.x())) / 1.01 + pp.x());
            m_OriginY = (int)(((double)(m_OriginY - pp.y())) / 1.01 + pp.y());
        }
        update();
    }
    else
    {
        if(Qt::ControlModifier == event->modifiers())
        {
            m_eStatus = STATUS_ZOOM;

            QPoint pp = event->pos();
            if(event->delta() > 0)
            {
                QCursor Cursor(Qt::OpenHandCursor);
                setCursor(Cursor);
                m_dZoomRadio *= 1.01;
                m_OriginX = (int)(((double)(m_OriginX - pp.x())) * 1.01 + pp.x());
                m_OriginY = (int)(((double)(m_OriginY - pp.y())) * 1.01 + pp.y());
            }
            else if(event->delta() < 0)
            {
                QCursor Cursor(Qt::ClosedHandCursor);
                setCursor(Cursor);
                m_dZoomRadio /= 1.01;
                m_OriginX = (int)(((double)(m_OriginX - pp.x())) / 1.01 + pp.x());
                m_OriginY = (int)(((double)(m_OriginY - pp.y())) / 1.01 + pp.y());
            }

            update();
        }
    }

}

void CEmulationCanvs::keyReleaseEvent(QKeyEvent *event)
{
    if(Qt::ControlModifier == event->key())
    {
        if(STATUS_ZOOM == m_eStatus)
        {
            m_eStatus = STATUS_NONE;
            QCursor Cursor(Qt::ArrowCursor);
            setCursor(Cursor);
            update();
        }
    }
}

void CEmulationCanvs::resize(const QSize &)
{

}

void  CEmulationCanvs::changeGraphicalToCanvs()
{
    for(int i = 0; i < m_Graphicals.size(); i++)
    {
        m_Graphicals[i]->CanvsPoints.clear();
        for(int j = 0; j < m_Graphicals[i]->PointLists.size(); j++)
        {
            m_Graphicals[i]->CanvsPoints.push_back(coordinateVChange(m_Graphicals[i]->PointLists.at(j)));
        }
    }
}

void CEmulationCanvs::setRadio(double x, double y)
{
    QRect size = rect();
    m_nCanvsWidth = size.width();
    m_nCanvsHeight = size.height();
    double x_radio = ((double)(m_nCanvsWidth - 20)) / x;
    double y_radio = ((double)(m_nCanvsHeight - 20)) / y;

    m_dRadio = (x_radio > y_radio) ? y_radio : x_radio;

//    m_OriginX = (int)((m_nCanvsWidth - x * m_dRadio) / 2);
//    m_OriginY = m_nCanvsHeight - (int)((m_nCanvsHeight - y * m_dRadio) / 2);
    m_OriginX = (int)((m_nCanvsWidth) / 2);
    m_OriginY = (int)((m_nCanvsHeight) / 2);
}

QPoint CEmulationCanvs::coordinateVChange(double x, double y)
{
    int x1 =  (int)(m_OriginX + x * m_dRadio * m_dZoomRadio);
    int y1 =  (int)(m_OriginY - y * m_dRadio * m_dZoomRadio);
    return QPoint(x1, y1);
}

QPoint CEmulationCanvs::coordinateVChange(Point p)
{
    int x1 =  (int)(m_OriginX + p.x * m_dRadio * m_dZoomRadio);
    int y1 =  (int)(m_OriginY - p.y * m_dRadio * m_dZoomRadio);
    return QPoint(x1, y1);
}

void CEmulationCanvs::drawLine(Point p1, Point p2, Qt::PenStyle s, QColor c)
{
    QPainter painter(this);
    QPen pen(c, m_nLineWidth, s, Qt::SquareCap, Qt::RoundJoin);
    painter.setPen(pen);
    painter.drawLine(coordinateVChange(0.0, 0.0), coordinateVChange(0.0, 320));
    painter.drawLine(coordinateVChange(0.0, 320), coordinateVChange(380, 320));
    painter.drawLine(coordinateVChange(380, 320), coordinateVChange(380, 0));
    painter.drawLine(coordinateVChange(380, 0.0), coordinateVChange(0.0, 0.0));
}

void  CEmulationCanvs::drawRegion(QPoint pf, QPoint ps, QColor c)
{
    QPainter painter(this);
    QPen pen(c);
    painter.setPen(pen);
    QPoint p1(ps.x(), pf.y());
    QPoint p2(pf.x(), ps.y());
    painter.drawLine(pf, p1);
    painter.drawLine(p1, ps);
    painter.drawLine(ps, p2);
    painter.drawLine(p2, pf);
}

void CEmulationCanvs::drawGraphical(GraphicalInfo *Graphical)
{
    QPainter painter(this);
    QPen pen(Graphical->color, m_nLineWidth, Graphical->style, Qt::SquareCap, Qt::RoundJoin);
    painter.setPen(pen);

    if(Graphical->PointLists.size() > 1)
    {
        for(int i = 0; i < (Graphical->PointLists.size() - 1); i++)
        {
            painter.drawLine(coordinateVChange(Graphical->PointLists[i]), coordinateVChange(Graphical->PointLists[i + 1]));
        }

        if(Graphical->isClose)
        {
            painter.drawLine(coordinateVChange(Graphical->PointLists[0]), coordinateVChange(Graphical->PointLists[Graphical->PointLists.size() - 1]));
        }
    }
}

void  CEmulationCanvs::drawCarrierGraphical()
{
    for(int i = 0; i < m_CarrierGraphicals->Graphicals.size(); i++)
    {
        drawGraphical(m_CarrierGraphicals->Graphicals.at(i));
    }

    if(m_CarrierGraphicals->text.size() > 0)
    {
        QPainter painter(this);
        QFont font;
        font.setPointSize(m_CarrierGraphicals->textSize);
        painter.setFont(font);
        QPen pen(m_CarrierGraphicals->textcolor);
        painter.setPen(pen);
        painter.drawText(coordinateVChange(m_CarrierGraphicals->textPos), m_CarrierGraphicals->text);
    }
}

void  CEmulationCanvs::clearGraphicals()
{
    mutex.lock();
    for(int i = 0; i < m_Graphicals.size(); i++)
    {
        m_Graphicals.at(i)->PointLists.clear();
        delete  m_Graphicals.at(i);
    }

    m_Graphicals.clear();
    mutex.unlock();
}

int  CEmulationCanvs::getGraphicalsSize()
{
    return m_Graphicals.size();
}

void  CEmulationCanvs::init()
{
    clearGraphicals();
    m_nCanvsWidth = -1;
    m_nCanvsHeight = -1;
    m_dZoomRadio = 1.0;
    m_MoveX = 0;
    m_MoveY = 0;

    m_eStatus = STATUS_NONE;
}

void  CEmulationCanvs::addGraphicals(GraphicalInfo *Graphical)
{
    mutex.lock();
    if(Graphical != NULL)
    {
        for(int i = 0; i < Graphical->PointLists.size(); i++)
        {
            Graphical->CanvsPoints.append(coordinateVChange(Graphical->PointLists.at(i)));
        }
        m_Graphicals.append(Graphical);
    }
    mutex.unlock();
}

void CEmulationCanvs::clearCarrierGraphical()
{
    mutex.lock();
    for(int i = 0; i < m_CarrierGraphicals->Graphicals.size(); i++)
    {
        m_CarrierGraphicals->Graphicals.at(i)->PointLists.clear();
        //delete  m_CarrierGraphicals->Graphicals.at(i);
    }
    //m_CarrierGraphicals->Graphicals.clear();
    m_CarrierGraphicals->text = "";
    mutex.unlock();
}

void  CEmulationCanvs::updataCarrierInfo(GraphicalInfo* info1, GraphicalInfo* info2, GraphicalInfo* info3, Point p, QString s)
{
    mutex.lock();
    for(int i = 0; i < m_CarrierGraphicals->Graphicals.size(); i++)
    {
        m_CarrierGraphicals->Graphicals.at(i)->PointLists.clear();
    //    delete  m_CarrierGraphicals->Graphicals.at(i);
    }
    m_CarrierGraphicals->Graphicals.clear();
    m_CarrierGraphicals->Graphicals.push_back(info1);
    m_CarrierGraphicals->Graphicals.push_back(info2);
    m_CarrierGraphicals->Graphicals.push_back(info3);
    m_CarrierGraphicals->textPos = p;
    m_CarrierGraphicals->text = s;

    mutex.unlock();
}

void  CEmulationCanvs::updataCarrierInfo(Point p, double r, double len, Point pt, QString s)
{
    mutex.lock();

    for(int i = 0; i < m_CarrierGraphicals->Graphicals.size(); i++)
    {
        m_CarrierGraphicals->Graphicals.at(i)->PointLists.clear();
    }
    Q_ASSERT(m_CarrierGraphicals->Graphicals.size() == 3);


    double t = 2 * PI / CIRCLE_SEGMENTATION_NUM;

    for(int i = 0; i < CIRCLE_SEGMENTATION_NUM; i++)
    {
        m_CarrierGraphicals->Graphicals.at(0)->PointLists.push_back(Point(r * cos(t * i), r * sin(t * i),0,0,0,0) + p);
    }

    m_CarrierGraphicals->Graphicals.at(1)->PointLists.push_back(Point((p.x - len), p.y,0,0,0,0));
    m_CarrierGraphicals->Graphicals.at(1)->PointLists.push_back(Point((p.x + len), p.y,0,0,0,0));

    m_CarrierGraphicals->Graphicals.at(2)->PointLists.push_back(Point(p.x, (p.y - len),0,0,0,0));
    m_CarrierGraphicals->Graphicals.at(2)->PointLists.push_back(Point(p.x, (p.y + len),0,0,0,0));

    m_CarrierGraphicals->textPos = pt;
    m_CarrierGraphicals->text = s;

    mutex.unlock();
}

void  CEmulationCanvs::updataCarrierInfo(Point p, double r, double len)
{
    mutex.lock();

    for(int i = 0; i < m_CarrierGraphicals->Graphicals.size(); i++)
    {
        m_CarrierGraphicals->Graphicals.at(i)->PointLists.clear();
    }
    Q_ASSERT(m_CarrierGraphicals->Graphicals.size() == 3);


    double t = 2 * PI / CIRCLE_SEGMENTATION_NUM;

//    for(int i = 0; i < CIRCLE_SEGMENTATION_NUM; i++)
//    {
//        m_CarrierGraphicals->Graphicals.at(0)->PointLists.push_back(Point(r * cos(t * i), r * sin(t * i)) + p);
//    }
    len = m_dCursorRadio;
    r = m_dCursorRadio;
    m_CarrierGraphicals->Graphicals.at(0)->PointLists.push_back(p);
    m_CarrierGraphicals->Graphicals.at(0)->PointLists.push_back(p);

    m_CarrierGraphicals->Graphicals.at(1)->PointLists.push_back(Point((p.x - len), p.y,0,0,0,0));
    m_CarrierGraphicals->Graphicals.at(1)->PointLists.push_back(Point((p.x + len), p.y,0,0,0,0));

    m_CarrierGraphicals->Graphicals.at(2)->PointLists.push_back(Point(p.x, (p.y - len),0,0,0,0));
    m_CarrierGraphicals->Graphicals.at(2)->PointLists.push_back(Point(p.x, (p.y + len),0,0,0,0));

    m_CarrierGraphicals->textPos = p + Point(r, r,0,0,0,0);
    m_CarrierGraphicals->text = "(" + QString::number(p.x) + "," + QString::number(p.y) + ")";

    mutex.unlock();
}

void  CEmulationCanvs::setSizeXY(double x, double y, double x0, double y0)
{
    m_dSizeX = x;
    m_dSizeY = y;
    m_dCenterX = x0;
    m_dCenterY = y0;
    m_dCursorRadio = y / 50.0;//设置光标比例
}

CarrierGraphicalInfo*   CEmulationCanvs::getCarrierGraphicalsHandle()
{
    return m_CarrierGraphicals;
}

void  CEmulationCanvs::setCanvasStatus(viewStatus vv, int arg1, int arg2)
{
    switch(vv)
    {
    case STATUS_MOVE:
        {
            m_eStatus = STATUS_NO_DELAY_MOVE;
            m_bisKeepStatus = true;
            QCursor Cursor(Qt::SizeAllCursor);
            setCursor(Cursor);
        }break;
    case STATUS_ZOOM_OUT:
        {
            m_eStatus = STATUS_ZOOM_OUT;
            m_bisKeepStatus = true;
            QCursor Cursor(Qt::OpenHandCursor);
            setCursor(Cursor);
        }break;
    case STATUS_ZOOM_IN:
        {
            m_eStatus = STATUS_ZOOM_IN;
            m_bisKeepStatus = true;
            QCursor Cursor(Qt::ClosedHandCursor);
            setCursor(Cursor);
        }break;
    case STATUS_ZOOM_REGION:
        {
            m_eStatus = STATUS_ZOOM_REGION;
            m_bisKeepStatus = true;
            QCursor Cursor(Qt::UpArrowCursor);
            setCursor(Cursor);
        }break;
    case STATUS_RESTORE:
        {
            m_eStatus = STATUS_NONE;
            m_bisKeepStatus = false;
            QCursor Cursor(Qt::ArrowCursor);
            setCursor(Cursor);

            m_nCanvsWidth = -1;
            m_nCanvsHeight = -1;
            m_dZoomRadio = 1.0;
            update();

        }break;
    case STATUS_NONE:
        {
            m_eStatus = STATUS_NONE;
            m_bisKeepStatus = false;
            QCursor Cursor(Qt::ArrowCursor);
            setCursor(Cursor);
        }break;
    }
}

void CEmulationCanvs::setOrignalPoint(double x, double y)
{
    m_OriginX -= x * m_dRadio;
    m_OriginY += y * m_dRadio;
}





