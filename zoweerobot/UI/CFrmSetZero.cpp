#include "CFrmSetZero.h"
#include "ui_CFrmSetZero.h"
#include <QMessageBox>
#include "GlobalVariableType.h"
#include "SystemSchedule.h"

CFrmSetZero::CFrmSetZero(QWidget *parent) :
    CCommmomUIWidget(parent),
    ui(new Ui::CFrmSetZero)
{
    ui->setupUi(this);
}

CFrmSetZero::CFrmSetZero(QWidget *parent, SystemSchedule* schedule) :
    CCommmomUIWidget(parent),
    m_pScheduler(schedule),
    ui(new Ui::CFrmSetZero)
{
    ui->setupUi(this);

}

CFrmSetZero::~CFrmSetZero()
{
    delete ui;
}

// 每次进入界面时 调用
void CFrmSetZero::UpdateViewData(void)
{
    int i;

    CCommmomUIWidget::UpdateViewData();
}

// 定时更新的数据
void CFrmSetZero::TimerUpdateViewData(void)
{
    QString str1[MOF],str2[MOF];
    int i;

    CCommmomUIWidget::TimerUpdateViewData(); //

    for(i=0;i<MOF;i++){
        str1[i] = QString().sprintf(CoordFormat,m_pScheduler->m_pSystemParameter->coor_car_pos[i]);
        str2[i] = QString().sprintf(CoordFormat,m_pScheduler->m_pSystemParameter->coor_joint_pos[i]);
    }
    ui->x->setText(str1[0]);
    ui->y->setText(str1[1]);
    ui->z->setText(str1[2]);
    ui->w->setText(str1[3]);
    ui->p->setText(str1[4]);
    ui->r->setText(str1[5]);
    ui->m->setText(str1[6]);
    ui->A->setText(str2[0]);
    ui->B->setText(str2[1]);
    ui->C->setText(str2[2]);
    ui->D->setText(str2[3]);
    ui->E->setText(str2[4]);
    ui->F->setText(str2[5]);
    ui->G->setText(str2[6]);


     str1[0] = QString().sprintf("%10d",(int)m_pScheduler->m_pSystemParameter->zeropos[1]);
     ui->axis1_zero_single->setText(str1[0]);

     str1[0] = QString().sprintf("%10d",(int)m_pScheduler->m_pSystemParameter->zeropos[3]);
     ui->axis2_zero_single->setText(str1[0]);

     str1[0] = QString().sprintf("%10d",(int)m_pScheduler->m_pSystemParameter->zeropos[5]);
     ui->axis3_zero_single->setText(str1[0]);

     str1[0] = QString().sprintf("%10d",(int)m_pScheduler->m_pSystemParameter->zeropos[7]);
     ui->axis4_zero_single->setText(str1[0]);

     str1[0] = QString().sprintf("%10d",(int)m_pScheduler->m_pSystemParameter->zeropos[9]);
     ui->axis5_zero_single->setText(str1[0]);

     str1[0] = QString().sprintf("%10d",(int)m_pScheduler->m_pSystemParameter->zeropos[11]);
     ui->axis6_zero_single->setText(str1[0]);

     str1[0] = QString().sprintf("%10d",(int)m_pScheduler->m_pSystemParameter->zeropos[13]);
     ui->axis7_zero_single->setText(str1[0]);
}

void CFrmSetZero::on_setzeroaxis1_clicked()
{
    m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/setorian", "2");
}

void CFrmSetZero::on_setzeroaxis2_clicked()
{
    m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/setorian", "3");
}

void CFrmSetZero::on_setzeroaxis3_clicked()
{
    m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/setorian", "4");
}

void CFrmSetZero::on_setzeroaxis4_clicked()
{
    m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/setorian", "5");
}

void CFrmSetZero::on_setzeroaxis5_clicked()
{
    m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/setorian", "6");
}

void CFrmSetZero::on_setzeroaxis6_clicked()
{
    m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/setorian", "7");
}

void CFrmSetZero::on_setzeroaxis7_clicked()
{
    m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/setorian", "8");
}
