#include "cstructparam.h"
#include "ui_cstructparam.h"

#include <QTimer>
#include <QFileDialog>
#include <QMessageBox>

#include "GlobalVariableType.h"
#include "SystemSchedule.h"
#include "GlobalDefine.h"
#include "MotionControllerModule.h"

#define USE_ARMPARAM
cstructparam::cstructparam(QWidget *parent) :
    CCommmomUIWidget(parent),
    ui(new Ui::cstructparam)
{
    ui->setupUi(this);
}

cstructparam::cstructparam(QWidget *parent, SystemSchedule* schedule) :
    CCommmomUIWidget(parent),
    m_pScheduler(schedule),
    ui(new Ui::cstructparam)
{
    ui->setupUi(this);
    m_pScheduler->m_pSystemParameter->getsysparamfinished=1;
    //QTimer* timer = new QTimer(this);
    //connect(timer, SIGNAL(timeout()),this, SLOT(updataData()));
    //timer->start(100);
}

cstructparam::~cstructparam()
{
    delete ui;
}

// ÿ�ν������ʱ ����
void cstructparam::UpdateViewData(void)
{
    int i;

    CCommmomUIWidget::UpdateViewData();

    SetEditCtrlReadOnly();
}

// ��ʱ���µ�����
void cstructparam::TimerUpdateViewData(void)
{
    CCommmomUIWidget::TimerUpdateViewData();
    updataData();
}

// ���� Ȩ��,���ñ༭����Ƿ��ܹ��༭
void cstructparam::SetEditCtrlReadOnly()
{
//    int i;
//    QList<QLineEdit *> listEdt;
//    UserParameter user;
//    int level;

//    user = m_pScheduler->m_pSystemParameter->getUserInfo();
//    level = user.m_permission;

//    switch(level)
//    {
//        case ORDINARY_USER:
//            listEdt = ui->grbCommon->findChildren<QLineEdit *>();
//            for(i=0;i<listEdt.size();i++){
//                if(listEdt.at(i)->objectName().indexOf("limit")>0){
//                    listEdt.at(i)->setEnabled(true);
//                }else{
//                    listEdt.at(i)->setEnabled(false);
//                }
//            }
//            listEdt = ui->grbLink->findChildren<QLineEdit *>();
//            for(i=0;i<listEdt.size();i++){
//                listEdt.at(i)->setEnabled(false);
//            }

//            listEdt = ui->grbAxisNO->findChildren<QLineEdit *>();
//            for(i=0;i<listEdt.size();i++){
//                listEdt.at(i)->setEnabled(false);
//            }
//        break;
//        case ADVANCED_USER:
//            listEdt = ui->grbCommon->findChildren<QLineEdit *>();
//            for(i=0;i<listEdt.size();i++){
//                listEdt.at(i)->setEnabled(true);
//            }
//            listEdt = ui->grbLink->findChildren<QLineEdit *>();
//            for(i=0;i<listEdt.size();i++){
//                listEdt.at(i)->setEnabled(true);
//            }

//            listEdt = ui->grbAxisNO->findChildren<QLineEdit *>();
//            for(i=0;i<listEdt.size();i++){
//                listEdt.at(i)->setEnabled(true);
//            }
//        break;
//        case SUPER_USER:
//            listEdt = ui->grbCommon->findChildren<QLineEdit *>();
//            for(i=0;i<listEdt.size();i++){
//                listEdt.at(i)->setEnabled(true);
//            }
//            listEdt = ui->grbLink->findChildren<QLineEdit *>();
//            for(i=0;i<listEdt.size();i++){
//                listEdt.at(i)->setEnabled(true);
//            }

//            listEdt = ui->grbAxisNO->findChildren<QLineEdit *>();
//            for(i=0;i<listEdt.size();i++){
//                listEdt.at(i)->setEnabled(true);
//            }
//        break;
//        default: break;
//    }
}

void cstructparam::updataData()
{
#ifdef USE_ARMPARAM
    if(m_pScheduler->m_pSystemParameter->getsysparamfinished)
    {
        refresh_structParam();
        m_pScheduler->m_pSystemParameter->getsysparamfinished=0;
    }
#endif
}

void cstructparam::refresh_structParam()
{
    ui->bx->setText(    QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pLinkDimension]));
    ui->by->setText(    QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pLinkDimension + 1]));
    ui->bz->setText(    QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pLinkDimension + 2]));
    ui->link_R->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pLinkDimension + 3]));
    ui->link_U->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pLinkDimension + 4]));
    ui->ep_cn2->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[pLinkDimension + 5]));
    ui->ep_c00->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[ep_CoorID]));
    ui->ep_c01->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[ep_CoorID + 1]));
    ui->ep_c10->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[ep_CoorID + 2]));
    ui->ep_c11->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[ep_CoorID + 3]));
    ui->ep_c20->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[ep_CoorID + 4]));
    ui->ep_c21->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[ep_CoorID + 5]));
    ui->ep_c30->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[ep_CoorID + 6]));
    ui->ep_c31->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[ep_CoorID + 7]));
    ui->ep_c40->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[ep_CoorID + 8]));
    ui->ep_c41->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[ep_CoorID + 9]));
    ui->ep_c50->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[ep_CoorID + 10]));
    ui->ep_c51->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[ep_CoorID + 11]));
    ui->lb0->setText(   QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[ep_CoorID + 12]));
    ui->lb1->setText(   QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[ep_CoorID + 13]));
    ui->lb2->setText(   QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[ep_CoorID + 14]));
    ui->lb3->setText(   QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[ep_CoorID + 15]));
    ui->lb4->setText(   QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[ep_CoorID + 16]));
    ui->lb5->setText(   QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[ep_CoorID + 17]));
    ui->ub0->setText(   QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[ep_CoorID + 18]));
    ui->ub1->setText(   QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[ep_CoorID + 19]));
    ui->ub2->setText(   QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[ep_CoorID + 20]));
    ui->ub3->setText(   QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[ep_CoorID + 21]));
    ui->ub4->setText(   QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[ep_CoorID + 22]));
    ui->ub5->setText(   QString("%1").arg(m_pScheduler->m_pSystemParameter->SystemParam[ep_CoorID + 23]));
}

void cstructparam::on_readBtn_clicked()
{
#ifdef USE_ARMPARAM
    // �ж������Ƿ�����
    if(!m_pScheduler->NetIsConnect()){
        QMessageBox::information(this,QString::fromLocal8Bit("����"),QString::fromLocal8Bit("�����ѶϿ�����!"));
        return;
    }
    m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/getsysparam", "yes");
#else
//    ui->A1->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->Robot_Link.A1));
//    ui->A2->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->Robot_Link.A2));
//    ui->A3->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->Robot_Link.A3));
//    ui->D3->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->Robot_Link.D3));
//    ui->D4->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->Robot_Link.D4));
//    ui->Df->setText(QString("%1").arg(m_pScheduler->m_pSystemParameter->Robot_Link.Df));
#endif
}

void cstructparam::on_saveBtn_clicked()
{
    m_pScheduler->m_pSystemParameter->SystemParam[pLinkDimension]   = ui->bx->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pLinkDimension+1] = ui->by->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pLinkDimension+2] = ui->bz->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pLinkDimension+3] = ui->link_R->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pLinkDimension+4] = ui->link_U->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[pLinkDimension+5] = ui->ep_cn2->text().toFloat();

    m_pScheduler->m_pSystemParameter->SystemParam[ep_CoorID]      = ui->ep_c00->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[ep_CoorID + 1]  = ui->ep_c01->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[ep_CoorID + 2]  = ui->ep_c10->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[ep_CoorID + 3]  = ui->ep_c11->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[ep_CoorID + 4]  = ui->ep_c20->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[ep_CoorID + 5]  = ui->ep_c21->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[ep_CoorID + 6]  = ui->ep_c30->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[ep_CoorID + 7]  = ui->ep_c31->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[ep_CoorID + 8]  = ui->ep_c40->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[ep_CoorID + 9]  = ui->ep_c41->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[ep_CoorID + 10] = ui->ep_c50->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[ep_CoorID + 11] = ui->ep_c51->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[ep_CoorID + 12] = ui->lb0->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[ep_CoorID + 13] = ui->lb1->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[ep_CoorID + 14] = ui->lb2->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[ep_CoorID + 15] = ui->lb3->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[ep_CoorID + 16] = ui->lb4->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[ep_CoorID + 17] = ui->lb5->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[ep_CoorID + 18] = ui->ub0->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[ep_CoorID + 19] = ui->ub1->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[ep_CoorID + 20] = ui->ub2->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[ep_CoorID + 21] = ui->ub3->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[ep_CoorID + 22] = ui->ub4->text().toFloat();
    m_pScheduler->m_pSystemParameter->SystemParam[ep_CoorID + 23] = ui->ub5->text().toFloat();
    m_pScheduler->WriteSystemParamInformation();

    // �ж������Ƿ�����
    if(!m_pScheduler->NetIsConnect()){
        QMessageBox::information(this,QString::fromLocal8Bit("����"),QString::fromLocal8Bit("�����ѶϿ�����!"));
        return;
    }
    m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/setParamer", "linkparam");
}

void cstructparam::on_restoreParam_clicked()
{
    bool res=false;

    // �ж������Ƿ�����
    if(!m_pScheduler->NetIsConnect()){
        QMessageBox::information(this,QString::fromLocal8Bit("����"),QString::fromLocal8Bit("�����ѶϿ�����!"));
        return;
    }

    // ��ȡ ����ֵ
    res = m_pScheduler->ReadSystemParamInformation("SysParInit.dat");
    if(!res){
        QMessageBox::information(this,QString::fromLocal8Bit("����"),QString::fromLocal8Bit("�����ļ���ʧ"));
        return;
    }
    if(!m_pScheduler->ReadAxisParamInformation("AxisParInit.dat")){
        QMessageBox::information(this,QString::fromLocal8Bit("ʧ��"),QString::fromLocal8Bit("�����ļ���ʧ"));
        return;
    }

    // ����� ��ǰֵ
    if(!m_pScheduler->WriteSystemParamInformation()){
        QMessageBox::information(this,QString::fromLocal8Bit("ʧ��"),QString::fromLocal8Bit("д���ļ�ʧ��"));
        return;
    }
    if(!m_pScheduler->WriteAxisParamInformation()){
        QMessageBox::information(this,QString::fromLocal8Bit("ʧ��"),QString::fromLocal8Bit("д���ļ�ʧ��"));
        return;
    }
    // ��λ������
    m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/setParamer", "all");
    QThread::msleep(300);
    while(((MotionControllerModule *)(m_pScheduler->getModulesList()->at(1)))->IsManualIdleStatus() == false){ // MANUAL_IDLE
        QThread::msleep(100);
    }
    m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/setaxisno", "yes");

    // ���½�������
//    refresh_systemParam();
//    refresh_AixsNOParam();
    // PID �����

    QMessageBox::information(this,QString::fromLocal8Bit("�ɹ�"),QString::fromLocal8Bit("�������!"));
}
