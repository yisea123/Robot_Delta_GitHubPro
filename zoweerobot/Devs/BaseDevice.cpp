/************************************************************
  Copyright (C),
  文件名：BaseDevice.cpp
  作者：zhou    版本：V1.0    创建日期：2018/07/10
  模块描述：BaseDevice类的实现
  历史修改记录：<author>    <time>    <version >    <desc>
                  zhou    2018/07/10     V1.0      添加注释
*************************************************************/

#include "BaseDevice.h"
#include "ComonTypes.h"

BaseDevice::BaseDevice(QObject *parent) :
    QObject(parent)
{
    m_workStatus = WORK_STATUS_IDLE;
}

/*************************************************
  函数名称：initDevice(ErrorInfo& e)
  函数功能：设备初始化
  输入参数：
  输出参数： e  错误信息
  返 回 值： 0   成功
           非0  失败
  备    注：
**************************************************/
int BaseDevice::initDevice(ErrorInfo& e)
{
    return 0;
}

/*************************************************
  函数名称：getDeviceName()
  函数功能：获取设备名
  输入参数：
  输出参数：
  返 回 值：设备名
  备    注：
**************************************************/
QString BaseDevice::getDeviceName()
{
    return m_sDeviceName;
}

/*************************************************
  函数名称：startDevice(ErrorInfo& e)
  函数功能：启动设备
  输入参数：
  输出参数：
  返 回 值：0   成功
           非0  失败
  备    注：
**************************************************/
int BaseDevice::startDevice(ErrorInfo& e)
{
    return 0;
}

/*************************************************
  函数名称：restartDevice(ErrorInfo& e)
  函数功能：重启设备
  输入参数：
  输出参数：
  返 回 值：0   成功
           非0  失败
  备    注：
**************************************************/
int BaseDevice::restartDevice(ErrorInfo& e)
{
    return 0;
}

/*************************************************
  函数名称：pauseDevice(ErrorInfo& e)
  函数功能：暂停设备
  输入参数：
  输出参数：
  返 回 值：0   成功
           非0  失败
  备    注：
**************************************************/
int BaseDevice::pauseDevice(ErrorInfo& e)
{
    return 0;
}

/*************************************************
  函数名称：openDevice(ErrorInfo& e)
  函数功能：打开设备
  输入参数：
  输出参数：
  返 回 值：0   成功
           非0  失败
  备    注：
**************************************************/
int BaseDevice::openDevice(ErrorInfo& e)
{
    return 0;
}

/*************************************************
  函数名称：stopDevice(ErrorInfo& e)
  函数功能：停止设备
  输入参数：
  输出参数：
  返 回 值：0   成功
           非0  失败
  备    注：
**************************************************/
int BaseDevice::stopDevice(ErrorInfo& e)
{
    return 0;
}






