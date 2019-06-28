/************************************************************
  Copyright (C),
  文件名：BaseModule.cpp
  作者：zhou    版本：V1.0    创建日期：2018/07/10
  模块描述：BaseModule类的实现
  历史修改记录：<author>    <time>    <version >    <desc>
                  zhou   2018/07/10     V1.0      添加注释
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
  函数名称：initModule(QList<InfoUI *>& u)
  函数功能：Module初始化 加载UI 加载设备
  输入参数：u UI链表
  输出参数：
  返 回 值：
  备    注：
**************************************************/
int BaseModule::initModule(QList<InfoUI *>& u)
{
    return 0;
}

/*************************************************
  函数名称：getErrorType()
  函数功能：获取错误信息类型
  输入参数：
  输出参数：
  返 回 值：int  错误类型
  备    注：
**************************************************/
int   BaseModule::getErrorType()
{
    return m_errInfo.e_type;
}

/*************************************************
  函数名称：getErrorType()
  函数功能：获取错误信息类型
  输入参数：
  输出参数：
  返 回 值：int  错误类型
  备    注：
**************************************************/
ModuleInfo* BaseModule::getModuleInfo()
{
    return &m_ModuleInfo;
}

/*************************************************
  函数名称：getDevice()
  函数功能：获取设备指针
  输入参数：
  输出参数：
  返 回 值：BaseDevice*  设备指针
  备    注：
**************************************************/
BaseDevice* BaseModule::getDevice()
{
    return m_pDevice;
}

/*************************************************
  函数名称：getUIs()
  函数功能：获取Moudle所属的UI类型
  输入参数：
  输出参数：
  返 回 值：QList<InfoUI *>*  返回Moudle所属的UI链表指针
  备    注：
**************************************************/
QList<InfoUI *>* BaseModule::getUIs()
{
    return &m_UIs;
}

/*************************************************
  函数名称：getUIbyNames(QString s, ErrorInfo& e)
  函数功能：根据设备名，获得InfoUI信息
  输入参数：s  设备名
  输出参数：e  错误信息
  返 回 值：InfoUI *  返回UI指针
  备    注：
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


