#include "CFrmUserLevel.h"
#include "ui_CFrmUserLevel.h"
#include "SystemSchedule.h"
#include "GlobalVariableType.h"
#include "CUserLevelManage.h"

CFrmUserLevel::CFrmUserLevel(QWidget *parent) :
    CCommmomUIWidget(parent),
    ui(new Ui::CFrmUserLevel)
{
    ui->setupUi(this);
}

CFrmUserLevel::CFrmUserLevel(QWidget *parent, SystemSchedule* schedule) :
    CCommmomUIWidget(parent),
    m_pScheduler(schedule),
    ui(new Ui::CFrmUserLevel)
{
    int level;

    ui->setupUi(this);

    UserParameter user;
    user = m_pScheduler->m_pSystemParameter->getUserInfo();
    level = user.m_permission;
    ui->cmbUserLevel->setCurrentIndex(level);

    if(level >= SUPER_USER){
        ui->ckbCollectMotionData->setHidden(false);
    }else{
        ui->ckbCollectMotionData->setHidden(true);
    }
}

CFrmUserLevel::~CFrmUserLevel()
{
    delete ui;
}

void CFrmUserLevel::on_btnOK_clicked()
{
    QString psw;
    int level;
    UserParameter user;

    psw = ui->edtUserPsw->text();
    level = CUserLevelManage::CheckUserPassword(psw);
    if(level<ORDINARY_USER){
        QMessageBox::information(this,QString::fromLocal8Bit("��Ϣ��ʾ"),QString::fromLocal8Bit("�������!"));
        return;
    }
    user = CUserLevelManage::m_OptUser[level];
    //
    m_pScheduler->m_pSystemParameter->setUserParm(user);

    ui->cmbUserLevel->setCurrentIndex(level);

    if(level >= SUPER_USER){
        ui->ckbCollectMotionData->setHidden(false);
    }else{
        ui->ckbCollectMotionData->setHidden(true);
    }

    ui->edtUserPsw->clear();
}

void CFrmUserLevel::on_ckbCollectMotionData_clicked(bool checked)
{
    m_pScheduler->m_pSystemParameter->m_bIsCollectMotionData = checked;
}

void CFrmUserLevel::on_btnModify_clicked()
{
    QString str1;
    int level, err;

    if(ui->edtModityPsw->text() != ui->edtModityPsw2->text()){
        QMessageBox::information(this,QString::fromLocal8Bit("��Ϣ��ʾ"),QString::fromLocal8Bit("�����������벻һ��!"));
        return;
    }
    level = ui->cmbUserLevel->currentIndex();
    err = CUserLevelManage::ModifyUserPassword(level,ui->edtModityPsw->text());
    switch(err){  // 1:������������Χ 2:���볤�Ȳ�ƥ�� 3:����Ƿ��ַ�
    case 0:
        ui->edtModityPsw->clear();
        ui->edtModityPsw2->clear();
        QMessageBox::information(this,QString::fromLocal8Bit("��Ϣ��ʾ"),QString::fromLocal8Bit("�����޸ĳɹ�!"));
        break;
    case 1:
        QMessageBox::information(this,QString::fromLocal8Bit("��Ϣ��ʾ"),QString::fromLocal8Bit("������������Χ!"));
        break;
    case 2:
        QMessageBox::information(this,QString::fromLocal8Bit("��Ϣ��ʾ"),QString::fromLocal8Bit("���볤�Ȳ�ƥ��!"));
        break;
    case 3:
        QMessageBox::information(this,QString::fromLocal8Bit("��Ϣ��ʾ"),QString::fromLocal8Bit("�����˷Ƿ��ַ�!"));
        break;
    default: break;
    }
}

void CFrmUserLevel::on_edtUserPsw_returnPressed()
{
   ui->btnOK->click();
}

void CFrmUserLevel::on_edtModityPsw2_returnPressed()
{
    ui->btnModify->click();
}
