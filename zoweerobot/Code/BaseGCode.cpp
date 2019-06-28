/************************************************************
  Copyright (C),
  文件名：BaseGCode.cpp
  作者：zhou   版本：V1.0    创建日期：2018/07/10
  模块描述：BaseGCode类的实现
  历史修改记录：<author>    <time>    <version >    <desc>
                  zhou   2018/07/10     V1.0      添加注释
*************************************************************/

#include "BaseGCode.h"
#include "ComonTypes.h"

#include "ParseGCodeModel.h"

BaseGCode::BaseGCode()
{
    m_LineSerialNum = -1;
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
bool BaseGCode::fillGCodeVarient(ParseGCodeInterVariableCache*  Cache, GCodeParseInfo& inf, ErrorInfo& e)
{
    m_LineSerialNum = inf.m_SerialNumber;   

    return TRUE;
}

bool BaseGCode::genrateAction(EmulationInterVariableCache* Cache, QVector<BaseAction *>& actions,ErrorInfo& e)
{
    return TRUE;
}

QString  BaseGCode::toDebugString()
{
    QString  ss;

    ss += "\n";
    ss += ss.number(m_LineSerialNum);
    ss += "\n";
    ss += "BASE：\n";
    ss += "BASEGCODE类型：";
    ss += GCodeString[m_GCodeType];
    ss += ",G代码行号(N)：";
    ss += ss.number(m_NGSerialNum);
    ss += "\n";

    return ss;
}
