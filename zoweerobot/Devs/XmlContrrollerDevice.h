#ifndef XMLCONTRROLLERDEVICE_H
#define XMLCONTRROLLERDEVICE_H

#include  "BaseDevice.h"
#include  "BaseModule.h"

#include <QDomDocument>
#include "GlobalVariableType.h"

//#include "ciostateui.h"


class  SystemParameter;
class  CSystemPreferencesUi;
class  CAxesPreferencesUi;
class  CToolPreferencesUi;
class  CCommunicationSettingUi;
class  CCraftPreferencesUi;


class XmlContrrollerDevice : public BaseDevice
{
    Q_OBJECT
public:
    XmlContrrollerDevice();
    XmlContrrollerDevice(QString s, BaseModule* m);

    int startDevice(ErrorInfo& e);
    int restartDevice(ErrorInfo& e);
    int pauseDevice(ErrorInfo& e);
    int openDevice(ErrorInfo& e);
    int stopDevice(ErrorInfo& e);

    bool readXmlDoc(SystemParameter* para, ErrorInfo& e);
    bool readXmlDoc(QString sXmlFilePath, SystemParameter* para, ErrorInfo& e);

    bool writeUIPramToXmlDoc(QString sXmlFilePath, SystemParameter* para, ErrorInfo& e);
    bool readUIPramFromXmlDoc(QString sXmlFilePath, SystemParameter* para, ErrorInfo& e);

    bool writeNetSettingToXmlDoc(QString sXmlFilePath, SystemParameter* para, ErrorInfo& e);
    bool readNetSettingFromXmlDoc(QString sXmlFilePath, SystemParameter* para, ErrorInfo& e);
    bool writePlcSettingToXmlDoc(QString sXmlFilePath, SystemParameter* para, ErrorInfo& e);
    bool readPlcSettingFromXmlDoc(QString sXmlFilePath, SystemParameter* para, ErrorInfo& e);
    bool writeKeySettingToXmlDoc(QString sXmlFilePath, SystemParameter* para, ErrorInfo& e);
    bool readKeySettingFromXmlDoc(QString sXmlFilePath, SystemParameter* para, ErrorInfo& e);
    bool readConnectSettingFromXmlDoc(QString sXmlFilePath, SystemParameter* para, ErrorInfo& e);

    bool addUserMenber(UserParameter user);

    //static bool readXmlIOStatus(IOStatusInfo* io, int num);
    //static bool writeXmlIOStatus(IOStatusInfo* io, int num);

    void setHomeCoordinate(SystemParameter* para);

private:
    int initDevice(ErrorInfo& e);
    bool writeXmlDoc(QString sXmlFilePath, SystemParameter* para, ErrorInfo& e);

    bool writeXmlParm(QDomDocument& doc, QDomElement& topElement, QString label, QString value);
    bool writeXmlSpeedParm(QDomDocument& doc, QDomElement& root, SystemParameter* para, ErrorInfo& e);
    bool writeXmlMachineParm(QDomDocument& doc, QDomElement& root, MachineParemeter* para, ErrorInfo& e);
    bool writeXmlAxisParm(QDomDocument& doc, QDomElement& root, QString axistext, AxisParemeter* para, ErrorInfo& e);
    bool writeXmlMouldsParm(QDomDocument& doc, QDomElement& root, QString toolNum, const MouldsParameter* para, ErrorInfo& e);
    bool writeXmlCraftParm(QDomDocument& doc, QDomElement& root, const CraftParemeter* para, ErrorInfo& e);
    bool writeXmlClampParm(QDomDocument& doc, QDomElement& root, QString num, const Point* para, ErrorInfo& e);

    bool readXmlDParm(QDomNodeList& list, int listNum, QString label, double& value, ErrorInfo& e);
    bool readXmlNParm(QDomNodeList& list, int listNum, QString label, int& value, ErrorInfo& e);
    bool readXmlSParm(QDomNodeList& list, int listNum, QString label, QString& value, ErrorInfo& e);
    bool readXmlBParm(QDomNodeList& list, int listNum, QString label, bool& value, ErrorInfo& e);
    //bool readXmlParm(QDomNodeList& list, int listNum, QString label, void* value, VariableType type, ErrorInfo& e);
    bool readXmlTopNParm(QDomNode& node, QString label, int& value, ErrorInfo& e);
    bool readXmlSpeedParm(QDomNode& node, SpeedParameter* para, ErrorInfo& e);
    bool readXmlMachineParm(QDomNode& node, MachineParemeter* para, ErrorInfo& e);
    bool readXmlAxisParm(QDomNode& node, QString axistext, AxisParemeter* para, ErrorInfo& e);
    bool readXmlMouldsParm(QDomNode& node, MouldsParameter* para, ErrorInfo& e);
    bool readXmlCraftParm(QDomNode& node, CraftParemeter* para, ErrorInfo& e);
    bool readXmlClampParm(QDomNode& node, Point* para, ErrorInfo& e);

    bool writeCommunicationSettingXmlDoc(QString sXmlFilePath, SystemParameter* para, ErrorInfo& e);
    bool readCommunicationSettingXmlDoc(QString sXmlFilePath, SystemParameter* para, ErrorInfo& e);
    bool writeXmlNetSetting(QDomDocument& doc, QDomElement& root, SystemParameter* para, ErrorInfo& e);
    bool writeXmlPlcSetting(QDomDocument& doc, QDomElement& root, SystemParameter* para, ErrorInfo& e);
    bool writeXmlKeySetting(QDomDocument& doc, QDomElement& root, SystemParameter* para, ErrorInfo& e);
    bool readXmlNetSetting(QDomNode& node, SystemParameter* para, ErrorInfo& e);
    bool readXmlPlcSetting(QDomNode& node, SystemParameter* para, ErrorInfo& e);
    bool readXmlKeySetting(QDomNode& node, SystemParameter* para, ErrorInfo& e);

    void creatParmFolder();

    bool readUserInfo(QVector<UserParameter> usersparm);
    bool writeUserInfo(QVector<UserParameter> usersparm);

private:
    SystemParameter*         m_pSyspara;
    CSystemPreferencesUi*    m_pui_SysPreference;
    CAxesPreferencesUi*      m_pui_AxesPreference;
    CToolPreferencesUi*      m_pui_ToolPreference;
    CCraftPreferencesUi*     m_pui_CraftPreference;
    CCommunicationSettingUi* m_pui_CommunicationSetting;
};

#endif // XMLCONTRROLLERDEVICE_H
