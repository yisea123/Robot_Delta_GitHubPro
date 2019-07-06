#include "XmlControllerModule.h"
#include "XmlContrrollerDevice.h"
#include "SystemSchedule.h"



XmlControllerModule::XmlControllerModule()
{

}

XmlControllerModule::XmlControllerModule(QList<InfoUI *>& u, SystemSchedule* ss)
{
    m_ModuleInfo = ModulesInfo[XML_CONTROLLER_ID];
    m_pScheduler = ss;   

    m_errInfo.setModuleInfo(m_ModuleInfo);
    m_errInfo.e_type = DEBUG_NO_ERROR;
}

XmlControllerModule::~XmlControllerModule()
{
}

int XmlControllerModule::initModule(QList<InfoUI *>& u)
{
    findUIResource(u, m_errInfo);

    if(addDevice(m_errInfo) != 0)
    {
        m_pScheduler->recvErrMsgFromAllModules(m_errInfo);
        return -1;
    }
    else
    {
        return 0;
    }
}

CSystemPreferencesUi*  XmlControllerModule::getSysPreferencesUIHandler()
{
    for(int i = 0; i < m_UIs.size(); i++)
    {
        if(m_UIs.at(i)->nUISerial == UI_SYSTEM_PREFERENCES)
        {
            return   (CSystemPreferencesUi *)m_UIs.at(i)->pUi;
        }
    }
    return  NULL;
}

CAxesPreferencesUi*  XmlControllerModule::getAxisPreferencesUIHandler()
{
    for(int i = 0; i < m_UIs.size(); i++)
    {
        if(m_UIs.at(i)->nUISerial == UI_AXES_PREFERENCES)
        {
            return   (CAxesPreferencesUi *)m_UIs.at(i)->pUi;
        }
    }
    return  NULL;
}

CToolPreferencesUi*  XmlControllerModule::getToolPreferencesUIHandler()
{
    for(int i = 0; i < m_UIs.size(); i++)
    {
        if(m_UIs.at(i)->nUISerial == UI_TOOL_PREFERENCES)
        {
            return   (CToolPreferencesUi *)m_UIs.at(i)->pUi;
        }
    }
    return  NULL;
}

CStructSettingsUi*  XmlControllerModule::getStructsSettingsUIHandler()
{
    for(int i = 0; i < m_UIs.size(); i++)
    {
        if(m_UIs.at(i)->nUISerial == UI_TOOL_STRUCT_SETTINGS)
        {
            return   (CStructSettingsUi *)m_UIs.at(i)->pUi;
        }
    }
    return  NULL;
}

CCraftPreferencesUi*  XmlControllerModule::getCraftPreferencesUIHandler()
{
    for(int i = 0; i < m_UIs.size(); i++)
    {
        if(m_UIs.at(i)->nUISerial == UI_CRAFT_PREFERENCES)
        {
            return   (CCraftPreferencesUi *)m_UIs.at(i)->pUi;
        }
    }
    return  NULL;
}

CCommunicationSettingUi*  XmlControllerModule::getCommunicationSettingUIHandler()
{
    for(int i = 0; i < m_UIs.size(); i++)
    {
        if(m_UIs.at(i)->nUISerial == UI_COMMUNICATION_SETTINGS)
        {
            return   (CCommunicationSettingUi *)m_UIs.at(i)->pUi;
        }
    }
    return  NULL;
}

/*************************************************
  函数名称：findUIResource(QList<InfoUI *>& u, ErrorInfo& e)
  函数功能：在系统的所有注册的UI下，寻找MotionControllerModule使用的UI
  输入参数：u UI链表
           e  错误信息
  输出参数：
  返 回 值：
  备    注：
**************************************************/
int XmlControllerModule::findUIResource(QList<InfoUI *>& u, ErrorInfo& e)
{
    for(int i = 0; i < u.size(); i++)
    {
        for(int j = 0; j < u.at(i)->nModulesUser.size(); j++)
        {
            if(u.at(i)->nModulesUser[j] == XML_CONTROLLER_ID)   /* 匹配该Module */
               m_UIs.append(u.at(i));
        }
    }

    if(m_UIs.size() == 0)
    {
        e.insertDebug(DEBUG_WARNING, "ProcEmulationModule initModule find no ui");
    }
    return 0;
}

/*************************************************
  函数名称：addDevice(ErrorInfo& e)
  函数功能：加载设备
  输入参数： e  错误信息
  输出参数：
  返 回 值：
  备    注：
**************************************************/
int XmlControllerModule::addDevice(ErrorInfo& e)
{
    m_pDevice = new XmlContrrollerDevice(m_ModuleInfo.name, this);
    if (!loadSystemParameter(m_pScheduler->getSystemParameterHandler(), e))
    {
        m_pScheduler->recvErrMsgFromAllModules(e);
    }

    e.e_type = DEBUG_NO_ERROR;
    if(m_pDevice->openDevice(e) != 0)
    {
        e.insertDebug(DEBUG_SERIOUS_ERROR, "EmulationControllerModule addDevice failed!");
        m_pScheduler->recvErrMsgFromAllModules(e);
        return  -1;
    }
    else
    {
        return 0;
    }
}

bool XmlControllerModule::loadSystemParameter(SystemParameter* para, ErrorInfo& e)
{
    return ((XmlContrrollerDevice *)m_pDevice)->readXmlDoc(para, e);
}

void XmlControllerModule::absorbXMLModule(QString msg, QString arg)
{
    if (msg == "save XML")
    {
        saveParaToXML();
    }
    else if (msg == "read XML")
    {
        readParafromXML();
    }
    else if (msg == "write parm to XML")
    {
        saveParaToNewXML(arg);
    }
    else if (msg == "read parm from XML")
    {
        readParafromNewXML(arg);
    }
    else if (msg == "recover Parm from XML")
    {
        recoverParafromXML();
    }
    else if (msg == "save Net seting XML")
    {
        saveNetSettingToXML();
    }
    else if (msg == "recover Net seting XML")
    {
        recoverNetSettingfromXML();
    }
    else if (msg == "save Plc seting XML")
    {
        savePlcSettingToXML();
    }
    else if (msg == "recover Plc seting XML")
    {
        recoverPlcSettingfromXML();
    }
    else if (msg == "save Key seting XML")
    {
        saveKeySettingToXML();
    }
    else if (msg == "recover Key seting XML")
    {
        recoverKeySettingfromXML();
    }
    else if (msg == "cancel connect seting XML")
    {
        recoverConnectSettingfromXML();
    }
    else if (msg == "setHome")
    {
        ((XmlContrrollerDevice *)m_pDevice)->setHomeCoordinate(m_pScheduler->getSystemParameterHandler());
    }
}

void XmlControllerModule::saveParaToXML()
{
    ((XmlContrrollerDevice *)m_pDevice)->writeUIPramToXmlDoc(".//data//user.xml", m_pScheduler->getSystemParameterHandler(), m_errInfo);
}

void XmlControllerModule::readParafromXML()
{
    ((XmlContrrollerDevice *)m_pDevice)->readUIPramFromXmlDoc(".//data//user.xml", m_pScheduler->getSystemParameterHandler(), m_errInfo);
}

void XmlControllerModule::saveParaToNewXML(QString s)
{
    ((XmlContrrollerDevice *)m_pDevice)->writeUIPramToXmlDoc(s, m_pScheduler->getSystemParameterHandler(), m_errInfo);
}

void XmlControllerModule::readParafromNewXML(QString s)
{
    ((XmlContrrollerDevice *)m_pDevice)->readUIPramFromXmlDoc(s, m_pScheduler->getSystemParameterHandler(), m_errInfo);
}

void XmlControllerModule::recoverParafromXML()
{
    ((XmlContrrollerDevice *)m_pDevice)->readUIPramFromXmlDoc(".//data//default.xml", m_pScheduler->getSystemParameterHandler(), m_errInfo);
}

void XmlControllerModule::saveNetSettingToXML()
{
    ((XmlContrrollerDevice *)m_pDevice)->writeNetSettingToXmlDoc(".//connect//userConnectSetting.xml", m_pScheduler->getSystemParameterHandler(), m_errInfo);
}

void XmlControllerModule::recoverNetSettingfromXML()
{
    ((XmlContrrollerDevice *)m_pDevice)->readNetSettingFromXmlDoc(".//connect//defaultConnectSetting.xml", m_pScheduler->getSystemParameterHandler(), m_errInfo);
}

void XmlControllerModule::savePlcSettingToXML()
{
    ((XmlContrrollerDevice *)m_pDevice)->writePlcSettingToXmlDoc(".//connect//userConnectSetting.xml", m_pScheduler->getSystemParameterHandler(), m_errInfo);
}

void XmlControllerModule::recoverPlcSettingfromXML()
{
    ((XmlContrrollerDevice *)m_pDevice)->readPlcSettingFromXmlDoc(".//connect//defaultConnectSetting.xml", m_pScheduler->getSystemParameterHandler(), m_errInfo);
}

void XmlControllerModule::saveKeySettingToXML()
{
    ((XmlContrrollerDevice *)m_pDevice)->writeKeySettingToXmlDoc(".//connect//userConnectSetting.xml", m_pScheduler->getSystemParameterHandler(), m_errInfo);
}

void XmlControllerModule::recoverKeySettingfromXML()
{
    ((XmlContrrollerDevice *)m_pDevice)->readKeySettingFromXmlDoc(".//connect//defaultConnectSetting.xml", m_pScheduler->getSystemParameterHandler(), m_errInfo);
}

void XmlControllerModule::recoverConnectSettingfromXML()
{
    ((XmlContrrollerDevice *)m_pDevice)->readConnectSettingFromXmlDoc(".//connect//userConnectSetting.xml", m_pScheduler->getSystemParameterHandler(), m_errInfo);
}
