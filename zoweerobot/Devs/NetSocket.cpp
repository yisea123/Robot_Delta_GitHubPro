#include <QtNetwork>
#include "NetSocket.h"
#include "NetCtrl.h"

#include "MotionControllerDevice.h"


/////////////////////////////////////////////////////////////////////////////
// CNetSocket

CNetSocket::CNetSocket(MotionControllerDevice * pDlg, QString serverIP,quint16 netPort1,quint16 netPort2)
{

    m_serverip.setAddress(serverIP);
	m_serverport=netPort1;
      cDlg=pDlg;
	m_nLength=0;
	memset(m_szBuffer,0,sizeof(m_szBuffer));

	pUart0_rev=&Uart0_Buffer[0],	//Uart0 ?óê??o3???????
	pUart0_read=&Uart0_Buffer[0];	
	nRev_Uart0=1;
	ReadCount0=0;
	bind(netPort2);
    connect(this, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));   //接收准备

}

CNetSocket::~CNetSocket()
{

}


// Do not edit the following lines, which are needed by ClassWizard.


/////////////////////////////////////////////////////////////////////////////
// CNetSocket member functions

/*void CNetSocket::OnClose(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CAsyncSocket::OnClose(nErrorCode);
}

void CNetSocket::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	//m_nLength=Receive(m_szBuffer,sizeof(m_szBuffer),0);
	int i;
	int nrev;
	m_nLength=ReceiveFrom(m_szBuffer,4096,cDlg->m_serverip,cDlg->m_serverport,0);
	 for(i=0;i<m_nLength;i++)
		        {
				  //memcpy(pUart0_rev, buffer, len);
				  *pUart0_rev=m_szBuffer[i];
				  pUart0_rev++;
			     	  if(pUart0_rev==Uart0_Buffer+UART0_REVBUFFER_SIZE)
				          pUart0_rev=Uart0_Buffer;

				  nrev=pUart0_rev-pUart0_read;
				  if(nrev<0)//?-?·?o3???μ÷??
				          nrev+=UART0_REVBUFFER_SIZE;


				  ReadCount0=nrev;
				  
				  if(nrev>(9*UART0_REVBUFFER_SIZE/10))
				  {
				          cDlg->m_netctrl->gb_ctrl.sendtrafficctrl  |=0x1;
				  }
				  else if(nrev<(6*UART0_REVBUFFER_SIZE/10))
				  {
				          cDlg->m_netctrl->gb_ctrl.sendtrafficctrl  &=~0x1;
				  }
		           }
	
	memset(m_szBuffer,0,sizeof(m_szBuffer));
	m_nLength=0;
	CAsyncSocket::OnReceive(nErrorCode);
}

void CNetSocket::OnSend(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	//Send(m_szBuffer,m_nLength,0);
	//m_nLength=0;
	//memset(m_szBuffer,0,sizeof(m_szBuffer));
	//继续提请一个"读"的网络事件，接收Server消息
	AsyncSelect(FD_READ);

	CAsyncSocket::OnSend(nErrorCode);
}

void CNetSocket::OnConnect(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (nErrorCode==0)
	{
	      m_bConnected=TRUE;
	      CNetCommuciateApp* pApp=(CNetCommuciateApp*)AfxGetApp();
	      //CNetCommuciateDlg* pDlg=(CNetCommuciateDlg*)pApp- >m_pMainWnd;
	      //memcpy(m_szBuffer,"Connected to ",13);
	      //strncat(m_szBuffer,pDlg- >m_szServerAdr,
	      //    sizeof(pDlg- >m_szServerAdr));
	      //pDlg- >m_MSGS.InsertString(0,m_szBuffer);
	      AsyncSelect(FD_READ);    ////提请一个"读"的网络事件，准备接收
	}

	CAsyncSocket::OnConnect(nErrorCode);
}

*/

int CNetSocket::OSReadUart(char *data, int num, int ntimeout)
{
	int i=0;
	int nrev=0;
	
    
	nRev_Uart0=num;	//éè??Uart0?óê?êy?Yêy

	nrev=pUart0_rev-pUart0_read;
	if(nrev<0)//?-?·?o3???μ÷??
	{
	   
		nrev+=UART0_REVBUFFER_SIZE;
        }
    //nrev=ReadCount0;
	if(nrev<num)
	{//è?1??o3????ú′?2?μ??ùDèòaμ?×??úêy
	  // nrev=pUart0_rev-pUart0_read;//?′?á3?μ?×??ú??êy
	  // if(nrev<0)//?-?·?o3???μ÷??
		//    nrev+=UART0_REVBUFFER_SIZE;
	   ReadCount0=nrev;
	   TimeCount0=0;
	   if(ntimeout==0) 
	   {  
	      while((nrev=ReadCount0)<num);
	   }
	    
	   else
	   {
	 
	      while((nrev=ReadCount0)<num)
	      {
             if(TimeCount0++>=ntimeout)
	         {
	             nrev=pUart0_rev-pUart0_read;
	             if(nrev<0)//?-?·?o3???μ÷??
		            nrev+=UART0_REVBUFFER_SIZE;
	             num=(num>nrev)?nrev:num;
	             break;
	             //return 0;
	         }
	      }
	   }     
	}
	
  
	for(i=0;i<num;i++)
	{
		data[i]=*pUart0_read;
		pUart0_read++;
		if(pUart0_read==Uart0_Buffer+UART0_REVBUFFER_SIZE)
			pUart0_read=Uart0_Buffer;
		
	}
	return num;
}


void CNetSocket::Uart_Send(int Uartnum, char *pt,int cnt)
{
	//SendTo(pt,cnt,cDlg->m_serverport,cDlg->m_serverip,0);
	//QByteArray datagram;
    writeDatagram(pt,cnt, m_serverip, m_serverport);
}




void CNetSocket::slotReadyRead() 
{

	qint64 i;
	qint64 nrev;
	
	QByteArray datagram;
	do{
        i=pendingDatagramSize();
        //readDatagram(datagram.data(), datagram.size());  //接收数据
        m_nLength=readDatagram(m_szBuffer,i,&m_serverip,&m_serverport);
        if(m_nLength!=-1)
        {
               if( cDlg->m_netctrl->ParseNetFrame((U8*)m_szBuffer,&cDlg->m_netctrl->gb_frame,& cDlg->m_netctrl->gb_ctrl))
		   {
		   	   cDlg->m_netctrl->DealNetFrame( cDlg->m_netctrl->gb_frame,& cDlg->m_netctrl->gb_ctrl);
		   }
         
        }
	}while(hasPendingDatagrams() );

	memset(m_szBuffer,0,sizeof(m_szBuffer));
	m_nLength=0;
}


/*void CNetSocket::slotReadyRead() 
{

	qint64 i;
	qint64 nrev;
	
	QByteArray datagram;
	do{
        i=pendingDatagramSize();
        //readDatagram(datagram.data(), datagram.size());  //接收数据
        m_nLength=readDatagram(m_szBuffer,i,&m_serverip,&m_serverport);
        if(m_nLength!=-1)
        {
          for(i=0;i<m_nLength;i++)
            {
                   //memcpy(pUart0_rev, buffer, len);
                   *pUart0_rev=m_szBuffer[i];
                   pUart0_rev++;
                       if(pUart0_rev==Uart0_Buffer+UART0_REVBUFFER_SIZE)
                           pUart0_rev=Uart0_Buffer;

                   nrev=pUart0_rev-pUart0_read;
                   if(nrev<0)//?-?·?o3???μ÷??
                           nrev+=UART0_REVBUFFER_SIZE;


                   ReadCount0=nrev;

                   if(nrev>(9*UART0_REVBUFFER_SIZE/10))
                   {
                           cDlg->m_netctrl->gb_ctrl.sendtrafficctrl  |=0x1;
                   }
                   else if(nrev<(6*UART0_REVBUFFER_SIZE/10))
                   {
                           cDlg->m_netctrl->gb_ctrl.sendtrafficctrl  &=~0x1;
                   }
             }
        }
	}while(hasPendingDatagrams() );

	memset(m_szBuffer,0,sizeof(m_szBuffer));
	m_nLength=0;
}*/


