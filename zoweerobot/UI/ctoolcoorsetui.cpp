#include "ctoolcoorsetui.h"
#include "ui_ctoolcoorsetui.h"

#include <QTimer>
#include <QFileDialog>
#include <QMessageBox>
#include "GlobalDefine.h"
#include "GlobalVariableType.h"
#include "SystemSchedule.h"

CToolCoorSetUi::CToolCoorSetUi(QWidget *parent) :
    CCommmomUIWidget(parent),
    ui(new Ui::CToolCoorSetUi)
{
    ui->setupUi(this);
}

CToolCoorSetUi::CToolCoorSetUi(QWidget *parent, SystemSchedule* schedule) :
    CCommmomUIWidget(parent),
    m_pScheduler(schedule),
    ui(new Ui::CToolCoorSetUi)
{
    ui->setupUi(this);
    init();
}


CToolCoorSetUi::~CToolCoorSetUi()
{
    delete ui;
}


void CToolCoorSetUi::init()
{
     memset(stepnum,0,sizeof(stepnum));
    
    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()),
            this, SLOT(updataData()));
    timer->start(100);


}

void CToolCoorSetUi::updataData()
{
     /*ui->x->setNum(m_pScheduler->m_pSystemParameter->coor_car_pos[0]);
	 ui->y->setNum(m_pScheduler->m_pSystemParameter->coor_car_pos[1]);
	 ui->z->setNum(m_pScheduler->m_pSystemParameter->coor_car_pos[2]);
	 ui->r->setNum(m_pScheduler->m_pSystemParameter->coor_car_pos[3]);

	 ui->A->setNum(m_pScheduler->m_pSystemParameter->coor_joint_pos[0]);
	 ui->B->setNum(m_pScheduler->m_pSystemParameter->coor_joint_pos[1]);
	 ui->C->setNum(m_pScheduler->m_pSystemParameter->coor_joint_pos[2]);
	 ui->D->setNum(m_pScheduler->m_pSystemParameter->coor_joint_pos[3]);*/
#ifdef PROGRAM_USELVDS
        ui->zero_multi->setText(QString::fromLocal8Bit("ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½È¦"));
        ui->zero_single->setText(QString::fromLocal8Bit("ï¿½ï¿½ï¿½ãµ¥È¦"));
	  ui->axis1_zero_multi->setNum((int)m_pScheduler->m_pSystemParameter->zeropos[0]);
	 ui->axis1_zero_single->setNum((int)m_pScheduler->m_pSystemParameter->zeropos[1]);
	 ui->axis2_zero_multi->setNum((int)m_pScheduler->m_pSystemParameter->zeropos[2]);
	 ui->axis2_zero_single->setNum((int)m_pScheduler->m_pSystemParameter->zeropos[3]);
	  ui->axis3_zero_multi->setNum((int)m_pScheduler->m_pSystemParameter->zeropos[4]);
	 ui->axis3_zero_single->setNum((int)m_pScheduler->m_pSystemParameter->zeropos[5]);
	 ui->axis4_zero_multi->setNum((int)m_pScheduler->m_pSystemParameter->zeropos[6]);
	 ui->axis4_zero_single->setNum((int)m_pScheduler->m_pSystemParameter->zeropos[7]);
	 ui->axis5_zero_multi->setNum((int)m_pScheduler->m_pSystemParameter->zeropos[8]);
	 ui->axis5_zero_single->setNum((int)m_pScheduler->m_pSystemParameter->zeropos[9]);
	 ui->axis6_zero_multi->setNum((int)m_pScheduler->m_pSystemParameter->zeropos[10]);
	 ui->axis6_zero_single->setNum((int)m_pScheduler->m_pSystemParameter->zeropos[11]);
#else
	ui->real_multi->setText(QString::fromLocal8Bit("                         "));
     ui->real_single->setText(QString::fromLocal8Bit("Êµ¼ÊÎ»ÖÃ"));
       //ui->axis1_multi->setNum(0);
     ui->axis1_single->setNum((int)m_pScheduler->m_pSystemParameter->realaxis[1]);
      //ui->axis2_multi->setNum(0);
     ui->axis2_single->setNum((int)m_pScheduler->m_pSystemParameter->realaxis[3]);
       //ui->axis3_multi->setNum(0);
     ui->axis3_single->setNum((int)m_pScheduler->m_pSystemParameter->realaxis[5]);
      //ui->axis4_multi->setNum(0);
     ui->axis4_single->setNum((int)m_pScheduler->m_pSystemParameter->realaxis[7]);
      // ui->axis5_multi->setNum(0);
     ui->axis5_single->setNum((int)m_pScheduler->m_pSystemParameter->realaxis[9]);
      //ui->axis6_multi->setNum(0);
     ui->axis6_single->setNum((int)m_pScheduler->m_pSystemParameter->realaxis[11]);

	ui->zero_multi->setText(QString::fromLocal8Bit("                       "));
        ui->zero_single->setText(QString::fromLocal8Bit("ÁãµãÎ»ÖÃ"));
	 //ui->axis1_zero_multi->setNum(0);
	 ui->axis1_zero_single->setNum((int)m_pScheduler->m_pSystemParameter->zeropos[1]);
	  //ui->axis2_zero_multi->setNum(0);
	 ui->axis2_zero_single->setNum((int)m_pScheduler->m_pSystemParameter->zeropos[3]);
	  // ui->axis3_zero_multi->setNum(0);
	 ui->axis3_zero_single->setNum((int)m_pScheduler->m_pSystemParameter->zeropos[5]);
	  //ui->axis4_zero_multi->setNum(0);
	 ui->axis4_zero_single->setNum((int)m_pScheduler->m_pSystemParameter->zeropos[7]);
	  //ui->axis5_zero_multi->setNum(0);
	 ui->axis5_zero_single->setNum((int)m_pScheduler->m_pSystemParameter->zeropos[9]);
	  //ui->axis6_zero_multi->setNum(0);
	 ui->axis6_zero_single->setNum((int)m_pScheduler->m_pSystemParameter->zeropos[11]);
#endif
	/* ui->A1->setNum(m_pScheduler->m_pSystemParameter->Robot_Link.A1);
	 ui->A2->setNum(m_pScheduler->m_pSystemParameter->Robot_Link.A2);
	 ui->A3->setNum(m_pScheduler->m_pSystemParameter->Robot_Link.A3);
	 ui->D3->setNum(m_pScheduler->m_pSystemParameter->Robot_Link.D3);
	 ui->D4->setNum(m_pScheduler->m_pSystemParameter->Robot_Link.D4);
	 ui->Df->setNum(m_pScheduler->m_pSystemParameter->Robot_Link.Df);

	  

        m_curtoolindex = ui->toolindex->value();
        ui->tool_x->setNum(m_pScheduler->m_pSystemParameter->ToolCalResult[m_curtoolindex].X);
        ui->tool_y->setNum(m_pScheduler->m_pSystemParameter->ToolCalResult[m_curtoolindex].Y);
    if( m_curtoolindex <21)
    {
        if(!stepnum[m_curtoolindex])
        {
            ui->step->setText(QString::fromLocal8Bit("ï¿½ï¿½Ò»ï¿½ï¿½,ï¿½Æ¶ï¿½ï¿½ï¿½ï¿½ê¶¨ï¿½ï¿½"));

        }
        else
        {
            ui->step->setText(QString::fromLocal8Bit("ï¿½Ú¶ï¿½ï¿½ï¿½,ï¿½ï¿½×ªRï¿½á£¬ï¿½Ù´ï¿½ï¿½Æ¶ï¿½ï¿½ï¿½ï¿½ê¶¨ï¿½ï¿½"));
        }
    }*/

	 
}
/*void CToolCoorSetUi::on_biaoding_clicked()
{
 
     m_curtoolindex = ui->toolindex->value();
     if (m_curtoolindex<21)
     {
         if(!stepnum[m_curtoolindex])
         {
            m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/setToolstep1", QString::number(m_curtoolindex));
            stepnum[m_curtoolindex]=1;
         }
         else
         {
            m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/setToolstep2", QString::number(m_curtoolindex));
            stepnum[m_curtoolindex]=0;
         }
     }
}

void CToolCoorSetUi::on_currenttool_valueChanged(int arg1)
{
    m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/setcurTool", QString::number(arg1));
}

void CToolCoorSetUi::on_zerostep1_clicked()
{
	m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/setZerostep", "1");
}

void CToolCoorSetUi::on_zerostep2_clicked()
{
	m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/setZerostep", "2");
}

void CToolCoorSetUi::on_zerostep3_clicked()
{
	m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/setZerostep", "3");
}

void CToolCoorSetUi::on_zerostep4_clicked()
{
	m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/setZerostep", "4");
}*/

void CToolCoorSetUi::on_setzeroaxis1_clicked()
{
	m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/setorian", "2");
}

void CToolCoorSetUi::on_setzeroaxis2_clicked()
{
	m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/setorian", "3");
}

void CToolCoorSetUi::on_setzeroaxis3_clicked()
{
	m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/setorian", "4");
}

void CToolCoorSetUi::on_setzeroaxis4_clicked()
{
	m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/setorian", "5");
}

void CToolCoorSetUi::on_setzeroaxis5_clicked()
{
	m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/setorian", "6");
}

void CToolCoorSetUi::on_setzeroaxis6_clicked()
{
	m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/setorian", "7");
}
