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

void CAutoMachiningStateUi::updataData()
{
	/*if((m_pScheduler->m_pSystemParameter->sys_ctrl.sendinput[0])&0x2)//stopbit set
	{
		if((m_pScheduler->m_pSystemParameter->sys_ctrl.statebit&0x400)
			&&(0==(m_pScheduler->m_pSystemParameter->sys_ctrl.statebit&0x80)))
		{
			(m_pScheduler->m_pSystemParameter->sys_ctrl.sendinput[0])&=~0x2;//clear stopbit
		}

	}*/
        ui->x->setNum(m_pScheduler->m_pSystemParameter->coor_car_pos[0]);
	 ui->y->setNum(m_pScheduler->m_pSystemParameter->coor_car_pos[1]);
	 ui->z->setNum(m_pScheduler->m_pSystemParameter->coor_car_pos[2]);
	 ui->w->setNum(m_pScheduler->m_pSystemParameter->coor_car_pos[3]);
	 ui->p->setNum(m_pScheduler->m_pSystemParameter->coor_car_pos[4]);
	 ui->r->setNum(m_pScheduler->m_pSystemParameter->coor_car_pos[5]);

	 ui->A->setNum(m_pScheduler->m_pSystemParameter->coor_joint_pos[0]);
	 ui->B->setNum(m_pScheduler->m_pSystemParameter->coor_joint_pos[1]);
	 ui->C->setNum(m_pScheduler->m_pSystemParameter->coor_joint_pos[2]);
	 ui->D->setNum(m_pScheduler->m_pSystemParameter->coor_joint_pos[3]);
	 ui->E->setNum(m_pScheduler->m_pSystemParameter->coor_joint_pos[4]);
	 ui->F->setNum(m_pScheduler->m_pSystemParameter->coor_joint_pos[5]);
#ifdef PROGRAM_USELVDS
     ui->comand_multi->setText(QString::fromLocal8Bit("Ö¸Áî¶àÈ¦"));
     ui->comand_single->setText(QString::fromLocal8Bit("Ö¸Áîµ¥È¦"));
     ui->real_multi->setText(QString::fromLocal8Bit("Êµ¼Ê¶àÈ¦"));
     ui->real_single->setText(QString::fromLocal8Bit("Êµ¼Êµ¥È¦"));
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
#else
      ui->comand_multi->setText(QString::fromLocal8Bit("                        "));
     ui->comand_single->setText(QString::fromLocal8Bit("Ö¸ÁîÎ»ÖÃ"));
     ui->real_multi->setText(QString::fromLocal8Bit("                         "));
     ui->real_single->setText(QString::fromLocal8Bit("Êµ¼ÊÎ»ÖÃ"));
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
#endif
     if((0x40==(m_pScheduler->m_pSystemParameter->sys_ctrl.statebit&0x60))
	 &&(m_pScheduler->m_pSystemParameter->sys_ctrl.decode_id<m_pScheduler->getNGFileContexts()->size())
	 && (!(m_pScheduler->m_pSystemParameter->sys_ctrl.statebit&0x400)))
     {
	ui->NGCodeText->highlightSpecifiedLine((int)m_pScheduler->m_pSystemParameter->sys_ctrl.decode_id);
     }
}

void CAutoMachiningStateUi::on_openfile_clicked()
{
    QFileDialog *fileDialog = new QFileDialog();
	if(fileDialog!=NULL)
	{
	    fileDialog->setWindowTitle(tr("Open Gï¿½ï¿½ï¿½ï¿½ï¿½Ä¼ï¿½"));
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
  ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Æ£ï¿½readNGFile(QString f)
  ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ü£ï¿½ï¿½ï¿½È¡Gï¿½ï¿½ï¿½ï¿½ï¿½Ä¼ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ý±ï¿½ï¿½ï¿½ï¿½ï¿½m_fileContextsï¿½ï¿½
  ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½f  ï¿½Ä¼ï¿½Â·ï¿½ï¿½
  ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
  ï¿½ï¿½ ï¿½ï¿½ Öµï¿½ï¿½
  ï¿½ï¿½    ×¢ï¿½ï¿½
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
  ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Æ£ï¿½showFileContext()
  ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ü£ï¿½ï¿½Ú¿Ø¼ï¿½ï¿½ï¿½ï¿½ï¿½Ê¾Gï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
  ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½dNewValue ï¿½Âµï¿½IOï¿½ï¿½Öµ
  ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
  ï¿½ï¿½ ï¿½ï¿½ Öµï¿½ï¿½
  ï¿½ï¿½    ×¢ï¿½ï¿½
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
                QMessageBox::information(NULL,QString::fromLocal8Bit("´íÎó"), QString::fromLocal8Bit("¼Ó¹¤ÎÄ¼þÎÞ·¨´ò¿ª£¡"));
                return;
            }

            QTextStream in(&file);
            in << ui->NGCodeText->toPlainText();

            file.close();

            readNGFile(ui->filename->text());
            m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "draw background");
        }
        else//Ã»ï¿½ï¿½ï¿½Ä¼ï¿½ï¿½ò´´½ï¿½ï¿½Âµï¿½ï¿½Ä¼ï¿½
        {
            QString fileName;
            fileName = QFileDialog::getSaveFileName(this, QString::fromLocal8Bit("±£´æÎÄ¼þ"), " ", QString::fromLocal8Bit("Save Files(*.PNC)"));

            if (!fileName.isNull())
            {
                ui->filename->setText(fileName);
                m_pScheduler->setNGFilePath(fileName);

                QFile file(fileName);
                if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
                {
                    QMessageBox::information(NULL, QString::fromLocal8Bit("´íÎó"), QString::fromLocal8Bit("¼Ó¹¤ÎÄ¼þÎÞ·¨´ò¿ª£¡"));
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
                QMessageBox::information(NULL, QString::fromLocal8Bit("´íÎó"), QString::fromLocal8Bit("ÇëÏÈ±£´æÎÄ¼þ!"));
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
    QString text= QString("G01x%1y%2z%3w%4p%5r%6").arg(m_pScheduler->m_pSystemParameter->coor_car_pos[0]).arg(m_pScheduler->m_pSystemParameter->coor_car_pos[1]).arg(m_pScheduler->m_pSystemParameter->coor_car_pos[2]).arg(m_pScheduler->m_pSystemParameter->coor_car_pos[3]).arg(m_pScheduler->m_pSystemParameter->coor_car_pos[4]).arg(m_pScheduler->m_pSystemParameter->coor_car_pos[5]);
    ui->NGCodeText->textCursor().insertText(text);
}
