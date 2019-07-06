#include "cmanualmachiningstateui.h"
#include "ui_cmanualmachiningstateui.h"
#include "SystemSchedule.h"

unsigned char ReadBitMap[8]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
unsigned char ReadBitMap0[8]={0xFE,0xFD,0xFB,0xF7,0xEF,0xDF,0xBF,0x7F};

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
    int i, j;
    QCheckBox *pCkb=NULL;
    QString ctrlName;

    ui->setupUi(this);
    //QTimer* timer = new QTimer(this);
    //connect(timer, SIGNAL(timeout()),this, SLOT(updataData()));
    //timer->start(300);

    //init();
    m_tempAxis = DAxis(0, 0, 0, 0, 0, 0);

    // 由参数读入 输入信号接通时为高/低电平
    m_InVol = 0;  // 现在硬件的设计  是低电平时接通
    ui->cmbInVol->setCurrentIndex(m_InVol);
    ui->cmbInVol->hide(); // test

    m_IconOff = QIcon(":/images/setoff2.png");
    m_IconOn = QIcon(":/images/seton.png");
    //ui->btnOut0_0->setIcon(m_IconOff);

    for(i=0;i<6;i++){
        for(j=0;j<8;j++){
            ctrlName = QString::asprintf("ckbOut%d_%d",i,j);
            pCkb = ui->groupBox_2->findChild<QCheckBox *>(ctrlName);
            if(pCkb != NULL){
                connect(pCkb, SIGNAL(clicked(bool)),this, SLOT(on_ckbOut_clicked(bool)));
            }
        }
    }
}

CManualMachiningStateUi::~CManualMachiningStateUi()
{
    delete ui;
}

// 定时更新的数据
void CManualMachiningStateUi::TimerUpdateViewData(void)
{
    CCommmomUIWidget::TimerUpdateViewData();
    updataData();
}

void CManualMachiningStateUi::on_ckbOut_clicked(bool checked) // const QObject *sender,
{
    QString str;
    QCheckBox *pCkb=NULL;
    int num,bit;

    pCkb = dynamic_cast<QCheckBox *>(sender());
    if(pCkb != NULL){
        str = pCkb->objectName();  // ckbOut3_4
        num = str.mid(6,1).toLong();
        bit = str.mid(8,1).toInt();
        if(checked){
          SetOutSignal(num,bit,1);
        }else{
          SetOutSignal(num,bit,0);
        }
    }

    //if(sender!=NULL){
    //    str = sender->objectName();
    //}
}

// 判断 输入信号 有否接通
bool CManualMachiningStateUi::InSignalIsvalid(int num,int bit)
{
    unsigned char bitVal[8]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
    unsigned char val;

    if(!m_pScheduler->NetIsConnect()){
        return false;
    }
    
    val = (m_pScheduler->m_pSystemParameter->sys_ctrl.recvinput[num]&bitVal[bit]);
    if(m_InVol==val){
        return true;
    }else{
        return false;
    }
}

void CManualMachiningStateUi::SetOutSignal(int num,int bit,unsigned char val)
{
    //unsigned char cReadBitMap[8]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
    //unsigned char cReadBitMap0[8]={0xFE,0xFD,0xFB,0xF7,0xEF,0xDF,0xBF,0x7F};

    if(val==1){
        m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[num]|= ReadBitMap[bit];
    }else{
        m_pScheduler->m_pSystemParameter->sys_ctrl.sendoutput[num]&= ReadBitMap0[bit];
    }
}

unsigned char CManualMachiningStateUi::GetOutSignal(int num,int bit)
{
    unsigned char val;
    val = m_pScheduler->m_pSystemParameter->sys_ctrl.recvoutput[num]&ReadBitMap[bit];
    return val==0?0:1;
}

void CManualMachiningStateUi::updataData()
{
    int i,j;
    QCheckBox *pCkb=NULL;
    QPushButton *pBtn=NULL;
    QString ctrlName;
    //char val[6][8]={0};

    // 刷新 输入信号
    for(i=0;i<6;i++){
        for(j=0;j<8;j++){
            ctrlName = QString::asprintf("btnIn%d_%d",i,j);
            pBtn = ui->grbIN->findChild<QPushButton *>(ctrlName);
            if(pBtn != NULL){
                if(InSignalIsvalid(i,j)){
                    pBtn->setIcon(m_IconOn);
                }else{
                    pBtn->setIcon(m_IconOff);
                }
            }
        }
    }

    // 刷新 输出信号
    for(i=0;i<6;i++){
        for(j=0;j<8;j++){
            ctrlName = QString::asprintf("ckbOut%d_%d",i,j);
            pCkb = ui->groupBox_2->findChild<QCheckBox *>(ctrlName);
            if(pCkb != NULL){
                if(GetOutSignal(i,j)==0){
                    pCkb->setChecked(false);
                }else{
                    pCkb->setChecked(true);
                }
            }
        }
    }

}



