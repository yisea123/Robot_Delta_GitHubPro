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

// ÿ�ν������ʱ ����
void CFrmSetZero::UpdateViewData(void)
{
    int i,zeroCnt=0;

    CCommmomUIWidget::UpdateViewData();

    for(i=0;i<4;i++){
        m_PreHomeOrder[i] = m_pScheduler->m_pSystemParameter->SystemParam[GoHomeOrder1+i];
        if(m_PreHomeOrder[i]<0 || m_PreHomeOrder[i]>4321){
            m_PreHomeOrder[i] = 0;
            m_pScheduler->m_pSystemParameter->SystemParam[GoHomeOrder1+i] = 0;
        }
        if(m_PreHomeOrder[i] == 0){
            zeroCnt++;
        }
    }
    if(zeroCnt==4){ // ȫΪ0ʱ,Ĭ��Ϊȫ��ͬʱ����
        m_PreHomeOrder[0] = 1234;
        m_pScheduler->m_pSystemParameter->SystemParam[GoHomeOrder1] = 1234;
    }

    if(m_PreHomeOrder[0]==0){
        ui->edtHomeOrder1->clear();
    }else{
        ui->edtHomeOrder1->setText(QString::asprintf("%d",m_PreHomeOrder[0]));
    }
    if(m_PreHomeOrder[1]==0){
        ui->edtHomeOrder2->clear();
    }else{
        ui->edtHomeOrder2->setText(QString::asprintf("%d",m_PreHomeOrder[1]));
    }
    if(m_PreHomeOrder[2]==0){
        ui->edtHomeOrder3->clear();
    }else{
        ui->edtHomeOrder3->setText(QString::asprintf("%d",m_PreHomeOrder[2]));
    }
    if(m_PreHomeOrder[3]==0){
        ui->edtHomeOrder4->clear();
    }else{
        ui->edtHomeOrder4->setText(QString::asprintf("%d",m_PreHomeOrder[3]));
    }
}

// ��ʱ���µ�����
void CFrmSetZero::TimerUpdateViewData(void)
{
    QString str[4];
    int i;

    CCommmomUIWidget::TimerUpdateViewData(); //

    for(i=0;i<4;i++){
        str[i] = QString().sprintf(CoordFormat,m_pScheduler->m_pSystemParameter->coor_car_pos[i]);
    }
    ui->x->setText(str[0]);
    ui->y->setText(str[1]);
    ui->z->setText(str[2]);
    ui->r->setText(str[3]);

    for(i=0;i<4;i++){
        str[i] = QString().sprintf(CoordFormat,m_pScheduler->m_pSystemParameter->coor_joint_pos[i]);
    }
    ui->A->setText(str[0]);
    ui->B->setText(str[1]);
    ui->C->setText(str[2]);
    ui->D->setText(str[3]);


     str[0] = QString().sprintf("%10d",(int)m_pScheduler->m_pSystemParameter->zeropos[1]);
     ui->axis1_zero_single->setText(str[0]);

     str[0] = QString().sprintf("%10d",(int)m_pScheduler->m_pSystemParameter->zeropos[3]);
     ui->axis2_zero_single->setText(str[0]);

     str[0] = QString().sprintf("%10d",(int)m_pScheduler->m_pSystemParameter->zeropos[5]);
     ui->axis3_zero_single->setText(str[0]);

     str[0] = QString().sprintf("%10d",(int)m_pScheduler->m_pSystemParameter->zeropos[7]);
     ui->axis4_zero_single->setText(str[0]);

     ui->A1->setNum(m_pScheduler->m_pSystemParameter->Robot_Link.A1);
     ui->A2->setNum(m_pScheduler->m_pSystemParameter->Robot_Link.A2);
     ui->A3->setNum(m_pScheduler->m_pSystemParameter->Robot_Link.A3);
     ui->D3->setNum(m_pScheduler->m_pSystemParameter->Robot_Link.D3);
     ui->D4->setNum(m_pScheduler->m_pSystemParameter->Robot_Link.D4);
     ui->Df->setNum(m_pScheduler->m_pSystemParameter->Robot_Link.Df);

}


void CFrmSetZero::on_zerostep1_clicked()
{
    if(!m_pScheduler->NetIsConnect()){ // �ж������Ƿ�����
        QMessageBox::information(this,QString::fromLocal8Bit("����"),QString::fromLocal8Bit("�����ѶϿ�!"));
        return;
    }
    m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/setZerostep", "1");
}

void CFrmSetZero::on_zerostep2_clicked()
{
    if(!m_pScheduler->NetIsConnect()){ // �ж������Ƿ�����
        QMessageBox::information(this,QString::fromLocal8Bit("����"),QString::fromLocal8Bit("�����ѶϿ�!"));
        return;
    }
    m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/setZerostep", "2");
}

void CFrmSetZero::on_zerostep3_clicked()
{
    if(!m_pScheduler->NetIsConnect()){ // �ж������Ƿ�����
        QMessageBox::information(this,QString::fromLocal8Bit("����"),QString::fromLocal8Bit("�����ѶϿ�!"));
        return;
    }
    m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/setZerostep", "3");
}

void CFrmSetZero::on_zerostep4_clicked()
{
    if(!m_pScheduler->NetIsConnect()){ // �ж������Ƿ�����
        QMessageBox::information(this,QString::fromLocal8Bit("����"),QString::fromLocal8Bit("�����ѶϿ�!"));
        return;
    }
    m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/setZerostep", "4");
}

void CFrmSetZero::on_edtHomeOrder_editingFinished()
{
    int i,index,len;
    QString str;
    QLineEdit * edt=NULL;
    int cnt[4]; //
    int ch;
    bool err=false;

    edt = dynamic_cast<QLineEdit *>(sender());
    if(edt != NULL){
        str = edt->text().trimmed();
        len = str.length();
        index = edt->objectName().right(1).toInt()-1; // edtHomeOrder1

        if(len == 0){ // û����,��0��ʾ
            edt->clear();
            m_PreHomeOrder[index] = 0;
            return;
        }else if(len>4){ //
            err = true;
            edt->setText(QString::asprintf("%d",m_PreHomeOrder[index])); // ��ԭ����
        }
        memset(cnt,0,sizeof(cnt));
        for(i=0;i<len;i++){
            ch = str.at(i).toLatin1()-'1';
            if(ch<0 || ch>3){
                err = true;
                break;
            }
            cnt[ch]++;
            if(cnt[ch]>1){
                err = true;
                break;
            }
        }
        if(err){
            edt->setText(QString::asprintf("%d",m_PreHomeOrder[index])); // ��ԭ����
        }else{
            m_PreHomeOrder[index] = str.toInt();
            edt->setText(str);
        }
    }
}

void CFrmSetZero::on_edtHomeOrder1_editingFinished()
{
    on_edtHomeOrder_editingFinished();
}

void CFrmSetZero::on_edtHomeOrder2_editingFinished()
{
    on_edtHomeOrder_editingFinished();
}

void CFrmSetZero::on_edtHomeOrder3_editingFinished()
{
    on_edtHomeOrder_editingFinished();
}

void CFrmSetZero::on_edtHomeOrder4_editingFinished()
{
    on_edtHomeOrder_editingFinished();
}

// ��� ����� ����˳�� ������Ч
// line: ������к�
// ���� 0:��Ч 1:��ų�����Χ 2:����ظ� 3:
int CFrmSetZero::IsHomeOrderValid(int &line)
{
    int i,j, len,all;
    int index,cnt[4]; //
    QString str;

    memset(cnt,0,sizeof(cnt));
    all = 0;
    for(i=0;i<4;i++){ // 4���� ���4��
        if(m_PreHomeOrder[i]==0) continue;
        str = QString::asprintf("%d",m_PreHomeOrder[i]);
        len = str.length();
        for(j=0;j<len;j++){
            index = str.at(j).toLatin1()-'1';
            if(index<0 || index>3){
                line = i;
                return 1;
            }
            cnt[index]++;
            all++;
            if(cnt[index]>1){
                line = i;
                return 2;
            }
        }
        if(all==4){ // ����ľͲ��ټ��
            return 0;
        }
    }
    return 0;
}

void CFrmSetZero::on_btnHomeOrder_clicked()
{
    int i,line;
    int err; //
    QString str;
    QLineEdit * edt=NULL;
    QString ctrlName;
    char errString[4][30]={"��ų�����Χ!","����ظ�!","",""};

    if(ui->edtHomeOrder1->isModified()){
        ui->edtHomeOrder1->editingFinished();
    }
    if(ui->edtHomeOrder2->isModified()){
        ui->edtHomeOrder2->editingFinished();
    }
    if(ui->edtHomeOrder3->isModified()){
        ui->edtHomeOrder3->editingFinished();
    }
    if(ui->edtHomeOrder4->isModified()){
        ui->edtHomeOrder4->editingFinished();
    } 

    // ��� �Ϸ���
    err = IsHomeOrderValid(line);
    if(err > 0){  // ���� 0:��Ч 1:��ų�����Χ 2:����ظ�
        QMessageBox::information(this,QString::fromLocal8Bit("��Ϣ��ʾ"),QString::fromLocal8Bit(errString[err-1]));
        ctrlName = QString::asprintf("edtHomeOrder%d",line+1);
        edt = ui->frmHomeOrder->findChild<QLineEdit *>(ctrlName);
        if(edt != NULL){
            edt->setFocus();
            edt->selectAll();
        }
        return;
    }
    //
    for(i=0;i<4;i++){
        m_pScheduler->m_pSystemParameter->SystemParam[GoHomeOrder1+i] = m_PreHomeOrder[i];
    }
    m_pScheduler->WriteSystemParamInformation();

    // �ж������Ƿ�����
    if(!m_pScheduler->NetIsConnect()){
        QMessageBox::information(this,QString::fromLocal8Bit("����"),QString::fromLocal8Bit("�����ѶϿ�!"));
        return;
    }
    m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/setParamer", "all");
    QMessageBox::information(this,QString::fromLocal8Bit("��Ϣ"),QString::fromLocal8Bit("�������!"));
    //m_pScheduler->writeMsgToStatusBar(QString::fromLocal8Bit("�������!"));
}


