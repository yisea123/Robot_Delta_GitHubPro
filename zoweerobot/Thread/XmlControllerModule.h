#ifndef XMLCONTROLLERMODULE_H
#define XMLCONTROLLERMODULE_H

#include "BaseModule.h"
#include <QWidget>

class  SystemParameter;
class  CSystemPreferencesUi;
class  CAxesPreferencesUi;
class  CToolPreferencesUi;
class  CCraftPreferencesUi;
class  CCommunicationSettingUi;

class XmlControllerModule : public BaseModule
{
    Q_OBJECT
public:
    XmlControllerModule();
    XmlControllerModule(QList<InfoUI *>& u, SystemSchedule* ss);
    ~XmlControllerModule();

    int  initModule(QList<InfoUI *>& u);
    CSystemPreferencesUi*  getSysPreferencesUIHandler();
    CAxesPreferencesUi*  getAxisPreferencesUIHandler();
    CToolPreferencesUi*  getToolPreferencesUIHandler();
    CCraftPreferencesUi*  getCraftPreferencesUIHandler();
    CCommunicationSettingUi* getCommunicationSettingUIHandler();
private:
    int  findUIResource(QList<InfoUI *>& u, ErrorInfo& e);
    int  addDevice(ErrorInfo& e);
    bool loadSystemParameter(SystemParameter* para, ErrorInfo& e);

    void saveParaToXML();
    void readParafromXML();
    void saveParaToNewXML(QString s);
    void readParafromNewXML(QString s);
    void recoverParafromXML();

    void saveNetSettingToXML();
    void recoverNetSettingfromXML();
    void savePlcSettingToXML();
    void recoverPlcSettingfromXML();
    void saveKeySettingToXML();
    void recoverKeySettingfromXML();
    void recoverConnectSettingfromXML();

public slots:
    void absorbXMLModule(QString msg, QString arg);
//    void saveParaToXML();
//    void readParafromXML();
//    void saveParaToNewXML(QString s);
//    void readParafromNewXML(QString s);
//    void recoverParafromXML();

//    void saveNetSettingToXML();
//    void recoverNetSettingfromXML();
//    void savePlcSettingToXML();
//    void recoverPlcSettingfromXML();
//    void saveKeySettingToXML();
//    void recoverKeySettingfromXML();


};

#endif // XMLCONTROLLERMODULE_H
