/************************************************************
  Copyright (C),
  文件名：ActionPunchGCode.cpp
  作者：zhou    版本：V1.0    创建日期：2018/07/10
  模块描述：ActionPunchGCode类的实现
  历史修改记录：<author>    <time>    <version >    <desc>
                  zhou    2018/07/10    V1.0      添加注释
*************************************************************/

#include "ActionPunchGCode.h"
#include "BaseGCode.h"
#include "ParseGCodeModel.h"
#include "ActionMoveGCode.h"
#include "BaseAction.h"
#include "MoveAction.h"
#include "PunchAction.h"
#include "IOAction.h"
#include "ToolRotateAction.h"
#include "ToolChangeAction.h"

#include <QString>

//#define RADIANS(angle) ((angle)*PI/180)

/*************************************************
  函数名称：ActionPunchGCode()
  函数功能：构造函数
  输入参数：
  输出参数：
  返 回 值：
  备    注：
**************************************************/
ActionPunchGCode::ActionPunchGCode()
{
    m_GCodeType = GCODE_PUNCH_TYPE;

    m_nPunchG = GCODE_PUNCH_NULL;
    m_isPunchKAssign = FALSE;
    m_isPunchPAssign = FALSE;
    m_isPunchQAssign = FALSE;
    m_isPunchDAssign = FALSE;
    m_isPunchTAssign = FALSE;

    m_nT = -1;
}

/*************************************************
  函数名称：fillGCodeVarient(ParseGCodeInterVariableCache*  Cache, GCodeParseInfo& inf, ErrorInfo& e)
  函数功能：对类里面的变量进行赋值
  输入参数： Cache  解析时临时变量存储数据
  输出参数： inf   一行G代码分割后存储数据
           e     错误代码
  返 回 值：  TRUE 成功
            FALSE  失败
  备    注：
**************************************************/
bool  ActionPunchGCode::fillGCodeVarient(ParseGCodeInterVariableCache*  Cache, GCodeParseInfo& inf, ErrorInfo& e)
{
    QString s;

    /*用于语法错误检测，出现一次数值增加1*/
    int gFlag = 0;
    int pFlag = 0;
    int qFlag = 0;
    int kFlag = 0;
    int dFlag = 0;
    int iFlag = 0;
    int jFlag = 0;
    int tFlag = 0;

    m_nT = Cache->m_ToolLastSerialNum;//刀具缺省值与上一条代码相同

    if (inf.m_info.size() == 1 && (inf.m_info.at(0).symbol == "T" || inf.m_info.at(0).symbol == "t"))
    {
        m_nT = inf.m_info.at(0).value.toInt();
        if (Cache->m_ToolLastSerialNum != m_nT)
        {
            m_isPunchTAssign = TRUE;
            Cache->m_isSelectTool = TRUE;
            Cache->m_ToolLastSerialNum = m_nT;
            m_nPunchG = GCODE_PUNCH_IO;
        }
        return ActionMoveGCode::fillGCodeVarient(Cache, inf, e);
    }

    for(int i = 0; i < inf.m_info.size(); i++)
    {
        s = inf.m_info.at(i).symbol;
        if(s == "G" || s == "g")
        {
            switch(inf.m_info.at(i).value.toInt())
            {
                case 28:
                {
                    m_nPunchG = GCODE_G28;
                    inf.m_info.removeAt(i--);

                    gFlag++;
                }break;
                case 38:
                {
                    m_nPunchG = GCODE_G38;
                    inf.m_info.removeAt(i--);

                    gFlag++;
                }break;
                case 29:
                {
                    m_nPunchG = GCODE_G29;
                    inf.m_info.removeAt(i--);

                    gFlag++;
                }break;
                case 39:
                {
                    m_nPunchG = GCODE_G39;
                    inf.m_info.removeAt(i--);

                    gFlag++;
                }break;
                case 26:
                {
                    m_nPunchG = GCODE_G26;
                    inf.m_info.removeAt(i--);

                    gFlag++;
                }break;
                case 36:
                {
                    m_nPunchG = GCODE_G36;
                    inf.m_info.removeAt(i--);

                    gFlag++;
                }break;
                case 37:
                {
                    m_nPunchG = GCODE_G37;
                    inf.m_info.removeAt(i--);

                    gFlag++;
                }break;
                case 66:
                {
                    m_nPunchG = GCODE_G66;
                    inf.m_info.removeAt(i--);

                    gFlag++;
                }break;
                case 67:
                {
                    m_nPunchG = GCODE_G67;
                    inf.m_info.removeAt(i--);

                    gFlag++;
                }break;
                case 68:
                {
                    m_nPunchG = GCODE_G68;
                    inf.m_info.removeAt(i--);

                    gFlag++;
                }break;
                case 69:
                {
                    m_nPunchG = GCODE_G69;
                    inf.m_info.removeAt(i--);

                    gFlag++;
                }break;
                case 78:
                {
                    m_nPunchG = GCODE_G78;
                    inf.m_info.removeAt(i--);

                    gFlag++;
                }break;
                case 79:
                {
                    m_nPunchG = GCODE_G79;
                    inf.m_info.removeAt(i--);

                    gFlag++;
                }break;
                case 0:
                {
                    inf.m_info.removeAt(i--);
                    gFlag++;
                }break;
                case 70:
                {
                    m_nPunchG = GCODE_NOT_PUNCH;
                    inf.m_info.removeAt(i--);
                    //gFlag++;//此处运行重复
                }break;
                default:
                {
                    e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, QString("alarm14 : 第%1行 出现未支持G指令!").arg(m_LineSerialNum));
                    return FALSE;
                }break;
            }
        }
        else if(s == "P" || s == "p")
        {
            m_sP = inf.m_info.at(i).value;
            m_isPunchPAssign = TRUE;
            inf.m_info.removeAt(i--);

            pFlag++;
        }
        else if(s == "Q" || s == "q")
        {
            m_dQ = inf.m_info.at(i).value.toDouble();
            m_isPunchQAssign = TRUE;
            inf.m_info.removeAt(i--);

            qFlag++;
        }
        else if(s == "D" || s == "d")
        {
            m_dD = inf.m_info.at(i).value.toDouble();
            m_isPunchDAssign = TRUE;
            inf.m_info.removeAt(i--);

            dFlag++;
        }
        else if(s == "K" || s == "k")
        {
            m_sK = inf.m_info.at(i).value;
            m_isPunchKAssign = TRUE;
            inf.m_info.removeAt(i--);

            kFlag++;
        }
        else if(s == "I" || s == "i")
        {
            m_dI = inf.m_info.at(i).value.toDouble();
            m_isPunchIAssign = TRUE;
            inf.m_info.removeAt(i--);

            iFlag++;
        }
        else if(s == "J" || s == "j")
        {
            m_dJ = inf.m_info.at(i).value.toDouble();
            m_isPunchJAssign = TRUE;
            inf.m_info.removeAt(i--);

            jFlag++;
        }
        else if(s == "T" || s == "t")
        {
            m_nT = inf.m_info.at(i).value.toInt();
            //m_isPunchTAssign = TRUE;//xodd
            if (Cache->m_ToolLastSerialNum != m_nT)//xodd
            {
                m_isPunchTAssign = TRUE;
                Cache->m_isSelectTool = TRUE;
                Cache->m_ToolLastSerialNum = m_nT;
            }
            inf.m_info.removeAt(i--);

            tFlag++;
        }
    }

    /*G代码格式校验*/
    if(!Cache->m_isSelectTool)
    {
        e.insertDebug(DEBUG_NG_PUNCH_FILE_ALARM, QString("alarm14 : 第%1行 没有选择刀具").arg(m_LineSerialNum));
        return FALSE;
    }
    if (gFlag > 1)
    {
        e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, QString("alarm14 : 第%1行 G指令重复出现!").arg(m_LineSerialNum));
        return FALSE;
    }
    if (tFlag > 1)
    {
        e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, QString("alarm14 : 第%1行 刀具重复出现!").arg(m_LineSerialNum));
        return FALSE;
    }
    switch (m_nPunchG)
    {
    case GCODE_PUNCH_NULL:
        break;
    case GCODE_G28:    //G28 线性角度冲裁：G28 I  J  K  T
        if ((pFlag + qFlag + dFlag) > 0)
        {
            e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, QString("alarm14 : 第%1行 G28指令格式错误!").arg(m_LineSerialNum));
            return FALSE;
        }
        if ((iFlag > 1) || (jFlag > 1) || (kFlag > 1))
        {
            e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, QString("alarm14 : 第%1行 G28指令格式错误!").arg(m_LineSerialNum));
            return FALSE;
        }
        break;
    case GCODE_G38:    //G38 线性角度冲裁：G38 I J K T
        if ((pFlag + qFlag + dFlag) > 0)
        {
            e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, QString("alarm14 : 第%1行 G38指令格式错误!").arg(m_LineSerialNum));
            return FALSE;
        }
        if ((iFlag > 1) || (jFlag > 1) || (kFlag > 1))
        {
            e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, QString("alarm14 : 第%1行 G38指令格式错误!").arg(m_LineSerialNum));
            return FALSE;
        }
        break;
    case GCODE_G29:    //G29 圆弧等分冲孔：G29 I J P K T
        if ((qFlag + dFlag) > 0)
        {
            e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, QString("alarm14 : 第%1行 G29指令格式错误!").arg(m_LineSerialNum));
            return FALSE;
        }
        if ((iFlag > 1) || (jFlag > 1) || (pFlag > 1) || (kFlag > 1))
        {
            e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, QString("alarm14 : 第%1行 G29指令格式错误!").arg(m_LineSerialNum));
            return FALSE;
        }
        break;
    case GCODE_G39:    //G39 圆弧等分冲孔：G39 I J P K T
        if ((qFlag + dFlag) > 0)
        {
            e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, QString("alarm14 : 第%1行 G39指令格式错误!").arg(m_LineSerialNum));
            return FALSE;
        }
        if ((iFlag > 1) || (jFlag > 1) || (pFlag > 1) || (kFlag > 1))
        {
            e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, QString("alarm14 : 第%1行 G39指令格式错误!").arg(m_LineSerialNum));
            return FALSE;
        }
        break;
    case GCODE_G26:    //G26 圆周等分冲孔：G26 I J K T
        if ((pFlag + qFlag + dFlag) > 0)
        {
            e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, QString("alarm14 : 第%1行 G26指令格式错误!").arg(m_LineSerialNum));
            return FALSE;
        }
        if ((iFlag > 1) || (jFlag > 1) || (kFlag > 1))
        {
            e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, QString("alarm14 : 第%1行 G26指令格式错误!").arg(m_LineSerialNum));
            return FALSE;
        }
        break;
    case GCODE_G36:    //G36 X方向栅格冲孔：G36 I P J K T
        if ((qFlag + dFlag) > 0)
        {
            e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, QString("alarm14 : 第%1行 G36指令格式错误!").arg(m_LineSerialNum));
            return FALSE;
        }
        if ((iFlag > 1) || (jFlag > 1) || (pFlag > 1) || (kFlag > 1))
        {
            e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, QString("alarm14 : 第%1行 G36指令格式错误!").arg(m_LineSerialNum));
            return FALSE;
        }
        break;
    case GCODE_G37:    //G37 Y方向栅格冲孔：G37 I P J K T
        if ((qFlag + dFlag) > 0)
        {
            e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, QString("alarm14 : 第%1行 G37指令格式错误!").arg(m_LineSerialNum));
            return FALSE;
        }
        if ((iFlag > 1) || (jFlag > 1) || (pFlag > 1) || (kFlag > 1))
        {
            e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, QString("alarm14 : 第%1行 G37指令格式错误!").arg(m_LineSerialNum));
            return FALSE;
        }
        break;
    case GCODE_G66:    //G66 直线剪切：G66 I J P Q D T || G66 I J P K T
        if (kFlag > 1)
        {
            e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, QString("alarm14 : 第%1行 G66指令格式错误!").arg(m_LineSerialNum));
            return FALSE;
        }
        else if (kFlag == 1)
        {
            if ((qFlag + dFlag) > 0)
            {
                e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, QString("alarm14 : 第%1行 G66指令格式错误!").arg(m_LineSerialNum));
                return FALSE;
            }
            if ((iFlag > 1) || (jFlag > 1) || (pFlag > 1))
            {
                e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, QString("alarm14 : 第%1行 G66指令格式错误!").arg(m_LineSerialNum));
                return FALSE;
            }
        }
        else
        {
            if ((iFlag > 1) || (jFlag > 1) || (pFlag > 1) || (qFlag > 1) || (dFlag > 1))
            {
                e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, QString("alarm14 : 第%1行 G66指令格式错误!").arg(m_LineSerialNum));
                return FALSE;
            }
        }
        break;
    case GCODE_G67:    //G67 正方形剪切：G67 I J P T
        if ((kFlag + qFlag + dFlag) > 0)
        {
            e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, QString("alarm14 : 第%1行 G67指令格式错误!").arg(m_LineSerialNum));
            return FALSE;
        }
        if ((iFlag > 1) || (jFlag > 1) || (pFlag > 1))
        {
            e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, QString("alarm14 : 第%1行 G67指令格式错误!").arg(m_LineSerialNum));
            return FALSE;
        }
        break;
    case GCODE_G68:    //G68 圆弧蚕食：G68 I J k P Q T
        if (dFlag > 0)
        {
            e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, QString("alarm14 : 第%1行 G68指令格式错误!").arg(m_LineSerialNum));
            return FALSE;
        }
        if ((iFlag > 1) || (jFlag > 1) || (kFlag > 1) || (pFlag > 1) || (qFlag > 1))
        {
            e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, QString("alarm14 : 第%1行 G68指令格式错误!").arg(m_LineSerialNum));
            return FALSE;
        }
        break;
    case GCODE_G69:    //G69 直线蚕食：G69 I J P Q T
        if ((kFlag + dFlag) > 0)
        {
            e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, QString("alarm14 : 第%1行 G69指令格式错误!").arg(m_LineSerialNum));
            return FALSE;
        }
        if ((iFlag > 1) || (jFlag > 1) || (pFlag > 1) || (qFlag > 1))
        {
            e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, QString("alarm14 : 第%1行 G69指令格式错误!").arg(m_LineSerialNum));
            return FALSE;
        }
        break;
    case GCODE_G78:    //G78 圆弧冲孔：G78 I J k P Q D T
        if ((iFlag > 1) || (jFlag > 1) || (kFlag > 1) || (pFlag > 1) || (qFlag > 1) || (dFlag > 1))
        {
            e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, QString("alarm14 : 第%1行 G78指令格式错误!").arg(m_LineSerialNum));
            return FALSE;
        }
        break;
    case GCODE_G79:    //G79 直线冲孔：G79 I J P Q D T
        if (kFlag > 0)
        {
            e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, QString("alarm14 : 第%1行 G79指令格式错误!").arg(m_LineSerialNum));
            return FALSE;
        }
        if ((iFlag > 1) || (jFlag > 1) || (dFlag > 1) || (pFlag > 1) || (qFlag > 1))
        {
            e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, QString("alarm14 : 第%1行 G79指令格式错误!").arg(m_LineSerialNum));
            return FALSE;
        }
        break;
    default:
        break;
    }

    return ActionMoveGCode::fillGCodeVarient(Cache, inf, e);
}

/*************************************************
  函数名称：genrateAction(EmulationInterVariableCache* Cache, QVector<BaseAction *>& actions, ErrorInfo& e)
  函数功能：解析G代码（冲压部分代码）
  输入参数： Cache  解析（仿真）时临时变量存储数据
  输出参数： actions  解析后产生的动作
           e     错误代码
  返 回 值：  TRUE 成功
            FALSE  失败
  备    注：
**************************************************/
bool ActionPunchGCode::genrateAction(EmulationInterVariableCache* Cache, QVector<BaseAction *>& actions, ErrorInfo& e)
{
    int errNum=0;

    m_ReferencePosition.x = Cache->m_ReferenceCoordinate.x;
    m_ReferencePosition.y = Cache->m_ReferenceCoordinate.y;

    m_prevToolInfo = m_curToolInfo;
    m_curToolInfo = getToolInfoById(Cache, m_nT, e);
    if (e.e_type != DEBUG_NO_ERROR)//不确定
    {
        return false;
    }

    switch (m_nPunchG)
    {
        case GCODE_PUNCH_IO:
        {
            if (m_prevToolInfo == NULL)
            {
                if (fabs(Cache->m_dModulesAngle - m_curToolInfo->getPosAngle()) > 0.01)
                {
                }
                else
                {
                    ioAction(Cache, actions, IO_DINGWEIXIAO_PLC_M, true, IO_DINGWEIXIAO_SIGNAL_PLC_M);//插销进
                    toolAngleRotary(Cache, actions, 0);//C轴转到零点

                    if (m_curToolInfo->isRotaryTool())//C轴离合合（如果是转模的话）
                        ioAction(Cache, actions, IO_AXISCLIHE_PLC_M, true, IO_AXIS_C_JIEHE_SIGNAL_PLC_M);
                }
            }
            return true;
        }break;
        case GCODE_PUNCH_NULL:    //直接冲压
        {
            if(m_nMoveG == GCODE_MOVE_NULL)
            {
                m_nMoveG = GCODE_G00;
            }


            //ActionMoveGCode::genrateAction(Cache, actions, e);    //运动到冲压位置
            //toolPunch(Cache, actions);    //冲压动作

            return true;
        }break;
        case GCODE_NOT_PUNCH:
        {
            Cache->m_MoveType = GCODE_G70;
            return moveToPoint(Cache, actions, Point(m_dX, m_dY,m_dZ,m_dW,m_dP,m_dR), e);
        }break;

        case GCODE_G28:    //G28 线性角度冲裁
        {
            runG28(Cache, actions, m_dI, m_dJ, m_sK.toInt());
            return true;
        }break;

        case GCODE_G38:    //G38 线性角度冲裁
        {
            runG38(Cache, actions, m_dI, m_dJ, m_sK.toInt());
            return true;
        }break;

        case GCODE_G29:    //G29 圆弧等分冲孔
        {
            if (m_dI <= 0)
            {
                e.insertDebug(DEBUG_NG_PUNCH_FILE_ALARM, QString("ActionPunchGCode::genrateAction : 第%1行 G29（圆弧等分冲孔）的参数I错误").arg(m_LineSerialNum));
                return false;
            }
            if (m_sK.toInt() <= 0)
            {
                e.insertDebug(DEBUG_NG_PUNCH_FILE_ALARM, QString("ActionPunchGCode::genrateAction : 第%1行 G29（圆弧等分冲孔）的参数K错误").arg(m_LineSerialNum));
                return false;
            }
            runG29(Cache, actions, m_dI, m_dJ, m_sP.toDouble(), m_sK.toInt());
            return true;
        }break;
        case GCODE_G39:    //G39 圆弧等分冲孔
        {
            if (m_dI <= 0)
            {
                e.insertDebug(DEBUG_NG_PUNCH_FILE_ALARM, QString("ActionPunchGCode::genrateAction : 第%1行 G39（圆弧等分冲孔）的参数I错误").arg(m_LineSerialNum));
                return false;
            }
            if (m_sK.toInt() <= 0)
            {
                e.insertDebug(DEBUG_NG_PUNCH_FILE_ALARM, QString("ActionPunchGCode::genrateAction : 第%1行 G39（圆弧等分冲孔）的参数K错误").arg(m_LineSerialNum));
                return false;
            }
            runG39(Cache, actions, m_dI, m_dJ, m_sP.toDouble(), m_sK.toInt());
            return true;
        }break;

        case GCODE_G26:    //G26 圆周等分冲孔
        {
            if (m_dI <= 0)
            {
                e.insertDebug(DEBUG_NG_PUNCH_FILE_ALARM, QString("ActionPunchGCode::genrateAction : 第%1行 G26（圆周等分冲孔）的参数I错误").arg(m_LineSerialNum));
                return false;
            }
            runG26(Cache, actions, m_dI, m_dJ, m_sK.toInt());
            return true;
        }break;

        case GCODE_G36:    //G36 X方向栅格冲孔
        {
            if (m_sP.toInt() <= 0 || m_sK.toInt() <= 0)
            {
                e.insertDebug(DEBUG_NG_PUNCH_FILE_ALARM, QString("ActionPunchGCode::genrateAction : 第%1行 G36（X方向栅格冲孔）的参数P/K错误").arg(m_LineSerialNum));
                return false;
            }
            runG36(Cache, actions, m_dI, m_sP.toInt(), m_dJ, m_sK.toInt());
            return true;
        }break;

        case GCODE_G37:    //G37 Y方向栅格冲孔
        {
            if (m_sP.toInt() <= 0 || m_sK.toInt() <= 0)
            {
                e.insertDebug(DEBUG_NG_PUNCH_FILE_ALARM, QString("ActionPunchGCode::genrateAction : 第%1行 G36（X方向栅格冲孔）的参数P/K错误").arg(m_LineSerialNum));
                return false;
            }
            runG37(Cache, actions, m_dI, m_sP.toInt(), m_dJ, m_sK.toInt());
            return true;
        }break;

        case GCODE_G66:    //G66 直线剪切
        {
            if (m_isPunchKAssign)    //G66 I J P K T
            {
                errNum=runG66(Cache, actions, m_dI, m_dJ, m_sP.toDouble(), m_sK.toDouble());
                if (errNum == -1)
                {
                    e.insertDebug(DEBUG_NG_PUNCH_FILE_ALARM, QString("ActionPunchGCode::genrateAction : 第%1行 冲裁长度应大于1.5倍模具（刀具）的长度").arg(m_LineSerialNum));
                    return false;
                }

            }
            else    //G66 I J P Q D T
            {
                errNum=runG66(Cache, actions, m_dI, m_dJ, m_sP.toDouble(), m_dQ, m_dD);
                if (errNum == -1)
                {
                    e.insertDebug(DEBUG_NG_PUNCH_FILE_ALARM, QString("ActionPunchGCode::genrateAction : 第%1行 G66 冲裁长度应大于1.5倍模具（刀具）的长度").arg(m_LineSerialNum));
                    return false;
                }
                else if (errNum == -2)
                {
                    e.insertDebug(DEBUG_NG_PUNCH_FILE_ALARM, QString("ActionPunchGCode::genrateAction : 第%1行 G66 模具（刀具）的长度和宽度必须同为正值或负值").arg(m_LineSerialNum));
                    return false;
                }
            }
            return true;
        }break;

        case GCODE_G67:    //G67 正方形剪切
        {
            errNum = runG67(Cache, actions, m_dI, m_dJ, m_sP.toDouble());
            if (errNum == -1)
            {
                e.insertDebug(DEBUG_NG_PUNCH_FILE_ALARM, QString("ActionPunchGCode::genrateAction : 第%1行 G67 冲裁矩形边长应大于3倍模具（刀具）的长度").arg(m_LineSerialNum));
                return false;
            }
            else if (errNum == -2)
            {
                e.insertDebug(DEBUG_NG_PUNCH_FILE_ALARM, QString("ActionPunchGCode::genrateAction : 第%1行 G67 正方形模具（刀具）的边长不能为负数").arg(m_LineSerialNum));
                return false;
            }
            return true;
        }break;

        case GCODE_G68:    //G68 圆弧蚕食
        {
            errNum = runG68(Cache, actions, m_dI, m_dJ, m_sK.toDouble(), m_sP.toDouble(), m_dQ);
            if (errNum == -1)
            {
                e.insertDebug(DEBUG_NG_PUNCH_FILE_ALARM, QString("ActionPunchGCode::genrateAction : 第%1行 G68 圆弧半径不能小于零").arg(m_LineSerialNum));
                return false;
            }
            else if (errNum == -2)
            {
                e.insertDebug(DEBUG_NG_PUNCH_FILE_ALARM, QString("ActionPunchGCode::genrateAction : 第%1行 G68 连续冲裁间距不能小于零或大于8mm").arg(m_LineSerialNum));
                return false;
            }
            return true;
        }break;

        case GCODE_G69:    //G69 直线蚕食
        {
            errNum = runG69(Cache, actions, m_dI, m_dJ, m_sP.toDouble(), m_dQ);
            if (errNum == -1)
            {
                e.insertDebug(DEBUG_NG_PUNCH_FILE_ALARM, QString("ActionPunchGCode::genrateAction : 第%1行 G69 冲孔长度不能小于零").arg(m_LineSerialNum));
                return false;
            }
            else if (errNum == -2)
            {
                e.insertDebug(DEBUG_NG_PUNCH_FILE_ALARM, QString("ActionPunchGCode::genrateAction : 第%1行 G69 蚕食间距不能小于零").arg(m_LineSerialNum));
                return false;
            }
            return true;
        }break;

        case GCODE_G78:    //G78 圆弧冲孔
        {
            errNum = runG78(Cache, actions, m_dI, m_dJ, m_sK.toDouble(), m_sP.toDouble(), m_dQ, m_dD);
            if (errNum == -1)
            {
                e.insertDebug(DEBUG_NG_PUNCH_FILE_ALARM, QString("ActionPunchGCode::genrateAction : 第%1行 G78 圆弧半径不能小于零").arg(m_LineSerialNum));
                return false;
            }
            else if (errNum == -2)
            {
                e.insertDebug(DEBUG_NG_PUNCH_FILE_ALARM, QString("ActionPunchGCode::genrateAction : 第%1行 G78 连续冲裁间距不能小于零或大于8mm").arg(m_LineSerialNum));
                return false;
            }
            else if (errNum == -3)
            {
                e.insertDebug(DEBUG_NG_PUNCH_FILE_ALARM, QString("ActionPunchGCode::genrateAction : 第%1行 G78 加工板厚不能大于冲裁间距").arg(m_LineSerialNum));
                return false;
            }
            return true;
        }break;

        case GCODE_G79:    //G79 直线冲孔
        {
            errNum = runG79(Cache, actions, m_dI, m_dJ, m_sP.toDouble(), m_dQ, m_dD);
            if (errNum == -1)
            {
                e.insertDebug(DEBUG_NG_PUNCH_FILE_ALARM, QString("ActionPunchGCode::genrateAction : 第%1行 G79 圆弧半径不能小于零").arg(m_LineSerialNum));
                return false;
            }
            else if (errNum == -2)
            {
                e.insertDebug(DEBUG_NG_PUNCH_FILE_ALARM, QString("ActionPunchGCode::genrateAction : 第%1行 G79 连续冲裁间距不能小于零或大于8mm").arg(m_LineSerialNum));
                return false;
            }
            else if (errNum == -3)
            {
                e.insertDebug(DEBUG_NG_PUNCH_FILE_ALARM, QString("ActionPunchGCode::genrateAction : 第%1行 G79 加工板厚不能大于冲裁间距").arg(m_LineSerialNum));
                return false;
            }
            return true;
        }break;

        default:
            return false;
    }

}


/*************************************************
  函数名称：toDebugString()
  函数功能：
  输入参数：
  输出参数：
  返 回 值：
  备    注：
**************************************************/
QString  ActionPunchGCode::toDebugString()
{
    QString  ss;

    ss += "冲压代码：\n";
    ss += "冲压类型：";
    ss += GCodePunchString[m_nPunchG];
    ss += ",P：";
    if(m_isPunchPAssign)
    {
        ss += m_sP;
    }
    else
    {
        ss += "NULL";
    }

    ss += ",Q：";
    if(m_isPunchQAssign)
    {
        ss += ss.number(m_dQ);
    }
    else
    {
        ss += "NULL";
    }
    ss += ",D：";
    if(m_isPunchDAssign)
    {
        ss += ss.number(m_dD);
    }
    else
    {
        ss += "NULL";
    }
    ss += ",I：";
    if(m_isPunchIAssign)
    {
        ss += ss.number(m_dI);
    }
    else
    {
        ss += "NULL";
    }
    ss += ",J：";
    if(m_isPunchJAssign)
    {
        ss += ss.number(m_dJ);
    }
    else
    {
        ss += "NULL";
    }

    ss += ",K：";
    if(m_isPunchKAssign)
    {
        ss += m_sK;
    }
    else
    {
        ss += "NULL";
    }

    ss += ",T：";
    ss += ss.number(m_nT);
    ss += "\n";

    return ActionMoveGCode::toDebugString() + ss;
}

/*************************************************
  函数名称：toolAngleRotary(EmulationInterVariableCache *Cache, QVector<BaseAction *> &actions, double angle)
  函数功能：刀具角度旋转动作产生
  输入参数： Cache  解析（仿真）时临时变量存储数据
           angle  旋转角度（绝对坐标系）
  输出参数： actions  解析后产生的动作
  返 回 值：  TRUE 成功
            FALSE  失败
  备    注：
**************************************************/
bool ActionPunchGCode::toolAngleRotary(EmulationInterVariableCache *Cache, QVector<BaseAction *> &actions, double angle)
{
    //return true;
    if (fabs(Cache->m_dToolAngle - angle) > 0.01)
    {
        Cache->m_dToolAngle = angle;
        //Cache->m_dToolAngle = RADIANS(m_dC);
        //    MoveAction *toolRotateAction = new MoveAction;
        ToolRotateAction *toolRotateAction = new ToolRotateAction;

        toolRotateAction->m_nLineSerialNum = m_LineSerialNum;
        toolRotateAction->m_startPoint = Cache->m_LastPosition;
        toolRotateAction->m_endPoint = Cache->m_LastPosition;
        toolRotateAction->m_basePoint = Cache->m_LastPosition;
        toolRotateAction->m_tool = m_curToolInfo;
        toolRotateAction->m_dAngle = angle;
        actions.push_back(toolRotateAction);
    }

    return true;
}



/*************************************************
  函数名称：runG28(EmulationInterVariableCache* Cache, QVector<BaseAction *> &actions, double dI, double dJ, int nK)
  函数功能：G28 线性角度冲裁： G28 I J K T
  输入参数： Cache  解析（仿真）时临时变量存储数据
           dI、dJ、nK  G28指令参数
  输出参数： actions  解析后产生的动作
  返 回 值：  0 成功
            其它  失败
  备    注：
**************************************************/
int ActionPunchGCode::runG28(EmulationInterVariableCache* Cache, QVector<BaseAction *> &actions, double dI, double dJ, int nK)
{
    Point temPoint(0,0,0,0,0,0);

    double angle = PI*dJ/180;
    double dx = dI*cos(angle);
    double dy = dI*sin(angle);


    for (int i=1; i <= nK; i++)
    {
        temPoint.x = m_ReferencePosition.x + i*dx;
        temPoint.y = m_ReferencePosition.y + i*dy;

    }

    return 0;
}

/*************************************************
  函数名称：runG38(EmulationInterVariableCache* Cache, QVector<BaseAction *> &actions, double dI, double dJ, int nK)
  函数功能：G38 线性角度冲裁： G38 I J K T
  输入参数： Cache  解析（仿真）时临时变量存储数据
           dI、dJ、nK  G38指令参数
  输出参数： actions  解析后产生的动作
  返 回 值： 0 成功
            其它 失败
  备    注：
**************************************************/
int ActionPunchGCode::runG38(EmulationInterVariableCache* Cache, QVector<BaseAction *> &actions, double dI, double dJ, int nK)
{
    runG28(Cache, actions, dI, dJ, nK);
    return 0;
}

/*************************************************
  函数名称：runG29(EmulationInterVariableCache *Cache, QVector<BaseAction *> &actions, double dI, double dJ, double dP, int nK)
  函数功能：G29 圆弧等分冲孔： G29 I J P K T
  输入参数： Cache  解析（仿真）时临时变量存储数据
           dI、dJ、dP、nK  G29指令参数
  输出参数： actions  解析后产生的动作
  返 回 值：  0 成功
            其它  失败
  备    注：
**************************************************/
int ActionPunchGCode::runG29(EmulationInterVariableCache *Cache, QVector<BaseAction *> &actions, double dI, double dJ, double dP, int nK)
{
    Point temPoint(0,0,0,0,0,0);

    double angleJ = PI*dJ/180;
    double angleP = PI*dP/180;
    double dAngle = 0;


    for (int i=0; i < nK; i++)
    {
        dAngle = angleJ + i*angleP;
        temPoint.x = m_ReferencePosition.x + dI*cos(dAngle);
        temPoint.y = m_ReferencePosition.y + dI*sin(dAngle);

        Cache->m_dToolAngle = dAngle*180.0/PI + 90.0;//dAngle + PI*0.5;

    }

    return 0;
}

/*************************************************
  函数名称：runG39(EmulationInterVariableCache *Cache, QVector<BaseAction *> &actions, double dI, double dJ, double dP, int nK)
  函数功能：G39 圆弧等分冲孔： G39 I J P K T
  输入参数： Cache  解析（仿真）时临时变量存储数据
           dI、dJ、dP、nK  G39指令参数
  输出参数： actions  解析后产生的动作
  返 回 值：  0 成功
            其它  失败
  备    注：
**************************************************/
int ActionPunchGCode::runG39(EmulationInterVariableCache *Cache, QVector<BaseAction *> &actions, double dI, double dJ, double dP, int nK)
{
    runG29(Cache, actions, dI, dJ, dP, nK);
    return 0;
}

/*************************************************
  函数名称：runG26(EmulationInterVariableCache *Cache, QVector<BaseAction *> &actions, double dI, double dJ, int nK)
  函数功能：G26 圆周等分冲孔： G26 I J K T
  输入参数： Cache  解析（仿真）时临时变量存储数据
           dI、dJ、nK  G26指令参数
  输出参数： actions  解析后产生的动作
  返 回 值：  0 成功
            其它  失败
  备    注：
**************************************************/
int ActionPunchGCode::runG26(EmulationInterVariableCache *Cache, QVector<BaseAction *> &actions, double dI, double dJ, int nK)
{
    Point temPoint(0,0,0,0,0,0);

    double angleJ = PI*dJ/180;
    double angleP = 2*PI/nK;
    double dAngle = 0;


    for (int i=0; i < abs(nK); i++)
    {
        dAngle = angleJ + i*angleP;
        temPoint.x = m_ReferencePosition.x + dI*cos(dAngle);
        temPoint.y = m_ReferencePosition.y + dI*sin(dAngle);

        Cache->m_dToolAngle = dAngle*180.0/PI + 90.0;//dAngle + PI*0.5;

    }

    return 0;
}

/*************************************************
  函数名称：runG36(EmulationInterVariableCache *Cache, QVector<BaseAction *> &actions, double dI, int nP, double dJ, int nK)
  函数功能：G36 X方向栅格冲孔：G36 I P J K T
  输入参数： Cache  解析（仿真）时临时变量存储数据
           dI、dJ、dP、nK  G36指令参数
  输出参数： actions  解析后产生的动作
  返 回 值：  0 成功
            其它  失败
  备    注：
**************************************************/
int ActionPunchGCode::runG36(EmulationInterVariableCache *Cache, QVector<BaseAction *> &actions, double dI, int nP, double dJ, int nK)
{
    Point temPoint;
    int i=0, j=0;

    temPoint.x = m_ReferencePosition.x;
    temPoint.y = m_ReferencePosition.y;


    for (i=0; i<=nK; i++)
    {
        for (j=1; j<=nP; j++)
        {
            if (i%2 == 0)
            {
                temPoint.x += dI;
            }
            else
            {
                temPoint.x -= dI;
            }
        }

        if(i < nK)
        {
            temPoint.y += dJ;
        }
    }

    return 0;
}

/*************************************************
  函数名称：runG37(EmulationInterVariableCache *Cache, QVector<BaseAction *> &actions, double dI, int nP, double dJ, int nK)
  函数功能：G37 Y方向栅格冲孔：G37 I P J K T
  输入参数： Cache  解析（仿真）时临时变量存储数据
           dI、dJ、dP、nK  G37指令参数
  输出参数： actions  解析后产生的动作
  返 回 值：  0 成功
            其它  失败
  备    注：
**************************************************/
int ActionPunchGCode::runG37(EmulationInterVariableCache *Cache, QVector<BaseAction *> &actions, double dI, int nP, double dJ, int nK)
{
    Point temPoint;
    int i=0, j=0;

    temPoint.x = m_ReferencePosition.x;
    temPoint.y = m_ReferencePosition.y;


    for (j=0; j<=nP; j++)
    {
        for (i=1; i<=nK; i++)
        {
            if (j%2 == 0)
            {
                temPoint.y += dJ;
            }
            else
            {
                temPoint.y -= dJ;
            }
        }

        if(j < nP)
        {
            temPoint.x += dI;
        }
    }

    return 0;
}

/*************************************************
  函数名称：runG66(EmulationInterVariableCache *Cache, QVector<BaseAction *> &actions, double dI, double dJ, double dP, double dQ, double dD)
  函数功能：G66 直线剪切：G66 I J P Q D T
  输入参数： Cache  解析（仿真）时临时变量存储数据
           dI、dJ、dP、dQ、dD  G66指令参数
  输出参数： actions  解析后产生的动作
  返 回 值：  0 成功
            -1 冲裁长度应大于1.5倍模具（刀具）的长度
            -2 模具（刀具）的长度和宽度必须同为正值或负值
  备    注：
**************************************************/
int ActionPunchGCode::runG66(EmulationInterVariableCache *Cache, QVector<BaseAction *> &actions, double dI, double dJ, double dP, double dQ, double dD)
{
    Point temPoint(0,0,0,0,0,0);

    if (dQ*dP > 0)
    {
        if (dP < 0)
        {
            dP = -dP;//模具长度在计算时需要用到绝对值
        }
    }
    else
        return -2;//模具（刀具）的长度和宽度必须同为正值或负值

    if (dI < 1.5*dP)
        return -1;//冲裁长度应大于1.5倍模具（刀具）的长度

    double dp = dP/2 - dD;//刀具中心点距离路径原点的长度
    double dq = dQ/2;//刀具中心点距离路径原点的宽度
    double angle = PI*dJ/180;
    double cosAngle = cos(angle);
    double sinAngle = sin(angle);
    double toolx = dp*cosAngle - dq*sinAngle;//刀具中心在X方向的投影距离
    double tooly = dp*sinAngle + dq*cosAngle;//刀具中心在Y方向的投影距离

    double dLength = dI + 2*dD;//总冲裁长度
    //int nK = ceil(dLength/dP);//总冲裁次数
    int nK = (int)(dLength/dP) + 1;
    double dL = (dLength-dP)/(nK-1);//每两个冲孔之间的中心距

    double dx = dL*cosAngle;
    double dy = dL*sinAngle;


    /*将参考坐标系移到刀具起点*/
    m_ReferencePosition.x += toolx;
    m_ReferencePosition.y += tooly;

    for (int i=0; i < nK; i++)
    {
        temPoint.x = m_ReferencePosition.x + i*dx;
        temPoint.y = m_ReferencePosition.y + i*dy;

    }

    /*将参考坐标系移到原来位置*/
    m_ReferencePosition.x -= toolx;
    m_ReferencePosition.y -= tooly;

    return 0;
}

/*************************************************
  函数名称：runG66(EmulationInterVariableCache *Cache, QVector<BaseAction *> &actions, double dI, double dJ, double dP, double dK)
  函数功能：G66 直线剪切：G66 I J P K T
  输入参数： Cache  解析（仿真）时临时变量存储数据
           dI、dJ、dP、dK  G66指令参数
  输出参数： actions  解析后产生的动作
  返 回 值：  0 成功
            -1 冲裁长度应大于1.5倍模具（刀具）的长度
  备    注：
**************************************************/
int ActionPunchGCode::runG66(EmulationInterVariableCache *Cache, QVector<BaseAction *> &actions, double dI, double dJ, double dP, double dK)
{
    Point temPoint(0,0,0,0,0,0);

    double dLength = fabs(dI);
    double dWidth = fabs(dK);
    double dTool = fabs(dP);

    if (dLength < 1.5*dTool || dWidth < 1.5*dTool)
        return -1;//冲裁长度应大于1.5倍模具（刀具）的长度

    double angle = PI*dJ/180;
    double cosAngle = cos(angle);
    double sinAngle = sin(angle);


    /*计算刀具中心点（刀具起点）与路径原点的距离*/
    double toolu = dI > 0 ? dTool/2 : -dTool/2;
    double toolv = dP/2;
    double toolx = toolu*cosAngle - toolv*sinAngle;
    double tooly = toolu*sinAngle + toolv*cosAngle;

    /*将参考坐标系移到刀具起点*/
    m_ReferencePosition.x += toolx;
    m_ReferencePosition.y += tooly;

    /*冲碎平面，计算时先计算局部坐标系UOV下的坐标，再映射到工件坐标系XOY中*/
    int nku = ceil(dLength/dTool);
    int nkv = ceil(dWidth/dTool);
    //int nku = (int)(dLength/dTool) + 1;
    //int nkv = (int)(dWidth/dTool) + 1;
    double du = dI > 0 ? (dLength-dTool)/(nku-1) : (dTool-dLength)/(nku-1);
    double dv = dP > 0 ? (dWidth-dTool)/(nkv-1) : (dTool-dWidth)/(nkv-1);
    double temu = 0, temv = 0;
    int i = 0, j = 0;
    for (i = 0; i < nkv; i++)
    {
        for (j = 0; j < nku; j++)
        {
            temPoint.x = temu*cosAngle - temv*sinAngle + m_ReferencePosition.x;
            temPoint.y = temu*sinAngle + temv*cosAngle+ m_ReferencePosition.y;

            temu = i%2 == 0 ? temu+du : temu-du;

        }

        temu = i%2 == 0 ? temu-du : temu+du;
        temv += dv;
    }

    /*将参考坐标系移到原来位置*/
    m_ReferencePosition.x -= toolx;
    m_ReferencePosition.y -= tooly;

    return 0;
}

/*************************************************
  函数名称：runG67(EmulationInterVariableCache *Cache, QVector<BaseAction *> &actions, double dI, double dJ, double dP)
  函数功能：G67 正方形剪切：G67 I J P T
  输入参数： Cache  解析（仿真）时临时变量存储数据
           dI、dJ、dP  G67指令参数
  输出参数： actions  解析后产生的动作
  返 回 值：  0 成功
            -1 冲裁矩形边长应大于3倍模具（刀具）的长度
            -2 正方形模具的边长不能为负数
  备    注：
**************************************************/
int ActionPunchGCode::runG67(EmulationInterVariableCache *Cache, QVector<BaseAction *> &actions, double dI, double dJ, double dP)
{
    Point temPoint(0,0,0,0,0,0);
    double dLength = fabs(dI);
    double dWidth = fabs(dJ);

    if (dP <= 0)
        return -2;//正方形模具的边长不能为负数

    if (dLength < 3*dP || dWidth < 3*dP)
        return -1;//冲裁矩形边长应大于3倍模具（刀具）的长度


    /*计算刀具中心点（刀具起点）与路径原点的距离*/
    double toolx = dI > 0 ? dP/2 : -dP/2;
    double tooly = dJ > 0 ? dP/2 : -dP/2;

    /*将参考坐标系移到刀具起点*/
    m_ReferencePosition.x += toolx;
    m_ReferencePosition.y += tooly;
    temPoint.x = m_ReferencePosition.x;
    temPoint.y = m_ReferencePosition.y;


    /*冲裁矩形边框*/
    int nkx = ceil(dLength/dP)-1;
    int nky = ceil(dWidth/dP)-1;
    //int nkx = (int)(dLength/dP);
    //int nky = (int)(dWidth/dP);
    double dx = dI > 0 ? (dLength-dP)/nkx : (dP-dLength)/nkx;
    double dy = dJ > 0 ? (dWidth-dP)/nky : (dP-dWidth)/nky;
    int i = 0, j = 0;
    for (i = 0; i < nkx; i++)
    {

        temPoint.x += dx;
    }
    for (j = 0; j < nky; j++)
    {
        Cache->m_dToolAngle += 90.0;//PI*0.5;

        temPoint.y += dy;
    }
    for (i = 0; i < nkx; i++)
    {
        Cache->m_dToolAngle += 90.0;//PI*0.5;

        temPoint.x -= dx;
    }
    for (j = 0; j < nky; j++)
    {
        Cache->m_dToolAngle += 90.0;//PI*0.5;

        temPoint.y -= dy;
    }

    /*将参考坐标系移到原来位置*/
    m_ReferencePosition.x -= toolx;
    m_ReferencePosition.y -= tooly;

    return 0;
}

/*************************************************
  函数名称：runG68(EmulationInterVariableCache *Cache, QVector<BaseAction *> &actions, double dI, double dJ, double dK, double dP, double dQ)
  函数功能：G68 圆弧蚕食：G68 I J k P Q T
  输入参数： Cache  解析（仿真）时临时变量存储数据
           dI、dJ、dP、dK、dQ  G68指令参数
  输出参数： actions  解析后产生的动作
  返 回 值：  0 成功
            -1 圆弧半径不能小于零
            -2 连续冲裁间距不能小于零或大于8mm
  备    注：
**************************************************/
int ActionPunchGCode::runG68(EmulationInterVariableCache *Cache, QVector<BaseAction *> &actions, double dI, double dJ, double dK, double dP, double dQ)
{
    if (dI <= 0)
        return -1;//圆弧半径不能小于零

    if (dQ < 0 || dQ > 8)
        return -2;//连续冲裁间距不能小于零或大于8mm

    Point temPoint(0,0,0,0,0,0);

    double angleJ = PI*dJ/180;
    double angleK = PI*dK/180;
    double radius = dI + dP/2;
    double dA = dQ/radius;
    int nK = ceil(fabs(angleK)/dA);
    //int nK = (int)(fabs(angleK)/dA) + 1;


    double dAngle = 0;
    dA = angleK/nK;
    for (int i=0; i <= nK; i++)
    {
        dAngle = angleJ + i*dA;
        temPoint.x = m_ReferencePosition.x + radius*cos(dAngle);
        temPoint.y = m_ReferencePosition.y + radius*sin(dAngle);
        Cache->m_dToolAngle = dAngle*180.0/PI + 90.0;//dAngle + PI*0.5;

    }

    return 0;
}

/*************************************************
  函数名称：runG69(EmulationInterVariableCache *Cache, QVector<BaseAction *> &actions, double dI, double dJ, double dP, double dQ)
  函数功能：G69 直线蚕食：G69 I J P Q T
  输入参数： Cache  解析（仿真）时临时变量存储数据
           dI、dJ、dP、dQ  G69指令参数
  输出参数： actions  解析后产生的动作
  返 回 值：  0 成功
            -1 冲孔长度不能小于零
            -2 蚕食间距不能小于零
  备    注：
**************************************************/
int ActionPunchGCode::runG69(EmulationInterVariableCache *Cache, QVector<BaseAction *> &actions, double dI, double dJ, double dP, double dQ)
{
    if (dI <= 0)
        return -1;//冲孔长度不能小于零

    if (dQ < 0)
        return -2;//蚕食间距不能小于零

    Point temPoint(0,0,0,0,0,0);

    double angle = PI*dJ/180;
    double cosAngle = cos(angle);
    double sinAngle = sin(angle);


    /*计算刀具中心点（刀具起点）与路径原点的距离*/
    double toolx = -dP*sinAngle/2;
    double tooly = dP*cosAngle/2;

    /*将参考坐标系移到刀具起点*/
    m_ReferencePosition.x += toolx;
    m_ReferencePosition.y += tooly;

    /*直线蚕食*/
    int nK = ceil(dI/dQ);
    //int nK = (int)(dI/dQ) + 1;
    double dL=dI/nK;
    double dx = dL*cosAngle;
    double dy = dL*sinAngle;
    for (int i=0; i <= nK; i++)
    {
        temPoint.x = m_ReferencePosition.x + i*dx;
        temPoint.y = m_ReferencePosition.y + i*dy;

    }

    /*将参考坐标系移到原来位置*/
    m_ReferencePosition.x -= toolx;
    m_ReferencePosition.y -= tooly;

    return 0;
}

/*************************************************
  函数名称：runG78(EmulationInterVariableCache *Cache, QVector<BaseAction *> &actions, double dI, double dJ, double dK, double dP, double dQ, double dD)
  函数功能：G78 圆弧冲孔：G78 I J k P Q D T
  输入参数： Cache  解析（仿真）时临时变量存储数据
           dI、dJ、dP、dK、dQ、dD  G78指令参数
  输出参数： actions  解析后产生的动作
  返 回 值：  0 成功
            -1 圆弧半径不能小于零
            -2 连续冲裁间距不能小于零或大于8mm
            -3 加工板厚不能大于冲裁间距
  备    注：
**************************************************/
int ActionPunchGCode::runG78(EmulationInterVariableCache *Cache, QVector<BaseAction *> &actions, double dI, double dJ, double dK, double dP, double dQ, double dD)
{
    if (dI <= 0)
        return -1;//圆弧半径不能小于零

    if (dQ < 0 || dQ > 10)
        return -2;//连续冲裁间距不能小于零或大于8mm

    if (dD > dQ)
        return -3;//加工板厚不能大于冲裁间距

    Point temPoint(0,0,0,0,0,0);

    double angleJ = PI*dJ/180;
    double angleK = PI*dK/180;
    double radius = dI + dP/2;
    double dA = dQ/radius;
    int nK = ceil(fabs(angleK)/dA);
    //int nK = (int)(fabs(angleK)/dA) + 1;


    double dAngle = 0;
    dA = angleK/nK;
    for (int i=0; i <= nK; i++)
    {
        dAngle = angleJ + i*dA;
        temPoint.x = m_ReferencePosition.x + radius*cos(dAngle);
        temPoint.y = m_ReferencePosition.y + radius*sin(dAngle);
        Cache->m_dToolAngle = dAngle*180.0/PI + 90.0;//dAngle + PI*0.5;

    }

    return 0;
}

/*************************************************
  函数名称：runG79(EmulationInterVariableCache *Cache, QVector<BaseAction *> &actions, double dI, double dJ, double dP, double dQ, double dD)
  函数功能：G79 直线冲孔：G79 I J P Q D T
  输入参数： Cache  解析（仿真）时临时变量存储数据
           dI、dJ、dP、dQ、dD  G79指令参数
  输出参数： actions  解析后产生的动作
  返 回 值：  0 成功
            -1 冲孔长度不能小于零
            -2 蚕食间距不能小于零
            -3 加工板厚不能大于冲裁间距
  备    注：
**************************************************/
int ActionPunchGCode::runG79(EmulationInterVariableCache *Cache, QVector<BaseAction *> &actions, double dI, double dJ, double dP, double dQ, double dD)
{
    if (dI <= 0)
        return -1;//冲孔长度不能小于零

    if (dQ < 0 || dQ > 10)
        return -2;//蚕食间距不能小于零

    if (dD > dQ)
        return -3;//加工板厚不能大于冲裁间距

    Point temPoint(0,0,0,0,0,0);

    double angle = PI*dJ/180;
    double cosAngle = cos(angle);
    double sinAngle = sin(angle);


    /*计算刀具中心点（刀具起点）与路径原点的距离*/
    double toolx = -dP*sinAngle/2;
    double tooly = dP*cosAngle/2;

    /*将参考坐标系移到刀具起点*/
    m_ReferencePosition.x += toolx;
    m_ReferencePosition.y += tooly;

    /*直线蚕食*/
    int nK = ceil(dI/dQ);
    //int nK = (int)(dI/dQ) + 1;
    double dL=dI/nK;
    double dx = dL*cosAngle;
    double dy = dL*sinAngle;
    for (int i=0; i <= nK; i++)
    {
        temPoint.x = m_ReferencePosition.x + i*dx;
        temPoint.y = m_ReferencePosition.y + i*dy;

    }

    /*将参考坐标系移到原来位置*/
    m_ReferencePosition.x -= toolx;
    m_ReferencePosition.y -= tooly;

    return 0;
}

