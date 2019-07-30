#include "cjointparamsetui.h"
#include "ui_cjointparamsetui.h"

#include <QTimer>
#include <QFileDialog>
#include <QMessageBox>

#include "GlobalVariableType.h"
#include "SystemSchedule.h"
#include "GlobalDefine.h"
#include "MotionControllerModule.h"

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

//    ui->grbAxisNO->setStyleSheet("#Qgroupbox{border:2px solid #014F84}#background-color:#00d8ff");
//    ui->frame->setStyleSheet("#frame{border:2px solid #014F84}#background-color:#00d8ff");
    //QTimer* timer = new QTimer(this);
    //connect(timer, SIGNAL(timeout()),this, SLOT(updataData()));
    //timer->start(100);

//    setTabOrder(ui->acctime,ui->carmaxpvel); // 控件焦点 顺序
//    setTabOrder(ui->carmaxpvel,ui->carmaxgvel);
//    setTabOrder(ui->carmaxgvel,ui->carmaxpacc);
//    setTabOrder(ui->carmaxpacc,ui->carmaxgacc);
//    setTabOrder(ui->carmaxgacc,ui->carstoppdec);
//    setTabOrder(ui->carstoppdec,ui->carstopgdec);
//    setTabOrder(ui->carstopgdec,ui->zonelevel);
}

cjointparamsetui::~cjointparamsetui()
{
    delete ui;
}

// 每次进入界面时 调用
void cjointparamsetui::UpdateViewData(void)
{
    int i;

    CCommmomUIWidget::UpdateViewData();

    SetEditCtrlReadOnly();
}

// 定时更新的数据
void cjointparamsetui::TimerUpdateViewData(void)
{
    CCommmomUIWidget::TimerUpdateViewData();
    updataData();
}

// 根据 权限,设置编辑框的是否能够编辑
void cjointparamsetui::SetEditCtrlReadOnly()
{
    int i;
    QList<QLineEdit *> listEdt;
    UserParameter user;
    int level;

    user = m_pScheduler->m_pSystemParameter->getUserInfo();
    level = user.m_permission;

    switch(level)
    {
    case ORDINARY_USER:
        listEdt = ui->grbCommon->findChildren<QLineEdit *>();
        for(i=0;i<listEdt.size();i++){
            if(listEdt.at(i)->objectName().indexOf("limit")>0){
                listEdt.at(i)->setEnabled(true);
            }else{
                listEdt.at(i)->setEnabled(false);
            }
        }
        listEdt = ui->gridGroupBox_2->findChildren<QLineEdit *>();
        for(i=0;i<listEdt.size();i++)
            listEdt.at(i)->setEnabled(false);
        break;
    case ADVANCED_USER:
    case SUPER_USER:
        listEdt = ui->grbCommon->findChildren<QLineEdit *>();
        for(i=0;i<listEdt.size();i++){
            listEdt.at(i)->setEnabled(true);
        }
        listEdt = ui->gridGroupBox_2->findChildren<QLineEdit *>();
        for(i=0;i<listEdt.size();i++)
            listEdt.at(i)->setEnabled(true);
        break;
    default: break;
    }
}

void cjointparamsetui::updataData()
{
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

    ui->axis7maxvel->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pJointMaxAcc7]) );
    ui->axis7maxacc->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pJointMaxAcc7 + 1]) );
    ui->axis7maxdec->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pJointMaxAcc7 + 2]) );
    ui->axis7poslimit->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pJointMaxAcc7 + 3]) );
    ui->axis7neglimit->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pJointMaxAcc7 + 4]) );
    ui->axis7ratio->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pJointMaxAcc7 + 5]) );
    ui->axis7precision->setText(QString("%1").arg((int)m_pScheduler->m_pSystemParameter->SystemParam[pJointMaxAcc7 + 7]) );
    if(*((int *)&m_pScheduler->m_pSystemParameter->SystemParam[pMotorDir])&0x40==0x40)
    {
        ui->axis7dir->setText(QString("1") );
    }
    else
    {
        ui->axis7dir->setText(QString("0") );
    }

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

    if(*((char *)&m_pScheduler->m_pSystemParameter->SystemParam[pMotorDir])&0x10==0x10)
        //        if((data&0x10)==0x10)
    {
        ui->axis5dir->setText(QString("1") );
    }
    else
    {
        ui->axis5dir->setText(QString("0") );
    }
    
    if(*((char *)&m_pScheduler->m_pSystemParameter->SystemParam[pMotorDir])&0x20==0x20)
        //    if((data&0x20)==0x20)
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
    ui->zonelevel->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pZoneLevel]) );

    ui->LineMaxLen->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pLineMaxLen]) );
    ui->LineMaxVel->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pLineMaxVel]) );
    ui->LineMinPer->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pLineMinPer]) );
    ui->LineMaxPer->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pLineMaxPer]) );
    ui->ArcRadMaxLen->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pArcRadMaxLen]) );
    ui->ArcLenMaxLen->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pArcLenMaxLen]) );
    ui->ArcMaxVel->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pArcMaxVel]) );
    ui->ArcMaxPer->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pArcMaxPer]) );
    ui->ArcMinPer->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pArcMinPer]) );
    ui->ArcError->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pArcError]) );
}

void cjointparamsetui::on_axisParamSVBtn_clicked()
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

    m_pScheduler->m_pSystemParameter->SystemParam[pJointMaxAcc7]=ui->axis1maxvel->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pJointMaxAcc7 + 1]=ui->axis1maxacc->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pJointMaxAcc7 + 2]=ui->axis1maxdec->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pJointMaxAcc7 + 3]=ui->axis1poslimit->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pJointMaxAcc7 + 4]=ui->axis1neglimit->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pJointMaxAcc7 + 5]=ui->axis1ratio->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pJointMaxAcc7 + 7]=ui->axis1precision->text().toFloat();
    if(ui->axis7dir->text().toInt()>0)
    {
            *((int *)&m_pScheduler->m_pSystemParameter->SystemParam[pMotorDir])|=0x40;
    }
    else
    {
            *((int *)&m_pScheduler->m_pSystemParameter->SystemParam[pMotorDir])&=~0x40;
    }



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
    m_pScheduler->m_pSystemParameter->SystemParam[pZoneLevel]=ui->zonelevel->text().toFloat();

    m_pScheduler->m_pSystemParameter->SystemParam[pLineMaxLen]=ui->LineMaxLen->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pLineMaxVel]=ui->LineMaxVel->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pLineMinPer]=ui->LineMinPer->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pLineMaxPer]=ui->LineMaxPer->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pArcRadMaxLen]=ui->ArcRadMaxLen->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pArcLenMaxLen]=ui->ArcLenMaxLen->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pArcMaxVel]=ui->ArcMaxVel->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pArcMaxPer]=ui->ArcMaxPer->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pArcMinPer]=ui->ArcMinPer->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pArcError]=ui->ArcError->text().toFloat();

    m_pScheduler->WriteSystemParamInformation();

    // 判断网络是否连接
    if(!m_pScheduler->NetIsConnect()){
        QMessageBox::information(this,QString::fromLocal8Bit("错误"),QString::fromLocal8Bit("网络已断开连接!"));
        return;
    }

    m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/setParamer", "all");

}

void cjointparamsetui::on_axisParamRDBtn_clicked()
{
#ifdef USE_ARMPARAM
    // 判断网络是否连接
    if(!m_pScheduler->NetIsConnect()){
        QMessageBox::information(this,QString::fromLocal8Bit("错误"),QString::fromLocal8Bit("网络已断开连接!"));
        return;
    }
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
    ui->zonelevel->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pZoneLevel]) );

    ui->A1->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pLinkDimension]) );
    ui->A2->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pLinkDimension+1]) );
    ui->A3->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pLinkDimension+2]) );
    ui->D3->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pLinkDimension+3]) );
    ui->D4->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pLinkDimension+4]) );
    ui->Df->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pLinkDimension+5]) );

#endif

}
