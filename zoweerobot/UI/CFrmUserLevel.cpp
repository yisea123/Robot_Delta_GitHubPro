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
        QMessageBox::information(this,QString::fromLocal8Bit("信息提示"),QString::fromLocal8Bit("密码错误!"));
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
        QMessageBox::information(this,QString::fromLocal8Bit("信息提示"),QString::fromLocal8Bit("两次输入密码不一致!"));
        return;
    }
    level = ui->cmbUserLevel->currentIndex();
    err = CUserLevelManage::ModifyUserPassword(level,ui->edtModityPsw->text());
    switch(err){  // 1:级别数超出范围 2:密码长度不匹配 3:输入非法字符
    case 0:
        ui->edtModityPsw->clear();
        ui->edtModityPsw2->clear();
        QMessageBox::information(this,QString::fromLocal8Bit("信息提示"),QString::fromLocal8Bit("密码修改成功!"));
        break;
    case 1:
        QMessageBox::information(this,QString::fromLocal8Bit("信息提示"),QString::fromLocal8Bit("级别数超出范围!"));
        break;
    case 2:
        QMessageBox::information(this,QString::fromLocal8Bit("信息提示"),QString::fromLocal8Bit("密码长度不匹配!"));
        break;
    case 3:
        QMessageBox::information(this,QString::fromLocal8Bit("信息提示"),QString::fromLocal8Bit("输入了非法字符!"));
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
