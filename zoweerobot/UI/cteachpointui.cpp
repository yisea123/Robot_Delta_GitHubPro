#include "cteachpointui.h"
#include "ui_cteachpointui.h"
#include "SystemSchedule.h"
#include <QTimer>
#include <QFileDialog>
cteachpointui::cteachpointui(QWidget *parent) :
    CCommmomUIWidget(parent),
    ui(new Ui::cteachpointui)
{
    ui->setupUi(this);

    m_pDialog = NULL;
    m_Saving = false;
}


cteachpointui::cteachpointui(QWidget *parent, SystemSchedule* schedule) :
    CCommmomUIWidget(parent),
    m_pScheduler(schedule),
    ui(new Ui::cteachpointui)
{
    ui->setupUi(this);
	m_pDialog=NULL;
	m_Saving=0;
    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()),
            this, SLOT(updataData()));
    timer->start(100);
    model = new QStandardItemModel(this);
    init();
    
    ui->getteachpoint->setHidden(true);
}
cteachpointui::~cteachpointui()
{
    delete ui;
}

// ��ʱ���µ�����
void cteachpointui::TimerUpdateViewData(void)
{
    CCommmomUIWidget::TimerUpdateViewData();
    updataData();
}


void cteachpointui::init()
{
	
	m_idisplaycnt=0;
	/*�������ֶ���*/
	model->setColumnCount(9);
	model->setHeaderData(0,Qt::Horizontal, QString::fromLocal8Bit("ʾ�̵����"));
	model->setHeaderData(1,Qt::Horizontal, "X");
	model->setHeaderData(2,Qt::Horizontal, "Y");
	model->setHeaderData(3,Qt::Horizontal, "Z");
	model->setHeaderData(4,Qt::Horizontal, "W");
	model->setHeaderData(5,Qt::Horizontal, "P");
	model->setHeaderData(6,Qt::Horizontal, "R");
    model->setHeaderData(7,Qt::Horizontal, "M");
	//model->setHeaderData(7,Qt::Horizontal, QString::fromLocal8Bit("���ߺ�"));
	//model->setHeaderData(8,Qt::Horizontal, QString::fromLocal8Bit("��ϵ"));
	//model->setHeaderData(9,Qt::Horizontal, QString::fromLocal8Bit("���ɵȼ�"));
    model->setHeaderData(8,Qt::Horizontal, QString::fromLocal8Bit("ע��"));
	/*�������ֶ���*/
	/*model->setRowCount(3);
	model->setHeaderData(0,Qt::Vertical, QString::fromLocal8Bit("��¼һ"));
	model->setHeaderData(1,Qt::Vertical, QString::fromLocal8Bit("��¼��"));
	model->setHeaderData(2,Qt::Vertical, QString::fromLocal8Bit("��¼��"));*/

	/*����һ������*/ 
	for(int i=0;i<TEACHPIONTNUM;i++)
	{
		model->setItem(i, 0, new QStandardItem(QString("%1").arg(m_pScheduler->m_pSystemParameter->teachpoint[i].index))); 
		model->setItem(i, 1, new QStandardItem(QString("%1").arg(m_pScheduler->m_pSystemParameter->teachpoint[i].pos[0])));
		model->setItem(i, 2, new QStandardItem(QString("%1").arg(m_pScheduler->m_pSystemParameter->teachpoint[i].pos[1])));
		model->setItem(i, 3, new QStandardItem(QString("%1").arg(m_pScheduler->m_pSystemParameter->teachpoint[i].pos[2]))); 
		model->setItem(i, 4, new QStandardItem(QString("%1").arg(m_pScheduler->m_pSystemParameter->teachpoint[i].pos[3]))); 
		model->setItem(i, 5, new QStandardItem(QString("%1").arg(m_pScheduler->m_pSystemParameter->teachpoint[i].pos[4]))); 
		model->setItem(i, 6, new QStandardItem(QString("%1").arg(m_pScheduler->m_pSystemParameter->teachpoint[i].pos[5])));
        model->setItem(i, 7, new QStandardItem(QString("%1").arg(m_pScheduler->m_pSystemParameter->teachpoint[i].pos[6])));
		//model->setItem(i, 7, new QStandardItem(QString("%1").arg(m_pScheduler->m_pSystemParameter->teachpoint[i].toolindex)));
		//model->setItem(i, 8, new QStandardItem(QString("%1").arg(m_pScheduler->m_pSystemParameter->teachpoint[i].hand)));
		//model->setItem(i, 9, new QStandardItem(QString("%1").arg(m_pScheduler->m_pSystemParameter->teachpoint[i].zone)));
		
        model->setItem(i, 8, new QStandardItem(QString::fromLocal8Bit(m_pScheduler->m_pSystemParameter->teachpointtext[i])));
	}
	ui->teachpointtable->setModel(model);

}

void cteachpointui::updataData()
{
	 int progress=0;
    QString str[4];
    int i;
     bool bNet;
	 
	/*ui->x->setNum(m_pScheduler->m_pSystemParameter->coor_car_pos[0]);
	 ui->y->setNum(m_pScheduler->m_pSystemParameter->coor_car_pos[1]);
	 ui->z->setNum(m_pScheduler->m_pSystemParameter->coor_car_pos[2]);
	 ui->r->setNum(m_pScheduler->m_pSystemParameter->coor_car_pos[3]);

	 ui->A->setNum(m_pScheduler->m_pSystemParameter->coor_joint_pos[0]);
	 ui->B->setNum(m_pScheduler->m_pSystemParameter->coor_joint_pos[1]);
	 ui->C->setNum(m_pScheduler->m_pSystemParameter->coor_joint_pos[2]);
	 ui->D->setNum(m_pScheduler->m_pSystemParameter->coor_joint_pos[3]);*/

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

	 if(m_pScheduler->m_pSystemParameter->teachreadfinished)
	 {
	 	//if(m_idisplaycnt<TEACHPIONTNUM)
	 	{
		       for(int i=0;i<TEACHPIONTNUM;i++)
			{
				model->setItem(i, 0, new QStandardItem(QString("%1").arg(m_pScheduler->m_pSystemParameter->teachpoint[i].index))); 
				model->setItem(i, 1, new QStandardItem(QString("%1").arg(m_pScheduler->m_pSystemParameter->teachpoint[i].pos[0])));
				model->setItem(i, 2, new QStandardItem(QString("%1").arg(m_pScheduler->m_pSystemParameter->teachpoint[i].pos[1])));
				model->setItem(i, 3, new QStandardItem(QString("%1").arg(m_pScheduler->m_pSystemParameter->teachpoint[i].pos[2]))); 
				model->setItem(i, 4, new QStandardItem(QString("%1").arg(m_pScheduler->m_pSystemParameter->teachpoint[i].pos[3]))); 
				model->setItem(i, 5, new QStandardItem(QString("%1").arg(m_pScheduler->m_pSystemParameter->teachpoint[i].pos[4]))); 
                model->setItem(i, 6, new QStandardItem(QString("%1").arg(m_pScheduler->m_pSystemParameter->teachpoint[i].pos[5])));
                model->setItem(i, 7, new QStandardItem(QString("%1").arg(m_pScheduler->m_pSystemParameter->teachpoint[i].pos[6])));
				//model->setItem(i, 7, new QStandardItem(QString("%1").arg(m_pScheduler->m_pSystemParameter->teachpoint[i].toolindex)));
				//model->setItem(i, 8, new QStandardItem(QString("%1").arg(m_pScheduler->m_pSystemParameter->teachpoint[i].hand)));
				//model->setItem(i, 7, new QStandardItem(m_pScheduler->m_pSystemParameter->teachpointtext[i].toString()));
                model->setItem(i, 8, new QStandardItem(QString::fromLocal8Bit(m_pScheduler->m_pSystemParameter->teachpointtext[i])));
			}
			ui->teachpointtable->setModel(model);
			//m_idisplaycnt+=256;
	 	}
		m_pScheduler->m_pSystemParameter->teachreadfinished=0;
	 }

	 if(m_Saving){         
         progress = m_pScheduler->m_pSystemParameter->TeachSaveCnt; // ��ȡ����
         if(m_pDialog != NULL){
             m_pDialog->UpdateProgressBar(progress);      
             bNet = m_pScheduler->NetIsConnect();
             if((progress>=100) || !bNet){  //      
                 progress = 0;
                 m_Saving = false;
                 m_pScheduler->m_pSystemParameter->TeachSaveCnt = 0;

                 delete m_pDialog;
                 m_pDialog = NULL;
                 if(!bNet)
		  {                     
                     QMessageBox::information(this,QString::fromLocal8Bit("����δ����"),QString::fromLocal8Bit("����ʧ��!"));
                 }
		  else
		  {                    
                     QMessageBox::information(this,QString::fromLocal8Bit("�ɹ�"),QString::fromLocal8Bit("�������!"));
                 }
             }
         }
    }
}

void cteachpointui::on_openteachpoint_clicked()
{
    if(!m_pScheduler->NetIsConnect()){
        QMessageBox::information(this,QString::fromLocal8Bit("����δ����"),QString::fromLocal8Bit("û����ȡ!"));
        return;
    }
      m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/getteachpoint", "all");
      /*for(int i=0;i<TEACHPIONTNUM;i++)
	{
		model->setItem(i, 0, new QStandardItem(QString("%1").arg(m_pScheduler->m_pSystemParameter->teachpoint[i].index))); 
		model->setItem(i, 1, new QStandardItem(QString("%1").arg(m_pScheduler->m_pSystemParameter->teachpoint[i].pos[0])));
		model->setItem(i, 2, new QStandardItem(QString("%1").arg(m_pScheduler->m_pSystemParameter->teachpoint[i].pos[1])));
		model->setItem(i, 3, new QStandardItem(QString("%1").arg(m_pScheduler->m_pSystemParameter->teachpoint[i].pos[2]))); 
		model->setItem(i, 4, new QStandardItem(QString("%1").arg(m_pScheduler->m_pSystemParameter->teachpoint[i].pos[3]))); 
		model->setItem(i, 5, new QStandardItem(QString("%1").arg(m_pScheduler->m_pSystemParameter->teachpoint[i].toolindex)));
		model->setItem(i, 6, new QStandardItem(QString("%1").arg(m_pScheduler->m_pSystemParameter->teachpoint[i].hand)));
		//model->setItem(i, 7, new QStandardItem(m_pScheduler->m_pSystemParameter->teachpointtext[i].toString()));
		model->setItem(i, 7, new QStandardItem(QString::fromLocal8Bit(m_pScheduler->m_pSystemParameter->teachpointtext[i])));
	}
	ui->teachpointtable->setModel(model);*/
}

void cteachpointui::on_addteachpoint_clicked()
{
       int curRow=ui->teachpointtable->currentIndex().row();//ѡ����

	  //for(int i=0;i<TEACHPIONTNUM;i++)
	{
		model->setItem(curRow, 0, new QStandardItem(QString("%1").arg(curRow+1))); 
		model->setItem(curRow, 1, new QStandardItem(QString("%1").arg(m_pScheduler->m_pSystemParameter->coor_car_pos[0])));
		model->setItem(curRow, 2, new QStandardItem(QString("%1").arg(m_pScheduler->m_pSystemParameter->coor_car_pos[1])));
		model->setItem(curRow, 3, new QStandardItem(QString("%1").arg(m_pScheduler->m_pSystemParameter->coor_car_pos[2]))); 
		model->setItem(curRow, 4, new QStandardItem(QString("%1").arg(m_pScheduler->m_pSystemParameter->coor_car_pos[3]))); 
		model->setItem(curRow, 5, new QStandardItem(QString("%1").arg(m_pScheduler->m_pSystemParameter->coor_car_pos[4]))); 
        model->setItem(curRow, 6, new QStandardItem(QString("%1").arg(m_pScheduler->m_pSystemParameter->coor_car_pos[5])));
        model->setItem(curRow, 7, new QStandardItem(QString("%1").arg(m_pScheduler->m_pSystemParameter->coor_car_pos[6])));
		//model->setItem(curRow, 7, new QStandardItem(QString("%1").arg(m_pScheduler->m_pSystemParameter->m_currentTool)));
               
               {
		     //model->setItem(curRow,8, new QStandardItem(QString("%1").arg(0)));
               }
            model->setItem(curRow, 8, new QStandardItem(QString("")));
	}
	ui->teachpointtable->setModel(model);
}

void cteachpointui::on_saveteachpoint_clicked()
{
    int cnt=0;
	//int curRow=ui->teachpointtable->currentIndex().row();//ѡ����
       QAbstractItemModel *modessl = ui->teachpointtable->model();
       QModelIndex indextemp;// = modessl->index(curRow,i);//������һ�е�������i ����Ҫȡֵ���е��±�
         //������һ����Ԫ����ֵ��tostring()----ok
        QVariant datatemp;// = modessl->data(indextemp);

	 if(m_Saving) return;

    if(!m_pScheduler->NetIsConnect()){
        QMessageBox::information(this,QString::fromLocal8Bit("����δ����"),QString::fromLocal8Bit("�޷�����!"));
        return;
    }
	
        for(int i=0;i<TEACHPIONTNUM;i++)
	{
		datatemp = modessl->data(modessl->index(i,0));
		m_pScheduler->m_pSystemParameter->teachpoint[i].index=datatemp.toInt();
		datatemp = modessl->data(modessl->index(i,1));
		m_pScheduler->m_pSystemParameter->teachpoint[i].pos[0]=datatemp.toDouble();
		datatemp = modessl->data(modessl->index(i,2));
		m_pScheduler->m_pSystemParameter->teachpoint[i].pos[1]=datatemp.toDouble();
		datatemp = modessl->data(modessl->index(i,3));
		m_pScheduler->m_pSystemParameter->teachpoint[i].pos[2]=datatemp.toDouble();
		datatemp = modessl->data(modessl->index(i,4));
		m_pScheduler->m_pSystemParameter->teachpoint[i].pos[3]=datatemp.toDouble();
		datatemp = modessl->data(modessl->index(i,5));
		m_pScheduler->m_pSystemParameter->teachpoint[i].pos[4]=datatemp.toDouble();
		datatemp = modessl->data(modessl->index(i,6));
		m_pScheduler->m_pSystemParameter->teachpoint[i].pos[5]=datatemp.toDouble();
        datatemp = modessl->data(modessl->index(i,7));
        m_pScheduler->m_pSystemParameter->teachpoint[i].pos[6]=datatemp.toDouble();
		//datatemp = modessl->data(modessl->index(i,7));
		//m_pScheduler->m_pSystemParameter->teachpoint[i].toolindex=datatemp.toInt();
		//datatemp = modessl->data(modessl->index(i,8));
		//m_pScheduler->m_pSystemParameter->teachpoint[i].hand=datatemp.toInt();
        datatemp = modessl->data(modessl->index(i,8));
		//m_pScheduler->m_pSystemParameter->teachpointtext[i]=datatemp;
		strcpy(m_pScheduler->m_pSystemParameter->teachpointtext[i],datatemp.toString().toLocal8Bit().data());
	}
	m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/saveteachpoint", "all");

	m_pScheduler->m_pSystemParameter->TeachSaveCnt = 0;
    m_Saving = true;

    m_pDialog = new TDialog(this);
    m_pDialog->exec();
}

void cteachpointui::on_deleteteachpoint_clicked()
{
	int curRow=ui->teachpointtable->currentIndex().row();//ѡ����
	{
		model->setItem(curRow, 0, new QStandardItem(QString("%1").arg(0))); 
		model->setItem(curRow, 1, new QStandardItem(QString("%1").arg(0)));
		model->setItem(curRow, 2, new QStandardItem(QString("%1").arg(0)));
		model->setItem(curRow, 3, new QStandardItem(QString("%1").arg(0))); 
		model->setItem(curRow, 4, new QStandardItem(QString("%1").arg(0))); 
		model->setItem(curRow, 5, new QStandardItem(QString("%1").arg(0)));
		model->setItem(curRow, 6, new QStandardItem(QString("%1").arg(0)));
        model->setItem(curRow, 7, new QStandardItem(QString("%1").arg(0)));
		//model->setItem(curRow, 7, new QStandardItem(QString("%1").arg(0)));
		//model->setItem(curRow, 8, new QStandardItem(QString("%1").arg(0)));
        model->setItem(curRow, 8, new QStandardItem(QString("")));
	}
	ui->teachpointtable->setModel(model);
        //if((curRow>=0)&&(curRow<256))model->removeRow(curRow);//�Ƴ���0������
}

void cteachpointui::on_getteachpoint_clicked()
{
	int curRow=ui->teachpointtable->currentIndex().row();//ѡ����

	  //for(int i=0;i<TEACHPIONTNUM;i++)
	{
		model->setItem(curRow, 0, new QStandardItem(QString("%1").arg(curRow+1))); 
		model->setItem(curRow, 1, new QStandardItem(QString("%1").arg(m_pScheduler->m_pSystemParameter->coor_car_pos[0])));
		model->setItem(curRow, 2, new QStandardItem(QString("%1").arg(m_pScheduler->m_pSystemParameter->coor_car_pos[1])));
		model->setItem(curRow, 3, new QStandardItem(QString("%1").arg(m_pScheduler->m_pSystemParameter->coor_car_pos[2]))); 
		model->setItem(curRow, 4, new QStandardItem(QString("%1").arg(m_pScheduler->m_pSystemParameter->coor_car_pos[3]))); 
		model->setItem(curRow, 5, new QStandardItem(QString("%1").arg(m_pScheduler->m_pSystemParameter->coor_car_pos[4]))); 
        model->setItem(curRow, 6, new QStandardItem(QString("%1").arg(m_pScheduler->m_pSystemParameter->coor_car_pos[5])));
        model->setItem(curRow, 7, new QStandardItem(QString("%1").arg(m_pScheduler->m_pSystemParameter->coor_car_pos[6])));
		//model->setItem(curRow, 7, new QStandardItem(QString("%1").arg(m_pScheduler->m_pSystemParameter->m_currentTool)));
               
               {
		     //model->setItem(curRow,8, new QStandardItem(QString("%1").arg(0)));
               }
            model->setItem(curRow, 8, new QStandardItem(QString("")));
	}
	ui->teachpointtable->setModel(model);
}

void cteachpointui::stop_clicked()
{	
	(m_pScheduler->m_pSystemParameter->sys_ctrl.sendinput[0])|=0x2;	
}
void cteachpointui::on_movetopoint_pressed()
{
	int curRow=ui->teachpointtable->currentIndex().row();//ѡ����
	if((curRow<0)||(curRow>=TEACHPIONTNUM))
	{
		QMessageBox::information(NULL, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("��ѡ��ʾ�̵㣡"));
	        qDebug()<<"��ǰʾ�̵�"<<curRow;
		return ;
	}
	QAbstractItemModel *modessl = ui->teachpointtable->model();
       QModelIndex indextemp;// = modessl->index(curRow,i);//������һ�е�������i ����Ҫȡֵ���е��±�
         //������һ����Ԫ����ֵ��tostring()----ok
        QVariant datatemp;// = modessl->data(indextemp);
       // for(int i=0;i<TEACHPIONTNUM;i++)
       int i=curRow;
	{
		datatemp = modessl->data(modessl->index(i,0));
		m_pScheduler->m_pSystemParameter->teachpoint[i].index=datatemp.toInt();
		 if((m_pScheduler->m_pSystemParameter->teachpoint[i].index<1)
					  ||(m_pScheduler->m_pSystemParameter->teachpoint[i].index>1024))	  
		{
		        QMessageBox::information(NULL, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("��������ʾ�̵㣡"));
		        qDebug()<<"ʾ�̵�����"<<m_pScheduler->m_pSystemParameter->teachpoint[i].index;
			return ;
		}
		datatemp = modessl->data(modessl->index(i,1));
		m_pScheduler->m_pSystemParameter->teachpoint[i].pos[0]=datatemp.toDouble();
		datatemp = modessl->data(modessl->index(i,2));
		m_pScheduler->m_pSystemParameter->teachpoint[i].pos[1]=datatemp.toDouble();
		datatemp = modessl->data(modessl->index(i,3));
		m_pScheduler->m_pSystemParameter->teachpoint[i].pos[2]=datatemp.toDouble();
		datatemp = modessl->data(modessl->index(i,4));
		m_pScheduler->m_pSystemParameter->teachpoint[i].pos[3]=datatemp.toDouble();
		datatemp = modessl->data(modessl->index(i,5));
		m_pScheduler->m_pSystemParameter->teachpoint[i].pos[4]=datatemp.toDouble();
		datatemp = modessl->data(modessl->index(i,6));
		m_pScheduler->m_pSystemParameter->teachpoint[i].pos[5]=datatemp.toDouble();
        datatemp = modessl->data(modessl->index(i,7));
        m_pScheduler->m_pSystemParameter->teachpoint[i].pos[6]=datatemp.toDouble();
		//datatemp = modessl->data(modessl->index(i,7));
		//m_pScheduler->m_pSystemParameter->teachpoint[i].toolindex=datatemp.toInt();
		//datatemp = modessl->data(modessl->index(i,8));
		//m_pScheduler->m_pSystemParameter->teachpoint[i].hand=datatemp.toInt();
//		datatemp = modessl->data(modessl->index(i,8));
//		m_pScheduler->m_pSystemParameter->teachpoint[i].zone=datatemp.toInt();
        datatemp = modessl->data(modessl->index(i,8));
		//m_pScheduler->m_pSystemParameter->teachpointtext[i]=datatemp;
		strcpy(m_pScheduler->m_pSystemParameter->teachpointtext[i],datatemp.toString().toLocal8Bit().data());
	}

	m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/movetopoint", QString::number(curRow));
}

void cteachpointui::on_movetopoint_released()
{
      //stop_clicked();
}

void cteachpointui::on_openuserfile_clicked()
{
	 QFileDialog *fileDialog = new QFileDialog();
	if(fileDialog!=NULL)
	{
	    fileDialog->setWindowTitle(tr("Open user-app.bin"));
	    fileDialog->setDirectory("../NGfile");
	    
         fileDialog->setNameFilter(tr("Binrary Files(*.bin)"));
	    if(fileDialog->exec() == QDialog::Accepted)
	    {
                ui->userfilename->setText(fileDialog->selectedFiles()[0]);
	        m_pScheduler->m_pSystemParameter->m_userfilePath=QString(fileDialog->selectedFiles()[0]);
	        
	        //m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "draw background");
	    }
	    else
	    {
	        QMessageBox::information(NULL, tr("Path"), tr("You didn't select any files."));
	    }
	}	
    delete(fileDialog);
	fileDialog=NULL;
}

void cteachpointui::on_downuserfile_clicked()
{
	m_pScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/downuserfile");
}
