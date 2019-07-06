#ifndef _NETSOCKET_H
#define _NETSOCKET_H

#include <QUdpSocket>
#include <QObject>

#define UART0_REVBUFFER_SIZE		4096
class MotionControllerDevice;
/////////////////////////////////////////////////////////////////////////////
// CNetSocket command target

class CNetSocket : public QUdpSocket
{
    Q_OBJECT

public:
      
      qint64 m_nLength;        //消息长度
      char m_szBuffer[4096];    //消息缓冲区


// Operations
public:
    CNetSocket(MotionControllerDevice * pDlg, QString serverIP,quint16 netPort1,quint16 netPort2);
    CNetSocket(MotionControllerDevice * pDlg, QHostAddress serverIP,quint16 netPort1,quint16 netPort2);
	virtual ~CNetSocket();

// Overrides
public:
    MotionControllerDevice * cDlg;
	QHostAddress m_serverip;
	quint16 m_serverport;
	char Uart0_Buffer[UART0_REVBUFFER_SIZE];

	char* pUart0_rev;	
	char* pUart0_read;	

    int nRev_Uart0;
	volatile int ReadCount0;
	volatile int TimeCount0;
	
	void Uart_Send(int Uartnum, char *pt,int cnt);
    int OSReadUart(char *data, int num, int ntimeout);
	

	/*public:
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
	virtual void OnConnect(int nErrorCode);*/
	
signals:
	 
public slots:
		void slotReadyRead();

};



#endif 

