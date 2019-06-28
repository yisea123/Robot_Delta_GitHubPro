/************************************************************
  Copyright (C),
  文件名：BaseDevice.cpp
  作者：zhou    版本：V1.0    创建日期：2018/07/10
  模块描述：BaseDevice类的实现
  历史修改记录：<author>    <time>    <version >    <desc>
                  zhou    2018/07/10     V1.0      添加注释
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
  函数名称：openDevice(ErrorInfo& e)
  函数功能：打开设备
  输入参数：
  输出参数：e  错误信息
  返 回 值：int 未定义
  备    注：
**************************************************/
int EmulationControllerDevice::openDevice(ErrorInfo& e)
{
    return  initDevice(e);
}

/*************************************************
  函数名称：initDevice(ErrorInfo& e)
  函数功能：设备初始化
  输入参数：
  输出参数： e  错误信息
  返 回 值： 0   成功
           非0  失败
  备    注：
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
  函数名称：setCanvsHandle(CEmulationCanvs* c)
  函数功能：停止设备
  输入参数：
  输出参数：
  返 回 值：0   成功
           非0  失败
  备    注：
**************************************************/
void EmulationControllerDevice::setCanvsHandle(CEmulationCanvs* c)
{
    //m_canvs = c;
}

/*************************************************
  函数名称：clearCanvsData()
  函数功能：停止设备
  输入参数：
  输出参数：
  返 回 值：0   成功
           非0  失败
  备    注：
**************************************************/
void EmulationControllerDevice::clearCanvsData()
{
    //Q_CHECK_PTR(m_canvs);
    //m_canvs->clearGraphicals();
}

/*************************************************
  函数名称：startDevice(ErrorInfo& e)
  函数功能：启动设备
  输入参数：
  输出参数：
  返 回 值：0   成功
           非0  失败
  备    注：
**************************************************/
int EmulationControllerDevice::startDevice(ErrorInfo& e)
{
    //Q_CHECK_PTR(m_canvs);
    //m_canvs->enableDrawFunction();

    return 0;
}

/*************************************************
  函数名称：restartDevice(ErrorInfo& e)
  函数功能：重启设备
  输入参数：
  输出参数：
  返 回 值：0   成功
           非0  失败
  备    注：
**************************************************/
int EmulationControllerDevice::restartDevice(ErrorInfo& e)
{
    //Q_CHECK_PTR(m_canvs);
    //m_canvs->init();

    return 0;
}

/*************************************************
  函数名称：pauseDevice(ErrorInfo& e)
  函数功能：暂停设备
  输入参数：
  输出参数：
  返 回 值：0   成功
           非0  失败
  备    注：
**************************************************/
int EmulationControllerDevice::pauseDevice(ErrorInfo& e)
{
    //m_canvs->disableDrawFunction();

    return 0;
}

/*************************************************
  函数名称：stopDevice(ErrorInfo& e)
  函数功能：停止设备
  输入参数：
  输出参数：
  返 回 值：0   成功
           非0  失败
  备    注：
**************************************************/
int EmulationControllerDevice::stopDevice(ErrorInfo& e)
{
    //m_canvs->disableDrawFunction();

    return 0;
}

/*************************************************
  函数名称：drawBackground(Material mater, QVector<BaseAction *>*  actions, EmulationInterVariableCache* cache, ErrorInfo& e)
  函数功能：绘制加工文件的背景图案以及边框
  输入参数：mater 加工文件解析后的头部信息
          actions   BaseAction列表
          cache  中间变量缓存
  输出参数：e  记录错误信息
  返 回 值： 0   成功
           非0  失败
  备    注：
**************************************************/
int EmulationControllerDevice::drawBackground(Material mater, QVector<BaseAction *>*  actions, EmulationInterVariableCache* cache, ErrorInfo& e)
{
    //drawDesk(mater, cache);
    //drawDesk(mater, cache, actions);
    //drawBackgroundActions(actions, e);

    return 0;
}

/*************************************************
  函数名称：drawDesk(Material mater, EmulationInterVariableCache* cache)
  函数功能：绘制加工边框
  输入参数：mater 加工文件解析后的头部信息
          cache  中间变量缓存
  输出参数：
  返 回 值：0   成功
           非0  失败
  备    注：
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
  函数名称：drawBackgroundActions(QVector<BaseAction *>*  actions, ErrorInfo& e)
  函数功能：绘制加工文件的背景图案
  输入参数：actions   BaseAction列表
  输出参数：e  记录错误信息
  返 回 值：DONE_SUCCESS 成功
           DONE_FAIL  失败
  备    注：
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
  函数名称：drawOneActions(BaseAction *  action, ErrorInfo& e)
  函数功能：绘制一条Actions图案
  输入参数：action 记录冲压、移动等动作的结构体
  输出参数：e  记录错误信息
  返 回 值：DONE_SUCCESS 成功
           DONE_FAIL  失败
  备    注：
**************************************************/
bool EmulationControllerDevice::drawOneActions(BaseAction *  action, ErrorInfo& e)
{
    //将action变红
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

    //画出当前执行刀架位置
    drawCarrierGraphical(action);*/
//    m_canvs->update();

    return true;
}

/*************************************************
  函数名称：drawOneActionsWithNoCarrier(BaseAction *  action, ErrorInfo& e)
  函数功能：绘制一条Actions图案不带刀架
  输入参数：action 记录冲压、移动等动作的结构体
  输出参数：e  记录错误信息
  返 回 值：DONE_SUCCESS 成功
           DONE_FAIL  失败
  备    注：
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
  函数名称：drawCarrierGraphical(BaseAction *act)
  函数功能：绘制刀架图案
  输入参数：action 记录冲压、移动等动作的结构体
  输出参数：
  返 回 值：
  备    注：
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
  函数名称：removeCarrierGraphical()
  函数功能：清楚当前刀架图案
  输入参数：
  输出参数：
  返 回 值：
  备    注：
**************************************************/
void EmulationControllerDevice::removeCarrierGraphical()
{
    //m_canvs->clearCarrierGraphical();
//    m_canvs->update();
}

/*************************************************
  函数名称：drawCarrierGraphical(ToolInfo* tool, Point p, double r, double len, double a)
  函数功能：绘制刀架图案
  输入参数：tool 当前刀具信息
          p 当前位置
          r 刀具半径
          len  刀具十字架长度
          a 刀具角度
  输出参数：
  返 回 值：
  备    注：
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
  函数名称：drawCarrierGraphical(ToolInfo* tool, Point p, double a)
  函数功能：绘制刀架图案
  输入参数：tool 当前刀具信息
          p 当前位置
          a 刀具角度
  输出参数：
  返 回 值：
  备    注：
**************************************************/
void EmulationControllerDevice::drawCarrierGraphical(ToolInfo* tool, Point p, double a)
{
    //return;
    //drawCarrierGraphical(tool, p, CARRIER_RADIUS_SIZE, CARRIER_CROSS_SIZE, a);
}

/*************************************************
  函数名称：drawHorizontalCross(Point p, double len)
  函数功能：绘制刀架中的十字架中的横线
  输入参数：p 当前位置
          len  刀具十字架长度
  输出参数：
  返 回 值：
  备    注：
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
  函数名称：drawVerticalCross(Point p, double len)
  函数功能：绘制刀架中的十字架中的竖线
  输入参数：p 当前位置
          len  刀具十字架长度
  输出参数：
  返 回 值：
  备    注：
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
  函数名称：drawTool(ToolInfo* tool, Point p, double a)
  函数功能：将刀具图案打断成小线段，以供canvas进行绘图显示
  输入参数：tool 当前刀具信息
          p 当前位置
          a 刀具角度
  输出参数：
  返 回 值：canvas可识别的图形信息
  备    注：
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
  函数名称：drawCircle(Point p, double r)
  函数功能：将圆形图案打断成小线段，以供canvas进行绘图显示
  输入参数：p 当前位置
          r  半径
  输出参数：
  返 回 值：canvas可识别的图形信息
  备    注：
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
  函数名称：drawMoveAction(MoveAction *a, Qt::PenStyle s, QColor c, ErrorInfo& e)
  函数功能：绘制移动的Action
  输入参数：a Action
          s 绘图采用的PenStyle
          c 绘图的颜色
  输出参数：e 错误信息
  返 回 值：canvas可识别的图形信息
  备    注：
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
  函数名称：drawPunchAction(PunchAction *a, Qt::PenStyle s, QColor c, ErrorInfo& e)
  函数功能：绘制冲压的Action
  输入参数：a Action
          s 绘图采用的PenStyle
          c 绘图的颜色
  输出参数：e 错误信息
  返 回 值：canvas可识别的图形信息
  备    注：
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
  函数名称：drawToolChangeAction(ToolChangeAction *a, Qt::PenStyle s, QColor c, ErrorInfo& e)
  函数功能：绘制换刀的Action
  输入参数：a Action
          s 绘图采用的PenStyle
          c 绘图的颜色
  输出参数：e 错误信息
  返 回 值：canvas可识别的图形信息
  备    注：
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
  函数名称：drawToolRotateAction(ToolRotateAction *a, Qt::PenStyle s, QColor c, ErrorInfo& e)
  函数功能：绘制刀具旋转的Action
  输入参数：a Action
          s 绘图采用的PenStyle
          c 绘图的颜色
  输出参数：e 错误信息
  返 回 值：canvas可识别的图形信息
  备    注：
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
  函数名称：getSimulationUiHandle()
  函数功能：获取仿真界面UI的指针
  输入参数：
  输出参数：
  返 回 值： 仿真界面UI的指针
  备    注：
**************************************************/
CMachiningSimulationUi* EmulationControllerDevice::getSimulationUiHandle()
{
    return m_pSimulationUI;
}

/*************************************************
  函数名称：setSimulationStateUi(CMachiningSimulationUi* ui)
  函数功能：设置仿真界面UI
  输入参数：ui 仿真界面UI的指针
  输出参数：
  返 回 值：
  备    注：
**************************************************/
void EmulationControllerDevice::setSimulationStateUi(CMachiningSimulationUi* ui)
{
    m_pSimulationUI = ui;
}







