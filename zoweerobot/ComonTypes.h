/**
Copyright (c)
@file ComonTypes.h
@author zhou
@version 1.0
@date 2018/07/10
@brief ϵͳ�����������͵Ķ��塣
*/

#ifndef COMONTYPES_H
#define COMONTYPES_H

#include <QString>
#include <QWidget>
#include <QVector>
#include <QDebug>
#include <QFile>
#include <QDateTime>
#include <QMutex>

#include "GCodeComonType.h"

//#define _CNC_DEBUG
//#define _CNC_SYSTEM_PROCESS_DEBUG

//#define _CNC_PARSE_GCODE_RECOED_DEBUG

#define DONE_SUCCESS            TRUE
#define DONE_FAIL               FALSE

#define MOTION_CONTROLLER_ID           0
#define EMULATION_CONTROLLER_ID        1
#define XML_CONTROLLER_ID              2
#define KEY_CONTROLLER_ID              3
#define PLC_CONTROLLER_ID              4



#define MODULES_TIMER_INTERVAL          1000

/** @brief Module��Ϣ */
typedef struct _ModuleInfo
{
    QString name;
    int     nID;
}ModuleInfo;

/** @brief ����Module�����ַ����� */
const QString ModulesName[] = { "Motion Controller",    \
                                "Emulation Controller", \
                                "XML Controller",       \
                                "Key Controller",       \
                                "PLC Controller"
                                };

#define UI_AUTO_MACHINING_STATE              0
#define UI_MANUAL_MACHINING_STATE            1
#define UI_IO_STATE                          2
#define UI_MACHINING_SIMULATION              3
#define UI_SYSTEM_PREFERENCES                4
#define UI_AXES_PREFERENCES                  5
#define UI_CRAFT_PREFERENCES                 6
#define UI_TOOL_PREFERENCES                  7
#define UI_WARNING_MSG_DIAGNOSIS             8
#define UI_WARNING_RECORD                    9
#define UI_SYSTEM_INFORMATION                10
#define UI_COMMUNICATION_SETTINGS            11
#define UI_ADVANCED_SYSTEM_SETTINGS          12


enum MainMenuId
{
    UI_PROGRAM_ID = 0,
    UI_STATE_ID = 1,
    UI_SETTING_ID = 2,
    UI_WARNING_ID = 3,

    UI_PAGE_NUMBER = 4
};



/** @brief ����ModuleInfo�� */
const ModuleInfo ModulesInfo[] = { {ModulesName[MOTION_CONTROLLER_ID], MOTION_CONTROLLER_ID}, \
                                   {ModulesName[EMULATION_CONTROLLER_ID], EMULATION_CONTROLLER_ID},  \
                                   {ModulesName[XML_CONTROLLER_ID], XML_CONTROLLER_ID},  \
                                   {ModulesName[KEY_CONTROLLER_ID], KEY_CONTROLLER_ID},  \
                                   {ModulesName[PLC_CONTROLLER_ID], PLC_CONTROLLER_ID},
                                };

/** @brief ÿ��UI����ϸ��Ϣ */
typedef struct _InfoUI
{
    QString  sName;    /**< UI���� */
    QWidget  *pUi;     /**< UIָ�� */
    QWidget  *pParentUi;  /**< UI�ĸ�Widgetָ�� */

    bool     isActive;    /**< �Ƿ�ǰ��Ҫ������ */
    int      nType;         /**< �����ĸ����͵�UI,��MainMenuId�ҹ�*/
    int      nUISerial;
    bool bShow;


    /**< �����ĸ��ҳ�� */

    QVector<int> nModulesUser;   /**< ��UI�ɱ�ʹ�õ�Modules���� */

}InfoUI;

/** @brief IO�˿ڵ��ź����� */
typedef enum  _WorkStatus
{
    WORK_STATUS_IDLE,
    WORK_STATUS_PAUSE,
    WORK_STATUS_STOP
}WorkStatus;

typedef  struct  _ContextRecord
{
    int       SerialNumber;
    QString   TextStr;
}ContextRecord;

#define  DEBUG_NO_ERROR                      0        /**< �޴� */
#define  DEBUG_WARNING                       100      /**< ������Ϣ */
#define  DEBUG_PARAMETER_ERROR               1000     /**< ����������Ϣ */
#define  DEBUG_CREAT_ERROR                   1001     /**< ����ʱ������Ϣ */
#define  DEBUG_NG_COMAND_NO_SUPPORT_ERROR    1002     /**< G�����ʽ������Ϣ */

#define  DEBUG_NG_FILE_FORMAT_ALARM          1400     /**< G�����ʽ������Ϣ */
#define  DEBUG_NG_PUNCH_FILE_ALARM           1500     /**< ��ѹ����������Ϣ */
#define  DEBUG_SERIOUS_ERROR                 2000     /**< ���ش�����Ϣ */
#define  DEBUG_NET_PARM_ERROR                2500     /**< ���緢�����ݴ�����Ϣ */
#define  DEBUG_CONTROLLER_RESPONSE_ERROR     3000

/** @brief ������Ϣ�࣬ÿ��Module����һ�����Ὣ�����¼��111111111.txt */
class  ErrorInfo
{
public:
    int         e_type;      /**< �������� */
    ModuleInfo  e_moudle;     /**< Moduleӵ���� */
    QString     e_str;         /**< ������ϸ��Ϣ */

    ErrorInfo()
    {
        #ifdef  _CNC_DEBUG
        pfile = new QFile("111111111.txt");

        if(pfile->exists())
            pfile->remove();

        if (!pfile->open(QIODevice::ReadWrite | QIODevice::Text))
            return;
        #endif
    }
    ~ErrorInfo()
    {
        #ifdef  _CNC_DEBUG
        pfile->close();
        #endif
    }

    void setModuleInfo(ModuleInfo m)
    {
       e_moudle=m;
    }

    void printfErr()
    {
        qDebug() << "ErrorType: " << e_type <<", " << e_str;
    }

    QString toString()
    {
        return  QString::fromLocal8Bit("ErrorType:%1,  ModuleInfo:%2,  ErrInfo:%3").arg(e_type).arg(e_moudle.name).arg(e_str);
    }

    void writeToFile()
    {
        #ifdef  _CNC_DEBUG
        pfile->write(toString().toLocal8Bit().data());
        #endif
    }

    void insertDebug()
    {
        #ifdef  _CNC_DEBUG
        if(e_type != DEBUG_NO_ERROR)
            writeToFile();
        #endif
    }

    void  insertDebug(int t, QString s)
    {
        e_type = t;
        e_str = s;
        insertDebug();
    }

private:
    #ifdef  _CNC_DEBUG
    QFile *pfile;
    #endif

};



class  LogInfo
{
public:
    LogInfo(QString name)
    {
        pfile = new QFile(name);

        if(pfile->exists())
            pfile->remove();

        if (!pfile->open(QIODevice::WriteOnly | QIODevice::Text))
        {
            pfile = NULL;
        }
    }
    ~LogInfo()
    {
        if(pfile != NULL)
        {
            pfile->close();
        }
    }

    void printfLog(QString s)
    {
        qDebug() << toString(s);
    }

    QString toString(QString s)
    {
        QDateTime current_date_time = QDateTime::currentDateTime();
        QString  current_date = current_date_time.toString("yyyy-MM-dd hh:mm:ss");
        return  (current_date + " : " + s);
    }

    void writeToFile(QString s)
    {
        if(pfile != NULL)
        {
            pfile->write(toString(s).toLocal8Bit().data());
            pfile->flush();
        }
    }

    void writeToFile(const char *data, int len)
    {
        if(pfile != NULL)
        {
            mutex.lock();

            //pfile->write(data, len);
            //pfile->flush();
            QString str="";
            for (int i = 0; i < len; i++)
            {
                str = QString::number((unsigned int)((unsigned char)data[i]));
                str += " ";
                pfile->write(str.toLocal8Bit().data());
                pfile->flush();
            }

            mutex.unlock();
        }
    }

    void newline()
    {
        if(pfile != NULL)
        {
            pfile->write("\r\n");
            pfile->flush();
        }
    }

    void insertDebug(QString s)
    {
#ifdef  _CNC_DEBUG
        writeToFile(s);
#endif
    }

private:
    QFile   *pfile;
    QMutex   mutex;

};

/** @brief �ж��ַ��Ƿ����� */

extern  bool  isDigital(const QChar c);

/** @brief �ж��ַ����Ƿ�ȫ�������� */
extern  bool  checkAllDigital(const QString c);

/** @brief �ж��ַ��Ƿ��ַ� */
extern  bool  isCharacter(const char c);

/** @brief �ж�G�����˶����ָ���Ƿ�֧�� {"G", "I", "j", "X", "Y", "T", "M"} */
extern  bool  isActionCommandSupport(QString s, int& ii);

/** @brief ���ַ�����Ѱ�ҵ�һ�����֣���������λ�� */
extern  int   findQStringFirstDigitPos(QString s);

#define STATUSBAR_ERROR_INFO 4
#define STATUSBAR_WARNING_INFO 3
#define STATUSBAR_STATUS_ON_INFO 2
#define STATUSBAR_STATUS_OFF_INFO 1
#define STATUSBAR_OTHER_INFO 0

//�������IO��Plc�Ĵ���M�ڵĵ�ַ��Ӧ��ϵ
#define IO_DINGWEIXIAO_PLC_M 0
#define IO_DINGWEIKUAI_PLC_M 1
#define IO_PUNCH_PLC_M      2
#define IO_OUTCYCLE_PLC_M    3
#define IO_INCYCLE_PLC_M     4
#define IO_PUNCHLOCK_PLC_M   5
#define IO_PUNCHRESET_PLC_M  6
#define IO_CLEARALARM_PLC_M  7
#define IO_RUNHUA_PLC_M      8
#define IO_AXISCLIHE_PLC_M   9
#define IO_AXISVTOP_PLC_M    10
#define IO_ZAIDINGWEIKUAI_PLC_M 11
#define IO_EMSSTOP_PLC_M     12
#define IO_JIAQIAN_PLC_M     13
#define IO_XMOION_PLC_M      14
#define IO_YMOION_PLC_M      15
#define IO_TMOION_PLC_M      16
#define IO_CMOION_PLC_M      17
#define IO_VMOION_PLC_M      18
#define IO_REFHOMEMOV_PLC_M  19
#define IO_MANNULMOV_PLC_M   20
#define IO_AUTOMOV_PLC_M     21

#define IO_SHOUDONG_RUIHUO_SIGNAL_PLC_M             48
#define IO_DINGWEIXIAO_SIGNAL_PLC_M                 43

#define IO_AXIS_C_JIEHE_SIGNAL_PLC_M                53
//#define IO_AXIS_C_FENGLI_SIGNAL_PLC_M               54
#define IO_ZAIDINGWEIKUAI_SHANGSHENG_SIGNAL_PLC_M   55
//#define IO_ZAIDINGWEIKUAI_XIAJIANG_SIGNAL_PLC_M     56
#define IO_JIAQIAN_SONGJIN_SIGNAL_PLC_M             61
#define IO_DINGWEIKUAI_JIANG_SIGNAL_PLC_M           60
#define IO_CHONGTOU_LOCK_SIGNAL_PLC_M           40

#define IO_SERVOENABLE_BNT_ADD  100

#define IO_CLAMPPROTECT_SIGNAL_PLC_M  59

#define IO_CANTOOLCHANGE_SIGNAL_PLC_M 70
#define IO_HANDLEPUNCH_SIGNAL_PLC_M 50
#define IO_ISROTATETOOL_SIGNAL_PLC_M 64

#define IO_MACHINEALARM_SIGNAL_PLC_M 42
#define IO_OILLEVELERROR_SIGNAL_PLC_M 63
#define IO_SAFTDOOROPEN_SIGNAL_PLC_M 65
#define IO_EMSSTOP_SIGANL_PLC_M 49
#define IO_STARTPROCESS_SIGNAL_PLC_M 57
#define IO_STOPPROCESS_SIGNAL_PLC_M 58
#define IO_MOULDS_PROTECT_SIGNAL_PLC_M 67
#define IO_AIRPRESSURE_PROTECT_SIGNAL_PLC_M 66

#define IO_AXIST_FWD_SIGNAL_PLC_M 71
#define IO_AXIST_REV_SIGNAL_PLC_M 72


#endif // COMONTYPES_H
