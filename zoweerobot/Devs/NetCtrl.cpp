#pragma execution_character_set("utf-8")
//#include "GlobalDefine.h"
#include "NetCtrl.h"
#include "NetSocket.h"
#include "MotionControllerDevice.h"

#include <string.h>
#include <stdio.h>
#ifdef  PROGRAM_USELVDS
#define SINGLECIRCLE 20//17//
#define SINGLECIRCLEMASK 0xfffff//0x1ffff//
#define MUlTICIRCLEMASK 0xffff//0x7fff//
#else
#define SINGLECIRCLE 17//
#define SINGLECIRCLEMASK 0x1ffff//
#define MUlTICIRCLEMASK 0x7fff//
#endif
static const unsigned short crc16tab[256]= {
 0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,
 0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef,
 0x1231,0x0210,0x3273,0x2252,0x52b5,0x4294,0x72f7,0x62d6,
 0x9339,0x8318,0xb37b,0xa35a,0xd3bd,0xc39c,0xf3ff,0xe3de,
 0x2462,0x3443,0x0420,0x1401,0x64e6,0x74c7,0x44a4,0x5485,
 0xa56a,0xb54b,0x8528,0x9509,0xe5ee,0xf5cf,0xc5ac,0xd58d,
 0x3653,0x2672,0x1611,0x0630,0x76d7,0x66f6,0x5695,0x46b4,
 0xb75b,0xa77a,0x9719,0x8738,0xf7df,0xe7fe,0xd79d,0xc7bc,
 0x48c4,0x58e5,0x6886,0x78a7,0x0840,0x1861,0x2802,0x3823,
 0xc9cc,0xd9ed,0xe98e,0xf9af,0x8948,0x9969,0xa90a,0xb92b,
 0x5af5,0x4ad4,0x7ab7,0x6a96,0x1a71,0x0a50,0x3a33,0x2a12,
 0xdbfd,0xcbdc,0xfbbf,0xeb9e,0x9b79,0x8b58,0xbb3b,0xab1a,
 0x6ca6,0x7c87,0x4ce4,0x5cc5,0x2c22,0x3c03,0x0c60,0x1c41,
 0xedae,0xfd8f,0xcdec,0xddcd,0xad2a,0xbd0b,0x8d68,0x9d49,
 0x7e97,0x6eb6,0x5ed5,0x4ef4,0x3e13,0x2e32,0x1e51,0x0e70,
 0xff9f,0xefbe,0xdfdd,0xcffc,0xbf1b,0xaf3a,0x9f59,0x8f78,
 0x9188,0x81a9,0xb1ca,0xa1eb,0xd10c,0xc12d,0xf14e,0xe16f,
 0x1080,0x00a1,0x30c2,0x20e3,0x5004,0x4025,0x7046,0x6067,
 0x83b9,0x9398,0xa3fb,0xb3da,0xc33d,0xd31c,0xe37f,0xf35e,
 0x02b1,0x1290,0x22f3,0x32d2,0x4235,0x5214,0x6277,0x7256,
 0xb5ea,0xa5cb,0x95a8,0x8589,0xf56e,0xe54f,0xd52c,0xc50d,
 0x34e2,0x24c3,0x14a0,0x0481,0x7466,0x6447,0x5424,0x4405,
 0xa7db,0xb7fa,0x8799,0x97b8,0xe75f,0xf77e,0xc71d,0xd73c,
 0x26d3,0x36f2,0x0691,0x16b0,0x6657,0x7676,0x4615,0x5634,
 0xd94c,0xc96d,0xf90e,0xe92f,0x99c8,0x89e9,0xb98a,0xa9ab,
 0x5844,0x4865,0x7806,0x6827,0x18c0,0x08e1,0x3882,0x28a3,
 0xcb7d,0xdb5c,0xeb3f,0xfb1e,0x8bf9,0x9bd8,0xabbb,0xbb9a,
 0x4a75,0x5a54,0x6a37,0x7a16,0x0af1,0x1ad0,0x2ab3,0x3a92,
 0xfd2e,0xed0f,0xdd6c,0xcd4d,0xbdaa,0xad8b,0x9de8,0x8dc9,
 0x7c26,0x6c07,0x5c64,0x4c45,0x3ca2,0x2c83,0x1ce0,0x0cc1,
 0xef1f,0xff3e,0xcf5d,0xdf7c,0xaf9b,0xbfba,0x8fd9,0x9ff8,
 0x6e17,0x7e36,0x4e55,0x5e74,0x2e93,0x3eb2,0x0ed1,0x1ef0
};
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNetCtrl::CNetCtrl(MotionControllerDevice * pDlg)
{
    cDlg = pDlg;
    memset(&gb_ctrl,0,20);
    InitCommuncate();
    gb_ctrl.sendtrafficctrl |= 0x4;
    m_bIsCtlAlarm = false;
    m_bIsVersionGet = false;
    memset(m_cVersion, '\0', 13);
    memset(&gb_ctrl.alarm, 0, sizeof(gb_ctrl.alarm));
    m_bIsFollowValueGet = false;
    m_nFollowValueSize = 0;

	m_RevCount = 0;
	m_IsConn = false;
}

CNetCtrl::~CNetCtrl()
{

}

/*unsigned int CNetCtrl::calcrc(const char *buf, int len)
{
    register int counter;
    register unsigned short crc = 0;
    for( counter = 0; counter < len; counter++)
        crc = (crc<<8) ^ crc16tab[((crc>>8) ^ *(char *)buf++)&0x00FF];
    return crc;
}*/

int CNetCtrl::calcrc(unsigned char *ptr, int count)
{
    int crc, i;
	crc = 0;
    while(--count >= 0) {
	crc = crc ^ (int)*ptr++ << 8;
	for(i = 0; i < 8; ++i)
	    if(crc & 0x8000)
		crc = crc << 1 ^ 0x1021;
	    else
		crc = crc << 1;
	}
    return (crc & 0xFFFF);
}


int CNetCtrl::InitFrameQueue(Framequeue *q, U16 size)
{
    // 检查size是否有效，若无效则退出运行
    if(size <= 0)
	{
		return 0;
	}

    // 置队列空间大小为size
	q->maxsize= size + 1;

	// 动态存储空间分配，若失败则退出运行
	q->queue = &Frame[0];//malloc((size+1) * sizeof(ElemType));

	if(!q->queue)
	{
		return 0;
	}
    // 初始置队列为空
	q->front = q->rear = 0;

    return 1;
}

int CNetCtrl::InitHaveSendFrameQueue(Framequeue *q, U16 size)
{
    // 检查size是否有效，若无效则退出运行
    if(size <= 0)
	{
		return 0;
	}

    // 置队列空间大小为size
	q->maxsize= size + 1;

	// 动态存储空间分配，若失败则退出运行
	q->queue = &HaveSendFrame[0];//malloc((size+1) * sizeof(ElemType));

	if(!q->queue)
	{
		return 0;
	}
    // 初始置队列为空
	q->front = q->rear = 0;

    return 1;
}

int CNetCtrl::InitPulseFrameQueue(Framequeue *q, U16 size)
{
    // 检查size是否有效，若无效则退出运行
    if(size <= 0)
	{
		return 0;
	}

    // 置队列空间大小为size
	q->maxsize= size + 1;

	// 动态存储空间分配，若失败则退出运行
	q->queue = &PulseFrame[0];//malloc((size+1) * sizeof(ElemType));

	if(!q->queue)
	{
		return 0;
	}
    // 初始置队列为空
	q->front = q->rear = 0;

    return 1;
}

// 2.向队列中插入元素e
int CNetCtrl::EnFrameQueue(Framequeue *q, FrameBody e)
{
    // 当队列满时,不能插入
     if(!q->queue)
	{
		return 0;
	}
    if((q->rear + 1) % q->maxsize == q->front)
	{
		return 0;
	}

    // 求出队尾的下一个位置
	//q->Rear = (q->Rear + 1) % q->MaxSize;
	// 把x的值赋给新队尾	
	q->queue[q->rear] = e;  
	q->rear = (q->rear + 1) % q->maxsize;

    return 1;
}

// 3.从队列中删除元素并返回
 int CNetCtrl::DeFrameQueue(Framequeue *q, FrameBody *e)
{
	 if(!q->queue)
	{
		return 0;
	}
	// 若队列为空则删除失败
       if(q->front == q->rear)
	{
		return 0;
	}
	
	// 使队首指针指向下一个位置
	//q->Front = (q->Front + 1) % q->MaxSize;
	                
	// 返回队首元素
	*e = q->queue[q->front]; 

	q->front = (q->front + 1) % q->maxsize;
	

	return 1;            
}

// 2.向队列中插入元素e
int CNetCtrl::EnHaveSendFrameQueue(Framequeue *q, FrameBody e)
{
    // 当队列满时,不能插入
     if(!q->queue)
	{
		return 0;
	}
    if((q->rear + 1) % q->maxsize == q->front)
	{
		return 0;
	}

    // 求出队尾的下一个位置
	//q->Rear = (q->Rear + 1) % q->MaxSize;
	// 把x的值赋给新队尾	
	q->queue[q->rear] = e;  
	q->rear = (q->rear + 1) % q->maxsize;

    return 1;
}

// 3.从队列中删除元素并返回
 int CNetCtrl::DeHaveSendFrameQueue(Framequeue *q, FrameBody *e)
{
	 if(!q->queue)
	{
		return 0;
	}
	// 若队列为空则删除失败
       if(q->front == q->rear)
	{
		return 0;
	}
	
	// 使队首指针指向下一个位置
	//q->Front = (q->Front + 1) % q->MaxSize;
	                
	// 返回队首元素
	*e = q->queue[q->front]; 

	q->front = (q->front + 1) % q->maxsize;
	

	return 1;            
}

// 4.读取队首元素，不改变队列状态
int CNetCtrl::PeekQueue(Framequeue *q, FrameBody *e)
{
    // 若队列为空则读取失败
	if(q->front == q->rear)
	{
		return 0;
	}
	
	// 队首元素是队首指针的下一个位置中的元素
	*e = q->queue[q->front];

	return 1;
}

// 5.检查一个队列是否为空，若是则返回1,否则返回0
int CNetCtrl::EmptyQueue(Framequeue *q)
{
    if(q->front == q->rear)
	{
		return 1;
	}
	else
	{
        return 0;     
	}
}

// 6. 返回队列的长度,元素个数
int CNetCtrl::QueueLen(Framequeue *q)
{
	return (q->rear - q->front + q->maxsize) % q->maxsize;
}

// 7.检查一个队列是否为满
int CNetCtrl::FullQueue(Framequeue *q)
{
    // 当队列满时,不能插入
    if((q->rear + 1) % q->maxsize == q->front)
	{
		return 1;
	}
	else return 0;
}

int CNetCtrl::EnPulseFrameQueue(Framequeue *q, FrameBody e)
{
    // 当队列满时,不能插入
     if(!q->queue)
	{
		return 0;
	}
    if((q->rear + 1) % q->maxsize == q->front)
	{
		return 0;
	}

    // 求出队尾的下一个位置
	//q->Rear = (q->Rear + 1) % q->MaxSize;
	// 把x的值赋给新队尾	
	q->queue[q->rear] = e;  
	q->rear = (q->rear + 1) % q->maxsize;

    return 1;
}

// 3.从队列中删除元素并返回
 int CNetCtrl::DePulseQueue(Framequeue *q, FrameBody *e)
{
	 if(!q->queue)
	{
		return 0;
	}
	// 若队列为空则删除失败
       if(q->front == q->rear)
	{
		return 0;
	}
	
	// 使队首指针指向下一个位置
	//q->Front = (q->Front + 1) % q->MaxSize;
	                
	// 返回队首元素
	*e = q->queue[q->front]; 

	q->front = (q->front + 1) % q->maxsize;
	

	return 1;            
}

int CNetCtrl::AddDataToFrameQueue(U8 buf,Framequeue *q, FrameBody *e)
{
    return 0;
}

void CNetCtrl::ComposeAckFrame(FrameTrafficCtrl* ctrl,FrameBody *elem,U8 IfOK,U8 FailReason)
{

	U16 crc;

	elem->framehead=FrameHeadWord;
    elem->funcode=9;
    elem->length=10;
    elem->frameno=ctrl->recvframeno;
    elem->trafficctrl=ctrl->sendtrafficctrl;
    elem->databuf[0]=IfOK;
    elem->databuf[1]=FailReason;
    crc=calcrc((U8*)elem,elem->length-2);
    elem->databuf[elem->length-8]=(U8)(crc>>8);
    elem->databuf[elem->length-7]=(U8)crc;

}

int CNetCtrl::SendAckFrame(FrameTrafficCtrl* ctrl,U8 IfOK,U8 FailReason)
{
	FrameBody elem;
	ComposeAckFrame(ctrl,&elem,IfOK,FailReason);
	Uart_SendBuf(0,elem,elem.length);
	return 0;
}

int CNetCtrl::DealPulseFrame(FrameBody frame,FrameTrafficCtrl* ctrl)
{
	/*ctrl->programline=frame.databuf[0]|(frame.databuf[1]<<8);
	ctrl->currentpoint=frame.databuf[2]|(frame.databuf[3]<<8);
	memcpy(&ctrl->realpos[0],&frame.databuf[4],20);*/
    if(QueueLen(&gb_pulseframequeue)>(9*PulseFramequeueLen/10))
    {
         ctrl->sendtrafficctrl  |=0x1;
    }
    else if(QueueLen(&gb_pulseframequeue)<(8*PulseFramequeueLen/10))
	{
	     ctrl->sendtrafficctrl  &=~0x1;
	} 
    EnPulseFrameQueue(&gb_pulseframequeue, frame);
       
	return 0;
}


int CNetCtrl::DealPeriodFrame(FrameBody frame,FrameTrafficCtrl* ctrl)
{   
     if(cDlg->m_pSystemParm->m_bIsCollectMotionData){
	     if(cDlg->m_socket2!=NULL){
	         //cDlg->m_socket2->m_serverip.setAddress("192.168.3.10");
	         //cDlg->m_socket2->m_serverport = 53001;
	         cDlg->m_socket2->Uart_Send(0, (char *)&frame, frame.length);	
	     }
     }
         
    if(((ctrl->sendframeno)!=(ctrl->succedrecvno))&&((frame.frameno)==(ctrl->succedrecvno)))
	{
		ctrl->periodovertime++;
	}
	else
	{
		ctrl->periodovertime=0;  
	}
	if((ctrl->periodovertime>10))
	{
        if((!ctrl->recvtrafficctrl)&&(!ctrl->sendstate))
		{
            ctrl->sendstate|=0x2;
		}
		ctrl->periodovertime=0;  
	}
        /*if((ctrl->periodovertime++>0x2))
	{
	      if(!ctrl->recvtrafficctrl)
		{
			if(((ctrl->sendframeno)!=(ctrl->succedrecvno))&&((frame.frameno)==(ctrl->succedrecvno)))
			{
			      ctrl->sendstate|=0x2;
			}
		}
		ctrl->periodovertime=0;  
	}
	else
	{
	      if(((ctrl->sendframeno)==(ctrl->succedrecvno))||((frame.frameno)!=(ctrl->succedrecvno)))
	                ctrl->periodovertime=0;
	}*/
    ctrl->succedrecvno=frame.frameno;
    ctrl->recvtrafficctrl=frame.trafficctrl;
    //ctrl->periodstateword=frame.databuf[0]|(frame.databuf[1]<<8);
    //memcpy(&ctrl->recvinput[0],&frame.databuf[8],8);
    //memcpy(&ctrl->realpos[0],&frame.databuf[16],8);

    //memcpy(&ctrl->recvinput[0],&frame.databuf[0],8);
    //memcpy(&ctrl->recvoutput[0],&frame.databuf[8],8);
#ifdef SIXAXIS
    memcpy(&cDlg->m_pSystemParm->sys_ctrl.recvinput[0],&frame.databuf[0],16);
    
    //memcpy(&ctrl->realpos[0],&frame.databuf[16],16);// 16
    memcpy(&ctrl->realpos[0],&frame.databuf[16],24);// 16
    //memcpy(&cDlg->m_pSystemParm->sys_ctrl.statebit,&frame.databuf[32],4);
    memcpy(&cDlg->m_pSystemParm->sys_ctrl.statebit,&frame.databuf[40],4);
     //memcpy(&cDlg->m_pSystemParm->sys_ctrl.decode_id,&frame.databuf[36],4);
    memcpy(&cDlg->m_pSystemParm->sys_ctrl.decode_id,&frame.databuf[44],4);
    
    memcpy(&ctrl->apos,&frame.databuf[48],4);
    memcpy(&ctrl->bpos,&frame.databuf[52],4);
    memcpy(&ctrl->cpos,&frame.databuf[56],4);
    memcpy(&ctrl->dpos,&frame.databuf[60],4);
    memcpy(&ctrl->epos,&frame.databuf[64],4);
    memcpy(&ctrl->fpos,&frame.databuf[68],4);
#ifdef  PROGRAM_USELVDS
    memcpy(&ctrl->pulsepos[0],&frame.databuf[72],48);
    memcpy(&ctrl->realaxispos[0],&frame.databuf[120],48);
	ctrl->plusepos[0]=(U32)((ctrl->pulsepos[0]>>SINGLECIRCLE)&MUlTICIRCLEMASK);
	ctrl->plusepos[1]=(U32)(ctrl->pulsepos[0]&SINGLECIRCLEMASK);
	ctrl->plusepos[2]=(U32)((ctrl->pulsepos[1]>>SINGLECIRCLE)&MUlTICIRCLEMASK);
	ctrl->plusepos[3]=(U32)(ctrl->pulsepos[1]&SINGLECIRCLEMASK);
	ctrl->plusepos[4]=(U32)((ctrl->pulsepos[2]>>SINGLECIRCLE)&MUlTICIRCLEMASK);
	ctrl->plusepos[5]=(U32)(ctrl->pulsepos[2]&SINGLECIRCLEMASK);
	ctrl->plusepos[6]=(U32)((ctrl->pulsepos[3]>>SINGLECIRCLE)&MUlTICIRCLEMASK);
	ctrl->plusepos[7]=(U32)(ctrl->pulsepos[3]&SINGLECIRCLEMASK);
	ctrl->plusepos[8]=(U32)((ctrl->pulsepos[4]>>SINGLECIRCLE)&MUlTICIRCLEMASK);
	ctrl->plusepos[9]=(U32)(ctrl->pulsepos[4]&SINGLECIRCLEMASK);
	ctrl->plusepos[10]=(U32)((ctrl->pulsepos[5]>>SINGLECIRCLE)&MUlTICIRCLEMASK);
	ctrl->plusepos[11]=(U32)(ctrl->pulsepos[5]&SINGLECIRCLEMASK);
	
	ctrl->realaxis[0]=(U32)((ctrl->realaxispos[0]>>SINGLECIRCLE)&MUlTICIRCLEMASK);
	ctrl->realaxis[1]=(U32)(ctrl->realaxispos[0]&SINGLECIRCLEMASK);
	ctrl->realaxis[2]=(U32)((ctrl->realaxispos[1]>>SINGLECIRCLE)&MUlTICIRCLEMASK);
	ctrl->realaxis[3]=(U32)(ctrl->realaxispos[1]&SINGLECIRCLEMASK);
	ctrl->realaxis[4]=(U32)((ctrl->realaxispos[2]>>SINGLECIRCLE)&MUlTICIRCLEMASK);
	ctrl->realaxis[5]=(U32)(ctrl->realaxispos[2]&SINGLECIRCLEMASK);
	ctrl->realaxis[6]=(U32)((ctrl->realaxispos[3]>>SINGLECIRCLE)&MUlTICIRCLEMASK);
	ctrl->realaxis[7]=(U32)(ctrl->realaxispos[3]&SINGLECIRCLEMASK);
	ctrl->realaxis[8]=(U32)((ctrl->realaxispos[4]>>SINGLECIRCLE)&MUlTICIRCLEMASK);
	ctrl->realaxis[9]=(U32)(ctrl->realaxispos[4]&SINGLECIRCLEMASK);
	ctrl->realaxis[10]=(U32)((ctrl->realaxispos[5]>>SINGLECIRCLE)&MUlTICIRCLEMASK);
	ctrl->realaxis[11]=(U32)(ctrl->realaxispos[5]&SINGLECIRCLEMASK);
#else
    memcpy(&ctrl->pulsepos[0],&frame.databuf[72],24);//32);
    memcpy(&ctrl->realaxispos[0],&frame.databuf[96],24);//32);
	ctrl->plusepos[0]=0;//ctrl->pulsepos[0]>>SINGLECIRCLE;
	ctrl->plusepos[1]=ctrl->pulsepos[0];//&SINGLECIRCLEMASK;
	ctrl->plusepos[2]=0;//ctrl->pulsepos[1]>>SINGLECIRCLE;
    ctrl->plusepos[3]=ctrl->pulsepos[1];//&SINGLECIRCLEMASK;
	ctrl->plusepos[4]=0;//ctrl->pulsepos[2]>>SINGLECIRCLE;
	ctrl->plusepos[5]=ctrl->pulsepos[2];//&SINGLECIRCLEMASK;
	ctrl->plusepos[6]=0;//ctrl->pulsepos[3]>>SINGLECIRCLE;
	ctrl->plusepos[7]=ctrl->pulsepos[3];//&SINGLECIRCLEMASK;
	ctrl->plusepos[8]=0;//ctrl->pulsepos[2]>>SINGLECIRCLE;
	ctrl->plusepos[9]=ctrl->pulsepos[4];//&SINGLECIRCLEMASK;
	ctrl->plusepos[10]=0;//ctrl->pulsepos[3]>>SINGLECIRCLE;
	ctrl->plusepos[11]=ctrl->pulsepos[5];//&SINGLECIRCLEMASK;
	
	ctrl->realaxis[0]=0;//ctrl->realaxispos[0]>>SINGLECIRCLE;
	ctrl->realaxis[1]=ctrl->realaxispos[0];//&SINGLECIRCLEMASK;
	ctrl->realaxis[2]=0;//ctrl->realaxispos[1]>>SINGLECIRCLE;
	ctrl->realaxis[3]=ctrl->realaxispos[1];//&SINGLECIRCLEMASK;
	ctrl->realaxis[4]=0;//ctrl->realaxispos[2]>>SINGLECIRCLE;
	ctrl->realaxis[5]=ctrl->realaxispos[2];//&SINGLECIRCLEMASK;
	ctrl->realaxis[6]=0;//ctrl->realaxispos[3]>>SINGLECIRCLE;
	ctrl->realaxis[7]=ctrl->realaxispos[3];//&SINGLECIRCLEMASK;
	ctrl->realaxis[8]=0;//ctrl->realaxispos[2]>>SINGLECIRCLE;
	ctrl->realaxis[9]=ctrl->realaxispos[4];//&SINGLECIRCLEMASK;
	ctrl->realaxis[10]=0;//ctrl->realaxispos[3]>>SINGLECIRCLE;
	ctrl->realaxis[11]=ctrl->realaxispos[5];//&SINGLECIRCLEMASK;

	if(frame.length>144)
	{
	memcpy(&ctrl->alarm,&frame.databuf[96+24],24);//32);

	if(ctrl->alarm.Err_Comm||ctrl->alarm.Err_Plan||
		ctrl->alarm.Err_Run||ctrl->alarm.Err_Lim||
		ctrl->alarm.Alm_Ser||ctrl->alarm.OTC_CIA)
	{
	        if(!m_bIsCtlAlarm)
	        {
	        	if(ctrl->alarm.Err_Comm)
		 		ctrl->alarmno=ctrl->alarm.Err_Comm;
			else if(ctrl->alarm.Err_Plan)
				ctrl->alarmno=ctrl->alarm.Err_Plan;
			else if(ctrl->alarm.Err_Run)
		 		ctrl->alarmno=ctrl->alarm.Err_Run;
			else if(ctrl->alarm.Err_Lim)
				ctrl->alarmno=ctrl->alarm.Err_Lim;
			else if(ctrl->alarm.Alm_Ser)
		 		ctrl->alarmno=ctrl->alarm.Alm_Ser;
			else if(ctrl->alarm.OTC_CIA)
				ctrl->alarmno=ctrl->alarm.OTC_CIA;

			m_bIsCtlAlarm = true;
	        }
	}
	}

	if(frame.length>168)
	{
		memcpy(&cDlg->m_pSystemParm->m_current[0],&frame.databuf[96+48],4*MOF);
	}
	if(frame.length>172)
	{
		memcpy(&ctrl->realpos[6],&frame.databuf[172],4);// 16
		memcpy(&ctrl->gpos,&frame.databuf[176],4);
		memcpy(&ctrl->pulsepos[6],&frame.databuf[180],4);
    		memcpy(&ctrl->realaxispos[6],&frame.databuf[184],4);
		ctrl->plusepos[12]=0;
		ctrl->plusepos[13]=ctrl->pulsepos[6];
		ctrl->realaxis[12]=0;//ctrl->realaxispos[3]>>SINGLECIRCLE;
	       ctrl->realaxis[13]=ctrl->realaxispos[6];//&SINGLECIRCLEMASK;
	
	}
#endif

#else

    memcpy(&cDlg->m_pSystemParm->sys_ctrl.recvinput[0],&frame.databuf[0],16);
    
    memcpy(&ctrl->realpos[0],&frame.databuf[16],16);
    //memcpy(&ctrl->statebit,&frame.databuf[32],4);
    memcpy(&cDlg->m_pSystemParm->sys_ctrl.statebit,&frame.databuf[32],4);
    //memcpy(&ctrl->decode_id,&frame.databuf[36],4);
     memcpy(&cDlg->m_pSystemParm->sys_ctrl.decode_id,&frame.databuf[36],4);
    memcpy(&ctrl->apos,&frame.databuf[40],4);
    memcpy(&ctrl->bpos,&frame.databuf[44],4);
    memcpy(&ctrl->cpos,&frame.databuf[48],4);
    memcpy(&ctrl->dpos,&frame.databuf[52],4);
   
#ifdef  PROGRAM_USELVDS
    memcpy(&ctrl->pulsepos[0],&frame.databuf[56],32);
    memcpy(&ctrl->realaxispos[0],&frame.databuf[88],32);
	ctrl->plusepos[0]=(U32)((ctrl->pulsepos[0]>>SINGLECIRCLE)&MUlTICIRCLEMASK);
	ctrl->plusepos[1]=(U32)(ctrl->pulsepos[0]&SINGLECIRCLEMASK);
	ctrl->plusepos[2]=(U32)((ctrl->pulsepos[1]>>SINGLECIRCLE)&MUlTICIRCLEMASK);
	ctrl->plusepos[3]=(U32)(ctrl->pulsepos[1]&SINGLECIRCLEMASK);
	ctrl->plusepos[4]=(U32)((ctrl->pulsepos[2]>>SINGLECIRCLE)&MUlTICIRCLEMASK);
	ctrl->plusepos[5]=(U32)(ctrl->pulsepos[2]&SINGLECIRCLEMASK);
	ctrl->plusepos[6]=(U32)((ctrl->pulsepos[3]>>SINGLECIRCLE)&MUlTICIRCLEMASK);
	ctrl->plusepos[7]=(U32)(ctrl->pulsepos[3]&SINGLECIRCLEMASK);
	
	ctrl->realaxis[0]=(U32)((ctrl->realaxispos[0]>>SINGLECIRCLE)&MUlTICIRCLEMASK);
	ctrl->realaxis[1]=(U32)(ctrl->realaxispos[0]&SINGLECIRCLEMASK);
	ctrl->realaxis[2]=(U32)((ctrl->realaxispos[1]>>SINGLECIRCLE)&MUlTICIRCLEMASK);
	ctrl->realaxis[3]=(U32)(ctrl->realaxispos[1]&SINGLECIRCLEMASK);
	ctrl->realaxis[4]=(U32)((ctrl->realaxispos[2]>>SINGLECIRCLE)&MUlTICIRCLEMASK);
	ctrl->realaxis[5]=(U32)(ctrl->realaxispos[2]&SINGLECIRCLEMASK);
	ctrl->realaxis[6]=(U32)((ctrl->realaxispos[3]>>SINGLECIRCLE)&MUlTICIRCLEMASK);
	ctrl->realaxis[7]=(U32)(ctrl->realaxispos[3]&SINGLECIRCLEMASK);
#else
    memcpy(&ctrl->pulsepos[0],&frame.databuf[56],16);//32);
    memcpy(&ctrl->realaxispos[0],&frame.databuf[72],16);//32);
	ctrl->plusepos[0]=0;//ctrl->pulsepos[0]>>SINGLECIRCLE;
	ctrl->plusepos[1]=ctrl->pulsepos[0];//&SINGLECIRCLEMASK;
	ctrl->plusepos[2]=0;//ctrl->pulsepos[1]>>SINGLECIRCLE;
    ctrl->plusepos[3]=ctrl->pulsepos[1];//&SINGLECIRCLEMASK;
	ctrl->plusepos[4]=0;//ctrl->pulsepos[2]>>SINGLECIRCLE;
	ctrl->plusepos[5]=ctrl->pulsepos[2];//&SINGLECIRCLEMASK;
	ctrl->plusepos[6]=0;//ctrl->pulsepos[3]>>SINGLECIRCLE;
	ctrl->plusepos[7]=ctrl->pulsepos[3];//&SINGLECIRCLEMASK;
	
	ctrl->realaxis[0]=0;//ctrl->realaxispos[0]>>SINGLECIRCLE;
	ctrl->realaxis[1]=ctrl->realaxispos[0];//&SINGLECIRCLEMASK;
	ctrl->realaxis[2]=0;//ctrl->realaxispos[1]>>SINGLECIRCLE;
	ctrl->realaxis[3]=ctrl->realaxispos[1];//&SINGLECIRCLEMASK;
	ctrl->realaxis[4]=0;//ctrl->realaxispos[2]>>SINGLECIRCLE;
	ctrl->realaxis[5]=ctrl->realaxispos[2];//&SINGLECIRCLEMASK;
	ctrl->realaxis[6]=0;//ctrl->realaxispos[3]>>SINGLECIRCLE;
	ctrl->realaxis[7]=ctrl->realaxispos[3];//&SINGLECIRCLEMASK;
#endif

#endif
	return 0;
}

int CNetCtrl::DealStateFrame(FrameBody frame,FrameTrafficCtrl* ctrl)
{
	
	ctrl->state=frame.databuf[0];
    switch (ctrl->state)
    {
    case 1:
        ctrl->alarmno=frame.databuf[1];
        m_bIsCtlAlarm = true;
        break;
    case 2:
        memcpy(m_cVersion, &frame.databuf[1], 13);
        m_bIsVersionGet = true;
        break;
    case 3:
        if (frame.databuf[1] == 0)
        {
#ifdef  PROGRAM_USELVDS       	
               memcpy(Motor_AbsZero, &frame.databuf[2], MOF*sizeof(uint64_t));
               zeropos[0]=(U32)((Motor_AbsZero[0]>>SINGLECIRCLE)&MUlTICIRCLEMASK);
		zeropos[1]=(U32)(Motor_AbsZero[0]&SINGLECIRCLEMASK);
		zeropos[2]=(U32)((Motor_AbsZero[1]>>SINGLECIRCLE)&MUlTICIRCLEMASK);
		zeropos[3]=(U32)(Motor_AbsZero[1]&SINGLECIRCLEMASK);
		zeropos[4]=(U32)((Motor_AbsZero[2]>>SINGLECIRCLE)&MUlTICIRCLEMASK);
		zeropos[5]=(U32)(Motor_AbsZero[2]&SINGLECIRCLEMASK);
		zeropos[6]=(U32)((Motor_AbsZero[3]>>SINGLECIRCLE)&MUlTICIRCLEMASK);
		zeropos[7]=(U32)(Motor_AbsZero[3]&SINGLECIRCLEMASK);
		zeropos[8]=(U32)((Motor_AbsZero[4]>>SINGLECIRCLE)&MUlTICIRCLEMASK);
		zeropos[9]=(U32)(Motor_AbsZero[4]&SINGLECIRCLEMASK);
		zeropos[10]=(U32)((Motor_AbsZero[5]>>SINGLECIRCLE)&MUlTICIRCLEMASK);
		zeropos[11]=(U32)(Motor_AbsZero[5]&SINGLECIRCLEMASK);
#else
               memcpy(Motor_AbsZero, &frame.databuf[2], MOF*sizeof(int));
		zeropos[0]=0;//(U32)((Motor_AbsZero[0]>>SINGLECIRCLE)&MUlTICIRCLEMASK);
		zeropos[1]=Motor_AbsZero[0];//(U32)(Motor_AbsZero[0]&SINGLECIRCLEMASK);
		zeropos[2]=0;//(U32)((Motor_AbsZero[1]>>SINGLECIRCLE)&MUlTICIRCLEMASK);
		zeropos[3]=Motor_AbsZero[1];//(U32)(Motor_AbsZero[1]&SINGLECIRCLEMASK);
		zeropos[4]=0;//(U32)((Motor_AbsZero[2]>>SINGLECIRCLE)&MUlTICIRCLEMASK);
		zeropos[5]=Motor_AbsZero[2];//(U32)(Motor_AbsZero[2]&SINGLECIRCLEMASK);
		zeropos[6]=0;//(U32)((Motor_AbsZero[3]>>SINGLECIRCLE)&MUlTICIRCLEMASK);
		zeropos[7]=Motor_AbsZero[3];//(U32)(Motor_AbsZero[3]&SINGLECIRCLEMASK);
		zeropos[8]=0;//(U32)((Motor_AbsZero[2]>>SINGLECIRCLE)&MUlTICIRCLEMASK);
		zeropos[9]=Motor_AbsZero[4];//(U32)(Motor_AbsZero[2]&SINGLECIRCLEMASK);
		zeropos[10]=0;//(U32)((Motor_AbsZero[3]>>SINGLECIRCLE)&MUlTICIRCLEMASK);
		zeropos[11]=Motor_AbsZero[5];//(U32)(Motor_AbsZero[3]&SINGLECIRCLEMASK);
#endif
		memcpy(&cDlg->m_pSystemParm->zeropos[0],&zeropos[0],8*MOF);
        }
	else if (frame.databuf[1] == 1)
        {
            //memcpy(&Robot_Link, &frame.databuf[2], sizeof(LinkDimension));
	     memcpy(&cDlg->m_pSystemParm->Robot_Link,&frame.databuf[2],sizeof(LinkDimension));	
        }

        break;
    case 4:
	//memcpy(&ToolCalResult[frame.databuf[1]],&frame.databuf[2],sizeof(CartesianPose));
	 memcpy(&cDlg->m_pSystemParm->ToolCalResult[frame.databuf[1]],&frame.databuf[2],sizeof(CartesianPose));	
        break;

    case 5:
	//memcpy(&ToolCalResult[frame.databuf[1]],&frame.databuf[2],sizeof(CartesianPose));
	 memcpy(&cDlg->m_pSystemParm->m_battery[0],&frame.databuf[1],6);//5);	
        break;

    case 6:
	//memcpy(&ToolCalResult[frame.databuf[1]],&frame.databuf[2],sizeof(CartesianPose));
	 memcpy(&cDlg->m_pSystemParm->m_current[0],&frame.databuf[2],4*MOF);	
        break;

   case 7:
	//memcpy(&ToolCalResult[frame.databuf[1]],&frame.databuf[2],sizeof(CartesianPose));
	{
	    unsigned short start=0,end=0;
	    /*start=((frame.databuf[3])<<8)|0xff00;
	     start|= frame.databuf[2];
	     end=((frame.databuf[5])<<8)|0xff00;
	     end|= frame.databuf[4]; */
	     start=(frame.databuf[3]&0xff)<<8;	 
	     start|= frame.databuf[2];
	     end=(frame.databuf[5]&0xff)<<8;	
	     end|= frame.databuf[4];
	//if((frame.databuf[2]<0)||(frame.databuf[2]>255)||(frame.databuf[3]<0)||(frame.databuf[3]>255)||(frame.databuf[3]<frame.databuf[2])) return 0;
	    if((start<0)||(start>=TEACHPIONTNUM)||(end<0)||(end>=TEACHPIONTNUM)||(end<start)) break;
	     memcpy(&cDlg->m_pSystemParm->teachpoint[start],&frame.databuf[6],(end-start+1)*TEACHPIONTSIZE);	
	}
	break;

   case 8:
	//memcpy(&ToolCalResult[frame.databuf[1]],&frame.databuf[2],sizeof(CartesianPose));
	{
	    unsigned short start=0,end=0;
	    /*start=((frame.databuf[3])<<8)|0xff00;
	     start|= frame.databuf[2];
	     end=((frame.databuf[5])<<8)|0xff00;
	     end|= frame.databuf[4]; */
	     start=(frame.databuf[3]&0xff)<<8;	 
	     start|= frame.databuf[2];
	     end=(frame.databuf[5]&0xff)<<8;	
	     end|= frame.databuf[4];
	//if((frame.databuf[2]<0)||(frame.databuf[2]>255)||(frame.databuf[3]<0)||(frame.databuf[3]>255)||(frame.databuf[3]<frame.databuf[2])) return 0;
	    if((start<0)||(start>=TEACHPIONTNUM)||(end<0)||(end>=TEACHPIONTNUM)||(end<start)) break;
	     memcpy(&cDlg->m_pSystemParm->teachpointtext[start][0],&frame.databuf[6],(end-start+1)*128);	
	      if(end>=(TEACHPIONTNUM-1)) cDlg->m_pSystemParm->teachreadfinished=1;
	}
	break;

	case 9:
	//memcpy(&ToolCalResult[frame.databuf[1]],&frame.databuf[2],sizeof(CartesianPose));
	{
	  
	     memcpy(&cDlg->m_pSystemParm->axisno[0],&frame.databuf[2],68);	
	      cDlg->m_pSystemParm->getaxinofinished=1;
	}
	break;

	case 10:
	//memcpy(&ToolCalResult[frame.databuf[1]],&frame.databuf[2],sizeof(CartesianPose));
	{
	  
	     unsigned short start=0,end=0;
	     start=(frame.databuf[3]&0xff)<<8;	 
	     start|= frame.databuf[2];
	     end=(frame.databuf[5]&0xff)<<8;	
	     end|= frame.databuf[4];
	//if((frame.databuf[2]<0)||(frame.databuf[2]>255)||(frame.databuf[3]<0)||(frame.databuf[3]>255)||(frame.databuf[3]<frame.databuf[2])) return 0;
	    if((start<0)||(start>=256)||(end<0)||(end>=256)||(end<start)) break;
	     memcpy(&cDlg->m_pSystemParm->SystemParam[start],&frame.databuf[6],(end-start+1)*4);
	       if(end>=(pAccTime))  cDlg->m_pSystemParm->getsysparamfinished=1;
		if((start<=(pAIXS1PID+4))&&(end>=pAIXS1PID))  cDlg->m_pSystemParm->getpidparamfinished[0]=1;
		if((start<=(pAIXS2PID+4))&&(end>=pAIXS2PID))   cDlg->m_pSystemParm->getpidparamfinished[1]=1;
		if((start<=(pAIXS3PID+4))&&(end>=pAIXS3PID))   cDlg->m_pSystemParm->getpidparamfinished[2]=1;
		if((start<=(pAIXS4PID+4))&&(end>=pAIXS4PID))   cDlg->m_pSystemParm->getpidparamfinished[3]=1;
		if((start<=(pAIXS5PID+4))&&(end>=pAIXS5PID))   cDlg->m_pSystemParm->getpidparamfinished[4]=1;
		if((start<=(pAIXS6PID+4))&&(end>=pAIXS6PID))   cDlg->m_pSystemParm->getpidparamfinished[5]=1;
		if((start<=(pAIXS7PID+4))&&(end>=pAIXS7PID))   cDlg->m_pSystemParm->getpidparamfinished[6]=1;
	}
	break;
    default:
        break;
    }

	return 0;
}

int CNetCtrl::DealParameterFrame(FrameBody frame,FrameTrafficCtrl* ctrl)
{

	
	return 0;
}

int CNetCtrl::DealAckFrame(FrameBody frame,FrameTrafficCtrl* ctrl)
{
       /*if(1==frame.databuf[0])
       {
		ctrl->sendstate|=0x2;
       }
	else if((ctrl->sendframeno==frame.frameno)&&(0==frame.databuf[0]))
	{
		
		ctrl->sendstate&=~0x2;
		ctrl->sendstate&=~0x1;
	}*/
	/*if(((ctrl->sendframeno)!=(ctrl->succedrecvno))&&((frame.frameno)==(ctrl->succedrecvno)))
	{
		ctrl->periodovertime++;
	}
	else
	{
		ctrl->periodovertime=0;  
	}
	if((ctrl->periodovertime>0x2))
	{
	      if((!ctrl->recvtrafficctrl)&&(!ctrl->sendstate))
		{

			  ctrl->sendstate|=0x2;

		}
		ctrl->periodovertime=0;  
	}*/
	ctrl->periodovertime=0; 
	ctrl->succedrecvno=frame.frameno;
	ctrl->recvtrafficctrl=frame.trafficctrl;
	ctrl->resendno=frame.databuf[1];
	
	return 0;
}

int CNetCtrl::DealFrame(FrameBody frame,FrameTrafficCtrl* ctrl)
{
	if((frame.funcode!=4)&&(frame.funcode!=9))
	{
		if(((ctrl->sendtrafficctrl)&0x2)==0x2)//要求对方重发状态
		{
			if(((U8)(ctrl->recvframeno+1))!=frame.frameno)//不成功,	
				return 1;
			else
				ctrl->sendtrafficctrl &=~0x2;//重发成功	
	       }
		   
		//if(((U8)(ctrl->recvframeno+1))==frame.frameno)
		if((((U8)(ctrl->recvframeno+1))==frame.frameno)&&(frame.funcode>2))
			ctrl->recvframeno=frame.frameno;
		else
		{
			//ctrl->sendtrafficctrl  |=0x2;
			return 1;
		}
	}
	
	switch(frame.funcode)
	{
		case 1:
                    //DealPulseFrame(frame,ctrl);
		break;
		
		case 2://当前执行点所在行，当前的位置
			DealPulseFrame(frame,ctrl);
		break;
		
		case 3:
                  //DealPeriodFrame(frame,ctrl);
		break;

		case 4:
			DealPeriodFrame(frame,ctrl);
		break;
		
		case 5:
                  //DealStateFrame(frame,ctrl);
		break;

		case 6:
			DealStateFrame(frame,ctrl);
		break;

		case 7:
                  //DealParameterFrame(frame,ctrl);
		break;

		case 8:
                  DealParameterFrame(frame,ctrl);
		break;

        case 9:
                  DealAckFrame(frame,ctrl);
		break;
		default:
		break;
			
	}
	return 0;
}

int CNetCtrl::GetFrame(U8 *buf,FrameTrafficCtrl* ctrl)
{
    int num=0,state=0;
    U16 crc,crcr;
    while(!state)
    {
    do
    {
        num=OSReadUart0(buf,1,1);
    }while((num)&&(buf[0]!=FirstByte));

    if(!num)
    {
        break;
    }
    else if(buf[0]==FirstByte)
    {
        num=OSReadUart0(&buf[1],1,1);
        if(!num) continue;

        if(buf[1]==SecondByte)
        {
            num=OSReadUart0(&buf[2],2,20);

            if(num==2)
            {
                if(buf[3]>MAXFRAMELEN)
                {
                      num=0;
                      state=FAIL;
                      continue;
                }
                num=OSReadUart0(&buf[4],(buf[3]-4),100);
                if(num==(buf[3]-4))
                {
                    num=buf[3];
                    crc=calcrc(buf,num-2);
                    crcr=buf[num-2];
                    crcr=crcr<<8;
                    crcr=crcr|buf[num-1];
                    if(crc==crcr)
                    {
                        state=OK;break;
                    }
                    else
                    {
                           //if(buf[2]!=3)//非周期性数据出错要重发
                        //	ctrl->sendtrafficctrl  |=2;
                        num=0;
                        state=FAIL;
                        continue;
                    }
                }
            }
        }
    }


    }
	return num;
}

int CNetCtrl::ParseFrame(U8 *buf,FrameBody *frame,FrameTrafficCtrl* ctrl)
{
	frame->framehead=((buf[1]<<8)&0xff00)|buf[0];
	frame->funcode=buf[2];
	frame->length=buf[3];
	frame->frameno=buf[4];
	frame->trafficctrl=buf[5];
	memcpy(&frame->databuf[0],&buf[6],(buf[3]-6));
	return 0;
}

 int CNetCtrl::GetHaveSendFrameQueue(Framequeue *q, FrameBody *e,FrameTrafficCtrl*ctrl)
{
	static U16 front,resendflag=0;
	 if(!q->queue)
	{
		resendflag=0;
		ctrl->sendstate&=~0x2;
		return 1;
	}
	// 若队列为空则失败
       if(q->front == q->rear)
	{
		resendflag=0;
		ctrl->sendstate&=~0x2;
		return 1;
	}
	if(!resendflag)
	{
		front=q->front;
		do{
			// 返回元素
			*e = q->queue[front]; 

			front = (front + 1) % q->maxsize;
		}while(((e->frameno)!=((U8)(ctrl->succedrecvno+1)))&&(front != q->rear));
		
		if((e->frameno)==((U8)(ctrl->succedrecvno+1)))
		{
			
			resendflag=1;
		}
                else if(front==q->rear)
		{
			resendflag=0;
			ctrl->sendstate&=~0x2;
			return 1;
		} 
	}
	else
	{
		if(front == q->rear)
		{
			resendflag=0;
                        ctrl->sendstate&=~0x2;
			return 1;
		}
		*e = q->queue[front]; 

		front = (front + 1) % q->maxsize;	
	}
	return 0;            
}

int CNetCtrl::MovehaverecvdFromQueue(Framequeue *q,FrameTrafficCtrl*ctrl)
{
	U16 front;
	FrameBody e;
	 if(!q->queue)
	{
		return 0;
	}
	// 若队列为空则删除失败
       if(q->front == q->rear)
	{
		return 0;
	}

	front=q->front;
	
	do{
		// 返回队首元素
		e = q->queue[front]; 

		front = (front + 1) % q->maxsize;
	}while((e.frameno!=ctrl->succedrecvno)&&(front != q->rear));
	
	if(e.frameno==ctrl->succedrecvno)
	{
		q->front=front ;
	}
	else if(front==q->rear)
	{
		return 1;
	}	
	
	return 0;
}

int CNetCtrl::SendNoPeriodFrame(FrameTrafficCtrl* ctrl)
{
	FrameBody elem;
	U16 crc;
	static int resendcnt=0;
	if(ctrl->recvtrafficctrl&0x2)
	{
		//if(ctrl->sendstate&0x2!=2)
		//	ctrl->resendno=ctrl->succedrecvno+1;
		ctrl->sendstate|=0x2;//重发状态
		
	}
	if(ctrl->recvtrafficctrl&0x1)
	{
		ctrl->sendstate|=0x1;//停止状态
		
	}
	if(!ctrl->recvtrafficctrl)
	{
		ctrl->sendstate&=~0x1;
	}
	if(ctrl->recvtrafficctrl&0x4)
	{

		InitCommuncate();
		return 1;
	}

	if(((ctrl->sendstate)&0x2)==2)
	{
		resendcnt=0;
        while(!GetHaveSendFrameQueue(&gb_havesendframequeue, &elem,ctrl))
		{
			
			Uart_SendBuf(0,elem,elem.length);
			ctrl->periodovertime=0;
			if(resendcnt++>16) 
			{
				resendcnt=0;
				break;
			}
		}
	}
	else if(!ctrl->sendstate)
	{
		//正常发送
		resendcnt=0;
        while(1)
		{
			if(!FullQueue(&gb_havesendframequeue))
			{
				if(DeFrameQueue(&gb_framequeue, &elem))
				{
					ctrl->sendframeno++;
					elem.frameno=ctrl->sendframeno;
					crc=calcrc((U8*)&elem,elem.length-2);
					elem.databuf[elem.length-8]=(U8)(crc>>8);
					elem.databuf[elem.length-7]=(U8)crc;
					EnHaveSendFrameQueue(&gb_havesendframequeue, elem);
					Uart_SendBuf(0,elem,elem.length);
					ctrl->periodovertime=0;
					//ctrl->sendstate|=0x1;
					//加入已发送队列
					if(resendcnt++>16) 
					{
						resendcnt=0;
						break;
					}
					
				}
				else
				{
					break;
				}
			}
			else
			{
				break;
			}
            if(ctrl->overtime>10)
            {
                break;
            }
		}
						
	}
	//清除已发送队列中已成功接收到的数据
	MovehaverecvdFromQueue(&gb_havesendframequeue,ctrl);	
	return 0;
}

void CNetCtrl::ComposePeriodFrame(FrameTrafficCtrl* ctrl,FrameBody *elem)
{

	U16 crc;

    elem->framehead=FrameHeadWord;
    elem->funcode=3;
    elem->length=24;
    elem->frameno=ctrl->recvframeno;
    elem->trafficctrl=ctrl->sendtrafficctrl;

    memcpy(&elem->databuf[0],&cDlg->m_pSystemParm->sys_ctrl.sendinput[0],16);	
    //memcpy(&elem->databuf[0],&ctrl->sendinput[0],16);
  
    crc=calcrc((U8*)elem,elem->length-2);
    elem->databuf[elem->length-8]=(U8)(crc>>8);
    elem->databuf[elem->length-7]=(U8)crc;


}


int CNetCtrl::SendPeriodFrame(FrameTrafficCtrl* ctrl)
{
	FrameBody elem;
	ComposePeriodFrame(ctrl,&elem);
	Uart_SendBuf(0,elem,elem.length);

    if(ctrl->sendtrafficctrl&0x4)
    {
        InitCommuncate();
        ctrl->sendtrafficctrl&=~0x4;

    }
	return 0;
}

void CNetCtrl::InitCommuncate(void)
{
	memset(framebuf,0,sizeof(framebuf));
	memset(&gb_frame,0,sizeof(gb_frame));
    if(gb_ctrl.sendtrafficctrl&0x4)
    {
        memset(&gb_ctrl,0,20);
    }
    else
    {
        memset(&gb_ctrl,0,sizeof(gb_ctrl));
	 cDlg->m_pSystemParm->sys_ctrl.sendinput[0]&=~0x1;	 
    }
	//memcpy(&cDlg->m_pSystemParm->sys_ctrl.sendinput[0],0,16);	
	
	memset(pulsecnt,0,sizeof(pulsecnt));
	InitFrameQueue(&gb_framequeue,FramequeueLen);
	InitHaveSendFrameQueue(&gb_havesendframequeue,HaveSendqueueLen);
	InitPulseFrameQueue(&gb_pulseframequeue,PulseFramequeueLen);

    gb_ctrl.sendoutput[0] = 128;//CTL_IO_ZAXIS_FOLLOW（第7位）的初始值是1
}

int CNetCtrl::ParseNetFrame(U8 *buf,FrameBody *frame,FrameTrafficCtrl* ctrl)
{
	U16 crc,crcr;
	frame->framehead=((buf[1]<<8)&0xff00)|buf[0];
	if(frame->framehead!=FrameHeadWord)
	{
		frame->funcode=0;
		return FAIL;
	}
	
	frame->funcode=buf[2];
	if(frame->funcode>8)
	{
		frame->funcode=0;
		return FAIL;
	}
	
	frame->length=buf[3];
	if(frame->length>MAXFRAMELEN)
	{
		frame->funcode=0;
		return FAIL;
	}
					      						      
	crc=calcrc(buf,buf[3]-2);
	crcr=buf[buf[3]-2];
	crcr=crcr<<8;
	crcr=crcr|buf[buf[3]-1];
	if(crc!=crcr) 
	{
		frame->funcode=0;
		return FAIL;
	}

	frame->frameno=buf[4];
	frame->trafficctrl=buf[5];
	memcpy(&frame->databuf[0],&buf[6],(buf[3]-6));
	return OK;
}

int CNetCtrl::DealNetFrame(FrameBody frame,FrameTrafficCtrl* ctrl)
{
		if((frame.funcode!=4)&&(frame.funcode!=9))
		{
			if(((ctrl->sendtrafficctrl)&0x2)==0x2)//要求对方重发状态
			{
				if(((U8)(ctrl->recvframeno+1))!=frame.frameno)//不成功,	
					return 1;
				else
					ctrl->sendtrafficctrl &=~0x2;//重发成功 
			   }
			   
			//if(((U8)(ctrl->recvframeno+1))==frame.frameno)
			if((((U8)(ctrl->recvframeno+1))==frame.frameno)&&(frame.funcode>2))
				ctrl->recvframeno=frame.frameno;
			else
			{
				//ctrl->sendtrafficctrl  |=0x2;
				return 1;
			}
		}
		
		switch(frame.funcode)
		{
			case 1:
						//DealPulseFrame(frame,ctrl);
			break;
			
			case 2://当前执行点所在行，当前的位置
				DealPulseFrame(frame,ctrl);
			break;
			
			case 3:
					  //DealPeriodFrame(frame,ctrl);
			break;
	
			case 4:
				m_RevCount++;
				DealPeriodFrame(frame,ctrl);
			break;
			
			case 5:
					  //DealStateFrame(frame,ctrl);
			break;
	
			case 6:
				DealStateFrame(frame,ctrl);
			break;
	
			case 7:
					  //DealParameterFrame(frame,ctrl);
			break;
	
			case 8:
					  DealParameterFrame(frame,ctrl);
			break;
	
			case 9:
					  DealAckFrame(frame,ctrl);
			break;
			default:
			break;
				
		}
		return 0;
}

void CNetCtrl::Communicate_Task(void)
{
    static int timeover=0;
   /* while(GetFrame(framebuf,&gb_ctrl)>6)
    {
        ParseFrame(framebuf,&gb_frame,&gb_ctrl);
        DealFrame(gb_frame,&gb_ctrl);
        if(gb_ctrl.overtime++>10)
        {
        	gb_ctrl.overtime=0;
            break;
        }
    }*/


    SendNoPeriodFrame(&gb_ctrl);

    //if(gb_ctrl.overtime++>10)
    if(timeover++>2)
    {
        SendPeriodFrame(&gb_ctrl);
	 timeover=0;
        //gb_ctrl.overtime=0;

        //qDebug()<<"send id="<<gb_ctrl.sendframeno<<" ,receve id="<<gb_ctrl.succedrecvno;
    }
}

void CNetCtrl::initSendQueue()
{
    InitFrameQueue(&gb_framequeue,FramequeueLen);
    InitHaveSendFrameQueue(&gb_havesendframequeue,HaveSendqueueLen);
}

bool CNetCtrl::isFrameNotFull()
{
    if (FullQueue(&gb_framequeue)==1)
        return false;
    else
        return true;
//    // 当队列满时,不能插入
//     if(!gb_framequeue.queue)
//    {
//        return false;
//    }
//    if((gb_framequeue.rear + 1) % gb_framequeue.maxsize == gb_framequeue.front)
//    {
//        return false;
//    }
//    return true;
}

void CNetCtrl::Uart_SendBuf(int Uartnum, FrameBody pt,int cnt)
{
     if(cDlg->m_socket!=NULL)
         cDlg->m_socket->Uart_Send(Uartnum, (char *)&pt, cnt);
	 
	 //cDlg->motionplanmod->RecvFrame(pt);
}

void CNetCtrl::RecvFrame(FrameBody buffer)
{
     

	 DealNetFrame(buffer,&gb_ctrl);
}

int CNetCtrl::OSReadUart0(U8 *data, int num, int ntimeout)
{
	if(cDlg->m_socket!=NULL)
        return (cDlg->m_socket->OSReadUart((char *)data, num, ntimeout));
	else
	   return 0;
}



void CNetCtrl::ontimeout()
{
	static int cnt=0;
    static unsigned int preRevCount=0;
	if(cDlg->m_socket!=NULL)
		cDlg->m_socket->TimeCount0++;

	/*if(gb_ctrl.overtime++>50)
	{
		gb_ctrl.overtime=51;
	}*/

	/* if(gb_ctrl.periodovertime++>50)
	{
		gb_ctrl.periodovertime=51;
	}*/
	
	Communicate_Task();

	cnt = (cnt+1)%50;
	if(cnt==49){
	    if(m_RevCount == preRevCount){  // �Ͽ�����
	        m_IsConn = false;
	    }else{
	        preRevCount = m_RevCount;
	        if(!m_IsConn){
	            m_IsConn = true;
	        }
	    }
	}
}


// �ж� �����Ƿ�����
bool CNetCtrl::NetIsConn(void)
{
    return m_IsConn;
}

