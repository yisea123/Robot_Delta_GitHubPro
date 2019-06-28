#include "ToolInfo.h"
#include "GlobalVariableType.h"

#include <QMatrix>

ToolInfo::ToolInfo()
{
    m_nFrameNum = -1;
    m_toolType = -1;
    m_toolSerialNum = -1;
    m_toolX = -1;
    m_toolY = -1;
    m_radius = -1;
    m_angle = 0;    
    m_trC = 0;
    m_MJd = 0;
    m_CRa = 0;
    m_CRb = 0;
    m_CRr = 0;
    m_bIsRotary = false;
    m_dPosAngle = 0.0;
}

ToolInfo::ToolInfo(QString s)
{
    m_nFrameNum = -1;
    m_toolType = -1;
    m_toolSerialNum = -1;
    m_toolX = -1;
    m_toolY = -1;
    m_radius = -1;
    m_angle = 0;
    m_trC = 0;
    m_MJd = 0;
    m_CRa = 0;
    m_CRb = 0;
    m_CRr = 0;
    m_bIsRotary = false;
    m_dPosAngle = 0.0;
}

void ToolInfo::setFrameNum(int i)
{
    m_nFrameNum = i;
}

void ToolInfo::setSerialNum(int i)
{
    m_toolSerialNum = i;
}

int  ToolInfo::getSerialNum()
{
    return m_toolSerialNum;
}

void ToolInfo::settoolXY(double x, double y)
{
    m_toolX = x;
    m_toolY = y;
}

void ToolInfo::setTrC(double c)
{
    m_trC = c;
}

void ToolInfo::setMJD(double d)
{
    m_MJd = d;
}

void ToolInfo::setCRA(double a)
{
    m_CRa = a;
}

void ToolInfo::setCRB(double b)
{
    m_CRb = b;
}

void ToolInfo::setCRR(double r)
{
    m_CRr = r;
}

void ToolInfo::setRadius(double r)
{
    m_radius = r;
}

void ToolInfo::setAngle(double a)
{
    m_angle = a;// / 180.0 * PI;
}

void ToolInfo::setName(QString s)
{
    m_name = s;
}

void ToolInfo::setToolType(QString s)
{
    for(int i = 0; i < TOOL_TYPE_NUM; i++)
    {
        if(s == ToolNames[i])
        {
            m_toolType = i;
            break;
        }
    }
}

void ToolInfo::setRotaryFlag(bool b)
{
    m_bIsRotary = b;
}

bool  ToolInfo::breakupSquareGraph()
{
    if(m_toolX > 0)
    {
        Point  p;
        p.x = -m_toolX / 2;
        p.y = p.x;
        m_PointList.push_back(p);
        p.y = -p.x;
        m_PointList.push_back(p);
        p.x = -p.x;
        m_PointList.push_back(p);
        p.y = -p.x;
        m_PointList.push_back(p);

        return TRUE;

    }
    else
    {
        return FALSE;
    }
}

bool  ToolInfo::breakupSquareGraph(QVector<Point>& ps, Point p, double angle)
{
    if(m_toolX > 0)
    {
        Point  pp;
        double d = sqrt(2.0) / 2.0 * m_toolX;
        angle = PI * angle / 180.0;

        for(int i = 0; i < 4; i++)
        {
            pp.x = d * cos(angle + PI / 4.0) + p.x;
            pp.y = d * sin(angle + PI / 4.0) + p.y;
            angle += PI / 2;
            ps.push_back(pp);
        }
        return TRUE;

    }
    else
    {
        return FALSE;
    }
}

bool  ToolInfo::breakupRectangularGraph()
{
    if((m_toolX > 0) && (m_toolY > 0))
    {
        Point  p;
        p.x = -m_toolX / 2;
        p.y = p.x;
        m_PointList.push_back(p);
        p.y = -p.y;
        m_PointList.push_back(p);
        p.x = -p.x;
        m_PointList.push_back(p);
        p.y = -p.y;
        m_PointList.push_back(p);

        return TRUE;

    }
    else
    {
        return FALSE;
    }
}

bool  ToolInfo::breakupRectangularGraph(QVector<Point>& ps, Point p, double angle)
{
    if((m_toolX > 0) && (m_toolY > 0))
    {
        Point  pp;
        double d = sqrt(m_toolX * m_toolX + m_toolY * m_toolY) / 2.0;
        double a = atan2(m_toolY, m_toolX);

        angle = PI * angle / 180.0;
        double aa = angle + a;

        pp.x = d * cos(aa) + p.x;
        pp.y = d * sin(aa) + p.y;
        ps.push_back(pp);

        aa += PI - 2 * a;
        pp.x = d * cos(aa) + p.x;
        pp.y = d * sin(aa) + p.y;
        ps.push_back(pp);

        aa += 2 * a;
        pp.x = d * cos(aa) + p.x;
        pp.y = d * sin(aa) + p.y;
        ps.push_back(pp);

        aa += PI - 2 * a;
        pp.x = d * cos(aa) + p.x;
        pp.y = d * sin(aa) + p.y;
        ps.push_back(pp);

        return TRUE;

    }
    else
    {
        return FALSE;
    }
}

bool  ToolInfo::breakupRoundGraph()
{
    if(m_radius > 0)
    {
        double r = m_radius / 2.0;
        double n = 2 * PI / 16;
        for(int i = 0; i < 16; i++)
        {
            m_PointList.push_back(Point(r * cos(n * i), r * sin(n * i),0,0,0,0));
        }

        return TRUE;

    }
    else
    {
        return FALSE;
    }
}

bool  ToolInfo::breakupRoundGraph(QVector<Point>& ps, Point p, double angle)
{
    if(m_radius > 0)
    {
        double r = m_radius / 2.0;
        double n = 2 * PI / 16;
        for(int i = 0; i < 16; i++)
        {
            ps.push_back(Point(p.x + r * cos(n * i), p.y + r * sin(n * i),0,0,0,0));
        }

        return TRUE;

    }
    else
    {
        return FALSE;
    }
}

bool  ToolInfo::breakupOBGraph()
{
    return breakupOBGraph(m_PointList, Point(0, 0,0,0,0,0), m_angle);
}

bool  ToolInfo::breakupOBGraph(QVector<Point>& ps, Point p, double angle)
{
    double x, y, n, r, a, d;

    angle = PI * angle / 180.0;
    x = -m_toolX/2.0;
    y = m_radius; //m_toolY/2.0;
    a = atan2(y, x) + angle;
    d = sqrt(x*x+y*y);
    ps.push_back(Point(p.x+d*cos(a), p.y+d*sin(a),0,0,0,0));

    x = m_toolX/2.0;
    y = m_radius; //m_toolY/2.0;
    a = atan2(y, x) + angle;
    d = sqrt(x*x+y*y);
    ps.push_back(Point(p.x+d*cos(a), p.y+d*sin(a),0,0,0,0));

    r = m_radius; // m_toolY / 2.0;
    n = PI / 8;
    for (int i = 1; i < 8; ++i)
    {
        x = r * cos(-n * i + PI/2.0) + m_toolX/2.0;
        y = r * sin(-n * i + PI/2.0);

        a = atan2(y, x) + angle;
        d = sqrt(x*x+y*y);
        ps.push_back(Point(p.x+d*cos(a), p.y+d*sin(a),0,0,0,0));
    }

    x = m_toolX/2.0;
    y = -m_radius; // -m_toolY/2.0;
    a = atan2(y, x) + angle;
    d = sqrt(x*x+y*y);
    ps.push_back(Point(p.x+d*cos(a), p.y+d*sin(a),0,0,0,0));

    x = -m_toolX/2.0;
    y = -m_radius; // -m_toolY/2.0;
    a = atan2(y, x) + angle;
    d = sqrt(x*x+y*y);
    ps.push_back(Point(p.x+d*cos(a), p.y+d*sin(a),0,0,0,0));

    n = PI / 8;
    for (int i = 1; i <= 8; ++i)
    {
        x = r * cos(-n * i + PI*3.0/2.0) - m_toolX/2.0;
        y = r * sin(-n * i + PI*3.0/2.0);

        a = atan2(y, x) + angle;
        d = sqrt(x*x+y*y);
        ps.push_back(Point(p.x+d*cos(a), p.y+d*sin(a),0,0,0,0));
    }

    return TRUE;
}

bool  ToolInfo::breakupWDGraph()
{
    return breakupWDGraph(m_PointList, Point(0, 0,0,0,0,0), m_angle);
}

bool  ToolInfo::breakupWDGraph(QVector<Point>& ps, Point p, double angle)
{
    double a1, a2, x1, y1, x2, y2, n, r;
    angle = PI * angle / 180.0;

    a1 = asin(m_toolY/m_toolX);

    a2 = PI - a1 + angle;
    x1 = (m_toolX/2.0)*cos(a2);
    y1 = (m_toolX/2.0)*sin(a2);

    a2 = a1 + angle;
    x2 = (m_toolX/2.0)*cos(a2);
    y2 = (m_toolX/2.0)*sin(a2);

    ps.push_back(Point(p.x+x1, p.y+y1,0,0,0,0));
    ps.push_back(Point(p.x+x2, p.y+y2,0,0,0,0));

    n = a1*2 / 10.0;
    r = m_toolX/2.0;
    a2 = a1 + angle;
    for (int i = 1; i <= 10; ++i)
    {
        x1 = r * cos(a2);
        y1 = r * sin(a2);

        ps.push_back(Point(p.x+x1, p.y+y1,0,0,0,0));
        a2 -= n;
    }

    a2 = -a1 + angle;
    x1 = (m_toolX/2.0)*cos(a2);
    y1 = (m_toolX/2.0)*sin(a2);

    a2 = PI + a1 + angle;
    x2 = (m_toolX/2.0)*cos(a2);
    y2 = (m_toolX/2.0)*sin(a2);
    ps.push_back(Point(p.x+x1, p.y+y1,0,0,0,0));
    ps.push_back(Point(p.x+x2, p.y+y2,0,0,0,0));

    n = a1*2 / 10.0;
    r = m_toolX/2.0;
    a2 = PI + a1 + angle;
    for (int i = 1; i <= 10; ++i)
    {
        x1 = r * cos(a2);
        y1 = r * sin(a2);

        ps.push_back(Point(p.x+x1, p.y+y1,0,0,0,0));
        a2 -= n;
    }

    return TRUE;
}

bool  ToolInfo::breakupSDGraph()
{
    return breakupOBGraph(m_PointList, Point(0, 0,0,0,0,0), m_angle);
}

bool ToolInfo::breakupTRGraph()
{
    return breakupTRGraph(m_PointList, Point(0, 0,0,0,0,0), m_angle);
}

bool ToolInfo::breakupMJGraph()
{
    return breakupMJGraph(m_PointList, Point(0, 0,0,0,0,0), m_angle);
}

bool ToolInfo::breakupRRGraph()
{
    return breakupRRGraph(m_PointList, Point(0, 0,0,0,0,0), m_angle);
}

bool ToolInfo::breakupCRGraph()
{
    return breakupCRGraph(m_PointList, Point(0, 0,0,0,0,0), m_angle);
}

bool ToolInfo::breakupCRGraph(QVector<Point> &ps, Point p, double angle)
{
    double x, y, n, r, a, x0, y0;
    angle = PI * angle / 180.0;
    QPointF pt1, pt2;
    QMatrix m;
    m.reset();
    m.translate(p.x, p.y);
    m.rotate(angle/PI*180);


    pt2 = m.map(QPointF(-m_CRa/2, m_toolY/2));
    ps.push_back(Point(pt2.x(), pt2.y(),0,0,0,0));

    pt2 = m.map(QPointF(m_CRa/2, m_toolY/2));
    ps.push_back(Point(pt2.x(), pt2.y(),0,0,0,0));

    x0 = m_CRa/2 + m_CRr;
    y0 = m_CRb/2 + m_CRr;
    n = PI / 20.0;
    a = PI;
    for (int i = 1; i <= 10; ++i)
    {
        x = x0 + m_CRr * cos(a);
        y = y0 + m_CRr * sin(a);

        pt2 = m.map(QPointF(x, y));
        ps.push_back(Point(pt2.x(), pt2.y(),0,0,0,0));
        a += n;
    }

    pt2 = m.map(QPointF(m_toolX/2, m_CRb/2));
    ps.push_back(Point(pt2.x(), pt2.y(),0,0,0,0));

    pt2 = m.map(QPointF(m_toolX/2, -m_CRb/2));
    ps.push_back(Point(pt2.x(), pt2.y(),0,0,0,0));

    x0 = m_CRa/2 + m_CRr;
    y0 = -m_CRb/2 - m_CRr;
    n = PI / 20.0;
    a = PI/2;
    for (int i = 1; i <= 10; ++i)
    {
        x = x0 + m_CRr * cos(a);
        y = y0 + m_CRr * sin(a);

        pt2 = m.map(QPointF(x, y));
        ps.push_back(Point(pt2.x(), pt2.y(),0,0,0,0));
        a += n;
    }

    pt2 = m.map(QPointF(m_CRa/2, -m_toolY/2));
    ps.push_back(Point(pt2.x(), pt2.y(),0,0,0,0));

    pt2 = m.map(QPointF(-m_CRa/2, -m_toolY/2));
    ps.push_back(Point(pt2.x(), pt2.y(),0,0,0,0));

    x0 = -m_CRa/2 - m_CRr;
    y0 = -m_CRb/2 - m_CRr;
    n = PI / 20.0;
    a = 0;
    for (int i = 1; i <= 10; ++i)
    {
        x = x0 + m_CRr * cos(a);
        y = y0 + m_CRr * sin(a);

        pt2 = m.map(QPointF(x, y));
        ps.push_back(Point(pt2.x(), pt2.y(),0,0,0,0));
        a += n;
    }

    pt2 = m.map(QPointF(-m_toolX/2, -m_CRb/2));
    ps.push_back(Point(pt2.x(), pt2.y(),0,0,0,0));

    pt2 = m.map(QPointF(-m_toolX/2, m_CRb/2));
    ps.push_back(Point(pt2.x(), pt2.y(),0,0,0,0));

    x0 = -m_CRa/2 - m_CRr;
    y0 = m_CRb/2 + m_CRr;
    n = PI / 20.0;
    a = -PI/2;
    for (int i = 1; i <= 10; ++i)
    {
        x = x0 + m_CRr * cos(a);
        y = y0 + m_CRr * sin(a);

        pt2 = m.map(QPointF(x, y));
        ps.push_back(Point(pt2.x(), pt2.y(),0,0,0,0));
        a += n;
    }

    return TRUE;
}

bool ToolInfo::breakupRRGraph(QVector<Point> &ps, Point p, double angle)
{
    double x, y, a, a1, r, n;
    angle = PI * angle / 180.0;
    n = PI / 20.0;
    a1 = PI / 2;
    for (int i = 1; i <= 10; ++i)
    {
        x = m_radius * cos(a1) + m_toolX/2 - m_radius;
        y = m_radius * sin(a1) + m_toolY/2 - m_radius;

        r = sqrt(x*x + y*y);
        a = atan(y/x);

        a = a + angle;
        x = r * cos(a);
        y = r * sin(a);

        ps.push_back(Point(p.x+x, p.y+y,0,0,0,0));

        a1 -= n;
    }

    a1 = 0;
    for (int i = 1; i <= 10; ++i)
    {
        x = m_radius * cos(a1) + m_toolX/2 - m_radius;
        y = m_radius * sin(a1) - m_toolY/2 + m_radius;

        r = sqrt(x*x + y*y);
        a = atan2(y, x);

        a = a + angle;
        x = r * cos(a);
        y = r * sin(a);

        ps.push_back(Point(p.x+x, p.y+y,0,0,0,0));

        a1 -= n;
    }

    a1 = -PI / 2;
    for (int i = 1; i <= 10; ++i)
    {
        x = m_radius * cos(a1) - m_toolX/2 + m_radius;
        y = m_radius * sin(a1) - m_toolY/2 + m_radius;

        r = sqrt(x*x + y*y);
        a = atan2(y, x);

        a = a + angle;
        x = r * cos(a);
        y = r * sin(a);

        ps.push_back(Point(p.x+x, p.y+y,0,0,0,0));

        a1 -= n;
    }

    a1 = PI;
    for (int i = 1; i <= 10; ++i)
    {
        x = m_radius * cos(a1) - m_toolX/2 + m_radius;
        y = m_radius * sin(a1) + m_toolY/2 - m_radius;

        r = sqrt(x*x + y*y);
        a = atan2(y, x);

        a = a + angle;
        x = r * cos(a);
        y = r * sin(a);

        ps.push_back(Point(p.x+x, p.y+y,0,0,0,0));

        a1 -= n;
    }

    return TRUE;
}

bool ToolInfo::breakupMJGraph(QVector<Point> &ps, Point p, double angle)
{
    double a, x, y, r;
    double r1, r2, a1, a2;
    angle = PI * angle / 180.0;

    x = m_toolX / 2;
    y = m_toolY / 2;
    r1 = r =sqrt(x*x + y*y);
    a1 = a = atan(y/x);
    a = PI - a + angle;

    x = r * cos(a);
    y = r * sin(a);
    ps.push_back(Point(p.x+x, p.y+y,0,0,0,0));

    a = a1 + angle;
    x = r * cos(a);
    y = r * sin(a);
    ps.push_back(Point(p.x+x, p.y+y,0,0,0,0));

    r2 = r = m_toolX/2 - m_MJd;
    a2 = a = angle;
    x = r * cos(a);
    y = r * sin(a);
    ps.push_back(Point(p.x+x, p.y+y,0,0,0,0));

    r = r1;
    a = a1;
    a = -a + angle;
    x = r * cos(a);
    y = r * sin(a);
    ps.push_back(Point(p.x+x, p.y+y,0,0,0,0));

    r = r1;
    a = a1;
    a = PI + a + angle;
    x = r * cos(a);
    y = r * sin(a);
    ps.push_back(Point(p.x+x, p.y+y,0,0,0,0));

    r = r2;
    a = angle + PI;
    x = r * cos(a);
    y = r * sin(a);
    ps.push_back(Point(p.x+x, p.y+y,0,0,0,0));

    return TRUE;
}

bool ToolInfo::breakupTRGraph(QVector<Point> &ps, Point p, double angle)
{
    double a, x, y, d;
    angle = PI * angle / 180.0;
    a = angle;

    x = (m_toolY - m_trC) * cos(PI/2+angle);
    y = (m_toolY - m_trC) * sin(PI/2+angle);
    ps.push_back(Point(p.x+x, p.y+y,0,0,0,0));

    x = m_toolX / 2.0;
    y = -m_trC;
    d = sqrt(x*x + y*y);

    a = atan(-y/x);
    x = d * cos(angle - a);
    y = d * sin(angle - a);
    ps.push_back(Point(p.x+x, p.y+y,0,0,0,0));

    x = d * cos(PI + angle + a);
    y = d * sin(PI + angle + a);
    ps.push_back(Point(p.x+x, p.y+y,0,0,0,0));

    return TRUE;
}

bool  ToolInfo::breakupSDGraph(QVector<Point>& ps, Point p, double angle)
{
    double x, y, n, r, a1, a2, d;
    angle = PI * angle / 180.0;

    r = m_toolX / 2.0;
    a1 = acos( (m_toolY-r)/r);

    a2 = -a1 + angle;
    ps.push_back(Point(p.x+r*cos(a2), p.y+r*sin(a2),0,0,0,0));

    a2 = a1 + angle;
    ps.push_back(Point(p.x+r*cos(a2), p.y+r*sin(a2),0,0,0,0));

    n = 2.0*(PI-a1)/16.0;
    for (int i = 1; i < 16; ++i)
    {
        a2 = i*n+a1+angle;
        x = r * cos(a2) + p.x;
        y = r * sin(a2) + p.y;
        ps.push_back(Point(x, y,0,0,0,0));
    }

    return TRUE;
}


bool ToolInfo::breakupGraph()
{
    /* 将图形打断成点 */
    switch(m_toolType)
    {
        case TOOL_TYPE_RECTANGULAR_SHAPE:
        {
            return  breakupRectangularGraph();
        }
        case TOOL_TYPE_SQUARE_SHAPE:
        {
            return  breakupSquareGraph();
        }
        case TOOL_TYPE_ROUND_SHAPE:
        {
            return  breakupRoundGraph();
        }
        case TOOL_TYPE_OB_SHAPE:
        {
            return breakupOBGraph();
        }
        case TOOL_TYPE_WD_SHAPE:
        {
            return breakupWDGraph();
        }
        case TOOL_TYPE_SD_SHAPE:
        {
            return breakupSDGraph();
        }
        case TOOL_TYPE_TR_SHAPE:
        {
            return breakupTRGraph();
        }
        case TOOL_TYPE_MJ_SHAPE:
        {
            return breakupMJGraph();
        }
        case TOOL_TYPE_RR_SHAPE:
        {
            return breakupRRGraph();
        }
        case TOOL_TYPE_CR_SHAPE:
        {
            return breakupCRGraph();
        }
        default:
        {
            return FALSE;
        }
    }
}

bool ToolInfo::breakupGraph(QVector<Point>& ps, Point p, double angle)
{
    /* 将图形打断成点 */
    switch(m_toolType)
    {
        case TOOL_TYPE_RECTANGULAR_SHAPE:
        {
            return  breakupRectangularGraph(ps, p, m_angle + angle);
        }
        case TOOL_TYPE_SQUARE_SHAPE:
        {
            return  breakupSquareGraph(ps, p, m_angle + angle);
        }
        case TOOL_TYPE_ROUND_SHAPE:
        {
            return  breakupRoundGraph(ps, p, m_angle + angle);
        }
        case TOOL_TYPE_OB_SHAPE:
        {
            return breakupOBGraph(ps, p, m_angle + angle);
        }
        case TOOL_TYPE_WD_SHAPE:
        {
            return breakupWDGraph(ps, p, m_angle + angle);
        }
        case TOOL_TYPE_SD_SHAPE:
        {
            return breakupSDGraph(ps, p, m_angle + angle);
        }
        case TOOL_TYPE_TR_SHAPE:
        {
            return breakupTRGraph(ps, p, m_angle + angle);
        }
        case TOOL_TYPE_MJ_SHAPE:
        {
            return breakupMJGraph(ps, p, m_angle + angle);
        }
        case TOOL_TYPE_RR_SHAPE:
        {
            return breakupRRGraph(ps, p, m_angle + angle);
        }
        case TOOL_TYPE_CR_SHAPE:
        {
            return breakupCRGraph(ps, p, m_angle + angle);
        }
        default:
        {
            return FALSE;
        }
    }
}

void ToolInfo::setPosAngle(double t)
{
    m_dPosAngle = t;
}

double ToolInfo::getPosAngle()
{
    return m_dPosAngle;
}



bool  ToolInfo::getDrawPointList(QVector<Point>& pp, Point p, double ang)
{
    return breakupGraph(pp, p, ang);
}

bool ToolInfo::isRotaryTool()
{
    /*switch (m_toolType)
    {
        case TOOL_TYPE_RECTANGULAR_SHAPE:
        {
            return  TRUE;
        }
        case TOOL_TYPE_SQUARE_SHAPE:
        {
            return  TRUE;
        }
        case TOOL_TYPE_ROUND_SHAPE:
        {
            return  FALSE;
        }
        case TOOL_TYPE_OB_SHAPE:
        {
            return TRUE;
        }
        case TOOL_TYPE_WD_SHAPE:
        {
            return TRUE;
        }
        case TOOL_TYPE_SD_SHAPE:
        {
            return TRUE;
        }
        case TOOL_TYPE_TR_SHAPE:
        {
            return TRUE;
        }
        case TOOL_TYPE_MJ_SHAPE:
        {
            return TRUE;
        }
        case TOOL_TYPE_RR_SHAPE:
        {
            return TRUE;
        }
        case TOOL_TYPE_CR_SHAPE:
        {
            return TRUE;
        }
        default:
        {
            return FALSE;
        }
    }*/
    return m_bIsRotary;
}

double ToolInfo::getToolAngle()
{
    return m_angle;
}
