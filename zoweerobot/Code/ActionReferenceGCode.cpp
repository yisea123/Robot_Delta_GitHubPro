/************************************************************
  Copyright (C),
  文件名：ActionReferenceGCode.cpp
  作者：zhou    版本：V1.0    创建日期：2018/07/10
  模块描述：ActionReferenceGCode类的实现
  历史修改记录：<author>    <time>    <version >    <desc>
                  zhou    2018/07/10     V1.0      添加注释
*************************************************************/

#include "ActionReferenceGCode.h"
#include "ParseGCodeModel.h"


ActionReferenceGCode::ActionReferenceGCode()
{
//    m_isCoordinateReferenceAssign = FALSE;
    m_CoordinateReference = GCODE_REFERENCE_NULL;
    m_isReferencePositionIAssign = FALSE;
    m_isReferencePositionJAssign = FALSE;

    m_bIsAbsCoordinate = ABSOLUTE_COORDINATE_REFERENCE;

    m_GCodeType = GCODE_REFERENCE_TYPE;
}

/*************************************************
  函数名称：fillGCodeVarient(ParseGCodeInterVariableCache*  Cache, GCodeParseInfo& inf, ErrorInfo& e)
  函数功能：对类里面的变量进行赋值
  输入参数： Cache  解析时临时变量存储数据
  输出参数： inf   一行G代码分割后存储数据
           e     错误代码
  返 回 值：  DONE_SUCCESS 成功
            DONE_FAIL  失败
  备    注：
**************************************************/
bool ActionReferenceGCode::fillGCodeVarient(ParseGCodeInterVariableCache*  Cache, GCodeParseInfo& inf, ErrorInfo& e)
{
    QString ss;
    for(int i = 0; i < inf.m_info.size(); i++)
    {
        ss = inf.m_info.at(i).symbol;
        if(ss == "G" || ss == "g")
        {
            switch(inf.m_info.at(i).value.toInt())
            {
                case 90:
                {
                    Cache->m_isSelectCoordinateReference = TRUE;
                //    m_isCoordinateReferenceAssign = TRUE;
                    //m_bIsAbsCoordinate = ABSOLUTE_COORDINATE_REFERENCE;
                    Cache->m_bIsAbsCoordinate = ABSOLUTE_COORDINATE_REFERENCE;
                    m_CoordinateReference = GCODE_G90;
                }break;
                case 91:
                {
                    Cache->m_isSelectCoordinateReference = TRUE;
                //    m_isCoordinateReferenceAssign = TRUE;
                    //m_bIsAbsCoordinate = RELATIVE_COORDINATE_REFERENCE;
                    Cache->m_bIsAbsCoordinate = RELATIVE_COORDINATE_REFERENCE;
                    m_CoordinateReference = GCODE_G91;
                }break;

                default:
                {
                    e.insertDebug(DEBUG_PARAMETER_ERROR, "fillGCodeVarient G error:" + inf.m_info.at(i).value);
                }break;
            }
        }
//        else if((ss == "I") || (ss == "X"))
//        {
//            m_ReferencePosition.x = inf.m_info.at(i).value.toInt();
//            m_isReferencePositionIAssign = TRUE;
//        }
//        else  if((ss == "J") || (ss == "Y"))
//        {
//            m_ReferencePosition.y = inf.m_info.at(i).value.toInt();
//            m_isReferencePositionJAssign = TRUE;
//        }
    }

    m_bIsAbsCoordinate = Cache->m_bIsAbsCoordinate;

    return   BaseGCode::fillGCodeVarient(Cache, inf, e);
}

bool  ActionReferenceGCode::isReferencePositionIAssign()
{
    return m_isReferencePositionIAssign;
}
bool  ActionReferenceGCode::isReferencePositionJAssign()
{
    return m_isReferencePositionJAssign;
}
GCodePosition  ActionReferenceGCode::getReferencePosition()
{
    return m_ReferencePosition;
}

double ActionReferenceGCode::getReferencePositionX()
{
    return m_ReferencePosition.x;
}
double ActionReferenceGCode::getReferencePositionY()
{
    return m_ReferencePosition.y;
}

bool  ActionReferenceGCode::isAbsCoordinate()
{
    return m_bIsAbsCoordinate;
}

bool ActionReferenceGCode::genrateAction(EmulationInterVariableCache* Cache, QVector<BaseAction *>& actions,ErrorInfo& e)
{
//    if(m_CoordinateReference != GCODE_REFERENCE_NULL)
//    {
//        Cache->m_bIsAbsCoordinate = m_bIsAbsCoordinate;
//        Cache->m_ReferenceCoordinate.x = m_ReferencePosition.x;
//        Cache->m_ReferenceCoordinate.y = m_ReferencePosition.y;
//    }
//    m_ReferencePosition.x = Cache->m_ReferenceCoordinate.x;
//    m_ReferencePosition.y = Cache->m_ReferenceCoordinate.y;

    if(m_bIsAbsCoordinate)
    {
        Cache->m_bIsAbsCoordinate = ABSOLUTE_COORDINATE_REFERENCE;

        /*if(m_isReferencePositionIAssign)
        {
            Cache->m_ReferenceCoordinate.x = m_ReferencePosition.x;
        }
        if(m_isReferencePositionJAssign)
        {
            Cache->m_ReferenceCoordinate.y = m_ReferencePosition.y;
        }*/
    }
    else
    {
        Cache->m_bIsAbsCoordinate = RELATIVE_COORDINATE_REFERENCE;

        /*if(m_isReferencePositionIAssign)
        {
            Cache->m_ReferenceCoordinate.x = m_ReferencePosition.x;
        }
        if(m_isReferencePositionJAssign)
        {
            Cache->m_ReferenceCoordinate.y = m_ReferencePosition.y;
        }*/
    }

    return true;
}

QString  ActionReferenceGCode::toDebugString()
{
    QString ss;

    ss += "参考坐标系代码：\n";
    ss += "参考坐标系类型：";
    ss += GCodeReferenceString[m_CoordinateReference];

    ss += ",X：";
    if(m_isReferencePositionIAssign)
    {
        ss += ss.number(m_ReferencePosition.x);
    }
    else
    {
        ss += "NULL";
    }

    ss += ",Y：";
    if(m_isReferencePositionJAssign)
    {
        ss += ss.number(m_ReferencePosition.y);
    }
    else
    {
        ss += "NULL";
    }

    ss += '\n';

    return BaseGCode::toDebugString() + ss;
}
