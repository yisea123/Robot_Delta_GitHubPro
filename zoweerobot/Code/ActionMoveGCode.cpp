/************************************************************
  Copyright (C),
  文件名：ActionMoveGCode.cpp
  作者：zhou    版本：V1.0    创建日期：2018/07/10
  模块描述：ActionMoveGCode类的实现
  历史修改记录：<author>    <time>    <version >    <desc>
*************************************************************/

#include "ActionMoveGCode.h"
#include "BaseGCode.h"
#include "BaseAction.h"
#include "MoveAction.h"
#include "IOAction.h"
#include "DelayAction.h"

ActionMoveGCode::ActionMoveGCode()
{
    m_GCodeType = GCODE_MOVE_TYPE;

    m_nMoveG = GCODE_MOVE_NULL;
    m_isMoveXAssign = FALSE;
    m_isMoveYAssign = FALSE;
    m_isMoveZAssign = FALSE;
    m_isMoveRAssign = FALSE;
    m_isMoveCAssign = FALSE;
    m_isMoveIAssign = FALSE;
    m_isMoveJAssign = FALSE;
    m_isMoveFAssign = FALSE;
    m_isMoveTAssign = FALSE;
    m_isMoveOAssign = FALSE;
    m_isMoveBAssign = FALSE;
    m_isMoveUAssign = FALSE;
    m_isMoveDAssign = FALSE;
    m_isMoveHAssign = FALSE;
    m_dX = 0;
    m_dY = 0;
    m_dZ = 0;
    m_dR = 0;
    m_dC = 0;
    m_dI = 0;
    m_dJ = 0;
    m_dF = 0;
    m_dT = 0;
    m_dO = 0;
    m_dB = 0;
    m_dU = 0;
    m_dD = 0;
    m_dH = 0;
    m_curToolInfo = NULL;
    m_prevToolInfo = NULL;
}

/*************************************************
  函数名称：getEndPosition()
  函数功能：获取位置信息
  输入参数：
  输出参数：
  返 回 值：m_EndPoint  记录的位置信息
  备    注：
**************************************************/
Point  ActionMoveGCode::getEndPosition()
{
    return m_EndPoint;
}

/*************************************************
  函数名称：getMoveXAssign()
  函数功能：m_isMoveXAssign是否以赋值
  输入参数：
  输出参数：
  返 回 值：  DONE_SUCCESS 成功
            DONE_FAIL  失败
  备    注：
**************************************************/
bool ActionMoveGCode::getMoveXAssign()
{
    return m_isMoveXAssign;
}

/*************************************************
  函数名称：getMoveYAssign()
  函数功能：m_isMoveYAssign是否以赋值
  输入参数：
  输出参数：
  返 回 值：  DONE_SUCCESS 成功
            DONE_FAIL  失败
  备    注：
**************************************************/
bool ActionMoveGCode::getMoveYAssign()
{
    return m_isMoveYAssign;
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
bool ActionMoveGCode::fillGCodeVarient(ParseGCodeInterVariableCache*  Cache, GCodeParseInfo& inf, ErrorInfo& e)
{
    QString s;

    /*用于语法错误检测，出现一次数值增加1*/
    int gFlag = 0;
    int xFlag = 0;
    int yFlag = 0;
    int zFlag = 0;
    int wFlag = 0;
    int pFlag = 0;
    int rFlag = 0;
    int cFlag = 0;
    int iFlag = 0;
    int jFlag = 0;
    int fFlag = 0;
    int tFlag = 0;
    int oFlag = 0;
    int dFlag = 0;
    int bFlag = 0;
    int uFlag = 0;
  
    int kFlag = 0;
    int hFlag = 0;
    for(int i = 0; i < inf.m_info.size(); i++)
    {
        s = inf.m_info.at(i).symbol;
        if(s == "G" || s == "g")
        {
            switch(inf.m_info.at(i).value.toInt())
            {
                case 0:
                {
                    m_nMoveG = GCODE_G00;
                    inf.m_info.removeAt(i--);

                    gFlag++;
                }break;
                case 1:
                {
                    m_nMoveG = GCODE_G01;
                    inf.m_info.removeAt(i--);

                    gFlag++;
                }break;
                case 2:
                {
                    m_nMoveG = GCODE_G02;
                    inf.m_info.removeAt(i--);

                    gFlag++;
                }break;
                case 3:
                {
                    m_nMoveG = GCODE_G03;
                    inf.m_info.removeAt(i--);

                    gFlag++;
                }break;
                case 4:
                {
                    m_nMoveG = GCODE_G04;
                    inf.m_info.removeAt(i--);

                    gFlag++;
                }break;
		case 5:
                {
                    m_nMoveG = GCODE_G05;
                    inf.m_info.removeAt(i--);

                    gFlag++;
                }break;

		case 6:
                {
                    m_nMoveG = GCODE_G06;
                    inf.m_info.removeAt(i--);

                    gFlag++;
                }break;
                /*case 25:
                {
                    m_nMoveG = GCODE_G25;
                    inf.m_info.removeAt(i--);

                    gFlag++;
                }break;
                case 27:
                {
                    m_nMoveG = GCODE_G27;
                    inf.m_info.removeAt(i--);

                    gFlag++;
                }break;*/
                case 50:
                {
                    m_nMoveG = GCODE_G50;
                    inf.m_info.removeAt(i--);

                    gFlag++;
                }break;
                case 70:
                {
                    m_nMoveG = GCODE_G70;
                    inf.m_info.removeAt(i--);

                    gFlag++;
                }break;
            }
        }
        else if(s == "X" || s == "x")
        {
            m_dX = inf.m_info.at(i).value.toDouble();
            Cache->m_LastPosition.x = m_dX;

            m_isMoveXAssign = TRUE;
            inf.m_info.removeAt(i--);

            xFlag++;
        }
        else if(s == "Y" || s == "y")
        {
            m_dY = inf.m_info.at(i).value.toDouble();
            Cache->m_LastPosition.y = m_dY;

            m_isMoveYAssign = TRUE;
            inf.m_info.removeAt(i--);

            yFlag++;
        }
	  else if(s == "Z" || s == "z")
        {
            m_dZ = inf.m_info.at(i).value.toDouble();
            Cache->m_LastPosition.z = m_dZ;

            m_isMoveZAssign = TRUE;
            inf.m_info.removeAt(i--);

            zFlag++;
        }
	   else if(s == "W" || s == "w")
        {
            m_dW = inf.m_info.at(i).value.toDouble();
	     Cache->m_LastPosition.w = m_dW;

	     m_isMoveWAssign = TRUE;
            inf.m_info.removeAt(i--);

            wFlag++;
        }
	 else if(s == "P" || s == "p")
        {
            m_dP = inf.m_info.at(i).value.toDouble();
	     Cache->m_LastPosition.p = m_dP;

	     m_isMovePAssign = TRUE;
            inf.m_info.removeAt(i--);

            pFlag++;
        }
        else if(s == "R" || s == "r")
        {
            m_dR = inf.m_info.at(i).value.toDouble();
            Cache->m_LastPosition.r = m_dR;

            m_isMoveRAssign = TRUE;
            inf.m_info.removeAt(i--);

            rFlag++;
        }
        /*else if(s == "C" || s == "c")
        {
            m_dC = inf.m_info.at(i).value.toDouble();
            Cache->m_dLastToolAngle = m_dC;

            m_isMoveCAssign = TRUE;
            inf.m_info.removeAt(i--);

            cFlag++;
        }*/
        else if(s == "I" || s == "i")
        {
            m_dI = inf.m_info.at(i).value.toDouble();
            m_isMoveIAssign = TRUE;
            inf.m_info.removeAt(i--);

            iFlag++;
        }
        else if(s == "J" || s == "j")
        {
            m_dJ = inf.m_info.at(i).value.toDouble();
            m_isMoveJAssign = TRUE;
            inf.m_info.removeAt(i--);

            jFlag++;
        }
        else if(s == "F" || s == "f")
        {
            m_dF = inf.m_info.at(i).value.toDouble();
	    Cache->m_dLastToolAngle=m_dF;
            m_isMoveFAssign = TRUE;
            inf.m_info.removeAt(i--);

            fFlag++;
        }
	 else if(s == "T" || s == "t")
        {
            m_dT = inf.m_info.at(i).value.toInt();
	     Cache->m_ToolLastSerialNum=m_dT;
            m_isMoveTAssign = TRUE;
            inf.m_info.removeAt(i--);

            tFlag++;
        }	
	else if(s == "O" || s == "o")//Output index
        {
            m_dO = inf.m_info.at(i).value.toInt();
            m_isMoveOAssign = TRUE;
            inf.m_info.removeAt(i--);

            oFlag++;
        }
	else if(s == "D" || s == "d")//Delay Input Wait Time:ms
        {
            m_dD = inf.m_info.at(i).value.toDouble();
            m_isMoveDAssign = TRUE;
            inf.m_info.removeAt(i--);

            dFlag++;
        }
	else if(s == "B" || s == "b")//Input Output bit value
        {
            m_dB = inf.m_info.at(i).value.toInt();
            m_isMoveBAssign = TRUE;
            inf.m_info.removeAt(i--);

            bFlag++;
        }
	else if(s == "U" || s == "u")//user tool index
        {
            m_dU = inf.m_info.at(i).value.toInt();
            m_isMoveUAssign = TRUE;
            inf.m_info.removeAt(i--);

            uFlag++;
        }
	
	else if(s == "K" || s == "k")//G25 G27 row number
        {
            m_dK = inf.m_info.at(i).value.toInt();
            m_isMoveKAssign = TRUE;
            inf.m_info.removeAt(i--);

            kFlag++;
        }
	else if(s == "H" || s == "h")//scara hand
        {
            m_dH= inf.m_info.at(i).value.toInt();
	    Cache->m_hand=m_dH;
            m_isMoveHAssign = TRUE;
            inf.m_info.removeAt(i--);

            hFlag++;
        }
    }

    if (Cache->m_bIsAbsCoordinate == ABSOLUTE_COORDINATE_REFERENCE)//绝对值坐标系继承上一个坐标值，相对值坐标都是0
    {
        if (m_isMoveXAssign == FALSE)
        {
            if(m_dU)
            {
	            	e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, QString("alarm14 : 第%1行 G指令格式错误!U=%2!But No X").arg(m_LineSerialNum).arg(m_dU));
	        	return FALSE;
            }
            m_dX = Cache->m_LastPosition.x;
        }
        if (m_isMoveYAssign == FALSE)
        {
             if(m_dU)
            {
	            	e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, QString("alarm14 : 第%1行 G指令格式错误!U=%2!But No Y").arg(m_LineSerialNum).arg(m_dU));
	        	return FALSE;
            }
            m_dY = Cache->m_LastPosition.y;
        }
	 if (m_isMoveZAssign == FALSE)
        {
            m_dZ = Cache->m_LastPosition.z;
        }
	 if (m_isMoveWAssign == FALSE)
        {
            m_dW = Cache->m_LastPosition.w;
        }
	 if (m_isMovePAssign == FALSE)
        {
            m_dP = Cache->m_LastPosition.p;
        }
        if (m_isMoveRAssign == FALSE)
        {
            m_dR = Cache->m_LastPosition.r;
        }
	 if (m_isMoveUAssign == FALSE)
        {
            m_dU = 0;
        }
	 if((m_dU)&&(m_nMoveG != GCODE_G25)&&(m_nMoveG != GCODE_G27))
	 {
	 	UserToRobot(m_dX,m_dY,m_dU,Cache);
		Cache->m_LastPosition.x = m_dX;
		Cache->m_LastPosition.y = m_dY;
	 }
        /*if (m_isMoveCAssign == FALSE)
        {
            m_dC = Cache->m_dLastToolAngle;
        }*/
    }
    if (m_isMoveTAssign == FALSE)
    {
        m_dT = Cache->m_ToolLastSerialNum;
    }
     if (m_isMoveFAssign == FALSE)
    {
        m_dF = Cache->m_dLastToolAngle;
    }
    if (m_isMoveDAssign == FALSE)
    {
        m_dD = 0;
    }
     if (m_isMoveHAssign == FALSE)
    {
        m_dH = Cache->m_hand;
    }
    if (gFlag > 1)
    {
        e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, QString("alarm14 : 第%1行 G指令重复出现!").arg(m_LineSerialNum));
        return FALSE;
    }
    if (xFlag > 1 || yFlag > 1 ||zFlag > 1 || rFlag > 1 || cFlag > 1 || iFlag > 1 || jFlag > 1 || fFlag > 1|| tFlag > 1)
    {
        e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, QString("alarm14 : 第%1行 G指令格式错误!").arg(m_LineSerialNum));
        return FALSE;
    }
    if((xFlag  || yFlag ||zFlag || rFlag  || fFlag|| tFlag )&&(oFlag|| bFlag ))
    {
        e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, QString("alarm14 : 第%1行 G指令格式错误!").arg(m_LineSerialNum));
        return FALSE;
    }
    if((m_nMoveG == GCODE_G25||m_nMoveG == GCODE_G27)&&((!pFlag)||(!kFlag)||(!iFlag)||(!jFlag)))
    {
        e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, QString("alarm14 : 第%1行 G25/G27指令格式错误!").arg(m_LineSerialNum));
        return FALSE;
    }
    return  ActionReferenceGCode::fillGCodeVarient(Cache, inf, e);

}

int ActionMoveGCode::User2Robot(IN  CartesianPose* i_CUser, OUT  CartesianPose* i_CPos, IN CartesianPose* i_UserTrans)
{
	i_CPos->X=i_CUser->X*i_UserTrans->X-i_CUser->Y*i_UserTrans->Y+i_UserTrans->Z;//x=xv*a-yv*b+c;
	i_CPos->Y=i_CUser->X*i_UserTrans->Y+i_CUser->Y*i_UserTrans->X+i_UserTrans->W;//y=xv*b+yv*a+d;	
}

int ActionMoveGCode::UserToRobot(double &x,   double &y, int userindex,ParseGCodeInterVariableCache*  Cache)
{
	CartesianPose i_CUser;
	CartesianPose i_CPos;
	CartesianPose i_UserTrans=Cache->m_userParam[userindex];
	i_CUser.X=x;
	i_CUser.Y=y;

	User2Robot(&i_CUser,&i_CPos,&i_UserTrans);
	x=i_CPos.X;
	y=i_CPos.Y;		
} 

int ActionMoveGCode::UserToRobot(double &x,   double &y, int userindex,EmulationInterVariableCache*  Cache)
{
	CartesianPose i_CUser;
	CartesianPose i_CPos;
	CartesianPose i_UserTrans=Cache->m_userParam[userindex];
	i_CUser.X=x;
	i_CUser.Y=y;

	User2Robot(&i_CUser,&i_CPos,&i_UserTrans);
	x=i_CPos.X;
	y=i_CPos.Y;		
}

bool ActionMoveGCode::genrateAction(EmulationInterVariableCache* Cache, QVector<BaseAction *>& actions,ErrorInfo& e)
{
    bool b = ActionReferenceGCode::genrateAction(Cache, actions, e);
    //int errNum=0;

    if(m_nMoveG == GCODE_MOVE_NULL)
    {
        if(Cache->m_MoveType != GCODE_MOVE_NULL)
        {
            m_nMoveG = Cache->m_MoveType;
        }
        else
        {
            m_nMoveG = GCODE_G00;
        }
    }

    
    if(b)
    {
        switch(m_nMoveG)
        {
            case GCODE_G00:
            {
                Cache->m_MoveType = GCODE_G00;
                return moveToPoint(Cache, actions, Point(m_dX, m_dY,m_dZ, m_dW,m_dP, m_dR), e);
            }
            case GCODE_G01:
            {
                Cache->m_MoveType = GCODE_G01;
                return lineMove(Cache, actions, e);
            }break;
            case GCODE_G02:
            {
                Cache->m_MoveType = GCODE_G02;
                return arcMove(Cache, actions, e);
            }break;
            case GCODE_G03:
            {
                Cache->m_MoveType = GCODE_G03;
                return arcMove(Cache, actions, e);
            }break;
            case GCODE_G04:
            {
                Cache->m_MoveType = GCODE_G04;
                return delayAction(Cache, actions, e);
            }break;
	     case GCODE_G05:
            {
                Cache->m_MoveType = GCODE_G05;
                return ioAction(Cache, actions, e);
            }break;
	     case GCODE_G06:
            {
                Cache->m_MoveType = GCODE_G06;
                return ioAction(Cache, actions, e);
            }break;
            /*case GCODE_G25:
            {
		 if (m_dP<= 0 || m_dK<= 0)
               {
                   e.insertDebug(DEBUG_NG_PUNCH_FILE_ALARM, QString("ActionPunchGCode::genrateAction : 第%1行 G25（X方向栅格冲孔）的参数P/K错误").arg(m_LineSerialNum));
                   return false;
               }
               runG25(Cache, actions, m_dI, m_dP, m_dJ, m_dK,e);
               return true;
           
            }break;
            case GCODE_G27:
            {
		 if (m_dP<= 0 || m_dK<= 0)
               {
                   e.insertDebug(DEBUG_NG_PUNCH_FILE_ALARM, QString("ActionPunchGCode::genrateAction : 第%1行 G25（X方向栅格冲孔）的参数P/K错误").arg(m_LineSerialNum));
                   return false;
               }
               runG27(Cache, actions, m_dI, m_dP, m_dJ, m_dK,e);
               return true;
  
            }break;*/
            case GCODE_G70:
            {
                Cache->m_MoveType = GCODE_G70;
                return moveToPoint(Cache, actions, Point(m_dX, m_dY,m_dZ,m_dW,m_dP,m_dR), e);
                //return true;
            }break;
            case GCODE_G50:
            {
                Cache->m_MoveType = GCODE_G50;
                return moveToPoint(Cache, actions, (Point(0,0,0,0,0,0) - Cache->m_MachineCoordinate - Cache->m_WorkCoordinate), e);//回机械原点
                //return moveToPoint(Cache, actions, (Point(0.0, 0.0) - Cache->m_WorkCoordinate), e);//回工件原点
                //return true;
            }break;
            case GCODE_MOVE_NULL:
            {
                return false;
            }break;
        }
    }
    else
    {
        return false;
    }
}

bool ActionMoveGCode::moveToPoint(EmulationInterVariableCache* Cache, QVector<BaseAction *>& actions, Point point, ErrorInfo& e)
{
    if (Cache->m_bIsAbsCoordinate == ABSOLUTE_COORDINATE_REFERENCE)
    {
        Point endP = point + Cache->m_WorkCoordinate + Cache->m_MachineCoordinate;
        if (fabs(endP.x - Cache->m_LastPosition.x) > 0.001 || fabs(endP.y - Cache->m_LastPosition.y) > 0.001
			||fabs(endP.z - Cache->m_LastPosition.z) > 0.001 || fabs(endP.w - Cache->m_LastPosition.w) > 0.001
			||fabs(endP.p - Cache->m_LastPosition.p) > 0.001 || fabs(endP.r - Cache->m_LastPosition.r) > 0.001)
        {
            MoveAction *move = new MoveAction;
            move->m_nLineSerialNum = m_LineSerialNum;

            move->m_startPoint = Cache->m_LastPosition;
            move->m_endPoint = (point + Cache->m_WorkCoordinate + Cache->m_MachineCoordinate);
            Cache->m_LastPosition = move->m_endPoint;
            move->m_basePoint = Cache->m_LastPosition;
            //move->m_tool = getToolInfoById(Cache, Cache->m_nToolNum, e);
	     move->m_toolSerialNum=m_dT;//
	     move->m_dAngle = m_dF;//Cache->m_dToolAngle;
            if(m_dD>0.000001) move->m_delaytime=(int)(m_dD*1000);
	     move->m_hand=m_dH;	
            actions.push_back(move);

            Cache->m_ReferenceCoordinate = point;
        }
    }
    else
    {
        if (fabs(point.x) > 0.001 || fabs(point.y) > 0.001||fabs(point.z) > 0.001 || fabs(point.w) > 0.001||fabs(point.p) > 0.001 || fabs(point.r) > 0.001)
        {
            MoveAction *move = new MoveAction;
            move->m_nLineSerialNum = m_LineSerialNum;

            move->m_startPoint = Cache->m_LastPosition;
            move->m_endPoint = move->m_startPoint + point;
            Cache->m_LastPosition = move->m_endPoint;
            move->m_basePoint = Cache->m_LastPosition;
            //move->m_tool = getToolInfoById(Cache, Cache->m_nToolNum, e);
	     move->m_toolSerialNum=m_dT;//	
	     move->m_dAngle = m_dF;//Cache->m_dToolAngle;
             if(m_dD>0.000001) move->m_delaytime= (int)(m_dD*1000);
	      move->m_hand=m_dH;	
            actions.push_back(move);

            Cache->m_ReferenceCoordinate = move->m_endPoint - Cache->m_WorkCoordinate - Cache->m_MachineCoordinate;
        }
    }

    return true;
}

bool ActionMoveGCode::lineMove(EmulationInterVariableCache* Cache, QVector<BaseAction *>& actions, ErrorInfo& e)
{
    MoveAction *move = new MoveAction;

    move->m_nLineSerialNum = m_LineSerialNum;
    move->m_moveType = MOVE_LINE;
    move->m_startPoint = Cache->m_LastPosition;

    if (Cache->m_bIsAbsCoordinate == ABSOLUTE_COORDINATE_REFERENCE)
    {
        move->m_endPoint = (Point(m_dX, m_dY,m_dZ,m_dW,m_dP,m_dR) + Cache->m_WorkCoordinate + Cache->m_MachineCoordinate);
        Cache->m_ReferenceCoordinate = Point(m_dX, m_dY,m_dZ,m_dW,m_dP,m_dR);
    }
    else
    {
        move->m_endPoint = move->m_startPoint + Point(m_dX, m_dY,m_dZ,m_dW,m_dP,m_dR);
        Cache->m_ReferenceCoordinate = move->m_endPoint - Cache->m_WorkCoordinate - Cache->m_MachineCoordinate;
    }
    Cache->m_LastPosition = move->m_endPoint;
    move->m_basePoint = Cache->m_LastPosition;
    //move->m_tool = getToolInfoById(Cache, Cache->m_nToolNum, e);
    move->m_toolSerialNum=m_dT;//
    move->m_dAngle = m_dF;//Cache->m_dToolAngle;
    if(m_dD>0.000001) move->m_delaytime= (int)(m_dD*1000);
     move->m_hand=m_dH;	
    actions.push_back(move);

    return true;
}

bool ActionMoveGCode::delayAction(EmulationInterVariableCache* Cache, QVector<BaseAction *>& actions, ErrorInfo& e)
{
    DelayAction *delay = new DelayAction;
    delay->m_nDelayTime = (int)(m_dD*1000);//m_dX * 1000;//单位化为ms
    delay->m_nLineSerialNum = m_LineSerialNum;

    actions.push_back(delay);
    return true;
}

bool ActionMoveGCode::ioAction(EmulationInterVariableCache* Cache, QVector<BaseAction *>& actions, int ioNum, bool sts)
{
//    return true;
    IOAction *action = new IOAction;
    action->m_nIONum = ioNum;
    action->m_bIoSts = sts;
    action->m_nCheckIONum = ioNum;
    action->m_bCheckIoSts = sts;
    action->m_nLineSerialNum = m_LineSerialNum;
    action->m_basePoint = Cache->m_LastPosition;

    actions.push_back(action);
    return true;
}

bool ActionMoveGCode::ioAction(EmulationInterVariableCache* Cache, QVector<BaseAction *>& actions, int ioNum, bool sts, int checkIONum)
{
//    return true;
    IOAction *action = new IOAction;
    action->m_nIONum = ioNum;
    action->m_bIoSts = sts;
    action->m_nLineSerialNum = m_LineSerialNum;
    action->m_nCheckIONum = checkIONum;
    action->m_bCheckIoSts = sts;
    action->m_basePoint = Cache->m_LastPosition;

    actions.push_back(action);
    return true;
}

bool ActionMoveGCode::ioAction(EmulationInterVariableCache* Cache, QVector<BaseAction *>& actions, int ioNum, bool sts, int checkIONum, bool checkSts)
{
//    return true;
    IOAction *action = new IOAction;
    action->m_nIONum = ioNum;
    action->m_bIoSts = sts;
    action->m_nLineSerialNum = m_LineSerialNum;
    action->m_nCheckIONum = checkIONum;
    action->m_bCheckIoSts = checkSts;
    action->m_basePoint = Cache->m_LastPosition;

    actions.push_back(action);
    return true;
}

bool ActionMoveGCode::ioAction(EmulationInterVariableCache* Cache, QVector<BaseAction *>& actions, ErrorInfo& e)
{
//    return true;
    IOAction *action = new IOAction;
    if(Cache->m_MoveType ==GCODE_G05)
    {
            action->m_nIoType=IO_OUT;
	    action->m_nIONum = m_dO;
	    action->m_bIoSts = (m_dB)?1:0;
    }
    else if(Cache->m_MoveType ==GCODE_G06)
    {
    	    action->m_nIoType=IO_IN;
	    action->waitTime= (int)(m_dD*1000);//m_dD;
	    action->m_nCheckIONum = m_dO;
	    action->m_bCheckIoSts = (m_dB)?1:0;
    }
    action->m_nLineSerialNum = m_LineSerialNum;
    action->m_basePoint = Cache->m_LastPosition;

    actions.push_back(action);
    return true;
}
bool ActionMoveGCode::arcMove(EmulationInterVariableCache* Cache, QVector<BaseAction *>& actions, ErrorInfo& e)
{
    MoveAction *move = new MoveAction;

    move->m_nLineSerialNum = m_LineSerialNum;

    if (m_nMoveG == GCODE_G02)//顺时针圆弧
    {
        move->m_moveType = MOVE_C_ARC;
    }
    else//逆时针圆弧
    {
        move->m_moveType = MOVE_CC_ARC;
    }

    move->m_startPoint = Cache->m_LastPosition;

    if (Cache->m_bIsAbsCoordinate == ABSOLUTE_COORDINATE_REFERENCE)//绝对坐标系
    {
        move->m_endPoint = (Point(m_dX, m_dY,m_dZ,m_dW,m_dP,m_dR) + Cache->m_WorkCoordinate + Cache->m_MachineCoordinate);
        Cache->m_ReferenceCoordinate = Point(m_dX, m_dY,m_dZ,m_dW,m_dP,m_dR);
    }
    else//相对坐标系
    {
        move->m_endPoint = move->m_startPoint + Point(m_dX, m_dY,m_dZ,m_dW,m_dP,m_dR);
        Cache->m_ReferenceCoordinate = move->m_endPoint - Cache->m_WorkCoordinate - Cache->m_MachineCoordinate;
    }
    move->m_centerPoint = move->m_startPoint + Point(m_dI, m_dJ,0,0,0,0);
    move->m_dRadius = sqrt(m_dI * m_dI + m_dJ * m_dJ);

    Point tempDelPoint = move->m_endPoint - move->m_centerPoint;
    double tempRadius = sqrt(tempDelPoint.x * tempDelPoint.x + tempDelPoint.y * tempDelPoint.y);
    if (fabs(tempRadius - move->m_dRadius) > 0.001)
    {
        delete move;
        e.insertDebug(DEBUG_NG_PUNCH_FILE_ALARM, QString("ActionPunchGCode::genrateAction : 第%1行 G02参数错误，终点不在圆上").arg(m_LineSerialNum));
        return false;//-1; G02/G03参数错误，终点不在圆上
    }
    if (move->m_dRadius < 0.001)
    {
        delete move;
        e.insertDebug(DEBUG_NG_PUNCH_FILE_ALARM, QString("ActionPunchGCode::genrateAction : 第%1行 G02参数错误，圆弧半径太小").arg(m_LineSerialNum));
        return false;// -2; G02/G03参数错误，圆弧半径太小
    }

    move->m_basePoint = Cache->m_LastPosition;
    //move->m_tool = getToolInfoById(Cache, Cache->m_nToolNum, e);
    move->m_toolSerialNum=m_dT;//
    move->m_dAngle = Cache->m_dToolAngle;

    actions.push_back(move);

    return true;
}

int ActionMoveGCode::runG25(EmulationInterVariableCache *Cache, QVector<BaseAction *> &actions, double dI, int nP, double dJ, int nK,ErrorInfo& e)
{
    Point temPoint,startPoint;
    int i=0, j=0;

    startPoint.x = m_dX;
    startPoint.y = m_dY;
    startPoint.z = m_dZ;
    startPoint.r = m_dR;

   /* temPoint.x = m_dX;
    temPoint.y = m_dY;
    temPoint.z = m_dZ;
    temPoint.r = m_dR;*/
    temPoint=startPoint;
    if(m_dU)
    {
    	 UserToRobot(temPoint.x,temPoint.y,m_dU,Cache);
    }	
    moveToPoint(Cache, actions, temPoint, e);
    		
	
    for (i=0; i<=nK; i++)
    {
        for (j=1; j<=nP; j++)
        {
            if (i%2 == 0)
            {
                startPoint.x += dI;
            }
            else
            {
                startPoint.x -= dI;
            }
	     temPoint=startPoint;
	    if(m_dU)
	    {
	    	 UserToRobot(temPoint.x,temPoint.y,m_dU,Cache);
	    }	
	     moveToPoint(Cache, actions, temPoint, e);
        }

        if(i < nK)
        {
            startPoint.y += dJ;
	    temPoint=startPoint;
	    if(m_dU)
	    {
	    	 UserToRobot(temPoint.x,temPoint.y,m_dU,Cache);
	    }	
	    moveToPoint(Cache, actions, temPoint, e);
        }
    }

    return 0;
}

int ActionMoveGCode::runG27(EmulationInterVariableCache *Cache, QVector<BaseAction *> &actions, double dI, int nP, double dJ, int nK,ErrorInfo& e)
{
    Point temPoint,startPoint;
    int i=0, j=0;

    startPoint.x = m_dX;
    startPoint.y = m_dY;
    startPoint.z = m_dZ;
    startPoint.r = m_dR;

   /* temPoint.x = m_dX;
    temPoint.y = m_dY;
    temPoint.z = m_dZ;
    temPoint.r = m_dR;*/
    temPoint=startPoint;
    if(m_dU)
    {
    	 UserToRobot(temPoint.x,temPoint.y,m_dU,Cache);
    }	
    moveToPoint(Cache, actions, temPoint, e);
 
    for (j=0; j<=nP; j++)
    {
        for (i=1; i<=nK; i++)
        {
            if (j%2 == 0)
            {
                startPoint.y += dJ;
            }
            else
            {
                startPoint.y -= dJ;
            }
             temPoint=startPoint;
	    if(m_dU)
	    {
	    	 UserToRobot(temPoint.x,temPoint.y,m_dU,Cache);
	    }
	     moveToPoint(Cache, actions, temPoint, e);
        }

        if(j < nP)
        {
            startPoint.x += dI;
            temPoint=startPoint;
	    if(m_dU)
	    {
	    	 UserToRobot(temPoint.x,temPoint.y,m_dU,Cache);
	    }
	    moveToPoint(Cache, actions, temPoint, e);
        }
    }

    return 0;
}

ToolInfo*  ActionMoveGCode::getToolInfoById(EmulationInterVariableCache* Cache, int id)
{
    int i;
    for(i = 0; i < Cache->m_ToolsInfo.size(); i++)
    {
        if(id == Cache->m_ToolsInfo.at(i)->getSerialNum())
        {
            break;
        }
    }

    if(i == Cache->m_ToolsInfo.size())
    {
        qDebug()<<"alarm15 : 模具库中没有模具T!"<<id;
        return NULL;
    }
    return Cache->m_ToolsInfo.at(i);
}

ToolInfo*  ActionMoveGCode::getToolInfoById(EmulationInterVariableCache* Cache, int id, ErrorInfo& e)
{
    int i;
    for(i = 0; i < Cache->m_ToolsInfo.size(); i++)
    {
        if(id == Cache->m_ToolsInfo.at(i)->getSerialNum())
        {
            break;
        }
    }

    if(i == Cache->m_ToolsInfo.size())
    {
        e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, QString("alarm15 : 模具库中没有模具T%1!").arg(id));
        return NULL;
    }
    return Cache->m_ToolsInfo.at(i);
}

QString  ActionMoveGCode::toDebugString()
{
    QString  ss;

    ss += "移动代码：\n";
    ss += "移动类型：";
    ss += GCodeMoveString[m_nMoveG];
    ss += ",X：";
    ss += ss.number(m_dX);
    ss += ",Y：";
    ss += ss.number(m_dY);

    ss += ",C: ";
    if(m_isMoveCAssign)
    {
        ss += ss.number(m_dC);
    }
    else
    {
        ss += "NULL";
    }
    ss += ",I: ";
    if(m_isMoveIAssign)
    {
        ss += ss.number(m_dI);
    }
    else
    {
        ss += "NULL";
    }
    ss += ",J: ";
    if(m_isMoveJAssign)
    {
        ss += ss.number(m_dJ);
    }
    else
    {
        ss += "NULL";
    }
    ss += ",F: ";
    if(m_isMoveFAssign)
    {
        ss += ss.number(m_dF);
    }
    else
    {
        ss += "NULL";
    }
    ss += "\n";

    return  ActionReferenceGCode::toDebugString() + ss;
}
