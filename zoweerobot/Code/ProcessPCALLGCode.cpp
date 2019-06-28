/************************************************************
  Copyright (C),
  文件名：ProcessPCALLGCode.cpp
  作者：zhou    版本：V1.0    创建日期：2018/07/10
  模块描述：ProcessPCALLGCode类的实现
  历史修改记录：<author>    <time>    <version >    <desc>
                  zhou    2018/07/10     V1.0      添加注释
*************************************************************/

#include "ProcessPCALLGCode.h"
#include "ParseGCodeModel.h"

ProcessPCALLGCode::ProcessPCALLGCode()
{
    for(int i = 0; i < PCALL_LOCAL_VARIABLE; i++)
        m_localVariable[i] = -1;
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

bool ProcessPCALLGCode::fillGCodeVarient(ParseGCodeInterVariableCache*  Cache, GCodeParseInfo& inf, ErrorInfo& e)
{
    ProcessCALLGCode::fillGCodeVarient(Cache, inf, e);
    return fillLocalVariable(Cache, inf, e);
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
bool ProcessPCALLGCode::fillLocalVariable(ParseGCodeInterVariableCache*  Cache, GCodeParseInfo& inf, ErrorInfo& e)
{
    int tt = -1;
    if(inf.m_info.size() > 1)
    {
        for(int i = 1; i < inf.m_info.size(); i++)
        {
            if(inf.m_info[i].symbol.size() == 1)
            {
                if(matchLocalVariable(inf.m_info[i].symbol[0].toLatin1(), tt, e))
                {
                    if(checkAllDigital(inf.m_info[i].value))
                    {
                        m_localVariable[tt] = inf.m_info[i].value.toInt();
                        return DONE_SUCCESS;
                    }
                    else
                    {
                        e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, "filllocalVariable value no all num" + inf.m_info[i].value);
                        return DONE_FAIL;
                    }
                }

                return DONE_FAIL;
            }
            else
            {
                e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, "filllocalVariable no support:" + QString::number(m_LineSerialNum) + " :" + inf.m_info[i].symbol);
                return DONE_FAIL;
            }
        }
    }
    return DONE_SUCCESS;
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
bool  ProcessPCALLGCode::matchLocalVariable(char c, int& t, ErrorInfo& e)
{
    switch(c)
    {
        case 'R':
        case 'r':
        {
            t = PCALL_R_PATH_LENGTH_OR_RADIUS;
            return DONE_SUCCESS;
        }
        case 'Q':
        case 'q':
        {
            t = PCALL_R_PATH_LENGTH_OR_RADIUS;
            return DONE_SUCCESS;
        }
        case 'N':
        case 'n':
        {
            t = PCALL_R_PATH_LENGTH_OR_RADIUS;
            return DONE_SUCCESS;
        }
        case 'T':
        case 't':
        {
            t = PCALL_R_PATH_LENGTH_OR_RADIUS;
            return DONE_SUCCESS;
        }
        default:
        {
            e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, "filllocalVariable no support:" + c);
            return DONE_FAIL;
        }
    }
}


