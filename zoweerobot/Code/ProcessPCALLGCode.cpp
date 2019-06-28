/************************************************************
  Copyright (C),
  �ļ�����ProcessPCALLGCode.cpp
  ���ߣ�zhou    �汾��V1.0    �������ڣ�2018/07/10
  ģ��������ProcessPCALLGCode���ʵ��
  ��ʷ�޸ļ�¼��<author>    <time>    <version >    <desc>
                  zhou    2018/07/10     V1.0      ���ע��
*************************************************************/

#include "ProcessPCALLGCode.h"
#include "ParseGCodeModel.h"

ProcessPCALLGCode::ProcessPCALLGCode()
{
    for(int i = 0; i < PCALL_LOCAL_VARIABLE; i++)
        m_localVariable[i] = -1;
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

bool ProcessPCALLGCode::fillGCodeVarient(ParseGCodeInterVariableCache*  Cache, GCodeParseInfo& inf, ErrorInfo& e)
{
    ProcessCALLGCode::fillGCodeVarient(Cache, inf, e);
    return fillLocalVariable(Cache, inf, e);
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
  �������ƣ�fillGCodeVarient(ParseGCodeInterVariableCache*  Cache, GCodeParseInfo& inf, ErrorInfo& e)
  �������ܣ���������ı������и�ֵ
  ��������� Cache  ����ʱ��ʱ�����洢����
  ��������� inf   һ��G����ָ��洢����
           e     �������
  �� �� ֵ��  DONE_SUCCESS �ɹ�
            DONE_FAIL  ʧ��
  ��    ע��
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


