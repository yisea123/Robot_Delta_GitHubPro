/************************************************************
  Copyright (C),
  �ļ�����BaseDevice.cpp
  ���ߣ�zhou    �汾��V1.0    �������ڣ�2018/07/10
  ģ��������BaseDevice���ʵ��
  ��ʷ�޸ļ�¼��<author>    <time>    <version >    <desc>
                  zhou    2018/07/10     V1.0      ���ע��
*************************************************************/

#include "EmulationControllerDevice.h"
#include "BaseModule.h"
//#include "EmulationControllerModule.h"
#include "cemulationcanvs.h"
//#include "UI/cprogramemulation.h"
//#include "cmachiningsimulationui.h"
#include "cautomachiningstateui.h"
#include "GlobalVariableType.h"
//#include "ProcEmulationModel.h"


#include <QPainter>
#include <QTimer>

EmulationControllerDevice::EmulationControllerDevice()
{

}

EmulationControllerDevice::EmulationControllerDevice(QString s, BaseModule* m)
{
    m_sDeviceName = s;
    m_pModuleHandle = m;
}

/*************************************************
  �������ƣ�openDevice(ErrorInfo& e)
  �������ܣ����豸
  ���������
  ���������e  ������Ϣ
  �� �� ֵ��int δ����
  ��    ע��
**************************************************/
int EmulationControllerDevice::openDevice(ErrorInfo& e)
{
    return  initDevice(e);
}

/*************************************************
  �������ƣ�initDevice(ErrorInfo& e)
  �������ܣ��豸��ʼ��
  ���������
  ��������� e  ������Ϣ
  �� �� ֵ�� 0   �ɹ�
           ��0  ʧ��
  ��    ע��
**************************************************/
int EmulationControllerDevice::initDevice(ErrorInfo& e)
{
//    m_canvs = ((EmulationControllerModule *)m_pModuleHandle)->getEmulationCanvs();
//    if(m_canvs == NULL)
//    {
//        return -1;
//    }

    return 0;
}

/*************************************************
  �������ƣ�setCanvsHandle(CEmulationCanvs* c)
  �������ܣ�ֹͣ�豸
  ���������
  ���������
  �� �� ֵ��0   �ɹ�
           ��0  ʧ��
  ��    ע��
**************************************************/
void EmulationControllerDevice::setCanvsHandle(CEmulationCanvs* c)
{
    //m_canvs = c;
}

/*************************************************
  �������ƣ�clearCanvsData()
  �������ܣ�ֹͣ�豸
  ���������
  ���������
  �� �� ֵ��0   �ɹ�
           ��0  ʧ��
  ��    ע��
**************************************************/
void EmulationControllerDevice::clearCanvsData()
{
    //Q_CHECK_PTR(m_canvs);
    //m_canvs->clearGraphicals();
}

/*************************************************
  �������ƣ�startDevice(ErrorInfo& e)
  �������ܣ������豸
  ���������
  ���������
  �� �� ֵ��0   �ɹ�
           ��0  ʧ��
  ��    ע��
**************************************************/
int EmulationControllerDevice::startDevice(ErrorInfo& e)
{
    //Q_CHECK_PTR(m_canvs);
    //m_canvs->enableDrawFunction();

    return 0;
}

/*************************************************
  �������ƣ�restartDevice(ErrorInfo& e)
  �������ܣ������豸
  ���������
  ���������
  �� �� ֵ��0   �ɹ�
           ��0  ʧ��
  ��    ע��
**************************************************/
int EmulationControllerDevice::restartDevice(ErrorInfo& e)
{
    //Q_CHECK_PTR(m_canvs);
    //m_canvs->init();

    return 0;
}

/*************************************************
  �������ƣ�pauseDevice(ErrorInfo& e)
  �������ܣ���ͣ�豸
  ���������
  ���������
  �� �� ֵ��0   �ɹ�
           ��0  ʧ��
  ��    ע��
**************************************************/
int EmulationControllerDevice::pauseDevice(ErrorInfo& e)
{
    //m_canvs->disableDrawFunction();

    return 0;
}

/*************************************************
  �������ƣ�stopDevice(ErrorInfo& e)
  �������ܣ�ֹͣ�豸
  ���������
  ���������
  �� �� ֵ��0   �ɹ�
           ��0  ʧ��
  ��    ע��
**************************************************/
int EmulationControllerDevice::stopDevice(ErrorInfo& e)
{
    //m_canvs->disableDrawFunction();

    return 0;
}

/*************************************************
  �������ƣ�drawBackground(Material mater, QVector<BaseAction *>*  actions, EmulationInterVariableCache* cache, ErrorInfo& e)
  �������ܣ����Ƽӹ��ļ��ı���ͼ���Լ��߿�
  ���������mater �ӹ��ļ��������ͷ����Ϣ
          actions   BaseAction�б�
          cache  �м��������
  ���������e  ��¼������Ϣ
  �� �� ֵ�� 0   �ɹ�
           ��0  ʧ��
  ��    ע��
**************************************************/
int EmulationControllerDevice::drawBackground(Material mater, QVector<BaseAction *>*  actions, EmulationInterVariableCache* cache, ErrorInfo& e)
{
    //drawDesk(mater, cache);
    //drawDesk(mater, cache, actions);
    //drawBackgroundActions(actions, e);

    return 0;
}

/*************************************************
  �������ƣ�drawDesk(Material mater, EmulationInterVariableCache* cache)
  �������ܣ����Ƽӹ��߿�
  ���������mater �ӹ��ļ��������ͷ����Ϣ
          cache  �м��������
  ���������
  �� �� ֵ��0   �ɹ�
           ��0  ʧ��
  ��    ע��
**************************************************/
int EmulationControllerDevice::drawDesk(Material mater, EmulationInterVariableCache* cache)
{
    /*if((mater.size_x > 0) && (mater.size_y > 0))
    {
        //m_canvs->setSizeXY(mater.size_x, mater.size_y, mater.size_x/2, mater.size_y/2);
        double sx = 0;
        double sy = 0;
        double cx = 0;
        double cy = 0;
        if (cache->m_MachineCoordinate.x >= 0)
        {
            sx = mater.size_x + cache->m_MachineCoordinate.x;
            sy = mater.size_y + cache->m_MachineCoordinate.y;
            cx = sx / 2;
            cy = sy / 2;
        }
        else
        {
            sx = -cache->m_MachineCoordinate.x;
            sy = -cache->m_MachineCoordinate.y;
            sx = sx > mater.size_x ? sx : mater.size_x;
            sy = sy > mater.size_y ? sy : mater.size_y;
            cx = sx > mater.size_x ? -sx / 2 : cache->m_MachineCoordinate.x + sx / 2;
            cy = sx > mater.size_x ? -sy / 2 : cache->m_MachineCoordinate.y + sy / 2;
        }
        m_canvs->setSizeXY(sx, sy, cx, cy);

        GraphicalInfo *info = new GraphicalInfo;
        info->PointLists.push_back(Point(0, 0) + cache->m_MachineCoordinate);
        info->PointLists.push_back(Point(0, mater.size_y) + cache->m_MachineCoordinate);
        info->PointLists.push_back(Point(mater.size_x, mater.size_y) + cache->m_MachineCoordinate);
        info->PointLists.push_back(Point(mater.size_x, 0) + cache->m_MachineCoordinate);
        info->isClose = true;
        info->color = Qt::red;

        m_canvs->addGraphicals(info);
    }
//    m_canvs->update();*/

    return 0;
}

int EmulationControllerDevice::drawDesk(Material mater, EmulationInterVariableCache* cache, QVector<BaseAction *>*  actions)
{
    /*if((mater.size_x > 0) && (mater.size_y > 0))
    {
        double sx = 0;
        double sy = 0;
        double cx = 0;
        double cy = 0;
        if (cache->m_MachineCoordinate.x >= 0)
        {
            sx = mater.size_x + cache->m_MachineCoordinate.x;
            sy = mater.size_y + cache->m_MachineCoordinate.y;
            cx = sx / 2;
            cy = sy / 2;
        }
        else
        {
            sx = -cache->m_MachineCoordinate.x;
            sy = -cache->m_MachineCoordinate.y;
            sx = sx > mater.size_x ? sx : mater.size_x;
            sy = sy > mater.size_y ? sy : mater.size_y;
            cx = sx > mater.size_x ? -sx / 2 : cache->m_MachineCoordinate.x + sx / 2;
            cy = sx > mater.size_x ? -sy / 2 : cache->m_MachineCoordinate.y + sy / 2;
        }
        m_canvs->setSizeXY(sx, sy, cx, cy);


        GraphicalInfo *info = new GraphicalInfo;
        info->PointLists.push_back(Point(0, 0) + cache->m_MachineCoordinate);
        info->PointLists.push_back(Point(0, mater.size_y) + cache->m_MachineCoordinate);
        info->PointLists.push_back(Point(mater.size_x, mater.size_y) + cache->m_MachineCoordinate);
        info->PointLists.push_back(Point(mater.size_x, 0) + cache->m_MachineCoordinate);
        info->isClose = true;
        info->color = Qt::red;

        m_canvs->addGraphicals(info);
    }
    else
    {
        double maxX = 0;
        double maxY = 0;
        double minX = 0;
        double minY = 0;
        for(int i = 0; i < actions->size(); i++)
        {
            if(actions->at(i)->m_type == ACTION_PUNCH_TYPE || actions->at(i)->m_type == ACTION_MOVE_TYPE)
            {
                maxX = maxX > ((MoveAction *)actions->at(i))->m_endPoint.x ? maxX : ((MoveAction *)actions->at(i))->m_endPoint.x;
                maxY = maxY > ((MoveAction *)actions->at(i))->m_endPoint.y ? maxY : ((MoveAction *)actions->at(i))->m_endPoint.y;
                minX = minX < ((MoveAction *)actions->at(i))->m_endPoint.x ? minX : ((MoveAction *)actions->at(i))->m_endPoint.x;
                minY = minY < ((MoveAction *)actions->at(i))->m_endPoint.y ? minY : ((MoveAction *)actions->at(i))->m_endPoint.y;
            }
        }

//        maxX = maxX > 0.1 ? maxX : (maxY > 0.1 ? maxY : 800);
//        maxY = maxY > 0.1 ? maxY : (maxX > 0.1 ? maxX : 600);
//        m_canvs->setSizeXY(maxX, maxY);
        double sizeX = maxX - minX;
        double sizeY = maxY - minY;
        double oriX = (maxX + minX) / 2;
        double oriY = (maxY + minY) / 2;
        sizeX = sizeX > 0.1 ? sizeX : 800;
        sizeY = sizeY > 0.1 ? sizeY : 600;
        m_canvs->setSizeXY(sizeX, sizeY, oriX, oriY);
    }*/

    return 0;
}
/*************************************************
  �������ƣ�drawBackgroundActions(QVector<BaseAction *>*  actions, ErrorInfo& e)
  �������ܣ����Ƽӹ��ļ��ı���ͼ��
  ���������actions   BaseAction�б�
  ���������e  ��¼������Ϣ
  �� �� ֵ��DONE_SUCCESS �ɹ�
           DONE_FAIL  ʧ��
  ��    ע��
**************************************************/
bool EmulationControllerDevice::drawBackgroundActions(QVector<BaseAction *>*  actions, ErrorInfo& e)
{
   /* for(int i = 0; i < actions->size(); i++)
    {
        if(actions->at(i)->m_type == ACTION_PUNCH_TYPE && ((PunchAction *)actions->at(i))->m_nPunchType == PUNCH_BACKTO_PREPUNCH)
        {
            m_canvs->addGraphicals(drawPunchAction(((PunchAction *)actions->at(i)), Qt::DotLine, Qt::yellow, e));
        }
    }
//    m_canvs->update();*/

    return true;
}

/*************************************************
  �������ƣ�drawOneActions(BaseAction *  action, ErrorInfo& e)
  �������ܣ�����һ��Actionsͼ��
  ���������action ��¼��ѹ���ƶ��ȶ����Ľṹ��
  ���������e  ��¼������Ϣ
  �� �� ֵ��DONE_SUCCESS �ɹ�
           DONE_FAIL  ʧ��
  ��    ע��
**************************************************/
bool EmulationControllerDevice::drawOneActions(BaseAction *  action, ErrorInfo& e)
{
    //��action���
   /* if(action->m_type == ACTION_MOVE_TYPE)
    {
        m_canvs->addGraphicals(drawMoveAction(((MoveAction *)action), Qt::DotLine, Qt::red, e));
    }
    else if(action->m_type == ACTION_PUNCH_TYPE && ((PunchAction *)action)->m_nPunchType == PUNCH_BACKTO_PREPUNCH)
    {
        m_canvs->addGraphicals(drawMoveAction(((MoveAction *)action), Qt::DotLine, Qt::red, e));
        m_canvs->addGraphicals(drawPunchAction(((PunchAction *)action), Qt::SolidLine, Qt::red, e));
    }
//    else if(action->m_type == ACTION_TOOL_CHANGE_TYPE)
//    {
//        //m_canvs->addGraphicals(drawToolChangeAction(((ToolChangeAction *)action), Qt::DotLine, Qt::yellow, e));
//    }
//    else if(action->m_type == ACTION_TOOL_ROTATE_TYPE)
//    {
//        //m_canvs->addGraphicals(drawToolRotateAction(((ToolRotateAction *)action), Qt::DotLine, Qt::yellow, e));
//    }

    //������ǰִ�е���λ��
    drawCarrierGraphical(action);*/
//    m_canvs->update();

    return true;
}

/*************************************************
  �������ƣ�drawOneActionsWithNoCarrier(BaseAction *  action, ErrorInfo& e)
  �������ܣ�����һ��Actionsͼ����������
  ���������action ��¼��ѹ���ƶ��ȶ����Ľṹ��
  ���������e  ��¼������Ϣ
  �� �� ֵ��DONE_SUCCESS �ɹ�
           DONE_FAIL  ʧ��
  ��    ע��
**************************************************/
bool EmulationControllerDevice::drawOneActionsWithNoCarrier(BaseAction *  action, ErrorInfo& e)
{
    /*if(action->m_type == ACTION_MOVE_TYPE)
    {
        m_canvs->addGraphicals(drawMoveAction(((MoveAction *)action), Qt::DotLine, Qt::red, e));
    }
    else if(action->m_type == ACTION_PUNCH_TYPE && ((PunchAction *)action)->m_nPunchType == PUNCH_BACKTO_PREPUNCH)
    {
        m_canvs->addGraphicals(drawMoveAction(((MoveAction *)action), Qt::DotLine, Qt::red, e));
        m_canvs->addGraphicals(drawPunchAction(((PunchAction *)action), Qt::SolidLine, Qt::red, e));
    }
//    else if(action->m_type == ACTION_TOOL_CHANGE_TYPE)
//    {
//        qDebug()<<"action err tc";
//        //m_canvs->addGraphicals(drawToolChangeAction(((ToolChangeAction *)action), Qt::DotLine, Qt::yellow, e));
//    }
//    else if(action->m_type == ACTION_TOOL_ROTATE_TYPE)
//    {
//        qDebug()<<"action err tr";
//        //m_canvs->addGraphicals(drawToolRotateAction(((ToolRotateAction *)action), Qt::DotLine, Qt::yellow, e));
//    }
//    else
//    {
//        qDebug()<<"action err!";
//        return false;
//    }
//    m_canvs->update();*/
    return true;
}

/*************************************************
  �������ƣ�drawCarrierGraphical(BaseAction *act)
  �������ܣ����Ƶ���ͼ��
  ���������action ��¼��ѹ���ƶ��ȶ����Ľṹ��
  ���������
  �� �� ֵ��
  ��    ע��
**************************************************/
void EmulationControllerDevice::drawCarrierGraphical(BaseAction *act)
{
    /*if (act->m_type == ACTION_MOVE_TYPE)
    {
        drawCarrierGraphical(NULL, act->m_basePoint, CARRIER_RADIUS_SIZE, CARRIER_CROSS_SIZE, act->m_dAngle);
    }
    else if (act->m_type == ACTION_PUNCH_TYPE && ((PunchAction *)act)->m_nPunchType == PUNCH_BACKTO_PREPUNCH)
    {
        drawCarrierGraphical(act->m_tool, act->m_basePoint, CARRIER_RADIUS_SIZE, CARRIER_CROSS_SIZE, act->m_dAngle);
    }*/
}

/*************************************************
  �������ƣ�removeCarrierGraphical()
  �������ܣ������ǰ����ͼ��
  ���������
  ���������
  �� �� ֵ��
  ��    ע��
**************************************************/
void EmulationControllerDevice::removeCarrierGraphical()
{
    //m_canvs->clearCarrierGraphical();
//    m_canvs->update();
}

/*************************************************
  �������ƣ�drawCarrierGraphical(ToolInfo* tool, Point p, double r, double len, double a)
  �������ܣ����Ƶ���ͼ��
  ���������tool ��ǰ������Ϣ
          p ��ǰλ��
          r ���߰뾶
          len  ����ʮ�ּܳ���
          a ���߽Ƕ�
  ���������
  �� �� ֵ��
  ��    ע��
**************************************************/
void EmulationControllerDevice::drawCarrierGraphical(ToolInfo* tool, Point p, double r, double len, double a)
{
//    m_canvs->updataCarrierInfo(drawCircle(p ,r),drawHorizontalCross(p ,len),drawVerticalCross(p ,len),p + Point(TEXT_OFFSET_BASEPOS, TEXT_OFFSET_BASEPOS),"(" + QString::number(p.x) + "," + QString::number(p.y) + ")");
     //m_canvs->updataCarrierInfo(p, r, len, p + Point(TEXT_OFFSET_BASEPOS, TEXT_OFFSET_BASEPOS), "(" + QString::number(p.x) + "," + QString::number(p.y) + ")");
     //m_canvs->updataCarrierInfo(p, 5, 15);
}

void EmulationControllerDevice::drawCarrierGraphical(Point p)
{
   // return;
//    m_canvs->updataCarrierInfo(drawCircle(p ,r),drawHorizontalCross(p ,len),drawVerticalCross(p ,len),p + Point(TEXT_OFFSET_BASEPOS, TEXT_OFFSET_BASEPOS),"(" + QString::number(p.x) + "," + QString::number(p.y) + ")");
     //m_canvs->updataCarrierInfo(p, r, len, p + Point(TEXT_OFFSET_BASEPOS, TEXT_OFFSET_BASEPOS), "(" + QString::number(p.x) + "," + QString::number(p.y) + ")");
     //m_canvs->updataCarrierInfo(p, 5, 15);
}

/*************************************************
  �������ƣ�drawCarrierGraphical(ToolInfo* tool, Point p, double a)
  �������ܣ����Ƶ���ͼ��
  ���������tool ��ǰ������Ϣ
          p ��ǰλ��
          a ���߽Ƕ�
  ���������
  �� �� ֵ��
  ��    ע��
**************************************************/
void EmulationControllerDevice::drawCarrierGraphical(ToolInfo* tool, Point p, double a)
{
    //return;
    //drawCarrierGraphical(tool, p, CARRIER_RADIUS_SIZE, CARRIER_CROSS_SIZE, a);
}

/*************************************************
  �������ƣ�drawHorizontalCross(Point p, double len)
  �������ܣ����Ƶ����е�ʮ�ּ��еĺ���
  ���������p ��ǰλ��
          len  ����ʮ�ּܳ���
  ���������
  �� �� ֵ��
  ��    ע��
**************************************************/
GraphicalInfo* EmulationControllerDevice::drawHorizontalCross(Point p, double len)
{
    GraphicalInfo* info = new GraphicalInfo;
    info->color = Qt::white;
    info->style = Qt::SolidLine;
    info->isClose = true;

    info->PointLists.push_back(Point((p.x - len), p.y,0,0,0,0));
    info->PointLists.push_back(Point((p.x + len), p.y,0,0,0,0));
    return  info;
}

/*************************************************
  �������ƣ�drawVerticalCross(Point p, double len)
  �������ܣ����Ƶ����е�ʮ�ּ��е�����
  ���������p ��ǰλ��
          len  ����ʮ�ּܳ���
  ���������
  �� �� ֵ��
  ��    ע��
**************************************************/
GraphicalInfo* EmulationControllerDevice::drawVerticalCross(Point p, double len)
{
    GraphicalInfo* info = new GraphicalInfo;
    info->color = Qt::white;
    info->style = Qt::SolidLine;
    info->isClose = true;

    info->PointLists.push_back(Point(p.x, (p.y - len),0,0,0,0));
    info->PointLists.push_back(Point(p.x, (p.y + len),0,0,0,0));
    return  info;
}

/*************************************************
  �������ƣ�drawTool(ToolInfo* tool, Point p, double a)
  �������ܣ�������ͼ����ϳ�С�߶Σ��Թ�canvas���л�ͼ��ʾ
  ���������tool ��ǰ������Ϣ
          p ��ǰλ��
          a ���߽Ƕ�
  ���������
  �� �� ֵ��canvas��ʶ���ͼ����Ϣ
  ��    ע��
**************************************************/
GraphicalInfo* EmulationControllerDevice::drawTool(ToolInfo* tool, Point p, double a)
{
    GraphicalInfo* info = new GraphicalInfo;
    info->color = Qt::white;
    info->style = Qt::SolidLine;
    if(tool != NULL)
        tool->breakupGraph(info->PointLists, p, a);
    return  info;
}

/*************************************************
  �������ƣ�drawCircle(Point p, double r)
  �������ܣ���Բ��ͼ����ϳ�С�߶Σ��Թ�canvas���л�ͼ��ʾ
  ���������p ��ǰλ��
          r  �뾶
  ���������
  �� �� ֵ��canvas��ʶ���ͼ����Ϣ
  ��    ע��
**************************************************/
GraphicalInfo* EmulationControllerDevice::drawCircle(Point p, double r)
{
    GraphicalInfo* info = new GraphicalInfo;
    info->color = Qt::white;
    info->style = Qt::SolidLine;
    info->isClose = true;

    double t = 2 * PI / CIRCLE_SEGMENTATION_NUM;

    for(int i = 0; i < CIRCLE_SEGMENTATION_NUM; i++)
    {
        info->PointLists.push_back(Point(r * cos(t * i), r * sin(t * i),0,0,0,0) + p);
    }

    return  info;
}

/*************************************************
  �������ƣ�drawMoveAction(MoveAction *a, Qt::PenStyle s, QColor c, ErrorInfo& e)
  �������ܣ������ƶ���Action
  ���������a Action
          s ��ͼ���õ�PenStyle
          c ��ͼ����ɫ
  ���������e ������Ϣ
  �� �� ֵ��canvas��ʶ���ͼ����Ϣ
  ��    ע��
**************************************************/
GraphicalInfo* EmulationControllerDevice::drawMoveAction(MoveAction *a, Qt::PenStyle s, QColor c, ErrorInfo& e)
{
    GraphicalInfo*  info = new GraphicalInfo;

    info->isClose = false;
    info->color = c;
    info->style = s;
    info->PointLists.push_back(a->m_startPoint);
    info->PointLists.push_back(a->m_endPoint);

    return  info;
}

/*************************************************
  �������ƣ�drawPunchAction(PunchAction *a, Qt::PenStyle s, QColor c, ErrorInfo& e)
  �������ܣ����Ƴ�ѹ��Action
  ���������a Action
          s ��ͼ���õ�PenStyle
          c ��ͼ����ɫ
  ���������e ������Ϣ
  �� �� ֵ��canvas��ʶ���ͼ����Ϣ
  ��    ע��
**************************************************/
GraphicalInfo* EmulationControllerDevice::drawPunchAction(PunchAction *a, Qt::PenStyle s, QColor c, ErrorInfo& e)
{
    if(a->m_tool != NULL)
    {
        GraphicalInfo*  info = new GraphicalInfo;
        info->color = c;
        info->style = s;
        a->m_tool->breakupGraph(info->PointLists, a->m_basePoint, a->m_dAngle);
        return  info;
    }
    else
    {
        e.insertDebug(DEBUG_SERIOUS_ERROR, "EmulationControllerDevice no find toolinfo!");
        return NULL;
    }

}

/*************************************************
  �������ƣ�drawToolChangeAction(ToolChangeAction *a, Qt::PenStyle s, QColor c, ErrorInfo& e)
  �������ܣ����ƻ�����Action
  ���������a Action
          s ��ͼ���õ�PenStyle
          c ��ͼ����ɫ
  ���������e ������Ϣ
  �� �� ֵ��canvas��ʶ���ͼ����Ϣ
  ��    ע��
**************************************************/
GraphicalInfo* EmulationControllerDevice::drawToolChangeAction(ToolChangeAction *a, Qt::PenStyle s, QColor c, ErrorInfo& e)
{
    if(a->m_newTool != NULL)
    {
        GraphicalInfo*  info = new GraphicalInfo;
        info->color = c;
        info->style = s;
        a->m_newTool->breakupGraph(info->PointLists, a->m_basePoint, a->m_dAngle);
        return  info;
    }
    else
    {
        e.insertDebug(DEBUG_SERIOUS_ERROR, "EmulationControllerDevice no find toolinfo!");
        return NULL;
    }
}

/*************************************************
  �������ƣ�drawToolRotateAction(ToolRotateAction *a, Qt::PenStyle s, QColor c, ErrorInfo& e)
  �������ܣ����Ƶ�����ת��Action
  ���������a Action
          s ��ͼ���õ�PenStyle
          c ��ͼ����ɫ
  ���������e ������Ϣ
  �� �� ֵ��canvas��ʶ���ͼ����Ϣ
  ��    ע��
**************************************************/
GraphicalInfo* EmulationControllerDevice::drawToolRotateAction(ToolRotateAction *a, Qt::PenStyle s, QColor c, ErrorInfo& e)
{
    if(a->m_tool != NULL)
    {
        GraphicalInfo*  info = new GraphicalInfo;
        info->color = c;
        info->style = s;
        a->m_tool->breakupGraph(info->PointLists, a->m_basePoint, a->m_dAngle);
        return  info;
    }
    else
    {
        e.insertDebug(DEBUG_SERIOUS_ERROR, "EmulationControllerDevice no find toolinfo!");
        return NULL;
    }
}

/*************************************************
  �������ƣ�getSimulationUiHandle()
  �������ܣ���ȡ�������UI��ָ��
  ���������
  ���������
  �� �� ֵ�� �������UI��ָ��
  ��    ע��
**************************************************/
CMachiningSimulationUi* EmulationControllerDevice::getSimulationUiHandle()
{
    return m_pSimulationUI;
}

/*************************************************
  �������ƣ�setSimulationStateUi(CMachiningSimulationUi* ui)
  �������ܣ����÷������UI
  ���������ui �������UI��ָ��
  ���������
  �� �� ֵ��
  ��    ע��
**************************************************/
void EmulationControllerDevice::setSimulationStateUi(CMachiningSimulationUi* ui)
{
    m_pSimulationUI = ui;
}







