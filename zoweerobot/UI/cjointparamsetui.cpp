#include "cjointparamsetui.h"
#include "ui_cjointparamsetui.h"

#include <QTimer>
#include <QFileDialog>
#include <QMessageBox>

#include "GlobalVariableType.h"
#include "SystemSchedule.h"
#include "GlobalDefine.h"
#define USE_ARMPARAM
cjointparamsetui::cjointparamsetui(QWidget *parent) :
    CCommmomUIWidget(parent),
    ui(new Ui::cjointparamsetui)
{
    ui->setupUi(this);
}

cjointparamsetui::cjointparamsetui(QWidget *parent, SystemSchedule* schedule) :
    CCommmomUIWidget(parent),
    m_pScheduler(schedule),
    ui(new Ui::cjointparamsetui)
{
    ui->setupUi(this);
    //on_cancle_clicked();
    m_pScheduler->m_pSystemParameter->getsysparamfinished=1;
    m_pScheduler->m_pSystemParameter->getaxinofinished=1;
    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()),
            this, SLOT(updataData()));
    timer->start(100);
}

cjointparamsetui::~cjointparamsetui()
{
    delete ui;
}

void cjointparamsetui::updataData()
{
	if(m_pScheduler->m_pSystemParameter->getaxinofinished)
	{
		m_pScheduler->m_pSystemParameter->getaxinofinished=0;
		ui->axis1_slot->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->axisno[0]) );
		ui->axis2_slot->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->axisno[1]) );
		ui->axis3_slot->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->axisno[2]) );
		ui->axis4_slot->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->axisno[3]) );
		ui->axis5_slot->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->axisno[4]) );
		ui->axis6_slot->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->axisno[5]) );
		ui->axis1_canid->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->axisno[6]) );
		ui->axis2_canid->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->axisno[7]) );
		ui->axis3_canid->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->axisno[8]) );
		ui->axis4_canid->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->axisno[9]) );
		ui->axis5_canid->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->axisno[10]) );
		ui->axis6_canid->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->axisno[11]) );
		ui->battery_slot->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->axisno[12]) );
		ui->input_slot->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->axisno[13]) );
		ui->output_slot->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->axisno[14]) );
		
		
	}
#ifdef USE_ARMPARAM
	if(m_pScheduler->m_pSystemParameter->getsysparamfinished)
	{
		refresh_systemParam();
		m_pScheduler->m_pSystemParameter->getsysparamfinished=0;
	}
#endif
}


void cjointparamsetui::refresh_systemParam()
{

	ui->axis1maxvel->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pJointMaxVel]) );
	ui->axis2maxvel->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pJointMaxVel+1]) );
	ui->axis3maxvel->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pJointMaxVel+2]) );
	ui->axis4maxvel->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pJointMaxVel+3]) );
	ui->axis5maxvel->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pJointMaxVel+4]) );
	ui->axis6maxvel->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pJointMaxVel+5]) );
	ui->axis1maxacc->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pJointMaxAcc]) );
	ui->axis2maxacc->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pJointMaxAcc+1]) );
	ui->axis3maxacc->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pJointMaxAcc+2]) );
	ui->axis4maxacc->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pJointMaxAcc+3]) );
	ui->axis5maxacc->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pJointMaxAcc+4]) );
	ui->axis6maxacc->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pJointMaxAcc+5]) );
	
       ui->axis1maxdec->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pJointStopDec]) );
	ui->axis2maxdec->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pJointStopDec+1]) );
	ui->axis3maxdec->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pJointStopDec+2]) );
	ui->axis4maxdec->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pJointStopDec+3]) );
	ui->axis5maxdec->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pJointStopDec+4]) );
	ui->axis6maxdec->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pJointStopDec+5]) );
       ui->axis1poslimit->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pJointPLim]) );
	ui->axis2poslimit->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pJointPLim+1]) );
	ui->axis3poslimit->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pJointPLim+2]) );
	ui->axis4poslimit->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pJointPLim+3]) );
	ui->axis5poslimit->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pJointPLim+4]) );
	ui->axis6poslimit->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pJointPLim+5]) );

       ui->axis1neglimit->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pJointNLim]) );
	ui->axis2neglimit->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pJointNLim+1]) );
	ui->axis3neglimit->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pJointNLim+2]) );
	ui->axis4neglimit->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pJointNLim+3]) );
	ui->axis5neglimit->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pJointNLim+4]) );
	ui->axis6neglimit->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pJointNLim+5]) );

	 ui->axis1ratio->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pMotorRatio]) );
	ui->axis2ratio->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pMotorRatio+1]) );
	ui->axis3ratio->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pMotorRatio+2]) );
	ui->axis4ratio->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pMotorRatio+3]) );
	ui->axis5ratio->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pMotorRatio+4]) );
	ui->axis6ratio->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pMotorRatio+5]) );

	 ui->axis1precision->setText(QString("%1").arg((int)m_pScheduler->m_pSystemParameter->SystemParam[pMotorPrec]) );
	ui->axis2precision->setText(QString("%1").arg((int)m_pScheduler->m_pSystemParameter->SystemParam[pMotorPrec+1]) );
	ui->axis3precision->setText(QString("%1").arg((int)m_pScheduler->m_pSystemParameter->SystemParam[pMotorPrec+2]) );
	ui->axis4precision->setText(QString("%1").arg((int)m_pScheduler->m_pSystemParameter->SystemParam[pMotorPrec+3]) );
	ui->axis5precision->setText(QString("%1").arg((int)m_pScheduler->m_pSystemParameter->SystemParam[pMotorPrec+4]) );
	ui->axis6precision->setText(QString("%1").arg((int)m_pScheduler->m_pSystemParameter->SystemParam[pMotorPrec+5]) );

    char data=*((char *)&m_pScheduler->m_pSystemParameter->SystemParam[pMotorDir]);
       //if(*((char *)&m_pScheduler->m_pSystemParameter->SystemParam[pMotorDir])&0x1==0x1)
     if((data&0x1)==0x1)
    {
    	    ui->axis1dir->setText(QString("1") );
    }
    else
    {
    	    ui->axis1dir->setText(QString("0") );
    }

	 // if(*((char *)&m_pScheduler->m_pSystemParameter->SystemParam[pMotorDir])&0x2==0x2)
     if((data&0x2)==0x2)
    {
    	    ui->axis2dir->setText(QString("1") );
    }
    else
    {
    	    ui->axis2dir->setText(QString("0") );
    }

     // if(*((char *)&m_pScheduler->m_pSystemParameter->SystemParam[pMotorDir])&0x4==0x4)
      if((data&0x4)==0x4)
    {
    	    ui->axis3dir->setText(QString("1") );
    }
    else
    {
    	    ui->axis3dir->setText(QString("0") );
    }
	
     // if(*((char *)&m_pScheduler->m_pSystemParameter->SystemParam[pMotorDir])&0x8==0x8)
     if((data&0x8)==0x8)
    {
    	    ui->axis4dir->setText(QString("1") );
    }
    else
    {
    	    ui->axis4dir->setText(QString("0") );
    }

   // if(*((char *)&m_pScheduler->m_pSystemParameter->SystemParam[pMotorDir])&0x10==0x10)
    if((data&0x10)==0x10)
    {
    	    ui->axis5dir->setText(QString("1") );
    }
    else
    {
    	    ui->axis5dir->setText(QString("0") );
    }
	
     // if(*((char *)&m_pScheduler->m_pSystemParameter->SystemParam[pMotorDir])&0x20==0x20)
    if((data&0x20)==0x20)
    {
    	    ui->axis6dir->setText(QString("1") );
    }
    else
    {
    	    ui->axis6dir->setText(QString("0") );
    }

     ui->acctime->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pAccTime]) );
	ui->carmaxpvel->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pCarMaxPVel]) );
	ui->carmaxgvel->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pCarMaxGVel]) );
	ui->carmaxpacc->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pCarMaxPAcc]) );
       ui->carmaxgacc->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pCarMaxGAcc]) );
	ui->carstoppdec->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pCarStopPDec]) );
	ui->carstopgdec->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pCarStopGDec]) );
	
	ui->A1->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pLinkDimension]) );
	ui->A2->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pLinkDimension+1]) );
	ui->A3->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pLinkDimension+2]) );
	ui->D3->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pLinkDimension+3]) );
	ui->D4->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pLinkDimension+4]) );
	ui->Df->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pLinkDimension+5]) );
	
      

}

void cjointparamsetui::on_ok_clicked()
{
    int i=0;
   
    m_pScheduler->m_pSystemParameter->SystemParam[pJointMaxVel]=ui->axis1maxvel->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pJointMaxVel+1]=ui->axis2maxvel->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pJointMaxVel+2]=ui->axis3maxvel->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pJointMaxVel+3]=ui->axis4maxvel->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pJointMaxVel+4]=ui->axis5maxvel->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pJointMaxVel+5]=ui->axis6maxvel->text().toFloat();
	
    m_pScheduler->m_pSystemParameter->SystemParam[pJointMaxAcc]=ui->axis1maxacc->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pJointMaxAcc+1]=ui->axis2maxacc->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pJointMaxAcc+2]=ui->axis3maxacc->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pJointMaxAcc+3]=ui->axis4maxacc->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pJointMaxAcc+4]=ui->axis5maxacc->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pJointMaxAcc+5]=ui->axis6maxacc->text().toFloat();

    m_pScheduler->m_pSystemParameter->SystemParam[pJointStopDec]=ui->axis1maxdec->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pJointStopDec+1]=ui->axis2maxdec->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pJointStopDec+2]=ui->axis3maxdec->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pJointStopDec+3]=ui->axis4maxdec->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pJointStopDec+4]=ui->axis5maxdec->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pJointStopDec+5]=ui->axis6maxdec->text().toFloat();

    m_pScheduler->m_pSystemParameter->SystemParam[pJointPLim]=ui->axis1poslimit->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pJointPLim+1]=ui->axis2poslimit->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pJointPLim+2]=ui->axis3poslimit->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pJointPLim+3]=ui->axis4poslimit->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pJointPLim+4]=ui->axis5poslimit->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pJointPLim+5]=ui->axis6poslimit->text().toFloat();
	
    m_pScheduler->m_pSystemParameter->SystemParam[pJointNLim]=ui->axis1neglimit->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pJointNLim+1]=ui->axis2neglimit->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pJointNLim+2]=ui->axis3neglimit->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pJointNLim+3]=ui->axis4neglimit->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pJointNLim+4]=ui->axis5neglimit->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pJointNLim+5]=ui->axis6neglimit->text().toFloat();
	
    m_pScheduler->m_pSystemParameter->SystemParam[pMotorRatio]=ui->axis1ratio->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pMotorRatio+1]=ui->axis2ratio->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pMotorRatio+2]=ui->axis3ratio->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pMotorRatio+3]=ui->axis4ratio->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pMotorRatio+4]=ui->axis5ratio->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pMotorRatio+5]=ui->axis6ratio->text().toFloat();

    m_pScheduler->m_pSystemParameter->SystemParam[pMotorPrec]=ui->axis1precision->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pMotorPrec+1]=ui->axis2precision->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pMotorPrec+2]=ui->axis3precision->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pMotorPrec+3]=ui->axis4precision->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pMotorPrec+4]=ui->axis5precision->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pMotorPrec+5]=ui->axis6precision->text().toFloat();


    if(ui->axis1dir->text().toInt()>0)
    {
    	    *((int *)&m_pScheduler->m_pSystemParameter->SystemParam[pMotorDir])|=0x1;
    }
    else
    {
    	    *((int *)&m_pScheduler->m_pSystemParameter->SystemParam[pMotorDir])&=~0x1;
    }
	
     if(ui->axis2dir->text().toInt()>0)
    {
    	    *((int *)&m_pScheduler->m_pSystemParameter->SystemParam[pMotorDir])|=0x2;
    }
    else
    {
    	    *((int *)&m_pScheduler->m_pSystemParameter->SystemParam[pMotorDir])&=~0x2;
    }

     if(ui->axis3dir->text().toInt()>0)
    {
    	    *((int *)&m_pScheduler->m_pSystemParameter->SystemParam[pMotorDir])|=0x4;
    }
    else
    {
    	    *((int *)&m_pScheduler->m_pSystemParameter->SystemParam[pMotorDir])&=~0x4;
    }

     if(ui->axis4dir->text().toInt()>0)
    {
    	    *((int *)&m_pScheduler->m_pSystemParameter->SystemParam[pMotorDir])|=0x8;
    }
    else
    {
    	    *((int *)&m_pScheduler->m_pSystemParameter->SystemParam[pMotorDir])&=~0x8;
    }

    if(ui->axis5dir->text().toInt()>0)
    {
    	    *((int *)&m_pScheduler->m_pSystemParameter->SystemParam[pMotorDir])|=0x10;
    }
    else
    {
    	    *((int *)&m_pScheduler->m_pSystemParameter->SystemParam[pMotorDir])&=~0x10;
    }

     if(ui->axis6dir->text().toInt()>0)
    {
    	    *((int *)&m_pScheduler->m_pSystemParameter->SystemParam[pMotorDir])|=0x20;
    }
    else
    {
    	    *((int *)&m_pScheduler->m_pSystemParameter->SystemParam[pMotorDir])&=~0x20;
    }

	

    m_pScheduler->m_pSystemParameter->SystemParam[pAccTime]=ui->acctime->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pCarMaxPVel]=ui->carmaxpvel->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pCarMaxGVel]=ui->carmaxgvel->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pCarMaxPAcc]=ui->carmaxpacc->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pCarMaxGAcc]=ui->carmaxgacc->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pCarStopPDec]=ui->carstoppdec->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pCarStopGDec]=ui->carstopgdec->text().toFloat();

    m_pScheduler->m_pSystemParameter->SystemParam[pLinkDimension]=ui->A1->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pLinkDimension+1]=ui->A2->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pLinkDimension+2]=ui->A3->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pLinkDimension+3]=ui->D3->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pLinkDimension+4]=ui->D4->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pLinkDimension+5]=ui->Df->text().toFloat();

    m_pScheduler->WriteSystemParamInformation();
    m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/setParamer", "all");

}

void cjointparamsetui::on_cancle_clicked()
{
#ifdef USE_ARMPARAM
	m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/getsysparam", "yes");
#else
   
	m_pScheduler->ReadSystemParamInformation();
	ui->axis1maxvel->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pJointMaxVel]) );
	ui->axis2maxvel->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pJointMaxVel+1]) );
	ui->axis3maxvel->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pJointMaxVel+2]) );
	ui->axis4maxvel->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pJointMaxVel+3]) );
	ui->axis5maxvel->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pJointMaxVel+4]) );
	ui->axis6maxvel->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pJointMaxVel+5]) );

	ui->axis1maxacc->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pJointMaxAcc]) );
	ui->axis2maxacc->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pJointMaxAcc+1]) );
	ui->axis3maxacc->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pJointMaxAcc+2]) );
	ui->axis4maxacc->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pJointMaxAcc+3]) );
	ui->axis5maxacc->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pJointMaxAcc+4]) );
	ui->axis6maxacc->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pJointMaxAcc+5]) );
	
       ui->axis1maxdec->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pJointStopDec]) );
	ui->axis2maxdec->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pJointStopDec+1]) );
	ui->axis3maxdec->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pJointStopDec+2]) );
	ui->axis4maxdec->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pJointStopDec+3]) );
	ui->axis5maxdec->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pJointStopDec+4]) );
	ui->axis6maxdec->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pJointStopDec+5]) );

       ui->axis1poslimit->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pJointPLim]) );
	ui->axis2poslimit->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pJointPLim+1]) );
	ui->axis3poslimit->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pJointPLim+2]) );
	ui->axis4poslimit->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pJointPLim+3]) );
	ui->axis5poslimit->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pJointPLim+4]) );
	ui->axis6poslimit->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pJointPLim+5]) );

       ui->axis1neglimit->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pJointNLim]) );
	ui->axis2neglimit->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pJointNLim+1]) );
	ui->axis3neglimit->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pJointNLim+2]) );
	ui->axis4neglimit->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pJointNLim+3]) );
	ui->axis5neglimit->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pJointNLim+4]) );
	ui->axis6neglimit->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pJointNLim+5]) );

	 ui->axis1ratio->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pMotorRatio]) );
	ui->axis2ratio->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pMotorRatio+1]) );
	ui->axis3ratio->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pMotorRatio+2]) );
	ui->axis4ratio->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pMotorRatio+3]) );
	ui->axis5ratio->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pMotorRatio+4]) );
	ui->axis6ratio->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pMotorRatio+5]) );

	 ui->axis1precision->setText(QString("%1").arg((int)m_pScheduler->m_pSystemParameter->SystemParam[pMotorPrec]) );
	ui->axis2precision->setText(QString("%1").arg((int)m_pScheduler->m_pSystemParameter->SystemParam[pMotorPrec+1]) );
	ui->axis3precision->setText(QString("%1").arg((int)m_pScheduler->m_pSystemParameter->SystemParam[pMotorPrec+2]) );
	ui->axis4precision->setText(QString("%1").arg((int)m_pScheduler->m_pSystemParameter->SystemParam[pMotorPrec+3]) );
	ui->axis5precision->setText(QString("%1").arg((int)m_pScheduler->m_pSystemParameter->SystemParam[pMotorPrec+4]) );
	ui->axis6precision->setText(QString("%1").arg((int)m_pScheduler->m_pSystemParameter->SystemParam[pMotorPrec+5]) );

       if(*((int *)&m_pScheduler->m_pSystemParameter->SystemParam[pMotorDir])&0x1==0x1)
    {
    	    ui->axis1dir->setText(QString("1") );
    }
    else
    {
    	    ui->axis1dir->setText(QString("0") );
    }

	  if(*((int *)&m_pScheduler->m_pSystemParameter->SystemParam[pMotorDir])&0x2==0x2)
    {
    	    ui->axis2dir->setText(QString("1") );
    }
    else
    {
    	    ui->axis2dir->setText(QString("0") );
    }

      if(*((int *)&m_pScheduler->m_pSystemParameter->SystemParam[pMotorDir])&0x4==0x4)
    {
    	    ui->axis3dir->setText(QString("1") );
    }
    else
    {
    	    ui->axis3dir->setText(QString("0") );
    }
	
      if(*((int *)&m_pScheduler->m_pSystemParameter->SystemParam[pMotorDir])&0x8==0x8)
    {
    	    ui->axis4dir->setText(QString("1") );
    }
    else
    {
    	    ui->axis4dir->setText(QString("0") );
    }

      if(*((int *)&m_pScheduler->m_pSystemParameter->SystemParam[pMotorDir])&0x10==0x10)
    {
    	    ui->axis5dir->setText(QString("1") );
    }
    else
    {
    	    ui->axis5dir->setText(QString("0") );
    }

      if(*((int *)&m_pScheduler->m_pSystemParameter->SystemParam[pMotorDir])&0x20==0x20)
    {
    	    ui->axis6dir->setText(QString("1") );
    }
    else
    {
    	    ui->axis6dir->setText(QString("0") );
    }
	

     ui->acctime->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pAccTime]) );
	ui->carmaxpvel->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pCarMaxPVel]) );
	ui->carmaxgvel->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pCarMaxGVel]) );
	ui->carmaxpacc->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pCarMaxPAcc]) );
       ui->carmaxgacc->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pCarMaxGAcc]) );
	ui->carstoppdec->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pCarStopPDec]) );
	ui->carstopgdec->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pCarStopGDec]) );
	
	ui->A1->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pLinkDimension]) );
	ui->A2->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pLinkDimension+1]) );
	ui->A3->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pLinkDimension+2]) );
	ui->D3->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pLinkDimension+3]) );
	ui->D4->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pLinkDimension+4]) );
	ui->Df->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pLinkDimension+5]) );
	
#endif      

}

void cjointparamsetui::on_setlinkparam_clicked()
{
    m_pScheduler->m_pSystemParameter->SystemParam[pLinkDimension]=ui->A1->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pLinkDimension+1]=ui->A2->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pLinkDimension+2]=ui->A3->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pLinkDimension+3]=ui->D3->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pLinkDimension+4]=ui->D4->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pLinkDimension+5]=ui->Df->text().toFloat();
    m_pScheduler->WriteSystemParamInformation();
    m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/setParamer", "linkparam");

}

void cjointparamsetui::on_renewlinkparam_clicked()
{
#ifdef USE_ARMPARAM
	m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/getsysparam", "yes");
#else
    ui->A1->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->Robot_Link.A1));
    ui->A2->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->Robot_Link.A2));
    ui->A3->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->Robot_Link.A3));
    ui->D3->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->Robot_Link.D3));
    ui->D4->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->Robot_Link.D4));
    ui->Df->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->Robot_Link.Df));
#endif
}

void cjointparamsetui::on_axisetbutton_clicked()
{
     m_pScheduler->m_pSystemParameter->axisno[0]=ui->axis1_slot->text().toInt();
     m_pScheduler->m_pSystemParameter->axisno[1]=ui->axis2_slot->text().toInt();
     m_pScheduler->m_pSystemParameter->axisno[2]=ui->axis3_slot->text().toInt();
     m_pScheduler->m_pSystemParameter->axisno[3]=ui->axis4_slot->text().toInt();
     m_pScheduler->m_pSystemParameter->axisno[4]=ui->axis5_slot->text().toInt();
     m_pScheduler->m_pSystemParameter->axisno[5]=ui->axis6_slot->text().toInt();
     m_pScheduler->m_pSystemParameter->axisno[6]=ui->axis1_canid->text().toInt();
     m_pScheduler->m_pSystemParameter->axisno[7]=ui->axis2_canid->text().toInt();
     m_pScheduler->m_pSystemParameter->axisno[8]=ui->axis3_canid->text().toInt();
     m_pScheduler->m_pSystemParameter->axisno[9]=ui->axis4_canid->text().toInt();
     m_pScheduler->m_pSystemParameter->axisno[10]=ui->axis5_canid->text().toInt();
     m_pScheduler->m_pSystemParameter->axisno[11]=ui->axis6_canid->text().toInt();
	 
     m_pScheduler->m_pSystemParameter->axisno[12]=ui->battery_slot->text().toInt();
     m_pScheduler->m_pSystemParameter->axisno[13]=ui->input_slot->text().toInt();
     m_pScheduler->m_pSystemParameter->axisno[14]=ui->output_slot->text().toInt();
     m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/setaxisno", "yes");
}

void cjointparamsetui::on_axigetbutton_clicked()
{
    m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/getaxisno", "yes");
}
