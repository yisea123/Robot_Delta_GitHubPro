// NetCtrl.h: interface for the CNetCtrl class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _NETCTRL_H_
#define _NETCTRL_H_
#include <QObject>
#include <QTimer>
#include "globalvariabletype.h"
#include "GlobalDefine.h"
#include "MotionControllerDevice.h"

class MotionControllerDevice;

class CNetCtrl : public QObject  
{

     Q_OBJECT
public:
    CNetCtrl(MotionControllerDevice* pDlg);
	virtual ~CNetCtrl();
	void InitCommuncate(void);
	int GetFrame(U8 *buf,FrameTrafficCtrl* ctrl);
	
	int ParseFrame(U8 *buf,FrameBody *frame,FrameTrafficCtrl* ctrl);
	int DealFrame(FrameBody frame,FrameTrafficCtrl* ctrl);

	int SendNoPeriodFrame(FrameTrafficCtrl* ctrl);
	int SendPeriodFrame(FrameTrafficCtrl* ctrl);
	int DePulseQueue(Framequeue *q, FrameBody *e);
	int EnFrameQueue(Framequeue *q, FrameBody e);
    void Communicate_Task(void);

    void initSendQueue();
    bool isFrameNotFull();

public:
    MotionControllerDevice *cDlg;
	QTimer *m_NetCtrlTimer;
	int PeriodCount;
	FrameBody gb_frame;
	FrameTrafficCtrl gb_ctrl;
	U8 framebuf[MAXFRAMELEN];
	FrameBody Frame[FramequeueLen+1];
	FrameBody HaveSendFrame[HaveSendqueueLen+1];
	FrameBody PulseFrame[PulseFramequeueLen+1];


	Framequeue gb_framequeue;
	Framequeue gb_havesendframequeue;
	Framequeue gb_pulseframequeue;

	int pulsecnt[5];
	int lppulsecnt[5];

    bool m_bIsCtlAlarm;
    char m_cVersion[13];
    bool m_bIsVersionGet;
#ifdef PROGRAM_USELVDS
    uint64_t Motor_AbsZero[MOF]; 
#else
    U32 Motor_AbsZero[MOF]; 
#endif
	U32 zeropos[2*MOF];
    LinkDimension Robot_Link;
    CartesianPose ToolCalResult[21];
    
    float m_fFollowValueRadio[100];
    float m_fFollowValueHigh[100];
    int   m_nFollowValueSize;
    bool  m_bIsFollowValueGet;

    void Uart_SendBuf(int Uartnum, FrameBody pt,int cnt);
    int OSReadUart0(U8 *data, int num, int ntimeout);
    void ontimeout();
    bool NetIsConn(void);
	
public:
	int calcrc(unsigned char *ptr, int count);
	int InitFrameQueue(Framequeue *q, U16 size);
	int InitHaveSendFrameQueue(Framequeue *q, U16 size);
	int InitPulseFrameQueue(Framequeue *q, U16 size);
	
    int DeFrameQueue(Framequeue *q, FrameBody *e);
    int EnHaveSendFrameQueue(Framequeue *q, FrameBody e);
    int DeHaveSendFrameQueue(Framequeue *q, FrameBody *e);
    int PeekQueue(Framequeue *q, FrameBody *e);
    int EmptyQueue(Framequeue *q);
    int QueueLen(Framequeue *q);
    int FullQueue(Framequeue *q);
    int EnPulseFrameQueue(Framequeue *q, FrameBody e);

    int AddDataToFrameQueue(U8 buf,Framequeue *q, FrameBody *e);
    void ComposeAckFrame(FrameTrafficCtrl* ctrl,FrameBody *elem,U8 IfOK,U8 FailReason);
    int SendAckFrame(FrameTrafficCtrl* ctrl,U8 IfOK,U8 FailReason);
    int DealPulseFrame(FrameBody frame,FrameTrafficCtrl* ctrl);
    int DealPeriodFrame(FrameBody frame,FrameTrafficCtrl* ctrl);
    int DealStateFrame(FrameBody frame,FrameTrafficCtrl* ctrl);
    int DealParameterFrame(FrameBody frame,FrameTrafficCtrl* ctrl);
    int DealAckFrame(FrameBody frame,FrameTrafficCtrl* ctrl);




    int GetHaveSendFrameQueue(Framequeue *q, FrameBody *e,FrameTrafficCtrl*ctrl);
    int MovehaverecvdFromQueue(Framequeue *q,FrameTrafficCtrl*ctrl);
    void ComposePeriodFrame(FrameTrafficCtrl* ctrl,FrameBody *elem);
	int ParseNetFrame(U8 *buf,FrameBody *frame,FrameTrafficCtrl* ctrl);
	int DealNetFrame(FrameBody frame,FrameTrafficCtrl* ctrl);
	void RecvFrame(FrameBody buffer);

signals:
	//void emitSendFrame(FrameBody buffer);
public slots:
    //void RecvFrame(FrameBody buffer);
private:
      unsigned int m_RevCount;
      bool m_IsConn;
};

#endif // !defined(AFX_NETCTRL_H__0D2BAA93_4574_45B4_8BAB_DCCB5D2E602A__INCLUDED_)

