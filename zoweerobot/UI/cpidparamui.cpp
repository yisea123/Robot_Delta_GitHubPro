#include "cpidparamui.h"
#include "ui_cpidparamui.h"

#include <QFileDialog>
#include <QMessageBox>

#include "GlobalVariableType.h"
#include "SystemSchedule.h"
#include "GlobalDefine.h"
cpidparamui::cpidparamui(QWidget *parent) :
    CCommmomUIWidget(parent),
    ui(new Ui::cpidparamui)
{
    ui->setupUi(this);
}

cpidparamui::cpidparamui(QWidget *parent, SystemSchedule* schedule) :
    CCommmomUIWidget(parent),
    m_pScheduler(schedule),
    ui(new Ui::cpidparamui)
{
    ui->setupUi(this);
    //on_cancle_clicked();
    m_pScheduler->m_pSystemParameter->getsysparamfinished=1;
    m_pScheduler->m_pSystemParameter->getpidparamfinished[0]=1;
    m_pScheduler->m_pSystemParameter->getpidparamfinished[1]=1;
    m_pScheduler->m_pSystemParameter->getpidparamfinished[2]=1;
    m_pScheduler->m_pSystemParameter->getpidparamfinished[3]=1;
    m_pScheduler->m_pSystemParameter->getpidparamfinished[4]=1;
    m_pScheduler->m_pSystemParameter->getpidparamfinished[5]=1;
    m_pScheduler->m_pSystemParameter->getpidparamfinished[6]=1;
    //QTimer* timer = new QTimer(this);
    //connect(timer, SIGNAL(timeout()),this, SLOT(updataData()));
    //timer->start(100);

    //

    m_pModel = new QStandardItemModel();
    m_pModel->setColumnCount(2);
    //m_pModel->setHeaderData()
    ui->tbvParaGrp->setModel(m_pModel);

    ui->tbvParaGrp->setColumnWidth(0,94);
    ui->tbvParaGrp->setColumnWidth(1,280);

    ui->tbvParaGrp->horizontalHeader()->setVisible(false); //隐藏行表头
    ui->tbvParaGrp->setEditTriggers(QAbstractItemView::NoEditTriggers);
    UpdateTbvParaGrp();
}

cpidparamui::~cpidparamui()
{
    delete ui;
}

// 每次进入界面时 调用
void cpidparamui::UpdateViewData(void)
{
    int i;

    CCommmomUIWidget::UpdateViewData();

    refresh_LineParam();
    for(i=0;i<MOF;i++){
        refresh_pidParam(i);
    }

    SetCtrlEnabled();
}

// 定时更新的数据
void cpidparamui::TimerUpdateViewData(void)
{
    CCommmomUIWidget::TimerUpdateViewData();
    updataData();
}

void cpidparamui::UpdateTbvParaGrp()
{
    m_pModel->setItem(0,0,new QStandardItem(QString::fromLocal8Bit("组1")));
    m_pModel->setItem(0,1,new QStandardItem(m_pScheduler->m_pSystemParameter->PidParaGrpName[0]));
    m_pModel->setItem(1,0,new QStandardItem(QString::fromLocal8Bit("组2")));
    m_pModel->setItem(1,1,new QStandardItem(m_pScheduler->m_pSystemParameter->PidParaGrpName[1]));
    m_pModel->setItem(2,0,new QStandardItem(QString::fromLocal8Bit("组3")));
    m_pModel->setItem(2,1,new QStandardItem(m_pScheduler->m_pSystemParameter->PidParaGrpName[2]));
    m_pModel->setItem(3,0,new QStandardItem(QString::fromLocal8Bit("组4")));
    m_pModel->setItem(3,1,new QStandardItem(m_pScheduler->m_pSystemParameter->PidParaGrpName[3]));
    m_pModel->setItem(4,0,new QStandardItem(QString::fromLocal8Bit("组5")));
    m_pModel->setItem(4,1,new QStandardItem(m_pScheduler->m_pSystemParameter->PidParaGrpName[4]));
}


// 根据 权限,设置编辑框的是否能够编辑
void cpidparamui::SetCtrlEnabled()
{
    int i;
    QList<QLineEdit *> listEdt;
    QList<QPushButton *> listBtn;
    UserParameter user;
    int level;

    user = m_pScheduler->m_pSystemParameter->getUserInfo();
    level = user.m_permission;

    switch(level)
    {
        case ORDINARY_USER:
            listEdt = ui->grbPidPara->findChildren<QLineEdit *>();
            for(i=0;i<listEdt.size();i++){
                listEdt.at(i)->setEnabled(false);
            }
            listBtn = ui->grbPidPara->findChildren<QPushButton *>();
            for(i=0;i<listBtn.size();i++){
                if(listBtn.at(i)->objectName().indexOf("setpid")>0){
                    listBtn.at(i)->setEnabled(false);
                }
            }

            ui->cmbParaGrp->setEnabled(false);
            ui->btnAsSave1->setEnabled(false);
            break;
        case ADVANCED_USER:
            listEdt = ui->grbPidPara->findChildren<QLineEdit *>();
            for(i=0;i<listEdt.size();i++){
                listEdt.at(i)->setEnabled(true);
            }
            listBtn = ui->grbPidPara->findChildren<QPushButton *>();
            for(i=0;i<listBtn.size();i++){
                if(listBtn.at(i)->objectName().indexOf("setpid")>0){
                    listBtn.at(i)->setEnabled(true);
                }
            }

            ui->cmbParaGrp->setEnabled(true);
            ui->btnAsSave1->setEnabled(true);
            break;
        case SUPER_USER:
            listEdt = ui->grbPidPara->findChildren<QLineEdit *>();
            for(i=0;i<listEdt.size();i++){
                listEdt.at(i)->setEnabled(true);
            }
            listBtn = ui->grbPidPara->findChildren<QPushButton *>();
            for(i=0;i<listBtn.size();i++){
                if(listBtn.at(i)->objectName().indexOf("setpid")>0){
                    listBtn.at(i)->setEnabled(true);
                }
            }

            ui->cmbParaGrp->setEnabled(true);
            ui->btnAsSave1->setEnabled(true);
            break;
        default: break;
    }
}

void cpidparamui::updataData()
{
       int i=0;
       //bool b=false;
	
       if(m_pScheduler->m_pSystemParameter->getsysparamfinished)
       	{
       		refresh_LineParam();
			//m_pScheduler->m_pSystemParameter->getsysparamfinished=0;//cjointparamsetui::updataData()中置0
       	}
	for(i=0;i<MOF;i++)
	{
		if(m_pScheduler->m_pSystemParameter->getpidparamfinished[i])
		{
			refresh_pidParam(i);
			m_pScheduler->m_pSystemParameter->getpidparamfinished[i]=0;
		}
	}
}

void cpidparamui::refresh_LineParam()
{
       
       	{
	ui->LineMaxLen->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pLineMaxLen]) );
	ui->LineMaxVel->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pLineMaxVel]) );
	ui->LineMinPer->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pLineMinPer]) );
	ui->LineMaxPer->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pLineMaxPer]) );
	
       	}
}	   
void cpidparamui::refresh_pidParam(int axis)
{
    if(axis==0)
    {
        ui->axis1poskp->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pAIXS1PID]) );
        ui->axis1velkp->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pAIXS1PID+1]) );
        ui->axis1velki->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pAIXS1PID+2]) );
        ui->axis1curkp->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pAIXS1PID+3]) );
        ui->axis1curki->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pAIXS1PID+4]) );
    }
    else if(axis==1)
    {
        ui->axis2poskp->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pAIXS2PID]) );
        ui->axis2velkp->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pAIXS2PID+1]) );
        ui->axis2velki->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pAIXS2PID+2]) );
        ui->axis2curkp->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pAIXS2PID+3]) );
        ui->axis2curki->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pAIXS2PID+4]) );
    }
    else if(axis==2)
    {
        ui->axis3poskp->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pAIXS3PID]) );
        ui->axis3velkp->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pAIXS3PID+1]) );
        ui->axis3velki->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pAIXS3PID+2]) );
        ui->axis3curkp->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pAIXS3PID+3]) );
        ui->axis3curki->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pAIXS3PID+4]) );
    }
    else if(axis==3)
    {
        ui->axis4poskp->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pAIXS4PID]) );
        ui->axis4velkp->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pAIXS4PID+1]) );
        ui->axis4velki->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pAIXS4PID+2]) );
        ui->axis4curkp->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pAIXS4PID+3]) );
        ui->axis4curki->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pAIXS4PID+4]) );
    }
    else if(axis == 4)
    {
        ui->axis5poskp->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pAIXS5PID]) );
        ui->axis5velkp->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pAIXS5PID+1]) );
        ui->axis5velki->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pAIXS5PID+2]) );
        ui->axis5curkp->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pAIXS5PID+3]) );
        ui->axis5curki->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pAIXS5PID+4]) );
    }
    else if(axis == 5)
    {
        ui->axis6poskp->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pAIXS6PID]) );
        ui->axis6velkp->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pAIXS6PID+1]) );
        ui->axis6velki->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pAIXS6PID+2]) );
        ui->axis6curkp->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pAIXS6PID+3]) );
        ui->axis6curki->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pAIXS6PID+4]) );
    }
    else if(axis == 6)
    {
        ui->axis7poskp->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pAIXS7PID]) );
        ui->axis7velkp->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pAIXS7PID+1]) );
        ui->axis7velki->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pAIXS7PID+2]) );
        ui->axis7curkp->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pAIXS7PID+3]) );
        ui->axis7curki->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pAIXS7PID+4]) );
    }
}

void cpidparamui::on_axis1readpid_clicked()
{    
    if(!m_pScheduler->NetIsConnect()){ // 判断网络是否连接
        QMessageBox::information(this,QString::fromLocal8Bit("错误"),QString::fromLocal8Bit("网络已断开!"));
        return;
    }
    m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "getPIDParamer", "15");
}

void cpidparamui::on_axis2readpid_clicked()
{
    if(!m_pScheduler->NetIsConnect()){ // 判断网络是否连接
        QMessageBox::information(this,QString::fromLocal8Bit("错误"),QString::fromLocal8Bit("网络已断开!"));
        return;
    }
    m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "getPIDParamer", "25");
}

void cpidparamui::on_axis3readpid_clicked()
{
    if(!m_pScheduler->NetIsConnect()){ // 判断网络是否连接
        QMessageBox::information(this,QString::fromLocal8Bit("错误"),QString::fromLocal8Bit("网络已断开!"));
        return;
    }
	m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "getPIDParamer", "35");
}

void cpidparamui::on_axis4readpid_clicked()
{
    if(!m_pScheduler->NetIsConnect()){ // 判断网络是否连接
        QMessageBox::information(this,QString::fromLocal8Bit("错误"),QString::fromLocal8Bit("网络已断开!"));
        return;
    }
	m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "getPIDParamer", "45");
}

void cpidparamui::on_axis5readpid_clicked()
{
    if(!m_pScheduler->NetIsConnect()){ // 判断网络是否连接
        QMessageBox::information(this,QString::fromLocal8Bit("错误"),QString::fromLocal8Bit("网络已断开!"));
        return;
    }
    m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "getPIDParamer", "55");
}

void cpidparamui::on_axis6readpid_clicked()
{
    if(!m_pScheduler->NetIsConnect()){ // 判断网络是否连接
        QMessageBox::information(this,QString::fromLocal8Bit("错误"),QString::fromLocal8Bit("网络已断开!"));
        return;
    }
    m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "getPIDParamer", "65");
}

void cpidparamui::on_axis7readpid_clicked()
{
    if(!m_pScheduler->NetIsConnect()){ // 判断网络是否连接
        QMessageBox::information(this,QString::fromLocal8Bit("错误"),QString::fromLocal8Bit("网络已断开!"));
        return;
    }
    m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "getPIDParamer", "75");
}

void cpidparamui::on_readlineparam_clicked()
{
//#ifdef USE_ARMPARAM
    if(!m_pScheduler->NetIsConnect()){ // 判断网络是否连接
        QMessageBox::information(this,QString::fromLocal8Bit("错误"),QString::fromLocal8Bit("网络已断开!"));
        return;
    }
	m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/getsysparam", "yes");
//#endif
}

void cpidparamui::on_setlineparam_clicked()
{
    m_pScheduler->m_pSystemParameter->SystemParam[pLineMaxLen]=ui->LineMaxLen->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pLineMaxVel]=ui->LineMaxVel->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pLineMinPer]=ui->LineMinPer->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pLineMaxPer]=ui->LineMaxPer->text().toFloat();

    m_pScheduler->WriteSystemParamInformation();
    if(!m_pScheduler->NetIsConnect()){ // 判断网络是否连接
        QMessageBox::information(this,QString::fromLocal8Bit("错误"),QString::fromLocal8Bit("网络已断开!"));
        return;
    }
    m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/setParamer", "all");
}

void cpidparamui::on_axis1setpid_clicked()
{
    m_pScheduler->m_pSystemParameter->SystemParam[pAIXS1PID]=ui->axis1poskp->text().toInt();
    m_pScheduler->m_pSystemParameter->SystemParam[pAIXS1PID+1]=ui->axis1velkp->text().toInt();
    m_pScheduler->m_pSystemParameter->SystemParam[pAIXS1PID+2]=ui->axis1velki->text().toInt();
    m_pScheduler->m_pSystemParameter->SystemParam[pAIXS1PID+3]=ui->axis1curkp->text().toInt();
    m_pScheduler->m_pSystemParameter->SystemParam[pAIXS1PID+4]=ui->axis1curki->text().toInt();

    m_pScheduler->WriteSystemParamInformation();

    if(!m_pScheduler->NetIsConnect()){ // 判断网络是否连接
        QMessageBox::information(this,QString::fromLocal8Bit("错误"),QString::fromLocal8Bit("网络已断开!"));
        return;
    }
    m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "setPIDParamer", "15");
}

void cpidparamui::on_axis2setpid_clicked()
{
    m_pScheduler->m_pSystemParameter->SystemParam[pAIXS2PID]=ui->axis2poskp->text().toInt();
    m_pScheduler->m_pSystemParameter->SystemParam[pAIXS2PID+1]=ui->axis2velkp->text().toInt();
    m_pScheduler->m_pSystemParameter->SystemParam[pAIXS2PID+2]=ui->axis2velki->text().toInt();
    m_pScheduler->m_pSystemParameter->SystemParam[pAIXS2PID+3]=ui->axis2curkp->text().toInt();
    m_pScheduler->m_pSystemParameter->SystemParam[pAIXS2PID+4]=ui->axis2curki->text().toInt();

    m_pScheduler->WriteSystemParamInformation();

    if(!m_pScheduler->NetIsConnect()){ // 判断网络是否连接
        QMessageBox::information(this,QString::fromLocal8Bit("错误"),QString::fromLocal8Bit("网络已断开!"));
        return;
    }
    m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "setPIDParamer", "25");
}

void cpidparamui::on_axis3setpid_clicked()
{
    m_pScheduler->m_pSystemParameter->SystemParam[pAIXS3PID]=ui->axis3poskp->text().toInt();
    m_pScheduler->m_pSystemParameter->SystemParam[pAIXS3PID+1]=ui->axis3velkp->text().toInt();
    m_pScheduler->m_pSystemParameter->SystemParam[pAIXS3PID+2]=ui->axis3velki->text().toInt();
    m_pScheduler->m_pSystemParameter->SystemParam[pAIXS3PID+3]=ui->axis3curkp->text().toInt();
    m_pScheduler->m_pSystemParameter->SystemParam[pAIXS3PID+4]=ui->axis3curki->text().toInt();

    m_pScheduler->WriteSystemParamInformation();

    if(!m_pScheduler->NetIsConnect()){ // 判断网络是否连接
        QMessageBox::information(this,QString::fromLocal8Bit("错误"),QString::fromLocal8Bit("网络已断开!"));
        return;
    }
    m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "setPIDParamer", "35");
}

void cpidparamui::on_axis4setpid_clicked()
{
    m_pScheduler->m_pSystemParameter->SystemParam[pAIXS4PID]=ui->axis4poskp->text().toInt();
    m_pScheduler->m_pSystemParameter->SystemParam[pAIXS4PID+1]=ui->axis4velkp->text().toInt();
    m_pScheduler->m_pSystemParameter->SystemParam[pAIXS4PID+2]=ui->axis4velki->text().toInt();
    m_pScheduler->m_pSystemParameter->SystemParam[pAIXS4PID+3]=ui->axis4curkp->text().toInt();
    m_pScheduler->m_pSystemParameter->SystemParam[pAIXS4PID+4]=ui->axis4curki->text().toInt();

    m_pScheduler->WriteSystemParamInformation();

    if(!m_pScheduler->NetIsConnect()){ // 判断网络是否连接
        QMessageBox::information(this,QString::fromLocal8Bit("错误"),QString::fromLocal8Bit("网络已断开!"));
        return;
    }
    m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "setPIDParamer", "45");
}

void cpidparamui::on_axis5setpid_clicked()
{
    m_pScheduler->m_pSystemParameter->SystemParam[pAIXS5PID]=ui->axis5poskp->text().toInt();
    m_pScheduler->m_pSystemParameter->SystemParam[pAIXS5PID+1]=ui->axis5velkp->text().toInt();
    m_pScheduler->m_pSystemParameter->SystemParam[pAIXS5PID+2]=ui->axis5velki->text().toInt();
    m_pScheduler->m_pSystemParameter->SystemParam[pAIXS5PID+3]=ui->axis5curkp->text().toInt();
    m_pScheduler->m_pSystemParameter->SystemParam[pAIXS5PID+4]=ui->axis5curki->text().toInt();

    m_pScheduler->WriteSystemParamInformation();

    if(!m_pScheduler->NetIsConnect()){ // 判断网络是否连接
        QMessageBox::information(this,QString::fromLocal8Bit("错误"),QString::fromLocal8Bit("网络已断开!"));
        return;
    }
    m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "setPIDParamer", "55");
}

void cpidparamui::on_axis6setpid_clicked()
{
    m_pScheduler->m_pSystemParameter->SystemParam[pAIXS6PID]=ui->axis6poskp->text().toInt();
    m_pScheduler->m_pSystemParameter->SystemParam[pAIXS6PID+1]=ui->axis6velkp->text().toInt();
    m_pScheduler->m_pSystemParameter->SystemParam[pAIXS6PID+2]=ui->axis6velki->text().toInt();
    m_pScheduler->m_pSystemParameter->SystemParam[pAIXS6PID+3]=ui->axis6curkp->text().toInt();
    m_pScheduler->m_pSystemParameter->SystemParam[pAIXS6PID+4]=ui->axis6curki->text().toInt();

    m_pScheduler->WriteSystemParamInformation();

    if(!m_pScheduler->NetIsConnect()){ // 判断网络是否连接
        QMessageBox::information(this,QString::fromLocal8Bit("错误"),QString::fromLocal8Bit("网络已断开!"));
        return;
    }
    m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "setPIDParamer", "65");
}

void cpidparamui::on_axis7setpid_clicked()
{
    m_pScheduler->m_pSystemParameter->SystemParam[pAIXS7PID]=ui->axis7poskp->text().toInt();
    m_pScheduler->m_pSystemParameter->SystemParam[pAIXS7PID+1]=ui->axis7velkp->text().toInt();
    m_pScheduler->m_pSystemParameter->SystemParam[pAIXS7PID+2]=ui->axis7velki->text().toInt();
    m_pScheduler->m_pSystemParameter->SystemParam[pAIXS7PID+3]=ui->axis7curkp->text().toInt();
    m_pScheduler->m_pSystemParameter->SystemParam[pAIXS7PID+4]=ui->axis7curki->text().toInt();

    m_pScheduler->WriteSystemParamInformation();

    if(!m_pScheduler->NetIsConnect()){ // 判断网络是否连接
        QMessageBox::information(this,QString::fromLocal8Bit("错误"),QString::fromLocal8Bit("网络已断开!"));
        return;
    }
    m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "setPIDParamer", "75");
}

void cpidparamui::on_setcandebug_clicked()
{
    if(!m_pScheduler->NetIsConnect()){ // 判断网络是否连接
        QMessageBox::information(this,QString::fromLocal8Bit("错误"),QString::fromLocal8Bit("网络已断开!"));
        return;
    }
     m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "setCanDebug", ui->candebug->text());
}


bool cpidparamui::CheckEditParaVal(QLineEdit *edt,int &newVal)
{
    QString str;
    bool bOK=false;
    int val;

    str = edt->text().trimmed();
    val = str.toInt(&bOK);
    if(!bOK){
        edt->selectAll();
        edt->setFocus();
        return false;
    }
    if(val<0){
        edt->selectAll();
        edt->setFocus();
        return false;
    }
    newVal = val;

    return true;
}

bool cpidparamui::CheckAllEditParaVal(int val[])
{
    if(!CheckEditParaVal(ui->axis1poskp,val[0])){
        return false;
    }
    if(!CheckEditParaVal(ui->axis1velkp,val[1])){
        return false;
    }
    if(!CheckEditParaVal(ui->axis1velki,val[2])){
        return false;
    }
    if(!CheckEditParaVal(ui->axis1curkp,val[3])){
        return false;
    }
    if(!CheckEditParaVal(ui->axis1curki,val[4])){
        return false;
    }
    // 2
    if(!CheckEditParaVal(ui->axis2poskp,val[6])){
        return false;
    }
    if(!CheckEditParaVal(ui->axis2velkp,val[7])){
        return false;
    }
    if(!CheckEditParaVal(ui->axis2velki,val[8])){
        return false;
    }
    if(!CheckEditParaVal(ui->axis2curkp,val[9])){
        return false;
    }
    if(!CheckEditParaVal(ui->axis2curki,val[10])){
        return false;
    }
    // 3
    if(!CheckEditParaVal(ui->axis3poskp,val[12])){
        return false;
    }
    if(!CheckEditParaVal(ui->axis3velkp,val[13])){
        return false;
    }
    if(!CheckEditParaVal(ui->axis3velki,val[14])){
        return false;
    }
    if(!CheckEditParaVal(ui->axis3curkp,val[15])){
        return false;
    }
    if(!CheckEditParaVal(ui->axis3curki,val[16])){
        return false;
    }
    // 4
    if(!CheckEditParaVal(ui->axis4poskp,val[18])){
        return false;
    }
    if(!CheckEditParaVal(ui->axis4velkp,val[19])){
        return false;
    }
    if(!CheckEditParaVal(ui->axis4velki,val[20])){
        return false;
    }
    if(!CheckEditParaVal(ui->axis4curkp,val[21])){
        return false;
    }
    if(!CheckEditParaVal(ui->axis4curki,val[22])){        
        return false;
    }
    // 5
    if(!CheckEditParaVal(ui->axis5poskp,val[24])){
        return false;
    }
    if(!CheckEditParaVal(ui->axis5velkp,val[25])){
        return false;
    }
    if(!CheckEditParaVal(ui->axis5velki,val[26])){
        return false;
    }
    if(!CheckEditParaVal(ui->axis5curkp,val[27])){
        return false;
    }
    if(!CheckEditParaVal(ui->axis5curki,val[28])){
        return false;
    }
    // 6
    if(!CheckEditParaVal(ui->axis6poskp,val[30])){
        return false;
    }
    if(!CheckEditParaVal(ui->axis6velkp,val[31])){
        return false;
    }
    if(!CheckEditParaVal(ui->axis6velki,val[32])){
        return false;
    }
    if(!CheckEditParaVal(ui->axis6curkp,val[33])){
        return false;
    }
    if(!CheckEditParaVal(ui->axis6curki,val[34])){
        return false;
    }
    // 7
    if(!CheckEditParaVal(ui->axis7poskp,val[36])){
        return false;
    }
    if(!CheckEditParaVal(ui->axis7velkp,val[37])){
        return false;
    }
    if(!CheckEditParaVal(ui->axis7velki,val[38])){
        return false;
    }
    if(!CheckEditParaVal(ui->axis7curkp,val[39])){
        return false;
    }
    if(!CheckEditParaVal(ui->axis7curki,val[40])){
        return false;
    }
    //   
    return true;
}

void cpidparamui::on_btnAsSaveGrp_clicked(int grp)
{
    int i, val[42];
    QLineEdit *edt=NULL;
    QString str;

    if(!CheckAllEditParaVal(val)){
        QMessageBox::information(this,QString::fromLocal8Bit("信息提示"),QString::fromLocal8Bit("非法输入值!"));
        return ;
    }

    memcpy(m_pScheduler->m_pSystemParameter->PidParaGrpName[grp],ui->edtGrpMemo->text().toStdString().c_str(),
               sizeof(m_pScheduler->m_pSystemParameter->PidParaGrpName[0]));


    for(i=0;i<24;i++){
        m_pScheduler->m_pSystemParameter->PidParaGrp[grp][i] = val[i];
    }

    m_pScheduler->SavePidParaGrp("PidParaGrp.dat");
    QMessageBox::information(this,QString::fromLocal8Bit("信息提示"),QString::fromLocal8Bit("保存成功!"));
}

// 读 参数组 缓冲 到当前PID参数上, 然后 保存成最新的参数到本地和下位机
void cpidparamui::ReadGrpPara(int grp)
{
    ;
}

void cpidparamui::on_btnReadGrp_clicked(int grp)
{
    QString str;
    int i;

    ui->edtGrpMemo->setText(m_pScheduler->m_pSystemParameter->PidParaGrpName[grp]);

    for(i=0;i<24;i++){
        m_pScheduler->m_pSystemParameter->SystemParam[pAIXS1PID+i] = m_pScheduler->m_pSystemParameter->PidParaGrp[grp][i];
    }
    for(i=0;i<18;i++){
        m_pScheduler->m_pSystemParameter->SystemParam[pAIXS5PID+i] = m_pScheduler->m_pSystemParameter->PidParaGrp[grp][i + 24];
    }
    for(i=0;i<MOF;i++){
        refresh_pidParam(i);
    }

    m_pScheduler->WriteSystemParamInformation();
    /*
    if(!m_pScheduler->NetIsConnect()){ // 判断网络是否连接
        QMessageBox::information(this,QString::fromLocal8Bit("错误"),QString::fromLocal8Bit("网络已断开!"));
        return;
    }*/
    m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "setPIDParamer", "15");
    QThread::msleep(50);
    m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "setPIDParamer", "25");
    QThread::msleep(50);
    m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "setPIDParamer", "35");
    QThread::msleep(50);
    m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "setPIDParamer", "45");
    QThread::msleep(50);
    m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "setPIDParamer", "55");
    QThread::msleep(50);
    m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "setPIDParamer", "65");
    QThread::msleep(50);
    m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "setPIDParamer", "75");

    //QMessageBox::information(this,QString::fromLocal8Bit("信息提示"),QString::fromLocal8Bit("操作完成!"));
}


void cpidparamui::on_btnAsSave1_clicked()
{
    int index;

    index = ui->cmbParaGrp->currentIndex()-1;
    if(index<0){
        QMessageBox::information(this,QString::fromLocal8Bit("信息提示"),QString::fromLocal8Bit("请先选择要保存的参数组!"));
        return;
    }
    on_btnAsSaveGrp_clicked(index);
    UpdateTbvParaGrp();
}

void cpidparamui::on_cmbParaGrp_currentIndexChanged(int index)
{
    ui->edtGrpMemo->clear();
    if(index>0){
        on_btnReadGrp_clicked(index-1);
    }
}

