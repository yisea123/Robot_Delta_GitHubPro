/************************************************************
  Copyright (C),
  �ļ�����ProcessCALLGCode.cpp
  ���ߣ�zhou    �汾��V1.0    �������ڣ�2018/07/10
  ģ��������ProcessCALLGCode���ʵ��
  ��ʷ�޸ļ�¼��<author>    <time>    <version >    <desc>
                  zhou    2018/07/10     V1.0      ���ע��
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
  �������ƣ�fillGCodeVarient(ParseGCodeInterVariableCache*  Cache, GCodeParseInfo& inf, ErrorInfo& e)
  �������ܣ���������ı������и�ֵ
  ��������� Cache  ����ʱ��ʱ�����洢����
  ��������� inf   һ��G����ָ��洢����
           e     �������
  �� �� ֵ��  DONE_SUCCESS �ɹ�
            DONE_FAIL  ʧ��
  ��    ע��
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


