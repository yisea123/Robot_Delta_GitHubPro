/************************************************************
  Copyright (C),
  �ļ�����BaseDevice.cpp
  ���ߣ�zhou    �汾��V1.0    �������ڣ�2018/07/10
  ģ��������BaseDevice���ʵ��
  ��ʷ�޸ļ�¼��<author>    <time>    <version >    <desc>
                  zhou    2018/07/10     V1.0      ���ע��
*************************************************************/

#include "XmlContrrollerDevice.h"
#include <QFile>
#include <QMessageBox>
#include <QtXml>
//#include <QDomDocument>
//#include "GlobalVariable/GlobalVariableType.h"

#include "XmlControllerModule.h"
//#include "csystempreferencesui.h"
//#include "caxespreferencesui.h"
//#include "ctoolpreferencesui.h"
//#include "ccraftpreferencesui.h"
//#include "ccommunicationsettingui.h"
//#include "ciostateui.h"

#include "DesEncryptionModel.h"

XmlContrrollerDevice::XmlContrrollerDevice()
{
}

XmlContrrollerDevice::XmlContrrollerDevice(QString s, BaseModule* m)
{
    m_sDeviceName = s;
    m_pModuleHandle = m;
}

/*************************************************
  �������ƣ�openDevice(ErrorInfo& e)
  �������ܣ����豸
  ���������
  ���������e  ������Ϣ
  �� �� ֵ��int δ����
  ��    ע��
**************************************************/
int XmlContrrollerDevice::openDevice(ErrorInfo& e)
{
    return  initDevice(e);
}

int XmlContrrollerDevice::initDevice(ErrorInfo& e)
{
    /*m_pui_SysPreference = ((XmlControllerModule *)m_pModuleHandle)->getSysPreferencesUIHandler();
    m_pui_SysPreference->updataView(m_pSyspara);

    m_pui_AxesPreference = ((XmlControllerModule *)m_pModuleHandle)->getAxisPreferencesUIHandler();
    m_pui_AxesPreference->updataView(m_pSyspara);

    m_pui_ToolPreference = ((XmlControllerModule *)m_pModuleHandle)->getToolPreferencesUIHandler();
    m_pui_ToolPreference->updataView(m_pSyspara);

    m_pui_CraftPreference = ((XmlControllerModule *)m_pModuleHandle)->getCraftPreferencesUIHandler();
    m_pui_CraftPreference->updataView(m_pSyspara);

    m_pui_CommunicationSetting = ((XmlControllerModule *)m_pModuleHandle)->getCommunicationSettingUIHandler();
    m_pui_CommunicationSetting->updataView(m_pSyspara);*/
    return 0;
}

int XmlContrrollerDevice::startDevice(ErrorInfo& e)
{
    return 0;
}

int XmlContrrollerDevice::restartDevice(ErrorInfo& e)
{
    return 0;
}

int XmlContrrollerDevice::pauseDevice(ErrorInfo& e)
{
    return 0;
}

int XmlContrrollerDevice::stopDevice(ErrorInfo& e)
{
    return 0;
}

bool XmlContrrollerDevice::writeXmlParm(QDomDocument& doc, QDomElement& topElement, QString label, QString value)
{
    QDomElement labelName = doc.createElement(label);
    QDomText text = doc.createTextNode(value);
    labelName.appendChild(text);

    topElement.appendChild(labelName);

    return true;
}

bool XmlContrrollerDevice::writeXmlSpeedParm(QDomDocument& doc, QDomElement& root, SystemParameter* para, ErrorInfo& e)
{
    QDomElement speedparameter = doc.createElement(QString("�ٶȲ���"));
    writeXmlParm(doc, speedparameter, QString("�ٶ�"), QString::number(para->m_speed.m_dV));
    writeXmlParm(doc, speedparameter, QString("���ٶ�"), QString::number(para->m_speed.m_dA));
    root.appendChild(speedparameter);

    return true;
}

bool XmlContrrollerDevice::writeXmlMachineParm(QDomDocument& doc, QDomElement& root, MachineParemeter* para, ErrorInfo& e)
{
    QDomElement machineParm = doc.createElement(QString("ϵͳ����"));

    /* ��е���� */
    writeXmlParm(doc, machineParm, QString("�����ߴ�X"), QString::number(para->m_dMachineSizeX));
    writeXmlParm(doc, machineParm, QString("�����ߴ�Y"), QString::number(para->m_dMachineSizeY));
    writeXmlParm(doc, machineParm, QString("��ǯ1ƫ��X"), QString::number(para->m_dGripper1OffsetX));
    writeXmlParm(doc, machineParm, QString("��ǯ1ƫ��Y"), QString::number(para->m_dGripper1OffsetY));
    //writeXmlParm(doc, machineParm, QString("��ǯ2ƫ��X"), QString::number(para->m_dGripper2OffsetX));
    //writeXmlParm(doc, machineParm, QString("��ǯ2ƫ��Y"), QString::number(para->m_dGripper2OffsetY));
    writeXmlParm(doc, machineParm, QString("ж��λ��X"), QString::number(para->m_unloadPosition.x));
    writeXmlParm(doc, machineParm, QString("ж��λ��Y"), QString::number(para->m_unloadPosition.y));
    writeXmlParm(doc, machineParm, QString("T1�����϶"), QString::number(para->m_dReverseGapT1));
    writeXmlParm(doc, machineParm, QString("T1�����϶"), QString::number(para->m_dFrontGapT1));
    writeXmlParm(doc, machineParm, QString("T1�Ƕ�ƫ��"), QString::number(para->m_dAngleOffsetT1));
    writeXmlParm(doc, machineParm, QString("T16�����϶"), QString::number(para->m_dReverseGapT16));
    writeXmlParm(doc, machineParm, QString("T16�����϶"), QString::number(para->m_dFrontGapT16));
    writeXmlParm(doc, machineParm, QString("T16�Ƕ�ƫ��"), QString::number(para->m_dAngleOffsetT16));

    /* �ӹ����� */
    writeXmlParm(doc, machineParm, QString("����ߴ�X"), QString::number(para->m_dPartSizeX));
    writeXmlParm(doc, machineParm, QString("����ߴ�Y"), QString::number(para->m_dPartSizeY));
    writeXmlParm(doc, machineParm, QString("������"), QString::number(para->m_dPartThickness));
    writeXmlParm(doc, machineParm, QString("������������X"), QString::number(para->m_machineOriginalPosition.x));
    writeXmlParm(doc, machineParm, QString("������������Y"), QString::number(para->m_machineOriginalPosition.y));
    writeXmlParm(doc, machineParm, QString("������������T"), QString::number(para->m_machineOriginalPosition.z));
    writeXmlParm(doc, machineParm, QString("������������C"), QString::number(para->m_machineOriginalPosition.r));
//    writeXmlParm(doc, machineParm, QString("������������V"), QString::number(para->m_machineOriginalPosition.v));

    /* �������� */
    writeXmlParm(doc, machineParm, QString("˿����ʱ��"), QString::number(para->m_dScrewleverOilTime));
    writeXmlParm(doc, machineParm, QString("˿���󻬼��"), QString::number(para->m_dScrewleverOilInterval));
    writeXmlParm(doc, machineParm, QString("������ʱ��"), QString::number(para->m_dConnectingrodOilTime));
    writeXmlParm(doc, machineParm, QString("�����󻬼��"), QString::number(para->m_dConnectingrodOilInterval));
    writeXmlParm(doc, machineParm, QString("��ӹ��������"), QString::number(para->m_nProcessPartNum));
    writeXmlParm(doc, machineParm, QString("������ͣ����־"), QString::number(para->m_bIsProcessDoneDown));

    root.appendChild(machineParm);

    return true;
}

bool XmlContrrollerDevice::writeXmlAxisParm(QDomDocument& doc, QDomElement& root, QString axistext, AxisParemeter* para, ErrorInfo& e)
{
    QDomElement axisParm = doc.createElement("�˶���" + axistext + "����");

    /* �ٶȲ��� */
    writeXmlParm(doc, axisParm, axistext + QString("�����ת���ٶ�"), QString::number(para->m_dCWMaxAcc));
    writeXmlParm(doc, axisParm, axistext + QString("���ת���ٶ�"), QString::number(para->m_dCCWMaxAcc));
    writeXmlParm(doc, axisParm, axistext + QString("�����ת�ٶ�"), QString::number(para->m_dCWMaxVel));
    writeXmlParm(doc, axisParm, axistext + QString("���ת�ٶ�"), QString::number(para->m_dCCWMaxVel));
    writeXmlParm(doc, axisParm, axistext + QString("�㶯�ٶ�"), QString::number(para->m_dPTPVel));
    writeXmlParm(doc, axisParm, axistext + QString("�����ο����ٶ�"), QString::number(para->m_dToRefPointVel));
    writeXmlParm(doc, axisParm, axistext + QString("�����ٶ�"), QString::number(para->m_dToHomeVel));
    //writeXmlParm(doc, axisParm, axistext + QString("���ٶȼ���ʱ��"), QString::number(para->m_dAccTime));

    /* ���ٱȲ��� */
    //writeXmlParm(doc, axisParm, axistext + QString("���ٱ�"), QString::number(para->m_dGearRatio));
    //writeXmlParm(doc, axisParm, axistext + QString("˿���ݾ�"), QString::number(para->m_dScrewleverPitch));
    writeXmlParm(doc, axisParm, axistext + QString("���嵱��"), QString::number(para->m_nPulseEquivalent));
    writeXmlParm(doc, axisParm, axistext + QString("���ٱȷ���"), QString::number(para->m_nRatioNumerator));
    writeXmlParm(doc, axisParm, axistext + QString("���ٱȷ�ĸ"), QString::number(para->m_nRatioDenominator));

    /* ��λ */
    writeXmlParm(doc, axisParm, axistext + QString("����λ��"), QString::number(para->m_axisSoftwarePositiveLimit));
    writeXmlParm(doc, axisParm, axistext + QString("����λ��"), QString::number(para->m_axisSoftwareNegativeLimit));

    /* �����϶ */
    writeXmlParm(doc, axisParm, axistext + QString("�˶��ᷴ���϶"), QString::number(para->m_axisReverseGap));

    /* ���ղ��� */
    //writeXmlParm(doc, axisParm, axistext + QString("�ٶ�"), QString::number(para->m_nVec));
    //writeXmlParm(doc, axisParm, axistext + QString("����"), QString::number(para->m_nPulse));
    //writeXmlParm(doc, axisParm, axistext + QString("����"), QString::number(para->m_nDir));

    root.appendChild(axisParm);

    return true;
}

bool XmlContrrollerDevice::writeXmlMouldsParm(QDomDocument& doc, QDomElement& root, QString toolNum, const MouldsParameter* para, ErrorInfo& e)
{
    QDomElement mouldsParm = doc.createElement(QString("ģ�߲���")+toolNum);

    writeXmlParm(doc, mouldsParm, QString("ģ�߼ܱ��"), QString::number(para->m_nFrameNum));
    writeXmlParm(doc, mouldsParm, QString("ģ��ID"), QString::number(para->m_nNum));
    writeXmlParm(doc, mouldsParm, QString("ģ������"), para->m_sType);
    writeXmlParm(doc, mouldsParm, QString("ģ�߳���"), QString::number(para->m_dLength));
    writeXmlParm(doc, mouldsParm, QString("ģ�߿��"), QString::number(para->m_dWidth));
    writeXmlParm(doc, mouldsParm, QString("ģ��ֱ��"), QString::number(para->m_dDiameter));
    writeXmlParm(doc, mouldsParm, QString("ģ�߳�ʼ�Ƕ�"), QString::number(para->m_dAngle));
    writeXmlParm(doc, mouldsParm, QString("ģ�߿���ת��־"), QString::number(para->m_bIsRotary));
    writeXmlParm(doc, mouldsParm, QString("ģ�߷ֶ���λ��"), QString::number(para->m_dPosAngle));

    root.appendChild(mouldsParm);

    return true;
}

bool XmlContrrollerDevice::writeXmlCraftParm(QDomDocument& doc, QDomElement& root, const CraftParemeter* para, ErrorInfo& e)
{
    QDomElement craftParm = doc.createElement(QString("���ղ���"));

    //��ѹ���ղ���
    for (int i = 0; i < 5; i++)
    {
        writeXmlParm(doc, craftParm, QString("��ѹ�ٶ�" + QString::number(i)), QString::number(para->m_nPunchSpeed[i]));
        writeXmlParm(doc, craftParm, QString("��ѹ����" + QString::number(i)), QString::number(para->m_nPunchPlus[i]));
        writeXmlParm(doc, craftParm, QString("��ѹ���ٶ�" + QString::number(i)), QString::number(para->m_nPunchAcc[i]));
    }

    //�����˶�����
    for (int i = 0; i < AXIS_ALL; i++)
    {
        writeXmlParm(doc, craftParm, QString("��" + QString::number(i) + "�ٶ�"), QString::number(para->m_nSpeed[i]));//xml�ļ���֧�ֱ�ǩΪ���ֿ�ͷ
        writeXmlParm(doc, craftParm, QString("��" + QString::number(i) + "���ٶ�"), QString::number(para->m_nAcc[i]));
        writeXmlParm(doc, craftParm, QString("��" + QString::number(i) + "�Ӽ��ٶ�"), QString::number(para->m_nJ[i]));
    }

    //XY��������������
    writeXmlParm(doc, craftParm, QString("X��T0"), QString::number(para->m_nLimitT0[AXIS_X]));
    writeXmlParm(doc, craftParm, QString("X��S1"), QString::number(para->m_nLimitS1[AXIS_X]));
    writeXmlParm(doc, craftParm, QString("X��S2"), QString::number(para->m_nLimitS2[AXIS_X]));
    writeXmlParm(doc, craftParm, QString("X��S3"), QString::number(para->m_nLimitS3[AXIS_X]));

    writeXmlParm(doc, craftParm, QString("Y��T0"), QString::number(para->m_nLimitT0[AXIS_Y]));
    writeXmlParm(doc, craftParm, QString("Y��S1"), QString::number(para->m_nLimitS1[AXIS_Y]));
    writeXmlParm(doc, craftParm, QString("Y��S2"), QString::number(para->m_nLimitS2[AXIS_Y]));
    writeXmlParm(doc, craftParm, QString("Y��S3"), QString::number(para->m_nLimitS3[AXIS_Y]));

//    writeXmlParm(doc, craftParm, QString("X��T1"), QString::number(para->m_nLimitT1[AXIS_X]));
//    writeXmlParm(doc, craftParm, QString("X��T2"), QString::number(para->m_nLimitT2[AXIS_X]));
//    writeXmlParm(doc, craftParm, QString("X��P1"), QString::number(para->m_nLimitP1[AXIS_X]));
//    writeXmlParm(doc, craftParm, QString("X��P2"), QString::number(para->m_nLimitP2[AXIS_X]));

//    writeXmlParm(doc, craftParm, QString("Y��T1"), QString::number(para->m_nLimitT1[AXIS_Y]));
//    writeXmlParm(doc, craftParm, QString("Y��T2"), QString::number(para->m_nLimitT2[AXIS_Y]));
//    writeXmlParm(doc, craftParm, QString("Y��P1"), QString::number(para->m_nLimitP1[AXIS_Y]));
//    writeXmlParm(doc, craftParm, QString("Y��P2"), QString::number(para->m_nLimitP2[AXIS_Y]));

    writeXmlParm(doc, craftParm, QString("T��T0"), QString::number(para->m_nLimitT0[AXIS_T]));
    writeXmlParm(doc, craftParm, QString("T��S1"), QString::number(para->m_nLimitS1[AXIS_T]));
    writeXmlParm(doc, craftParm, QString("T��S2"), QString::number(para->m_nLimitS2[AXIS_T]));
    writeXmlParm(doc, craftParm, QString("T��S3"), QString::number(para->m_nLimitS3[AXIS_T]));

    writeXmlParm(doc, craftParm, QString("C��T0"), QString::number(para->m_nLimitT0[AXIS_C]));
    writeXmlParm(doc, craftParm, QString("C��S1"), QString::number(para->m_nLimitS1[AXIS_C]));
    writeXmlParm(doc, craftParm, QString("C��S2"), QString::number(para->m_nLimitS2[AXIS_C]));
    writeXmlParm(doc, craftParm, QString("C��S3"), QString::number(para->m_nLimitS3[AXIS_C]));

    writeXmlParm(doc, craftParm, QString("X��T1"), QString::number(para->m_nLimitT1[AXIS_X]));
    writeXmlParm(doc, craftParm, QString("X��T2"), QString::number(para->m_nLimitT2[AXIS_X]));
    writeXmlParm(doc, craftParm, QString("X��P1"), QString::number(para->m_nLimitP1[AXIS_X]));
    writeXmlParm(doc, craftParm, QString("X��P2"), QString::number(para->m_nLimitP2[AXIS_X]));

    writeXmlParm(doc, craftParm, QString("Y��T1"), QString::number(para->m_nLimitT1[AXIS_Y]));
    writeXmlParm(doc, craftParm, QString("Y��T2"), QString::number(para->m_nLimitT2[AXIS_Y]));
    writeXmlParm(doc, craftParm, QString("Y��P1"), QString::number(para->m_nLimitP1[AXIS_Y]));
    writeXmlParm(doc, craftParm, QString("Y��P2"), QString::number(para->m_nLimitP2[AXIS_Y]));

    root.appendChild(craftParm);

    return true;
}

bool XmlContrrollerDevice::writeXmlClampParm(QDomDocument& doc, QDomElement& root, QString num, const Point* para, ErrorInfo& e)
{
    QDomElement mouldsParm = doc.createElement(QString("��ǯ����")+num);

    writeXmlParm(doc, mouldsParm, QString("��ǯλ��X"), QString::number(para->x));
    writeXmlParm(doc, mouldsParm, QString("��ǯλ��Y"), QString::number(para->y));

    root.appendChild(mouldsParm);

    return true;
}

bool XmlContrrollerDevice::writeXmlDoc(QString sXmlFilePath, SystemParameter* para, ErrorInfo& e)
{

    QDomDocument doc;
    QDomText     text;
    QDomProcessingInstruction  instruction;
    instruction = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");

    doc.appendChild(instruction);

    QDomElement root = doc.createElement(QString("���س�ѹϵͳ����"));
    doc.appendChild(root);

    QDomElement version = doc.createElement("����汾");
    text = doc.createTextNode(QString::number(para->m_nVersion));
    version.appendChild(text);
    root.appendChild(version);

    QDomElement machineType = doc.createElement("�����ͺ�");
    text = doc.createTextNode(QString::number(para->m_nMachineType));
    machineType.appendChild(text);
    root.appendChild(machineType);

    /*�ٶȲ���*/
    writeXmlSpeedParm(doc, root, para, e);

    /*ϵͳ����*/
    writeXmlMachineParm(doc, root, &para->m_machineParm, e);

    /*�˶������*/
    writeXmlAxisParm(doc, root, "X", &para->m_axisParm[0], e);
    writeXmlAxisParm(doc, root, "Y", &para->m_axisParm[1], e);
    writeXmlAxisParm(doc, root, "T", &para->m_axisParm[2], e);
    writeXmlAxisParm(doc, root, "C", &para->m_axisParm[3], e);
    writeXmlAxisParm(doc, root, "V", &para->m_axisParm[4], e);

    /*���ղ���*/
    writeXmlCraftParm(doc, root, &para->m_craftParm, e);

    /*ģ�߲���*/
    for(int i = 0;i < para->m_mouldsParm.size();i++)
    {
        writeXmlMouldsParm(doc, root, QString::number(i), &para->m_mouldsParm.at(i), e);
    }

    /*��ǯ����*/
    for(int i = 0;i < para->m_clampPosition.size();i++)
    {
        writeXmlClampParm(doc, root, QString::number(i), &para->m_clampPosition.at(i), e);
    }

    QFile file(sXmlFilePath);

    if (!file.open(QIODevice::ReadWrite | QIODevice::Truncate))
    {
        e.insertDebug(DEBUG_CREAT_ERROR, sXmlFilePath + "no creat success!");
        return false;
    }
    QTextStream out(&file);
    doc.save(out, 4);
    file.close();

    return true;
}

bool XmlContrrollerDevice::readXmlDParm(QDomNodeList& list, int listNum, QString label, double& value, ErrorInfo& e)
{
    if(!(list.at(listNum).toElement().tagName() == label))
    {
        e.insertDebug(DEBUG_SERIOUS_ERROR, ("XML  " + label + "�Ҳ���!"));
        return false;
    }
    value = list.at(listNum).toElement().text().toDouble();

    return true;
}

bool XmlContrrollerDevice::readXmlNParm(QDomNodeList& list, int listNum, QString label, int& value, ErrorInfo& e)
{
    if(!(list.at(listNum).toElement().tagName() == label))
    {
        e.insertDebug(DEBUG_SERIOUS_ERROR, ("XML  " + label + "�Ҳ���!"));
        return false;
    }
    value = list.at(listNum).toElement().text().toInt();

    return true;
}

bool XmlContrrollerDevice::readXmlSParm(QDomNodeList& list, int listNum, QString label, QString& value, ErrorInfo& e)
{
    if(!(list.at(listNum).toElement().tagName() == label))
    {
        e.insertDebug(DEBUG_SERIOUS_ERROR, ("XML  " + label + "�Ҳ���!"));
        return false;
    }
    value = list.at(listNum).toElement().text();

    return true;
}

bool XmlContrrollerDevice::readXmlBParm(QDomNodeList& list, int listNum, QString label, bool& value, ErrorInfo& e)
{
    if(!(list.at(listNum).toElement().tagName() == label))
    {
        e.insertDebug(DEBUG_SERIOUS_ERROR, ("XML  " + label + "�Ҳ���!"));
        return false;
    }
    value = (bool)(list.at(listNum).toElement().text().toInt());

    return true;
}

bool XmlContrrollerDevice::readXmlTopNParm(QDomNode& node, QString label, int& value, ErrorInfo& e)
{
    if(!(node.toElement().tagName() == label))
    {
        e.insertDebug(DEBUG_SERIOUS_ERROR, ("XML  " + label + "�����Ҳ���!"));
        return false;
    }
    value = node.toElement().text().toInt();

    node = node.nextSibling();

    return true;
}

bool XmlContrrollerDevice::readXmlSpeedParm(QDomNode& node, SpeedParameter* para, ErrorInfo& e)
{
    if(!(node.toElement().tagName() == QString("�ٶȲ���")))
    {
        e.insertDebug(DEBUG_SERIOUS_ERROR, "XML  �ٶȲ����Ҳ���!");
        return false;
    }
    QDomNodeList list = node.childNodes();
    int listNum = 0;
    if(list.size() >= 2)
    {
        readXmlDParm(list, listNum++, QString("�ٶ�"), para->m_dV, e);
        readXmlDParm(list, listNum++, QString("���ٶ�"), para->m_dA, e);
    }
    else
    {
        e.insertDebug(DEBUG_SERIOUS_ERROR, ("XML  �ٶȲ���ȱʧ��������!"));
    }

    node = node.nextSibling();

    return true;
}

bool XmlContrrollerDevice::readXmlMachineParm(QDomNode& node, MachineParemeter* para, ErrorInfo& e)
{
    if(!(node.toElement().tagName() == QString("ϵͳ����")))
    {
        e.insertDebug(DEBUG_SERIOUS_ERROR, "XML  ϵͳ�����Ҳ���!");
        return false;
    }
    QDomNodeList list = node.childNodes();
    int listNum = 0;
    if(list.size() >= 26)
    {
        readXmlDParm(list, listNum++, QString("�����ߴ�X"), para->m_dMachineSizeX, e);
        readXmlDParm(list, listNum++, QString("�����ߴ�Y"), para->m_dMachineSizeY, e);
        readXmlDParm(list, listNum++, QString("��ǯ1ƫ��X"), para->m_dGripper1OffsetX, e);
        readXmlDParm(list, listNum++, QString("��ǯ1ƫ��Y"), para->m_dGripper1OffsetY, e);
        //readXmlDParm(list, listNum++, QString("��ǯ2ƫ��X"), para->m_dGripper2OffsetX, e);
        //readXmlDParm(list, listNum++, QString("��ǯ2ƫ��Y"), para->m_dGripper2OffsetY, e);
        readXmlDParm(list, listNum++, QString("ж��λ��X"), para->m_unloadPosition.x, e);
        readXmlDParm(list, listNum++, QString("ж��λ��Y"), para->m_unloadPosition.y, e);
        readXmlDParm(list, listNum++, QString("T1�����϶"), para->m_dReverseGapT1, e);
        readXmlDParm(list, listNum++, QString("T1�����϶"), para->m_dFrontGapT1, e);
        readXmlDParm(list, listNum++, QString("T1�Ƕ�ƫ��"), para->m_dAngleOffsetT1, e);
        readXmlDParm(list, listNum++, QString("T16�����϶"), para->m_dReverseGapT16, e);
        readXmlDParm(list, listNum++, QString("T16�����϶"), para->m_dFrontGapT16, e);
        readXmlDParm(list, listNum++, QString("T16�Ƕ�ƫ��"), para->m_dAngleOffsetT16, e);

        readXmlDParm(list, listNum++, QString("����ߴ�X"), para->m_dPartSizeX, e);
        readXmlDParm(list, listNum++, QString("����ߴ�Y"), para->m_dPartSizeY, e);
        readXmlDParm(list, listNum++, QString("������"), para->m_dPartThickness, e);
        readXmlDParm(list, listNum++, QString("������������X"), para->m_machineOriginalPosition.x, e);
        readXmlDParm(list, listNum++, QString("������������Y"), para->m_machineOriginalPosition.y, e);
        readXmlDParm(list, listNum++, QString("������������T"), para->m_machineOriginalPosition.z, e);
        readXmlDParm(list, listNum++, QString("������������C"), para->m_machineOriginalPosition.r, e);
//        readXmlDParm(list, listNum++, QString("������������V"), para->m_machineOriginalPosition.v, e);

        readXmlDParm(list, listNum++, QString("˿����ʱ��"), para->m_dScrewleverOilTime, e);
        readXmlDParm(list, listNum++, QString("˿���󻬼��"), para->m_dScrewleverOilInterval, e);
        readXmlDParm(list, listNum++, QString("������ʱ��"), para->m_dConnectingrodOilTime, e);
        readXmlDParm(list, listNum++, QString("�����󻬼��"), para->m_dConnectingrodOilInterval, e);
        readXmlNParm(list, listNum++, QString("��ӹ��������"), para->m_nProcessPartNum, e);
        readXmlBParm(list, listNum++, QString("������ͣ����־"), para->m_bIsProcessDoneDown, e);
    }
    else
    {
        e.insertDebug(DEBUG_SERIOUS_ERROR, ("XML  ϵͳ����ȱʧ��������!"));
    }

    node = node.nextSibling();

    return true;
}

bool XmlContrrollerDevice::readXmlAxisParm(QDomNode& node, QString axistext, AxisParemeter* para, ErrorInfo& e)
{
    if(!(node.toElement().tagName() == ("�˶���" + axistext + "����")))
    {
        e.insertDebug(DEBUG_SERIOUS_ERROR, ("XML  �˶���" + axistext + "�����Ҳ���!"));
        return false;
    }
    QDomNodeList list = node.childNodes();
    int listNum = 0;
    if(list.size() >= 11)
    {
        readXmlDParm(list, listNum++, axistext + QString("�����ת���ٶ�"), para->m_dCWMaxAcc, e);
        readXmlDParm(list, listNum++, axistext + QString("���ת���ٶ�"), para->m_dCCWMaxAcc, e);
        readXmlDParm(list, listNum++, axistext + QString("�����ת�ٶ�"), para->m_dCWMaxVel, e);
        readXmlDParm(list, listNum++, axistext + QString("���ת�ٶ�"), para->m_dCCWMaxVel, e);
        readXmlDParm(list, listNum++, axistext + QString("�㶯�ٶ�"), para->m_dPTPVel, e);
        readXmlDParm(list, listNum++, axistext + QString("�����ο����ٶ�"), para->m_dToRefPointVel, e);
        readXmlDParm(list, listNum++, axistext + QString("�����ٶ�"), para->m_dToHomeVel, e);
        //readXmlDParm(list, listNum++, axistext + QString("���ٶȼ���ʱ��"), para->m_dAccTime, e);

        //readXmlDParm(list, listNum++, axistext + QString("���ٱ�"), para->m_dGearRatio, e);
        //readXmlDParm(list, listNum++, axistext + QString("˿���ݾ�"), para->m_dScrewleverPitch, e);
        readXmlNParm(list, listNum++, axistext + QString("���嵱��"), para->m_nPulseEquivalent, e);
        readXmlNParm(list, listNum++, axistext + QString("���ٱȷ���"), para->m_nRatioNumerator, e);
        readXmlNParm(list, listNum++, axistext + QString("���ٱȷ�ĸ"), para->m_nRatioDenominator, e);

        readXmlDParm(list, listNum++, axistext + QString("����λ��"), para->m_axisSoftwarePositiveLimit, e);
        readXmlDParm(list, listNum++, axistext + QString("����λ��"), para->m_axisSoftwareNegativeLimit, e);

        readXmlNParm(list, listNum++, axistext + QString("�˶��ᷴ���϶"), para->m_axisReverseGap, e);

        //readXmlNParm(list, listNum++, axistext + QString("�ٶ�"), para->m_nVec, e);
        //readXmlNParm(list, listNum++, axistext + QString("����"), para->m_nPulse, e);
        //readXmlNParm(list, listNum++, axistext + QString("����"), para->m_nDir, e);
    }
    else
    {
        e.insertDebug(DEBUG_SERIOUS_ERROR, ("XML  �˶������ȱʧ��������!"));
    }

    node = node.nextSibling();

    return true;
}

bool XmlContrrollerDevice::readXmlMouldsParm(QDomNode& node, MouldsParameter* para, ErrorInfo& e)
{
//    if(!(node.toElement().tagName() == QString("ģ�߲���")))
//    {
//        e.insertDebug(DEBUG_SERIOUS_ERROR, "XML  ģ�߲����Ҳ���!");
//        return false;
//    }
    QDomNodeList list = node.childNodes();
    int listNum = 0;
    if(list.size() >= 9)
    {
        readXmlNParm(list, listNum++, QString("ģ�߼ܱ��"), para->m_nFrameNum, e);
        readXmlNParm(list, listNum++, QString("ģ��ID"), para->m_nNum, e);
        readXmlSParm(list, listNum++, QString("ģ������"), para->m_sType, e);
        readXmlDParm(list, listNum++, QString("ģ�߳���"), para->m_dLength, e);
        readXmlDParm(list, listNum++, QString("ģ�߿��"), para->m_dWidth, e);
        readXmlDParm(list, listNum++, QString("ģ��ֱ��"), para->m_dDiameter, e);
        readXmlDParm(list, listNum++, QString("ģ�߳�ʼ�Ƕ�"), para->m_dAngle, e);
        readXmlBParm(list, listNum++, QString("ģ�߿���ת��־"), para->m_bIsRotary, e);
        readXmlDParm(list, listNum++, QString("ģ�߷ֶ���λ��"), para->m_dPosAngle, e);

        if (para->m_sType == "RO")
        {
            para->m_sDimensions = QString("D=%1mm").arg(para->m_dDiameter);
            para->m_sTypeName = "Բ��ģ(RO)";
        }
        else if (para->m_sType == "SQ")
        {
            para->m_sDimensions = QString("L=%1mm").arg(para->m_dLength);
            para->m_sTypeName = "����ģ(SQ)";
        }
        else if (para->m_sType == "RE")
        {
            para->m_sDimensions = QString("L=%1mm,W=%2mm").arg(para->m_dLength).arg(para->m_dWidth);
            para->m_sTypeName = "����ģ(RE)";
        }
        else if (para->m_sType == "OB")
        {
            para->m_sDimensions = QString("L=%1mm,D=%2mm").arg(para->m_dLength).arg(para->m_dDiameter);
            para->m_sTypeName = "��Բģ(OB)";
        }
        else if (para->m_sType == "SD")
        {
            para->m_sDimensions = QString("D=%1mm,W=%2mm").arg(para->m_dDiameter).arg(para->m_dWidth);
            para->m_sTypeName = "��D��ģ(SD)";
        }
        else if (para->m_sType == "WD")
        {
            para->m_sDimensions = QString("D=%1mm,W=%2mm").arg(para->m_dDiameter).arg(para->m_dWidth);
            para->m_sTypeName = "˫D��ģ(WD)";
        }
    }
    else
    {
        e.insertDebug(DEBUG_SERIOUS_ERROR, ("XML  ģ�߲���ȱʧ��������!"));
    }

    node = node.nextSibling();

    return true;
}

bool XmlContrrollerDevice::readXmlCraftParm(QDomNode& node, CraftParemeter* para, ErrorInfo& e)
{
    if(!(node.toElement().tagName() == ("���ղ���")))
    {
        e.insertDebug(DEBUG_SERIOUS_ERROR, ("XML  ���ղ����Ҳ���!"));
        return false;
    }
    QDomNodeList list = node.childNodes();
    int listNum = 0;
    if(list.size() >= 54)
    {
        //��ѹ���ղ���
        for (int i = 0; i < 5; i++)
        {
            readXmlNParm(list, listNum++, "��ѹ�ٶ�" + QString::number(i), para->m_nPunchSpeed[i], e);
            readXmlNParm(list, listNum++, "��ѹ����" + QString::number(i), para->m_nPunchPlus[i], e);
            readXmlNParm(list, listNum++, "��ѹ���ٶ�" + QString::number(i), para->m_nPunchAcc[i], e);
        }

        //�����˶�����
        for (int i = 0; i < AXIS_ALL; i++)
        {
            readXmlNParm(list, listNum++, "��" + QString::number(i) + "�ٶ�", para->m_nSpeed[i], e);
            readXmlNParm(list, listNum++, "��" + QString::number(i) + "���ٶ�", para->m_nAcc[i], e);
            readXmlNParm(list, listNum++, "��" + QString::number(i) + "�Ӽ��ٶ�", para->m_nJ[i], e);
        }

        //XY��������������
        readXmlDParm(list, listNum++, "X��T0", para->m_nLimitT0[AXIS_X], e);
        readXmlDParm(list, listNum++, "X��S1", para->m_nLimitS1[AXIS_X], e);
        readXmlDParm(list, listNum++, "X��S2", para->m_nLimitS2[AXIS_X], e);
        readXmlDParm(list, listNum++, "X��S3", para->m_nLimitS3[AXIS_X], e);

        readXmlDParm(list, listNum++, "Y��T0", para->m_nLimitT0[AXIS_Y], e);
        readXmlDParm(list, listNum++, "Y��S1", para->m_nLimitS1[AXIS_Y], e);
        readXmlDParm(list, listNum++, "Y��S2", para->m_nLimitS2[AXIS_Y], e);
        readXmlDParm(list, listNum++, "Y��S3", para->m_nLimitS3[AXIS_Y], e);

//        readXmlNParm(list, listNum++, "X��T1", para->m_nLimitT1[AXIS_X], e);
//        readXmlNParm(list, listNum++, "X��T2", para->m_nLimitT2[AXIS_X], e);
//        readXmlNParm(list, listNum++, "X��P1", para->m_nLimitP1[AXIS_X], e);
//        readXmlNParm(list, listNum++, "X��P2", para->m_nLimitP2[AXIS_X], e);

//        readXmlNParm(list, listNum++, "Y��T1", para->m_nLimitT1[AXIS_Y], e);
//        readXmlNParm(list, listNum++, "Y��T2", para->m_nLimitT2[AXIS_Y], e);
//        readXmlNParm(list, listNum++, "Y��P1", para->m_nLimitP1[AXIS_Y], e);
//        readXmlNParm(list, listNum++, "Y��P2", para->m_nLimitP2[AXIS_Y], e);

        readXmlDParm(list, listNum++, "T��T0", para->m_nLimitT0[AXIS_T], e);
        readXmlDParm(list, listNum++, "T��S1", para->m_nLimitS1[AXIS_T], e);
        readXmlDParm(list, listNum++, "T��S2", para->m_nLimitS2[AXIS_T], e);
        readXmlDParm(list, listNum++, "T��S3", para->m_nLimitS3[AXIS_T], e);

        readXmlDParm(list, listNum++, "C��T0", para->m_nLimitT0[AXIS_C], e);
        readXmlDParm(list, listNum++, "C��S1", para->m_nLimitS1[AXIS_C], e);
        readXmlDParm(list, listNum++, "C��S2", para->m_nLimitS2[AXIS_C], e);
        readXmlDParm(list, listNum++, "C��S3", para->m_nLimitS3[AXIS_C], e);

        readXmlNParm(list, listNum++, "X��T1", para->m_nLimitT1[AXIS_X], e);
        readXmlNParm(list, listNum++, "X��T2", para->m_nLimitT2[AXIS_X], e);
        readXmlNParm(list, listNum++, "X��P1", para->m_nLimitP1[AXIS_X], e);
        readXmlNParm(list, listNum++, "X��P2", para->m_nLimitP2[AXIS_X], e);

        readXmlNParm(list, listNum++, "Y��T1", para->m_nLimitT1[AXIS_Y], e);
        readXmlNParm(list, listNum++, "Y��T2", para->m_nLimitT2[AXIS_Y], e);
        readXmlNParm(list, listNum++, "Y��P1", para->m_nLimitP1[AXIS_Y], e);
        readXmlNParm(list, listNum++, "Y��P2", para->m_nLimitP2[AXIS_Y], e);
    }
    else
    {
        e.insertDebug(DEBUG_SERIOUS_ERROR, ("XML  ���ղ���ȱʧ��������!"));
    }

    node = node.nextSibling();

    return true;
}

bool XmlContrrollerDevice::readXmlClampParm(QDomNode& node, Point* para, ErrorInfo& e)
{
    QDomNodeList list = node.childNodes();
    int listNum = 0;
    if(list.size() >= 2)
    {
        readXmlDParm(list, listNum++, QString("��ǯλ��X"), para->x, e);
        readXmlDParm(list, listNum++, QString("��ǯλ��Y"), para->y, e);
    }
    else
    {
        e.insertDebug(DEBUG_SERIOUS_ERROR, ("XML  ��ǯλ�ò���ȱʧ��������!"));
    }

    node = node.nextSibling();

    return true;
}

bool XmlContrrollerDevice::readXmlDoc(QString sXmlFilePath, SystemParameter* para, ErrorInfo& e)
{
    QFile file(sXmlFilePath);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        e.insertDebug(DEBUG_CREAT_ERROR, sXmlFilePath + "no open success!");
        return false;
    }

    QDomDocument doc;

    if(!(doc.setContent(&file)))
    {
        file.close();
        e.insertDebug(DEBUG_CREAT_ERROR, sXmlFilePath + " no setContent success!");
        return false;
    }

    file.close();

    QDomElement root = doc.documentElement();

    QDomNode node = root.firstChild();

    readXmlTopNParm(node, "����汾", para->m_nVersion, e);
    readXmlTopNParm(node, "�����ͺ�", para->m_nMachineType, e);

    /*�ٶȲ���*/
    readXmlSpeedParm(node, &(para->m_speed), e);

    /*ϵͳ����*/
    readXmlMachineParm(node, &para->m_machineParm, e);

    /*�˶������*/
    readXmlAxisParm(node, "X", &para->m_axisParm[0], e);
    readXmlAxisParm(node, "Y", &para->m_axisParm[1], e);
    readXmlAxisParm(node, "T", &para->m_axisParm[2], e);
    readXmlAxisParm(node, "C", &para->m_axisParm[3], e);
    readXmlAxisParm(node, "V", &para->m_axisParm[4], e);

    //���ղ���
    readXmlCraftParm(node, &para->m_craftParm, e);

    /*ģ�߲���*/
    int i = 0;
    para->m_mouldsParm.clear();
    while (node.toElement().tagName() == ("ģ�߲���" + QString::number(i)))
    {
        MouldsParameter  pp;
        readXmlMouldsParm(node, &pp, e);
        para->m_mouldsParm.append(pp);
        i++;
    }
    if (i <= 0)
    {
        e.insertDebug(DEBUG_SERIOUS_ERROR, "XML  ģ�߲���Ϊ�գ������ģ��!");
    }

    /*��ǯ����*/
    i = 0;
    para->m_clampPosition.clear();
    while (node.toElement().tagName() == ("��ǯ����" + QString::number(i)))
    {
        Point  pp;
        readXmlClampParm(node, &pp, e);
        para->m_clampPosition.append(pp);
        i++;
    }
    return true;
}

bool XmlContrrollerDevice::readXmlDoc(SystemParameter* para, ErrorInfo& e)
{
    creatParmFolder();

    QFile  user_file(".//data//user.xml");
    m_pSyspara = para;
    if (user_file.exists())
    {
        readXmlDoc(".//data//user.xml", para, e);
        if (e.e_type != DEBUG_NO_ERROR)
        {
            return false;
        }
    }
    else
    {
        QFile  default_file(".//data//default.xml");
        if(!default_file.exists())
        {
            QMessageBox msgBox;
            msgBox.setText("Has no find user.xml or default.xml");
            msgBox.setInformativeText("Do you want to restore default parameters?");
            msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
            msgBox.setDefaultButton(QMessageBox::Ok);
            int ret = msgBox.exec();

            if(ret == QMessageBox::Cancel)
            {
                e.insertDebug(DEBUG_SERIOUS_ERROR, "System parameter has no init!");
                return false;
            }

            writeXmlDoc(".//data//default.xml", para, e);
            writeXmlDoc(".//data//user.xml", para, e);
        }
    }

    QFile  user_ConnectSettingfile(".//connect//userConnectSetting.xml");
    if (user_ConnectSettingfile.exists())
    {
        readCommunicationSettingXmlDoc(".//connect//userConnectSetting.xml", para, e);
        if (e.e_type != DEBUG_NO_ERROR)
        {
            return false;
        }
    }
    else
    {
        QFile  default_ConnectSettingfile(".//connect//defaultConnectSetting.xml");
        if(!default_ConnectSettingfile.exists())
        {
            QMessageBox msgBox;
            msgBox.setText("Has no find userConnectSetting.xml or defaultConnectSetting.xml");
            msgBox.setInformativeText("Do you want to restore default ConnectSetting?");
            msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
            msgBox.setDefaultButton(QMessageBox::Ok);
            int ret = msgBox.exec();

            if(ret == QMessageBox::Cancel)
            {
                e.insertDebug(DEBUG_SERIOUS_ERROR, "ConnectSetting has no init!");
                return false;
            }

            writeCommunicationSettingXmlDoc(".//connect//defaultConnectSetting.xml", para, e);
            writeCommunicationSettingXmlDoc(".//connect//userConnectSetting.xml", para, e);
        }
    }

    return true;
}

bool XmlContrrollerDevice::writeUIPramToXmlDoc(QString sXmlFilePath, SystemParameter* para, ErrorInfo& e)
{
    /*m_pui_AxesPreference->saveParm(para);
    m_pui_SysPreference->saveParm(para);
    m_pui_ToolPreference->saveParm(para);
    m_pui_CraftPreference->saveParm(para);
    writeXmlDoc(sXmlFilePath, para, e);*/

    return true;
}

bool XmlContrrollerDevice::readUIPramFromXmlDoc(QString sXmlFilePath, SystemParameter* para, ErrorInfo& e)
{
    /*readXmlDoc(sXmlFilePath, para, e);
    m_pui_AxesPreference->updataView(para);
    m_pui_SysPreference->updataView(para);
    m_pui_ToolPreference->updataView(para);
    m_pui_CraftPreference->updataView(para);*/

    return true;
}

bool XmlContrrollerDevice::writeXmlNetSetting(QDomDocument& doc, QDomElement& root, SystemParameter* para, ErrorInfo& e)
{
    QDomElement netSetting = doc.createElement(QString("������������"));
    writeXmlParm(doc, netSetting, QString("IP��ַ"), para->m_netConnectionParm.m_netIP);
    writeXmlParm(doc, netSetting, QString("����"), para->m_netConnectionParm.m_subNetMask);
    writeXmlParm(doc, netSetting, QString("����"), para->m_netConnectionParm.m_GateWay);
    writeXmlParm(doc, netSetting, QString("�˿ں�1"), QString::number(para->m_netConnectionParm.m_netPort1));
    writeXmlParm(doc, netSetting, QString("�˿ں�2"), QString::number(para->m_netConnectionParm.m_netPort2));
    root.appendChild(netSetting);

    return true;
}

bool XmlContrrollerDevice::writeXmlPlcSetting(QDomDocument& doc, QDomElement& root, SystemParameter* para, ErrorInfo& e)
{
    QDomElement netSetting = doc.createElement(QString("PLC��������"));
    writeXmlParm(doc, netSetting, QString("���ڶ˺�"), para->m_serialPLCParm.m_portSerialNum);
    writeXmlParm(doc, netSetting, QString("������"), QString::number(para->m_serialPLCParm.m_baud));
    writeXmlParm(doc, netSetting, QString("����λ"), QString::number(para->m_serialPLCParm.m_dataBits));
    writeXmlParm(doc, netSetting, QString("ֹͣλ"), QString::number(para->m_serialPLCParm.m_stopBits));
    writeXmlParm(doc, netSetting, QString("��żУ��"), para->m_serialPLCParm.m_ParityCheck);
    root.appendChild(netSetting);

    return true;
}

bool XmlContrrollerDevice::writeXmlKeySetting(QDomDocument& doc, QDomElement& root, SystemParameter* para, ErrorInfo& e)
{
    QDomElement netSetting = doc.createElement(QString("������������"));
    writeXmlParm(doc, netSetting, QString("���ڶ˺�"), para->m_serialKeyParm.m_portSerialNum);
    writeXmlParm(doc, netSetting, QString("������"), QString::number(para->m_serialKeyParm.m_baud));
    writeXmlParm(doc, netSetting, QString("����λ"), QString::number(para->m_serialKeyParm.m_dataBits));
    writeXmlParm(doc, netSetting, QString("ֹͣλ"), QString::number(para->m_serialKeyParm.m_stopBits));
    writeXmlParm(doc, netSetting, QString("��żУ��"), para->m_serialKeyParm.m_ParityCheck);
    root.appendChild(netSetting);

    return true;
}

bool XmlContrrollerDevice::writeCommunicationSettingXmlDoc(QString sXmlFilePath, SystemParameter *para, ErrorInfo &e)
{
    QDomDocument doc;
    QDomProcessingInstruction  instruction;
    instruction = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");

    doc.appendChild(instruction);

    QDomElement root = doc.createElement(QString("���س�ѹϵͳ��������ͨѶ����"));
    doc.appendChild(root);

    /*������������*/
    writeXmlNetSetting(doc, root, para, e);

    /*PLC��������*/
    writeXmlPlcSetting(doc, root, para, e);

    /*������������*/
    writeXmlKeySetting(doc, root, para, e);

    QFile file(sXmlFilePath);

    if (!file.open(QIODevice::ReadWrite | QIODevice::Truncate))
    {
        e.insertDebug(DEBUG_CREAT_ERROR, sXmlFilePath + "no creat success!");
        return false;
    }
    QTextStream out(&file);
    doc.save(out, 4);
    file.close();

    return true;
}

bool XmlContrrollerDevice::readXmlNetSetting(QDomNode& node, SystemParameter* para, ErrorInfo& e)
{
    if(!(node.toElement().tagName() == QString("������������")))
    {
        e.insertDebug(DEBUG_SERIOUS_ERROR, "XML  �������������Ҳ���!");
        return false;
    }
    QDomNodeList list = node.childNodes();
    int listNum = 0;
    if(list.size() >= 4)
    {
        readXmlSParm(list, listNum++, QString("IP��ַ"), para->m_netConnectionParm.m_netIP, e);
        readXmlSParm(list, listNum++, QString("����"), para->m_netConnectionParm.m_subNetMask, e);
        readXmlSParm(list, listNum++, QString("����"), para->m_netConnectionParm.m_GateWay, e);
        readXmlNParm(list, listNum++, QString("�˿ں�1"), para->m_netConnectionParm.m_netPort1, e);
        readXmlNParm(list, listNum++, QString("�˿ں�2"), para->m_netConnectionParm.m_netPort2, e);
    }

    node = node.nextSibling();

    return true;
}

bool XmlContrrollerDevice::readXmlPlcSetting(QDomNode& node, SystemParameter* para, ErrorInfo& e)
{
    if(!(node.toElement().tagName() == QString("PLC��������")))
    {
        e.insertDebug(DEBUG_SERIOUS_ERROR, "XML  PLC���������Ҳ���!");
        return false;
    }
    QDomNodeList list = node.childNodes();
    int listNum = 0;
    if(list.size() >= 5)
    {
        readXmlSParm(list, listNum++, QString("���ڶ˺�"), para->m_serialPLCParm.m_portSerialNum, e);
        readXmlNParm(list, listNum++, QString("������"), para->m_serialPLCParm.m_baud, e);
        readXmlNParm(list, listNum++, QString("����λ"), para->m_serialPLCParm.m_dataBits, e);
        readXmlNParm(list, listNum++, QString("ֹͣλ"), para->m_serialPLCParm.m_stopBits, e);
        readXmlSParm(list, listNum++, QString("��żУ��"), para->m_serialPLCParm.m_ParityCheck, e);
    }

    node = node.nextSibling();

    return true;
}

bool XmlContrrollerDevice::readXmlKeySetting(QDomNode& node, SystemParameter* para, ErrorInfo& e)
{
    if(!(node.toElement().tagName() == QString("������������")))
    {
        e.insertDebug(DEBUG_SERIOUS_ERROR, "XML  �������������Ҳ���!");
        return false;
    }
    QDomNodeList list = node.childNodes();
    int listNum = 0;
    if(list.size() >= 5)
    {
        readXmlSParm(list, listNum++, QString("���ڶ˺�"), para->m_serialKeyParm.m_portSerialNum, e);
        readXmlNParm(list, listNum++, QString("������"), para->m_serialKeyParm.m_baud, e);
        readXmlNParm(list, listNum++, QString("����λ"), para->m_serialKeyParm.m_dataBits, e);
        readXmlNParm(list, listNum++, QString("ֹͣλ"), para->m_serialKeyParm.m_stopBits, e);
        readXmlSParm(list, listNum++, QString("��żУ��"), para->m_serialKeyParm.m_ParityCheck, e);
    }

    node = node.nextSibling();

    return true;
}

bool XmlContrrollerDevice::readCommunicationSettingXmlDoc(QString sXmlFilePath, SystemParameter *para, ErrorInfo &e)
{
    QFile file(sXmlFilePath);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        e.insertDebug(DEBUG_CREAT_ERROR, sXmlFilePath + "no open success!");
        return false;
    }

    QDomDocument doc;

    if(!(doc.setContent(&file)))
    {
        file.close();
        e.insertDebug(DEBUG_CREAT_ERROR, sXmlFilePath + " no setContent success!");
        return false;
    }

    file.close();

    QDomElement root = doc.documentElement();

    QDomNode node = root.firstChild();

    /*������������*/
    readXmlNetSetting(node, para, e);

    /*PLC��������*/
    readXmlPlcSetting(node, para, e);

    /*������������*/
    readXmlKeySetting(node, para, e);

    return true;
}

bool XmlContrrollerDevice::writeNetSettingToXmlDoc(QString sXmlFilePath, SystemParameter *para, ErrorInfo &e)
{
    /*m_pui_CommunicationSetting->saveNetParm(para);
    writeCommunicationSettingXmlDoc(sXmlFilePath, para, e);*/

    return true;
}

bool XmlContrrollerDevice::readNetSettingFromXmlDoc(QString sXmlFilePath, SystemParameter *para, ErrorInfo &e)
{
    readCommunicationSettingXmlDoc(sXmlFilePath, para, e);
    //m_pui_CommunicationSetting->updataViewNet(para);

    return true;
}

bool XmlContrrollerDevice::writePlcSettingToXmlDoc(QString sXmlFilePath, SystemParameter *para, ErrorInfo &e)
{
   // m_pui_CommunicationSetting->savePlcParm(para);
    //writeCommunicationSettingXmlDoc(sXmlFilePath, para, e);

    return true;
}

bool XmlContrrollerDevice::readPlcSettingFromXmlDoc(QString sXmlFilePath, SystemParameter *para, ErrorInfo &e)
{
    //readCommunicationSettingXmlDoc(sXmlFilePath, para, e);
    //m_pui_CommunicationSetting->updataViewPlc(para);

    return true;
}

bool XmlContrrollerDevice::writeKeySettingToXmlDoc(QString sXmlFilePath, SystemParameter *para, ErrorInfo &e)
{
    //m_pui_CommunicationSetting->saveKeyParm(para);
   // writeCommunicationSettingXmlDoc(sXmlFilePath, para, e);

    return true;
}

bool XmlContrrollerDevice::readKeySettingFromXmlDoc(QString sXmlFilePath, SystemParameter *para, ErrorInfo &e)
{
    //readCommunicationSettingXmlDoc(sXmlFilePath, para, e);
    //m_pui_CommunicationSetting->updataViewKey(para);

    return true;
}

bool XmlContrrollerDevice::readConnectSettingFromXmlDoc(QString sXmlFilePath, SystemParameter *para, ErrorInfo &e)
{
    //readCommunicationSettingXmlDoc(sXmlFilePath, para, e);
    //m_pui_CommunicationSetting->updataView(para);

    return true;
}

void XmlContrrollerDevice::creatParmFolder()
{
    QDir data;
    if (!data.exists("./data"))
    {
        data.mkdir("./data");
    }

    QDir connect;
    if (!connect.exists(".//connect"))
    {
        connect.mkdir(".//connect");
    }
}

bool XmlContrrollerDevice::writeUserInfo(QVector<UserParameter> usersparm)
{
    QDomDocument doc;
    QDomProcessingInstruction  instruction;
    instruction = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(instruction);

    QDomElement root = doc.createElement(QString("���س�ѹϵͳ�û���Ϣ"));
    doc.appendChild(root);

    QDomElement users;
    QDomText text;
    QDomElement userInfo;
    DesEncryptionModel  model;
    QString userName = "";
    QString password = "";
    for(int i = 0; i < usersparm.size(); i++)
    {
        /* �洢����ǰ�ȼ��� */
        userName = model.Des_Run(usersparm.at(i).m_userName, DesEncryptionModel::ENCRYPT);
        password = model.Des_Run(usersparm.at(i).m_password, DesEncryptionModel::ENCRYPT);

        users = doc.createElement("user" + QString::number(i));

        userInfo = doc.createElement("userName");
        text = doc.createTextNode(userName);
        userInfo.appendChild(text);
        users.appendChild(userInfo);

        userInfo = doc.createElement("password");
        text = doc.createTextNode(password);
        userInfo.appendChild(text);
        users.appendChild(userInfo);

        userInfo = doc.createElement("permission");
        text = doc.createTextNode(QString::number(usersparm.at(i).m_permission));
        userInfo.appendChild(text);
        users.appendChild(userInfo);

        root.appendChild(users);
    }

    QFile file(".//connect//users.xml");

    if (!file.open(QIODevice::ReadWrite | QIODevice::Truncate))
    {
        return false;
    }
    QTextStream out(&file);
    doc.save(out, 4);
    file.close();

    return true;
}

bool XmlContrrollerDevice::readUserInfo(QVector<UserParameter> usersparm)
{
    QFile  usersfile(".//connect//users.xml");
    if (!usersfile.open(QFile::ReadOnly | QFile::Text))
    {
        return false;
    }

    QDomDocument doc;
    if(!(doc.setContent(&usersfile)))
    {
        usersfile.close();
        return false;
    }
    usersfile.close();

    QDomElement users = doc.documentElement();
    QDomNode node = users.firstChild();

    /*�û���Ϣ*/
    DesEncryptionModel  model;
    QString userName = "";
    QString password = "";
    int i = 0;
    usersparm.clear();
    while (node.toElement().tagName() == ("user" + QString::number(i)))
    {
        UserParameter  userparm;
        QDomNodeList list = node.childNodes();
        int listNum = 0;
        if (list.size() >= 3)
        {
            if (list.at(listNum).toElement().tagName() == "userName")
            {
                userName = list.at(listNum++).toElement().text();
                userparm.m_userName = model.Des_Run(userName, DesEncryptionModel::DECRYPT);//����
                //userparm.m_userName.chop(1);
            }
            else
            {
                return false;
            }

            if (list.at(listNum).toElement().tagName() == "password")
            {
                password = list.at(listNum++).toElement().text();
                password = model.Des_Run(password, DesEncryptionModel::DECRYPT);//����
                userparm.m_password = password.left(8);
            }
            else
            {
                return false;
            }

            if (list.at(listNum).toElement().tagName() == "permission")
            {
                unsigned int temp = list.at(listNum++).toElement().text().toInt();
                if (temp <= 2)
                {
                    userparm.m_permission = (UserPermission)temp;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }

        usersparm.append(userparm);
        node = node.nextSibling();
        i++;
    }

    if (i <= 0)
    {
        return false;
    }
    return true;
}

bool XmlContrrollerDevice::addUserMenber(UserParameter user)
{
    QVector<UserParameter> usersParm;
    if (readUserInfo(usersParm))
    {
        usersParm.append(user);
        return writeUserInfo(usersParm);
    }
    return false;
}

/*bool XmlContrrollerDevice::readXmlIOStatus(IOStatusInfo* io, int num)
{
    QFile file(".//connect//ioSetting.xml");
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        //e.insertDebug(DEBUG_CREAT_ERROR, "ioSetting.xml no open success!");
        return false;
    }

    QDomDocument doc;
    if(!(doc.setContent(&file)))
    {
        file.close();
        //e.insertDebug(DEBUG_CREAT_ERROR, "ioSetting.xml no setContent success!");
        return false;
    }
    file.close();

    QDomElement root = doc.documentElement();
    QDomNode node = root.firstChild();

    for (int i = 0; i < num; i++)
    {
        if(!(node.toElement().tagName() == QString("IO"+ QString::number(i))))
        {
            //e.insertDebug(DEBUG_SERIOUS_ERROR, "XML  IO" + QString::number(i) + "���������Ҳ���!");
            return false;
        }
        QDomNodeList list = node.childNodes();
        if(list.size() >= 3)
        {
            if(list.at(0).toElement().tagName() == "����")
            {
                io[i].type = list.at(0).toElement().text().toInt()==0 ? IO_STATUS_OUTPUT : IO_STATUS_INPUT;
            }
            else
                return false;

            if(list.at(1).toElement().tagName() == "��ʼ״̬")
            {
                io[i].status = (bool)list.at(1).toElement().text().toInt();
            }
            else
                return false;

            if(list.at(2).toElement().tagName() == "ID")
            {
                io[i].id = list.at(2).toElement().text().toInt();
            }
            else
                return false;
        }
        node = node.nextSibling();
    }
    return true;
}

bool XmlContrrollerDevice::writeXmlIOStatus(IOStatusInfo* io, int num)
{
    QDomDocument doc;
    //QDomText     text;
    QDomProcessingInstruction  instruction;
    instruction = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");

    doc.appendChild(instruction);

    QDomElement root = doc.createElement(QString("���س�ѹϵͳIO��������"));
    doc.appendChild(root);

    QDomElement ioSetting;
    QDomElement labelName;
    QDomText text;
    for (int i = 0; i < num; i++)
    {
        ioSetting = doc.createElement("IO"+ QString::number(i));

        labelName = doc.createElement(QString("����"));
        text = doc.createTextNode(QString::number(io[i].type));
        labelName.appendChild(text);
        ioSetting.appendChild(labelName);

        labelName = doc.createElement(QString("��ʼ״̬"));
        text = doc.createTextNode(QString::number(io[i].status));
        labelName.appendChild(text);
        ioSetting.appendChild(labelName);

        labelName = doc.createElement(QString("ID"));
        text = doc.createTextNode(QString::number(io[i].id));
        labelName.appendChild(text);
        ioSetting.appendChild(labelName);

        root.appendChild(ioSetting);
    }

    QFile file(".//connect//ioSetting.xml");
    if (!file.open(QIODevice::ReadWrite | QIODevice::Truncate))
    {
        //e.insertDebug(DEBUG_CREAT_ERROR, "ioSetting.xml no creat success!");
        return false;
    }
    QTextStream out(&file);
    doc.save(out, 4);
    file.close();

    return true;
}*/

void XmlContrrollerDevice::setHomeCoordinate(SystemParameter *para)
{
    /*Point home = para->getMachineCoordinate();
    m_pui_SysPreference->setHomeCoordinate(home.x, home.y);*/
}
