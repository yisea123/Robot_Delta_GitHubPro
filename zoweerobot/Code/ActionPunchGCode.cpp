/************************************************************
  Copyright (C),
  �ļ�����ActionPunchGCode.cpp
  ���ߣ�zhou    �汾��V1.0    �������ڣ�2018/07/10
  ģ��������ActionPunchGCode���ʵ��
  ��ʷ�޸ļ�¼��<author>    <time>    <version >    <desc>
                  zhou    2018/07/10    V1.0      ���ע��
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
  �������ƣ�ActionPunchGCode()
  �������ܣ����캯��
  ���������
  ���������
  �� �� ֵ��
  ��    ע��
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
  �������ƣ�fillGCodeVarient(ParseGCodeInterVariableCache*  Cache, GCodeParseInfo& inf, ErrorInfo& e)
  �������ܣ���������ı������и�ֵ
  ��������� Cache  ����ʱ��ʱ�����洢����
  ��������� inf   һ��G����ָ��洢����
           e     �������
  �� �� ֵ��  TRUE �ɹ�
            FALSE  ʧ��
  ��    ע��
**************************************************/
bool  ActionPunchGCode::fillGCodeVarient(ParseGCodeInterVariableCache*  Cache, GCodeParseInfo& inf, ErrorInfo& e)
{
    QString s;

    /*�����﷨�����⣬����һ����ֵ����1*/
    int gFlag = 0;
    int pFlag = 0;
    int qFlag = 0;
    int kFlag = 0;
    int dFlag = 0;
    int iFlag = 0;
    int jFlag = 0;
    int tFlag = 0;

    m_nT = Cache->m_ToolLastSerialNum;//����ȱʡֵ����һ��������ͬ

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
                    //gFlag++;//�˴������ظ�
                }break;
                default:
                {
                    e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, QString("alarm14 : ��%1�� ����δ֧��Gָ��!").arg(m_LineSerialNum));
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

    /*G�����ʽУ��*/
    if(!Cache->m_isSelectTool)
    {
        e.insertDebug(DEBUG_NG_PUNCH_FILE_ALARM, QString("alarm14 : ��%1�� û��ѡ�񵶾�").arg(m_LineSerialNum));
        return FALSE;
    }
    if (gFlag > 1)
    {
        e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, QString("alarm14 : ��%1�� Gָ���ظ�����!").arg(m_LineSerialNum));
        return FALSE;
    }
    if (tFlag > 1)
    {
        e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, QString("alarm14 : ��%1�� �����ظ�����!").arg(m_LineSerialNum));
        return FALSE;
    }
    switch (m_nPunchG)
    {
    case GCODE_PUNCH_NULL:
        break;
    case GCODE_G28:    //G28 ���ԽǶȳ�ã�G28 I  J  K  T
        if ((pFlag + qFlag + dFlag) > 0)
        {
            e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, QString("alarm14 : ��%1�� G28ָ���ʽ����!").arg(m_LineSerialNum));
            return FALSE;
        }
        if ((iFlag > 1) || (jFlag > 1) || (kFlag > 1))
        {
            e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, QString("alarm14 : ��%1�� G28ָ���ʽ����!").arg(m_LineSerialNum));
            return FALSE;
        }
        break;
    case GCODE_G38:    //G38 ���ԽǶȳ�ã�G38 I J K T
        if ((pFlag + qFlag + dFlag) > 0)
        {
            e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, QString("alarm14 : ��%1�� G38ָ���ʽ����!").arg(m_LineSerialNum));
            return FALSE;
        }
        if ((iFlag > 1) || (jFlag > 1) || (kFlag > 1))
        {
            e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, QString("alarm14 : ��%1�� G38ָ���ʽ����!").arg(m_LineSerialNum));
            return FALSE;
        }
        break;
    case GCODE_G29:    //G29 Բ���ȷֳ�ף�G29 I J P K T
        if ((qFlag + dFlag) > 0)
        {
            e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, QString("alarm14 : ��%1�� G29ָ���ʽ����!").arg(m_LineSerialNum));
            return FALSE;
        }
        if ((iFlag > 1) || (jFlag > 1) || (pFlag > 1) || (kFlag > 1))
        {
            e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, QString("alarm14 : ��%1�� G29ָ���ʽ����!").arg(m_LineSerialNum));
            return FALSE;
        }
        break;
    case GCODE_G39:    //G39 Բ���ȷֳ�ף�G39 I J P K T
        if ((qFlag + dFlag) > 0)
        {
            e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, QString("alarm14 : ��%1�� G39ָ���ʽ����!").arg(m_LineSerialNum));
            return FALSE;
        }
        if ((iFlag > 1) || (jFlag > 1) || (pFlag > 1) || (kFlag > 1))
        {
            e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, QString("alarm14 : ��%1�� G39ָ���ʽ����!").arg(m_LineSerialNum));
            return FALSE;
        }
        break;
    case GCODE_G26:    //G26 Բ�ܵȷֳ�ף�G26 I J K T
        if ((pFlag + qFlag + dFlag) > 0)
        {
            e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, QString("alarm14 : ��%1�� G26ָ���ʽ����!").arg(m_LineSerialNum));
            return FALSE;
        }
        if ((iFlag > 1) || (jFlag > 1) || (kFlag > 1))
        {
            e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, QString("alarm14 : ��%1�� G26ָ���ʽ����!").arg(m_LineSerialNum));
            return FALSE;
        }
        break;
    case GCODE_G36:    //G36 X����դ���ף�G36 I P J K T
        if ((qFlag + dFlag) > 0)
        {
            e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, QString("alarm14 : ��%1�� G36ָ���ʽ����!").arg(m_LineSerialNum));
            return FALSE;
        }
        if ((iFlag > 1) || (jFlag > 1) || (pFlag > 1) || (kFlag > 1))
        {
            e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, QString("alarm14 : ��%1�� G36ָ���ʽ����!").arg(m_LineSerialNum));
            return FALSE;
        }
        break;
    case GCODE_G37:    //G37 Y����դ���ף�G37 I P J K T
        if ((qFlag + dFlag) > 0)
        {
            e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, QString("alarm14 : ��%1�� G37ָ���ʽ����!").arg(m_LineSerialNum));
            return FALSE;
        }
        if ((iFlag > 1) || (jFlag > 1) || (pFlag > 1) || (kFlag > 1))
        {
            e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, QString("alarm14 : ��%1�� G37ָ���ʽ����!").arg(m_LineSerialNum));
            return FALSE;
        }
        break;
    case GCODE_G66:    //G66 ֱ�߼��У�G66 I J P Q D T || G66 I J P K T
        if (kFlag > 1)
        {
            e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, QString("alarm14 : ��%1�� G66ָ���ʽ����!").arg(m_LineSerialNum));
            return FALSE;
        }
        else if (kFlag == 1)
        {
            if ((qFlag + dFlag) > 0)
            {
                e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, QString("alarm14 : ��%1�� G66ָ���ʽ����!").arg(m_LineSerialNum));
                return FALSE;
            }
            if ((iFlag > 1) || (jFlag > 1) || (pFlag > 1))
            {
                e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, QString("alarm14 : ��%1�� G66ָ���ʽ����!").arg(m_LineSerialNum));
                return FALSE;
            }
        }
        else
        {
            if ((iFlag > 1) || (jFlag > 1) || (pFlag > 1) || (qFlag > 1) || (dFlag > 1))
            {
                e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, QString("alarm14 : ��%1�� G66ָ���ʽ����!").arg(m_LineSerialNum));
                return FALSE;
            }
        }
        break;
    case GCODE_G67:    //G67 �����μ��У�G67 I J P T
        if ((kFlag + qFlag + dFlag) > 0)
        {
            e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, QString("alarm14 : ��%1�� G67ָ���ʽ����!").arg(m_LineSerialNum));
            return FALSE;
        }
        if ((iFlag > 1) || (jFlag > 1) || (pFlag > 1))
        {
            e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, QString("alarm14 : ��%1�� G67ָ���ʽ����!").arg(m_LineSerialNum));
            return FALSE;
        }
        break;
    case GCODE_G68:    //G68 Բ����ʳ��G68 I J k P Q T
        if (dFlag > 0)
        {
            e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, QString("alarm14 : ��%1�� G68ָ���ʽ����!").arg(m_LineSerialNum));
            return FALSE;
        }
        if ((iFlag > 1) || (jFlag > 1) || (kFlag > 1) || (pFlag > 1) || (qFlag > 1))
        {
            e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, QString("alarm14 : ��%1�� G68ָ���ʽ����!").arg(m_LineSerialNum));
            return FALSE;
        }
        break;
    case GCODE_G69:    //G69 ֱ�߲�ʳ��G69 I J P Q T
        if ((kFlag + dFlag) > 0)
        {
            e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, QString("alarm14 : ��%1�� G69ָ���ʽ����!").arg(m_LineSerialNum));
            return FALSE;
        }
        if ((iFlag > 1) || (jFlag > 1) || (pFlag > 1) || (qFlag > 1))
        {
            e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, QString("alarm14 : ��%1�� G69ָ���ʽ����!").arg(m_LineSerialNum));
            return FALSE;
        }
        break;
    case GCODE_G78:    //G78 Բ����ף�G78 I J k P Q D T
        if ((iFlag > 1) || (jFlag > 1) || (kFlag > 1) || (pFlag > 1) || (qFlag > 1) || (dFlag > 1))
        {
            e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, QString("alarm14 : ��%1�� G78ָ���ʽ����!").arg(m_LineSerialNum));
            return FALSE;
        }
        break;
    case GCODE_G79:    //G79 ֱ�߳�ף�G79 I J P Q D T
        if (kFlag > 0)
        {
            e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, QString("alarm14 : ��%1�� G79ָ���ʽ����!").arg(m_LineSerialNum));
            return FALSE;
        }
        if ((iFlag > 1) || (jFlag > 1) || (dFlag > 1) || (pFlag > 1) || (qFlag > 1))
        {
            e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, QString("alarm14 : ��%1�� G79ָ���ʽ����!").arg(m_LineSerialNum));
            return FALSE;
        }
        break;
    default:
        break;
    }

    return ActionMoveGCode::fillGCodeVarient(Cache, inf, e);
}

/*************************************************
  �������ƣ�genrateAction(EmulationInterVariableCache* Cache, QVector<BaseAction *>& actions, ErrorInfo& e)
  �������ܣ�����G���루��ѹ���ִ��룩
  ��������� Cache  ���������棩ʱ��ʱ�����洢����
  ��������� actions  ����������Ķ���
           e     �������
  �� �� ֵ��  TRUE �ɹ�
            FALSE  ʧ��
  ��    ע��
**************************************************/
bool ActionPunchGCode::genrateAction(EmulationInterVariableCache* Cache, QVector<BaseAction *>& actions, ErrorInfo& e)
{
    int errNum=0;

    m_ReferencePosition.x = Cache->m_ReferenceCoordinate.x;
    m_ReferencePosition.y = Cache->m_ReferenceCoordinate.y;

    m_prevToolInfo = m_curToolInfo;
    m_curToolInfo = getToolInfoById(Cache, m_nT, e);
    if (e.e_type != DEBUG_NO_ERROR)//��ȷ��
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
                    ioAction(Cache, actions, IO_DINGWEIXIAO_PLC_M, true, IO_DINGWEIXIAO_SIGNAL_PLC_M);//������
                    toolAngleRotary(Cache, actions, 0);//C��ת�����

                    if (m_curToolInfo->isRotaryTool())//C����Ϻϣ������תģ�Ļ���
                        ioAction(Cache, actions, IO_AXISCLIHE_PLC_M, true, IO_AXIS_C_JIEHE_SIGNAL_PLC_M);
                }
            }
            return true;
        }break;
        case GCODE_PUNCH_NULL:    //ֱ�ӳ�ѹ
        {
            if(m_nMoveG == GCODE_MOVE_NULL)
            {
                m_nMoveG = GCODE_G00;
            }


            //ActionMoveGCode::genrateAction(Cache, actions, e);    //�˶�����ѹλ��
            //toolPunch(Cache, actions);    //��ѹ����

            return true;
        }break;
        case GCODE_NOT_PUNCH:
        {
            Cache->m_MoveType = GCODE_G70;
            return moveToPoint(Cache, actions, Point(m_dX, m_dY,m_dZ,m_dW,m_dP,m_dR), e);
        }break;

        case GCODE_G28:    //G28 ���ԽǶȳ��
        {
            runG28(Cache, actions, m_dI, m_dJ, m_sK.toInt());
            return true;
        }break;

        case GCODE_G38:    //G38 ���ԽǶȳ��
        {
            runG38(Cache, actions, m_dI, m_dJ, m_sK.toInt());
            return true;
        }break;

        case GCODE_G29:    //G29 Բ���ȷֳ��
        {
            if (m_dI <= 0)
            {
                e.insertDebug(DEBUG_NG_PUNCH_FILE_ALARM, QString("ActionPunchGCode::genrateAction : ��%1�� G29��Բ���ȷֳ�ף��Ĳ���I����").arg(m_LineSerialNum));
                return false;
            }
            if (m_sK.toInt() <= 0)
            {
                e.insertDebug(DEBUG_NG_PUNCH_FILE_ALARM, QString("ActionPunchGCode::genrateAction : ��%1�� G29��Բ���ȷֳ�ף��Ĳ���K����").arg(m_LineSerialNum));
                return false;
            }
            runG29(Cache, actions, m_dI, m_dJ, m_sP.toDouble(), m_sK.toInt());
            return true;
        }break;
        case GCODE_G39:    //G39 Բ���ȷֳ��
        {
            if (m_dI <= 0)
            {
                e.insertDebug(DEBUG_NG_PUNCH_FILE_ALARM, QString("ActionPunchGCode::genrateAction : ��%1�� G39��Բ���ȷֳ�ף��Ĳ���I����").arg(m_LineSerialNum));
                return false;
            }
            if (m_sK.toInt() <= 0)
            {
                e.insertDebug(DEBUG_NG_PUNCH_FILE_ALARM, QString("ActionPunchGCode::genrateAction : ��%1�� G39��Բ���ȷֳ�ף��Ĳ���K����").arg(m_LineSerialNum));
                return false;
            }
            runG39(Cache, actions, m_dI, m_dJ, m_sP.toDouble(), m_sK.toInt());
            return true;
        }break;

        case GCODE_G26:    //G26 Բ�ܵȷֳ��
        {
            if (m_dI <= 0)
            {
                e.insertDebug(DEBUG_NG_PUNCH_FILE_ALARM, QString("ActionPunchGCode::genrateAction : ��%1�� G26��Բ�ܵȷֳ�ף��Ĳ���I����").arg(m_LineSerialNum));
                return false;
            }
            runG26(Cache, actions, m_dI, m_dJ, m_sK.toInt());
            return true;
        }break;

        case GCODE_G36:    //G36 X����դ����
        {
            if (m_sP.toInt() <= 0 || m_sK.toInt() <= 0)
            {
                e.insertDebug(DEBUG_NG_PUNCH_FILE_ALARM, QString("ActionPunchGCode::genrateAction : ��%1�� G36��X����դ���ף��Ĳ���P/K����").arg(m_LineSerialNum));
                return false;
            }
            runG36(Cache, actions, m_dI, m_sP.toInt(), m_dJ, m_sK.toInt());
            return true;
        }break;

        case GCODE_G37:    //G37 Y����դ����
        {
            if (m_sP.toInt() <= 0 || m_sK.toInt() <= 0)
            {
                e.insertDebug(DEBUG_NG_PUNCH_FILE_ALARM, QString("ActionPunchGCode::genrateAction : ��%1�� G36��X����դ���ף��Ĳ���P/K����").arg(m_LineSerialNum));
                return false;
            }
            runG37(Cache, actions, m_dI, m_sP.toInt(), m_dJ, m_sK.toInt());
            return true;
        }break;

        case GCODE_G66:    //G66 ֱ�߼���
        {
            if (m_isPunchKAssign)    //G66 I J P K T
            {
                errNum=runG66(Cache, actions, m_dI, m_dJ, m_sP.toDouble(), m_sK.toDouble());
                if (errNum == -1)
                {
                    e.insertDebug(DEBUG_NG_PUNCH_FILE_ALARM, QString("ActionPunchGCode::genrateAction : ��%1�� ��ó���Ӧ����1.5��ģ�ߣ����ߣ��ĳ���").arg(m_LineSerialNum));
                    return false;
                }

            }
            else    //G66 I J P Q D T
            {
                errNum=runG66(Cache, actions, m_dI, m_dJ, m_sP.toDouble(), m_dQ, m_dD);
                if (errNum == -1)
                {
                    e.insertDebug(DEBUG_NG_PUNCH_FILE_ALARM, QString("ActionPunchGCode::genrateAction : ��%1�� G66 ��ó���Ӧ����1.5��ģ�ߣ����ߣ��ĳ���").arg(m_LineSerialNum));
                    return false;
                }
                else if (errNum == -2)
                {
                    e.insertDebug(DEBUG_NG_PUNCH_FILE_ALARM, QString("ActionPunchGCode::genrateAction : ��%1�� G66 ģ�ߣ����ߣ��ĳ��ȺͿ�ȱ���ͬΪ��ֵ��ֵ").arg(m_LineSerialNum));
                    return false;
                }
            }
            return true;
        }break;

        case GCODE_G67:    //G67 �����μ���
        {
            errNum = runG67(Cache, actions, m_dI, m_dJ, m_sP.toDouble());
            if (errNum == -1)
            {
                e.insertDebug(DEBUG_NG_PUNCH_FILE_ALARM, QString("ActionPunchGCode::genrateAction : ��%1�� G67 ��þ��α߳�Ӧ����3��ģ�ߣ����ߣ��ĳ���").arg(m_LineSerialNum));
                return false;
            }
            else if (errNum == -2)
            {
                e.insertDebug(DEBUG_NG_PUNCH_FILE_ALARM, QString("ActionPunchGCode::genrateAction : ��%1�� G67 ������ģ�ߣ����ߣ��ı߳�����Ϊ����").arg(m_LineSerialNum));
                return false;
            }
            return true;
        }break;

        case GCODE_G68:    //G68 Բ����ʳ
        {
            errNum = runG68(Cache, actions, m_dI, m_dJ, m_sK.toDouble(), m_sP.toDouble(), m_dQ);
            if (errNum == -1)
            {
                e.insertDebug(DEBUG_NG_PUNCH_FILE_ALARM, QString("ActionPunchGCode::genrateAction : ��%1�� G68 Բ���뾶����С����").arg(m_LineSerialNum));
                return false;
            }
            else if (errNum == -2)
            {
                e.insertDebug(DEBUG_NG_PUNCH_FILE_ALARM, QString("ActionPunchGCode::genrateAction : ��%1�� G68 ������ü�಻��С��������8mm").arg(m_LineSerialNum));
                return false;
            }
            return true;
        }break;

        case GCODE_G69:    //G69 ֱ�߲�ʳ
        {
            errNum = runG69(Cache, actions, m_dI, m_dJ, m_sP.toDouble(), m_dQ);
            if (errNum == -1)
            {
                e.insertDebug(DEBUG_NG_PUNCH_FILE_ALARM, QString("ActionPunchGCode::genrateAction : ��%1�� G69 ��׳��Ȳ���С����").arg(m_LineSerialNum));
                return false;
            }
            else if (errNum == -2)
            {
                e.insertDebug(DEBUG_NG_PUNCH_FILE_ALARM, QString("ActionPunchGCode::genrateAction : ��%1�� G69 ��ʳ��಻��С����").arg(m_LineSerialNum));
                return false;
            }
            return true;
        }break;

        case GCODE_G78:    //G78 Բ�����
        {
            errNum = runG78(Cache, actions, m_dI, m_dJ, m_sK.toDouble(), m_sP.toDouble(), m_dQ, m_dD);
            if (errNum == -1)
            {
                e.insertDebug(DEBUG_NG_PUNCH_FILE_ALARM, QString("ActionPunchGCode::genrateAction : ��%1�� G78 Բ���뾶����С����").arg(m_LineSerialNum));
                return false;
            }
            else if (errNum == -2)
            {
                e.insertDebug(DEBUG_NG_PUNCH_FILE_ALARM, QString("ActionPunchGCode::genrateAction : ��%1�� G78 ������ü�಻��С��������8mm").arg(m_LineSerialNum));
                return false;
            }
            else if (errNum == -3)
            {
                e.insertDebug(DEBUG_NG_PUNCH_FILE_ALARM, QString("ActionPunchGCode::genrateAction : ��%1�� G78 �ӹ�����ܴ��ڳ�ü��").arg(m_LineSerialNum));
                return false;
            }
            return true;
        }break;

        case GCODE_G79:    //G79 ֱ�߳��
        {
            errNum = runG79(Cache, actions, m_dI, m_dJ, m_sP.toDouble(), m_dQ, m_dD);
            if (errNum == -1)
            {
                e.insertDebug(DEBUG_NG_PUNCH_FILE_ALARM, QString("ActionPunchGCode::genrateAction : ��%1�� G79 Բ���뾶����С����").arg(m_LineSerialNum));
                return false;
            }
            else if (errNum == -2)
            {
                e.insertDebug(DEBUG_NG_PUNCH_FILE_ALARM, QString("ActionPunchGCode::genrateAction : ��%1�� G79 ������ü�಻��С��������8mm").arg(m_LineSerialNum));
                return false;
            }
            else if (errNum == -3)
            {
                e.insertDebug(DEBUG_NG_PUNCH_FILE_ALARM, QString("ActionPunchGCode::genrateAction : ��%1�� G79 �ӹ�����ܴ��ڳ�ü��").arg(m_LineSerialNum));
                return false;
            }
            return true;
        }break;

        default:
            return false;
    }

}


/*************************************************
  �������ƣ�toDebugString()
  �������ܣ�
  ���������
  ���������
  �� �� ֵ��
  ��    ע��
**************************************************/
QString  ActionPunchGCode::toDebugString()
{
    QString  ss;

    ss += "��ѹ���룺\n";
    ss += "��ѹ���ͣ�";
    ss += GCodePunchString[m_nPunchG];
    ss += ",P��";
    if(m_isPunchPAssign)
    {
        ss += m_sP;
    }
    else
    {
        ss += "NULL";
    }

    ss += ",Q��";
    if(m_isPunchQAssign)
    {
        ss += ss.number(m_dQ);
    }
    else
    {
        ss += "NULL";
    }
    ss += ",D��";
    if(m_isPunchDAssign)
    {
        ss += ss.number(m_dD);
    }
    else
    {
        ss += "NULL";
    }
    ss += ",I��";
    if(m_isPunchIAssign)
    {
        ss += ss.number(m_dI);
    }
    else
    {
        ss += "NULL";
    }
    ss += ",J��";
    if(m_isPunchJAssign)
    {
        ss += ss.number(m_dJ);
    }
    else
    {
        ss += "NULL";
    }

    ss += ",K��";
    if(m_isPunchKAssign)
    {
        ss += m_sK;
    }
    else
    {
        ss += "NULL";
    }

    ss += ",T��";
    ss += ss.number(m_nT);
    ss += "\n";

    return ActionMoveGCode::toDebugString() + ss;
}

/*************************************************
  �������ƣ�toolAngleRotary(EmulationInterVariableCache *Cache, QVector<BaseAction *> &actions, double angle)
  �������ܣ����߽Ƕ���ת��������
  ��������� Cache  ���������棩ʱ��ʱ�����洢����
           angle  ��ת�Ƕȣ���������ϵ��
  ��������� actions  ����������Ķ���
  �� �� ֵ��  TRUE �ɹ�
            FALSE  ʧ��
  ��    ע��
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
  �������ƣ�runG28(EmulationInterVariableCache* Cache, QVector<BaseAction *> &actions, double dI, double dJ, int nK)
  �������ܣ�G28 ���ԽǶȳ�ã� G28 I J K T
  ��������� Cache  ���������棩ʱ��ʱ�����洢����
           dI��dJ��nK  G28ָ�����
  ��������� actions  ����������Ķ���
  �� �� ֵ��  0 �ɹ�
            ����  ʧ��
  ��    ע��
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
  �������ƣ�runG38(EmulationInterVariableCache* Cache, QVector<BaseAction *> &actions, double dI, double dJ, int nK)
  �������ܣ�G38 ���ԽǶȳ�ã� G38 I J K T
  ��������� Cache  ���������棩ʱ��ʱ�����洢����
           dI��dJ��nK  G38ָ�����
  ��������� actions  ����������Ķ���
  �� �� ֵ�� 0 �ɹ�
            ���� ʧ��
  ��    ע��
**************************************************/
int ActionPunchGCode::runG38(EmulationInterVariableCache* Cache, QVector<BaseAction *> &actions, double dI, double dJ, int nK)
{
    runG28(Cache, actions, dI, dJ, nK);
    return 0;
}

/*************************************************
  �������ƣ�runG29(EmulationInterVariableCache *Cache, QVector<BaseAction *> &actions, double dI, double dJ, double dP, int nK)
  �������ܣ�G29 Բ���ȷֳ�ף� G29 I J P K T
  ��������� Cache  ���������棩ʱ��ʱ�����洢����
           dI��dJ��dP��nK  G29ָ�����
  ��������� actions  ����������Ķ���
  �� �� ֵ��  0 �ɹ�
            ����  ʧ��
  ��    ע��
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
  �������ƣ�runG39(EmulationInterVariableCache *Cache, QVector<BaseAction *> &actions, double dI, double dJ, double dP, int nK)
  �������ܣ�G39 Բ���ȷֳ�ף� G39 I J P K T
  ��������� Cache  ���������棩ʱ��ʱ�����洢����
           dI��dJ��dP��nK  G39ָ�����
  ��������� actions  ����������Ķ���
  �� �� ֵ��  0 �ɹ�
            ����  ʧ��
  ��    ע��
**************************************************/
int ActionPunchGCode::runG39(EmulationInterVariableCache *Cache, QVector<BaseAction *> &actions, double dI, double dJ, double dP, int nK)
{
    runG29(Cache, actions, dI, dJ, dP, nK);
    return 0;
}

/*************************************************
  �������ƣ�runG26(EmulationInterVariableCache *Cache, QVector<BaseAction *> &actions, double dI, double dJ, int nK)
  �������ܣ�G26 Բ�ܵȷֳ�ף� G26 I J K T
  ��������� Cache  ���������棩ʱ��ʱ�����洢����
           dI��dJ��nK  G26ָ�����
  ��������� actions  ����������Ķ���
  �� �� ֵ��  0 �ɹ�
            ����  ʧ��
  ��    ע��
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
  �������ƣ�runG36(EmulationInterVariableCache *Cache, QVector<BaseAction *> &actions, double dI, int nP, double dJ, int nK)
  �������ܣ�G36 X����դ���ף�G36 I P J K T
  ��������� Cache  ���������棩ʱ��ʱ�����洢����
           dI��dJ��dP��nK  G36ָ�����
  ��������� actions  ����������Ķ���
  �� �� ֵ��  0 �ɹ�
            ����  ʧ��
  ��    ע��
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
  �������ƣ�runG37(EmulationInterVariableCache *Cache, QVector<BaseAction *> &actions, double dI, int nP, double dJ, int nK)
  �������ܣ�G37 Y����դ���ף�G37 I P J K T
  ��������� Cache  ���������棩ʱ��ʱ�����洢����
           dI��dJ��dP��nK  G37ָ�����
  ��������� actions  ����������Ķ���
  �� �� ֵ��  0 �ɹ�
            ����  ʧ��
  ��    ע��
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
  �������ƣ�runG66(EmulationInterVariableCache *Cache, QVector<BaseAction *> &actions, double dI, double dJ, double dP, double dQ, double dD)
  �������ܣ�G66 ֱ�߼��У�G66 I J P Q D T
  ��������� Cache  ���������棩ʱ��ʱ�����洢����
           dI��dJ��dP��dQ��dD  G66ָ�����
  ��������� actions  ����������Ķ���
  �� �� ֵ��  0 �ɹ�
            -1 ��ó���Ӧ����1.5��ģ�ߣ����ߣ��ĳ���
            -2 ģ�ߣ����ߣ��ĳ��ȺͿ�ȱ���ͬΪ��ֵ��ֵ
  ��    ע��
**************************************************/
int ActionPunchGCode::runG66(EmulationInterVariableCache *Cache, QVector<BaseAction *> &actions, double dI, double dJ, double dP, double dQ, double dD)
{
    Point temPoint(0,0,0,0,0,0);

    if (dQ*dP > 0)
    {
        if (dP < 0)
        {
            dP = -dP;//ģ�߳����ڼ���ʱ��Ҫ�õ�����ֵ
        }
    }
    else
        return -2;//ģ�ߣ����ߣ��ĳ��ȺͿ�ȱ���ͬΪ��ֵ��ֵ

    if (dI < 1.5*dP)
        return -1;//��ó���Ӧ����1.5��ģ�ߣ����ߣ��ĳ���

    double dp = dP/2 - dD;//�������ĵ����·��ԭ��ĳ���
    double dq = dQ/2;//�������ĵ����·��ԭ��Ŀ��
    double angle = PI*dJ/180;
    double cosAngle = cos(angle);
    double sinAngle = sin(angle);
    double toolx = dp*cosAngle - dq*sinAngle;//����������X�����ͶӰ����
    double tooly = dp*sinAngle + dq*cosAngle;//����������Y�����ͶӰ����

    double dLength = dI + 2*dD;//�ܳ�ó���
    //int nK = ceil(dLength/dP);//�ܳ�ô���
    int nK = (int)(dLength/dP) + 1;
    double dL = (dLength-dP)/(nK-1);//ÿ�������֮������ľ�

    double dx = dL*cosAngle;
    double dy = dL*sinAngle;


    /*���ο�����ϵ�Ƶ��������*/
    m_ReferencePosition.x += toolx;
    m_ReferencePosition.y += tooly;

    for (int i=0; i < nK; i++)
    {
        temPoint.x = m_ReferencePosition.x + i*dx;
        temPoint.y = m_ReferencePosition.y + i*dy;

    }

    /*���ο�����ϵ�Ƶ�ԭ��λ��*/
    m_ReferencePosition.x -= toolx;
    m_ReferencePosition.y -= tooly;

    return 0;
}

/*************************************************
  �������ƣ�runG66(EmulationInterVariableCache *Cache, QVector<BaseAction *> &actions, double dI, double dJ, double dP, double dK)
  �������ܣ�G66 ֱ�߼��У�G66 I J P K T
  ��������� Cache  ���������棩ʱ��ʱ�����洢����
           dI��dJ��dP��dK  G66ָ�����
  ��������� actions  ����������Ķ���
  �� �� ֵ��  0 �ɹ�
            -1 ��ó���Ӧ����1.5��ģ�ߣ����ߣ��ĳ���
  ��    ע��
**************************************************/
int ActionPunchGCode::runG66(EmulationInterVariableCache *Cache, QVector<BaseAction *> &actions, double dI, double dJ, double dP, double dK)
{
    Point temPoint(0,0,0,0,0,0);

    double dLength = fabs(dI);
    double dWidth = fabs(dK);
    double dTool = fabs(dP);

    if (dLength < 1.5*dTool || dWidth < 1.5*dTool)
        return -1;//��ó���Ӧ����1.5��ģ�ߣ����ߣ��ĳ���

    double angle = PI*dJ/180;
    double cosAngle = cos(angle);
    double sinAngle = sin(angle);


    /*���㵶�����ĵ㣨������㣩��·��ԭ��ľ���*/
    double toolu = dI > 0 ? dTool/2 : -dTool/2;
    double toolv = dP/2;
    double toolx = toolu*cosAngle - toolv*sinAngle;
    double tooly = toolu*sinAngle + toolv*cosAngle;

    /*���ο�����ϵ�Ƶ��������*/
    m_ReferencePosition.x += toolx;
    m_ReferencePosition.y += tooly;

    /*����ƽ�棬����ʱ�ȼ���ֲ�����ϵUOV�µ����꣬��ӳ�䵽��������ϵXOY��*/
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

    /*���ο�����ϵ�Ƶ�ԭ��λ��*/
    m_ReferencePosition.x -= toolx;
    m_ReferencePosition.y -= tooly;

    return 0;
}

/*************************************************
  �������ƣ�runG67(EmulationInterVariableCache *Cache, QVector<BaseAction *> &actions, double dI, double dJ, double dP)
  �������ܣ�G67 �����μ��У�G67 I J P T
  ��������� Cache  ���������棩ʱ��ʱ�����洢����
           dI��dJ��dP  G67ָ�����
  ��������� actions  ����������Ķ���
  �� �� ֵ��  0 �ɹ�
            -1 ��þ��α߳�Ӧ����3��ģ�ߣ����ߣ��ĳ���
            -2 ������ģ�ߵı߳�����Ϊ����
  ��    ע��
**************************************************/
int ActionPunchGCode::runG67(EmulationInterVariableCache *Cache, QVector<BaseAction *> &actions, double dI, double dJ, double dP)
{
    Point temPoint(0,0,0,0,0,0);
    double dLength = fabs(dI);
    double dWidth = fabs(dJ);

    if (dP <= 0)
        return -2;//������ģ�ߵı߳�����Ϊ����

    if (dLength < 3*dP || dWidth < 3*dP)
        return -1;//��þ��α߳�Ӧ����3��ģ�ߣ����ߣ��ĳ���


    /*���㵶�����ĵ㣨������㣩��·��ԭ��ľ���*/
    double toolx = dI > 0 ? dP/2 : -dP/2;
    double tooly = dJ > 0 ? dP/2 : -dP/2;

    /*���ο�����ϵ�Ƶ��������*/
    m_ReferencePosition.x += toolx;
    m_ReferencePosition.y += tooly;
    temPoint.x = m_ReferencePosition.x;
    temPoint.y = m_ReferencePosition.y;


    /*��þ��α߿�*/
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

    /*���ο�����ϵ�Ƶ�ԭ��λ��*/
    m_ReferencePosition.x -= toolx;
    m_ReferencePosition.y -= tooly;

    return 0;
}

/*************************************************
  �������ƣ�runG68(EmulationInterVariableCache *Cache, QVector<BaseAction *> &actions, double dI, double dJ, double dK, double dP, double dQ)
  �������ܣ�G68 Բ����ʳ��G68 I J k P Q T
  ��������� Cache  ���������棩ʱ��ʱ�����洢����
           dI��dJ��dP��dK��dQ  G68ָ�����
  ��������� actions  ����������Ķ���
  �� �� ֵ��  0 �ɹ�
            -1 Բ���뾶����С����
            -2 ������ü�಻��С��������8mm
  ��    ע��
**************************************************/
int ActionPunchGCode::runG68(EmulationInterVariableCache *Cache, QVector<BaseAction *> &actions, double dI, double dJ, double dK, double dP, double dQ)
{
    if (dI <= 0)
        return -1;//Բ���뾶����С����

    if (dQ < 0 || dQ > 8)
        return -2;//������ü�಻��С��������8mm

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
  �������ƣ�runG69(EmulationInterVariableCache *Cache, QVector<BaseAction *> &actions, double dI, double dJ, double dP, double dQ)
  �������ܣ�G69 ֱ�߲�ʳ��G69 I J P Q T
  ��������� Cache  ���������棩ʱ��ʱ�����洢����
           dI��dJ��dP��dQ  G69ָ�����
  ��������� actions  ����������Ķ���
  �� �� ֵ��  0 �ɹ�
            -1 ��׳��Ȳ���С����
            -2 ��ʳ��಻��С����
  ��    ע��
**************************************************/
int ActionPunchGCode::runG69(EmulationInterVariableCache *Cache, QVector<BaseAction *> &actions, double dI, double dJ, double dP, double dQ)
{
    if (dI <= 0)
        return -1;//��׳��Ȳ���С����

    if (dQ < 0)
        return -2;//��ʳ��಻��С����

    Point temPoint(0,0,0,0,0,0);

    double angle = PI*dJ/180;
    double cosAngle = cos(angle);
    double sinAngle = sin(angle);


    /*���㵶�����ĵ㣨������㣩��·��ԭ��ľ���*/
    double toolx = -dP*sinAngle/2;
    double tooly = dP*cosAngle/2;

    /*���ο�����ϵ�Ƶ��������*/
    m_ReferencePosition.x += toolx;
    m_ReferencePosition.y += tooly;

    /*ֱ�߲�ʳ*/
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

    /*���ο�����ϵ�Ƶ�ԭ��λ��*/
    m_ReferencePosition.x -= toolx;
    m_ReferencePosition.y -= tooly;

    return 0;
}

/*************************************************
  �������ƣ�runG78(EmulationInterVariableCache *Cache, QVector<BaseAction *> &actions, double dI, double dJ, double dK, double dP, double dQ, double dD)
  �������ܣ�G78 Բ����ף�G78 I J k P Q D T
  ��������� Cache  ���������棩ʱ��ʱ�����洢����
           dI��dJ��dP��dK��dQ��dD  G78ָ�����
  ��������� actions  ����������Ķ���
  �� �� ֵ��  0 �ɹ�
            -1 Բ���뾶����С����
            -2 ������ü�಻��С��������8mm
            -3 �ӹ�����ܴ��ڳ�ü��
  ��    ע��
**************************************************/
int ActionPunchGCode::runG78(EmulationInterVariableCache *Cache, QVector<BaseAction *> &actions, double dI, double dJ, double dK, double dP, double dQ, double dD)
{
    if (dI <= 0)
        return -1;//Բ���뾶����С����

    if (dQ < 0 || dQ > 10)
        return -2;//������ü�಻��С��������8mm

    if (dD > dQ)
        return -3;//�ӹ�����ܴ��ڳ�ü��

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
  �������ƣ�runG79(EmulationInterVariableCache *Cache, QVector<BaseAction *> &actions, double dI, double dJ, double dP, double dQ, double dD)
  �������ܣ�G79 ֱ�߳�ף�G79 I J P Q D T
  ��������� Cache  ���������棩ʱ��ʱ�����洢����
           dI��dJ��dP��dQ��dD  G79ָ�����
  ��������� actions  ����������Ķ���
  �� �� ֵ��  0 �ɹ�
            -1 ��׳��Ȳ���С����
            -2 ��ʳ��಻��С����
            -3 �ӹ�����ܴ��ڳ�ü��
  ��    ע��
**************************************************/
int ActionPunchGCode::runG79(EmulationInterVariableCache *Cache, QVector<BaseAction *> &actions, double dI, double dJ, double dP, double dQ, double dD)
{
    if (dI <= 0)
        return -1;//��׳��Ȳ���С����

    if (dQ < 0 || dQ > 10)
        return -2;//��ʳ��಻��С����

    if (dD > dQ)
        return -3;//�ӹ�����ܴ��ڳ�ü��

    Point temPoint(0,0,0,0,0,0);

    double angle = PI*dJ/180;
    double cosAngle = cos(angle);
    double sinAngle = sin(angle);


    /*���㵶�����ĵ㣨������㣩��·��ԭ��ľ���*/
    double toolx = -dP*sinAngle/2;
    double tooly = dP*cosAngle/2;

    /*���ο�����ϵ�Ƶ��������*/
    m_ReferencePosition.x += toolx;
    m_ReferencePosition.y += tooly;

    /*ֱ�߲�ʳ*/
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

    /*���ο�����ϵ�Ƶ�ԭ��λ��*/
    m_ReferencePosition.x -= toolx;
    m_ReferencePosition.y -= tooly;

    return 0;
}

