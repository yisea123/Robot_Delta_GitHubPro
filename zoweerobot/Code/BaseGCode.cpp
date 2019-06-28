/************************************************************
  Copyright (C),
  �ļ�����BaseGCode.cpp
  ���ߣ�zhou   �汾��V1.0    �������ڣ�2018/07/10
  ģ��������BaseGCode���ʵ��
  ��ʷ�޸ļ�¼��<author>    <time>    <version >    <desc>
                  zhou   2018/07/10     V1.0      ���ע��
*************************************************************/

#include "BaseGCode.h"
#include "ComonTypes.h"

#include "ParseGCodeModel.h"

BaseGCode::BaseGCode()
{
    m_LineSerialNum = -1;
}

/*************************************************
  �������ƣ�fillGCodeVarient(ParseGCodeInterVariableCache*  Cache, GCodeParseInfo& inf, ErrorInfo& e)
  �������ܣ���������ı������и�ֵ
  ��������� Cache  ����ʱ��ʱ�����洢����
  ��������� inf   һ��G����ָ��洢����
           e     �������
  �� �� ֵ��  DONE_SUCCESS �ɹ�
            DONE_FAIL  ʧ��
  ��    ע��
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
    ss += "BASE��\n";
    ss += "BASEGCODE���ͣ�";
    ss += GCodeString[m_GCodeType];
    ss += ",G�����к�(N)��";
    ss += ss.number(m_NGSerialNum);
    ss += "\n";

    return ss;
}
