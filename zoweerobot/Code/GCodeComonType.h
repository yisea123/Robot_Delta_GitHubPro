/**
Copyright (c)
@file GCodeComonType.h
@author zhou
@version 1.0
@date 2018/07/10
@brief GCode���ݱ����������͵Ķ��塣
*/

#ifndef GCODECOMONTYPE_H
#define GCODECOMONTYPE_H

#include <QString>
#include <QList>

/** @brief ����G����ת�����ݽṹ���� */
typedef  enum _GCodeAllTypes
{
    GCODE_REFERENCE_TYPE,
    GCODE_MOVE_TYPE,
    GCODE_PUNCH_TYPE,
    GCODE_M_TYPE,
    GCODE_COORDINATE_TYPE,
    GCODE_PARAMETER_TYPE,
    GCODE_MACRO_TYPE,
    GCODE_MACRO_RUN_TYPE,
    GCODE_CALL_TYPE,
    GCODE_PCALL_TYPE,
    GCODE_MCALL_TYPE,
    GCODE_RPT_TYPE,
    GCODE_IF_TYPE,
    GCODE_GOTO_TYPE,
    GCODE_TOOL_CHANGE_TYPE
}GCodeAllTypes;

/** @brief G������Ҫ�����˶����������� */
typedef  enum _GCodeActionType
{
    GCODE_ACTION_REFERENCE_TYPE,
    GCODE_ACTION_MOVE_TYPE,
    GCODE_ACTION_PUNCH_TYPE,
    GCODE_ACTION_M_TYPE,
    GCODE_ACTION_COORDINATE_TYPE,
    GCODE_ACTION_PARAMETER_TYPE,
    GCODE_ACTION_MACRO_TYPE,
    GCODE_ACTION_MACRO_RUN_TYPE,
    GCODE_ACTION_NULL
}GCodeActionType;

/** @brief G�����ڲ��߼����������� */
typedef  enum _GCodeProcessType
{
    GCODE_PROCESS_CALL_TYPE,
    GCODE_PROCESS_PCALL_TYPE,
    GCODE_PROCESS_MCALL_TYPE,
    GCODE_PROCESS_RPT_TYPE,
    GCODE_PROCESS_IF_TYPE,
    GCODE_PROCESS_GOTO_TYPE

}GCodeProcessType;

//#define  GCODE_ACTION_COMMAND_SUPPORT_NUM          7
//#define  GCODE_ACTION_COMMAND_REFERENCE_LIMIT      3
//#define  GCODE_ACTION_COMMAND_MOVE_LIMIT           5
//#define  GCODE_ACTION_COMMAND_TOOL_LIMIT           6
//#define  GCODE_ACTION_COMMAND_PUNCH_LIMIT          7

///** @brief G�����˶�ָ��Ĺؼ��ַ��� */
//const QString  GCodeActionTypeCharacters[] = {"G", "I", "j", "X", "Y", "T", "M"};

/** @brief G�����߼�ָ��Ĺؼ��ַ��� */
const QString  GCodeProcessTypeCharacters[] = { "CALL", "PCALL"};

/** @brief G����ο���������� */
#define  RELATIVE_COORDINATE_REFERENCE    false
#define  ABSOLUTE_COORDINATE_REFERENCE    true

typedef  enum _GCodeCoordinateReferenceType
{
    GCODE_REFERENCE_NULL,
    GCODE_G90,
    GCODE_G91
}GCodeCoordinateReferenceType;

typedef  enum _GCodeCoordinateType
{
    GCODE_Coordinate_NULL,
    GCODE_G92,
    GCODE_G93,
    GCODE_G98,
    GCODE_G72
}GCodeCoordinateType;

typedef  enum _GCodeMoveType
{
    GCODE_MOVE_NULL,
    GCODE_G00,
    GCODE_G01,
    GCODE_G02,
    GCODE_G03,
    GCODE_G04,
    GCODE_G05,
    GCODE_G06,
    GCODE_G25,
    GCODE_G27,
    GCODE_G50,
    GCODE_G70
}GCodeMoveType;

typedef  enum _GCodePunchType
{
    GCODE_PUNCH_NULL,
    GCODE_G28,
    GCODE_G38,
    GCODE_G29,
    GCODE_G39,
    GCODE_G26,
    GCODE_G36,
    GCODE_G37,
    GCODE_G66,
    GCODE_G67,
    GCODE_G68,
    GCODE_G69,
    GCODE_G78,
    GCODE_G79,
    GCODE_NOT_PUNCH,
    GCODE_PUNCH_IO
}GCodePunchType;

typedef  enum _GCodeMacroType
{
    GCODE_Macro_NULL,
    GCODE_G75,
    GCODE_G76
}GCodeMacroType;


#define    DIRECT_LEFT_DOWN    0
#define    DIRECT_RIGHT_DOWN   1
#define    DIRECT_LEFT_TOP     2
#define    DIRECT_RIGHT_TOP    3


/** @brief ��¼λ����Ϣ�ṹ�� */
typedef  struct _GCodePosition
{
    double  x;
    double  y;
}GCodePosition;


///** @brief G��������ӳ������� */
//typedef  enum  _GCodeCallCommandType
//{
//    GCODE_PCALL,
//    GCODE_CALL,
//    GCODE_MCALL
//}GCodeCallCommandType;

///** @brief IO�˿ڵ��ź����� */
//typedef  enum  _GCodeSubProcessType
//{
//    GCODE_RPT,
//    GCODE_IF,
//    GCODE_GOTO
//}GCodeSubProcessType;

/** @brief G�����ַ��ָ�  ��X90  X����symbol y����value */
typedef struct _LineContextInfo
{
    QString  symbol;    /**< �ؼ��� */
    QString  value;     /**< ��ֵ */
}LineContextInfo;

/** @brief ����G����ֳ����ࣺ1���˶� 2;�߼� */
typedef  enum _GCodeType
{
    GCODE_NULL,
    GCODE_ACTION_TYPE,
    GCODE_PROCESS_COMMAND_TYPE
}GCodeType;

/** @brief ����ÿһ��G������ʱ������ݽṹ */
class  GCodeParseInfo
{
public:
    GCodeType  m_GCodeType;
    int        m_SerialNumber;

    QList<QString>   m_textStr;
    QList<LineContextInfo>  m_info;

    GCodeParseInfo()
    {
        m_GCodeType = GCODE_NULL;
    }

    ~GCodeParseInfo()
    {
        m_info.clear();
    }

    void clear()
    {
        m_GCodeType = GCODE_NULL;
        m_info.clear();
    }

};

#endif // GCODECOMONTYPE_H
