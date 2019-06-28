#include "cmanualmachiningstateui.h"
#include "ui_cmanualmachiningstateui.h"
#include "SystemSchedule.h"

#include <QTimer>

CManualMachiningStateUi::CManualMachiningStateUi(QWidget *parent) :
    CCommmomUIWidget(parent),
    ui(new Ui::CManualMachiningStateUi)
{
    ui->setupUi(this);
}

CManualMachiningStateUi::CManualMachiningStateUi(QWidget *parent, SystemSchedule* schedule) :
    CCommmomUIWidget(parent),
    m_pScheduler(schedule),
    ui(new Ui::CManualMachiningStateUi)
{
    ui->setupUi(this);
    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()),
            this, SLOT(updataData()));
    timer->start(100);

    //init();
    m_tempAxis = DAxis(0, 0, 0, 0, 0 , 0);
}

CManualMachiningStateUi::~CManualMachiningStateUi()
{
    delete ui;
}

void CManualMachiningStateUi::updataData()
{
    if(m_pScheduler->m_pSystemParameter->sys_ctrl.recvinput[0]&0x1)  ui->I1_0->setCheckState(Qt::Checked);//Qt::Unchecked
    else 	ui->I1_0->setCheckState(Qt::Unchecked);

    if(m_pScheduler->m_pSystemParameter->sys_ctrl.recvinput[0]&0x2)  ui->I1_1->setCheckState(Qt::Checked);//Qt::Unchecked
    else 	ui->I1_1->setCheckState(Qt::Unchecked);

    if(m_pScheduler->m_pSystemParameter->sys_ctrl.recvinput[0]&0x4)  ui->I1_2->setCheckState(Qt::Checked);//Qt::Unchecked
    else 	ui->I1_2->setCheckState(Qt::Unchecked);

    if(m_pScheduler->m_pSystemParameter->sys_ctrl.recvinput[0]&0x8)  ui->I1_3->setCheckState(Qt::Checked);//Qt::Unchecked
    else 	ui->I1_3->setCheckState(Qt::Unchecked);

    if(m_pScheduler->m_pSystemParameter->sys_ctrl.recvinput[0]&0x10)  ui->I1_4->setCheckState(Qt::Checked);//Qt::Unchecked
    else 	ui->I1_4->setCheckState(Qt::Unchecked);

    if(m_pScheduler->m_pSystemParameter->sys_ctrl.recvinput[0]&0x20)  ui->I1_5->setCheckState(Qt::Checked);//Qt::Unchecked
    else 	ui->I1_5->setCheckState(Qt::Unchecked);

    if(m_pScheduler->m_pSystemParameter->sys_ctrl.recvinput[0]&0x40)  ui->I1_6->setCheckState(Qt::Checked);//Qt::Unchecked
    else 	ui->I1_6->setCheckState(Qt::Unchecked);

    if(m_pScheduler->m_pSystemParameter->sys_ctrl.recvinput[0]&0x80)  ui->I1_7->setCheckState(Qt::Checked);//Qt::Unchecked
    else 	ui->I1_7->setCheckState(Qt::Unchecked);

   if(m_pScheduler->m_pSystemParameter->sys_ctrl.recvinput[1]&0x1)  ui->I1_8->setCheckState(Qt::Checked);//Qt::Unchecked
    else 	ui->I1_8->setCheckState(Qt::Unchecked);

    if(m_pScheduler->m_pSystemParameter->sys_ctrl.recvinput[1]&0x2)  ui->I1_9->setCheckState(Qt::Checked);//Qt::Unchecked
    else 	ui->I1_9->setCheckState(Qt::Unchecked);

    if(m_pScheduler->m_pSystemParameter->sys_ctrl.recvinput[1]&0x4)  ui->I1_10->setCheckState(Qt::Checked);//Qt::Unchecked
    else 	ui->I1_10->setCheckState(Qt::Unchecked);

    if(m_pScheduler->m_pSystemParameter->sys_ctrl.recvinput[1]&0x8)  ui->I1_11->setCheckState(Qt::Checked);//Qt::Unchecked
    else 	ui->I1_11->setCheckState(Qt::Unchecked);

    if(m_pScheduler->m_pSystemParameter->sys_ctrl.recvinput[1]&0x10)  ui->I1_12->setCheckState(Qt::Checked);//Qt::Unchecked
    else 	ui->I1_12->setCheckState(Qt::Unchecked);

    if(m_pScheduler->m_pSystemParameter->sys_ctrl.recvinput[1]&0x20)  ui->I1_13->setCheckState(Qt::Checked);//Qt::Unchecked
    else 	ui->I1_13->setCheckState(Qt::Unchecked);

    if(m_pScheduler->m_pSystemParameter->sys_ctrl.recvinput[1]&0x40)  ui->I1_14->setCheckState(Qt::Checked);//Qt::Unchecked
    else 	ui->I1_14->setCheckState(Qt::Unchecked);

    if(m_pScheduler->m_pSystemParameter->sys_ctrl.recvinput[1]&0x80)  ui->I1_15->setCheckState(Qt::Checked);//Qt::Unchecked
    else 	ui->I1_15->setCheckState(Qt::Unchecked);

    if(m_pScheduler->m_pSystemParameter->sys_ctrl.recvinput[2]&0x1)  ui->I1_16->setCheckState(Qt::Checked);//Qt::Unchecked
    else 	ui->I1_16->setCheckState(Qt::Unchecked);

    if(m_pScheduler->m_pSystemParameter->sys_ctrl.recvinput[2]&0x2)  ui->I1_17->setCheckState(Qt::Checked);//Qt::Unchecked
    else 	ui->I1_17->setCheckState(Qt::Unchecked);

    if(m_pScheduler->m_pSystemParameter->sys_ctrl.recvinput[2]&0x4)  ui->I1_18->setCheckState(Qt::Checked);//Qt::Unchecked
    else 	ui->I1_18->setCheckState(Qt::Unchecked);

    if(m_pScheduler->m_pSystemParameter->sys_ctrl.recvinput[2]&0x8)  ui->I1_19->setCheckState(Qt::Checked);//Qt::Unchecked
    else 	ui->I1_19->setCheckState(Qt::Unchecked);

    if(m_pScheduler->m_pSystemParameter->sys_ctrl.recvinput[2]&0x10)  ui->I1_20->setCheckState(Qt::Checked);//Qt::Unchecked
    else 	ui->I1_20->setCheckState(Qt::Unchecked);

    if(m_pScheduler->m_pSystemParameter->sys_ctrl.recvinput[2]&0x20)  ui->I1_21->setCheckState(Qt::Checked);//Qt::Unchecked
    else 	ui->I1_21->setCheckState(Qt::Unchecked);

    if(m_pScheduler->m_pSystemParameter->sys_ctrl.recvinput[2]&0x40)  ui->I1_22->setCheckState(Qt::Checked);//Qt::Unchecked
    else 	ui->I1_22->setCheckState(Qt::Unchecked);

    if(m_pScheduler->m_pSystemParameter->sys_ctrl.recvinput[2]&0x80)  ui->I1_23->setCheckState(Qt::Checked);//Qt::Unchecked
    else 	ui->I1_23->setCheckState(Qt::Unchecked);

   if(m_pScheduler->m_pSystemParameter->sys_ctrl.recvinput[3]&0x1)  ui->I1_24->setCheckState(Qt::Checked);//Qt::Unchecked
    else 	ui->I1_24->setCheckState(Qt::Unchecked);

    if(m_pScheduler->m_pSystemParameter->sys_ctrl.recvinput[3]&0x2)  ui->I1_25->setCheckState(Qt::Checked);//Qt::Unchecked
    else 	ui->I1_25->setCheckState(Qt::Unchecked);

    if(m_pScheduler->m_pSystemParameter->sys_ctrl.recvinput[3]&0x4)  ui->I1_26->setCheckState(Qt::Checked);//Qt::Unchecked
    else 	ui->I1_26->setCheckState(Qt::Unchecked);

    if(m_pScheduler->m_pSystemParameter->sys_ctrl.recvinput[3]&0x8)  ui->I1_27->setCheckState(Qt::Checked);//Qt::Unchecked
    else 	ui->I1_27->setCheckState(Qt::Unchecked);

    if(m_pScheduler->m_pSystemParameter->sys_ctrl.recvinput[3]&0x10)  ui->I1_28->setCheckState(Qt::Checked);//Qt::Unchecked
    else 	ui->I1_28->setCheckState(Qt::Unchecked);

    if(m_pScheduler->m_pSystemParameter->sys_ctrl.recvinput[3]&0x20)  ui->I1_29->setCheckState(Qt::Checked);//Qt::Unchecked
    else 	ui->I1_29->setCheckState(Qt::Unchecked);

    if(m_pScheduler->m_pSystemParameter->sys_ctrl.recvinput[3]&0x40)  ui->I1_30->setCheckState(Qt::Checked);//Qt::Unchecked
    else 	ui->I1_30->setCheckState(Qt::Unchecked);

    if(m_pScheduler->m_pSystemParameter->sys_ctrl.recvinput[3]&0x80)  ui->I1_31->setCheckState(Qt::Checked);//Qt::Unchecked
    else 	ui->I1_31->setCheckState(Qt::Unchecked);

     if(m_pScheduler->m_pSystemParameter->sys_ctrl.recvinput[4]&0x1)  ui->I1_32->setCheckState(Qt::Checked);//Qt::Unchecked
    else 	ui->I1_32->setCheckState(Qt::Unchecked);

    if(m_pScheduler->m_pSystemParameter->sys_ctrl.recvinput[4]&0x2)  ui->I1_33->setCheckState(Qt::Checked);//Qt::Unchecked
    else 	ui->I1_33->setCheckState(Qt::Unchecked);

    if(m_pScheduler->m_pSystemParameter->sys_ctrl.recvinput[4]&0x4)  ui->I1_34->setCheckState(Qt::Checked);//Qt::Unchecked
    else 	ui->I1_34->setCheckState(Qt::Unchecked);

    if(m_pScheduler->m_pSystemParameter->sys_ctrl.recvinput[4]&0x8)  ui->I1_35->setCheckState(Qt::Checked);//Qt::Unchecked
    else 	ui->I1_35->setCheckState(Qt::Unchecked);

    if(m_pScheduler->m_pSystemParameter->sys_ctrl.recvinput[4]&0x10)  ui->I1_36->setCheckState(Qt::Checked);//Qt::Unchecked
    else 	ui->I1_36->setCheckState(Qt::Unchecked);

    if(m_pScheduler->m_pSystemParameter->sys_ctrl.recvinput[4]&0x20)  ui->I1_37->setCheckState(Qt::Checked);//Qt::Unchecked
    else 	ui->I1_37->setCheckState(Qt::Unchecked);

    if(m_pScheduler->m_pSystemParameter->sys_ctrl.recvinput[4]&0x40)  ui->I1_38->setCheckState(Qt::Checked);//Qt::Unchecked
    else 	ui->I1_38->setCheckState(Qt::Unchecked);

    if(m_pScheduler->m_pSystemParameter->sys_ctrl.recvinput[4]&0x80)  ui->I1_39->setCheckState(Qt::Checked);//Qt::Unchecked
    else 	ui->I1_39->setCheckState(Qt::Unchecked);

   if(m_pScheduler->m_pSystemParameter->sys_ctrl.recvinput[5]&0x1)  ui->I1_40->setCheckState(Qt::Checked);//Qt::Unchecked
    else 	ui->I1_40->setCheckState(Qt::Unchecked);

    if(m_pScheduler->m_pSystemParameter->sys_ctrl.recvinput[5]&0x2)  ui->I1_41->setCheckState(Qt::Checked);//Qt::Unchecked
    else 	ui->I1_41->setCheckState(Qt::Unchecked);

    if(m_pScheduler->m_pSystemParameter->sys_ctrl.recvinput[5]&0x4)  ui->I1_42->setCheckState(Qt::Checked);//Qt::Unchecked
    else 	ui->I1_42->setCheckState(Qt::Unchecked);

    if(m_pScheduler->m_pSystemParameter->sys_ctrl.recvinput[5]&0x8)  ui->I1_43->setCheckState(Qt::Checked);//Qt::Unchecked
    else 	ui->I1_43->setCheckState(Qt::Unchecked);

    if(m_pScheduler->m_pSystemParameter->sys_ctrl.recvinput[5]&0x10)  ui->I1_44->setCheckState(Qt::Checked);//Qt::Unchecked
    else 	ui->I1_44->setCheckState(Qt::Unchecked);

    if(m_pScheduler->m_pSystemParameter->sys_ctrl.recvinput[5]&0x20)  ui->I1_45->setCheckState(Qt::Checked);//Qt::Unchecked
    else 	ui->I1_45->setCheckState(Qt::Unchecked);

    if(m_pScheduler->m_pSystemParameter->sys_ctrl.recvinput[5]&0x40)  ui->I1_46->setCheckState(Qt::Checked);//Qt::Unchecked
    else 	ui->I1_46->setCheckState(Qt::Unchecked);

    if(m_pScheduler->m_pSystemParameter->sys_ctrl.recvinput[5]&0x80)  ui->I1_47->setCheckState(Qt::Checked);//Qt::Unchecked
    else 	ui->I1_47->setCheckState(Qt::Unchecked);
    /*if(mutex.tryLock())
    {
        ui->label_xValue_machine->setText(QString::number(m_tempAxis.x));
        ui->label_yValue_machine->setText(QString::number(m_tempAxis.y));
        ui->label_tValue_machine->setText(QString::number(m_tempAxis.t));
        ui->label_cValue_machine->setText(QString::number(m_tempAxis.c));
        ui->label_vValue_machine->setText(QString::number(m_tempAxis.v));

        ui->label_xValue->setText(QString::number(m_tempAxis.x - m_pScheduler->getSystemParameterHandler()->getMachineCoordinate().x));
        ui->label_yValue->setText(QString::number(m_tempAxis.y - m_pScheduler->getSystemParameterHandler()->getMachineCoordinate().y));
        ui->label_tValue->setText(QString::number(m_tempAxis.t));
        ui->label_cValue->setText(QString::number(m_tempAxis.c));
        ui->label_vValue->setText(QString::number(m_tempAxis.v));

        ui->label_toolNum->setText(QString::number(m_tempToolNum));
        ui->label_toolType->setText(m_tempToolType);

        mutex.unlock();
    }*/
}

void CManualMachiningStateUi::on_O1_0_stateChanged(int arg1)
{
	if(arg1){m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[0]|=0x1;}
	else{m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[0]&=~0x1;}
}

void CManualMachiningStateUi::on_O1_1_stateChanged(int arg1)
{
	if(arg1){m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[0]|=0x2;}
	else{m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[0]&=~0x2;}
}

void CManualMachiningStateUi::on_O1_2_stateChanged(int arg1)
{
	if(arg1){m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[0]|=0x4;}
	else{m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[0]&=~0x4;}
}

void CManualMachiningStateUi::on_O1_3_stateChanged(int arg1)
{
	if(arg1){m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[0]|=0x8;}
	else{m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[0]&=~0x8;}
}

void CManualMachiningStateUi::on_O1_4_stateChanged(int arg1)
{
	if(arg1){m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[0]|=0x10;}
	else{m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[0]&=~0x10;}
}

void CManualMachiningStateUi::on_O1_5_stateChanged(int arg1)
{
	if(arg1){m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[0]|=0x20;}
	else{m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[0]&=~0x20;}
}

void CManualMachiningStateUi::on_O1_6_stateChanged(int arg1)
{
	if(arg1){m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[0]|=0x40;}
	else{m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[0]&=~0x40;}
}

void CManualMachiningStateUi::on_O1_7_stateChanged(int arg1)
{
	if(arg1){m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[0]|=0x80;}
	else{m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[0]&=~0x80;}
}

void CManualMachiningStateUi::on_O1_8_stateChanged(int arg1)
{
	if(arg1){m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[1]|=0x1;}
	else{m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[1]&=~0x1;}
}

void CManualMachiningStateUi::on_O1_9_stateChanged(int arg1)
{
	if(arg1){m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[1]|=0x2;}
	else{m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[1]&=~0x2;}
}

void CManualMachiningStateUi::on_O1_10_stateChanged(int arg1)
{
	if(arg1){m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[1]|=0x4;}
	else{m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[1]&=~0x4;}
}

void CManualMachiningStateUi::on_O1_11_stateChanged(int arg1)
{
	if(arg1){m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[1]|=0x8;}
	else{m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[1]&=~0x8;}
}

void CManualMachiningStateUi::on_O1_12_stateChanged(int arg1)
{
	if(arg1){m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[1]|=0x10;}
	else{m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[1]&=~0x10;}
}

void CManualMachiningStateUi::on_O1_13_stateChanged(int arg1)
{
	if(arg1){m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[1]|=0x20;}
	else{m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[1]&=~0x20;}
}

void CManualMachiningStateUi::on_O1_14_stateChanged(int arg1)
{
	if(arg1){m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[1]|=0x40;}
	else{m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[1]&=~0x40;}
}

void CManualMachiningStateUi::on_O1_15_stateChanged(int arg1)
{
	if(arg1){m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[1]|=0x80;}
	else{m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[1]&=~0x80;}
}


void CManualMachiningStateUi::on_O1_16_stateChanged(int arg1)
{
	if(arg1){m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[2]|=0x1;}
	else{m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[2]&=~0x1;}
}

void CManualMachiningStateUi::on_O1_17_stateChanged(int arg1)
{
	if(arg1){m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[2]|=0x2;}
	else{m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[2]&=~0x2;}
}

void CManualMachiningStateUi::on_O1_18_stateChanged(int arg1)
{
	if(arg1){m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[2]|=0x4;}
	else{m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[2]&=~0x4;}
}

void CManualMachiningStateUi::on_O1_19_stateChanged(int arg1)
{
	if(arg1){m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[2]|=0x8;}
	else{m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[2]&=~0x8;}
}

void CManualMachiningStateUi::on_O1_20_stateChanged(int arg1)
{
	if(arg1){m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[2]|=0x10;}
	else{m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[2]&=~0x10;}
}

void CManualMachiningStateUi::on_O1_21_stateChanged(int arg1)
{
	if(arg1){m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[2]|=0x20;}
	else{m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[2]&=~0x20;}
}

void CManualMachiningStateUi::on_O1_22_stateChanged(int arg1)
{
	if(arg1){m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[2]|=0x40;}
	else{m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[2]&=~0x40;}
}

void CManualMachiningStateUi::on_O1_23_stateChanged(int arg1)
{
	if(arg1){m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[2]|=0x80;}
	else{m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[2]&=~0x80;}
}

void CManualMachiningStateUi::on_O1_24_stateChanged(int arg1)
{
	if(arg1){m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[3]|=0x1;}
	else{m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[3]&=~0x1;}
}

void CManualMachiningStateUi::on_O1_25_stateChanged(int arg1)
{
	if(arg1){m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[3]|=0x2;}
	else{m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[3]&=~0x2;}
}

void CManualMachiningStateUi::on_O1_26_stateChanged(int arg1)
{
	if(arg1){m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[3]|=0x4;}
	else{m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[3]&=~0x4;}
}

void CManualMachiningStateUi::on_O1_27_stateChanged(int arg1)
{
	if(arg1){m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[3]|=0x8;}
	else{m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[3]&=~0x8;}
}

void CManualMachiningStateUi::on_O1_28_stateChanged(int arg1)
{
	if(arg1){m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[3]|=0x10;}
	else{m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[3]&=~0x10;}
}

void CManualMachiningStateUi::on_O1_29_stateChanged(int arg1)
{
	if(arg1){m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[3]|=0x20;}
	else{m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[3]&=~0x20;}
}

void CManualMachiningStateUi::on_O1_30_stateChanged(int arg1)
{
	if(arg1){m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[3]|=0x40;}
	else{m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[3]&=~0x40;}
}

void CManualMachiningStateUi::on_O1_31_stateChanged(int arg1)
{
	if(arg1){m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[3]|=0x80;}
	else{m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[3]&=~0x80;}
}

void CManualMachiningStateUi::on_O1_32_stateChanged(int arg1)
{
	if(arg1){m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[4]|=0x1;}
	else{m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[4]&=~0x1;}
}

void CManualMachiningStateUi::on_O1_33_stateChanged(int arg1)
{
	if(arg1){m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[4]|=0x2;}
	else{m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[4]&=~0x2;}
}

void CManualMachiningStateUi::on_O1_34_stateChanged(int arg1)
{
	if(arg1){m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[4]|=0x4;}
	else{m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[4]&=~0x4;}
}

void CManualMachiningStateUi::on_O1_35_stateChanged(int arg1)
{
	if(arg1){m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[4]|=0x8;}
	else{m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[4]&=~0x8;}
}

void CManualMachiningStateUi::on_O1_36_stateChanged(int arg1)
{
	if(arg1){m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[4]|=0x10;}
	else{m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[4]&=~0x10;}
}

void CManualMachiningStateUi::on_O1_37_stateChanged(int arg1)
{
	if(arg1){m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[4]|=0x20;}
	else{m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[4]&=~0x20;}
}

void CManualMachiningStateUi::on_O1_38_stateChanged(int arg1)
{
	if(arg1){m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[4]|=0x40;}
	else{m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[4]&=~0x40;}
}

void CManualMachiningStateUi::on_O1_39_stateChanged(int arg1)
{
	if(arg1){m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[4]|=0x80;}
	else{m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[4]&=~0x80;}
}

void CManualMachiningStateUi::on_O1_40_stateChanged(int arg1)
{
	if(arg1){m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[5]|=0x1;}
	else{m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[5]&=~0x1;}
}

void CManualMachiningStateUi::on_O1_41_stateChanged(int arg1)
{
	if(arg1){m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[5]|=0x2;}
	else{m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[5]&=~0x2;}
}

void CManualMachiningStateUi::on_O1_42_stateChanged(int arg1)
{
	if(arg1){m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[5]|=0x4;}
	else{m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[5]&=~0x4;}
}

void CManualMachiningStateUi::on_O1_43_stateChanged(int arg1)
{
	if(arg1){m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[5]|=0x8;}
	else{m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[5]&=~0x8;}
}

void CManualMachiningStateUi::on_O1_44_stateChanged(int arg1)
{
	if(arg1){m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[5]|=0x10;}
	else{m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[5]&=~0x10;}
}

void CManualMachiningStateUi::on_O1_45_stateChanged(int arg1)
{
	if(arg1){m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[5]|=0x20;}
	else{m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[5]&=~0x20;}
}

void CManualMachiningStateUi::on_O1_46_stateChanged(int arg1)
{
	if(arg1){m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[5]|=0x40;}
	else{m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[5]&=~0x40;}
}

void CManualMachiningStateUi::on_O1_47_stateChanged(int arg1)
{
	if(arg1){m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[5]|=0x80;}
	else{m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[5]&=~0x80;}
}


