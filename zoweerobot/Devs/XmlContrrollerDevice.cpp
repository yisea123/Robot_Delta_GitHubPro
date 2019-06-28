/************************************************************
  Copyright (C),
  文件名：BaseDevice.cpp
  作者：zhou    版本：V1.0    创建日期：2018/07/10
  模块描述：BaseDevice类的实现
  历史修改记录：<author>    <time>    <version >    <desc>
                  zhou    2018/07/10     V1.0      添加注释
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
  函数名称：openDevice(ErrorInfo& e)
  函数功能：打开设备
  输入参数：
  输出参数：e  错误信息
  返 回 值：int 未定义
  备    注：
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
    QDomElement speedparameter = doc.createElement(QString("速度参数"));
    writeXmlParm(doc, speedparameter, QString("速度"), QString::number(para->m_speed.m_dV));
    writeXmlParm(doc, speedparameter, QString("加速度"), QString::number(para->m_speed.m_dA));
    root.appendChild(speedparameter);

    return true;
}

bool XmlContrrollerDevice::writeXmlMachineParm(QDomDocument& doc, QDomElement& root, MachineParemeter* para, ErrorInfo& e)
{
    QDomElement machineParm = doc.createElement(QString("系统参数"));

    /* 机械参数 */
    writeXmlParm(doc, machineParm, QString("机床尺寸X"), QString::number(para->m_dMachineSizeX));
    writeXmlParm(doc, machineParm, QString("机床尺寸Y"), QString::number(para->m_dMachineSizeY));
    writeXmlParm(doc, machineParm, QString("夹钳1偏置X"), QString::number(para->m_dGripper1OffsetX));
    writeXmlParm(doc, machineParm, QString("夹钳1偏置Y"), QString::number(para->m_dGripper1OffsetY));
    //writeXmlParm(doc, machineParm, QString("夹钳2偏置X"), QString::number(para->m_dGripper2OffsetX));
    //writeXmlParm(doc, machineParm, QString("夹钳2偏置Y"), QString::number(para->m_dGripper2OffsetY));
    writeXmlParm(doc, machineParm, QString("卸料位置X"), QString::number(para->m_unloadPosition.x));
    writeXmlParm(doc, machineParm, QString("卸料位置Y"), QString::number(para->m_unloadPosition.y));
    writeXmlParm(doc, machineParm, QString("T1反向间隙"), QString::number(para->m_dReverseGapT1));
    writeXmlParm(doc, machineParm, QString("T1正向间隙"), QString::number(para->m_dFrontGapT1));
    writeXmlParm(doc, machineParm, QString("T1角度偏置"), QString::number(para->m_dAngleOffsetT1));
    writeXmlParm(doc, machineParm, QString("T16反向间隙"), QString::number(para->m_dReverseGapT16));
    writeXmlParm(doc, machineParm, QString("T16正向间隙"), QString::number(para->m_dFrontGapT16));
    writeXmlParm(doc, machineParm, QString("T16角度偏置"), QString::number(para->m_dAngleOffsetT16));

    /* 加工参数 */
    writeXmlParm(doc, machineParm, QString("零件尺寸X"), QString::number(para->m_dPartSizeX));
    writeXmlParm(doc, machineParm, QString("零件尺寸Y"), QString::number(para->m_dPartSizeY));
    writeXmlParm(doc, machineParm, QString("零件厚度"), QString::number(para->m_dPartThickness));
    writeXmlParm(doc, machineParm, QString("机器回零坐标X"), QString::number(para->m_machineOriginalPosition.x));
    writeXmlParm(doc, machineParm, QString("机器回零坐标Y"), QString::number(para->m_machineOriginalPosition.y));
    writeXmlParm(doc, machineParm, QString("机器回零坐标T"), QString::number(para->m_machineOriginalPosition.z));
    writeXmlParm(doc, machineParm, QString("机器回零坐标C"), QString::number(para->m_machineOriginalPosition.r));
    //writeXmlParm(doc, machineParm, QString("机器回零坐标V"), QString::number(para->m_machineOriginalPosition.v));

    /* 生产参数 */
    writeXmlParm(doc, machineParm, QString("丝杆润滑时间"), QString::number(para->m_dScrewleverOilTime));
    writeXmlParm(doc, machineParm, QString("丝杆润滑间隔"), QString::number(para->m_dScrewleverOilInterval));
    writeXmlParm(doc, machineParm, QString("连杆润滑时间"), QString::number(para->m_dConnectingrodOilTime));
    writeXmlParm(doc, machineParm, QString("连杆润滑间隔"), QString::number(para->m_dConnectingrodOilInterval));
    writeXmlParm(doc, machineParm, QString("拟加工零件数量"), QString::number(para->m_nProcessPartNum));
    writeXmlParm(doc, machineParm, QString("零件完成停机标志"), QString::number(para->m_bIsProcessDoneDown));

    root.appendChild(machineParm);

    return true;
}

bool XmlContrrollerDevice::writeXmlAxisParm(QDomDocument& doc, QDomElement& root, QString axistext, AxisParemeter* para, ErrorInfo& e)
{
    QDomElement axisParm = doc.createElement("运动轴" + axistext + "参数");

    /* 速度参数 */
    writeXmlParm(doc, axisParm, axistext + QString("最大正转加速度"), QString::number(para->m_dCWMaxAcc));
    writeXmlParm(doc, axisParm, axistext + QString("最大反转加速度"), QString::number(para->m_dCCWMaxAcc));
    writeXmlParm(doc, axisParm, axistext + QString("最大正转速度"), QString::number(para->m_dCWMaxVel));
    writeXmlParm(doc, axisParm, axistext + QString("最大反转速度"), QString::number(para->m_dCCWMaxVel));
    writeXmlParm(doc, axisParm, axistext + QString("点动速度"), QString::number(para->m_dPTPVel));
    writeXmlParm(doc, axisParm, axistext + QString("搜索参考点速度"), QString::number(para->m_dToRefPointVel));
    writeXmlParm(doc, axisParm, axistext + QString("回零速度"), QString::number(para->m_dToHomeVel));
    //writeXmlParm(doc, axisParm, axistext + QString("加速度加速时间"), QString::number(para->m_dAccTime));

    /* 减速比参数 */
    //writeXmlParm(doc, axisParm, axistext + QString("减速比"), QString::number(para->m_dGearRatio));
    //writeXmlParm(doc, axisParm, axistext + QString("丝杆螺距"), QString::number(para->m_dScrewleverPitch));
    writeXmlParm(doc, axisParm, axistext + QString("脉冲当量"), QString::number(para->m_nPulseEquivalent));
    writeXmlParm(doc, axisParm, axistext + QString("减速比分子"), QString::number(para->m_nRatioNumerator));
    writeXmlParm(doc, axisParm, axistext + QString("减速比分母"), QString::number(para->m_nRatioDenominator));

    /* 限位 */
    writeXmlParm(doc, axisParm, axistext + QString("软限位正"), QString::number(para->m_axisSoftwarePositiveLimit));
    writeXmlParm(doc, axisParm, axistext + QString("软限位负"), QString::number(para->m_axisSoftwareNegativeLimit));

    /* 反向间隙 */
    writeXmlParm(doc, axisParm, axistext + QString("运动轴反向间隙"), QString::number(para->m_axisReverseGap));

    /* 工艺参数 */
    //writeXmlParm(doc, axisParm, axistext + QString("速度"), QString::number(para->m_nVec));
    //writeXmlParm(doc, axisParm, axistext + QString("脉冲"), QString::number(para->m_nPulse));
    //writeXmlParm(doc, axisParm, axistext + QString("方向"), QString::number(para->m_nDir));

    root.appendChild(axisParm);

    return true;
}

bool XmlContrrollerDevice::writeXmlMouldsParm(QDomDocument& doc, QDomElement& root, QString toolNum, const MouldsParameter* para, ErrorInfo& e)
{
    QDomElement mouldsParm = doc.createElement(QString("模具参数")+toolNum);

    writeXmlParm(doc, mouldsParm, QString("模具架编号"), QString::number(para->m_nFrameNum));
    writeXmlParm(doc, mouldsParm, QString("模具ID"), QString::number(para->m_nNum));
    writeXmlParm(doc, mouldsParm, QString("模具类型"), para->m_sType);
    writeXmlParm(doc, mouldsParm, QString("模具长度"), QString::number(para->m_dLength));
    writeXmlParm(doc, mouldsParm, QString("模具宽度"), QString::number(para->m_dWidth));
    writeXmlParm(doc, mouldsParm, QString("模具直径"), QString::number(para->m_dDiameter));
    writeXmlParm(doc, mouldsParm, QString("模具初始角度"), QString::number(para->m_dAngle));
    writeXmlParm(doc, mouldsParm, QString("模具可旋转标志"), QString::number(para->m_bIsRotary));
    writeXmlParm(doc, mouldsParm, QString("模具分度盘位置"), QString::number(para->m_dPosAngle));

    root.appendChild(mouldsParm);

    return true;
}

bool XmlContrrollerDevice::writeXmlCraftParm(QDomDocument& doc, QDomElement& root, const CraftParemeter* para, ErrorInfo& e)
{
    QDomElement craftParm = doc.createElement(QString("工艺参数"));

    //冲压工艺参数
    for (int i = 0; i < 5; i++)
    {
        writeXmlParm(doc, craftParm, QString("冲压速度" + QString::number(i)), QString::number(para->m_nPunchSpeed[i]));
        writeXmlParm(doc, craftParm, QString("冲压脉冲" + QString::number(i)), QString::number(para->m_nPunchPlus[i]));
        writeXmlParm(doc, craftParm, QString("冲压加速度" + QString::number(i)), QString::number(para->m_nPunchAcc[i]));
    }

    //各轴运动参数
    for (int i = 0; i < AXIS_ALL; i++)
    {
        writeXmlParm(doc, craftParm, QString("轴" + QString::number(i) + "速度"), QString::number(para->m_nSpeed[i]));//xml文件不支持标签为数字开头
        writeXmlParm(doc, craftParm, QString("轴" + QString::number(i) + "加速度"), QString::number(para->m_nAcc[i]));
        writeXmlParm(doc, craftParm, QString("轴" + QString::number(i) + "加加速度"), QString::number(para->m_nJ[i]));
    }

    //XY轴限制条件参数
    writeXmlParm(doc, craftParm, QString("X轴T0"), QString::number(para->m_nLimitT0[AXIS_X]));
    writeXmlParm(doc, craftParm, QString("X轴S1"), QString::number(para->m_nLimitS1[AXIS_X]));
    writeXmlParm(doc, craftParm, QString("X轴S2"), QString::number(para->m_nLimitS2[AXIS_X]));
    writeXmlParm(doc, craftParm, QString("X轴S3"), QString::number(para->m_nLimitS3[AXIS_X]));

    writeXmlParm(doc, craftParm, QString("Y轴T0"), QString::number(para->m_nLimitT0[AXIS_Y]));
    writeXmlParm(doc, craftParm, QString("Y轴S1"), QString::number(para->m_nLimitS1[AXIS_Y]));
    writeXmlParm(doc, craftParm, QString("Y轴S2"), QString::number(para->m_nLimitS2[AXIS_Y]));
    writeXmlParm(doc, craftParm, QString("Y轴S3"), QString::number(para->m_nLimitS3[AXIS_Y]));

//    writeXmlParm(doc, craftParm, QString("X轴T1"), QString::number(para->m_nLimitT1[AXIS_X]));
//    writeXmlParm(doc, craftParm, QString("X轴T2"), QString::number(para->m_nLimitT2[AXIS_X]));
//    writeXmlParm(doc, craftParm, QString("X轴P1"), QString::number(para->m_nLimitP1[AXIS_X]));
//    writeXmlParm(doc, craftParm, QString("X轴P2"), QString::number(para->m_nLimitP2[AXIS_X]));

//    writeXmlParm(doc, craftParm, QString("Y轴T1"), QString::number(para->m_nLimitT1[AXIS_Y]));
//    writeXmlParm(doc, craftParm, QString("Y轴T2"), QString::number(para->m_nLimitT2[AXIS_Y]));
//    writeXmlParm(doc, craftParm, QString("Y轴P1"), QString::number(para->m_nLimitP1[AXIS_Y]));
//    writeXmlParm(doc, craftParm, QString("Y轴P2"), QString::number(para->m_nLimitP2[AXIS_Y]));

    writeXmlParm(doc, craftParm, QString("T轴T0"), QString::number(para->m_nLimitT0[AXIS_T]));
    writeXmlParm(doc, craftParm, QString("T轴S1"), QString::number(para->m_nLimitS1[AXIS_T]));
    writeXmlParm(doc, craftParm, QString("T轴S2"), QString::number(para->m_nLimitS2[AXIS_T]));
    writeXmlParm(doc, craftParm, QString("T轴S3"), QString::number(para->m_nLimitS3[AXIS_T]));

    writeXmlParm(doc, craftParm, QString("C轴T0"), QString::number(para->m_nLimitT0[AXIS_C]));
    writeXmlParm(doc, craftParm, QString("C轴S1"), QString::number(para->m_nLimitS1[AXIS_C]));
    writeXmlParm(doc, craftParm, QString("C轴S2"), QString::number(para->m_nLimitS2[AXIS_C]));
    writeXmlParm(doc, craftParm, QString("C轴S3"), QString::number(para->m_nLimitS3[AXIS_C]));

    writeXmlParm(doc, craftParm, QString("X轴T1"), QString::number(para->m_nLimitT1[AXIS_X]));
    writeXmlParm(doc, craftParm, QString("X轴T2"), QString::number(para->m_nLimitT2[AXIS_X]));
    writeXmlParm(doc, craftParm, QString("X轴P1"), QString::number(para->m_nLimitP1[AXIS_X]));
    writeXmlParm(doc, craftParm, QString("X轴P2"), QString::number(para->m_nLimitP2[AXIS_X]));

    writeXmlParm(doc, craftParm, QString("Y轴T1"), QString::number(para->m_nLimitT1[AXIS_Y]));
    writeXmlParm(doc, craftParm, QString("Y轴T2"), QString::number(para->m_nLimitT2[AXIS_Y]));
    writeXmlParm(doc, craftParm, QString("Y轴P1"), QString::number(para->m_nLimitP1[AXIS_Y]));
    writeXmlParm(doc, craftParm, QString("Y轴P2"), QString::number(para->m_nLimitP2[AXIS_Y]));

    root.appendChild(craftParm);

    return true;
}

bool XmlContrrollerDevice::writeXmlClampParm(QDomDocument& doc, QDomElement& root, QString num, const Point* para, ErrorInfo& e)
{
    QDomElement mouldsParm = doc.createElement(QString("夹钳参数")+num);

    writeXmlParm(doc, mouldsParm, QString("夹钳位置X"), QString::number(para->x));
    writeXmlParm(doc, mouldsParm, QString("夹钳位置Y"), QString::number(para->y));

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

    QDomElement root = doc.createElement(QString("数控冲压系统参数"));
    doc.appendChild(root);

    QDomElement version = doc.createElement("软件版本");
    text = doc.createTextNode(QString::number(para->m_nVersion));
    version.appendChild(text);
    root.appendChild(version);

    QDomElement machineType = doc.createElement("机器型号");
    text = doc.createTextNode(QString::number(para->m_nMachineType));
    machineType.appendChild(text);
    root.appendChild(machineType);

    /*速度参数*/
    writeXmlSpeedParm(doc, root, para, e);

    /*系统参数*/
    writeXmlMachineParm(doc, root, &para->m_machineParm, e);

    /*运动轴参数*/
    writeXmlAxisParm(doc, root, "X", &para->m_axisParm[0], e);
    writeXmlAxisParm(doc, root, "Y", &para->m_axisParm[1], e);
    writeXmlAxisParm(doc, root, "T", &para->m_axisParm[2], e);
    writeXmlAxisParm(doc, root, "C", &para->m_axisParm[3], e);
    writeXmlAxisParm(doc, root, "V", &para->m_axisParm[4], e);

    /*工艺参数*/
    writeXmlCraftParm(doc, root, &para->m_craftParm, e);

    /*模具参数*/
    for(int i = 0;i < para->m_mouldsParm.size();i++)
    {
        writeXmlMouldsParm(doc, root, QString::number(i), &para->m_mouldsParm.at(i), e);
    }

    /*夹钳参数*/
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
        e.insertDebug(DEBUG_SERIOUS_ERROR, ("XML  " + label + "找不到!"));
        return false;
    }
    value = list.at(listNum).toElement().text().toDouble();

    return true;
}

bool XmlContrrollerDevice::readXmlNParm(QDomNodeList& list, int listNum, QString label, int& value, ErrorInfo& e)
{
    if(!(list.at(listNum).toElement().tagName() == label))
    {
        e.insertDebug(DEBUG_SERIOUS_ERROR, ("XML  " + label + "找不到!"));
        return false;
    }
    value = list.at(listNum).toElement().text().toInt();

    return true;
}

bool XmlContrrollerDevice::readXmlSParm(QDomNodeList& list, int listNum, QString label, QString& value, ErrorInfo& e)
{
    if(!(list.at(listNum).toElement().tagName() == label))
    {
        e.insertDebug(DEBUG_SERIOUS_ERROR, ("XML  " + label + "找不到!"));
        return false;
    }
    value = list.at(listNum).toElement().text();

    return true;
}

bool XmlContrrollerDevice::readXmlBParm(QDomNodeList& list, int listNum, QString label, bool& value, ErrorInfo& e)
{
    if(!(list.at(listNum).toElement().tagName() == label))
    {
        e.insertDebug(DEBUG_SERIOUS_ERROR, ("XML  " + label + "找不到!"));
        return false;
    }
    value = (bool)(list.at(listNum).toElement().text().toInt());

    return true;
}

bool XmlContrrollerDevice::readXmlTopNParm(QDomNode& node, QString label, int& value, ErrorInfo& e)
{
    if(!(node.toElement().tagName() == label))
    {
        e.insertDebug(DEBUG_SERIOUS_ERROR, ("XML  " + label + "参数找不到!"));
        return false;
    }
    value = node.toElement().text().toInt();

    node = node.nextSibling();

    return true;
}

bool XmlContrrollerDevice::readXmlSpeedParm(QDomNode& node, SpeedParameter* para, ErrorInfo& e)
{
    if(!(node.toElement().tagName() == QString("速度参数")))
    {
        e.insertDebug(DEBUG_SERIOUS_ERROR, "XML  速度参数找不到!");
        return false;
    }
    QDomNodeList list = node.childNodes();
    int listNum = 0;
    if(list.size() >= 2)
    {
        readXmlDParm(list, listNum++, QString("速度"), para->m_dV, e);
        readXmlDParm(list, listNum++, QString("加速度"), para->m_dA, e);
    }
    else
    {
        e.insertDebug(DEBUG_SERIOUS_ERROR, ("XML  速度参数缺失部分内容!"));
    }

    node = node.nextSibling();

    return true;
}

bool XmlContrrollerDevice::readXmlMachineParm(QDomNode& node, MachineParemeter* para, ErrorInfo& e)
{
    if(!(node.toElement().tagName() == QString("系统参数")))
    {
        e.insertDebug(DEBUG_SERIOUS_ERROR, "XML  系统参数找不到!");
        return false;
    }
    QDomNodeList list = node.childNodes();
    int listNum = 0;
    if(list.size() >= 26)
    {
        readXmlDParm(list, listNum++, QString("机床尺寸X"), para->m_dMachineSizeX, e);
        readXmlDParm(list, listNum++, QString("机床尺寸Y"), para->m_dMachineSizeY, e);
        readXmlDParm(list, listNum++, QString("夹钳1偏置X"), para->m_dGripper1OffsetX, e);
        readXmlDParm(list, listNum++, QString("夹钳1偏置Y"), para->m_dGripper1OffsetY, e);
        //readXmlDParm(list, listNum++, QString("夹钳2偏置X"), para->m_dGripper2OffsetX, e);
        //readXmlDParm(list, listNum++, QString("夹钳2偏置Y"), para->m_dGripper2OffsetY, e);
        readXmlDParm(list, listNum++, QString("卸料位置X"), para->m_unloadPosition.x, e);
        readXmlDParm(list, listNum++, QString("卸料位置Y"), para->m_unloadPosition.y, e);
        readXmlDParm(list, listNum++, QString("T1反向间隙"), para->m_dReverseGapT1, e);
        readXmlDParm(list, listNum++, QString("T1正向间隙"), para->m_dFrontGapT1, e);
        readXmlDParm(list, listNum++, QString("T1角度偏置"), para->m_dAngleOffsetT1, e);
        readXmlDParm(list, listNum++, QString("T16反向间隙"), para->m_dReverseGapT16, e);
        readXmlDParm(list, listNum++, QString("T16正向间隙"), para->m_dFrontGapT16, e);
        readXmlDParm(list, listNum++, QString("T16角度偏置"), para->m_dAngleOffsetT16, e);

        readXmlDParm(list, listNum++, QString("零件尺寸X"), para->m_dPartSizeX, e);
        readXmlDParm(list, listNum++, QString("零件尺寸Y"), para->m_dPartSizeY, e);
        readXmlDParm(list, listNum++, QString("零件厚度"), para->m_dPartThickness, e);
        readXmlDParm(list, listNum++, QString("机器回零坐标X"), para->m_machineOriginalPosition.x, e);
        readXmlDParm(list, listNum++, QString("机器回零坐标Y"), para->m_machineOriginalPosition.y, e);
        readXmlDParm(list, listNum++, QString("机器回零坐标T"), para->m_machineOriginalPosition.z, e);
        readXmlDParm(list, listNum++, QString("机器回零坐标C"), para->m_machineOriginalPosition.r, e);
        //readXmlDParm(list, listNum++, QString("机器回零坐标V"), para->m_machineOriginalPosition.v, e);

        readXmlDParm(list, listNum++, QString("丝杆润滑时间"), para->m_dScrewleverOilTime, e);
        readXmlDParm(list, listNum++, QString("丝杆润滑间隔"), para->m_dScrewleverOilInterval, e);
        readXmlDParm(list, listNum++, QString("连杆润滑时间"), para->m_dConnectingrodOilTime, e);
        readXmlDParm(list, listNum++, QString("连杆润滑间隔"), para->m_dConnectingrodOilInterval, e);
        readXmlNParm(list, listNum++, QString("拟加工零件数量"), para->m_nProcessPartNum, e);
        readXmlBParm(list, listNum++, QString("零件完成停机标志"), para->m_bIsProcessDoneDown, e);
    }
    else
    {
        e.insertDebug(DEBUG_SERIOUS_ERROR, ("XML  系统参数缺失部分内容!"));
    }

    node = node.nextSibling();

    return true;
}

bool XmlContrrollerDevice::readXmlAxisParm(QDomNode& node, QString axistext, AxisParemeter* para, ErrorInfo& e)
{
    if(!(node.toElement().tagName() == ("运动轴" + axistext + "参数")))
    {
        e.insertDebug(DEBUG_SERIOUS_ERROR, ("XML  运动轴" + axistext + "参数找不到!"));
        return false;
    }
    QDomNodeList list = node.childNodes();
    int listNum = 0;
    if(list.size() >= 11)
    {
        readXmlDParm(list, listNum++, axistext + QString("最大正转加速度"), para->m_dCWMaxAcc, e);
        readXmlDParm(list, listNum++, axistext + QString("最大反转加速度"), para->m_dCCWMaxAcc, e);
        readXmlDParm(list, listNum++, axistext + QString("最大正转速度"), para->m_dCWMaxVel, e);
        readXmlDParm(list, listNum++, axistext + QString("最大反转速度"), para->m_dCCWMaxVel, e);
        readXmlDParm(list, listNum++, axistext + QString("点动速度"), para->m_dPTPVel, e);
        readXmlDParm(list, listNum++, axistext + QString("搜索参考点速度"), para->m_dToRefPointVel, e);
        readXmlDParm(list, listNum++, axistext + QString("回零速度"), para->m_dToHomeVel, e);
        //readXmlDParm(list, listNum++, axistext + QString("加速度加速时间"), para->m_dAccTime, e);

        //readXmlDParm(list, listNum++, axistext + QString("减速比"), para->m_dGearRatio, e);
        //readXmlDParm(list, listNum++, axistext + QString("丝杆螺距"), para->m_dScrewleverPitch, e);
        readXmlNParm(list, listNum++, axistext + QString("脉冲当量"), para->m_nPulseEquivalent, e);
        readXmlNParm(list, listNum++, axistext + QString("减速比分子"), para->m_nRatioNumerator, e);
        readXmlNParm(list, listNum++, axistext + QString("减速比分母"), para->m_nRatioDenominator, e);

        readXmlDParm(list, listNum++, axistext + QString("软限位正"), para->m_axisSoftwarePositiveLimit, e);
        readXmlDParm(list, listNum++, axistext + QString("软限位负"), para->m_axisSoftwareNegativeLimit, e);

        readXmlNParm(list, listNum++, axistext + QString("运动轴反向间隙"), para->m_axisReverseGap, e);

        //readXmlNParm(list, listNum++, axistext + QString("速度"), para->m_nVec, e);
        //readXmlNParm(list, listNum++, axistext + QString("脉冲"), para->m_nPulse, e);
        //readXmlNParm(list, listNum++, axistext + QString("方向"), para->m_nDir, e);
    }
    else
    {
        e.insertDebug(DEBUG_SERIOUS_ERROR, ("XML  运动轴参数缺失部分内容!"));
    }

    node = node.nextSibling();

    return true;
}

bool XmlContrrollerDevice::readXmlMouldsParm(QDomNode& node, MouldsParameter* para, ErrorInfo& e)
{
//    if(!(node.toElement().tagName() == QString("模具参数")))
//    {
//        e.insertDebug(DEBUG_SERIOUS_ERROR, "XML  模具参数找不到!");
//        return false;
//    }
    QDomNodeList list = node.childNodes();
    int listNum = 0;
    if(list.size() >= 9)
    {
        readXmlNParm(list, listNum++, QString("模具架编号"), para->m_nFrameNum, e);
        readXmlNParm(list, listNum++, QString("模具ID"), para->m_nNum, e);
        readXmlSParm(list, listNum++, QString("模具类型"), para->m_sType, e);
        readXmlDParm(list, listNum++, QString("模具长度"), para->m_dLength, e);
        readXmlDParm(list, listNum++, QString("模具宽度"), para->m_dWidth, e);
        readXmlDParm(list, listNum++, QString("模具直径"), para->m_dDiameter, e);
        readXmlDParm(list, listNum++, QString("模具初始角度"), para->m_dAngle, e);
        readXmlBParm(list, listNum++, QString("模具可旋转标志"), para->m_bIsRotary, e);
        readXmlDParm(list, listNum++, QString("模具分度盘位置"), para->m_dPosAngle, e);

        if (para->m_sType == "RO")
        {
            para->m_sDimensions = QString("D=%1mm").arg(para->m_dDiameter);
            para->m_sTypeName = "圆形模(RO)";
        }
        else if (para->m_sType == "SQ")
        {
            para->m_sDimensions = QString("L=%1mm").arg(para->m_dLength);
            para->m_sTypeName = "方形模(SQ)";
        }
        else if (para->m_sType == "RE")
        {
            para->m_sDimensions = QString("L=%1mm,W=%2mm").arg(para->m_dLength).arg(para->m_dWidth);
            para->m_sTypeName = "长方模(RE)";
        }
        else if (para->m_sType == "OB")
        {
            para->m_sDimensions = QString("L=%1mm,D=%2mm").arg(para->m_dLength).arg(para->m_dDiameter);
            para->m_sTypeName = "长圆模(OB)";
        }
        else if (para->m_sType == "SD")
        {
            para->m_sDimensions = QString("D=%1mm,W=%2mm").arg(para->m_dDiameter).arg(para->m_dWidth);
            para->m_sTypeName = "单D形模(SD)";
        }
        else if (para->m_sType == "WD")
        {
            para->m_sDimensions = QString("D=%1mm,W=%2mm").arg(para->m_dDiameter).arg(para->m_dWidth);
            para->m_sTypeName = "双D形模(WD)";
        }
    }
    else
    {
        e.insertDebug(DEBUG_SERIOUS_ERROR, ("XML  模具参数缺失部分内容!"));
    }

    node = node.nextSibling();

    return true;
}

bool XmlContrrollerDevice::readXmlCraftParm(QDomNode& node, CraftParemeter* para, ErrorInfo& e)
{
    if(!(node.toElement().tagName() == ("工艺参数")))
    {
        e.insertDebug(DEBUG_SERIOUS_ERROR, ("XML  工艺参数找不到!"));
        return false;
    }
    QDomNodeList list = node.childNodes();
    int listNum = 0;
    if(list.size() >= 54)
    {
        //冲压工艺参数
        for (int i = 0; i < 5; i++)
        {
            readXmlNParm(list, listNum++, "冲压速度" + QString::number(i), para->m_nPunchSpeed[i], e);
            readXmlNParm(list, listNum++, "冲压脉冲" + QString::number(i), para->m_nPunchPlus[i], e);
            readXmlNParm(list, listNum++, "冲压加速度" + QString::number(i), para->m_nPunchAcc[i], e);
        }

        //各轴运动参数
        for (int i = 0; i < AXIS_ALL; i++)
        {
            readXmlNParm(list, listNum++, "轴" + QString::number(i) + "速度", para->m_nSpeed[i], e);
            readXmlNParm(list, listNum++, "轴" + QString::number(i) + "加速度", para->m_nAcc[i], e);
            readXmlNParm(list, listNum++, "轴" + QString::number(i) + "加加速度", para->m_nJ[i], e);
        }

        //XY轴限制条件参数
        readXmlDParm(list, listNum++, "X轴T0", para->m_nLimitT0[AXIS_X], e);
        readXmlDParm(list, listNum++, "X轴S1", para->m_nLimitS1[AXIS_X], e);
        readXmlDParm(list, listNum++, "X轴S2", para->m_nLimitS2[AXIS_X], e);
        readXmlDParm(list, listNum++, "X轴S3", para->m_nLimitS3[AXIS_X], e);

        readXmlDParm(list, listNum++, "Y轴T0", para->m_nLimitT0[AXIS_Y], e);
        readXmlDParm(list, listNum++, "Y轴S1", para->m_nLimitS1[AXIS_Y], e);
        readXmlDParm(list, listNum++, "Y轴S2", para->m_nLimitS2[AXIS_Y], e);
        readXmlDParm(list, listNum++, "Y轴S3", para->m_nLimitS3[AXIS_Y], e);

//        readXmlNParm(list, listNum++, "X轴T1", para->m_nLimitT1[AXIS_X], e);
//        readXmlNParm(list, listNum++, "X轴T2", para->m_nLimitT2[AXIS_X], e);
//        readXmlNParm(list, listNum++, "X轴P1", para->m_nLimitP1[AXIS_X], e);
//        readXmlNParm(list, listNum++, "X轴P2", para->m_nLimitP2[AXIS_X], e);

//        readXmlNParm(list, listNum++, "Y轴T1", para->m_nLimitT1[AXIS_Y], e);
//        readXmlNParm(list, listNum++, "Y轴T2", para->m_nLimitT2[AXIS_Y], e);
//        readXmlNParm(list, listNum++, "Y轴P1", para->m_nLimitP1[AXIS_Y], e);
//        readXmlNParm(list, listNum++, "Y轴P2", para->m_nLimitP2[AXIS_Y], e);

        readXmlDParm(list, listNum++, "T轴T0", para->m_nLimitT0[AXIS_T], e);
        readXmlDParm(list, listNum++, "T轴S1", para->m_nLimitS1[AXIS_T], e);
        readXmlDParm(list, listNum++, "T轴S2", para->m_nLimitS2[AXIS_T], e);
        readXmlDParm(list, listNum++, "T轴S3", para->m_nLimitS3[AXIS_T], e);

        readXmlDParm(list, listNum++, "C轴T0", para->m_nLimitT0[AXIS_C], e);
        readXmlDParm(list, listNum++, "C轴S1", para->m_nLimitS1[AXIS_C], e);
        readXmlDParm(list, listNum++, "C轴S2", para->m_nLimitS2[AXIS_C], e);
        readXmlDParm(list, listNum++, "C轴S3", para->m_nLimitS3[AXIS_C], e);

        readXmlNParm(list, listNum++, "X轴T1", para->m_nLimitT1[AXIS_X], e);
        readXmlNParm(list, listNum++, "X轴T2", para->m_nLimitT2[AXIS_X], e);
        readXmlNParm(list, listNum++, "X轴P1", para->m_nLimitP1[AXIS_X], e);
        readXmlNParm(list, listNum++, "X轴P2", para->m_nLimitP2[AXIS_X], e);

        readXmlNParm(list, listNum++, "Y轴T1", para->m_nLimitT1[AXIS_Y], e);
        readXmlNParm(list, listNum++, "Y轴T2", para->m_nLimitT2[AXIS_Y], e);
        readXmlNParm(list, listNum++, "Y轴P1", para->m_nLimitP1[AXIS_Y], e);
        readXmlNParm(list, listNum++, "Y轴P2", para->m_nLimitP2[AXIS_Y], e);
    }
    else
    {
        e.insertDebug(DEBUG_SERIOUS_ERROR, ("XML  工艺参数缺失部分内容!"));
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
        readXmlDParm(list, listNum++, QString("夹钳位置X"), para->x, e);
        readXmlDParm(list, listNum++, QString("夹钳位置Y"), para->y, e);
    }
    else
    {
        e.insertDebug(DEBUG_SERIOUS_ERROR, ("XML  夹钳位置参数缺失部分内容!"));
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

    readXmlTopNParm(node, "软件版本", para->m_nVersion, e);
    readXmlTopNParm(node, "机器型号", para->m_nMachineType, e);

    /*速度参数*/
    readXmlSpeedParm(node, &(para->m_speed), e);

    /*系统参数*/
    readXmlMachineParm(node, &para->m_machineParm, e);

    /*运动轴参数*/
    readXmlAxisParm(node, "X", &para->m_axisParm[0], e);
    readXmlAxisParm(node, "Y", &para->m_axisParm[1], e);
    readXmlAxisParm(node, "T", &para->m_axisParm[2], e);
    readXmlAxisParm(node, "C", &para->m_axisParm[3], e);
    readXmlAxisParm(node, "V", &para->m_axisParm[4], e);

    //工艺参数
    readXmlCraftParm(node, &para->m_craftParm, e);

    /*模具参数*/
    int i = 0;
    para->m_mouldsParm.clear();
    while (node.toElement().tagName() == ("模具参数" + QString::number(i)))
    {
        MouldsParameter  pp;
        readXmlMouldsParm(node, &pp, e);
        para->m_mouldsParm.append(pp);
        i++;
    }
    if (i <= 0)
    {
        e.insertDebug(DEBUG_SERIOUS_ERROR, "XML  模具参数为空，请添加模具!");
    }

    /*夹钳参数*/
    i = 0;
    para->m_clampPosition.clear();
    while (node.toElement().tagName() == ("夹钳参数" + QString::number(i)))
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
    QDomElement netSetting = doc.createElement(QString("网络连接设置"));
    writeXmlParm(doc, netSetting, QString("IP地址"), para->m_netConnectionParm.m_netIP);
    writeXmlParm(doc, netSetting, QString("掩码"), para->m_netConnectionParm.m_subNetMask);
    writeXmlParm(doc, netSetting, QString("网关"), para->m_netConnectionParm.m_GateWay);
    writeXmlParm(doc, netSetting, QString("端口号1"), QString::number(para->m_netConnectionParm.m_netPort1));
    writeXmlParm(doc, netSetting, QString("端口号2"), QString::number(para->m_netConnectionParm.m_netPort2));
    root.appendChild(netSetting);

    return true;
}

bool XmlContrrollerDevice::writeXmlPlcSetting(QDomDocument& doc, QDomElement& root, SystemParameter* para, ErrorInfo& e)
{
    QDomElement netSetting = doc.createElement(QString("PLC连接设置"));
    writeXmlParm(doc, netSetting, QString("串口端号"), para->m_serialPLCParm.m_portSerialNum);
    writeXmlParm(doc, netSetting, QString("波特率"), QString::number(para->m_serialPLCParm.m_baud));
    writeXmlParm(doc, netSetting, QString("数据位"), QString::number(para->m_serialPLCParm.m_dataBits));
    writeXmlParm(doc, netSetting, QString("停止位"), QString::number(para->m_serialPLCParm.m_stopBits));
    writeXmlParm(doc, netSetting, QString("奇偶校验"), para->m_serialPLCParm.m_ParityCheck);
    root.appendChild(netSetting);

    return true;
}

bool XmlContrrollerDevice::writeXmlKeySetting(QDomDocument& doc, QDomElement& root, SystemParameter* para, ErrorInfo& e)
{
    QDomElement netSetting = doc.createElement(QString("键盘连接设置"));
    writeXmlParm(doc, netSetting, QString("串口端号"), para->m_serialKeyParm.m_portSerialNum);
    writeXmlParm(doc, netSetting, QString("波特率"), QString::number(para->m_serialKeyParm.m_baud));
    writeXmlParm(doc, netSetting, QString("数据位"), QString::number(para->m_serialKeyParm.m_dataBits));
    writeXmlParm(doc, netSetting, QString("停止位"), QString::number(para->m_serialKeyParm.m_stopBits));
    writeXmlParm(doc, netSetting, QString("奇偶校验"), para->m_serialKeyParm.m_ParityCheck);
    root.appendChild(netSetting);

    return true;
}

bool XmlContrrollerDevice::writeCommunicationSettingXmlDoc(QString sXmlFilePath, SystemParameter *para, ErrorInfo &e)
{
    QDomDocument doc;
    QDomProcessingInstruction  instruction;
    instruction = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");

    doc.appendChild(instruction);

    QDomElement root = doc.createElement(QString("数控冲压系统参数外设通讯设置"));
    doc.appendChild(root);

    /*网络连接设置*/
    writeXmlNetSetting(doc, root, para, e);

    /*PLC连接设置*/
    writeXmlPlcSetting(doc, root, para, e);

    /*键盘连接设置*/
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
    if(!(node.toElement().tagName() == QString("网络连接设置")))
    {
        e.insertDebug(DEBUG_SERIOUS_ERROR, "XML  网络连接设置找不到!");
        return false;
    }
    QDomNodeList list = node.childNodes();
    int listNum = 0;
    if(list.size() >= 4)
    {
        readXmlSParm(list, listNum++, QString("IP地址"), para->m_netConnectionParm.m_netIP, e);
        readXmlSParm(list, listNum++, QString("掩码"), para->m_netConnectionParm.m_subNetMask, e);
        readXmlSParm(list, listNum++, QString("网关"), para->m_netConnectionParm.m_GateWay, e);
        readXmlNParm(list, listNum++, QString("端口号1"), para->m_netConnectionParm.m_netPort1, e);
        readXmlNParm(list, listNum++, QString("端口号2"), para->m_netConnectionParm.m_netPort2, e);
    }

    node = node.nextSibling();

    return true;
}

bool XmlContrrollerDevice::readXmlPlcSetting(QDomNode& node, SystemParameter* para, ErrorInfo& e)
{
    if(!(node.toElement().tagName() == QString("PLC连接设置")))
    {
        e.insertDebug(DEBUG_SERIOUS_ERROR, "XML  PLC连接设置找不到!");
        return false;
    }
    QDomNodeList list = node.childNodes();
    int listNum = 0;
    if(list.size() >= 5)
    {
        readXmlSParm(list, listNum++, QString("串口端号"), para->m_serialPLCParm.m_portSerialNum, e);
        readXmlNParm(list, listNum++, QString("波特率"), para->m_serialPLCParm.m_baud, e);
        readXmlNParm(list, listNum++, QString("数据位"), para->m_serialPLCParm.m_dataBits, e);
        readXmlNParm(list, listNum++, QString("停止位"), para->m_serialPLCParm.m_stopBits, e);
        readXmlSParm(list, listNum++, QString("奇偶校验"), para->m_serialPLCParm.m_ParityCheck, e);
    }

    node = node.nextSibling();

    return true;
}

bool XmlContrrollerDevice::readXmlKeySetting(QDomNode& node, SystemParameter* para, ErrorInfo& e)
{
    if(!(node.toElement().tagName() == QString("键盘连接设置")))
    {
        e.insertDebug(DEBUG_SERIOUS_ERROR, "XML  键盘连接设置找不到!");
        return false;
    }
    QDomNodeList list = node.childNodes();
    int listNum = 0;
    if(list.size() >= 5)
    {
        readXmlSParm(list, listNum++, QString("串口端号"), para->m_serialKeyParm.m_portSerialNum, e);
        readXmlNParm(list, listNum++, QString("波特率"), para->m_serialKeyParm.m_baud, e);
        readXmlNParm(list, listNum++, QString("数据位"), para->m_serialKeyParm.m_dataBits, e);
        readXmlNParm(list, listNum++, QString("停止位"), para->m_serialKeyParm.m_stopBits, e);
        readXmlSParm(list, listNum++, QString("奇偶校验"), para->m_serialKeyParm.m_ParityCheck, e);
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

    /*网络连接设置*/
    readXmlNetSetting(node, para, e);

    /*PLC连接设置*/
    readXmlPlcSetting(node, para, e);

    /*键盘连接设置*/
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

    QDomElement root = doc.createElement(QString("数控冲压系统用户信息"));
    doc.appendChild(root);

    QDomElement users;
    QDomText text;
    QDomElement userInfo;
    DesEncryptionModel  model;
    QString userName = "";
    QString password = "";
    for(int i = 0; i < usersparm.size(); i++)
    {
        /* 存储数据前先加密 */
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

    /*用户信息*/
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
                userparm.m_userName = model.Des_Run(userName, DesEncryptionModel::DECRYPT);//解密
                //userparm.m_userName.chop(1);
            }
            else
            {
                return false;
            }

            if (list.at(listNum).toElement().tagName() == "password")
            {
                password = list.at(listNum++).toElement().text();
                password = model.Des_Run(password, DesEncryptionModel::DECRYPT);//解密
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
            //e.insertDebug(DEBUG_SERIOUS_ERROR, "XML  IO" + QString::number(i) + "连接设置找不到!");
            return false;
        }
        QDomNodeList list = node.childNodes();
        if(list.size() >= 3)
        {
            if(list.at(0).toElement().tagName() == "类型")
            {
                io[i].type = list.at(0).toElement().text().toInt()==0 ? IO_STATUS_OUTPUT : IO_STATUS_INPUT;
            }
            else
                return false;

            if(list.at(1).toElement().tagName() == "初始状态")
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

    QDomElement root = doc.createElement(QString("数控冲压系统IO参数设置"));
    doc.appendChild(root);

    QDomElement ioSetting;
    QDomElement labelName;
    QDomText text;
    for (int i = 0; i < num; i++)
    {
        ioSetting = doc.createElement("IO"+ QString::number(i));

        labelName = doc.createElement(QString("类型"));
        text = doc.createTextNode(QString::number(io[i].type));
        labelName.appendChild(text);
        ioSetting.appendChild(labelName);

        labelName = doc.createElement(QString("初始状态"));
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
