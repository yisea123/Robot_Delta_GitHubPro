#ifndef TCPCONNECT_H
#define TCPCONNECT_H

#include <QTcpSocket>
#include <QMutex>
#include "ComonTypes.h"

#define  TCP_BUFFER_SIZE   1024

class TCPSocket : public QTcpSocket
{
    Q_OBJECT

public:
    TCPSocket(QString ip, quint16 port, QObject *parent = NULL );
    ~TCPSocket();

    void   ConnectToServer(QString ip, quint16 port);
    bool   closeSocket();
    bool   TCPWriteData(const char *data, int len);
    bool   TCPWriteData(QString  data);
    bool   TCPWriteMsgData(const char *data, int len);
    //bool   TCPReadData(QString&  recv);
    //bool   TCPReadData(char *recv, int& size);
    bool   TCPReadMsgData(char *recv, int& size);
    bool   isConnect();

    void   recordTcpWriteData(QString  data, LogInfo* tcpLogInfo);
    void   recordTcpReadData(QString  data, LogInfo* tcpLogInfo);
    void   recordTcpWriteData(const char *data, int len, LogInfo* tcpLogInfo);
    void   recordTcpReadData(const char *data, int len, LogInfo* tcpLogInfo);

//public slots:
    bool   TCPReadData(char *recv, int& size);
    bool   TCPReadData(QString&  recv);

private:
    char    m_datas[TCP_BUFFER_SIZE];
    bool    m_bIsConnect;
    LogInfo* m_TcpLogInfo_send;
    LogInfo* m_TcpLogInfo_receive;
};

#endif // TCPCONNECT_H
