#include "cautomachiningstateui.h"
#include "ui_cautomachiningstateui.h"
#include "SystemSchedule.h"
#include "GlobalDefine.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTimer>
#include <QTextBlock>

CAutoMachiningStateUi::CAutoMachiningStateUi(QWidget *parent) :
    CCommmomUIWidget(parent),
    ui(new Ui::CAutoMachiningStateUi)
{
    ui->setupUi(this);
}

CAutoMachiningStateUi::CAutoMachiningStateUi(QWidget *parent, SystemSchedule* schedule) :
    CCommmomUIWidget(parent),
    m_pScheduler(schedule),
    ui(new Ui::CAutoMachiningStateUi)
{
    ui->setupUi(this);
    init();
}

CAutoMachiningStateUi::~CAutoMachiningStateUi()
{
    delete ui;
}

void CAutoMachiningStateUi::init()
{
    m_bIsEmulationRunning = MOTION_EMULATION_IDLE;
    m_nProcessValue = 0;
    m_nLineNumber = 0;

    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()),
            this, SLOT(updataData()));
    timer->start(100);


    m_dAxis = DAxis(0, 0, 0, 0, 0 ,0);
    m_nToolID = 0;
    m_sToolType = "NULL";
    m_bIsNetConnect = true;
}

// ��ʱ���µ�����
void CAutoMachiningStateUi::TimerUpdateViewData(void)
{
    CCommmomUIWidget::TimerUpdateViewData();
    updataData();
}

void CAutoMachiningStateUi::updataData()
{
    QString str[4];
    float crd[4]={0};
    int i,line,cnt;
	/*if((m_pScheduler->m_pSystemParameter->sys_ctrl.sendinput[0])&0x2)//stopbit set
	{
		if((m_pScheduler->m_pSystemParameter->sys_ctrl.statebit&0x400)
			&&(0==(m_pScheduler->m_pSystemParameter->sys_ctrl.statebit&0x80)))
		{
			(m_pScheduler->m_pSystemParameter->sys_ctrl.sendinput[0])&=~0x2;//clear stopbit
		}

    }*/
//    ui->widget_2->setStyleSheet("QWidget{"
//                                "border: 1px solid #FF00FF;"
//                                "border-radius: 5px;"
//                                "}");

        ui->x->setNum(m_pScheduler->m_pSystemParameter->coor_car_pos[0]);
	 ui->y->setNum(m_pScheduler->m_pSystemParameter->coor_car_pos[1]);
	 ui->z->setNum(m_pScheduler->m_pSystemParameter->coor_car_pos[2]);
	 ui->w->setNum(m_pScheduler->m_pSystemParameter->coor_car_pos[3]);
	 ui->p->setNum(m_pScheduler->m_pSystemParameter->coor_car_pos[4]);
	 ui->r->setNum(m_pScheduler->m_pSystemParameter->coor_car_pos[5]);
     ui->m->setNum(m_pScheduler->m_pSystemParameter->coor_car_pos[6]);

	 ui->A->setNum(m_pScheduler->m_pSystemParameter->coor_joint_pos[0]);
	 ui->B->setNum(m_pScheduler->m_pSystemParameter->coor_joint_pos[1]);
	 ui->C->setNum(m_pScheduler->m_pSystemParameter->coor_joint_pos[2]);
	 ui->D->setNum(m_pScheduler->m_pSystemParameter->coor_joint_pos[3]);
	 ui->E->setNum(m_pScheduler->m_pSystemParameter->coor_joint_pos[4]);
	 ui->F->setNum(m_pScheduler->m_pSystemParameter->coor_joint_pos[5]);
     ui->G->setNum(m_pScheduler->m_pSystemParameter->coor_joint_pos[6]);
#ifdef PROGRAM_USELVDS
     ui->comand_multi->setText(QString::fromLocal8Bit("ָ���Ȧ"));
     ui->comand_single->setText(QString::fromLocal8Bit("ָ�Ȧ"));
     ui->real_multi->setText(QString::fromLocal8Bit("ʵ�ʶ�Ȧ"));
     ui->real_single->setText(QString::fromLocal8Bit("ʵ�ʵ�Ȧ"));
     ui->axis1_pulse_multi->setNum((int)m_pScheduler->m_pSystemParameter->plusepos[0]);
     ui->axis1_pulse_single->setNum((int)m_pScheduler->m_pSystemParameter->plusepos[1]);
     ui->axis2_pulse_multi->setNum((int)m_pScheduler->m_pSystemParameter->plusepos[2]);
     ui->axis2_pulse_single->setNum((int)m_pScheduler->m_pSystemParameter->plusepos[3]);
      ui->axis3_pulse_multi->setNum((int)m_pScheduler->m_pSystemParameter->plusepos[4]);
     ui->axis3_pulse_single->setNum((int)m_pScheduler->m_pSystemParameter->plusepos[5]);
     ui->axis4_pulse_multi->setNum((int)m_pScheduler->m_pSystemParameter->plusepos[6]);
     ui->axis4_pulse_single->setNum((int)m_pScheduler->m_pSystemParameter->plusepos[7]);
     ui->axis5_pulse_multi->setNum((int)m_pScheduler->m_pSystemParameter->plusepos[8]);
     ui->axis5_pulse_single->setNum((int)m_pScheduler->m_pSystemParameter->plusepos[9]);
     ui->axis6_pulse_multi->setNum((int)m_pScheduler->m_pSystemParameter->plusepos[10]);
     ui->axis6_pulse_single->setNum((int)m_pScheduler->m_pSystemParameter->plusepos[11]);
     ui->axis7_pulse_single->setNum((int)m_pScheduler->m_pSystemParameter->plusepos[12]);

     ui->axis1_multi->setNum((int)m_pScheduler->m_pSystemParameter->realaxis[0]);
     ui->axis1_single->setNum((int)m_pScheduler->m_pSystemParameter->realaxis[1]);
     ui->axis2_multi->setNum((int)m_pScheduler->m_pSystemParameter->realaxis[2]);
     ui->axis2_single->setNum((int)m_pScheduler->m_pSystemParameter->realaxis[3]);
      ui->axis3_multi->setNum((int)m_pScheduler->m_pSystemParameter->realaxis[4]);
     ui->axis3_single->setNum((int)m_pScheduler->m_pSystemParameter->realaxis[5]);
     ui->axis4_multi->setNum((int)m_pScheduler->m_pSystemParameter->realaxis[6]);
     ui->axis4_single->setNum((int)m_pScheduler->m_pSystemParameter->realaxis[7]);
     ui->axis5_multi->setNum((int)m_pScheduler->m_pSystemParameter->realaxis[8]);
     ui->axis5_single->setNum((int)m_pScheduler->m_pSystemParameter->realaxis[9]);
     ui->axis6_multi->setNum((int)m_pScheduler->m_pSystemParameter->realaxis[10]);
     ui->axis6_single->setNum((int)m_pScheduler->m_pSystemParameter->realaxis[11]);
     ui->axis7_single->setNum((int)m_pScheduler->m_pSystemParameter->realaxis[12]);
#else
      ui->comand_multi->setText(QString::fromLocal8Bit("                        "));
     ui->comand_single->setText(QString::fromLocal8Bit("ָ��λ��"));
     ui->real_multi->setText(QString::fromLocal8Bit("                         "));
     ui->real_single->setText(QString::fromLocal8Bit("ʵ��λ��"));
     ui->axis1_pulse_multi->setNum(0);
     ui->axis1_pulse_single->setNum((int)m_pScheduler->m_pSystemParameter->plusepos[1]);
     ui->axis2_pulse_multi->setNum(0);
     ui->axis2_pulse_single->setNum((int)m_pScheduler->m_pSystemParameter->plusepos[3]);
      ui->axis3_pulse_multi->setNum(0);
     ui->axis3_pulse_single->setNum((int)m_pScheduler->m_pSystemParameter->plusepos[5]);
     ui->axis4_pulse_multi->setNum(0);
     ui->axis4_pulse_single->setNum((int)m_pScheduler->m_pSystemParameter->plusepos[7]);
    ui->axis5_pulse_multi->setNum(0);
     ui->axis5_pulse_single->setNum((int)m_pScheduler->m_pSystemParameter->plusepos[9]);
     ui->axis6_pulse_multi->setNum(0);
     ui->axis6_pulse_single->setNum((int)m_pScheduler->m_pSystemParameter->plusepos[11]);
     ui->axis7_pulse_multi->setNum(0);
     ui->axis7_pulse_single->setNum((int)m_pScheduler->m_pSystemParameter->plusepos[13]);
	 
     ui->axis1_multi->setNum(0);
     ui->axis1_single->setNum((int)m_pScheduler->m_pSystemParameter->realaxis[1]);
     ui->axis2_multi->setNum(0);
     ui->axis2_single->setNum((int)m_pScheduler->m_pSystemParameter->realaxis[3]);
      ui->axis3_multi->setNum(0);
     ui->axis3_single->setNum((int)m_pScheduler->m_pSystemParameter->realaxis[5]);
     ui->axis4_multi->setNum(0);
     ui->axis4_single->setNum((int)m_pScheduler->m_pSystemParameter->realaxis[7]);
      ui->axis5_multi->setNum(0);
     ui->axis5_single->setNum((int)m_pScheduler->m_pSystemParameter->realaxis[9]);
     ui->axis6_multi->setNum(0);
     ui->axis6_single->setNum((int)m_pScheduler->m_pSystemParameter->realaxis[11]);
     ui->axis7_multi->setNum(0);
     ui->axis7_single->setNum((int)m_pScheduler->m_pSystemParameter->realaxis[13]);
#endif
     cnt = m_pScheduler->getNGFileContexts()->size();
     if((0x40==(m_pScheduler->m_pSystemParameter->sys_ctrl.statebit&0x60)) // ģʽָ�� Ϊ�Զ���ʽ
     &&(m_pScheduler->m_pSystemParameter->sys_ctrl.decode_id<=cnt)
     && (!(m_pScheduler->m_pSystemParameter->sys_ctrl.statebit&0x400)))  // �Ƿ�ֹͣ״̬
     {
         line = (int)m_pScheduler->m_pSystemParameter->sys_ctrl.decode_id;
         ui->NGCodeText->highlightSpecifiedLine(line);
     }
}

void CAutoMachiningStateUi::on_openfile_clicked()
{
    QFileDialog *fileDialog = new QFileDialog();
	if(fileDialog!=NULL)
	{
	    fileDialog->setWindowTitle(tr("Open G�����ļ�"));
	    fileDialog->setDirectory("../NGfile");
	    //fileDialog->setFilter(tr("Image Files(*.PRG *.PNC *.FNC)"));
         fileDialog->setNameFilter(tr("Image Files(*.PRG *.PNC *.FNC)"));
	    if(fileDialog->exec() == QDialog::Accepted)
	    {
                ui->filename->setText(fileDialog->selectedFiles()[0]);
	        m_pScheduler->setNGFilePath(fileDialog->selectedFiles()[0]);
	        showFileContext(m_pScheduler->getNGFilePath());
	        m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "draw background");
	    }
	    else
	    {
	        QMessageBox::information(NULL, tr("Path"), tr("You didn't select any files."));
	    }
	}	
    delete(fileDialog);
	fileDialog=NULL;
}

/*************************************************
  �������ƣ�readNGFile(QString f)
  �������ܣ���ȡG�����ļ��������ݱ�����m_fileContexts��
  ����������f  �ļ�·��
  ����������
  �� �� ֵ��
  ��    ע��
**************************************************/
void  CAutoMachiningStateUi::readNGFile(QString f)
{
    QFile file(f);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    ContextRecord  text;
    int  i = 1;
    m_pScheduler->clearFileContextList();

    QList<ContextRecord>*  contexts = m_pScheduler->getNGFileContexts();

    while(!file.atEnd())
    {
        text.SerialNumber = i++;
        text.TextStr = file.readLine();
        text.TextStr.remove('\n');
        contexts->append(text);
    }
}

/*************************************************
  �������ƣ�showFileContext()
  �������ܣ��ڿؼ�����ʾG��������
  ����������dNewValue �µ�IO��ֵ
  ����������
  �� �� ֵ��
  ��    ע��
**************************************************/
void  CAutoMachiningStateUi::showFileContext(QString f)
{
    readNGFile(f);

    QList<ContextRecord>*  contexts = m_pScheduler->getNGFileContexts();


    ui->NGCodeText->clear();
    for(int i = 0; i < contexts->size(); i++)
    {
        ui->NGCodeText->appendPlainText(contexts->at(i).TextStr);
    }
}

CEmulationCanvs* CAutoMachiningStateUi::getCanvsHandle()
{

}

void CAutoMachiningStateUi::setProcessBarValue(int v)
{

    mutex.lock();
 
    m_nProcessValue = v;
    mutex.unlock();
}
void CAutoMachiningStateUi::setIsEmulationRunning(MotionRunStatue b)
{
    mutex.lock();
    m_bIsEmulationRunning = b;
    mutex.unlock();
    qDebug()<<"m_bIsEmulationRunning"<<m_bIsEmulationRunning;


}
void CAutoMachiningStateUi::setCurrentNGCodeLine(int lineNumber)
{
    mutex.lock();
    m_nLineNumber = lineNumber;
    mutex.unlock();
}


int CAutoMachiningStateUi::getBreakpoint()
{

}

void CAutoMachiningStateUi::setPostext(DAxis axis)
{
    mutex.lock();
    m_dAxis = axis;
    mutex.unlock();
}

void CAutoMachiningStateUi::setToolText(QString toolType, int toolNum)
{
    mutex.lock();
    m_nToolID = toolNum;
    m_sToolType = QString::number(toolNum);
    mutex.unlock();
}



void CAutoMachiningStateUi::motionRun()
{

}

void CAutoMachiningStateUi::motionPause()
{

}

void CAutoMachiningStateUi::motionStop()
{

}

void CAutoMachiningStateUi::updateAllOutputBnt(unsigned char* a, int n)
{
    
}

void CAutoMachiningStateUi::updateOutputBnt(int index, bool sts)
{
   
}

void CAutoMachiningStateUi::setDisableEdit(bool f)
{
    ui->editfile->setDisabled(f);
}

void CAutoMachiningStateUi::setClampPos(Point axis1, Point axis2)
{
    /*ui->clamp1Posx->setText(QString::number(axis1.x));
    ui->clamp1Posy->setText(QString::number(axis1.y));
    ui->clamp2Posx->setText(QString::number(axis2.x));
    ui->clamp2Posy->setText(QString::number(axis2.y));*/
}

bool CAutoMachiningStateUi::getIOStatus(unsigned char* a, int bit)
{
    int ik = bit / 8;
    int jk = bit % 8;

    return ((a[ik] & (0x01 << jk)) > 0) ? true : false;
}

void CAutoMachiningStateUi::on_savefile_clicked()
{
     {
        if (ui->filename->text() != "")
        {
            QFile file(ui->filename->text());
            if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                QMessageBox::information(NULL,QString::fromLocal8Bit("����"), QString::fromLocal8Bit("�ӹ��ļ��޷��򿪣�"));
                return;
            }

            QTextStream in(&file);
            in << ui->NGCodeText->toPlainText();

            file.close();

            readNGFile(ui->filename->text());
            m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "draw background");
        }
        else//û���ļ��򴴽��µ��ļ�
        {
            QString fileName;
            fileName = QFileDialog::getSaveFileName(this, QString::fromLocal8Bit("�����ļ�"), " ", QString::fromLocal8Bit("Save Files(*.PNC)"));

            if (!fileName.isNull())
            {
                ui->filename->setText(fileName);
                m_pScheduler->setNGFilePath(fileName);

                QFile file(fileName);
                if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
                {
                    QMessageBox::information(NULL, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("�ӹ��ļ��޷��򿪣�"));
                    return;
                }

                QTextStream in(&file);
                in << ui->NGCodeText->toPlainText();

                file.close();

                readNGFile(ui->filename->text());
                m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "draw background");
            }
            else
            {
                QMessageBox::information(NULL, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("���ȱ����ļ�!"));
            }
        }
    }
   
}

void CAutoMachiningStateUi::on_createfile_clicked()
{
    
        ui->NGCodeText->clear();
        ui->filename->clear();
        m_pScheduler->clearFileContextList();
 
}

void CAutoMachiningStateUi::on_editfile_clicked(bool checked)
{
    ui->NGCodeText->setReadOnly(!checked);
}



void CAutoMachiningStateUi::on_addtechpoint_clicked()
{
    int i;
    float crd[4]={0};
    QString text;
    int tool,hand;

    for(i=0;i<4;i++){
        crd[i] = m_pScheduler->m_pSystemParameter->coor_car_pos[i];
    }

    if(m_pScheduler->m_pSystemParameter->coor_joint_pos[1]>0.000001){  // ��ϵ
        hand = 1;
    }else if(m_pScheduler->m_pSystemParameter->coor_joint_pos[1]<-0.000001){
        hand = 2;
    }else{
        hand = 0;
    }
    tool = m_pScheduler->m_pSystemParameter->m_currentTool;

    text = QString::asprintf("G00 X%0.3f Y%0.3f Z%0.3f R%0.3f T%d H%d;",crd[0],crd[1],crd[2],crd[3],tool,hand);
    ui->NGCodeText->textCursor().insertText(text);    
}
