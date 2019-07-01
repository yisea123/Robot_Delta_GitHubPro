
/************************************************************
  Copyright (C),
  �ļ�����ParseGCode.cpp
  ���ߣ�zhou    �汾��V1.0    �������ڣ�2018/07/10
  ģ��������ParseGCode����ʵ��
  ��ʷ�޸ļ�¼��<author>    <time>    <version >    <desc>
                  zhou    2018/07/10     V1.0      ����ע��
*************************************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ComonTypes.h"

#include "SystemSchedule.h"

#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QTimer>
#include "GlobalVariableType.h"
#include "ccommmomuiwidget.h"


MainWindow::MainWindow(UserParameter  parm, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    nCurrentPage(-1)
{
    ui->setupUi(this);

    SystemScheduler = new SystemSchedule(this, parm);
    movemode=0;
    movecoor=0;
    cyclerun=0;
    enable=0;
    mode=0;
    ui->start->setEnabled(0);
    ui->cyclerun->setEnabled(0);

    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()),
            this, SLOT(updataData()));
    timer->start(100);
	
//    setWindowState(Qt::WindowMaximized);
    
}

MainWindow::~MainWindow()
{
    // ɾ���˵�����
    foreach (QVector<int>* menu, menus) {
        delete menu;
    }

    delete ui;
}

void MainWindow::createDisplayUIs(QList<InfoUI *> &widgets)
{
    // �������˵�
    setupMainMenuGroup();

    // Ϊ�������˵������Ӳ˵�
    createMenuList();

//        // �����Ӳ˵���ui
//        connectMenuWithUi(widgets);

    setDefaultMenuSelection();

    //createHomeMenu();
    createStatusBar();
//    createHelpText();
}

void MainWindow::goToUiPage(int page)
{
    Q_ASSERT(UI_PAGE_NUMBER > page);

    nCurrentPage = page;

    setCurrentUiPage();
}



void MainWindow::setCurrentUiPage()
{
    if(-1 == nCurrentPage)
        return;

    int rowId = listWidgets.at(nCurrentPage)->currentRow();
    if(-1 == rowId)
    {
        if(listWidgets.at(nCurrentPage)->count() > 0)
        {
            listWidgets.at(nCurrentPage)->setCurrentRow(0);
            rowId = 0;
        } else {
            return;
        }
    }
    int id = menus.at(nCurrentPage)->at(rowId);
    ui->uis->setCurrentIndex(id);

    ui->menuList->setCurrentIndex(nCurrentPage);
}

void MainWindow::currentMenuChanged(int row)
{
    setCurrentUiPage();
}

void MainWindow::setupMainMenuGroup()
{
    pMainMenuGroup = new QButtonGroup(this);
    pMainMenuGroup->addButton(ui->program, UI_PROGRAM_ID);
    pMainMenuGroup->addButton(ui->state, UI_STATE_ID);
    pMainMenuGroup->addButton(ui->setting, UI_SETTING_ID);
    pMainMenuGroup->addButton(ui->warning, UI_WARNING_ID);

    //exclusive : bool
    //If this property is true then only one button in the group can be checked at any given time.
    //By default, this property is true.
    pMainMenuGroup->setExclusive(true);

    connect(pMainMenuGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(goToUiPage(int)));

    nCurrentPage = UI_PROGRAM_ID;
    ui->program->setChecked(true);
}

void MainWindow::createMenuList()
{
//    for(int i = 0; i < UI_PAGE_NUMBER; i++)
//    {
//        QWidget *widget = new QWidget(ui->menuList);
//        widget->setGeometry(0,0,623,35);
//        QButtonGroup *pButtonGroup = new QButtonGroup(widget);
//        ui->menuList->addWidget(widget);
//        sonMenuGrp.push_back(pButtonGroup);
//        sonMenuLastID.push_back(0);

//        connect(pButtonGroup, SIGNAL(buttonClicked(int)),this, SLOT(setCurrentUiPage(int)));
//        //menus.push_back(new QVector<int>);
//    }
    for(int i = 0; i < UI_PAGE_NUMBER; i++)
    {
        QListWidget *pListWidget = new QListWidget(ui->menuList);
        pListWidget->setFrameShape(QFrame::Box);
        pListWidget->setStyleSheet("QListWidget{"
                                   "background-color:rgb(240, 240, 240);}"
//                                   "QListWidget::item:unselected{"
//                                   "border-image: url(:/images/itemunselected.png);}"
//                                   "QListWidget::item:selected{"
//                                   "border-image: url(:/images/itemselected.png);}"
                                   );

        //pListWidget->setFont(QFont("����", 22, QFont::Normal));//Courier
        pListWidget->setFont(QFont("Microsoft YaHei", 12, QFont::Normal));

        /*pListWidget->setViewMode(QListView::IconMode);
        pListWidget->setMovement(QListView::Static);
        pListWidget->setWrapping(false);
        pListWidget->setFlow(QListView::LeftToRight);*/
        pListWidget->setViewMode(QListView::ListMode);
        pListWidget->setFlow(QListView::LeftToRight);
        ui->menuList->addWidget(pListWidget);
        listWidgets.push_back(pListWidget);

        connect(pListWidget, SIGNAL(currentRowChanged(int)),
                this, SLOT(currentMenuChanged(int)));

        menus.push_back(new QVector<int>);
    }
}

void MainWindow::connectMenuWithUi(QList<InfoUI *> &widgets)
{
    for(int i = 0; i < ui->uis->count(); i++)
    {
        ui->uis->removeWidget(ui->uis->widget(i));
    }

    for(int i = 0; i < menus.count(); i++)
    {
        menus.at(i)->clear();
    }
    for(int i = 0; i < listWidgets.count(); i++)
    {
        listWidgets.at(i)->clear();
    }


    for(int i = 0; i < widgets.count(); i++)
    {
        if(widgets.at(i)->bShow)
        {
            widgets.at(i)->pUi->setHidden(false);
            ui->uis->addWidget(widgets.at(i)->pUi); // ��ui�ŵ������ں��ʵ�λ��
            widgets.at(i)->pParentUi = ui->uis; // ��������ui��parent

            // ������Ӧ�Ĳ˵���
            addMenuText(widgets.at(i)->nType, widgets.at(i)->sName);

            // �󶨲˵�id��ui��id
            menus.at(widgets.at(i)->nType)->push_back(ui->uis->count()-1);
        } 
	 else 
	 {
            widgets.at(i)->pUi->setHidden(true);
        }
    }

    for(int i = 0; i < listWidgets.count();i++)
    {
        int count = listWidgets.at(i)->count();
        for(int j = 0; j < count; j++)
        {
         QListWidgetItem *item = listWidgets.at(i)->item(j);
         //item->setSizeHint(QSize(item->sizeHint().width(), 50));
         item->setSizeHint(QSize(80, 30));
        }
    }

    for(int i = 0; i < listWidgets.count(); i++)
    {
        if(listWidgets.at(i)->count() < 1)
        {
            pMainMenuGroup->button(i)->setDisabled(true);
        }
    }
}

void MainWindow::addMenuText(int nType, QString name)
{
    QListWidgetItem *item = new QListWidgetItem();
    listWidgets.at(nType)->addItem(item);

    QLabel *text = new QLabel("    " + name, listWidgets.at(nType));
    text->setFont(QFont("����", 12, QFont::Bold));
    //text->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    text->setAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    text->setFocusPolicy(Qt::NoFocus);

    listWidgets.at(nType)->setItemWidget(item, text);
}

void MainWindow::setDefaultMenuSelection()
{
    foreach (QListWidget *w, listWidgets) {
        if(w->count() > 0)
        {
            w->setItemSelected(w->item(0), true);
        }
    }
}





void MainWindow::createStatusBar()
{
    messageLabel = new QLabel(this);
    messageLabel->setFont(QFont("����", 13, QFont::Normal));
    messageLabel->setAlignment(Qt::AlignLeft);

    statusBar()->addWidget(messageLabel);
    statusBar()->setFont(QFont("����", 13, QFont::Normal));
    statusBar()->setFixedHeight(40);
    statusBar()->setStyleSheet("QStatusBar{border: 1px solid rgb(204, 204, 204);}"
                               "QStatusBar::item{border: 0px;}");

}

void MainWindow::writeMsgToStatusBar(QString msg, int level)
{
    messageLabel->setText(msg);
    //messageLabel->setStyleSheet("color: red;");
    //return;

    //����������ɫ
    //QPalette pa;
    switch (level)
    {
    case STATUSBAR_ERROR_INFO://��������
        //pa.setColor(QPalette::WindowText, Qt::red);
        messageLabel->setStyleSheet("color: red;");
        break;
    case STATUSBAR_WARNING_INFO://����
        //pa.setColor(QPalette::WindowText, Qt::darkRed);
        messageLabel->setStyleSheet("color: darkRed;");
        break;
    case STATUSBAR_STATUS_ON_INFO://״̬�л������֣�
        //pa.setColor(QPalette::WindowText, Qt::darkYellow);
        messageLabel->setStyleSheet("color: blue;");
        break;
    case STATUSBAR_STATUS_OFF_INFO://״̬�л����رգ�
        //pa.setColor(QPalette::WindowText, Qt::darkGreen);
        messageLabel->setStyleSheet("color: darkGreen;");
        break;
    default://��ʾ��Ϣ/������¼
        //pa.setColor(QPalette::WindowText, Qt::black);
        messageLabel->setStyleSheet("color: black;");
        break;
    }
    //messageLabel->setPalette(pa);
}

void MainWindow::writeMsgToStatusBar(QString msg)
{
    messageLabel->setText(msg);
}

void MainWindow::showDisplayUIs(QList<InfoUI *> &widgets, UserParameter parm)
{
    switch (parm.m_permission)
    {
    case ORDINARY_USER://������
        for(int i = 0; i < widgets.count(); i++)
        {
            if(widgets.at(i)->sName == "ϵͳ��������"
                    || widgets.at(i)->sName == "�˶�����������"
                    || widgets.at(i)->sName == "���ղ�������"
                    || widgets.at(i)->sName == "ģ�߿���������"
                    || widgets.at(i)->sName == "�ⲿ�豸ͨ������")
            {
                widgets.at(i)->bShow = false;
            }
            else
            {
                widgets.at(i)->bShow = true;
                ((CCommmomUIWidget *)(widgets.at(i)->pUi))->setDisableEdit(true);
            }
        }
        break;
    case ADVANCED_USER://����Ա
        for(int i = 0; i < widgets.count(); i++)
        {
            if(widgets.at(i)->sName == "���ղ�������")
            {
                widgets.at(i)->bShow = false;
            }
            else
            {
                widgets.at(i)->bShow = true;
                ((CCommmomUIWidget *)(widgets.at(i)->pUi))->setDisableEdit(false);
            }
        }
        break;
    case SUPER_USER://������
        for(int i = 0; i < widgets.count(); i++)
        {
            widgets.at(i)->bShow = true;
            ((CCommmomUIWidget *)(widgets.at(i)->pUi))->setDisableEdit(false);
        }
        break;
    }

    // �����Ӳ˵���ui
    connectMenuWithUi(widgets);
}

void MainWindow::stop_clicked()
{
	//SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/STOP", "yes");
	(SystemScheduler->m_pSystemParameter->sys_ctrl.sendinput[0])|=0x2;
	//SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "cycle run", "single");
	cyclerun=0;
	//ui->cyclerun->setEnabled(1);
}
void MainWindow::on_enable_clicked(bool checked)
{
    //if (checked)
     if((SystemScheduler->m_pSystemParameter->sys_ctrl.statebit&0x1)==0)
    {
        SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/servoEnable", "yes");
	/* ui->enable->setText(QString::fromLocal8Bit("ʹ��"));
	 ui->enable->setChecked(1);
	 ui->start->setEnabled(1);
	 ui->cyclerun->setEnabled(1);*/
    }
    else
    {
        SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/servoEnable", "no");
	/*ui->enable->setText(QString::fromLocal8Bit("��ʹ��"));
	ui->enable->setChecked(0);
	ui->start->setEnabled(0);
	 ui->cyclerun->setEnabled(0);*/
    }
  // SystemScheduler->m_pSystemParameter->m_sysctrl.servoenable=checked;
}

void MainWindow::on_mode_clicked(bool checked)
{
     if((SystemScheduler->m_pSystemParameter->sys_ctrl.statebit&0x1)==1)
     {
	    // if (checked)
	   if((0x40==(SystemScheduler->m_pSystemParameter->sys_ctrl.statebit&0x60))||(0x0==(SystemScheduler->m_pSystemParameter->sys_ctrl.statebit&0x60)))
	 {
	 	 SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/mode", "1");
	 }
	else
	    {
	        SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/mode", "2");
		 //ui->mode->setText(QString::fromLocal8Bit("�Զ�"));
	    }
		
	   /* if((0x80000==(SystemScheduler->m_pSystemParameter->sys_ctrl.statebit&0xc0000))||(0x0==(SystemScheduler->m_pSystemParameter->sys_ctrl.statebit&0xc0000)))
	    {
	        SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/mode", "1");
		 //ui->mode->setText(QString::fromLocal8Bit("�ֶ�"));
	    }
	    else
	    {
	        SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/mode", "2");
		 //ui->mode->setText(QString::fromLocal8Bit("�Զ�"));
	    }*/
     }
     else
     {
     		SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/mode", "0");
		//ui->mode->setText(QString::fromLocal8Bit("ģʽ"));
     }

     /*if((SystemScheduler->m_pSystemParameter->sys_ctrl.statebit&0x1)==1)
     {
	    // if (checked)
	    if((0x40==(SystemScheduler->m_pSystemParameter->sys_ctrl.statebit&0x60))||(0x0==(SystemScheduler->m_pSystemParameter->sys_ctrl.statebit&0x60)))
	    {
	        SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/mode", "1");
		 //ui->mode->setText(QString::fromLocal8Bit("�ֶ�"));
	    }
	    else
	    {
	        SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/mode", "2");
		 //ui->mode->setText(QString::fromLocal8Bit("�Զ�"));
	    }
     }
     else
     {
     		SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/mode", "0");
		//ui->mode->setText(QString::fromLocal8Bit("ģʽ"));
     }*/
    
}

void MainWindow::on_axis1pos_pressed()
{
     //SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/xPMove start");
     (SystemScheduler->m_pSystemParameter->sys_ctrl.sendinput[0])&=~0x2;//clear stopbit
     if(movemode)//����
     {
     	     double moveLen = ui->moveLength->value();
            if(!movecoor)
            {
		     
		     if (fabs(moveLen) > 0.0001)
		     {
		         SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/xPMotion", QString::number(moveLen));
		     }
            }
	     else
	     {
	     		SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/xPMotion_world", QString::number(moveLen));
	     }
     }	
     else//����
     {
     	     if(!movecoor)
            {
		
		         SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/xPMove", "start");
            }
	     else
	     {
	     		SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/xPMove_world", "start");
	     }
     }
}

void MainWindow::on_axis1neg_pressed()
{
      (SystemScheduler->m_pSystemParameter->sys_ctrl.sendinput[0])&=~0x2;//clear stopbit
      if(movemode)//����
     {
     	     double moveLen = ui->moveLength->value();
            if(!movecoor)
            {
		     
		     if (fabs(moveLen) > 0.0001)
		     {
		         SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/xNMotion", QString::number(moveLen));
		     }
            }
	     else
	     {
	     		SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/xNMotion_world", QString::number(moveLen));
	     }
     }	
     else//����
     {
     	     if(!movecoor)
            {
		
		         SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/xNMove", "start");
            }
	     else
	     {
	     		SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/xNMove_world", "start");
	     }
     }
}

void MainWindow::on_axis2pos_pressed()
{
    (SystemScheduler->m_pSystemParameter->sys_ctrl.sendinput[0])&=~0x2;//clear stopbit
    if(movemode)//����
     {
     	     double moveLen = ui->moveLength->value();
            if(!movecoor)
            {
		     
		     if (fabs(moveLen) > 0.0001)
		     {
		         SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/yPMotion", QString::number(moveLen));
		     }
            }
	     else
	     {
	     		SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/yPMotion_world", QString::number(moveLen));
	     }
     }	
     else//����
     {
     	     if(!movecoor)
            {
		
		         SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/yPMove", "start");
            }
	     else
	     {
	     		SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/yPMove_world", "start");
	     }
     }
}

void MainWindow::on_axis2neg_pressed()
{
    (SystemScheduler->m_pSystemParameter->sys_ctrl.sendinput[0])&=~0x2;//clear stopbit
    if(movemode)//����
     {
     	     double moveLen = ui->moveLength->value();
            if(!movecoor)
            {
		     
		     if (fabs(moveLen) > 0.0001)
		     {
		         SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/yNMotion", QString::number(moveLen));
		     }
            }
	     else
	     {
	     		SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/yNMotion_world", QString::number(moveLen));
	     }
     }	
     else//����
     {
     	     if(!movecoor)
            {
		
		         SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/yNMove", "start");
            }
	     else
	     {
	     		SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/yNMove_world", "start");
	     }
     }
}

void MainWindow::on_axis3pos_pressed()
{
     (SystemScheduler->m_pSystemParameter->sys_ctrl.sendinput[0])&=~0x2;//clear stopbit
     if(movemode)//����
     {
     	     double moveLen = ui->moveLength->value();
            if(!movecoor)
            {
		     
		     if (fabs(moveLen) > 0.0001)
		     {
		         SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/zPMotion", QString::number(moveLen));
		     }
            }
	     else
	     {
	     		SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/zPMotion_world", QString::number(moveLen));
	     }
     }	
     else//����
     {
     	     if(!movecoor)
            {
		
		         SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/zPMove", "start");
            }
	     else
	     {
	     		SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/zPMove_world", "start");
	     }
     }
}

void MainWindow::on_axis3neg_pressed()
{
     (SystemScheduler->m_pSystemParameter->sys_ctrl.sendinput[0])&=~0x2;//clear stopbit
      if(movemode)//����
     {
     	     double moveLen = ui->moveLength->value();
            if(!movecoor)
            {
		     
		     if (fabs(moveLen) > 0.0001)
		     {
		         SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/zNMotion", QString::number(moveLen));
		     }
            }
	     else
	     {
	     		SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/zNMotion_world", QString::number(moveLen));
	     }
     }	
     else//����
     {
     	     if(!movecoor)
            {
		
		         SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/zNMove", "start");
            }
	     else
	     {
	     		SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/zNMove_world", "start");
	     }
     }
}

void MainWindow::on_axis4pos_pressed()
{
     (SystemScheduler->m_pSystemParameter->sys_ctrl.sendinput[0])&=~0x2;
    if(movemode)//����
     {
     	     double moveLen = ui->moveLength->value();
            if(!movecoor)
            {
		     
		     if (fabs(moveLen) > 0.0001)
		     {
		         SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/wPMotion", QString::number(moveLen));
		     }
            }
	     else
	     {
	     		SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/wPMotion_world", QString::number(moveLen));
	     }
     }	
     else//����
     {
     	     if(!movecoor)
            {
		
		         SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/wPMove", "start");
            }
	     else
	     {
	     		SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/wPMove_world", "start");
	     }
     }
}

void MainWindow::on_axis4neg_pressed()
{
	 (SystemScheduler->m_pSystemParameter->sys_ctrl.sendinput[0])&=~0x2;
    if(movemode)//����
     {
     	     double moveLen = ui->moveLength->value();
            if(!movecoor)
            {
		     
		     if (fabs(moveLen) > 0.0001)
		     {
		         SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/wNMotion", QString::number(moveLen));
		     }
            }
	     else
	     {
	     		SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/wNMotion_world", QString::number(moveLen));
	     }
     }	
     else//����
     {
     	     if(!movecoor)
            {
		
		         SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/wNMove", "start");
            }
	     else
	     {
	     		SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/wNMove_world", "start");
	     }
     }
}

void MainWindow::on_reset_clicked()
{
    SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/reset", "yes");
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    //QString tempvel;

	SystemScheduler->m_pSystemParameter->sysvel=value;
     ui->vel->setNum(value);
    //tempvel.number(value,10);
    //ui->vel->setText(tempvel);
    (SystemScheduler->m_pSystemParameter->sys_ctrl.sendinput[1])=value;

}

void MainWindow::on_clear_clicked()
{
    //(SystemScheduler->m_pSystemParameter->sys_ctrl.sendinput[0])|=0x2;
    SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/clear", "yes");
}

void MainWindow::on_setorian_clicked()
{
    SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/setorian", "yes");
}

void MainWindow::on_start_clicked()
{
	qDebug()<<"motion start";
    (SystemScheduler->m_pSystemParameter->sys_ctrl.sendinput[0])&=~0x2;
    SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "start motion");
    //ui->cyclerun->setEnabled(0);
}

void MainWindow::on_cyclerun_clicked(bool checked)
{
     
    //if (checked)
    if(!cyclerun)
   {
   	
   	(SystemScheduler->m_pSystemParameter->sys_ctrl.sendinput[0])&=~0x2;
	ui->cyclerun->setText(QString::fromLocal8Bit("ѭ���ӹ�"));
       SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "cycle run", "cycle");
	cyclerun=1;
    //SystemScheduler->m_pSystemParameter->m_sysctrl.ctrlmode=1;
   }
   else
   {
       //(SystemScheduler->m_pSystemParameter->sys_ctrl.sendinput[0])|=0x2;
       ui->cyclerun->setText(QString::fromLocal8Bit("ѭ��ȡ��"));
       SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "cycle run", "single");
       cyclerun=0;
    //SystemScheduler->m_pSystemParameter->m_sysctrl.ctrlmode=2;
   }
}

void MainWindow::on_movpos_clicked(bool checked)
{
   if (checked)
   {
       movemode=1;
   }
   else
   {
       movemode=0;
   }
}

void MainWindow::on_axis1pos_released()
{
      if(movemode)//����
     {
     	     
     }	
     else//����
     {
     	     if(!movecoor)
            {
		
		         SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/xPMove", "stop");
            }
	     else
	     {
	     		SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/xPMove_world", "stop");
	     }
	     stop_clicked();
     }
}

void MainWindow::on_axis1neg_released()
{
       if(movemode)//����
     {
     	     
     }	
     else//����
     {
     	     if(!movecoor)
            {
		
		         SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/xNMove", "stop");
            }
	     else
	     {
	     		SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/xNMove_world", "stop");
	     }
	     stop_clicked();
     }
}

void MainWindow::on_axis2pos_released()
{
       if(movemode)//����
     {
     	     
     }	
     else//����
     {
     	     if(!movecoor)
            {
		
		         SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/yPMove", "stop");
            }
	     else
	     {
	     		SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/yPMove_world", "stop");
	     }
	     stop_clicked();
     }
}

void MainWindow::on_axis2neg_released()
{
      if(movemode)//����
     {
     	     
     }	
     else//����
     {
     	     if(!movecoor)
            {
		
		         SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/yNMove", "stop");
            }
	     else
	     {
	     		SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/yNMove_world", "stop");
	     }
	     stop_clicked();
     }
}

void MainWindow::on_axis3pos_released()
{
      if(movemode)//����
     {
     	     
     }	
     else//����
     {
     	     if(!movecoor)
            {
		
		         SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/zPMove", "stop");
            }
	     else
	     {
	     		SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/zPMove_world", "stop");
	     }
	     stop_clicked();
     }
}

void MainWindow::on_axis3neg_released()
{
     if(movemode)//����
     {
     	     
     }	
     else//����
     {
     	     if(!movecoor)
            {
		
		         SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/zNMove", "stop");
            }
	     else
	     {
	     		SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/zNMove_world", "stop");
	     }
	     stop_clicked();
     }
}

void MainWindow::on_axis4pos_released()
{
	 if(movemode)//����
     {
     	     
     }	
     else//����
     {
     	     if(!movecoor)
            {
		
		         SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/wPMove", "stop");
            }
	     else
	     {
	     		SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/wPMove_world", "stop");
	     }
	     stop_clicked();
     }
}

void MainWindow::on_axis4neg_released()
{
      if(movemode)//����
     {
     	     
     }	
     else//����
     {
     	     if(!movecoor)
            {
		
		         SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/wNMove", "stop");
            }
	     else
	     {
	     		SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/wNMove_world", "stop");
	     }
	     stop_clicked();
     }
}

void MainWindow::on_carcoor_clicked(bool checked)
{
      if (checked)
   {
       movecoor=1;
   }
   else
   {
       movecoor=0;
   }
}

void MainWindow::on_stop_clicked()
{
	//SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/STOP", "yes");
	(SystemScheduler->m_pSystemParameter->sys_ctrl.sendinput[0])|=0x2;
	SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "cycle run", "single");
	cyclerun=0;
	ui->cyclerun->setEnabled(1);
}

void MainWindow::on_hold_clicked(bool checked)
{
    if(checked)
    {
        (SystemScheduler->m_pSystemParameter->sys_ctrl.sendinput[0])|=0x1;
    }
    else
    {
       (SystemScheduler->m_pSystemParameter->sys_ctrl.sendinput[0])&=~0x1;
    }
}

void MainWindow::on_backzero_clicked()
{
     (SystemScheduler->m_pSystemParameter->sys_ctrl.sendinput[0])&=~0x2;
    SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/backzero", "yes");
}

void MainWindow::updataData()
{
    if((SystemScheduler->m_pSystemParameter->sys_ctrl.statebit&0x1)==1)
    {
        //SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/servoEnable", "yes");
	 ui->enable->setText(QString::fromLocal8Bit("ʹ��"));
	 ui->enable->setChecked(1);
	 ui->start->setEnabled(1);
	 ui->cyclerun->setEnabled(1);
    }
    else
    {
        //SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/servoEnable", "no");
	ui->enable->setText(QString::fromLocal8Bit("ʧ��"));
	ui->enable->setChecked(0);
	ui->start->setEnabled(0);
	 ui->cyclerun->setEnabled(0);
    }

    /* if(0x40000==(SystemScheduler->m_pSystemParameter->sys_ctrl.statebit&0xc0000))
    {
        //SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/mode", "1");
	 ui->mode->setText(QString::fromLocal8Bit("�ֶ�"));
    }
    else  if(0x80000==(SystemScheduler->m_pSystemParameter->sys_ctrl.statebit&0xc0000))
    {
        //SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/mode", "2");
	 ui->mode->setText(QString::fromLocal8Bit("�Զ�"));
    }
    else
    {
    	ui->mode->setText(QString::fromLocal8Bit("ģʽ"));
    }*/

      if(0x20==(SystemScheduler->m_pSystemParameter->sys_ctrl.statebit&0x60))
	 {
	 	  ui->mode->setText(QString::fromLocal8Bit("�ֶ�"));
	 }
	 else if(0x40==(SystemScheduler->m_pSystemParameter->sys_ctrl.statebit&0x60))
	  {
        //SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/mode", "2");
	 ui->mode->setText(QString::fromLocal8Bit("�Զ�"));
        }
	else
       {
    	    ui->mode->setText(QString::fromLocal8Bit("ģʽ"));
       }

    if((SystemScheduler->m_pSystemParameter->sys_ctrl.statebit&0x200)==0x200)//hold
    {        
	 ui->hold->setChecked(1);
    }
    else
    {
	ui->hold->setChecked(0);
    }

    /*if((SystemScheduler->m_pSystemParameter->sys_ctrl.statebit&0x10000)==0x10000)//stop
    {        
	 ui->stop->setChecked(1);
    }
    else
    {
	ui->stop->setChecked(0);
    }*/
}
void MainWindow::on_axis5neg_pressed()
{
	(SystemScheduler->m_pSystemParameter->sys_ctrl.sendinput[0])&=~0x2;//clear stopbit
      if(movemode)//����
     {
     	     double moveLen = ui->moveLength->value();
            if(!movecoor)
            {
		     
		     if (fabs(moveLen) > 0.0001)
		     {
		         SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/pNMotion", QString::number(moveLen));
		     }
            }
	     else
	     {
	     		SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/pNMotion_world", QString::number(moveLen));
	     }
     }	
     else//����
     {
     	     if(!movecoor)
            {
		
		         SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/pNMove", "start");
            }
	     else
	     {
	     		SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/pNMove_world", "start");
	     }
     }
}

void MainWindow::on_axis5pos_pressed()
{
     (SystemScheduler->m_pSystemParameter->sys_ctrl.sendinput[0])&=~0x2;//clear stopbit
     if(movemode)//����
     {
     	     double moveLen = ui->moveLength->value();
            if(!movecoor)
            {
		     
		     if (fabs(moveLen) > 0.0001)
		     {
		         SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/pPMotion", QString::number(moveLen));
		     }
            }
	     else
	     {
	     		SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/pPMotion_world", QString::number(moveLen));
	     }
     }	
     else//����
     {
     	     if(!movecoor)
            {
		
		         SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/pPMove", "start");
            }
	     else
	     {
	     		SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/pPMove_world", "start");
	     }
     }
}

void MainWindow::on_axis6neg_pressed()
{
	(SystemScheduler->m_pSystemParameter->sys_ctrl.sendinput[0])&=~0x2;//clear stopbit
      if(movemode)//����
     {
     	     double moveLen = ui->moveLength->value();
            if(!movecoor)
            {
		     
		     if (fabs(moveLen) > 0.0001)
		     {
		         SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/rNMotion", QString::number(moveLen));
		     }
            }
	     else
	     {
	     		SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/rNMotion_world", QString::number(moveLen));
	     }
     }	
     else//����
     {
     	     if(!movecoor)
            {
		
		         SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/rNMove", "start");
            }
	     else
	     {
	     		SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/rNMove_world", "start");
	     }
     }
}

void MainWindow::on_axis6pos_pressed()
{
	 (SystemScheduler->m_pSystemParameter->sys_ctrl.sendinput[0])&=~0x2;//clear stopbit
     if(movemode)//����
     {
     	     double moveLen = ui->moveLength->value();
            if(!movecoor)
            {
		     
		     if (fabs(moveLen) > 0.0001)
		     {
		         SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/rPMotion", QString::number(moveLen));
		     }
            }
	     else
	     {
	     		SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/rPMotion_world", QString::number(moveLen));
	     }
     }	
     else//����
     {
     	     if(!movecoor)
            {
		
		         SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/rPMove", "start");
            }
	     else
	     {
	     		SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/rPMove_world", "start");
	     }
     }
}

void MainWindow::on_axis5neg_released()
{
	  if(movemode)//����
     {
     	     
     }	
     else//����
     {
     	     if(!movecoor)
            {
		
		         SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/pNMove", "stop");
            }
	     else
	     {
	     		SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/pNMove_world", "stop");
	     }
	     stop_clicked();
     }
}

void MainWindow::on_axis5pos_released()
{
	  if(movemode)//����
     {
     	     
     }	
     else//����
     {
     	     if(!movecoor)
            {
		
		         SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/pPMove", "stop");
            }
	     else
	     {
	     		SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/pPMove_world", "stop");
	     }
	     stop_clicked();
     }

}

void MainWindow::on_axis6neg_released()
{
	  if(movemode)//����
     {
     	     
     }	
     else//����
     {
     	     if(!movecoor)
            {
		
		         SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/rNMove", "stop");
            }
	     else
	     {
	     		SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/rNMove_world", "stop");
	     }
	     stop_clicked();
     }
}

void MainWindow::on_axis6pos_released()
{
	  if(movemode)//����
     {
     	     
     }	
     else//����
     {
     	     if(!movecoor)
            {
		
		         SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/rPMove", "stop");
            }
	     else
	     {
	     		SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/rPMove_world", "stop");
	     }
	     stop_clicked();
     }
}
