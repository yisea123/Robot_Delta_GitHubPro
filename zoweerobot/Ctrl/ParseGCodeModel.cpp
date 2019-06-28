/************************************************************
  Copyright (C),
  �ļ�����ParseGCode.cpp
  ���ߣ�zhou    �汾��V1.0    �������ڣ�2018/07/10
  ģ��������ParseGCode���ʵ��
  ��ʷ�޸ļ�¼��<author>    <time>    <version >    <desc>
                  zhou    2018/07/10     V1.0      ���ע��
*************************************************************/

#include "ParseGCodeModel.h"
#include "BaseModule.h"
//#include "QGCodeTextEdit.h"
#include "ComonTypes.h"
#include "BaseGCode.h"
#include "ActionMoveGCode.h"
#include "ActionPunchGCode.h"
//#include "ActionToolChangeGCode.h"
#include "ProcessCALLGCode.h"
#include "ProcessPCALLGCode.h"
#include "GlobalVariableType.h"

#include "MacroGCode.h"
#include "MacroRunGCode.h"
#include "ActionMGCode.h"
#include "CoordinateGcode.h"
#include "ParameteGcode.h"

#include "SystemSchedule.h"

#include <QFile>



/*************************************************
  �������ƣ�isDigital(const QChar c)
  �������ܣ��ж��ַ��Ƿ�����
  ���������c  ��Ҫ�жϵ��ַ�
  ���������
  �� �� ֵ�� DONE_SUCCESS �ɹ�
            DONE_FAIL  ʧ��
  ��    ע��
**************************************************/
inline  bool  isDigital(const QChar c)
{
    if((c >= '0' && c <= '9') || (c == '.') || (c == '-'))
    {
        return true;
    }

    return false;
}

/*************************************************
  �������ƣ�checkAllDigital(const QString c)
  �������ܣ��ж��ַ����Ƿ�ȫΪ����
  ���������c  ��Ҫ�жϵ��ַ���
  ���������
  �� �� ֵ�� DONE_SUCCESS �ɹ�
            DONE_FAIL  ʧ��
  ��    ע��
**************************************************/
bool  checkAllDigital(const QString c)
{
    //if (0 == c.size())
    if (c.isEmpty())
    {
        return false;
    }
    if (c.count(".") > 1)
    {
        return false;
    }
    if (c.count("-") > 1)
    {
        return false;
    }

    foreach(QChar cc, c)
    {
        if(!isDigital(cc))
        {
            return false;
        }
    }

    return true;
}

/*************************************************
  �������ƣ�isCharacter(const char c)
  �������ܣ��ж��ַ��Ƿ��ַ�A-Z
  ���������c ��Ҫ�жϵ��ַ�
  ���������
  �� �� ֵ�� DONE_SUCCESS �ɹ�
            DONE_FAIL  ʧ��
  ��    ע��
**************************************************/
inline  bool  isCharacter(const char c)
{
    if((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
    {
        return true;
    }

    return false;
}

/*************************************************
  �������ƣ�isCharacter(const QChar c)
  �������ܣ��ж��ַ��Ƿ��ַ�A-Z
  ���������c ��Ҫ�жϵ�QChar
  ���������
  �� �� ֵ��  DONE_SUCCESS �ɹ�
            DONE_FAIL  ʧ��
  ��    ע��
**************************************************/
inline  bool  isCharacter(const QChar c)
{
    if((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
    {
        return true;
    }

    return false;
}

/*************************************************
  �������ƣ�findQStringFirstDigitPos(QString s)
  �������ܣ����ַ�����Ѱ�ҵ�һ�����֣���������λ��
  ���������s  �ַ���
  ���������
  �� �� ֵ�� DONE_SUCCESS �ɹ�
            DONE_FAIL  ʧ��
  ��    ע��
**************************************************/
int   findQStringFirstDigitPos(QString s)
{
    for(int i = 0; i < s.size(); i++)
    {
        if(isDigital(s.at(i)))
        {
            return  i;
        }
    }

    return -1;
}

/*************************************************
  �������ƣ�findQStringFirstNotDigitPos(QString s)
  �������ܣ����ַ���ͷ����ʼѰ�ҵ�һ�������ֵ�λ��
  ���������  s  ��ҪѰ�ҵ��ַ���
  ���������
  �� �� ֵ�� ��һ�����������ַ�����λ��
  ��    ע��
**************************************************/
int   findQStringFirstNotDigitPos(QString s)
{
    for(int i = 0; i < s.size(); i++)
    {
        if(!(isDigital(s.at(i))))
        {
            return  i;
        }
    }

    return -1;
}

/*************************************************
  �������ƣ�findQStringFirstCharacterPos(QString s)
  �������ܣ����ַ���ͷ����ʼѰ�ҵ�һ���ַ���λ��
  ���������  s  ��ҪѰ�ҵ��ַ���
  ���������
  �� �� ֵ�� ��һ���ַ����ַ�����λ��
  ��    ע��
**************************************************/
int   findQStringFirstCharacterPos(QString s)
{
    for(int i = 0; i < s.size(); i++)
    {
        if(isCharacter(s.at(i)))
        {
            return  i;
        }
    }

    return -1;
}

/*************************************************
  �������ƣ�findQStringFirstNotCharacterPos(QString s)
  �������ܣ����ַ���ͷ����ʼѰ�ҵ�һ�����ַ���λ��
  ��������� s  ��ҪѰ�ҵ��ַ���
  ���������
  �� �� ֵ�� ��һ�����ַ����ַ�����λ��
  ��    ע��
**************************************************/
int   findQStringFirstNotCharacterPos(QString s)
{
    int i = 0;
    for(i; i < s.size(); i++)
    {
        if(!isCharacter(s.at(i)))
        {
            return  i;
        }
    }

    return i;
}

/*************************************************
  �������ƣ�getDigitQString(QString s, int p, int& k)
  �������ܣ����ַ���ָ����λ�ÿ�ʼ����õ�һ��ȫ�������ֹ��ɵ������ַ���
  ���������s  ��ҪѰ�ҵ��ַ����� p ָ����ʼѰ�ҵ�λ��
  ���������k  �����ֹ��ɵ������ַ��������һ���ַ��������ַ�����λ��
  �� �� ֵ����һ��ȫ�������ֹ��ɵ������ַ���
  ��    ע��
**************************************************/
QString   getDigitQString(QString s, int p, int& k)
{
    int i;
    for(i = p; i < s.size(); i++)
    {
        if(!isDigital(s.at(i)))
        {
            k = i;
            return  s.mid(p ,i - p);
        }
    }

    k = i - 1;
    //return s;
    return s.mid(p);
}

/*************************************************
  �������ƣ�getSpecialWordBackInteger(QString ss, QString key, int& jj, int& value)
  �������ܣ���ȡָ���ؼ��ַ��������ֵ  ��N90 �ؼ��ַ�N ���������ֵΪ����Ҫ��ȡ����ֵ90
  ���������ss  ԭʼ�ַ���, key �ؼ��ַ�
  ���������jj  ԭʼ�ַ��������������λ��
          value ��õ���ֵ
  �� �� ֵ��  DONE_SUCCESS �ɹ�
            DONE_FAIL  ʧ��
  ��    ע��
**************************************************/
bool  getSpecialWordBackInteger(QString ss, QString key, int& jj, int& value)
{
    int i, j;
    if((i = ss.indexOf(key)) >= 0)
    {
        ss = ss.mid(i + key.size());
        if((j = findQStringFirstDigitPos(ss)) >= 0)
        {
            value = getDigitQString(ss, j, jj).toInt();
            jj += i + key.size();
            return TRUE;
        }

    }
    else
    {
        return FALSE;
    }
}

/*************************************************
  �������ƣ�getCompleteCharacterBackward(QString ss, QString& key, int& jj)
  �������ܣ���ȡ�ַ����е�һ�����ַ���ɵ������ַ���
  ���������  ss ԭʼ�ַ���
  ���������  key  ��õĵ�һ�����ַ���ɵ������ַ���
            jj  ԭʼ�ַ��������������λ��
  �� �� ֵ��  DONE_SUCCESS �ɹ�
            DONE_FAIL  ʧ��
  ��    ע��
**************************************************/
bool getCompleteCharacterBackward(QString ss, QString& key, int& jj)
{
    key.clear();

    int i = findQStringFirstCharacterPos(ss);
    if(i >= 0)
    {
        QString ss1 = ss.mid(i);
        int j;
        j =  findQStringFirstNotCharacterPos(ss1);
        key = ss1.left(j);
        jj = i + j;

        return TRUE;
    }
    else
    {
        jj = -1;
        return FALSE;
    }


}

/*************************************************
  �������ƣ�getCompleteDigitBackward(QString ss, QString& key, int& jj)
  �������ܣ���ȡ�ַ����е�һ����������ɵ������ַ���
  ���������  ss ԭʼ�ַ���
  ���������  key  ��õĵ�һ����������ɵ������ַ���
            jj  ԭʼ�ַ��������������λ��
  �� �� ֵ��  DONE_SUCCESS �ɹ�
            DONE_FAIL  ʧ��
  ��    ע��
**************************************************/
bool getCompleteDigitBackward(QString ss, QString& key, int& jj)
{
    key.clear();

    int i = findQStringFirstDigitPos(ss);
    if(i >= 0)
    {
        QString ss1 = ss.mid(i);
        int j;
        j =  findQStringFirstNotDigitPos(ss1);
        key = ss1.left(j);
        jj = i + j;

        return TRUE;
    }
    else
    {
        jj = -1;
        return FALSE;
    }
}

/*************************************************
  �������ƣ�getFirstCharacter(QString s)
  �������ܣ�����ַ�����ͷ�ǿ��ַ�
  ��������� s  ԭʼ�ַ���
  ���������  QChar  �ַ�����ͷ�ǿ��ַ�
  �� �� ֵ��  DONE_SUCCESS �ɹ�
            DONE_FAIL  ʧ��
  ��    ע��
**************************************************/
QChar getFirstCharacter(QString s)
{
    int ll=s.size();
    for(int i = 0; i < ll; i++)
    {
        if(s[i] != ' ')
            return s[i];
    }
}

/*************************************************
  �������ƣ�getGSymbolWeight(int v, int& ii, ErrorInfo& e)
  �������ܣ�����ͬ��G��ͷ�Ĵ��루G90 G75����Ȩ�أ��Ա������з���
  ��������� v  G������ֵ
  ��������� ii Ȩ��
            e  ������Ϣ��¼
  �� �� ֵ�� DONE_SUCCESS �ɹ�
            DONE_FAIL  ʧ��
  ��    ע��
**************************************************/
bool  ParseGCodeModel::getGSymbolWeight(int v, int& ii, ErrorInfo& e)
{
    const  int  G_MarcoRunArray[] = {75, 76};
    //const  int  G_ParamateArray[] = {06};
    const  int  G_CoordinateArray[] = {92, 93, 72, 98};
    const  int  G_ReferenceArray[] = {90, 91};
    const  int  G_MoveArray[] = {70, 25, 27, 0, 1, 2, 4, 5 ,6,50};
    const  int  G_PunchArray[] = {28, 38, 29, 39, 26, 36, 37, 66, 67, 68, 69, 78, 79};

    for(int i = 0; i < (sizeof(G_MarcoRunArray) / sizeof(int)); i++)
    {
        if(v == G_MarcoRunArray[i])
        {
            ii = 32;
            return TRUE;
        }
    }

   /* for(int i = 0; i < (sizeof(G_ParamateArray) / sizeof(int)); i++)
    {
        if(v == G_ParamateArray[i])
        {
            ii = 21;
            return TRUE;
        }
    }*/

    for(int i = 0; i < (sizeof(G_CoordinateArray) / sizeof(int)); i++)
    {
        if(v == G_CoordinateArray[i])
        {
            ii = 11;
            return TRUE;
        }
    }

    for(int i = 0; i < (sizeof(G_ReferenceArray) / sizeof(int)); i++)
    {
        if(v == G_ReferenceArray[i])
        {
            ii = 1;
            return FALSE;
        }
    }

    for(int i = 0; i < (sizeof(G_MoveArray) / sizeof(int)); i++)
    {
        if(v == G_MoveArray[i])
        {
            ii = 2;
            return FALSE;
        }
    }

    for(int i = 0; i < (sizeof(G_PunchArray) / sizeof(int)); i++)
    {
        if(v == G_PunchArray[i])
        {
            ii = 3;
            return FALSE;
        }
    }
    e.insertDebug(DEBUG_NG_COMAND_NO_SUPPORT_ERROR, "getGSymbolWeight  G:" + QString::number(v));
    return TRUE;
}

/*************************************************
  �������ƣ�getKeySymbolWeight(LineContextInfo info, int& ii, ErrorInfo& e)
  �������ܣ�����ͬ��G���루G90 X90�ȣ���Ȩ�أ��Ա������з���
  ��������� info  G����ָ��¼�ṹ��
  ��������� ii  Ȩ��
            e  ������Ϣ��¼
  �� �� ֵ�� DONE_SUCCESS �ɹ�
            DONE_FAIL  ʧ��
  ��    ע��
**************************************************/
bool  ParseGCodeModel::getKeySymbolWeight(LineContextInfo info, int& ii, ErrorInfo& e)
{
    QString  s = info.symbol;
    if(s == "G" || s == "g")
    {
        int v = info.value.toInt();
        return getGSymbolWeight(v, ii, e);
    }
    else if((s == "X") || (s == "Y") || (s == "x") || (s == "y")|| (s == "Z") || (s == "R")|| (s == "z") || (s == "r"))
    {
        if(ii == -1 || ii == 2)//ȱʡģʽ(ii=-1)��G00(ii=2)��ͬ
        {
            /*if(m_interVariableCache->m_isSelectTool)
            {
                ii = 3;
            }
            else*/
            {
                ii = 2;
            }
        }
    }
    else if(s == "T" || s == "t"||s == "U" || s == "u")
    {
        ii = 2;// 3;
    }
    else if(s == "M" || s == "m")
    {
        ii = 4;
        return TRUE;
    }
    //else if(s == "U" || s == "V" || s == "u" || s == "v")
    else if( s == "V" || s == "v")
    {
        ii = 31;
        if(s == "U" || s == "u")
        {
            m_bMacroFlag = TRUE;
        }
        else
        {
            m_bMacroFlag = FALSE;
        }
        return TRUE;
    }
    else if (s == "N" || s == "n")
    {
        ii = 0;
        return TRUE;
    }

    return  FALSE;
}


ParseGCodeModel::ParseGCodeModel(QObject *parent) :
    QObject(parent)
{
    m_bMacroFlag = FALSE;
    m_pMacroGCode = NULL;
}

/*************************************************
  �������ƣ�loadParseController(BaseModule *m, ErrorInfo& e)
  �������ܣ�����ParseController����ȡG�����ļ�����
  ���������  m  Moduleָ��
  ��������� e  Module�Ĵ�����Ϣ
  �� �� ֵ�� DONE_SUCCESS �ɹ�
            DONE_FAIL  ʧ��
  ��    ע��
**************************************************/
ParseGCodeModel* ParseGCodeModel::loadParseController(BaseModule *m, ErrorInfo& e)
{
    if(!(m->getSystemScheduleHandle()->getNGFileContexts()->size() > 0))
    {
        e.insertDebug(DEBUG_WARNING, "loadParseController get no fileContext");  /* ���������Ϣ */

        return NULL;
    }

    return (new ParseGCodeModel);
}

/*************************************************
  �������ƣ�startupParseController(BaseModule *m, QList<BaseGCode *>* list, ErrorInfo& e)
  �������ܣ���������Controller����
  ���������  m  Moduleָ��
  ���������  list  �������ŵ���������
            e  Module�Ĵ�����Ϣ
  �� �� ֵ��  DONE_SUCCESS �ɹ�
            DONE_FAIL  ʧ��
  ��    ע��
**************************************************/
bool ParseGCodeModel::startupParseController(BaseModule *m, QVector<BaseGCode *>* list, ErrorInfo& e)
{
    m_pfileContext = m->getSystemScheduleHandle()->getNGFileContexts();

    if(m_pfileContext->size() > 0)
    {
//        m_nContextReadLine = 0;
        m_nContextLinesNum = m_pfileContext->size();

        m_interVariableCache = new ParseGCodeInterVariableCache;   /* ����������ʱ�洢�� */

        if(!recordFileHeadInfo(e))
        {
            //delete  m_interVariableCache;
            e.insertDebug(DEBUG_WARNING, "startupParseController get no FileStartFlag");

            //return  DONE_FAIL;
        }

#ifdef  _CNC_PARSE_GCODE_RECOED_DEBUG

        QFile file("333333333.txt");

        if(file.exists())
        {
            file.remove();
        }

        if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
        {
            return  DONE_FAIL;
        }

        file.write((m->getSystemScheduleHandle()->getNGFilePath() +  + "\n").toLocal8Bit().data());
        file.write("HEAD :\n");
        file.write((QString("�����Ϣ(PART): ���ȣ�%1,��ȣ�%2\n").arg(m_interVariableCache->m_parts.size_x).arg(m_interVariableCache->m_parts.size_y)).toLocal8Bit().data());
        file.write((QString("������Ϣ(SHEET): ���ȣ�%1,��ȣ�%2����ȣ�%3\n").arg(m_interVariableCache->m_RawMaterial.size_x).arg(m_interVariableCache->m_RawMaterial.size_y).arg(m_interVariableCache->m_RawMaterial.size_h)).toLocal8Bit().data());

        for(int i = 0; i < m_interVariableCache->m_ToolsInfo.size(); i++)
        {
            QString  s;
            for(int j = 0; j < m_interVariableCache->m_ToolsInfo.at(i).paraRecordNum; j++)
            {
                s += s.number(m_interVariableCache->m_ToolsInfo.at(i).para[j]);
                s += " ";
            }
            file.write(((QString("������Ϣ(*T): ��������:%1,���ߺ�:%2,���߿���:%3,������%4\n")).arg(m_interVariableCache->m_ToolsInfo[i].name).arg(m_interVariableCache->m_ToolsInfo[i].toolSerialNum).arg(m_interVariableCache->m_ToolsInfo[i].toolMagazineSerialNum).arg(s)).toLocal8Bit().data());

        }

 //       file.close();
#endif

        for(int i = m_nContextReadLine; i < m_nContextLinesNum; i++)  /* ����G����ÿһ�� */
        {
            if(!paserPerNGLine(m_pfileContext->at(i), list, e))
            {              
                //delete  m_interVariableCache;
                return  DONE_FAIL;
            }
        }

#ifdef  _CNC_PARSE_GCODE_RECOED_DEBUG

 //       QFile file("333333333.txt");

//        if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
//        {
//            return  DONE_FAIL;
//        }

        file.write("BODY :\n");

        for(int i = 0; i < list->size(); i++)
        {

            file.write(list->at(i)->toDebugString().toLocal8Bit().data());

        }

        file.close();
#endif
        return DONE_SUCCESS;
    }
    else
    {
        e.insertDebug(DEBUG_WARNING, "startupParseController get no fileContext");
        //delete  m_interVariableCache;

        return  DONE_FAIL;
    }
}

bool ParseGCodeModel::startupParseController(SystemParameter* para, BaseModule *m, QVector<BaseGCode *>* list, ErrorInfo& e)
{
    m_pfileContext = m->getSystemScheduleHandle()->getNGFileContexts();

    if(m_pfileContext->size() > 0)
    {
//        m_nContextReadLine = 0;
        m_nContextLinesNum = m_pfileContext->size();

        m_interVariableCache = new ParseGCodeInterVariableCache;   /* ����������ʱ�洢�� */
        m_interVariableCache->m_MachineCoordinate.x = -para->m_machineParm.m_machineOriginalPosition.x;
        m_interVariableCache->m_MachineCoordinate.y = -para->m_machineParm.m_machineOriginalPosition.y;
	 m_interVariableCache->m_MachineCoordinate.z = -para->m_machineParm.m_machineOriginalPosition.z;
        m_interVariableCache->m_MachineCoordinate.r = -para->m_machineParm.m_machineOriginalPosition.r;
	for(int i=1;i<21;i++)
	{
		m_interVariableCache->m_userParam[i]=para->m_userParam[i];
	}
	if(!recordFileHeadInfo(e))
        {
            //delete  m_interVariableCache;
            e.insertDebug(DEBUG_WARNING, "startupParseController get no FileStartFlag");

            //return  DONE_FAIL;
        }

#ifdef  _CNC_PARSE_GCODE_RECOED_DEBUG

        QFile file("333333333.txt");

        if(file.exists())
        {
            file.remove();
        }

        if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
        {
            return  DONE_FAIL;
        }

        file.write((m->getSystemScheduleHandle()->getNGFilePath() +  + "\n").toLocal8Bit().data());
        file.write("HEAD :\n");
        file.write((QString("�����Ϣ(PART): ���ȣ�%1,��ȣ�%2\n").arg(m_interVariableCache->m_parts.size_x).arg(m_interVariableCache->m_parts.size_y)).toLocal8Bit().data());
        file.write((QString("������Ϣ(SHEET): ���ȣ�%1,��ȣ�%2����ȣ�%3\n").arg(m_interVariableCache->m_RawMaterial.size_x).arg(m_interVariableCache->m_RawMaterial.size_y).arg(m_interVariableCache->m_RawMaterial.size_h)).toLocal8Bit().data());

        for(int i = 0; i < m_interVariableCache->m_ToolsInfo.size(); i++)
        {
            QString  s;
            for(int j = 0; j < m_interVariableCache->m_ToolsInfo.at(i).paraRecordNum; j++)
            {
                s += s.number(m_interVariableCache->m_ToolsInfo.at(i).para[j]);
                s += " ";
            }
            file.write(((QString("������Ϣ(*T): ��������:%1,���ߺ�:%2,���߿���:%3,������%4\n")).arg(m_interVariableCache->m_ToolsInfo[i].name).arg(m_interVariableCache->m_ToolsInfo[i].toolSerialNum).arg(m_interVariableCache->m_ToolsInfo[i].toolMagazineSerialNum).arg(s)).toLocal8Bit().data());

        }

 //       file.close();
#endif

        for(int i = m_nContextReadLine; i < m_nContextLinesNum; i++)  /* ����G����ÿһ�� */
        {
            if(!paserPerNGLine(m_pfileContext->at(i), list, e))
            {
                //delete  m_interVariableCache;
                return  DONE_FAIL;
            }
        }

#ifdef  _CNC_PARSE_GCODE_RECOED_DEBUG

 //       QFile file("333333333.txt");

//        if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
//        {
//            return  DONE_FAIL;
//        }

        file.write("BODY :\n");

        for(int i = 0; i < list->size(); i++)
        {

            file.write(list->at(i)->toDebugString().toLocal8Bit().data());

        }

        file.close();
#endif
        return DONE_SUCCESS;
    }
    else
    {
        e.insertDebug(DEBUG_WARNING, "startupParseController get no fileContext");
        //delete  m_interVariableCache;

        return  DONE_FAIL;
    }
}


/*************************************************
  �������ƣ�unloadParseController(BaseModule *m, ErrorInfo& e)
  �������ܣ�ж�ؽ���Controller����
  ���������  m  Moduleָ��
  ���������  e  Module�Ĵ�����Ϣ
  �� �� ֵ��  DONE_SUCCESS �ɹ�
            DONE_FAIL  ʧ��
  ��    ע��
**************************************************/
bool ParseGCodeModel::unloadParseController(BaseModule *m, ErrorInfo& e)
{
//    m_pfileContext->clear();
    delete m_interVariableCache;
    delete this;
    return 0;
}

/*************************************************
  �������ƣ�getInterVariableCache()
  �������ܣ�����ڲ��������̵ı�������ָ��
  ���������
  ���������
  �� �� ֵ�� ��������ָ��
  ��    ע��
**************************************************/
ParseGCodeInterVariableCache*  ParseGCodeModel::getInterVariableCache()
{
    return  m_interVariableCache;
}

/*************************************************
  �������ƣ�getToolsInfo()
  �������ܣ���õ�����Ϣ�б�
  ���������
  ���������
  �� �� ֵ�������б�ָ��
  ��    ע��
**************************************************/
QVector<HeadToolInfo>*   ParseGCodeModel::getToolsInfo()
{
    return  &m_interVariableCache->m_ToolsInfo;
}

/*************************************************
  �������ƣ�recordFileHeadInfo(ErrorInfo& e)
  �������ܣ���¼�ӹ������ļ���ͷ����Ϣ
  ���������
  ��������� e  Module�Ĵ�����Ϣ
  �� �� ֵ��  DONE_SUCCESS �ɹ�
            DONE_FAIL  ʧ��
  ��    ע��
**************************************************/
bool ParseGCodeModel::recordFileHeadInfo(ErrorInfo& e)
{
    int i=0;

    while((!isCharacter(m_pfileContext->at(i).TextStr[0])) || ((m_pfileContext->at(i).TextStr[0] == 'O')) || (m_pfileContext->at(i).TextStr[0] == 'N'))   /* ��N��ͷ */
    {
        QString ss = m_pfileContext->at(i).TextStr;

        if (ss[0] == 'N')
        {
            if ((ss.contains("%") || ss.contains(";")))
               ss = ss.section(';',1);//ȡ��';'���沿������
            else
                break;
        }

        int ii, jj;
        if(ss.contains("PART"))
        {
            if(ss.contains("SIZE"))
            {
                if((ii = findQStringFirstDigitPos(ss)) >0)
                {
                    m_interVariableCache->m_parts.size_x = getDigitQString(ss, ii, jj).toDouble();
                    ss = ss.mid(jj);
                    if((ii = findQStringFirstDigitPos(ss)) >0)
                    {
                        m_interVariableCache->m_parts.size_y = getDigitQString(ss, ii, jj).toDouble();
                    }
                }
            }

        }
        else if(ss.contains("*SHEET"))
        {
            if((ii = findQStringFirstDigitPos(ss)) >0)
            {
                m_interVariableCache->m_RawMaterial.size_x = getDigitQString(ss, ii, jj).toDouble();
                ss = ss.mid(jj);
                if((ii = findQStringFirstDigitPos(ss)) >0)
                {
                    m_interVariableCache->m_RawMaterial.size_y = getDigitQString(ss, ii, jj).toDouble();
                    ss = ss.mid(jj);
                    if((ii = findQStringFirstDigitPos(ss)) >0)
                    {
                        m_interVariableCache->m_RawMaterial.size_h = getDigitQString(ss, ii, jj).toDouble();
                    }
                }
            }
        }
        else if(ss.contains("MAT"))
        {
            QList<QString> sss = ss.split(' ');
            m_interVariableCache->m_RawMaterial.name = sss[1];
            m_interVariableCache->m_RawMaterial.size_h = getDigitQString(sss[2], 0, jj).toDouble();
        }
        else if(ss.contains("*T"))
        {
            HeadToolInfo  tt;
            getSpecialWordBackInteger(ss, "*T", jj, tt.toolSerialNum);
            ss = ss.mid(jj);
            getCompleteCharacterBackward(ss, tt.name, jj);
            ss = ss.mid(jj);

            QString ss1 = ss.left(ss.indexOf("N="));
            QString ss2;

            int j;
            for(j = 0 ; j < 5; j++)
            {
                if(getCompleteDigitBackward(ss1, ss2, jj))
                {
                    tt.para[j] = ss2.toDouble();
                    ss1 = ss1.mid(jj);
                }
                else
                {
                    break;
                }

            }
            tt.paraRecordNum = j;
            getSpecialWordBackInteger(ss, "N=", jj, tt.toolMagazineSerialNum);

            m_interVariableCache->m_ToolsInfo.append(tt);
        }

        i++;

        if(i >= m_nContextLinesNum)
        {
            e.insertDebug(DEBUG_WARNING, "findFileStartFlag get no start flag");
            return  DONE_FAIL;
        }

    }

    m_nContextReadLine = i;    

    return DONE_SUCCESS;
}

/*************************************************
  �������ƣ�paserPerNGLine(QString s, QList<BaseGCode *>* list, ErrorInfo& e)
  �������ܣ�����ÿһ��G����
  ���������  s  һ��G�����е��ַ���
  ���������  list  �������ŵ���������
            e  Module�Ĵ�����Ϣ
  �� �� ֵ��  DONE_SUCCESS �ɹ�
            DONE_FAIL  ʧ��
  ��    ע��
**************************************************/
bool  ParseGCodeModel::paserPerNGLine(ContextRecord s, QVector<BaseGCode *>* list, ErrorInfo& e)
{
    GCodeParseInfo  gParseInfo;

    gParseInfo.m_SerialNumber = s.SerialNumber;

    if((s.TextStr.size() > 0) && (s.TextStr[0] != '(') && (s.TextStr[0] != '%') )
    {
        QString ss = s.TextStr.section(';',0,0);

        if(ss.contains("F1"))
        {
            int ij;
            ij=0;
        }
        if(ss.contains("CALL"))    /* �ԣ� �ָ�G�����˶����߼����� */
        {
            gParseInfo.m_GCodeType = GCODE_PROCESS_COMMAND_TYPE;

            int p = ss.indexOf('(');

            /* ���ָ�G�߼���Ϣ�ָ��  ����m_textStr ��N010 ��PCALL 80��I3�� �ָ�ΪN010 �� PCALL 80��I3 */
            if(p > 0)
            {
                gParseInfo.m_textStr.append(ss.left(p));
                ss = ss.mid(p);

                QString str = gParseInfo.m_textStr[0];
                p = findQStringFirstDigitPos(str);
                LineContextInfo  inf;
                if(p == -1)
                {
                    inf.symbol = str;
                    inf.value.clear();
                }
                else
                {
                    inf.symbol = str.left(p);
                    inf.value = str.mid(p);
                }

                gParseInfo.m_info.append(inf);

                if((p = ss.indexOf(')')) > 0)
                {
                    gParseInfo.m_textStr.append(ss.left(p));
                    ss = ss.mid(p);
                }

                if(ss.size() > 0)
                {
                    gParseInfo.m_textStr.append(ss);
                }

                if(gParseInfo.m_textStr.size() > 1)
                {
                    if(!converBaseGCode(gParseInfo, list, e))
                        return DONE_FAIL;
                }
                else
                {
                    e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, "paserPerNGLine error" + ss);
                    return DONE_FAIL;
                }
            }

        }
        else
        {
            gParseInfo.m_GCodeType = GCODE_ACTION_TYPE;

            int ii;
            LineContextInfo  inf;
            /* ���ָ�G�߼���Ϣ�ָ��  ����m_textStr ��N010 G93 I30  �ָ�ΪN010��G93��I30 */
            while(getCompleteCharacterBackward(ss, inf.symbol, ii))
            {
                ss = ss.mid(ii);
                if(getCompleteDigitBackward(ss, inf.value, ii))
                {
                    ss = ss.mid(ii);
                    /*if(inf.symbol == "T")
                    {
                        if(getFirstCharacter(ss) == '(')
                        {
                            ss = ss.mid(ss.indexOf(')') + 1);
                        }
                    }
                    if(inf.symbol != "F")*/
                    {
                        gParseInfo.m_info.append(inf);
                    }

                }
                else
                {
                 //   e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, "paserPerNGLine no carry digit" + inf.symbol);
                 //   return DONE_FAIL;
                }
            }

            if(gParseInfo.m_info.size() >= 1)
            {
                if(!converBaseGCode(gParseInfo, list, e))
                {
                    return DONE_FAIL;
                }

            }
        }
    }



    gParseInfo.clear();

    return DONE_SUCCESS;
}

/*************************************************
  �������ƣ�converBaseGCode(GCodeParseInfo& inf, QList<BaseGCode *>* list, ErrorInfo& e)
  �������ܣ����������G����ת��ΪBaseGCode�洢
  ���������
  ���������  inf  ÿһ��G����������̵���ʱ�����洢
            list  �������ŵ���������
            e  Module�Ĵ�����Ϣ
  �� �� ֵ��  DONE_SUCCESS �ɹ�
            DONE_FAIL  ʧ��
  ��    ע��
**************************************************/
bool  ParseGCodeModel::converBaseGCode(GCodeParseInfo& inf, QVector<BaseGCode *>* list, ErrorInfo& e)
{
    int line = 0;
    if(inf.m_info[0].symbol == "N" || inf.m_info[0].symbol == "n")
    {
        if(checkAllDigital(inf.m_info[0].value))
        {
            line = inf.m_info[0].value.toInt();   /* �����к� */
            inf.m_info.removeAt(0);
        }
    }
    if (inf.m_info.size() <= 0)//��ָ�����
    {
        return DONE_SUCCESS;
    }

    if(inf.m_GCodeType == GCODE_ACTION_TYPE)
    {
        GCodeActionType  t;
        if(checkGCodeActionType(inf, t, e))
        {
            switch(t)
            {
                case GCODE_ACTION_REFERENCE_TYPE:
                {
                    ActionReferenceGCode *bb = new ActionReferenceGCode;
                    bb->setNGSerialNum(line);

                    if(!bb->fillGCodeVarient(m_interVariableCache, inf, e))
                    {
                        delete bb;
                        return DONE_FAIL;
                    }

                    (m_bMacroFlag)?(m_pMacroGCode->getGCodeList()->append(bb)):(list->append(bb));

                    return DONE_SUCCESS;
                }

                case GCODE_ACTION_MOVE_TYPE:
                {
                    ActionMoveGCode *bb = new ActionMoveGCode;
                    bb->setNGSerialNum(line);
		     bb->setLineSerialNum(inf.m_SerialNumber);//add text linenum
                    if(!bb->fillGCodeVarient(m_interVariableCache, inf, e))
                    {
                        delete bb;
                        return DONE_FAIL;
                    }

                    (m_bMacroFlag)?(m_pMacroGCode->getGCodeList()->append(bb)):(list->append(bb));
                    return DONE_SUCCESS;
                }


                case GCODE_ACTION_PUNCH_TYPE:
                {
                    ActionPunchGCode *bb = new ActionPunchGCode;
                    bb->setNGSerialNum(line);

                    if(!bb->fillGCodeVarient(m_interVariableCache, inf, e))
                    {
                        delete bb;
                        return DONE_FAIL;
                    }

                    (m_bMacroFlag)?(m_pMacroGCode->getGCodeList()->append(bb)):(list->append(bb));
                    return DONE_SUCCESS;
                }

                case GCODE_ACTION_M_TYPE:
                {
                    ActionMGCode *bb = new ActionMGCode;
                    bb->setNGSerialNum(line);

                    if(!bb->fillGCodeVarient(m_interVariableCache, inf, e))
                    {
                        delete bb;
                        return DONE_FAIL;
                    }

                    (m_bMacroFlag)?(m_pMacroGCode->getGCodeList()->append(bb)):(list->append(bb));
                    return DONE_SUCCESS;
                }
                case GCODE_ACTION_COORDINATE_TYPE:
                {
                    CoordinateGcode *bb = new CoordinateGcode;
                    bb->setNGSerialNum(line);

                    if(!bb->fillGCodeVarient(m_interVariableCache, inf, e))
                    {
                        delete bb;
                        return DONE_FAIL;
                    }

                    (m_bMacroFlag)?(m_pMacroGCode->getGCodeList()->append(bb)):(list->append(bb));
                    return DONE_SUCCESS;
                }
                case GCODE_ACTION_PARAMETER_TYPE:
                {
                    ParameteGcode *bb = new ParameteGcode;
                    bb->setNGSerialNum(line);

                    if(!bb->fillGCodeVarient(m_interVariableCache, inf, e))
                    {
                        delete bb;
                        return DONE_FAIL;
                    }

                    (m_bMacroFlag)?(m_pMacroGCode->getGCodeList()->append(bb)):(list->append(bb));
                    return DONE_SUCCESS;
                }
                case GCODE_ACTION_MACRO_TYPE:
                {
                    if(m_bMacroFlag)
                    {
                        MacroGCode *bb = new MacroGCode;
                        m_pMacroGCode = bb;
                        bb->setNGSerialNum(line);
                        bb->fillGCodeVarient(m_interVariableCache, inf, e);

                        if(e.e_type == DEBUG_NG_PUNCH_FILE_ALARM)
                        {
                            delete bb;
                            return DONE_FAIL;
                        }

                        list->append(bb);
                    }
                    return DONE_SUCCESS;
                }
                case GCODE_ACTION_MACRO_RUN_TYPE:
                {
                    MacroRunGCode *bb = new MacroRunGCode;
                    bb->setNGSerialNum(line);

                    if(!bb->fillGCodeVarient(m_interVariableCache, inf, e))
                    {
                        delete bb;
                        return DONE_FAIL;
                    }

                    (m_bMacroFlag)?(m_pMacroGCode->getGCodeList()->append(bb)):(list->append(bb));
                    return DONE_SUCCESS;
                }
                default:
                {
                    return DONE_SUCCESS;
                }
            }
        }
        else
        {
            return DONE_FAIL;
        }
    }
    else if(inf.m_GCodeType == GCODE_PROCESS_COMMAND_TYPE)
    {
        GCodeProcessType  t;
        if(checkGCodeProcessType(inf, t, e))
        {
            switch(t)
            {
                case GCODE_PROCESS_CALL_TYPE:
                {
                    ProcessCALLGCode *bb = new ProcessCALLGCode;
                    bb->setNGSerialNum(line);
                    bb->fillGCodeVarient(m_interVariableCache, inf, e);

                    if(e.e_type == DEBUG_NG_PUNCH_FILE_ALARM)
                    {
                        delete bb;
                        return DONE_FAIL;
                    }

                    list->append(bb);
                    return DONE_SUCCESS;
                }
                case GCODE_PROCESS_PCALL_TYPE:
                {
                    return DONE_SUCCESS;//�ݲ�֧��pcall
                    ProcessPCALLGCode *bb = new ProcessPCALLGCode;
                    bb->setNGSerialNum(line);
                    bb->fillGCodeVarient(m_interVariableCache, inf, e);

                    if(e.e_type == DEBUG_NG_PUNCH_FILE_ALARM)
                    {
                        delete bb;
                        return DONE_FAIL;
                    }

                    list->append(bb);
                    return DONE_SUCCESS;
                }

            }
        }
        else
        {
            return DONE_FAIL;
        }
    }
}

/*************************************************
  �������ƣ�checkGCodeActionType(GCodeParseInfo& inf, GCodeActionType& t, ErrorInfo& e)
  �������ܣ���G���������ж���Ҫ������Actiontype����
  ���������
  ���������  inf  ÿһ��G����������̵���ʱ�����洢
            t  G�����˶���������
            e  Module�Ĵ�����Ϣ
  �� �� ֵ��  DONE_SUCCESS �ɹ�
            DONE_FAIL  ʧ��
  ��    ע��
**************************************************/
bool ParseGCodeModel::checkGCodeActionType(GCodeParseInfo& inf, GCodeActionType& t, ErrorInfo& e)
{
    int ii = -1;

    for(int i = 0; i < inf.m_info.size(); i++)
    {
        if(getKeySymbolWeight(inf.m_info.at(i), ii, e))
        {
            break;
        }
    }

    //if(e.e_type != DEBUG_NO_ERROR)
    if (e.e_type > 0 && e.e_type <= DEBUG_NG_PUNCH_FILE_ALARM)
    {
        return DONE_FAIL;
    }

    if(ii < 0)
    {
        e.insertDebug(DEBUG_NG_COMAND_NO_SUPPORT_ERROR, "checkGCodeActionType " + QString::number(inf.m_SerialNumber));
        return DONE_FAIL;
    }

    if(ii == 1)
    {
        t = GCODE_ACTION_REFERENCE_TYPE;
    }
    else  if(ii == 2)
    {
        t = GCODE_ACTION_MOVE_TYPE;
    }
    else if(ii == 3)
    {
        t = GCODE_ACTION_PUNCH_TYPE;
    }
    else if(ii == 4)
    {
        t = GCODE_ACTION_M_TYPE;
    }
    else if(ii == 11)
    {
        t = GCODE_ACTION_COORDINATE_TYPE;
    }
    else  if(ii == 21)
    {
        t = GCODE_ACTION_PARAMETER_TYPE;
    }
    else if(ii == 31)
    {
        t = GCODE_ACTION_MACRO_TYPE;
    }
    else if(ii == 32)
    {
        t = GCODE_ACTION_MACRO_RUN_TYPE;
    }
    else
    {
        t = GCODE_ACTION_NULL;
    }
//    else
//    {
//        e.insertDebug(DEBUG_NG_COMAND_NO_SUPPORT_ERROR, "checkGCodeActionType ii" + QString::number(ii));
//        return DONE_FAIL;
//    }

    return DONE_SUCCESS;
}

/*************************************************
  �������ƣ�checkGCodeProcessType(GCodeParseInfo& inf, GCodeProcessType& t, ErrorInfo& e)
  �������ܣ����G�����߼�ָ���Ƿ�֧��  �� PCALL 83֧��   JPCALL 83  ��֧��   ֧��ָ����GCodeProcessTypeCharacters[] = { "CALL", "PCALL"};
  ���������
  ���������  inf  ÿһ��G����������̵���ʱ�����洢
            t  G�����߼���������
            e  Module�Ĵ�����Ϣ
  �� �� ֵ��  DONE_SUCCESS �ɹ�
            DONE_FAIL  ʧ��
  ��    ע��
**************************************************/
bool ParseGCodeModel::checkGCodeProcessType(GCodeParseInfo& inf, GCodeProcessType& t, ErrorInfo& e)
{
    if(inf.m_textStr[1].contains(GCodeProcessTypeCharacters[1]))
    {
        t = GCODE_PROCESS_PCALL_TYPE;
        return DONE_SUCCESS;
    }

    e.insertDebug(DEBUG_NG_FILE_FORMAT_ALARM, "checkGCodeProcessType no support:" + inf.m_textStr[1]);

    return DONE_FAIL;
}









