/************************************************************
  Copyright (C),
  文件名：TCPSocket.cpp
  作者：zhou   版本：V1.0    创建日期：2018/07/10
  模块描述：TCPSocket类的实现
  历史修改记录：<author>    <time>    <version >    <desc>
                  zhou   2018/07/10     V1.0      添加注释
*************************************************************/

#include "TCPSocket.h"
#include <QHostAddress>

TCPSocket::TCPSocket(QString ip, quint16 port, QObject *parent)
    : QTcpSocket( parent )
{
    m_TcpLogInfo_send = new LogInfo("TcpLogInfo_send.txt");
    m_TcpLogInfo_send->writeToFile("Port1 Send&Receive Data\n");
    m_TcpLogInfo_receive = new LogInfo("TcpLogInfo_receive.txt");
    m_TcpLogInfo_receive->writeToFile("Port2 Send&Receive Data\n");

    m_bIsConnect = true;
    ConnectToServer(ip, port);
}

TCPSocket::~TCPSocket()
{
}

void TCPSocket::ConnectToServer(QString ip, quint16 port )
{
    //QString strip(ip);
    abort();//xodd
    QHostAddress addr;
    addr.setAddress(ip);
    connectToHost(addr, port);
    if(waitForConnected(1000))
    {
        m_bIsConnect = true;
    }
    else
    {
        disconnectFromHost();//xodd
        m_bIsConnect = false;
    }
}

bool TCPSocket::closeSocket()
{
    m_bIsConnect = false;
    if(isValid())
    {
        disconnectFromHost();
        close();
        return true;
    }
    return false;
}

bool TCPSocket::TCPWriteData(const char *data, int len)//端口2
{
    //qDebug()<<"h";
    if(!m_bIsConnect)
    {
        return false;
    }
    if( NULL == data || len <= 0 )
    {
        return false;
    }
    qint64 ret = 0;

    while(ret < len)
    {
        ret += writeData(data + ret, (len - ret));
        if(!waitForBytesWritten(3))
        {
            return false;
        }
    }
    recordTcpWriteData(data, len, m_TcpLogInfo_receive);
//qDebug()<<"f";
    return true;
}

bool   TCPSocket::TCPWriteData(QString  data)//端口2
{
    if(!m_bIsConnect)
    {
        return false;
    }
    if(data.size() <= 0 )
    {
        return false;
    }
    qint64 ret = writeData(data.toLatin1().data(), data.size());
    if( 0 >= ret )
    {
        emit disconnected();
        return false;
    }
    recordTcpWriteData(data, m_TcpLogInfo_receive);

    return true;
}

bool TCPSocket::TCPWriteMsgData(const char *data, int len)//端口1
{
    //qDebug()<<"h";
    if(!m_bIsConnect)
    {
        return false;
    }
    if( NULL == data || len <= 0 )
    {
        return false;
    }
    qint64 ret = 0;

    while(ret < len)
    {
        qint64 ss = writeData(data + ret, (len - ret));
        if(ss == -1)
        {
            emit disconnected();
            return false;
        }
        ret += ss;
        if(!waitForBytesWritten(3))
        {
            return false;
        }
    }
    recordTcpWriteData(data, len, m_TcpLogInfo_send);
//qDebug()<<"f";
    return true;
}

bool  TCPSocket::TCPReadData(QString&  recv)//端口2
{
    memset(m_datas, 0, TCP_BUFFER_SIZE);
    int len = readData(m_datas, TCP_BUFFER_SIZE);

    if(0 >= len)
    {
        return false;
    }

    recv.clear();
    recv.append(m_datas);

    recordTcpReadData(recv, m_TcpLogInfo_receive);
    return true;
}

bool  TCPSocket::TCPReadData(char *recv, int& size)//端口2
{
    //size = readData(recv, TCP_BUFFER_SIZE);
    size = read(recv, TCP_BUFFER_SIZE);
    if(size > 0)
    {
        //recordTcpReadData(recv, size, m_TcpLogInfo_receive);
        return true;
    }
    return false;
}

bool   TCPSocket::TCPReadMsgData(char *recv, int& size)//端口1
{
    //qDebug()<<"q";
    if(m_bIsConnect)
    {
        flush();
        if(waitForReadyRead(1000))
        {
            //qDebug()<<"t";
            size = readData(recv, TCP_BUFFER_SIZE);
            //qDebug()<<"q"<<size;
            if(size > 0)
            {
                recordTcpReadData(recv, size, m_TcpLogInfo_send);
                //qDebug()<<"w";
                return true;
            }
        }
    }
    return false;
}

bool   TCPSocket::isConnect()
{
    return m_bIsConnect;
}

void   TCPSocket::recordTcpWriteData(QString  data, LogInfo* tcpLogInfo)
{
    tcpLogInfo->writeToFile("Tcp_Send_Data : " + data);
    tcpLogInfo->newline();
}

void   TCPSocket::recordTcpReadData(QString  data, LogInfo* tcpLogInfo)
{
    tcpLogInfo->writeToFile("Tcp_Receive_Data : " + data);
    tcpLogInfo->newline();
}

void   TCPSocket::recordTcpWriteData(const char *data, int len, LogInfo* tcpLogInfo)
{
    tcpLogInfo->writeToFile("Tcp_Send_Data : ");
    tcpLogInfo->writeToFile(data, len);
    tcpLogInfo->newline();
}

void   TCPSocket::recordTcpReadData(const char *data, int len, LogInfo* tcpLogInfo)
{
    tcpLogInfo->writeToFile("Tcp_Receive_Data : ");
    tcpLogInfo->writeToFile(data, len);
    tcpLogInfo->newline();
}



