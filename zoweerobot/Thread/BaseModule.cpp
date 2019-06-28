/************************************************************
  Copyright (C),
  �ļ�����BaseModule.cpp
  ���ߣ�zhou    �汾��V1.0    �������ڣ�2018/07/10
  ģ��������BaseModule���ʵ��
  ��ʷ�޸ļ�¼��<author>    <time>    <version >    <desc>
                  zhou   2018/07/10     V1.0      ���ע��
*************************************************************/

#include "BaseModule.h"
#include "SystemSchedule.h"


BaseModule::BaseModule()
{
    m_pDevice = NULL;
    m_pScheduler = NULL;
    m_ModuleTimer = NULL;
}

BaseModule::~BaseModule()
{
}

/*************************************************
  �������ƣ�initModule(QList<InfoUI *>& u)
  �������ܣ�Module��ʼ�� ����UI �����豸
  ���������u UI����
  ���������
  �� �� ֵ��
  ��    ע��
**************************************************/
int BaseModule::initModule(QList<InfoUI *>& u)
{
    return 0;
}

/*************************************************
  �������ƣ�getErrorType()
  �������ܣ���ȡ������Ϣ����
  ���������
  ���������
  �� �� ֵ��int  ��������
  ��    ע��
**************************************************/
int   BaseModule::getErrorType()
{
    return m_errInfo.e_type;
}

/*************************************************
  �������ƣ�getErrorType()
  �������ܣ���ȡ������Ϣ����
  ���������
  ���������
  �� �� ֵ��int  ��������
  ��    ע��
**************************************************/
ModuleInfo* BaseModule::getModuleInfo()
{
    return &m_ModuleInfo;
}

/*************************************************
  �������ƣ�getDevice()
  �������ܣ���ȡ�豸ָ��
  ���������
  ���������
  �� �� ֵ��BaseDevice*  �豸ָ��
  ��    ע��
**************************************************/
BaseDevice* BaseModule::getDevice()
{
    return m_pDevice;
}

/*************************************************
  �������ƣ�getUIs()
  �������ܣ���ȡMoudle������UI����
  ���������
  ���������
  �� �� ֵ��QList<InfoUI *>*  ����Moudle������UI����ָ��
  ��    ע��
**************************************************/
QList<InfoUI *>* BaseModule::getUIs()
{
    return &m_UIs;
}

/*************************************************
  �������ƣ�getUIbyNames(QString s, ErrorInfo& e)
  �������ܣ������豸�������InfoUI��Ϣ
  ���������s  �豸��
  ���������e  ������Ϣ
  �� �� ֵ��InfoUI *  ����UIָ��
  ��    ע��
**************************************************/
InfoUI*  BaseModule::getUIbyNames(QString s, ErrorInfo& e)
{
    foreach(InfoUI *u, m_UIs)
    {
        if(u->sName == s)
            return u;
    }

    e.insertDebug(DEBUG_WARNING, "getUIbyNames get no UI resource");

    return NULL;
}

SystemSchedule*  BaseModule::getSystemScheduleHandle()
{
    return m_pScheduler;
}

SystemParameter* BaseModule::getSystemParameterHandle()
{
    return m_pScheduler->getSystemParameterHandler();
}

ErrorInfo& BaseModule::getErrorInfo()
{
    return m_errInfo;
}

QWidget*  BaseModule::getUIHandlerByID(int id)
{
    for(int i = 0; i < m_UIs.size(); i++)
    {
        if(m_UIs.at(i)->nUISerial == id)
        {
            return  m_UIs.at(i)->pUi;
        }
    }
    return NULL;
}

void BaseModule::parseNGFileModel()
{

}

//void BaseModule::procEmulationModel()
//{

//}

void BaseModule::runTimer()
{

}

//void BaseModule::restartEmulation()
//{

//}
//void BaseModule::pauseEmulation()
//{}

//void BaseModule::stepEmulation()
//{}

//void BaseModule::continueEmulation()
//{}

//void BaseModule::runToBreakpointEmulation()
//{}

//void BaseModule::changeEmulationSpeed(int v)
//{}

void BaseModule::run()
{
}

void BaseModule::absorbXMLModule(QString, QString)
{}

void BaseModule::absorbPLCModule(QString, QString)
{}

void BaseModule::absorbMotionModule(QString, QString)
{}

void BaseModule::absorbEmulationModule(QString, QString)
{}

void BaseModule::absorbManualMotionModule(QString, QString)
{}

void BaseModule::absorbKeyModule(QString, QString)
{}


