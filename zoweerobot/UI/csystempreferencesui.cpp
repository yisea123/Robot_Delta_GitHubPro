#include "csystempreferencesui.h"
#include "ui_csystempreferencesui.h"


#include <QFileDialog>
#include <QMessageBox>
#include <QTimer>
#include <QTextBlock>
#include "GlobalVariableType.h"
#include "SystemSchedule.h"

CSystemPreferencesUi::CSystemPreferencesUi(QWidget *parent) :
    CCommmomUIWidget(parent),
    ui(new Ui::CSystemPreferencesUi)
{
    ui->setupUi(this);
}

CSystemPreferencesUi::CSystemPreferencesUi(QWidget *parent, SystemSchedule* schedule) :
    CCommmomUIWidget(parent),
    m_pScheduler(schedule),
    ui(new Ui::CSystemPreferencesUi)
{
    ui->setupUi(this);
    init();
}

CSystemPreferencesUi::~CSystemPreferencesUi()
{
    delete ui;
}



void CSystemPreferencesUi::init()
{
  
    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()),
            this, SLOT(updataData()));
    timer->start(100);


}


void CSystemPreferencesUi::updataData()
{
	 if(m_pScheduler->m_pSystemParameter->sys_ctrl.statebit&0x1)
	 {
	 	ui->enablestate->setNum(1);
	 }
	 else
	 {
	 	ui->enablestate->setNum(0);
	 }

	 if(0x40==(m_pScheduler->m_pSystemParameter->sys_ctrl.statebit&0x60))
	 {
	 	ui->modeorderstate->setNum(2);
	 }
	 else if(0x20==(m_pScheduler->m_pSystemParameter->sys_ctrl.statebit&0x60))
	 {
	 	ui->modeorderstate->setNum(1);
	 }
	 else
	 {
	 	ui->modeorderstate->setNum(0);
	 }

	  if(0x80000==(m_pScheduler->m_pSystemParameter->sys_ctrl.statebit&0xc0000))
	 {
	 	ui->realmodestate->setNum(2);
	 }
	 else if(0x40000==(m_pScheduler->m_pSystemParameter->sys_ctrl.statebit&0xc0000))
	 {
	 	ui->realmodestate->setNum(1);
	 }
	 else
	 {
	 	ui->realmodestate->setNum(0);
	 }

	  if(0x200000==(m_pScheduler->m_pSystemParameter->sys_ctrl.statebit&0x300000))
	 {
	 	ui->tempmodestate->setNum(2);
	 }
	 else if(0x100000==(m_pScheduler->m_pSystemParameter->sys_ctrl.statebit&0x300000))
	 {
	 	ui->tempmodestate->setNum(1);
	 }
	 else
	 {
	 	ui->tempmodestate->setNum(0);
	 }

     if(m_pScheduler->m_pSystemParameter->sys_ctrl.statebit&0x80)
	 {
	 	ui->inmotionstate->setNum(1);
	 }
	 else
	 {
	 	ui->inmotionstate->setNum(0);
	 }

      if(m_pScheduler->m_pSystemParameter->sys_ctrl.statebit&0x200)
	 {
	 	ui->holdstate->setNum(1);
	 }
	 else
	 {
	 	ui->holdstate->setNum(0);
	 }

     if(m_pScheduler->m_pSystemParameter->sys_ctrl.statebit&0x10000)
	 {
	 	ui->stoporder->setNum(1);
	 }
	 else
	 {
	 	ui->stoporder->setNum(0);
	 }

      if(m_pScheduler->m_pSystemParameter->sys_ctrl.statebit&0x400)
	 {
	 	ui->stopstate->setNum(1);
	 }
	 else
	 {
	 	ui->stopstate->setNum(0);
	 }

       if(m_pScheduler->m_pSystemParameter->sys_ctrl.statebit&0x100)
	 {
	 	ui->programstate->setNum(1);
	 }
	 else
	 {
	 	ui->programstate->setNum(0);
	 }

       if(m_pScheduler->m_pSystemParameter->sys_ctrl.statebit&0x800)
	 {
	 	ui->limitstate->setNum(1);
	 }
	 else
	 {
	 	ui->limitstate->setNum(0);
	 }

       if(m_pScheduler->m_pSystemParameter->sys_ctrl.statebit&0x1000)
	 {
	 	ui->commuciatestate->setNum(1);
	 }
	 else
	 {
	 	ui->commuciatestate->setNum(0);
	 }

        if(m_pScheduler->m_pSystemParameter->sys_ctrl.statebit&0x2000)
	 {
	 	ui->runwarnstate->setNum(1);
	 }
	 else
	 {
	 	ui->runwarnstate->setNum(0);
	 }

         if(m_pScheduler->m_pSystemParameter->sys_ctrl.statebit&0x4000)
	 {
	 	ui->servorwarnstate->setNum(1);
	 }
	 else
	 {
	 	ui->servorwarnstate->setNum(0);
	 }

         if(m_pScheduler->m_pSystemParameter->sys_ctrl.statebit&0x8000)
	 {
	 	ui->planwarnstate->setNum(1);
	 }
	 else
	 {
	 	ui->planwarnstate->setNum(0);
	 }

	 if(m_pScheduler->m_pSystemParameter->sys_ctrl.statebit&0x2)
	 {
	 	ui->netled->setNum(1);
	 }
	 else
	 {
	 	ui->netled->setNum(0);
	 }

	  if(m_pScheduler->m_pSystemParameter->sys_ctrl.statebit&0x4)
	 {
	 	ui->canled->setNum(1);
	 }
	 else
	 {
	 	ui->canled->setNum(0);
	 }

	 ui->movctrlversion->setText(&m_pScheduler->m_pSystemParameter->m_cVersion[0]);
	 ui->curtemp->setNum(m_pScheduler->m_pSystemParameter->m_battery[0]);
	 ui->curbatterystate->setNum(m_pScheduler->m_pSystemParameter->m_battery[1]);
	 ui->fanstarttemp->setNum(m_pScheduler->m_pSystemParameter->m_battery[2]);
	 ui->curfanspeedper->setNum(m_pScheduler->m_pSystemParameter->m_battery[3]);
	 ui->curvoltage->setNum(m_pScheduler->m_pSystemParameter->m_battery[4]);
	 ui->batteryvoltage->setNum(((float)(m_pScheduler->m_pSystemParameter->m_battery[5]))/10);
	 
	 ui->sendframeno->setNum(m_pScheduler->m_pSystemParameter->sys_ctrl.sendframeno);
	 ui->recvframeno->setNum(m_pScheduler->m_pSystemParameter->sys_ctrl.recvframeno);
	 ui->succedrecvno->setNum(m_pScheduler->m_pSystemParameter->sys_ctrl.succedrecvno);
	/* ui->current1->setNum((int)m_pScheduler->m_pSystemParameter->m_current[0]);
	 ui->current2->setNum((int)m_pScheduler->m_pSystemParameter->m_current[1]);
	 ui->current3->setNum((int)m_pScheduler->m_pSystemParameter->m_current[2]);
	 ui->current4->setNum((int)m_pScheduler->m_pSystemParameter->m_current[3]);
	 ui->current5->setNum((int)m_pScheduler->m_pSystemParameter->m_current[4]);
	 ui->current6->setNum((int)m_pScheduler->m_pSystemParameter->m_current[5]);*/

        if(m_pScheduler->m_pSystemParameter->m_current[0]&0x800)
        {
	 	ui->enable1->setNum(1);
        }
	else
	{
		ui->enable1->setNum(0);
	}

	 if(m_pScheduler->m_pSystemParameter->m_current[1]&0x800)
        {
	 	ui->enable2->setNum(1);
        }
	else
	{
		ui->enable2->setNum(0);
	}

	 if(m_pScheduler->m_pSystemParameter->m_current[2]&0x800)
        {
	 	ui->enable3->setNum(1);
        }
	else
	{
		ui->enable3->setNum(0);
	}
	 if(m_pScheduler->m_pSystemParameter->m_current[3]&0x800)
        {
	 	ui->enable4->setNum(1);
        }
	else
	{
		ui->enable4->setNum(0);
	}
	 if(m_pScheduler->m_pSystemParameter->m_current[4]&0x800)
        {
	 	ui->enable5->setNum(1);
        }
	else
	{
		ui->enable5->setNum(0);
	}
	 if(m_pScheduler->m_pSystemParameter->m_current[5]&0x800)
        {
	 	ui->enable6->setNum(1);
        }
	else
	{
		ui->enable6->setNum(0);
	}

	 if(m_pScheduler->m_pSystemParameter->m_current[0]&0x100)
        {
	 	ui->dspwarn1->setNum(1);
        }
	else
	{
		ui->dspwarn1->setNum(0);
	}

	 if(m_pScheduler->m_pSystemParameter->m_current[1]&0x100)
        {
	 	ui->dspwarn2->setNum(1);
        }
	else
	{
		ui->dspwarn2->setNum(0);
	}

	 if(m_pScheduler->m_pSystemParameter->m_current[2]&0x100)
        {
	 	ui->dspwarn3->setNum(1);
        }
	else
	{
		ui->dspwarn3->setNum(0);
	}
	 if(m_pScheduler->m_pSystemParameter->m_current[3]&0x100)
        {
	 	ui->dspwarn4->setNum(1);
        }
	else
	{
		ui->dspwarn4->setNum(0);
	}
	 if(m_pScheduler->m_pSystemParameter->m_current[4]&0x100)
        {
	 	ui->dspwarn5->setNum(1);
        }
	else
	{
		ui->dspwarn5->setNum(0);
	}
	 if(m_pScheduler->m_pSystemParameter->m_current[5]&0x100)
        {
	 	ui->dspwarn6->setNum(1);
        }
	else
	{
		ui->dspwarn6->setNum(0);
	}

	ui->encwarn1->setNum((int)m_pScheduler->m_pSystemParameter->m_current[0]&0xff);
	 ui->encwarn2->setNum((int)m_pScheduler->m_pSystemParameter->m_current[1]&0xff);
	 ui->encwarn3->setNum((int)m_pScheduler->m_pSystemParameter->m_current[2]&0xff);
	 ui->encwarn4->setNum((int)m_pScheduler->m_pSystemParameter->m_current[3]&0xff);
	 ui->encwarn5->setNum((int)m_pScheduler->m_pSystemParameter->m_current[4]&0xff);
	 ui->encwarn6->setNum((int)m_pScheduler->m_pSystemParameter->m_current[5]&0xff);
	 
	ui->servowarn1->setNum((int)(m_pScheduler->m_pSystemParameter->m_current[0]>>16)&0xffff);
	 ui->servowarn2->setNum((int)(m_pScheduler->m_pSystemParameter->m_current[1]>>16)&0xffff);
	 ui->servowarn3->setNum((int)(m_pScheduler->m_pSystemParameter->m_current[2]>>16)&0xffff);
	 ui->servowarn4->setNum((int)(m_pScheduler->m_pSystemParameter->m_current[3]>>16)&0xffff);
	 ui->servowarn5->setNum((int)(m_pScheduler->m_pSystemParameter->m_current[4]>>16)&0xffff);
	 ui->servowarn6->setNum((int)(m_pScheduler->m_pSystemParameter->m_current[5]>>16)&0xffff);
	 
	 
	 
    
}

void CSystemPreferencesUi::on_getversion_clicked()
{
	 m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/GetVersion", "yes");
}

void CSystemPreferencesUi::on_getbattery_clicked()
{
     m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/GetBattery", "yes");
}

void CSystemPreferencesUi::on_setbattery_clicked()
{
     bool ok;
     m_pScheduler->m_pSystemParameter->m_setbattery[0]=ui->starttemp->text().toInt(&ok, 10);
	 m_pScheduler->m_pSystemParameter->m_setbattery[1]=ui->speedper->text().toInt(&ok, 10);
	 m_pScheduler->m_pSystemParameter->m_setbattery[2]=ui->voltage->text().toInt(&ok, 10);
     m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/SetBattery", "yes");
}
