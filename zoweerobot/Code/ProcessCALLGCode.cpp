/************************************************************
  Copyright (C),
  文件名：ProcessCALLGCode.cpp
  作者：zhou    版本：V1.0    创建日期：2018/07/10
  模块描述：ProcessCALLGCode类的实现
  历史修改记录：<author>    <time>    <version >    <desc>
                  zhou    2018/07/10     V1.0      添加注释
*************************************************************/


#include "ProcessCALLGCode.h"
#include "ParseGCodeModel.h"
#include "ComonTypes.h"
#include "GCodeComonType.h"

ProcessCALLGCode::ProcessCALLGCode()
{
    m_callType = -1;
    m_GCodeType = GCODE_CALL_TYPE;
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
bool ProcessCALLGCode::fillGCodeVarient(ParseGCodeInterVariableCache*  Cache, GCodeParseInfo& inf, ErrorInfo& e)
{

    QString  ss = inf.m_textStr[1];
    QStringList sss = ss.split(',');

    inf.clear();
    LineContextInfo f;
    foreach(QString str, sss)
    {
        if(str.size() > 0)
        {
            int i = findQStringFirstDigitPos(str);

            if(i == -1)
            {
                f.symbol = str;
                f.value.clear();
            }
            else
            {
                f.symbol = str.left(i);
                f.value = str.mid(i);
                f.value.remove(' ');
            }

            f.symbol.remove(' ');

            inf.m_info.append(f);
        }
    }

    ss = inf.m_info[0].symbol;
    int p = ss.indexOf("CALL");

    ss = ss.mid(p);

    ss.remove(' ');
    if(checkAllDigital(ss))
    {
        m_callType = ss.toInt();
        return TRUE;
    }
    else
    {
        e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, "getCallType find no CallType");
        return FALSE;
    }

}


