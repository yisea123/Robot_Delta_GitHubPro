#include "cusercoorsetui.h"
#include "ui_cusercoorsetui.h"

#include <QTimer>
#include <QFileDialog>
#include <QMessageBox>

#include "GlobalVariableType.h"
#include "SystemSchedule.h"

cusercoorsetui::cusercoorsetui(QWidget *parent) :
    CCommmomUIWidget(parent),
    ui(new Ui::cusercoorsetui)
{
    ui->setupUi(this);
}

cusercoorsetui::cusercoorsetui(QWidget *parent,SystemSchedule* schedule) :
    CCommmomUIWidget(parent),
    m_pScheduler(schedule),
    ui(new Ui::cusercoorsetui)
{
    ui->setupUi(this);
    init();
}

cusercoorsetui::~cusercoorsetui()
{
    delete ui;
}




void cusercoorsetui::init()
{
     memset(stepnum,0,sizeof(stepnum));
    
    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()),
            this, SLOT(updataData()));
    timer->start(100);


}




void cusercoorsetui::updataData()
{
     ui->x->setNum(m_pScheduler->m_pSystemParameter->coor_car_pos[0]);
	 ui->y->setNum(m_pScheduler->m_pSystemParameter->coor_car_pos[1]);
	 ui->z->setNum(m_pScheduler->m_pSystemParameter->coor_car_pos[2]);
	 ui->r->setNum(m_pScheduler->m_pSystemParameter->coor_car_pos[3]);

	 ui->A->setNum(m_pScheduler->m_pSystemParameter->coor_joint_pos[0]);
	 ui->B->setNum(m_pScheduler->m_pSystemParameter->coor_joint_pos[1]);
	 ui->C->setNum(m_pScheduler->m_pSystemParameter->coor_joint_pos[2]);
	 ui->D->setNum(m_pScheduler->m_pSystemParameter->coor_joint_pos[3]);

        m_userindex = ui->userindex->value();
        ui->user_a->setNum(m_pScheduler->m_pSystemParameter->m_userParam[m_userindex].X);
        ui->user_b->setNum(m_pScheduler->m_pSystemParameter->m_userParam[m_userindex].Y);
	 ui->user_c->setNum(m_pScheduler->m_pSystemParameter->m_userParam[m_userindex].Z);
        ui->user_d->setNum(m_pScheduler->m_pSystemParameter->m_userParam[m_userindex].W);
   

	 
}

int cusercoorsetui::GetUserTrans(IN  CartesianPose* i_user1,IN  CartesianPose*  i_user2,IN  CartesianPose* i_CPos_1, IN  CartesianPose* i_CPos_2, 
	      OUT CartesianPose* i_usertrans)
{

       double a,b,c,d;
	double x1,y1,xv1,yv1,x2,y2,xv2,yv2;
	double gdJRad[2];
	double k1,k2,deltax,deltay;

	x1=i_CPos_1->X;
	x2=i_CPos_2->X;
	y1=i_CPos_1->Y;
	y2=i_CPos_2->Y;

	xv1=i_user1->X;
	xv2=i_user2->X;
	yv1=i_user1->Y;
	yv2=i_user2->Y;

	a=((x1-x2)*(xv1-xv2)+(y1-y2)*(yv1-yv2))/((xv1-xv2)*(xv1-xv2)+(yv1-yv2)*(yv1-yv2));
	b=((y1-y2)*(xv1-xv2)-(x1-x2)*(yv1-yv2))/((xv1-xv2)*(xv1-xv2)+(yv1-yv2)*(yv1-yv2));
	c=x1-a*xv1+b*yv1;
	d=y1-b*xv1-a*yv1;


    
	i_usertrans->X=a;
	i_usertrans->Y=b;
	i_usertrans->Z=c;
	i_usertrans->W=d;
	
}
void cusercoorsetui::on_biaoding1_clicked()
{
       m_cpos1.X=(double)m_pScheduler->m_pSystemParameter->coor_car_pos[0];
	m_cpos1.Y=(double)m_pScheduler->m_pSystemParameter->coor_car_pos[1];
	m_user1.X=ui->userx1->text().toDouble();
	m_user1.Y=ui->usery1->text().toDouble();
   
}

void cusercoorsetui::on_biaoding2_clicked()
{
	m_cpos2.X=(double)m_pScheduler->m_pSystemParameter->coor_car_pos[0];
	m_cpos2.Y=(double)m_pScheduler->m_pSystemParameter->coor_car_pos[1];
	m_user2.X=ui->userx2->text().toDouble();
	m_user2.Y=ui->usery2->text().toDouble();
	m_userindex = ui->userindex->value();
	GetUserTrans(&m_user1,&m_user2,&m_cpos1, &m_cpos2, &m_usertrans);
	m_pScheduler->m_pSystemParameter->m_userParam[m_userindex]=m_usertrans;
	m_pScheduler->WriteUserInformation();
}
