/************************************************************
  Copyright (C),
  �ļ�����ParseGCode.cpp
  ���ߣ�zhou    �汾��V1.0    �������ڣ�2018/07/10
  ģ��������ParseGCode���ʵ��
  ��ʷ�޸ļ�¼��<author>    <time>    <version >    <desc>
                  zhou    2018/07/10     V1.0      ���ע��
*************************************************************/

#include "MotionControllerDevice.h"
#include "BaseDevice.h"
#include "SystemProcessInfo.h"
#include "MotionControllerModule.h"
#include "SystemSchedule.h"
//#include "ccommunicationsettingui.h"
#include "NetCtrl.h"
#include "NetSocket.h"


union INTCHAR
{
    unsigned char c[4];
    int data;
};

union SHORTCHAR
{
    unsigned char c[2];
    unsigned short data;
};

union FLOATCHAR
{
    unsigned char c[4];
    float data;
};

/*************************************************
  �������ƣ�MotionControllerDevice(QString s, BaseModule* m, SystemParameter* para)
  �������ܣ����캯��
  ��������� s���豸����para��ϵͳ����
  ���������
  �� �� ֵ��
  ��    ע��
**************************************************/
MotionControllerDevice::MotionControllerDevice(QString s, BaseModule* m, SystemParameter* para)
{
    m_sDeviceName = s;
    m_pModuleHandle = m;

    initMotionControllerDevice();

    m_pSystemParm = para;

    m_PCRecvCommandSocket = NULL;
    m_PCSendCommandSocket = NULL;

    m_bisNetConnect = false;
    m_userfile=nullptr;
}

MotionControllerDevice::~MotionControllerDevice()
{
}


/*************************************************
  �������ƣ�initMotionControllerDevice()
  �������ܣ���ʼ���˶����ƹؼ�����
  ���������
  ���������
  �� �� ֵ��
  ��    ע��ϵͳ��λʱ��Ҫ����
**************************************************/
void MotionControllerDevice::initMotionControllerDevice()
{
    m_sysState = SYS_STETE_NORMAL;
    m_axisAlarm = NAxis(0, 0, 0, 0, 0, 0);
    m_axisLimit = NAxis(0, 0, 0, 0, 0, 0);

    m_nFIFOSpace = 50;

    m_nMotionCount = 0;
    m_currentRunActionIndex = 0;
    m_currentSendActionIndex = 0;
    m_bisFinshOneAction = false;

    m_nCurrentPosition = NAxis(0, 0, 0, 0, 0 , 0);
    m_currentPos = DAxis(0, 0, 0, 0, 0, 0);

    m_bIsPunchLock = false;
}

/*************************************************
  �������ƣ�openDevice(ErrorInfo& e)
  �������ܣ����豸
  ���������
  ���������e  ������Ϣ
  �� �� ֵ��int δ����
  ��    ע��
**************************************************/
int MotionControllerDevice::openDevice(ErrorInfo& e)
{
    initDevice(e);
    return  EmulationControllerDevice::openDevice(e);
}

/*************************************************
  �������ƣ�initDevice(ErrorInfo& e)
  �������ܣ���ʼ���豸
  ���������
  ���������
  �� �� ֵ��int δ����
  ��    ע��
**************************************************/
int MotionControllerDevice::initDevice(ErrorInfo& e)
{
    return  0;
}

/*************************************************
  �������ƣ�startDevice(ErrorInfo& e)
  �������ܣ������豸
  ���������
  ���������
  �� �� ֵ��int δ����
  ��    ע��
**************************************************/
int MotionControllerDevice::startDevice(ErrorInfo& e)
{
    return  EmulationControllerDevice::startDevice(e);
}
    

/*************************************************
  �������ƣ�pauseDevice(ErrorInfo& e)
  �������ܣ���ͣ�豸
  ���������
  ���������
  �� �� ֵ��int δ����
  ��    ע��
**************************************************/
int MotionControllerDevice::pauseDevice(ErrorInfo& e)
{
    return  EmulationControllerDevice::pauseDevice(e);
}

/*************************************************
  �������ƣ�stopDevice(ErrorInfo& e)
  �������ܣ�ֹͣ�豸
  ���������
  ���������
  �� �� ֵ��int δ����
  ��    ע��
**************************************************/
int MotionControllerDevice::stopDevice(ErrorInfo& e)
{
    return  EmulationControllerDevice::stopDevice(e);
}

/*************************************************
  �������ƣ�reConnectNet()
  �������ܣ�������������
  ���������
  ���������
  �� �� ֵ��
  ��    ע��
**************************************************/
bool MotionControllerDevice::reConnectNet()
{
    if(creatTCPSocket() == 0)
    {
        //m_pModuleHandle->getSystemScheduleHandle()->recvRunInfoFromAllModules("�����ɹ�");
        //m_pModuleHandle->getSystemScheduleHandle()->recvMsgFromWindows(MOTION_CONTROLLER_ID, "system reset");
        return true;
    }
    //m_pModuleHandle->getSystemScheduleHandle()->recvRunInfoFromAllModules("�������ɹ�");
    return false;
}

bool MotionControllerDevice::closeNet()
{
    if (!m_PCSendCommandSocket->closeSocket())
        return false;
    if (!m_PCRecvCommandSocket->closeSocket())
        return false;
    return true;
}

/*************************************************
  �������ƣ�restartDevice(ErrorInfo& e)
  �������ܣ������豸
  ���������
  ���������
  �� �� ֵ��
  ��    ע��
**************************************************/
int MotionControllerDevice::restartDevice(ErrorInfo& e)
{
   return 0;
}

/*************************************************
  �������ƣ�setMachiningStateUi(CAutoMachiningStateUi* ui)
  �������ܣ���������UI
  ���������ui���Զ��ӹ�ҳ���Ӧ��UI
  ���������
  �� �� ֵ��
  ��    ע��
**************************************************/
void MotionControllerDevice::setMachiningStateUi(CAutoMachiningStateUi* ui)
{
    m_pMachiningUI = ui;
}

/*************************************************
  �������ƣ�setMachiningStateUi(CAutoMachiningStateUi* ui)
  �������ܣ���������UI
  ���������ui���ֶ��ӹ�ҳ���Ӧ��UI
  ���������
  �� �� ֵ��
  ��    ע��
**************************************************/
void MotionControllerDevice::setManualMachiningStateUi(CManualMachiningStateUi* ui)
{
    m_pManualMachiningUI = ui;
}

/*************************************************
  �������ƣ�setMachiningStateUi(CAutoMachiningStateUi* ui)
  �������ܣ���������UI
  ���������ui���ⲿͨѶ����ҳ���Ӧ��UI
  ���������
  �� �� ֵ��
  ��    ע��
**************************************************/
void MotionControllerDevice::setCommunicationSettingUI(CCommunicationSettingUi* ui)
{
    m_pCommunicationSettingUI = ui;
}

/*************************************************
  �������ƣ�creatTCPSocket()
  �������ܣ��������˶�����������������
  ���������
  ���������
  �� �� ֵ��0���ɹ���-1��ʧ��
  ��    ע��
**************************************************/
int MotionControllerDevice::creatTCPSocket()
{
    SystemParameter* parmtemp = ((MotionControllerModule* )m_pModuleHandle)->getSystemParameterHandle();
    QString serverIP = "192.168.3.10";//parmtemp->m_netConnectionParm.m_netIP;
    quint16 netPort1 = 62000;//parmtemp->m_netConnectionParm.m_netPort1;
    quint16 netPort2 = 60000;//parmtemp->m_netConnectionParm.m_netPort2;

    //qDebug()<<"ip="<<serverIP<<" | netPort1="<<netPort1<<" | netPort2="<<netPort2;

    //m_PCRecvCommandSocket = new TCPSocket(CONTROLLER_SERVER_IP, PC_RECV_COMMAND_PORT);
    m_PCRecvCommandSocket = new TCPSocket(serverIP, netPort2);

    if(m_PCRecvCommandSocket->isConnect())
    {
        connect(m_PCRecvCommandSocket,SIGNAL(readyRead()), this, SLOT(tcpMonitorData()));
        connect(m_PCRecvCommandSocket, SIGNAL(disconnected()), this, SLOT(OnRecvSocketDisconnected()));
        connect(m_PCRecvCommandSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(OnRecvSocketError(QAbstractSocket::SocketError)));

       /* m_PCSendCommandSocket = new TCPSocket(serverIP, netPort1);
        if(m_PCSendCommandSocket->isConnect())
        {
          //  m_pCommunicationSettingUI->ctrlNetConnectLed(true);
            emit NetLedCtrl(true);
            connect(m_PCSendCommandSocket, SIGNAL(disconnected()), this, SLOT(OnSentSocketDisconnected()));
            connect(m_PCSendCommandSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(OnSentSocketError(QAbstractSocket::SocketError)));
            m_bisNetConnect = true;
            return 0;
        }
        emit NetLedCtrl(false);
        return -1;*/
        return 0;
    }
    else
    {
     //   m_pCommunicationSettingUI->ctrlNetConnectLed(false);
        emit NetLedCtrl(false);
    }
    emit NetLedCtrl(false);
    return -1;
}


/*************************************************
  �������ƣ�creatUdPSocket()
  �������ܣ��������˶�����������������
  ���������
  ���������
  �� �� ֵ��0���ɹ���-1��ʧ��
  ��    ע��
**************************************************/
int MotionControllerDevice::creatUdPSocket()
{
    /*SystemParameter* parmtemp = ((MotionControllerModule* )m_pModuleHandle)->getSystemParameterHandle();
    QString serverIP = parmtemp->m_netConnectionParm.m_netIP;
    quint16 netPort1 = parmtemp->m_netConnectionParm.m_netPort1;
    quint16 netPort2 = parmtemp->m_netConnectionParm.m_netPort2;*/

    //qDebug()<<"ip="<<serverIP<<" | netPort1="<<netPort1<<" | netPort2="<<netPort2;

    //m_PCRecvCommandSocket = new TCPSocket(serverIP, netPort2);

   QString serverIP = "192.168.3.124";//m_pSystemParm->m_netConnectionParm.m_netIP;
     quint16 netPort1 = 52000;//m_pSystemParm->m_netConnectionParm.m_netPort1;
    quint16 netPort2 = 50000;//m_pSystemParm->m_netConnectionParm.m_netPort2;
	
    m_socket = new CNetSocket(this,serverIP,netPort1,netPort2);
    m_netctrl= new CNetCtrl(this);
	
    m_NetCtrlTimer = new QTimer(this);
    connect(m_NetCtrlTimer, SIGNAL(timeout()), this, SLOT(onNetCtrlTimeout()), Qt::DirectConnection);
    m_NetCtrlTimer->start(5);//20
	
    //emit NetLedCtrl(true);
    return 0;
}
int MotionControllerDevice::SendNetReqByTCP(int num)
{
      if(m_PCRecvCommandSocket->isConnect())
      	{
      		m_PCRecvCommandSocket->write((char *)&num,4);
      	}
}
int MotionControllerDevice::RecvNetDataByTCP(double *pos)
{
     if(m_PCRecvCommandSocket->isConnect())
     {
     	   if(ifgetnetvalue)
     	   {
     	   	memcpy((char *)pos,(char *)scarapos,32);
     	   	ifgetnetvalue=0;
     	         return 1;
     	   }
	   else
	   {
	   	 return 0;
	   }
     }
     else 
	 	return 1;

}

int MotionControllerDevice::SendPTPByUDP()
{
    return 0;
}

/*************************************************
  �������ƣ�initRequestFrame()
  �������ܣ���ʼ������֡
  ���������
  ���������
  �� �� ֵ��
  ��    ע�������ڶ˿�1
**************************************************/
void MotionControllerDevice::initRequestFrame()//�˿�1
{
    m_dataRequest.clear();

    /* ����֡֡ͷ 2�ֽ� */
    SHORTCHAR requestFrame;
    requestFrame.data = 0xA050;
    if (ENDIANNESS == BIG_ENDIAN)
    {
        m_dataRequest.append(requestFrame.c[0]);
        m_dataRequest.append(requestFrame.c[1]);
    }
    else
    {
        m_dataRequest.append(requestFrame.c[1]);
        m_dataRequest.append(requestFrame.c[0]);
    }

    /* ������ 1�ֽ� */
    m_dataRequest.append(0);

    /* 4�����ܲ��� ��4�ֽ� */
    m_dataRequest.append(0);
    m_dataRequest.append(0);
    m_dataRequest.append(0);
    m_dataRequest.append(0);
}

/*************************************************
  �������ƣ�initRespondFrame_monitor()
  �������ܣ���ʼ����Ӧ֡
  ���������
  ���������
  �� �� ֵ��
  ��    ע�������ڶ˿�2
**************************************************/
void MotionControllerDevice::initRespondFrame_monitor()//�˿�2
{
    m_dataRequest_monitor.clear();

    /* ��Ӧ֡֡ͷ 2�ֽ� */
    SHORTCHAR requestFrame;
    requestFrame.data = 0xA051;
    if (ENDIANNESS == BIG_ENDIAN)
    {
        m_dataRequest_monitor.append(requestFrame.c[0]);
        m_dataRequest_monitor.append(requestFrame.c[1]);
    }
    else
    {
        m_dataRequest_monitor.append(requestFrame.c[1]);
        m_dataRequest_monitor.append(requestFrame.c[0]);
    }

    /* ������ 1�ֽ� */
    m_dataRequest_monitor.append(0);

    /* 4�����ܲ��� ��4�ֽ�
    m_dataRequest.append(0);
    m_dataRequest.append(0);
    m_dataRequest.append(0);
    m_dataRequest.append(0);*/
}

bool MotionControllerDevice::isPointInClampProtectArea(double x, double y, ProcessInterVariableCache* cache)
{
    for (int i = 0; i < cache->m_clampPos.size(); i++)
    {
        if (x > (cache->m_clampPos.at(i).x - m_pSystemParm->m_machineParm.m_dGripper1OffsetX) && x < (cache->m_clampPos.at(i).x + m_pSystemParm->m_machineParm.m_dGripper1OffsetX))
        {
            if (y > cache->m_clampPos.at(i).y && y < (cache->m_clampPos.at(i).y + m_pSystemParm->m_machineParm.m_dGripper1OffsetY))
                return true;
        }
    }
    return false;
}

/*************************************************
  �������ƣ�fillIntParmToRequestFrame(QVector<unsigned char> &dataRequest, int intParm)
  �������ܣ����int���ݵ���Ӧ֡
  ���������dataRequest����Ӧ֡�� intParm����������
  ���������
  �� �� ֵ��
  ��    ע��
**************************************************/
void MotionControllerDevice::fillIntParmToRequestFrame(QVector<unsigned char> &dataRequest, int intParm)
{
    INTCHAR n;
    n.data = intParm;

    if (ENDIANNESS == BIG_ENDIAN)
    {
        dataRequest.append(n.c[0]);
        dataRequest.append(n.c[1]);
        dataRequest.append(n.c[2]);
        dataRequest.append(n.c[3]);
    }
    else
    {
        dataRequest.append(n.c[3]);
        dataRequest.append(n.c[2]);
        dataRequest.append(n.c[1]);
        dataRequest.append(n.c[0]);
    }
}

/*************************************************
  �������ƣ�fillShortParmToRequestFrame(QVector<unsigned char> &dataRequest, short shortParm)
  �������ܣ����short���ݵ���Ӧ֡
  ���������dataRequest����Ӧ֡�� intParm����������
  ���������
  �� �� ֵ��
  ��    ע��
**************************************************/
void MotionControllerDevice::fillShortParmToRequestFrame(QVector<unsigned char> &dataRequest, short shortParm)
{
    INTCHAR n;
    n.data = shortParm;

    if (ENDIANNESS == BIG_ENDIAN)
    {
        dataRequest.append(n.c[0]);
        dataRequest.append(n.c[1]);
    }
    else
    {
        dataRequest.append(n.c[1]);
        dataRequest.append(n.c[0]);
    }
}

/*************************************************
  �������ƣ�charToInt(unsigned char c0, unsigned char c1, unsigned char c2, unsigned char c3)
  �������ܣ���4��char�ַ�ת��Ϊһ��int����
  ���������c0��c1��c2��c3��4��˳���ŵ��ַ�
  ���������ת����ı���
  �� �� ֵ��
  ��    ע��
**************************************************/
int MotionControllerDevice::charToInt(unsigned char c0, unsigned char c1, unsigned char c2, unsigned char c3)
{
    INTCHAR n;
    if (ENDIANNESS == BIG_ENDIAN)
    {
        n.c[0] = c0;
        n.c[1] = c1;
        n.c[2] = c2;
        n.c[3] = c3;
    }
    else
    {
        n.c[3] = c0;
        n.c[2] = c1;
        n.c[1] = c2;
        n.c[0] = c3;
    }
    return n.data;
}

/*************************************************
  �������ƣ�charToShort(unsigned char c0, unsigned char c1)
  �������ܣ���2��char�ַ�ת��Ϊһ��short����
  ���������c0��c1��2��˳���ŵ��ַ�
  ���������ת����ı���
  �� �� ֵ��
  ��    ע��
**************************************************/
short MotionControllerDevice::charToShort(unsigned char c0, unsigned char c1)
{
    SHORTCHAR n;
    if (ENDIANNESS == BIG_ENDIAN)
    {
        n.c[0] = c0;
        n.c[1] = c1;
    }
    else
    {
        n.c[1] = c0;
        n.c[0] = c1;
    }
    return n.data;
}

/*************************************************
  �������ƣ�fillOneFIFO(int* axisData, ErrorInfo& e)
  �������ܣ����1���λ�˶����ݵ�FIFO
  ���������axisData��5���������˶�������ʼ��ַ��
  ���������
  �� �� ֵ��-1������ 0��ʧ�ܣ�FIFO������ 1���ɹ����
  ��    ע�������ڶ˿�1
**************************************************/
int MotionControllerDevice::fillOneFIFO(int* axisData, ErrorInfo& e)//�˿�1
{
InputAutoData autodata;
    memset(&autodata, 0, sizeof(autodata));
    autodata.decode_id =0;// data->m_nCodeId;
    autodata.Prog_SequNum = 0;
    autodata.Prog_LineNum = 0;
    autodata.Prog_Mark =0;// isEnd ? 1 : 0;
    autodata.Mode = 1;
    autodata.Interp_Mode = 1;
    autodata.Joint_VelPct = 100;
    autodata.JPos_End[0] = (float)axisData[0];
    autodata.JPos_End[1] = (float)axisData[1];
    //autodata.Car_PVel = 200;

    if (PutAutoDataFrame(&autodata) == 1)
    {
        printFifoData(&autodata);
        return true;
    }
    else
        return false;
}

int MotionControllerDevice::fillOneFIFO(int movetype,double* axisData, ErrorInfo& e)//�˿�1
{
    InputAutoData autodata;
    memset(&autodata, 0, sizeof(autodata));
    autodata.decode_id =0;// data->m_nCodeId;
    autodata.Prog_SequNum = 0;
    autodata.Prog_LineNum = 0;
    autodata.Prog_Mark =0;// isEnd ? 1 : 0;
    autodata.Mode = 1;
    autodata.Interp_Mode = (movetype<3)?(movetype+1):1;
    autodata.Joint_VelPct = (int)m_pSystemParm->sysvel;//100;
    autodata.Car_PVel =  (double)m_pSystemParm->sysvel;
    autodata.Car_GVel =  (double)m_pSystemParm->sysvel;
    if(movetype==0)
    {
    	autodata.JPos_End[0] = (float)axisData[0];
    	autodata.JPos_End[1] = (float)axisData[1];
	autodata.JPos_End[2] = (float)axisData[2];
    	autodata.JPos_End[3] = (float)axisData[3];
	autodata.JPos_End[4] = (float)axisData[4];
    	autodata.JPos_End[5] = (float)axisData[5];
	qDebug("x =%g, y=%g, z=%g, r=%g\n", axisData[0], axisData[1], axisData[2], axisData[3]);
    }
    else if(movetype==1)
    {
    	autodata.JPos_End[0] = (float)axisData[0];
    	autodata.JPos_End[1] = (float)axisData[1];
	autodata.JPos_End[2] = (float)axisData[2];
    	autodata.JPos_End[3] = (float)axisData[3];
	qDebug("x =%g, y=%g, z=%g, r=%g\n",axisData[0], axisData[1], axisData[2], axisData[3]);
    }
    else if(movetype==2)
    {
    	autodata.JPos_End[0] = (float)axisData[0];
    	autodata.JPos_End[1] = (float)axisData[1];
	autodata.JPos_End[2] = (float)axisData[2];
    	autodata.JPos_End[3] = (float)axisData[3];
	return true;
    }
    //autodata.Car_PVel = 200;

    if (PutAutoDataFrame(&autodata) == 1)
    {
        printFifoData(&autodata);
        return true;
    }
    else
        return false;
}

int MotionControllerDevice::fillOneFIFO(MoveAction * action,double* axisData, ErrorInfo& e)
{
    InputAutoData autodata;
    memset(&autodata, 0, sizeof(autodata));
    autodata.decode_id =action->m_nLineSerialNum;//0;// data->m_nCodeId;
    //autodata.Prog_SequNum = 0;
    autodata.Prog_SequNum=action->m_toolSerialNum;//���ߺ�//m_dT
    autodata.Prog_LineNum = action->m_hand;//0;//scara hand
    autodata.Prog_Mark =0;// isEnd ? 1 : 0;
    autodata.Mode = 1;
    autodata.Interp_Mode = (action->m_moveType<3)?(action->m_moveType+1):1;
    autodata.Joint_VelPct = 100;//(int)m_pSystemParm->sysvel;//100;
    autodata.Car_PVel =(double)action->m_dAngle;//  (double)m_pSystemParm->sysvel;//m_dF
    autodata.Car_GVel =(double)m_pSystemParm->SystemParam[pCarMaxGVel];
   
    if(action->m_moveType==0)
    {
    	autodata.JPos_End[0] = (float)axisData[0];
    	autodata.JPos_End[1] = (float)axisData[1];
	autodata.JPos_End[2] = (float)axisData[2];
    	autodata.JPos_End[3] = (float)axisData[3];
	autodata.JPos_End[4] = (float)axisData[4];
    	autodata.JPos_End[5] = (float)axisData[5];
    }
    else if(action->m_moveType==1)
    {
    	autodata.JPos_End[0] = (float)axisData[0];
    	autodata.JPos_End[1] = (float)axisData[1];
	autodata.JPos_End[2] = (float)axisData[2];
    	autodata.JPos_End[3] = (float)axisData[3];
	autodata.JPos_End[4] = (float)axisData[4];
    	autodata.JPos_End[5] = (float)axisData[5];
    }
    else if(action->m_moveType==2)
    {
    	autodata.JPos_End[0] = (float)axisData[0];
    	autodata.JPos_End[1] = (float)axisData[1];
	autodata.JPos_End[2] = (float)axisData[2];
    	autodata.JPos_End[3] = (float)axisData[3];
	autodata.JPos_End[4] = (float)axisData[4];
    	autodata.JPos_End[5] = (float)axisData[5];
    }
    //autodata.Car_PVel = 200;

    if (PutAutoDataFrame(&autodata) == 1)
    {
        printFifoData(&autodata);
        return true;
    }
    else
        return false;
}

int MotionControllerDevice::fillOneFIFO(IOAction * action,ErrorInfo& e)
{
    InputAutoData autodata;
    memset(&autodata, 0, sizeof(autodata));
    autodata.decode_id =action->m_nLineSerialNum;//0;// data->m_nCodeId;
    //autodata.Prog_SequNum = 0;
    autodata.Prog_SequNum=0;//action->m_tool->m_toolSerialNum;//���ߺ�
    autodata.Prog_LineNum = 0;
    autodata.Prog_Mark =0;// isEnd ? 1 : 0;
    autodata.Mode = 2;
    autodata.Interp_Mode = 0;
    autodata.Joint_VelPct = 0;//(int)m_pSystemParm->sysvel;//100;
    autodata.Car_PVel =0;//  (double)m_pSystemParm->sysvel;//m_dF
    
    if(action->m_nIoType==IO_OUT)
    {
    	autodata.IO_Mode = 2;
    	autodata.IO_Name= action->m_nIONum;
	autodata.IO_Value= action->m_bIoSts;
    	
    }
    else if(action->m_nIoType==IO_IN)
    {
    	autodata.Mode = 5;
    	autodata.IO_Mode = 1;
    	autodata.IO_Name= action->m_nCheckIONum;
	autodata.IO_Value= action->m_bCheckIoSts;
	autodata.WaitTime= action->waitTime;
    }
    

    if (PutAutoDataFrame(&autodata) == 1)
    {
        printFifoData(&autodata);
        return true;
    }
    else
        return false;
}
int MotionControllerDevice::fillOneFIFO(DelayAction * action,ErrorInfo& e)
{
    InputAutoData autodata;
    memset(&autodata, 0, sizeof(autodata));
    autodata.decode_id =action->m_nLineSerialNum;//0;// data->m_nCodeId;
    //autodata.Prog_SequNum = 0;
    autodata.Prog_SequNum=0;//action->m_tool->m_toolSerialNum;//���ߺ�
    autodata.Prog_LineNum = 0;
    autodata.Prog_Mark =0;// isEnd ? 1 : 0;
    autodata.Mode = 3;
    autodata.Interp_Mode = 0;
    autodata.Joint_VelPct = 0;//(int)m_pSystemParm->sysvel;//100;
    autodata.Car_PVel =0;//  (double)m_pSystemParm->sysvel;//m_dF
    
   
	autodata.IO_Mode = 0;
	autodata.IO_Name=0;
	autodata.IO_Name= 0;
	autodata.WaitTime= (action->m_nDelayTime)?(action->m_nDelayTime):1;
    

    if (PutAutoDataFrame(&autodata) == 1)
    {
        printFifoData(&autodata);
        return true;
    }
    else
        return false;
}

int MotionControllerDevice::backHome()
{
    InputAutoData autodata;
    memset(&autodata, 0, sizeof(autodata));
    autodata.decode_id =0;//0;// data->m_nCodeId;
    //autodata.Prog_SequNum = 0;
    autodata.Prog_SequNum=0;//���ߺ�//m_dT
    autodata.Prog_LineNum = 0;
    autodata.Prog_Mark =0;// isEnd ? 1 : 0;
    autodata.Mode = 1;//mov
    autodata.Interp_Mode = 8;//MZX
    autodata.Joint_VelPct = -450;//(int)m_pSystemParm->sysvel;//100;
    autodata.PathSN=0x10;//use joint to End;
    autodata.Car_PVel =(m_pSystemParm->SystemParam[pCarMaxPVel]>6000)?6000:(m_pSystemParm->SystemParam[pCarMaxPVel]);//  (double)m_pSystemParm->sysvel;//m_dF
    autodata.Car_GVel =(m_pSystemParm->SystemParam[pCarMaxGVel]>400)?400:(m_pSystemParm->SystemParam[pCarMaxGVel]);
    autodata.WaitTime = 3000;
 
       autodata.JPos_End[0] = 0;
    	autodata.JPos_End[1] = 0;
	autodata.JPos_End[2] = 0;
    	autodata.JPos_End[3] = 0;
       autodata.JPos_End[4] = 0;
    	autodata.JPos_End[5] = 0;
 

    if (PutAutoDataFrame(&autodata) == 1)
    {
        printFifoData(&autodata);
        return true;
    }
    else
        return false;
}

int MotionControllerDevice::movetoteachpoint(int num)
{
     InputAutoData autodata;
     TeachPoint pointdata=m_pSystemParm->teachpoint[num];
    memset(&autodata, 0, sizeof(autodata));
    
   autodata.decode_id =0;//0;// data->m_nCodeId;
    //autodata.Prog_SequNum = 0;
    autodata.Prog_SequNum=0;//���ߺ�//m_dT
    autodata.Prog_LineNum = 0;
    autodata.Prog_Mark =0;// isEnd ? 1 : 0;
    autodata.Mode = 1;//mov
    autodata.Interp_Mode = 8;//MZX
    autodata.Joint_VelPct = -450;//(int)m_pSystemParm->sysvel;//100;
    autodata.PathSN=0x0;//use Car to End;
    autodata.Car_PVel =(m_pSystemParm->SystemParam[pCarMaxPVel]>6000)?6000:(m_pSystemParm->SystemParam[pCarMaxPVel]);//  (double)m_pSystemParm->sysvel;//m_dF
    autodata.Car_GVel =(m_pSystemParm->SystemParam[pCarMaxGVel]>400)?400:(m_pSystemParm->SystemParam[pCarMaxGVel]);
    autodata.WaitTime = 3000;
   
    	autodata.JPos_End[0] = pointdata.pos[0];
    	autodata.JPos_End[1] = pointdata.pos[1];
	autodata.JPos_End[2] = pointdata.pos[2];
    	autodata.JPos_End[3] = pointdata.pos[3];
       autodata.JPos_End[4] = pointdata.pos[4];
    	autodata.JPos_End[5] = pointdata.pos[5];
    
 
 

    if (PutAutoDataFrame(&autodata) == 1)
    {
        printFifoData(&autodata);
        return true;
    }
    else
        return false;
}



/*{
    int groupNum = 0;

    initRequestFrame();
    m_dataRequest[FUNCTION_CODE] = MSG_FUNCTION_ADD_FIFO;
    m_dataRequest[FUNCTION_PARM_1] = 1;

    for (int i = 0; i < 5; i++)
    {
        fillIntParmToRequestFrame(m_dataRequest, axisData[i]);
    }

    //����m_dataRequest; ����m_dataResponse
    if (tcpSentAndReceive(e))
    {
        if (m_nRecvMsgControllerDataNum == 7)
        {
            
            if (1 == m_dataResponse[FUNCTION_CODE])
            {
               
                m_nFIFOSpace = charToShort(m_dataResponse[FUNCTION_PARM_1], m_dataResponse[FUNCTION_PARM_2]);

                
                groupNum = charToShort(m_dataResponse[FUNCTION_PARM_3], m_dataResponse[FUNCTION_PARM_4]);
                if (1 != groupNum)
                {
                    //e.insertDebug(DEBUG_NET_PARM_ERROR, QString("FIFO����!, fifo����=%1,ʣ��=%2").arg(groupNum).arg(m_nFIFOSpace));
                    return 0;
                }
                else
                {
                    m_currentSendActionIndex++;
                    return 1;
                }
            }
            else if (2 == m_dataResponse[FUNCTION_CODE])
            {
                
                m_nFIFOSpace = charToShort(m_dataResponse[FUNCTION_PARM_1], m_dataResponse[FUNCTION_PARM_2]);

                
                groupNum = charToShort(m_dataResponse[FUNCTION_PARM_3], m_dataResponse[FUNCTION_PARM_4]);
                if (0 == groupNum || 0 == m_nFIFOSpace)
                {
                    //e.insertDebug(DEBUG_NET_PARM_ERROR, QString("FIFO����!, FIFO����=%1").arg(groupNum));
                    return 0;
                }
                else
                {
                    e.insertDebug(DEBUG_NET_PARM_ERROR, QString::fromLocal8Bit("��λ�˶����ݷ���ʧ��!"));
                    return -1;
                }
            }
        }
    }
    e.insertDebug(DEBUG_NET_PARM_ERROR, QString::fromLocal8Bit("��λ�˶����ݷ���ʧ�ܣ����ݽ���ʧ�ܣ�!"));
    return  -1;
}*/

/*************************************************
  �������ƣ�setParam(char* paramValue, int paramNum, ErrorInfo& e)
  �������ܣ���������
  ���������paramValue��������ֵ
          paramNum����������������44��
  ���������
  �� �� ֵ��true/false
  ��    ע�������ڶ˿�1
**************************************************/
bool MotionControllerDevice::setParam(char* paramValue, int paramNum, ErrorInfo& e)//�˿�1
{
    initRequestFrame();
    m_dataRequest[FUNCTION_CODE] = 10;

    for (int i = 0; i < paramNum; i++)
    {
        m_dataRequest.append(paramValue[i]);
    }

    //����m_dataRequest; ����m_dataResponse
    if (tcpSentAndReceive(e))
    {
        if (1 != m_dataResponse[FUNCTION_CODE])
        {
            e.insertDebug(DEBUG_NET_PARM_ERROR, QString::fromLocal8Bit("��������ʧ��!"));
            return false;
        }
        return true;
    }
    e.insertDebug(DEBUG_NET_PARM_ERROR, QString::fromLocal8Bit("��������ʧ�ܣ����ݽ���ʧ�ܣ�!"));
    return  false;
}

/*************************************************
  �������ƣ�getMontionCount(ErrorInfo& e)
  �������ܣ�ѯ������ɵ�λ�˶���
  ���������
  ���������e��������Ϣ
  �� �� ֵ��true/false
  ��    ע�������ڶ˿�1
**************************************************/
bool MotionControllerDevice::getMontionCount(ErrorInfo& e)//�˿�1
{
    initRequestFrame();
    m_dataRequest[FUNCTION_CODE] = 22;

    //����m_dataRequest; ����m_dataResponse
    if (tcpSentAndReceive(e))
    {
        /* ��¼��ǰ���꣬��Э��û��֧�֣������� */
        /*if (m_nRecvMsgControllerDataNum == 27)
        {
            NAxis curPos;
            curPos.x = charToInt(m_dataResponse[7], m_dataResponse[8], m_dataResponse[9], m_dataResponse[10]);
            curPos.y = charToInt(m_dataResponse[11], m_dataResponse[12], m_dataResponse[13], m_dataResponse[14]);
            curPos.z = charToInt(m_dataResponse[15], m_dataResponse[16], m_dataResponse[17], m_dataResponse[18]);
            curPos.r = charToInt(m_dataResponse[19], m_dataResponse[20], m_dataResponse[21], m_dataResponse[22]);
            curPos.v = charToInt(m_dataResponse[23], m_dataResponse[24], m_dataResponse[25], m_dataResponse[26]);

            curPos.z = curPos.z >= 0 ? curPos.z % (360 * m_pSystemParm->m_axisParm[AXIS_T].m_nPulseEquivalent) : -((-curPos.z) % (360 * m_pSystemParm->m_axisParm[AXIS_T].m_nPulseEquivalent));
            curPos.r = curPos.r % (360 * m_pSystemParm->m_axisParm[AXIS_C].m_nPulseEquivalent);
            m_currentPos.x = (double)curPos.x / m_pSystemParm->m_axisParm[AXIS_X].m_nPulseEquivalent * m_pSystemParm->m_axisParm[AXIS_X].m_nRatioDenominator / m_pSystemParm->m_axisParm[AXIS_X].m_nRatioNumerator;//m_pulseEquivalent.x;
            m_currentPos.y = (double)curPos.y / m_pSystemParm->m_axisParm[AXIS_Y].m_nPulseEquivalent * m_pSystemParm->m_axisParm[AXIS_Y].m_nRatioDenominator / m_pSystemParm->m_axisParm[AXIS_Y].m_nRatioNumerator;//m_pulseEquivalent.y;
            m_currentPos.z = (double)curPos.z / m_pSystemParm->m_axisParm[AXIS_T].m_nPulseEquivalent * m_pSystemParm->m_axisParm[AXIS_T].m_nRatioDenominator / m_pSystemParm->m_axisParm[AXIS_T].m_nRatioNumerator;//m_pulseEquivalent.t;
            m_currentPos.r = (double)curPos.r / m_pSystemParm->m_axisParm[AXIS_C].m_nPulseEquivalent * m_pSystemParm->m_axisParm[AXIS_C].m_nRatioDenominator / m_pSystemParm->m_axisParm[AXIS_C].m_nRatioNumerator;//m_pulseEquivalent.c;
            m_currentPos.v = (double)curPos.v / m_pSystemParm->m_axisParm[AXIS_V].m_nPulseEquivalent;//m_pulseEquivalent.v;

        }*/

        if (m_nRecvMsgControllerDataNum > 6)
        {
            if (1 != m_dataResponse[FUNCTION_CODE])
            {
                e.insertDebug(DEBUG_NET_PARM_ERROR, QString::fromLocal8Bit("ѯ������ɵ�λ�˶���ʧ��!"));
                return false;
            }

            /* ��¼����ɵ�λ�˶��� */
            m_nMotionCount = charToInt(m_dataResponse[FUNCTION_PARM_1], m_dataResponse[FUNCTION_PARM_2], m_dataResponse[FUNCTION_PARM_3], m_dataResponse[FUNCTION_PARM_4]);
            if (m_nMotionCount != m_currentRunActionIndex)
            {
                m_currentRunActionIndex = m_nMotionCount;
                m_bisFinshOneAction = true;
            }

            if (m_nRecvMsgControllerDataNum != 27)
            {
                e.insertDebug(DEBUG_NET_PARM_ERROR, QString::fromLocal8Bit("ѯ������ɵ�λ�˶���ʧ�ܣ���ʽ����!"));
                return false;
            }

            return true;
        }
    }
    e.insertDebug(DEBUG_NET_PARM_ERROR, QString::fromLocal8Bit("ѯ������ɵ�λ�˶���ʧ�ܣ����ݽ���ʧ�ܣ�!"));
    return false;
}

/*************************************************
  �������ƣ�getMsg()
  �������ܣ��ϴ��¼�֪ͨ
  ���������
  ���������e��������Ϣ
  �� �� ֵ��true/false
  ��    ע�������ڶ˿�2
**************************************************/
bool MotionControllerDevice::getMsg(ErrorInfo& e)//�˿�2
{
    m_dataResponse_monitor.clear();
    for (int i = 0; i < m_nRecvControllerDataNum; i++)
    {
        m_dataResponse_monitor.append(m_recvControllerData[i]);
        //tempStr += m_recvControllerData[i];
    }

    if (m_dataResponse_monitor[FUNCTION_CODE] == MSG_FUNCTION_GET_NOTIFY)
    {
        if (m_nRecvControllerDataNum > 6)
        {
            switch (m_dataResponse_monitor[FUNCTION_PARM_1])
            {
            case 1:
                m_sysState = SYS_STATE_EMERGSTOP_ASK;
                break;
            case 2:
                m_sysState = SYS_STATE_AXISALARM;
                break;
           /* case 3:
                m_sysState = SYS_STATE_LIMIT;

                if ((m_dataResponse_monitor[FUNCTION_PARM_2] & 0x01) == 1)
                {
                    m_axisLimit.x = 1;
                }
                if ((m_dataResponse_monitor[FUNCTION_PARM_2] & 0x02) == 1)
                {
                    m_axisLimit.y = 1;
                }
                if ((m_dataResponse_monitor[FUNCTION_PARM_2] & 0x04) == 1)
                {
                    m_axisLimit.z = 1;
                }
                if ((m_dataResponse_monitor[FUNCTION_PARM_2] & 0x08) == 1)
                {
                    m_axisLimit.r = 1;
                }
                if ((m_dataResponse_monitor[FUNCTION_PARM_2] & 0x10) == 1)
                {
                    m_axisLimit.v = 1;
                }

                if ((m_dataResponse_monitor[FUNCTION_PARM_3] & 0x01) == 1)
                {
                    m_axisLimit.x = -1;
                }
                if ((m_dataResponse_monitor[FUNCTION_PARM_3] & 0x02) == 1)
                {
                    m_axisLimit.y = -1;
                }
                if ((m_dataResponse_monitor[FUNCTION_PARM_3] & 0x04) == 1)
                {
                    m_axisLimit.z = -1;
                }
                if ((m_dataResponse_monitor[FUNCTION_PARM_3] & 0x08) == 1)
                {
                    m_axisLimit.r = -1;
                }
                if ((m_dataResponse_monitor[FUNCTION_PARM_3] & 0x10) == 1)
                {
                    m_axisLimit.v = -1;
                }
                break;*/
            case 4:
                m_sysState = SYS_STATE_IDENTIFY_DONE;
                break;
            case 5:
                m_sysState = SYS_STATE_EMERGSTOP_DONE;
                break;
            case 6:
                m_sysState = SYS_STATE_GOHOME_DONE;
                break;
            case 7:
                m_sysState = SYS_STATE_DI_CHANGE;
                m_nDI = charToShort(m_dataResponse_monitor[FUNCTION_PARM_3], m_dataResponse_monitor[FUNCTION_PARM_4]);
                break;
            default:
                m_sysState = SYS_STETE_NORMAL;
                break;
            }

            /* �ɹ�ִ�У�Ӧ��1 */
            initRespondFrame_monitor();
            m_dataRequest_monitor[FUNCTION_CODE] = 1;
            return tcpSent();
        }
    }

    /* δ�ܳɹ���ȡ�ϴ��¼���Ӧ��2 */
    initRespondFrame_monitor();
    m_dataRequest_monitor[FUNCTION_CODE] = 2;
    tcpSent();
    e.insertDebug(DEBUG_NET_PARM_ERROR, QString::fromLocal8Bit("�ϴ��¼�֪ͨʧ�ܣ����ݽ���ʧ�ܣ�!"));
    return false;
}

/*************************************************
  �������ƣ�getMotionState(ErrorInfo& e)
  �������ܣ��ϴ���λ�˶�״̬
  ���������
  ���������
  �� �� ֵ��true/false
  ��    ע�������ڶ˿�2
**************************************************/
bool MotionControllerDevice::getMotionState(ErrorInfo& e)//�˿�2
{
    m_dataResponse_monitor.clear();
    //QString tempStr = "";
    for (int i = 0; i < m_nRecvControllerDataNum; i++)
    {
        m_dataResponse_monitor.append(m_recvControllerData[i]);
        //tempStr += QString::number(m_recvControllerData[i]);
    }
    //qDebug()<<"duanshu"<<m_nRecvControllerDataNum;

    //if (m_nRecvControllerDataNum != 27)
    if (m_nRecvControllerDataNum < 27)
    {
        initRespondFrame_monitor();
        m_dataRequest_monitor[FUNCTION_CODE] = 2;
        tcpSent();
        //return false;
        e.insertDebug(DEBUG_NET_PARM_ERROR, QString::fromLocal8Bit("alarm15 : �ϴ���λ���ݴ���: ����֡���ȴ���"));
        return false;
    }

    /* ��¼����ɵ�λ�˶��� */
    m_nMotionCount = charToInt(m_dataResponse_monitor[FUNCTION_PARM_1], m_dataResponse_monitor[FUNCTION_PARM_2], m_dataResponse_monitor[FUNCTION_PARM_3], m_dataResponse_monitor[FUNCTION_PARM_4]);
    if (m_nMotionCount != m_currentRunActionIndex)
    {
        m_currentRunActionIndex = m_nMotionCount;
        m_bisFinshOneAction = true;
    }

    /* ��¼��ǰ���� */
    /*NAxis curPos;
    //int j = 1;
    //curPos.x = charToInt(m_dataResponse[FUNCTION_PARM_4 + j++], m_dataResponse[FUNCTION_PARM_4 + j++], m_dataResponse[FUNCTION_PARM_4 + j++], m_dataResponse[FUNCTION_PARM_4 + j++]);//��ջ����
    curPos.x = charToInt(m_dataResponse_monitor[7], m_dataResponse_monitor[8], m_dataResponse_monitor[9], m_dataResponse_monitor[10]);
    curPos.y = charToInt(m_dataResponse_monitor[11], m_dataResponse_monitor[12], m_dataResponse_monitor[13], m_dataResponse_monitor[14]);
    curPos.z = charToInt(m_dataResponse_monitor[15], m_dataResponse_monitor[16], m_dataResponse_monitor[17], m_dataResponse_monitor[18]);
    curPos.r = charToInt(m_dataResponse_monitor[19], m_dataResponse_monitor[20], m_dataResponse_monitor[21], m_dataResponse_monitor[22]);
    curPos.v = charToInt(m_dataResponse_monitor[23], m_dataResponse_monitor[24], m_dataResponse_monitor[25], m_dataResponse_monitor[26]);

    curPos.z = curPos.z >= 0 ? curPos.z % (360 * m_pSystemParm->m_axisParm[AXIS_T].m_nPulseEquivalent) : -((-curPos.z) % (360 * m_pSystemParm->m_axisParm[AXIS_T].m_nPulseEquivalent));
    curPos.r = curPos.r % (360 * m_pSystemParm->m_axisParm[AXIS_C].m_nPulseEquivalent);
    m_nCurrentPosition = curPos;
    m_currentPos.x = (double)curPos.x / m_pSystemParm->m_axisParm[AXIS_X].m_nPulseEquivalent * m_pSystemParm->m_axisParm[AXIS_X].m_nRatioDenominator / m_pSystemParm->m_axisParm[AXIS_X].m_nRatioNumerator;//m_pulseEquivalent.x;
    m_currentPos.y = (double)curPos.y / m_pSystemParm->m_axisParm[AXIS_Y].m_nPulseEquivalent * m_pSystemParm->m_axisParm[AXIS_Y].m_nRatioDenominator / m_pSystemParm->m_axisParm[AXIS_Y].m_nRatioNumerator;//m_pulseEquivalent.y;
    m_currentPos.z = (double)curPos.z / m_pSystemParm->m_axisParm[AXIS_T].m_nPulseEquivalent * m_pSystemParm->m_axisParm[AXIS_T].m_nRatioDenominator / m_pSystemParm->m_axisParm[AXIS_T].m_nRatioNumerator;//m_pulseEquivalent.t;
    m_currentPos.r = (double)curPos.r / m_pSystemParm->m_axisParm[AXIS_C].m_nPulseEquivalent * m_pSystemParm->m_axisParm[AXIS_C].m_nRatioDenominator / m_pSystemParm->m_axisParm[AXIS_C].m_nRatioNumerator;//m_pulseEquivalent.c;
    m_currentPos.v = (double)curPos.v / m_pSystemParm->m_axisParm[AXIS_V].m_nPulseEquivalent;//m_pulseEquivalent.v;
     */
    //qDebug("a=%d, b=%d, c=%d, d=%d ��ǰλ��  a=%d, b=%d, c=%d, d=%d", m_dataResponse[3], m_dataResponse[4], m_dataResponse[5],m_dataResponse[6], m_dataResponse[7], m_dataResponse[8], m_dataResponse[9], m_dataResponse[10]);
    //qDebug("n=%d, ��ǰλ�� x=%d, y=%d, t=%d, c=%d, v=%d\n", m_nMotionCount, curPos.x, curPos.y, curPos.t, curPos.c, curPos.v);
    //qDebug("n=%d, ��ǰλ�� x=%g, y=%g, t=%g, c=%g, v=%g", m_currentRunActionIndex, m_currentPos.x, m_currentPos.y, m_currentPos.t, m_currentPos.c, m_currentPos.v);

    unsigned char send[TCP_BUFFER_SIZE];

    /* ��Ӧ֡֡ͷ 2�ֽ� */
    SHORTCHAR requestFrame;
    requestFrame.data = 0xA051;
    if (ENDIANNESS == BIG_ENDIAN)
    {
        send[0] = requestFrame.c[0];
        send[1] = requestFrame.c[1];
    }
    else
    {
        send[0] = requestFrame.c[1];
        send[1] = requestFrame.c[0];
    }

    /* ��Ӧ�� 1�ֽ� */
    send[FUNCTION_CODE] = 1;
    return  m_PCRecvCommandSocket->TCPWriteData((char*)send, 3);
}

/*************************************************
  �������ƣ�getHeartBeat()
  �������ܣ��ϴ�����״̬
  ���������
  ���������
  �� �� ֵ��true/false
  ��    ע�������ڶ˿�2
**************************************************/
bool MotionControllerDevice::getHeartBeat(ErrorInfo& e)//�˿�2
{           
    unsigned char send[TCP_BUFFER_SIZE];

    /* ��Ӧ֡֡ͷ 2�ֽ� */
    SHORTCHAR requestFrame;
    requestFrame.data = 0xA051;
    if (ENDIANNESS == BIG_ENDIAN)
    {
        send[0] = requestFrame.c[0];
        send[1] = requestFrame.c[1];
    }
    else
    {
        send[0] = requestFrame.c[1];
        send[1] = requestFrame.c[0];
    }

    /* ��Ӧ�� 1�ֽ� */
    send[FUNCTION_CODE] = 1;

    /* ��Ӧ���� 1�ֽ� */
    send[FUNCTION_PARM_1] = m_recvControllerData[FUNCTION_PARM_1];
    return m_PCRecvCommandSocket->TCPWriteData((char*)send, 4);
}

/*************************************************
  �������ƣ�runOneActions(const BaseAction*  action, ProcessInterVariableCache* cache, SystemParameter* para, ErrorInfo& e)
  �������ܣ�����һ��action
  ���������action��G����������˶�ָ�FIFO�˶�ָ�
          cache���ӹ������е�ȫ�ֱ���
  ���������e��������Ϣ
  �� �� ֵ��-1��-2������ 0��ʧ�ܣ�FIFO������ 1���ɹ����
  ��    ע��
**************************************************/
int MotionControllerDevice::runOneActions(const BaseAction*  action, ProcessInterVariableCache* cache, SystemParameter* para, ErrorInfo& e)
{
    /* һ��ACtionִ���꣬�򻺳����п��з���true�� ��������false���д����¼��ErrorInfo�� */
    if(mutex.tryLock())
    {
        int bb;
        if(action->m_type == ACTION_MOVE_TYPE)
        {
            if(((MoveAction *)action)->m_moveType==MOVE_POINT)
            {
            		bb = pointToPoint((MoveAction *)action, cache, para, e);
            }
	      else if(((MoveAction *)action)->m_moveType==MOVE_LINE)
            {
            		bb = LineMove((MoveAction *)action, cache, para, e);
            }
        }
	else if(action->m_type == ACTION_IO_TYPE)
	{
	    if(((IOAction *)action)->m_nIoType==IO_OUT)
            {
            		bb = OutIO((IOAction *)action, cache, para, e);
            }
	      else if(((IOAction *)action)->m_nIoType==IO_IN)
            {
            		bb = WaitIN((IOAction *)action, cache, para, e);
            }
	}
	else if(action->m_type == ACTION_DELAY_TYPE)
	{
	    
            	bb = Delay((DelayAction *)action, cache, para, e);
         
	}
     
        mutex.unlock();
        return bb;
    }
    else
    {
        return -2;
    }


//    drawCarrierGraphical(action);
//    drawCarrierGraphical(ToolInfo* tool, Point p, double a);
    //m_canvs->update();
    return -1;
}

/*************************************************
  �������ƣ�tcpSent()
  �������ܣ��������ݵ�������
  ���������
  ���������
  �� �� ֵ��true/false
  ��    ע�������ڶ˿�2
**************************************************/
bool MotionControllerDevice::tcpSent()//�˿�2
{
    //if(m_PCSendCommandSocket->isConnect())
    if (m_bisNetConnect)
    {
        m_nSendControllerDataNum = m_dataRequest_monitor.size();
        for (int i = 0; i < m_nSendControllerDataNum; i++)
        {
            //m_sendMsgControllerData[i] = m_dataRequest_monitor[i];
            m_sendControllerData[i] = m_dataRequest_monitor[i];
        }
        //m_PCSendCommandSocket->TCPWriteData((char *)m_sendMsgControllerData, m_nSendMsgControllerDataNum);
        m_PCRecvCommandSocket->TCPWriteData((char *)m_sendControllerData, m_nSendControllerDataNum);

        return true;
    }
    return  false;
}

/*************************************************
  �������ƣ�tcpReceive(ErrorInfo &e)
  �������ܣ����տ���������������
  ���������
  ���������
  �� �� ֵ��true/false
  ��    ע�������ڶ˿�1
**************************************************/
bool MotionControllerDevice::tcpReceive(ErrorInfo &e)//�˿�1
{
    //if(m_PCSendCommandSocket->isConnect())
    if (m_bisNetConnect)
    {
        if(m_PCSendCommandSocket->TCPReadMsgData((char *)m_recvMsgControllerData, m_nRecvMsgControllerDataNum))
        {
            m_dataResponse.clear();
            if (m_nRecvMsgControllerDataNum > 2)
             {
                for (int i = 0; i < m_nRecvMsgControllerDataNum; i++)
                {
                    m_dataResponse.append(m_recvMsgControllerData[i]);
                }
                return true;
            }
        }
    }
    return  false;
}

/*************************************************
  �������ƣ�tcpSentAndReceive(ErrorInfo &e)
  �������ܣ��������ݵ������������տ�����������������
  ���������
  ���������
  �� �� ֵ��true/false
  ��    ע�������ڶ˿�1
**************************************************/
bool MotionControllerDevice::tcpSentAndReceive(ErrorInfo &e)//�˿�1
{
    if(m_bisNetConnect/*m_PCSendCommandSocket->isConnect()*/)
    {
        /* send data */
        m_nSendMsgControllerDataNum = m_dataRequest.size();
        if (m_nSendMsgControllerDataNum > TCP_BUFFER_SIZE)
        {
            return false;
        }
        for (int i = 0; i < m_nSendMsgControllerDataNum; i++)
        {
            m_sendMsgControllerData[i] = m_dataRequest[i];
        }

        if (m_PCSendCommandSocket->TCPWriteMsgData((char *)m_sendMsgControllerData, m_nSendMsgControllerDataNum))
        {
            /* receive data */
            if(m_PCSendCommandSocket->TCPReadMsgData((char *)m_recvMsgControllerData, m_nRecvMsgControllerDataNum))
            {
                m_dataResponse.clear();
                if (m_nRecvMsgControllerDataNum > 2)
                {
                    for (int i = 0; i < m_nRecvMsgControllerDataNum; i++)
                    {
                        m_dataResponse.append(m_recvMsgControllerData[i]);
                    }
                    return true;
                }
            }
        }
    }
    return  false;
}

/*************************************************
  �������ƣ�tcpMonitor(ErrorInfo &e)
  �������ܣ����տ�����������������
  ���������
  ���������
  �� �� ֵ��true/false
  ��    ע�������ڶ˿�2��δʹ�ã�
**************************************************/
bool MotionControllerDevice::tcpMonitor(ErrorInfo &e)//�˿�2
{
    if(m_PCRecvCommandSocket->TCPReadData((char *)m_recvControllerData, m_nRecvControllerDataNum))
    {
//       SystemProcessInfo::printfMesssage(QString(QLatin1String(m_recvControllerData)));
        m_dataResponse_monitor.clear();
        if (m_nRecvControllerDataNum > 2)
        {
            for (int i = 0; i < m_nRecvControllerDataNum; i++)
            {
                m_dataResponse_monitor.append(m_recvControllerData[i]);
            }
            return true;
        }
    }
    return false;
}
int MotionControllerDevice::SaveXYZRPos(char *data,int num)//�˿�2
{
     
      if((num>32)&&(data[0]==((char)0xaa))&&(data[1]==((char)0x55)))
      {
              memcpy((char *)&scarapos[0],&data[2],32);
	       return 1;
      }
      return 0;
	   
}
/*************************************************
  �������ƣ�tcpMonitorData()
  �������ܣ����տ�����������������
  ���������
  ���������
  �� �� ֵ��true/false
  ��    ע�������ڶ˿�2�����������ݴ���ʱ��ֱ�Ӵ𸴿��������մ���
**************************************************/
void MotionControllerDevice::tcpMonitorData()//�˿�2
{
    mutex.lock();
    if(m_PCRecvCommandSocket->TCPReadData((char *)m_recvControllerData, m_nRecvControllerDataNum))
    {
//      SystemProcessInfo::printfMesssage(QString::number(m_nRecvControllerDataNum));
#if 0
        if (m_nRecvControllerDataNum > 2)
        {
            ((MotionControllerModule *)m_pModuleHandle)->monitorControllerValue(m_recvControllerData, m_nRecvControllerDataNum);
        }
        else//����������Ч�����ش���
        {
            initRespondFrame_monitor();
            m_dataRequest_monitor[FUNCTION_CODE] = 2;
            tcpSent();
        }
#endif		
	if(SaveXYZRPos((char *)m_recvControllerData,m_nRecvControllerDataNum))
	{
		ifgetnetvalue=1;
	}
    }
    mutex.unlock();
}

/*************************************************
  �������ƣ�OnRecvSocketDisconnected()
  �������ܣ��˿�2�Ͽ����Ӵ���
  ���������
  ���������
  �� �� ֵ��
  ��    ע��
**************************************************/
void MotionControllerDevice::OnRecvSocketDisconnected()
{
    SystemProcessInfo::printfMesssage("OnRecvSocketDisconnected");

    m_PCRecvCommandSocket->closeSocket();
    //((MotionControllerModule *)m_pModuleHandle)->m_WorkStatus = MotionControllerModule::MOTION_NO_CONNECT_NET;
    //((MotionControllerModule *)m_pModuleHandle)->m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "not connect");
    m_bisNetConnect = false;
    //emit NetLedCtrl(false);
}

void MotionControllerDevice::OnRecvSocketError(QAbstractSocket::SocketError error)
{
    SystemProcessInfo::printfMesssage("�Ͽ�����Recv");

    m_PCRecvCommandSocket->closeSocket();
    //((MotionControllerModule *)m_pModuleHandle)->m_WorkStatus = MotionControllerModule::MOTION_NO_CONNECT_NET;
    //((MotionControllerModule *)m_pModuleHandle)->m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "not connect");
    m_bisNetConnect = false;
    //emit NetLedCtrl(false);
}

void MotionControllerDevice::OnSentSocketError(QAbstractSocket::SocketError error)
{
    SystemProcessInfo::printfMesssage("�Ͽ�����Sent");
    m_PCRecvCommandSocket->closeSocket();
    ((MotionControllerModule *)m_pModuleHandle)->m_WorkStatus = MotionControllerModule::MOTION_NO_CONNECT_NET;
    ((MotionControllerModule *)m_pModuleHandle)->m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "not connect");
    m_bisNetConnect = false;
    emit NetLedCtrl(false);
}
/*************************************************
  �������ƣ�OnRecvSocketDisconnected()
  �������ܣ��˿�1�Ͽ����Ӵ���
  ���������
  ���������
  �� �� ֵ��
  ��    ע��
**************************************************/
void MotionControllerDevice::OnSentSocketDisconnected()
{
    SystemProcessInfo::printfMesssage("OnSentSocketDisconnected");
    m_PCRecvCommandSocket->closeSocket();
    ((MotionControllerModule *)m_pModuleHandle)->m_WorkStatus = MotionControllerModule::MOTION_NO_CONNECT_NET;
    ((MotionControllerModule *)m_pModuleHandle)->m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "not connect");
    m_bisNetConnect = false;
    emit NetLedCtrl(false);
}

/*************************************************
  �������ƣ�pointToPoint(MoveAction * action, ProcessInterVariableCache* cache, SystemParameter* para, ErrorInfo& e)
  �������ܣ���FIFOдһ���λ�˶����ݣ���������ѹ��
  ���������
  ���������
  �� �� ֵ��
  ��    ע��
**************************************************/
int MotionControllerDevice::pointToPoint(MoveAction * action, ProcessInterVariableCache* cache, SystemParameter* para, ErrorInfo& e)
{
    NAxis deltaPluse = action->pointToPoint(cache, para);
    
    DAxis deltaPos=action->nAxisToDAxis(deltaPluse, para);
    cache->m_currentRunActionIndex = m_currentRunActionIndex;
    cache->m_currentSendActionIndex = m_currentSendActionIndex;

    /*int axisData[5];
    axisData[0] = deltaPluse.x;
    axisData[1] = deltaPluse.y;
    axisData[2] = deltaPluse.t;
    axisData[3] = deltaPluse.c;
    axisData[4] = 0;*/

    double axisData[MOF];
    axisData[0] = cache->m_newPoint.x;
    axisData[1] = cache->m_newPoint.y;
    axisData[2] = cache->m_newPoint.z;
    axisData[3] = cache->m_newPoint.w;
    axisData[4] = cache->m_newPoint.p;
    axisData[5] = cache->m_newPoint.r;
	
	return fillOneFIFO(action,axisData, e);
    //return fillOneFIFO((int)(action->m_moveType),axisData, e);
}

int MotionControllerDevice::LineMove(MoveAction * action, ProcessInterVariableCache* cache, SystemParameter* para, ErrorInfo& e)
{
    NAxis deltaPluse = action->pointToPoint(cache, para);
    
    DAxis deltaPos=action->nAxisToDAxis(deltaPluse, para);
    cache->m_currentRunActionIndex = m_currentRunActionIndex;
    cache->m_currentSendActionIndex = m_currentSendActionIndex;

    /*int axisData[5];
    axisData[0] = deltaPluse.x;
    axisData[1] = deltaPluse.y;
    axisData[2] = deltaPluse.t;
    axisData[3] = deltaPluse.c;
    axisData[4] = 0;*/

    double axisData[MOF];
    /*axisData[0] = deltaPos.x;
    axisData[1] = deltaPos.y;
    axisData[2] = deltaPos.z;
    axisData[3] = deltaPos.r;
    axisData[4] = deltaPos.v;*/

    axisData[0] = cache->m_newPoint.x;
    axisData[1] = cache->m_newPoint.y;
    axisData[2] = cache->m_newPoint.z;
    axisData[3] = cache->m_newPoint.w;
    axisData[4] = cache->m_newPoint.p;
    axisData[5] = cache->m_newPoint.r;
	
    return fillOneFIFO(action,axisData, e);
    //return fillOneFIFO((int)(action->m_moveType),axisData, e);
}

int MotionControllerDevice::OutIO(IOAction * action, ProcessInterVariableCache* cache, SystemParameter* para, ErrorInfo& e)
{
    
    
    
	
	return fillOneFIFO((IOAction *)action, e);
    //return fillOneFIFO((int)(action->m_moveType),axisData, e);
}

int MotionControllerDevice::WaitIN(IOAction * action, ProcessInterVariableCache* cache, SystemParameter* para, ErrorInfo& e)
{
   
	
	return fillOneFIFO((IOAction *)action, e);
    //return fillOneFIFO((int)(action->m_moveType),axisData, e);
}

int MotionControllerDevice::Delay(DelayAction * action, ProcessInterVariableCache* cache, SystemParameter* para, ErrorInfo& e)
{
    
	
	return fillOneFIFO((DelayAction *)action, e);
    //return fillOneFIFO((int)(action->m_moveType),axisData, e);
}

/*************************************************
  �������ƣ�emulationMotion()
  �������ܣ����棬��ͼ����������ǰλ��
  ���������
  ���������
  �� �� ֵ��
  ��    ע��
**************************************************/
bool MotionControllerDevice::emulationMotion()
{
    drawCarrierGraphical(m_currentTool, Point(m_currentPos.x, m_currentPos.y,0,0,0,0), m_currentPos.r);
//    m_canvs->update();
    return true;
}

/*************************************************
  �������ƣ�emergencyStop(ErrorInfo &e)
  �������ܣ���ͣ
  ���������
  ���������e��������Ϣ
  �� �� ֵ��
  ��    ע���ȷ��ͼ�ָͣ��������FIFOָ��
**************************************************/
bool MotionControllerDevice::emergencyStop(ErrorInfo &e)
{
    if (!sendMsg(3, e))
    {
        return false;
    }
    return clearFIFO(e);
}

bool MotionControllerDevice::clearEmergencyStop(ErrorInfo &e)
{
    if (!sendMsg(6, e))
    {
        return false;
    }
    return true;
}

/*************************************************
  �������ƣ�normalTurnOn(ErrorInfo &e)
  �������ܣ���������֪ͨ
  ���������
  ���������e��������Ϣ
  �� �� ֵ��
  ��    ע��
**************************************************/
bool MotionControllerDevice::normalTurnOn(ErrorInfo &e)
{
    return sendMsg(1, e);
}

/*************************************************
  �������ƣ�normalTurnOff(ErrorInfo &e)
  �������ܣ������ػ�֪ͨ
  ���������
  ���������e��������Ϣ
  �� �� ֵ��
  ��    ע��
**************************************************/
bool MotionControllerDevice::normalTurnOff(ErrorInfo &e)
{
    return sendMsg(2, e);
}

/*************************************************
  �������ƣ�getCurrentRunActionIndex()
  �������ܣ���ȡ��ǰλ�ã����������������ĵ�λλ�ã�
  ���������
  ���������
  �� �� ֵ����ǰ�˶�λ��
  ��    ע��
**************************************************/
int MotionControllerDevice::getCurrentRunActionIndex()
{
    return m_currentRunActionIndex;
}

/*************************************************
  �������ƣ�manualMove(int axis, double dis, ErrorInfo& e)
  �������ܣ��ֶ��˶�
  ���������axis���˶���
          dis���˶�����
  ���������e��������Ϣ
  �� �� ֵ��true/false
  ��    ע��
**************************************************/
bool MotionControllerDevice::manualMove(int axis, double dis, ErrorInfo& e)
{
    int axisData[5];
    switch (axis)
    {
    case AXIS_X:
        axisData[0] = dis * m_pSystemParm->m_axisParm[AXIS_X].m_nPulseEquivalent * m_pSystemParm->m_axisParm[AXIS_X].m_nRatioNumerator / m_pSystemParm->m_axisParm[AXIS_X].m_nRatioDenominator;
        axisData[1] = 0;
        axisData[2] = 0;
        axisData[3] = 0;
        axisData[4] = 0;
        break;
    case AXIS_Y:
        axisData[0] = 0;
        axisData[1] = dis * m_pSystemParm->m_axisParm[AXIS_Y].m_nPulseEquivalent * m_pSystemParm->m_axisParm[AXIS_Y].m_nRatioNumerator / m_pSystemParm->m_axisParm[AXIS_Y].m_nRatioDenominator;
        axisData[2] = 0;
        axisData[3] = 0;
        axisData[4] = 0;
        break;
    case AXIS_T:
        axisData[0] = 0;
        axisData[1] = 0;
        axisData[2] = dis * m_pSystemParm->m_axisParm[AXIS_T].m_nPulseEquivalent * m_pSystemParm->m_axisParm[AXIS_T].m_nRatioNumerator / m_pSystemParm->m_axisParm[AXIS_T].m_nRatioDenominator;
        axisData[3] = 0;
        axisData[4] = 0;
        break;
    case AXIS_C:
        axisData[0] = 0;
        axisData[1] = 0;
        axisData[2] = 0;
        axisData[3] = dis * m_pSystemParm->m_axisParm[AXIS_C].m_nPulseEquivalent * m_pSystemParm->m_axisParm[AXIS_C].m_nRatioNumerator / m_pSystemParm->m_axisParm[AXIS_C].m_nRatioDenominator;
        axisData[4] = 0;
        break;
    case AXIS_V:
        axisData[0] = 0;
        axisData[1] = 0;
        axisData[2] = 0;
        axisData[3] = 0;
        axisData[4] = abs(dis) > 3 ? (dis * m_pSystemParm->m_axisParm[AXIS_V].m_nPulseEquivalent) : abs(dis);//dis * m_pSystemParm->m_axisParm[AXIS_V].m_nPulseEquivalent;
        break;
    default:
        axisData[0] = 0;
        axisData[1] = 0;
        axisData[2] = 0;
        axisData[3] = 0;
        axisData[4] = 0;
        break;
    }
    return fillOneFIFO(axisData, e);
}

/*************************************************
  �������ƣ�manualMove(ErrorInfo& e, double x, double y, double t, double c, double v)
  �������ܣ��ֶ��˶�
  ���������x��y��t��c��v��������ƶ��ľ���
  ���������e��������Ϣ
  �� �� ֵ��true/false
  ��    ע��
**************************************************/
bool MotionControllerDevice::manualMove(ErrorInfo& e, double x, double y, double z, double w, double p,double r)
{
    int axisData[5];
    /*axisData[0] = x * m_pSystemParm->m_axisParm[AXIS_X].m_nPulseEquivalent * m_pSystemParm->m_axisParm[AXIS_X].m_nRatioNumerator / m_pSystemParm->m_axisParm[AXIS_X].m_nRatioDenominator;
    axisData[1] = y * m_pSystemParm->m_axisParm[AXIS_Y].m_nPulseEquivalent * m_pSystemParm->m_axisParm[AXIS_Y].m_nRatioNumerator / m_pSystemParm->m_axisParm[AXIS_Y].m_nRatioDenominator;
    axisData[2] = t * m_pSystemParm->m_axisParm[AXIS_T].m_nPulseEquivalent * m_pSystemParm->m_axisParm[AXIS_T].m_nRatioNumerator / m_pSystemParm->m_axisParm[AXIS_T].m_nRatioDenominator;
    axisData[3] = c * m_pSystemParm->m_axisParm[AXIS_C].m_nPulseEquivalent * m_pSystemParm->m_axisParm[AXIS_C].m_nRatioNumerator / m_pSystemParm->m_axisParm[AXIS_C].m_nRatioDenominator;
    axisData[4] = (int)(v + 0.01);//abs(v) > 3 ? 1 : abs(v);//v > 0.1 ? 1 : 0;

    return fillOneFIFO(axisData, e);*/
    return 0;
}

/*************************************************
  �������ƣ�getSts(int *sts, ErrorInfo& e)
  �������ܣ���ȡ���������˶�״̬
  ���������
  ���������sts������˶�״̬��Ϊ0��ʾ���е����ֹͣ
          e��������Ϣ
  �� �� ֵ��true/false
  ��    ע�������ڶ˿�1
**************************************************/
bool MotionControllerDevice::getSts(int *sts, ErrorInfo& e)//�˿�1
{
    initRequestFrame();
    m_dataRequest[FUNCTION_CODE] = 23;
    m_dataRequest[FUNCTION_PARM_1] = 1;

    //����m_dataRequest; ����m_dataResponse
    if (tcpSentAndReceive(e))
    {
        if (m_nRecvMsgControllerDataNum > 6)
        {
            if (1 != m_dataResponse[FUNCTION_CODE])
            {
                //e.insertDebug(DEBUG_NET_PARM_ERROR, QString::fromLocal8Bit("��ȡ�˶�״̬��Ϣʧ��!"));
                return false;
            }
            m_nSts = charToInt(m_dataResponse[FUNCTION_PARM_1],m_dataResponse[FUNCTION_PARM_2],m_dataResponse[FUNCTION_PARM_3],m_dataResponse[FUNCTION_PARM_4]);
            *sts = m_nSts;// & 0x02;//��1λ������˶���־�����е����ֹͣ�˶�ʱ��0

            return true;
        }
    }
    //e.insertDebug(DEBUG_NET_PARM_ERROR, QString::fromLocal8Bit("��ȡ�˶�״̬��Ϣʧ��!�����ݽ��մ���"));
    return  false;
}

bool MotionControllerDevice::clearSts(ErrorInfo& e)
{
    initRequestFrame();
    m_dataRequest[FUNCTION_CODE] = 23;
    m_dataRequest[FUNCTION_PARM_1] = 2;

    //����m_dataRequest; ����m_dataResponse
    if (tcpSentAndReceive(e))
    {
        if (m_nRecvMsgControllerDataNum > 6)
        {
            if (1 != m_dataResponse[FUNCTION_CODE])
            {
                e.insertDebug(DEBUG_NET_PARM_ERROR, QString::fromLocal8Bit("����˶�״̬��Ϣʧ��!"));
                return false;
            }
            return true;
        }
    }
    e.insertDebug(DEBUG_NET_PARM_ERROR, QString::fromLocal8Bit("����˶�״̬��Ϣʧ��!�����ݽ��մ���"));
    return  false;
}

/*************************************************
  �������ƣ�setPunchLockSts(bool bSts)
  �������ܣ����ó�ͷ����
  ���������bSts������/�رճ�ͷ����
  ���������
  �� �� ֵ��
  ��    ע��
**************************************************/
void MotionControllerDevice::setPunchLockSts(bool bSts)
{
    m_bIsPunchLock = bSts;
}

/*************************************************
  �������ƣ�isTcpSendComConnect()
  �������ܣ�tcp����״̬�ж�
  ���������
  ���������
  �� �� ֵ��tcp����״̬
  ��    ע��
**************************************************/
bool MotionControllerDevice::isTcpSendComConnect()
{
    return m_bisNetConnect;
}

/*************************************************
  �������ƣ�setServoEnable(bool sts, ErrorInfo& e)
  �������ܣ�����/�ر��ŷ�ʹ��
  ���������sts������/�ر�
  ���������
  �� �� ֵ��
  ��    ע��
**************************************************/
bool MotionControllerDevice::setServoEnable(bool sts, ErrorInfo& e)
{
    bool result=0;
    if (sts)
    {
        //result = sendMotionCommand(9, e);
    }
    else
    {
        //result = sendMotionCommand(10, e);
    }

    //ˢ��ui��ť״̬
    //m_pMachiningUI->updateOutputBnt(IO_SERVOENABLE_BNT_ADD, (sts && result));
    //m_pManualMachiningUI->updateOutputBnt(IO_SERVOENABLE_BNT_ADD, (sts && result));
    return result;
}

bool MotionControllerDevice::clearFIFO(ErrorInfo &)
{
    if(0==(m_netctrl->gb_ctrl.sendtrafficctrl&0x4))
    {
        m_netctrl->gb_ctrl.sendtrafficctrl |=0x4;
        //m_netctrl->SendPeriodFrame(&gb_ctrl);
        return true;
    }
    return false;
}

bool MotionControllerDevice::fillOneFIFO(MoveMotionCode* data, bool isEnd)
{
    InputAutoData autodata;
    memset(&autodata, 0, sizeof(autodata));
    autodata.decode_id = data->m_nCodeId;
    autodata.Prog_SequNum = 0;
    autodata.Prog_LineNum = 0;
    autodata.Prog_Mark = isEnd ? 1 : 0;
    autodata.Mode = 1;
    autodata.Interp_Mode = 1;
    autodata.Joint_VelPct = 100;
    autodata.JPos_End[0] = (float)data->m_EndPoint.x;
    autodata.JPos_End[1] = (float)data->m_EndPoint.y;
    //autodata.Car_PVel = 200;

    if (PutAutoDataFrame(&autodata) == 1)
    {
        printFifoData(&autodata);
        return true;
    }
    else
        return false;
}

bool MotionControllerDevice::fillOneFIFO(LineMotionCode *data, bool isEnd, double vel)
{
    InputAutoData autodata;
    memset(&autodata, 0, sizeof(autodata));
    autodata.decode_id = data->m_nCodeId;
    autodata.Prog_SequNum = 0;
    autodata.Prog_LineNum = 0;
    autodata.Prog_Mark = isEnd ? 1 : 0;
    autodata.Mode = 1;
    autodata.Interp_Mode = 2;
    //autodata.Joint_VelPct = 100;
    autodata.JPos_End[0] = (float)data->m_EndPoint.x;
    autodata.JPos_End[1] = (float)data->m_EndPoint.y;
    autodata.Car_PVel = vel;
    autodata.Zone = data->m_nCrossLevel;

    if (PutAutoDataFrame(&autodata) == 1)
    {
        printFifoData(&autodata);
        return true;
    }
    else
        return false;
}

bool MotionControllerDevice::fillOneFIFO(ArcMotionCode *data, bool isEnd, double vel)
{
    InputAutoData autodata;
    memset(&autodata, 0, sizeof(autodata));
    autodata.decode_id = data->m_nCodeId;
    autodata.Prog_SequNum = 0;
    autodata.Prog_LineNum = 0;
    autodata.Prog_Mark = isEnd ? 1 : 0;
    autodata.Mode = 1;
    autodata.Interp_Mode = 3;
    //autodata.Joint_VelPct = 100;
    autodata.JPos_Start[0] = (float)data->m_StartPoint.x;
    autodata.JPos_Start[1] = (float)data->m_StartPoint.y;
    autodata.Zone = data->m_nCrossLevel;

    double a1 = data->m_dStartAngle;
    double a2 = data->m_dEndAngle;
    if (data->m_bIsReversed)//顺时针
    {
        if (a2 > a1)
            a2 -= PI2;
    }
    else
    {
        if (a2 < a1)
            a2 += PI2;
    }
    double a3 = (a1 + a2) * 0.5;
    autodata.JPos_Mid[0] = (float)(data->m_dRadius * cos(a3) + data->m_CenterPoint.x);
    autodata.JPos_Mid[1] = (float)(data->m_dRadius * sin(a3) + data->m_CenterPoint.y);
    autodata.JPos_End[0] = (float)data->m_EndPoint.x;
    autodata.JPos_End[1] = (float)data->m_EndPoint.y;
    autodata.Car_PVel = vel;

    if (PutAutoDataFrame(&autodata) == 1)
    {
        printFifoData(&autodata);
        return true;
    }
    else
        return false;
}

bool MotionControllerDevice::fillOneFIFO(CircleMotionCode *data, bool isEnd, double vel)
{
    InputAutoData autodata;
    memset(&autodata, 0, sizeof(autodata));
    autodata.decode_id = data->m_nCodeId;
    autodata.Prog_SequNum = 0;
    autodata.Prog_LineNum = 0;
    autodata.Prog_Mark = isEnd ? 1 : 0;
    autodata.Mode = 1;
    autodata.Interp_Mode = 4;
    //autodata.Joint_VelPct = 100;
    autodata.Zone = data->m_nCrossLevel;
    if (data->m_bIsReversed)//顺时针
    {
        if (data->m_dStartAngle > 0)
        {
            if (data->m_dStartAngle < M_PI_2)//第一象限
            {
                autodata.JPos_Start[0] = (float)data->m_StartPoint.x;
                autodata.JPos_Start[1] = (float)data->m_StartPoint.y;

                autodata.JPos_Mid[0] = (float)(data->m_CenterPoint.x);
                autodata.JPos_Mid[1] = (float)(data->m_CenterPoint.y - data->m_dRadius);

                autodata.JPos_End[0] = (float)(data->m_CenterPoint.x - data->m_dRadius);
                autodata.JPos_End[1] = (float)(data->m_CenterPoint.y);
            }
            else//第二象限
            {
                autodata.JPos_Start[0] = (float)data->m_StartPoint.x;
                autodata.JPos_Start[1] = (float)data->m_StartPoint.y;

                autodata.JPos_Mid[0] = (float)(data->m_CenterPoint.x + data->m_dRadius);
                autodata.JPos_Mid[1] = (float)(data->m_CenterPoint.y);

                autodata.JPos_End[0] = (float)(data->m_CenterPoint.x);
                autodata.JPos_End[1] = (float)(data->m_CenterPoint.y - data->m_dRadius);
            }
        }
        else
        {
            if (data->m_dStartAngle > -M_PI_2)//第三象限
            {
                autodata.JPos_Start[0] = (float)data->m_StartPoint.x;
                autodata.JPos_Start[1] = (float)data->m_StartPoint.y;

                autodata.JPos_Mid[0] = (float)(data->m_CenterPoint.x - data->m_dRadius);
                autodata.JPos_Mid[1] = (float)(data->m_CenterPoint.y);

                autodata.JPos_End[0] = (float)(data->m_CenterPoint.x);
                autodata.JPos_End[1] = (float)(data->m_CenterPoint.y + data->m_dRadius);
            }
            else//第四象限
            {
                autodata.JPos_Start[0] = (float)data->m_StartPoint.x;
                autodata.JPos_Start[1] = (float)data->m_StartPoint.y;

                autodata.JPos_Mid[0] = (float)(data->m_CenterPoint.x);
                autodata.JPos_Mid[1] = (float)(data->m_CenterPoint.y + data->m_dRadius);

                autodata.JPos_End[0] = (float)(data->m_CenterPoint.x + data->m_dRadius);
                autodata.JPos_End[1] = (float)(data->m_CenterPoint.y);
            }
        }
    }
    else
    {
        if (data->m_dStartAngle > 0)
        {
            if (data->m_dStartAngle < M_PI_2)//第一象限
            {
                autodata.JPos_Start[0] = (float)data->m_StartPoint.x;
                autodata.JPos_Start[1] = (float)data->m_StartPoint.y;

                autodata.JPos_Mid[0] = (float)(data->m_CenterPoint.x - data->m_dRadius);
                autodata.JPos_Mid[1] = (float)(data->m_CenterPoint.y);

                autodata.JPos_End[0] = (float)(data->m_CenterPoint.x);
                autodata.JPos_End[1] = (float)(data->m_CenterPoint.y - data->m_dRadius);
            }
            else//第二象限
            {
                autodata.JPos_Start[0] = (float)data->m_StartPoint.x;
                autodata.JPos_Start[1] = (float)data->m_StartPoint.y;

                autodata.JPos_Mid[0] = (float)(data->m_CenterPoint.x);
                autodata.JPos_Mid[1] = (float)(data->m_CenterPoint.y - data->m_dRadius);

                autodata.JPos_End[0] = (float)(data->m_CenterPoint.x + data->m_dRadius);
                autodata.JPos_End[1] = (float)(data->m_CenterPoint.y);
            }
        }
        else
        {
            if (data->m_dStartAngle > -M_PI_2)//第三象限
            {
                autodata.JPos_Start[0] = (float)data->m_StartPoint.x;
                autodata.JPos_Start[1] = (float)data->m_StartPoint.y;

                autodata.JPos_Mid[0] = (float)(data->m_CenterPoint.x + data->m_dRadius);
                autodata.JPos_Mid[1] = (float)(data->m_CenterPoint.y);

                autodata.JPos_End[0] = (float)(data->m_CenterPoint.x);
                autodata.JPos_End[1] = (float)(data->m_CenterPoint.y + data->m_dRadius);
            }
            else//第四象限
            {
                autodata.JPos_Start[0] = (float)data->m_StartPoint.x;
                autodata.JPos_Start[1] = (float)data->m_StartPoint.y;

                autodata.JPos_Mid[0] = (float)(data->m_CenterPoint.x);
                autodata.JPos_Mid[1] = (float)(data->m_CenterPoint.y + data->m_dRadius);

                autodata.JPos_End[0] = (float)(data->m_CenterPoint.x - data->m_dRadius);
                autodata.JPos_End[1] = (float)(data->m_CenterPoint.y);
            }
        }
    }
    autodata.Car_PVel = vel;

    if (PutAutoDataFrame(&autodata) == 1)
    {
        printFifoData(&autodata);
        return true;
    }
    else
        return false;
}

bool MotionControllerDevice::fillOneFIFO(IoMotionCode *data, bool isEnd)
{
    InputAutoData autodata;
    memset(&autodata, 0, sizeof(autodata));
    autodata.decode_id = data->m_nCodeId;
    autodata.Prog_SequNum = 0;
    autodata.Prog_LineNum = 0;
    autodata.Prog_Mark = isEnd ? 1 : 0;
    autodata.Mode = 2;
    autodata.IO_Mode = 2;
    autodata.IO_Name = data->m_nName;
    autodata.IO_Value = data->m_bValue;

    if (PutAutoDataFrame(&autodata) == 1)
    {
        printFifoData(&autodata);
        return true;
    }
    else
        return false;
}

bool MotionControllerDevice::fillOneFIFO(WaitIoMotionCode *data, bool isEnd)
{
    InputAutoData autodata;
    memset(&autodata, 0, sizeof(autodata));
    autodata.decode_id = data->m_nCodeId;
    autodata.Prog_SequNum = 0;
    autodata.Prog_LineNum = 0;
    autodata.Prog_Mark = isEnd ? 1 : 0;
    autodata.Mode = 5;
    autodata.IO_Mode = 1;
    autodata.IO_Name = data->m_nName;
    autodata.IO_Value = data->m_bValue;
    autodata.WaitTime = data->m_nWaitTime;

    if (PutAutoDataFrame(&autodata) == 1)
    {
        printFifoData(&autodata);
        return true;
    }
    else
        return false;
}

bool MotionControllerDevice::fillOneFIFO(DelayMotionCode *data, bool isEnd)
{
    if (data->m_nTime <= 0)//延时时间不能为0
        return true;

    InputAutoData autodata;
    memset(&autodata, 0, sizeof(autodata));
    autodata.decode_id = data->m_nCodeId;
    autodata.Prog_SequNum = 0;
    autodata.Prog_LineNum = 0;
    autodata.Prog_Mark = isEnd ? 1 : 0;
    autodata.Mode = 3;
    autodata.WaitTime = data->m_nTime;

    if (PutAutoDataFrame(&autodata) == 1)
    {
        printFifoData(&autodata);
        return true;
    }
    else
        return false;
}

bool MotionControllerDevice::fillOneFIFO(LaserCraftMotionCode *data, bool isEnd)
{
    InputAutoData autodata;
    memset(&autodata, 0, sizeof(autodata));
    autodata.decode_id = data->m_nCodeId;
    autodata.Prog_SequNum = 0;
    autodata.Prog_LineNum = 0;
    autodata.Prog_Mark = isEnd ? 1 : 0;
    autodata.Mode = 2;
    autodata.IO_Mode = 6;
    switch (data->m_LaserCraftMode)
    {
    case LaserCraftMotionCode::LCMC_MAP_ON:
        autodata.IO_Name = 1;
        autodata.IO_Value = 1;
        break;
    case LaserCraftMotionCode::LCMC_MAP_OFF:
        autodata.IO_Name = 1;
        autodata.IO_Value = 0;
        break;
    case LaserCraftMotionCode::LCMC_TABLE:
        autodata.IO_Name = 2;
        autodata.IO_Value = data->m_nTableNum;
        break;
    case LaserCraftMotionCode::LCMC_LASER_ON:
        autodata.IO_Name = 3;
        autodata.IO_Value = 1;
        break;
    case LaserCraftMotionCode::LCMC_LASER_OFF:
        autodata.IO_Name = 3;
        autodata.IO_Value = 0;
        break;
    case LaserCraftMotionCode::LCMC_POWER_SET:
        autodata.IO_Name = 4;
        autodata.IO_Value = data->m_nLaserPower;
        break;
    }

    if (PutAutoDataFrame(&autodata) == 1)
    {
        printFifoData(&autodata);
        return true;
    }
    else
        return false;
}

bool MotionControllerDevice::runOneMotionCode(BaseMotionCode *data)
{
    switch (data->m_Type)
    {
    case BaseMotionCode::MOTION_MOVE_TYPE:
        return fillOneFIFO((MoveMotionCode *)data);
    case BaseMotionCode::MOTION_LINE_TYPE:
        return fillOneFIFO((LineMotionCode *)data);
    case BaseMotionCode::MOTION_ARC_TYPE:
        return fillOneFIFO((ArcMotionCode *)data);
    case BaseMotionCode::MOTION_CIRCLE_TYPE:
        return fillOneFIFO((CircleMotionCode *)data);
    case BaseMotionCode::MOTION_IO_TYPE:
        return fillOneFIFO((IoMotionCode *)data);
    case BaseMotionCode::MOTION_DELAY_TYPE:
        return fillOneFIFO((DelayMotionCode *)data);
    default:
        break;
    }
    return true;
}

bool MotionControllerDevice::addDelayFIFO(int id, int time)
{
    InputAutoData autodata;
    memset(&autodata, 0, sizeof(autodata));
    autodata.decode_id = id;
    autodata.Prog_SequNum = 0;
    autodata.Prog_LineNum = 0;
    autodata.Prog_Mark = 1;
    autodata.Mode = 3;
    autodata.WaitTime = time;

    if (PutAutoDataFrame(&autodata) == 1)
    {
        printFifoData(&autodata);
        return true;
    }
    else
        return false;
}

bool MotionControllerDevice::addMoveFIFO(int id, double x, double y)
{
    InputAutoData autodata;
    memset(&autodata, 0, sizeof(autodata));
    autodata.decode_id = id;
    autodata.Prog_SequNum = 0;
    autodata.Prog_LineNum = 0;
    autodata.Prog_Mark = 1;
    autodata.Mode = 1;
    autodata.Interp_Mode = 1;
    autodata.Joint_VelPct = 100;
    autodata.JPos_End[0] = (float)x;
    autodata.JPos_End[1] = (float)y;
    //autodata.Car_PVel = 200;

    if (PutAutoDataFrame(&autodata) == 1)
    {
        printFifoData(&autodata);
        qDebug()<<"auto back"<<endl;
        return true;
    }
    else
        return false;
}

bool MotionControllerDevice::addMotionEndCode(int id)
{
    InputAutoData autodata;
    memset(&autodata, 0, sizeof(autodata));
    autodata.decode_id = id;
    autodata.Prog_SequNum = 0;
    autodata.Prog_LineNum = 0;
    autodata.Prog_Mark = 1;
    autodata.Mode = 2;
    autodata.IO_Mode = 2;
    autodata.IO_Name = 8;
    autodata.IO_Value = 1;

    if (PutAutoDataFrame(&autodata) == 1)
    {
        printFifoData(&autodata);
        return true;
    }
    else
        return false;
}

bool MotionControllerDevice::setParam(char *paramValue, int paramNum)
{
    if (PutSetParamFrame(0, 0, (paramNum - 1), (unsigned char *)paramValue) == 1)
        return true;
    else
        return false;
}

bool MotionControllerDevice::setParam(char start, char *paramValue, int paramNum)
{
    if (PutSetParamFrame(0, start, (start + paramNum - 1), (unsigned char *)paramValue) == 1)
        return true;
    else
        return false;
}

bool MotionControllerDevice::setMotionParam1(SystemParameter *para)
{
    char parmdata[76];
    FLOATCHAR floatData;
    int i = 0, n = 0;
    for (i = 0; i < 3; i++)//0~2
    {
        floatData.data = para->m_MotionParm.m_dAxisMaxAcc[i];
        parmdata[n++] = floatData.c[0];
        parmdata[n++] = floatData.c[1];
        parmdata[n++] = floatData.c[2];
        parmdata[n++] = floatData.c[3];
    }
    for (i = 0; i < 3; i++)//3~5
    {
        floatData.data = para->m_MotionParm.m_dAxisMaxVel[i];
        parmdata[n++] = floatData.c[0];
        parmdata[n++] = floatData.c[1];
        parmdata[n++] = floatData.c[2];
        parmdata[n++] = floatData.c[3];
    }
    for (i = 0; i < 3; i++)//6~8
    {
        floatData.data = para->m_MotionParm.m_dAxisMaxDec[i];
        parmdata[n++] = floatData.c[0];
        parmdata[n++] = floatData.c[1];
        parmdata[n++] = floatData.c[2];
        parmdata[n++] = floatData.c[3];
    }
    for (i = 0; i < 3; i++)//9~11
    {
        floatData.data = para->m_MotionParm.m_dAxisPLimit[i];
        parmdata[n++] = floatData.c[0];
        parmdata[n++] = floatData.c[1];
        parmdata[n++] = floatData.c[2];
        parmdata[n++] = floatData.c[3];
    }
    for (i = 0; i < 3; i++)//12~14
    {
        floatData.data = para->m_MotionParm.m_dAxisNLimit[i];
        parmdata[n++] = floatData.c[0];
        parmdata[n++] = floatData.c[1];
        parmdata[n++] = floatData.c[2];
        parmdata[n++] = floatData.c[3];
    }
    for (i = 0; i < 4; i++)//15
    {
        parmdata[n++] = 0;
    }
    for (i = 0; i < 3; i++)//16~18
    {
        floatData.data = para->m_MotionParm.m_dAxisRadio[i];
        parmdata[n++] = floatData.c[0];
        parmdata[n++] = floatData.c[1];
        parmdata[n++] = floatData.c[2];
        parmdata[n++] = floatData.c[3];
    }
    return setParam(parmdata, 19);//共19个参数
}

bool MotionControllerDevice::setMotionParam2(SystemParameter *para)
{
    char parmdata[64];
    FLOATCHAR floatData;
    INTCHAR   intData;
    int i = 0, n = 0;

    floatData.data = para->m_MotionParm.m_dAccuracy;//22
    parmdata[n++] = floatData.c[0];
    parmdata[n++] = floatData.c[1];
    parmdata[n++] = floatData.c[2];
    parmdata[n++] = floatData.c[3];

    intData.data = para->m_MotionParm.m_nDirX + para->m_MotionParm.m_nDirY * 2;//23
    parmdata[n++] = intData.c[0];
    parmdata[n++] = intData.c[1];
    parmdata[n++] = intData.c[2];
    parmdata[n++] = intData.c[3];

    for (i = 0; i < 32; i++)//24~31
    {
        parmdata[n++] = 0;
    }

    floatData.data = para->m_MotionParm.m_dAccTime;//32
    parmdata[n++] = floatData.c[0];
    parmdata[n++] = floatData.c[1];
    parmdata[n++] = floatData.c[2];
    parmdata[n++] = floatData.c[3];

    floatData.data = para->m_MotionParm.m_dMaxVel;//33
    parmdata[n++] = floatData.c[0];
    parmdata[n++] = floatData.c[1];
    parmdata[n++] = floatData.c[2];
    parmdata[n++] = floatData.c[3];

    for (i = 0; i < 4; i++)//34
    {
        parmdata[n++] = 0;
    }

    floatData.data = para->m_MotionParm.m_dMaxAcc;//35
    parmdata[n++] = floatData.c[0];
    parmdata[n++] = floatData.c[1];
    parmdata[n++] = floatData.c[2];
    parmdata[n++] = floatData.c[3];

    for (i = 0; i < 4; i++)//36
    {
        parmdata[n++] = 0;
    }

    floatData.data = para->m_MotionParm.m_dMaxDec;//37
    parmdata[n++] = floatData.c[0];
    parmdata[n++] = floatData.c[1];
    parmdata[n++] = floatData.c[2];
    parmdata[n++] = floatData.c[3];

    return setParam(22, parmdata, 16);//从第22个参数开始，共16个参数
}

bool MotionControllerDevice::setMotionParamCtl(SystemParameter *para)
{
    char parmdata[80];
    FLOATCHAR floatData;
    INTCHAR   intData;
    int i = 0, n = 0;

    for (i = 0; i < 2; i++)//40~47
    {
        intData.data = para->m_MotionParm.m_nK[i];//40,44
        parmdata[n++] = intData.c[0];
        parmdata[n++] = intData.c[1];
        parmdata[n++] = intData.c[2];
        parmdata[n++] = intData.c[3];

        floatData.data = para->m_MotionParm.m_dKp[i];//41,45
        parmdata[n++] = floatData.c[0];
        parmdata[n++] = floatData.c[1];
        parmdata[n++] = floatData.c[2];
        parmdata[n++] = floatData.c[3];

        floatData.data = para->m_MotionParm.m_dKi[i];//42,46
        parmdata[n++] = floatData.c[0];
        parmdata[n++] = floatData.c[1];
        parmdata[n++] = floatData.c[2];
        parmdata[n++] = floatData.c[3];

        floatData.data = para->m_MotionParm.m_dKd[i];//43,47
        parmdata[n++] = floatData.c[0];
        parmdata[n++] = floatData.c[1];
        parmdata[n++] = floatData.c[2];
        parmdata[n++] = floatData.c[3];
    }

    for (i = 0; i < 8; i++)//48~49
    {
        parmdata[n++] = 0;
    }

    floatData.data = para->m_MotionParm.m_dpLineMaxLen;//50
    parmdata[n++] = floatData.c[0];
    parmdata[n++] = floatData.c[1];
    parmdata[n++] = floatData.c[2];
    parmdata[n++] = floatData.c[3];

    floatData.data = para->m_MotionParm.m_dpLineMaxVel;//51
    parmdata[n++] = floatData.c[0];
    parmdata[n++] = floatData.c[1];
    parmdata[n++] = floatData.c[2];
    parmdata[n++] = floatData.c[3];

    floatData.data = para->m_MotionParm.m_dpLineMaxPer;//52
    parmdata[n++] = floatData.c[0];
    parmdata[n++] = floatData.c[1];
    parmdata[n++] = floatData.c[2];
    parmdata[n++] = floatData.c[3];

    floatData.data = para->m_MotionParm.m_dpLineMinPer;//53
    parmdata[n++] = floatData.c[0];
    parmdata[n++] = floatData.c[1];
    parmdata[n++] = floatData.c[2];
    parmdata[n++] = floatData.c[3];

    floatData.data = para->m_MotionParm.m_dpArcRadMaxLen;//54
    parmdata[n++] = floatData.c[0];
    parmdata[n++] = floatData.c[1];
    parmdata[n++] = floatData.c[2];
    parmdata[n++] = floatData.c[3];

    floatData.data = para->m_MotionParm.m_dpArcLenMaxLen;//55
    parmdata[n++] = floatData.c[0];
    parmdata[n++] = floatData.c[1];
    parmdata[n++] = floatData.c[2];
    parmdata[n++] = floatData.c[3];

    floatData.data = para->m_MotionParm.m_dpArcMaxVel;//56
    parmdata[n++] = floatData.c[0];
    parmdata[n++] = floatData.c[1];
    parmdata[n++] = floatData.c[2];
    parmdata[n++] = floatData.c[3];

    floatData.data = para->m_MotionParm.m_dpArcMaxPer;//57
    parmdata[n++] = floatData.c[0];
    parmdata[n++] = floatData.c[1];
    parmdata[n++] = floatData.c[2];
    parmdata[n++] = floatData.c[3];

    floatData.data = para->m_MotionParm.m_dpArcMinPer;//58
    parmdata[n++] = floatData.c[0];
    parmdata[n++] = floatData.c[1];
    parmdata[n++] = floatData.c[2];
    parmdata[n++] = floatData.c[3];

    floatData.data = para->m_MotionParm.m_dpArcError;//59
    parmdata[n++] = floatData.c[0];
    parmdata[n++] = floatData.c[1];
    parmdata[n++] = floatData.c[2];
    parmdata[n++] = floatData.c[3];

    return setParam(40, parmdata, 20);//从第40个参数开始，共20个参数
}

bool MotionControllerDevice::sendMsg(char param1, ErrorInfo &e)
{
    PutControlFrame(0,param1);
    return true;
}

bool MotionControllerDevice::startJogMotion(int axis, int dir, int vRate)
{
    InputTeachData jogMotion;
    jogMotion.Stop = 2;//RUN
    jogMotion.Coor = 1;
    jogMotion.Aix = axis;
    jogMotion.Vel = vRate;
    jogMotion.Dir = dir;
    jogMotion.Acc = 0;
    jogMotion.Pos = 0;

    if (PutTeachDataFrame(&jogMotion) == 1)
        return true;
    else
        return false;

}

bool MotionControllerDevice::stopJogMotion(int axis)
{
    InputTeachData jogMotion;
    jogMotion.Stop = 1;
    jogMotion.Coor = 1;
    jogMotion.Aix = axis;
    jogMotion.Vel = 0;
    jogMotion.Dir = 0;
    jogMotion.Acc = 0;
    jogMotion.Pos = 0;

    if (PutTeachDataFrame(&jogMotion) == 1)
        return true;
    else
        return false;
}

bool MotionControllerDevice::startMotion(int axis, double dis, int vRate)
{
    InputTeachData axisMotion;
    axisMotion.Stop = 2;
    axisMotion.Coor = 11;
    axisMotion.Aix = (axis<MOF)?(axis+1):0;
    axisMotion.Vel = vRate;


    axisMotion.Dir = (dis >= 0 )? 1 :-1;


    axisMotion.Acc = 0;
    axisMotion.Pos = fabs(dis);

    if (PutTeachDataFrame(&axisMotion) == 1)
        return true;
    else
        return false;
}

bool MotionControllerDevice::startWorldMotion(int axis, double dis, int vRate)
{
    InputTeachData axisMotion;
    axisMotion.Stop = 2;
    axisMotion.Coor = 12;
     axisMotion.Aix = (axis<MOF)?(axis+1):0;
    axisMotion.Vel = vRate;


    axisMotion.Dir = (dis >= 0 )? 1 :-1;


    axisMotion.Acc = 0;
    axisMotion.Pos = fabs(dis);

    if (PutTeachDataFrame(&axisMotion) == 1)
        return true;
    else
        return false;
}

bool MotionControllerDevice::stopMotion(int axis)
{
    InputTeachData axisMotion;
    axisMotion.Stop = 1;
    axisMotion.Coor = 11;
    axisMotion.Aix = axis;
    axisMotion.Vel = 0;
    axisMotion.Dir = 0;
    axisMotion.Acc = 0;
    axisMotion.Pos = 0;

    if (PutTeachDataFrame(&axisMotion) == 1)
        return true;
    else
        return false;
}

bool MotionControllerDevice::startMove(int axis, double dis, int run,int vRate)
{
    InputTeachData axisMotion;
    axisMotion.Stop = run+1;
    axisMotion.Coor = 1;
    axisMotion.Aix = (axis<MOF)?(axis+1):0;
    axisMotion.Vel = vRate;


    axisMotion.Dir = (dis >= 0 )? 1 :-1;


    axisMotion.Acc = 0;
    axisMotion.Pos = 0;//fabs(dis);

    if (PutTeachDataFrame(&axisMotion) == 1)
        return true;
    else
        return false;
}

bool MotionControllerDevice::startWorldMove(int axis, double dis, int run,int vRate)
{
    InputTeachData axisMotion;
    axisMotion.Stop = run+1;
    axisMotion.Coor = 2;
    axisMotion.Aix = (axis<MOF)?(axis+1):0;
    axisMotion.Vel = vRate;


    axisMotion.Dir = (dis >= 0 )? 1 :-1;


    axisMotion.Acc = 0;
    axisMotion.Pos = 0;//fabs(dis);

    if (PutTeachDataFrame(&axisMotion) == 1)
        return true;
    else
        return false;
}

bool MotionControllerDevice::isFifoNotFull()
{
    return m_netctrl->isFrameNotFull();
}
bool MotionControllerDevice::setMotionMode(int mode)
{
//    m_netctrl->gb_ctrl.sendtrafficctrl |= 0x4;
//    if (m_netctrl->gb_ctrl.sendtrafficctrl & 0x4)
//        return false;
    //m_netctrl->initSendQueue();
    qDebug()<<"Enter set Mode:"<<mode;
    static bool isClearQueue = false;
    if (!isClearQueue)
    {
        m_netctrl->gb_ctrl.sendtrafficctrl |= 0x4;//清空队列
        isClearQueue = true;
    }
    if ((m_netctrl->gb_ctrl.sendtrafficctrl & 0x4) == 0x4){
        qDebug()<<"set mode false"<<endl;
        return false;
    }

    m_ContrlBit.bit[0] &= (~0x2);//复位时清空Stop标志
    m_netctrl->gb_ctrl.sendinput[0] &= (~0x2);

    //memcpy(&m_OutPutIo.bit[0], &m_netctrl->gb_ctrl.recvoutput[0], 8);//同步输出IO
    memcpy(&m_pSystemParm->sys_ctrl.sendoutput[0], &m_pSystemParm->sys_ctrl.recvoutput[0], 8);
    
    if (PutControlFrame(1, mode) == 1)
    {
        qDebug()<<"set Mode true";
        isClearQueue = false;
        return true;
    }
    else
        return false;
}

bool MotionControllerDevice::clearAlarm()
{
    if (PutControlFrame(3, 1) == 1)
    {
        m_netctrl->gb_ctrl.alarmno = 0;
        m_cCurAlarmNum = 0;
        m_netctrl->m_bIsCtlAlarm = false;
        //m_cOldAlarmNum = 0;

        return true;
    }
    else
        return false;
}


bool MotionControllerDevice::setOrian(int zeroaxis)
{
    if (PutControlFrame(10, zeroaxis) == 1)
    {     
        return true;
    }
    else
        return false;
}

bool MotionControllerDevice::getParamer()
{
    FrameBody elem;

    elem.framehead = FrameHeadWord;
    elem.funcode = 5;
    elem.length = 8+2;
    elem.frameno = 0;
    elem.trafficctrl = 0;

    elem.databuf[0] = 21;
    elem.databuf[1] = 1;


    return m_netctrl->EnFrameQueue(&m_netctrl->gb_framequeue, elem);
}

bool MotionControllerDevice::setParamer()
{
    if (!PutSetParamFrame(0, pJointMaxAcc, pMotorDir,(unsigned char*)&m_pSystemParm->SystemParam[pJointMaxAcc]))
    {
        return false;
    }
    if (!PutSetParamFrame(0, pAccTime, pArcError,(unsigned char*)&m_pSystemParm->SystemParam[pAccTime]) )
    {
        return false;
    }
    /*if (!PutSetParamFrame(0, pAccTime, pLinkDimension+5,(unsigned char*)&m_pSystemParm->SystemParam[pAccTime]) )
    {
        return false;
    }*/
	
    return true;
}

bool MotionControllerDevice::setLinkParamer()
{
    
    if (!PutSetParamFrame(0, pLinkDimension, pLinkDimension+5,(unsigned char*)&m_pSystemParm->SystemParam[pLinkDimension]) )
    {
        return false;
    }
    /*if (!PutSetParamFrame(0, pAccTime, pLinkDimension+5,(unsigned char*)&m_pSystemParm->SystemParam[pAccTime]) )
    {
        return false;
    }*/
	
    return true;
}


bool MotionControllerDevice::setTools(int toolindex,int toolsetstep)
{
    if (PutControlFrame(13, toolindex,toolsetstep) == 1)
    {     
        return true;
    }
    else
        return false;
}

bool MotionControllerDevice::setcurTool(int toolindex)
{
    if (PutControlFrame(14, toolindex) == 1)
    {     
        m_pSystemParm->m_currentTool=toolindex;
        return true;
    }
    else
        return false;
}


bool MotionControllerDevice::setZeroCalibration(int step)
{
    if (PutControlFrame(16, step) == 1)
    {     
        return true;
    }
    else
        return false;
}

bool MotionControllerDevice::axisEnable(bool isEnable)
{
    int enable = isEnable ? 1 : 0;
    if (PutControlFrame(4, enable) == 1)
        return true;
    else
        return false;
}

bool MotionControllerDevice::getVersion()
{
    
    if (PutControlFrame(15, 1) == 1)
        return true;
    else
        return false;
}

bool MotionControllerDevice::getBattery()
{
    
    FrameBody elem;

    elem.framehead = FrameHeadWord;
    elem.funcode = 5;
    elem.length = 8+2;
    elem.frameno = 0;
    elem.trafficctrl = 0;

    elem.databuf[0] = 17;
    elem.databuf[1] = 1;


    return m_netctrl->EnFrameQueue(&m_netctrl->gb_framequeue, elem);
}

bool MotionControllerDevice::setBattery()
{
    
    FrameBody elem;

    elem.framehead = FrameHeadWord;
    elem.funcode = 5;
    elem.length = 8+4;
    elem.frameno = 0;
    elem.trafficctrl = 0;

    elem.databuf[0] = 17;
    elem.databuf[1] = 2;
    elem.databuf[2] = m_pSystemParm->m_setbattery[0];
    elem.databuf[3] = m_pSystemParm->m_setbattery[1];

    if(0==m_netctrl->EnFrameQueue(&m_netctrl->gb_framequeue, elem)) return false;

     elem.databuf[0] = 17;
    elem.databuf[1] = 3;
    elem.databuf[2] = m_pSystemParm->m_setbattery[2];
    elem.databuf[3] = m_pSystemParm->m_setbattery[3];

    return m_netctrl->EnFrameQueue(&m_netctrl->gb_framequeue, elem);
   
}

bool MotionControllerDevice::setAxisNo()
{
    
    FrameBody elem;

    elem.framehead = FrameHeadWord;
    elem.funcode = 5;
    elem.length = 8+2+64;
    elem.frameno = 0;
    elem.trafficctrl = 0;

    elem.databuf[0] = 20;
    elem.databuf[1] = 1;
     memcpy(&elem.databuf[2],&m_pSystemParm->axisno[0],64);

  

    return m_netctrl->EnFrameQueue(&m_netctrl->gb_framequeue, elem);
   
}

bool MotionControllerDevice::getAxisNo()
{
    
    FrameBody elem;

    elem.framehead = FrameHeadWord;
    elem.funcode = 5;
    elem.length = 8+2;
    elem.frameno = 0;
    elem.trafficctrl = 0;

    elem.databuf[0] = 20;
    elem.databuf[1] = 2;   


    return m_netctrl->EnFrameQueue(&m_netctrl->gb_framequeue, elem);
   
}

int MotionControllerDevice::PutTeachPoint(unsigned short start,unsigned short end,unsigned char *paramdata)
{
    FrameBody elem;

    elem.framehead=FrameHeadWord;
    elem.funcode=5;
    elem.length=(end-start+1)*TEACHPIONTSIZE+14;
    elem.frameno=0;
    elem.trafficctrl=0;

    elem.databuf[0] = 18;
    elem.databuf[1] = 1;
    elem.databuf[2]=(unsigned char)(start&0xff);
    elem.databuf[3]=(unsigned char)((start>>8)&0xff);	
    elem.databuf[4]=(unsigned char)(end&0xff);//end;
    elem.databuf[5]=(unsigned char)((end>>8)&0xff);
    memcpy(&elem.databuf[6],paramdata,(end-start+1)*TEACHPIONTSIZE);

    return m_netctrl->EnFrameQueue(&m_netctrl->gb_framequeue, elem);
}

int MotionControllerDevice::PutTeachPointText(unsigned short start,unsigned short end,unsigned char *paramdata)
{
    FrameBody elem;

    elem.framehead=FrameHeadWord;
    elem.funcode=5;
    elem.length=(end-start+1)*128+14;
    elem.frameno=0;
    elem.trafficctrl=0;

    elem.databuf[0] = 18;
    elem.databuf[1] = 4;
    elem.databuf[2]=(unsigned char)(start&0xff);
    elem.databuf[3]=(unsigned char)((start>>8)&0xff);	
    elem.databuf[4]=(unsigned char)(end&0xff);//end;
    elem.databuf[5]=(unsigned char)((end>>8)&0xff);
    memcpy(&elem.databuf[6],paramdata,(end-start+1)*128);

    return m_netctrl->EnFrameQueue(&m_netctrl->gb_framequeue, elem);
}


int MotionControllerDevice::PutSaveTeachPointText()
{
    FrameBody elem;

    elem.framehead=FrameHeadWord;
    elem.funcode=5;
    elem.length=10;
    elem.frameno=0;
    elem.trafficctrl=0;

    elem.databuf[0] = 18;
    elem.databuf[1] = 5;//write to file cmd
    

    return m_netctrl->EnFrameQueue(&m_netctrl->gb_framequeue, elem);
}


int MotionControllerDevice::PutSaveTeachPoint()
{
    FrameBody elem;

    elem.framehead=FrameHeadWord;
    elem.funcode=5;
    elem.length=10;
    elem.frameno=0;
    elem.trafficctrl=0;

    elem.databuf[0] = 18;
    elem.databuf[1] = 3;//write to file cmd
    

    return m_netctrl->EnFrameQueue(&m_netctrl->gb_framequeue, elem);
}
bool MotionControllerDevice::saveTeachPoint()
{
    int i=0;
    for(i=0;i<(TEACHPIONTNUM/6)*6;i+=6)
    {
	    if (!PutTeachPoint(i, i+5, (unsigned char*)&m_pSystemParm->teachpoint[i]))
	    {
	        return i;
	    }
    }
    if (!PutTeachPoint((TEACHPIONTNUM/6)*6,TEACHPIONTNUM-1, (unsigned char*)&m_pSystemParm->teachpoint[(TEACHPIONTNUM/6)*6]))
    {
        return (TEACHPIONTNUM/6)*6;
    }

   if (!PutSaveTeachPoint())
    {
        return false;
    }
}

bool MotionControllerDevice::getTeachPoint()
{
    FrameBody elem;

    elem.framehead=FrameHeadWord;
    elem.funcode=5;
    elem.length=10;
    elem.frameno=0;
    elem.trafficctrl=0;

    elem.databuf[0] = 18;
    elem.databuf[1] = 2;

    return m_netctrl->EnFrameQueue(&m_netctrl->gb_framequeue, elem);
}

int MotionControllerDevice::saveUserFile(int openflag,unsigned short start,unsigned short end)
{
    FrameBody elem;
	int flag=0;
	int num=128;
	char buf[128];
    if(m_netctrl->FullQueue(&m_netctrl->gb_framequeue))
    {
          return 0;
    }
	if(start==0)
	{
		flag=1;
		m_userfile=new QFile(m_pSystemParm->m_userfilePath);
		
		if (!m_userfile->open(QIODevice::ReadOnly))
		{
		       delete m_userfile;
			m_userfile = nullptr;
			flag=0;
		        return -1;
		}
	}
	
      if(m_userfile != nullptr)
     {
     	
        if((num=m_userfile->read((char *)buf,128))<128)   
        {
        	flag=3;
        	m_userfile->close();
		delete m_userfile;
		m_userfile = nullptr;
        }
	 else if(start!=0)
 	{
 	      flag=2; 
 	}
    }
    elem.framehead=FrameHeadWord;
    elem.funcode=5;
    elem.length=14+num;
    elem.frameno=0;
    elem.trafficctrl=0;

    elem.databuf[0] = 19;
    elem.databuf[1] =(unsigned char)flag;
    elem.databuf[2]=(unsigned char)(start&0xff);
    elem.databuf[3]=(unsigned char)((start>>8)&0xff);	
    elem.databuf[4]=(unsigned char)(num&0xff);//end;
    elem.databuf[5]=(unsigned char)((num>>8)&0xff);
    memcpy(&elem.databuf[6],buf,num);
    m_netctrl->EnFrameQueue(&m_netctrl->gb_framequeue, elem);
    return flag;
}
bool MotionControllerDevice::controllerReset()
{
    static bool isClear = false;
    //if (!isClear)
    {
        m_netctrl->gb_ctrl.sendtrafficctrl |= 0x4;//清空队列
        //isClear = true;
    }
    return true;
    /*if ((m_netctrl->gb_ctrl.sendtrafficctrl & 0x4) == 0x4)
        return false;

    if (m_netctrl->gb_ctrl.succedrecvno != 0)//保证队列清空
        return false;

    if (PutControlFrame(5, 1) == 1)
    {
        isClear = false;
        m_netctrl->gb_ctrl.sendinput[0] &= (~0x3);//清除hold和stop标志位

        m_netctrl->gb_ctrl.alarmno = 0;//清除报警号
        m_cCurAlarmNum = 0;
        m_netctrl->m_bIsCtlAlarm = false;
        //m_cOldAlarmNum = 0;
        return true;
    }
    else
        return false;*/
}

bool MotionControllerDevice::emsStop(bool isStop)
{
    int sts = isStop ? 1 : 0;
    if (PutControlFrame(11, sts) == 1)
        return true;
    else
        return false;
}

bool MotionControllerDevice::getCtlVersion()
{
    if (PutControlFrame(15, 1) == 1)
        return true;
    else
        return false;
}

bool MotionControllerDevice::machineLock(bool isLock)
{
    int sts = isLock ? 1 : 0;
    if (PutControlFrame(6, sts) == 1)
        return true;
    else
        return false;
}

bool MotionControllerDevice::motionSpeedRate(int rate)
{
    char r = rate > 100 ? 100 : (rate < 0 ? 0 : (char)rate);

    m_ContrlBit.bit[1] = r;
    PutPeriodFrame(m_ContrlBit.bit, m_OutPutIo.bit);
    return true;

//    if (PutControlFrame(7, r) == 1)
//        return true;
//    else
//        return false;
}

bool MotionControllerDevice::holdMotion(bool ishold)
{
    if (ishold)
        m_ContrlBit.bit[0] |= 0x1;
    else
        m_ContrlBit.bit[0] &= 0xFE;
    //qDebug()<<"contrlBit = "<<m_ContrlBit.bit[0];

    PutPeriodFrame(m_ContrlBit.bit, m_OutPutIo.bit);
    return true;

//    int sts = ishold ? 1 : 0;
//    if (PutControlFrame(8, sts) == 1)
//        return true;
//    else
//        return false;
}

bool MotionControllerDevice::stopFifoMotion()
{
    m_ContrlBit.bit[0] |= 0x2;
    PutPeriodFrame(m_ContrlBit.bit, m_OutPutIo.bit);

    //qDebug()<<"stop"<<m_ContrlBit.bit[0];
    return true;

//    if (PutControlFrame(2, 1) == 1)
//        return true;
//    else
//        return false;
}

bool MotionControllerDevice::setOutputIo(int io, bool value)
{
    if (io <= CTL_IO_ZAXIS_FOLLOW)
    {
        if (value)
            m_OutPutIo.bit[0] |= (0x1 << io);
        else
            m_OutPutIo.bit[0] &= (~(0x1 << io));

        //调高器互斥处理
//        if (io == CTL_IO_ZAXIS_FOLLOW && value)
//        {
//            m_OutPutIo.bit[0] &= (~(0x1 << CTL_IO_ZAXIS_UP));
//            m_OutPutIo.bit[0] &= (~(0x1 << CTL_IO_ZAXIS_MIDDLE));
//        }
//        else if (io == CTL_IO_ZAXIS_UP && value)
//        {
//            m_OutPutIo.bit[0] &= (~(0x1 << CTL_IO_ZAXIS_FOLLOW));
//            m_OutPutIo.bit[0] &= (~(0x1 << CTL_IO_ZAXIS_MIDDLE));
//        }
//        else if (io == CTL_IO_ZAXIS_MIDDLE && value)
//        {
//            m_OutPutIo.bit[0] &= (~(0x1 << CTL_IO_ZAXIS_UP));
//            m_OutPutIo.bit[0] &= (~(0x1 << CTL_IO_ZAXIS_FOLLOW));
//        }

        //qDebug()<<"OutPutIo = "<<m_OutPutIo.bit[0];
        memcpy(&m_oldRecvOutputIo[0], &m_OutPutIo.bit[0], 8);//用于界面同步IO

        PutPeriodFrame(m_ContrlBit.bit, m_OutPutIo.bit);
        return true;
    }
//    else//激光的控制控制帧设置（控制器串口）
//    {
//        if (io == CTL_IO_SP_LASER)
//        {
//            if (PutControlFrame(13, (value ? 1 : 0)) == 1)
//            {
//                //同时控制PWM开关
//                if (value)
//                    m_OutPutIo.bit[0] |= (0x1 << CTL_IO_PWM);
//                else
//                    m_OutPutIo.bit[0] &= (~(0x1 << CTL_IO_PWM));

//                PutPeriodFrame(m_ContrlBit.bit, m_OutPutIo.bit);

//                return true;
//            }
//            else
//                return false;
//        }
//    }

    return true;
}


int MotionControllerDevice::PutAutoDataFrame(InputAutoData *autodata)
{
    FrameBody elem;

    elem.framehead=FrameHeadWord;
    elem.funcode=1;
    elem.length=172+48;//116;
    elem.frameno=0;
    elem.trafficctrl=0;

    memcpy(&elem.databuf[0], autodata, sizeof(InputAutoData));

    return m_netctrl->EnFrameQueue(&m_netctrl->gb_framequeue, elem);
}

//轴运动数据下发
int MotionControllerDevice::PutTeachDataFrame(InputTeachData *teachdata)
{
    FrameBody elem;

    elem.framehead=FrameHeadWord;
    elem.funcode=2;
    elem.length=40;
    elem.frameno=0;
    elem.trafficctrl=0;
    memcpy(&elem.databuf[0],teachdata,sizeof(InputTeachData));

    return m_netctrl->EnFrameQueue(&m_netctrl->gb_framequeue, elem);
}

//周期性数据（下发）
int MotionControllerDevice::PutPeriodFrame(unsigned char *control,unsigned char *outputio)
{
    memcpy(&m_netctrl->gb_ctrl.sendinput[0],control,8);
    memcpy(&m_netctrl->gb_ctrl.sendoutput[0],outputio,8);

    return 1;
}

//控制帧
int MotionControllerDevice::PutControlFrame(unsigned char control,unsigned char value)
{
    FrameBody elem;

    elem.framehead = FrameHeadWord;
    elem.funcode = 5;
    elem.length = 10;
    elem.frameno = 0;
    elem.trafficctrl = 0;

    elem.databuf[0] = control;
    elem.databuf[1] = value;


    return m_netctrl->EnFrameQueue(&m_netctrl->gb_framequeue, elem);
}

int MotionControllerDevice::PutControlFrame(unsigned char control,unsigned char value,unsigned char value1)
{
    FrameBody elem;

    elem.framehead = FrameHeadWord;
    elem.funcode = 5;
    elem.length = 12;
    elem.frameno = 0;
    elem.trafficctrl = 0;

    elem.databuf[0] = control;
    elem.databuf[1] = value;
    elem.databuf[2] = value1;
    elem.databuf[3] = 0;
    return m_netctrl->EnFrameQueue(&m_netctrl->gb_framequeue, elem);
}

//参数设置
int MotionControllerDevice::PutSetParamFrame(unsigned char ifupdown,unsigned char start,unsigned char end,unsigned char *paramdata)
{
    FrameBody elem;

    elem.framehead=FrameHeadWord;
    elem.funcode=7;
    elem.length=(end-start+1)*4+10;
    elem.frameno=0;
    elem.trafficctrl=ifupdown;

    elem.databuf[0]=start;
    elem.databuf[1]=end;
    memcpy(&elem.databuf[2],paramdata,(end-start+1)*4);

    return m_netctrl->EnFrameQueue(&m_netctrl->gb_framequeue, elem);
}


void MotionControllerDevice::printFifoData(InputAutoData * autodata)
{
#ifdef _CNC_NET_FIFO_RECORD_DEBUG
    QFile recordFifoFile("sendFifo.csv");
    if (recordFifoFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
    {
        QTextStream in(&recordFifoFile);
        in << QString("%1,%2,%3,%4,%5,%6,")
              .arg(autodata->decode_id)
              .arg(autodata->Prog_Mark)
              .arg(autodata->Mode)
              .arg(autodata->Interp_Mode)
              .arg(autodata->Joint_VelPct)
              .arg(autodata->PathSN);
        in << QString("%1,%2,%3,%4,%5,%6,")
              .arg(autodata->JPos_Start[0])
              .arg(autodata->JPos_Start[1])
              .arg(autodata->JPos_Mid[0])
              .arg(autodata->JPos_Mid[1])
              .arg(autodata->JPos_End[0])
              .arg(autodata->JPos_End[1]);
        in << QString("%1,%2,%3,%4,%5,%6\n")
              .arg(autodata->Car_PVel)
              .arg(autodata->Zone)
              .arg(autodata->IO_Mode)
              .arg(autodata->IO_Name)
              .arg(autodata->IO_Value)
              .arg(autodata->WaitTime);
        recordFifoFile.close();
    }
#endif
}


void MotionControllerDevice::onNetCtrlTimeout()
{
    m_netctrl->ontimeout();
    memcpy(&m_pSystemParm->coor_car_pos[0],&m_netctrl->gb_ctrl.realpos[0],32);
    memcpy(&m_pSystemParm->coor_joint_pos[0],&m_netctrl->gb_ctrl.apos,32);
    memcpy(&m_pSystemParm->realaxis[0],&m_netctrl->gb_ctrl.realaxis[0],64);
    memcpy(&m_pSystemParm->plusepos[0],&m_netctrl->gb_ctrl.plusepos[0],64);
    
    if(m_netctrl->m_bIsVersionGet)
    {
    		memcpy(&m_pSystemParm->m_cVersion[0],&m_netctrl->m_cVersion[0],13);
		m_netctrl->m_bIsVersionGet=0;
    }
    m_pSystemParm->sys_ctrl.sendframeno=m_netctrl->gb_ctrl.sendframeno;
    m_pSystemParm->sys_ctrl.recvframeno=m_netctrl->gb_ctrl.recvframeno;
    m_pSystemParm->sys_ctrl.succedrecvno=m_netctrl->gb_ctrl.succedrecvno;
  		
    //memcpy(&m_pSystemParm->sys_ctrl.recvinput[0],&m_netctrl->gb_ctrl.recvinput[0],8);
    //memcpy(&m_pSystemParm->sys_ctrl.recvoutput[0],&m_netctrl->gb_ctrl.recvoutput[0],8);
    //memcpy(&m_netctrl->gb_ctrl.recvinput[0],&m_pSystemParm->recvinput[0],8);
    //memcpy(&m_netctrl->gb_ctrl.sendoutput[0],&m_pSystemParm->sys_ctrl.sendoutput[0],8);

    /*m_CurPos.x = m_netctrl->gb_ctrl.realpos[0];
    m_CurPos.y = m_netctrl->gb_ctrl.realpos[1];
    m_nMontionCtlState = m_netctrl->gb_ctrl.statebit;
    m_nCurCtlRunId = m_netctrl->gb_ctrl.decode_id;
    //if (!m_CurPos.isEqual(m_OldPos))
//    if (fabs(m_CurPos.x - m_OldPos.x) >= 0.001 || fabs(m_CurPos.y - m_OldPos.y) >= 0.001)
//    {
//        m_OldPos = m_CurPos;
//        ((MotionControllerModule *)m_pModuleHandle)->setCurPos(m_CurPos);
//    }

    if (m_nOldMontionCtlSts != m_nMontionCtlState)
    {
        m_nOldMontionCtlSts = m_nMontionCtlState;
        ((MotionControllerModule *)m_pModuleHandle)->setMotionCtlSts(m_nMontionCtlState);

//        if ((m_nMontionCtlState & CTL_STS_IN_MOTION) == 0)//恢复停止标志位
//        {
//            m_ContrlBit.bit[0] &= (~0x2);
//            m_netctrl->gb_ctrl.sendinput[0] &= (~0x2);
//            qDebug()<<"stop done"<<m_ContrlBit.bit[0];
//        }
    }
    if ((m_nMontionCtlState & CTL_STS_IN_MOTION) == 0)//恢复停止标志位
    {
        m_ContrlBit.bit[0] &= (~0x2);
        m_netctrl->gb_ctrl.sendinput[0] &= (~0x2);
        //qDebug()<<"stop done"<<m_ContrlBit.bit[0];
    }

    if (m_nOldCtlRunId != m_nCurCtlRunId)
    {
        m_nOldCtlRunId = m_nCurCtlRunId;
        ((MotionControllerModule *)m_pModuleHandle)->setCurCtlRunId(m_nCurCtlRunId);
    }

    //报警号
//    m_cCurAlarmNum = m_netctrl->gb_ctrl.alarmno;
//    if (m_cCurAlarmNum != m_cOldAlarmNum)
    if (m_netctrl->m_bIsCtlAlarm)
    {
        //m_cOldAlarmNum = m_cCurAlarmNum;
        m_cCurAlarmNum = m_netctrl->gb_ctrl.alarmno;
        m_netctrl->m_bIsCtlAlarm = false;
        ((MotionControllerModule *)m_pModuleHandle)->setCtlAlarmNum(m_cCurAlarmNum);
    }

    if (m_netctrl->m_bIsVersionGet)
    {
        QString version = QString::fromLocal8Bit(m_netctrl->m_cVersion);
        m_pSystemParm->setCtlVersion(version);
        m_netctrl->m_bIsVersionGet = false;
    }

    if (m_netctrl->m_bIsFollowValueGet)
    {
        ((MotionControllerModule *)m_pModuleHandle)->setFollowValue(m_netctrl->m_fFollowValueRadio, m_netctrl->m_fFollowValueHigh);
        m_netctrl->m_bIsFollowValueGet = false;
    }

    memcpy(&m_curRecvInputIo[0], &m_netctrl->gb_ctrl.recvinput[0], 8);
    memcpy(&m_curRecvOutputIo[0], &m_netctrl->gb_ctrl.recvoutput[0], 8);
    if (m_curRecvInputIo[0] != m_oldRecvInputIo[0] || m_curRecvOutputIo[0] != m_oldRecvOutputIo[0])
    {
        memcpy(&m_oldRecvInputIo[0], &m_curRecvInputIo[0], 8);
        memcpy(&m_oldRecvOutputIo[0], &m_curRecvOutputIo[0], 8);
        ((MotionControllerModule *)m_pModuleHandle)->setCtlIoSts(m_curRecvInputIo, m_curRecvOutputIo);
    }

    m_dCurY1Pos = m_netctrl->gb_ctrl.y1pos;
    if (fabs(m_dCurY1Pos - m_dOldY1Pos) >= 0.001)
    {
        m_dOldY1Pos = m_dCurY1Pos;
        ((MotionControllerModule *)m_pModuleHandle)->setY1EncoderPos(m_dCurY1Pos);
    }

    m_dCurY2Pos = m_netctrl->gb_ctrl.y2pos;
    if (fabs(m_dCurY2Pos - m_dOldY2Pos) >= 0.001)
    {
        m_dOldY2Pos = m_dCurY2Pos;
        ((MotionControllerModule *)m_pModuleHandle)->setY2EncoderPos(m_dCurY2Pos);
    }

    m_dCurDelYPos = m_netctrl->gb_ctrl.delypos;
    if (fabs(m_dCurDelYPos - m_dOldDelYPos) >= 0.001)
    {
        m_dOldDelYPos = m_dCurDelYPos;
        ((MotionControllerModule *)m_pModuleHandle)->setDelYEncoderPos(m_dCurDelYPos);
    }

    m_dCurVel = m_netctrl->gb_ctrl.vel;
    if (fabs(m_dCurVel - m_dOldVel) >= 0.001)
    {
        m_dOldVel = m_dCurVel;
        ((MotionControllerModule *)m_pModuleHandle)->setCurVel(m_dCurVel);
    }

    //qDebug()<<"ctl"<<m_ContrlBit.bit[0];

#ifdef _CNC_AXIS_Y_DELTA_RECORD_DEBUG
    QFile recordFifoFile("encoderY.csv");
    if (m_nRecordEncoderAxisYNum > 50000)
    {
//        if (recordFifoFile.open(QIODevice::WriteOnly | QIODevice::Text))
//        {
//            QTextStream in(&recordFifoFile);
//            in << "y1,y2,deltaY\n";
//            in << QString("%1,%2,%3\n").arg(m_dCurY1Pos).arg(m_dCurY2Pos).arg(m_dCurY1Pos - m_dCurY2Pos);
//            recordFifoFile.close();
//            m_nRecordEncoderAxisYNum = 0;
//        }
        initPrintEncoderAxisY();
    }
    else
    {
        if (recordFifoFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
        {
            QTextStream in(&recordFifoFile);
            in << QString("%1,%2,%3\n").arg(m_dCurY1Pos).arg(m_dCurY2Pos).arg(m_dCurY1Pos - m_dCurY2Pos);
            recordFifoFile.close();
            m_nRecordEncoderAxisYNum++;
        }
    }
#endif*/
}


