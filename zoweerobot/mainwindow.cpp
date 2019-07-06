
/************************************************************
  Copyright (C),
  ï¿½Ä¼ï¿½ï¿½ï¿½ï¿½ï¿½ParseGCode.cpp
  ï¿½ï¿½ï¿½ß£ï¿½zhou    ï¿½æ±¾ï¿½ï¿½V1.0    ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ú£ï¿½2018/07/10
  Ä£ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ParseGCodeï¿½ï¿½ï¿½ï¿½Êµï¿½ï¿½
  ï¿½ï¿½Ê·ï¿½Þ¸Ä¼ï¿½Â¼ï¿½ï¿½<author>    <time>    <version >    <desc>
                  zhou    2018/07/10     V1.0      ï¿½ï¿½ï¿½ï¿½×¢ï¿½ï¿½
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

#include "MotionControllerDevice.h"
#include "MotionControllerModule.h"
#include "NetSocket.h"
#include "qtoolbutton.h"

#define btnStyleSheet "QPushButton{border: 2px groove gray;border-style: outset;}QPushButton:hover{\
                        background-color:white;color: black;}QPushButton:pressed{\
                        background-color:rgb(85, 170, 255);border-style: inset; }"

QUdpSocket *receiver=NULL;

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
    //ui->cyclerun->setEnabled(0);

    m_IconOff = QIcon(":/images/setoff.png");
    m_IconOn = QIcon(":/images/seton.png");

    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()),this, SLOT(updataData()));
    timer->start(100);

    //setWindowState(Qt::WindowMaximized);
    //resize(800, 600);
   // this->setWindowFlags(Qt::FramelessWindowHint); // 150527
    ui->warning->hide();
    ui->servoOn->setStyleSheet(btnStyleSheet);
    ui->servoOff->setStyleSheet(btnStyleSheet);
    ui->emgcStop->setStyleSheet("QPushButton{background-color:rgb(46, 170, 0);color: black;border-radius: 10px;border: 2px groove gray;border-style: outset;font: 20px;}\
                                QPushButton:hover{color: white;font: 25px;}");
    // test
    //receiver = new QUdpSocket(this);
    //receiver->bind(QHostAddress("192.168.3.10"), 65001);
    //connect(receiver, SIGNAL(readyRead()),this, SLOT(readPendingDatagrams()));

}

MainWindow::~MainWindow()
{
    // É¾ï¿½ï¿½ï¿½Ëµï¿½ï¿½ï¿½ï¿½ï¿½
//    foreach (QVector<int>* menu, menus) {
//        delete menu;
//    }

    delete ui;
}

void MainWindow::createDisplayUIs(QList<InfoUI *> &widgets)
{
    // ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ëµï¿½
    setupMainMenuGroup();

    // Îªï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ëµï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ó²Ëµï¿½
    createSubMenuList();

//        // ï¿½ï¿½ï¿½ï¿½ï¿½Ó²Ëµï¿½ï¿½ï¿½ui
//        connectMenuWithUi(widgets);

//    setDefaultMenuSelection();

    //createHomeMenu();
    createStatusBar();
//    createHelpText();
}

void MainWindow::goToUiPage(int page)
{
    int id;

    Q_ASSERT(UI_PAGE_NUMBER > page);

    if(nCurrentPage != page){
        pMainMenuGroup->buttons().at(nCurrentPage)->setDown(false);
    }
    pMainMenuGroup->buttons().at(page)->setDown(true);
    nCurrentPage = page;

    ui->menuList->setCurrentIndex(nCurrentPage); // ÏÔÊ¾µ±Ç°½çÃæµÄ×Ó²Ëµ¥(Ò³Ãæ°´Å¥)

    id = sonMenuLastID[page];
    setCurrentUiPage(id);
}

void MainWindow::setCurrentUiPage(int id)
{
    QAbstractButton * btn;
    int i;

    if(-1 == nCurrentPage)
        return;

    ui->uis->setCurrentIndex(id);
    sonMenuLastID[nCurrentPage] = id;
    ((CCommmomUIWidget *)(ui->uis->currentWidget()))->UpdateViewData(); //

    /*
    for(i=0;i<sonMenuGrp.at(nCurrentPage)->buttons().count();i++){
        sonMenuGrp.at(nCurrentPage)->buttons().at(i)->setDown(false);
    }
    btn = sonMenuGrp.at(nCurrentPage)->button(id);*/
    for(i=0;i<sonMenuGrp[nCurrentPage]->buttons().count();i++){
        sonMenuGrp[nCurrentPage]->buttons().at(i)->setDown(false);
    }
    btn = sonMenuGrp[nCurrentPage]->button(id);
    if(btn != NULL){
        btn->setDown(true);
    }
}

//void MainWindow::goToUiPage(int page)
//{
//    Q_ASSERT(UI_PAGE_NUMBER > page);

//    nCurrentPage = page;

//    setCurrentUiPage();
//}



//void MainWindow::setCurrentUiPage()
//{
//    if(-1 == nCurrentPage)
//        return;

//    int rowId = listWidgets.at(nCurrentPage)->currentRow();
//    if(-1 == rowId)
//    {
//        if(listWidgets.at(nCurrentPage)->count() > 0)
//        {
//            listWidgets.at(nCurrentPage)->setCurrentRow(0);
//            rowId = 0;
//        } else {
//            return;
//        }
//    }
//    int id = menus.at(nCurrentPage)->at(rowId);
//    ui->uis->setCurrentIndex(id);

//    ui->menuList->setCurrentIndex(nCurrentPage);
//}

//void MainWindow::currentMenuChanged(int row)
//{
//    setCurrentUiPage();
//}

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

void MainWindow::createSubMenuList()
{
    for(int i = 0; i < UI_PAGE_NUMBER; i++)
    {
        QWidget *widget = new QWidget(ui->menuList);
        widget->setGeometry(0,0,623,35);
        QButtonGroup *pButtonGroup = new QButtonGroup(widget);
        ui->menuList->addWidget(widget);
        sonMenuGrp.push_back(pButtonGroup);
        sonMenuLastID.push_back(0);

        connect(pButtonGroup, SIGNAL(buttonClicked(int)),this, SLOT(setCurrentUiPage(int)));
        //menus.push_back(new QVector<int>);
    }
//    for(int i = 0; i < UI_PAGE_NUMBER; i++)
//    {
//        QListWidget *pListWidget = new QListWidget(ui->menuList);
//        pListWidget->setFrameShape(QFrame::Box);
//        pListWidget->setStyleSheet("QListWidget{"
//                                   "background-color:rgb(240, 240, 240);}"
////                                   "QListWidget::item:unselected{"
////                                   "border-image: url(:/images/itemunselected.png);}"
////                                   "QListWidget::item:selected{"
////                                   "border-image: url(:/images/itemselected.png);}"
//                                   );

//        //pListWidget->setFont(QFont("ï¿½ï¿½ï¿½ï¿½", 22, QFont::Normal));//Courier
//        pListWidget->setFont(QFont("Microsoft YaHei", 12, QFont::Normal));

//        /*pListWidget->setViewMode(QListView::IconMode);
//        pListWidget->setMovement(QListView::Static);
//        pListWidget->setWrapping(false);
//        pListWidget->setFlow(QListView::LeftToRight);*/
//        pListWidget->setViewMode(QListView::ListMode);
//        pListWidget->setFlow(QListView::LeftToRight);
//        ui->menuList->addWidget(pListWidget);
//        listWidgets.push_back(pListWidget);

//        connect(pListWidget, SIGNAL(currentRowChanged(int)),
//                this, SLOT(currentMenuChanged(int)));

//        menus.push_back(new QVector<int>);
//    }
}

void MainWindow::connectMenuWithUi(QList<InfoUI *> &widgets)
{
    int id;

    for(int i = 0; i < ui->uis->count(); i++)
    {
        ui->uis->removeWidget(ui->uis->widget(i));
    }

    //for(int i = 0; i < menus.count(); i++){
    //    menus.at(i)->clear();
    //}
    //for(int i = 0; i < sonMenuGrp.count(); i++){
    //    sonMenuGrp.at(i)->clear();
   // }


    for(int i = 0; i < widgets.count(); i++)
    {
        if(widgets.at(i)->bShow)
        {
            widgets.at(i)->pUi->setHidden(false);
            ui->uis->addWidget(widgets.at(i)->pUi); //
            widgets.at(i)->pParentUi = ui->uis; //

            id = ui->uis->count()-1;
            //menus.at(widgets.at(i)->nType)->push_back(id);
            //
            addMenuButton(widgets.at(i)->nType, widgets.at(i)->sName,id);
        }
     else
     {
            widgets.at(i)->pUi->setHidden(true);
        }
    }

}

void MainWindow::addMenuButton(int nType, QString name,int id)
{
    int x;
    static int preType =-1;
    static int cnt=0;

    QWidget *curMenuFrm = ui->menuList->widget(nType);
    QToolButton *btn = new QToolButton(curMenuFrm);
    QFont fnt = btn->font();
    fnt.setPixelSize(20);
    btn->setFont(fnt);
    btn->setText(name);
    if(preType != nType){
        cnt = 0 ;
        sonMenuLastID[nType] = id;
        preType = nType;
    }else{
        cnt++;
    }
    x = 10+cnt*80;
    btn->setGeometry(x,2,65,30);
    sonMenuGrp[nType]->addButton(btn, id);
}

//void MainWindow::connectMenuWithUi(QList<InfoUI *> &widgets)
//{
//    for(int i = 0; i < ui->uis->count(); i++)
//    {
//        ui->uis->removeWidget(ui->uis->widget(i));
//    }

//    for(int i = 0; i < menus.count(); i++)
//    {
//        menus.at(i)->clear();
//    }
//    for(int i = 0; i < listWidgets.count(); i++)
//    {
//        listWidgets.at(i)->clear();
//    }


//    for(int i = 0; i < widgets.count(); i++)
//    {
//        if(widgets.at(i)->bShow)
//        {
//            widgets.at(i)->pUi->setHidden(false);
//            ui->uis->addWidget(widgets.at(i)->pUi); // ï¿½ï¿½uiï¿½Åµï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Úºï¿½ï¿½Êµï¿½Î»ï¿½ï¿½
//            widgets.at(i)->pParentUi = ui->uis; // ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½uiï¿½ï¿½parent

//            // ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ó¦ï¿½Ä²Ëµï¿½ï¿½ï¿½
//            addMenuText(widgets.at(i)->nType, widgets.at(i)->sName);

//            // ï¿½ó¶¨²Ëµï¿½idï¿½ï¿½uiï¿½ï¿½id
//            menus.at(widgets.at(i)->nType)->push_back(ui->uis->count()-1);
//        }
//	 else
//	 {
//            widgets.at(i)->pUi->setHidden(true);
//        }
//    }

//    for(int i = 0; i < listWidgets.count();i++)
//    {
//        int count = listWidgets.at(i)->count();
//        for(int j = 0; j < count; j++)
//        {
//         QListWidgetItem *item = listWidgets.at(i)->item(j);
//         //item->setSizeHint(QSize(item->sizeHint().width(), 50));
//         item->setSizeHint(QSize(80, 30));
//        }
//    }

//    for(int i = 0; i < listWidgets.count(); i++)
//    {
//        if(listWidgets.at(i)->count() < 1)
//        {
//            pMainMenuGroup->button(i)->setDisabled(true);
//        }
//    }
//}

//void MainWindow::addMenuText(int nType, QString name)
//{
//    QListWidgetItem *item = new QListWidgetItem();
//    listWidgets.at(nType)->addItem(item);

//    QLabel *text = new QLabel("    " + name, listWidgets.at(nType));
//    text->setFont(QFont("ï¿½ï¿½ï¿½ï¿½", 12, QFont::Bold));
//    //text->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
//    text->setAlignment(Qt::AlignHCenter | Qt::AlignCenter);
//    text->setFocusPolicy(Qt::NoFocus);

//    listWidgets.at(nType)->setItemWidget(item, text);
//}

//void MainWindow::setDefaultMenuSelection()
//{
//    foreach (QListWidget *w, listWidgets) {
//        if(w->count() > 0)
//        {
//            w->setItemSelected(w->item(0), true);
//        }
//    }
//}





void MainWindow::createStatusBar()
{
    messageLabel = new QLabel(this);
    messageLabel->setFont(QFont("ï¿½ï¿½ï¿½ï¿½", 13, QFont::Normal));
    messageLabel->setAlignment(Qt::AlignLeft);

    statusBar()->addWidget(messageLabel);
    statusBar()->setFont(QFont("ï¿½ï¿½ï¿½ï¿½", 13, QFont::Normal));
    statusBar()->setFixedHeight(40);
    statusBar()->setStyleSheet("QStatusBar{border: 1px solid rgb(204, 204, 204);}"
                               "QStatusBar::item{border: 0px;}");

}

void MainWindow::writeMsgToStatusBar(QString msg, int level)
{
    messageLabel->setText(msg);
    //messageLabel->setStyleSheet("color: red;");
    //return;

    //ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½É«
    //QPalette pa;
    switch (level)
    {
    case STATUSBAR_ERROR_INFO://ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
        //pa.setColor(QPalette::WindowText, Qt::red);
        messageLabel->setStyleSheet("color: red;");
        break;
    case STATUSBAR_WARNING_INFO://ï¿½ï¿½ï¿½ï¿½
        //pa.setColor(QPalette::WindowText, Qt::darkRed);
        messageLabel->setStyleSheet("color: darkRed;");
        break;
    case STATUSBAR_STATUS_ON_INFO://×´Ì¬ï¿½Ð»ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ö£ï¿½
        //pa.setColor(QPalette::WindowText, Qt::darkYellow);
        messageLabel->setStyleSheet("color: blue;");
        break;
    case STATUSBAR_STATUS_OFF_INFO://×´Ì¬ï¿½Ð»ï¿½ï¿½ï¿½ï¿½Ø±Õ£ï¿½
        //pa.setColor(QPalette::WindowText, Qt::darkGreen);
        messageLabel->setStyleSheet("color: darkGreen;");
        break;
    default://ï¿½ï¿½Ê¾ï¿½ï¿½Ï¢/ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Â¼
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
    case ORDINARY_USER://ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
        for(int i = 0; i < widgets.count(); i++)
        {
            if(widgets.at(i)->sName == "ÏµÍ³ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½"
                    || widgets.at(i)->sName == "ï¿½Ë¶ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½"
                    || widgets.at(i)->sName == "ï¿½ï¿½ï¿½Õ²ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½"
                    || widgets.at(i)->sName == "Ä£ï¿½ß¿ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½"
                    || widgets.at(i)->sName == "ï¿½â²¿ï¿½è±¸Í¨ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½")
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
    case ADVANCED_USER://ï¿½ï¿½ï¿½ï¿½Ô±
        for(int i = 0; i < widgets.count(); i++)
        {
            if(widgets.at(i)->sName == "ï¿½ï¿½ï¿½Õ²ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½")
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
    case SUPER_USER://ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
        for(int i = 0; i < widgets.count(); i++)
        {
            widgets.at(i)->bShow = true;
            ((CCommmomUIWidget *)(widgets.at(i)->pUi))->setDisableEdit(false);
        }
        break;
    }

    // ï¿½ï¿½ï¿½ï¿½ï¿½Ó²Ëµï¿½ï¿½ï¿½ui
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
	/* ui->enable->setText(QString::fromLocal8Bit("Ê¹ï¿½ï¿½"));
	 ui->enable->setChecked(1);
	 ui->start->setEnabled(1);
	 ui->cyclerun->setEnabled(1);*/
    }
    else
    {
        SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/servoEnable", "no");
	/*ui->enable->setText(QString::fromLocal8Bit("ï¿½ï¿½Ê¹ï¿½ï¿½"));
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
		 //ui->mode->setText(QString::fromLocal8Bit("ï¿½Ô¶ï¿½"));
	    }
		
	   /* if((0x80000==(SystemScheduler->m_pSystemParameter->sys_ctrl.statebit&0xc0000))||(0x0==(SystemScheduler->m_pSystemParameter->sys_ctrl.statebit&0xc0000)))
	    {
	        SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/mode", "1");
		 //ui->mode->setText(QString::fromLocal8Bit("ï¿½Ö¶ï¿½"));
	    }
	    else
	    {
	        SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/mode", "2");
		 //ui->mode->setText(QString::fromLocal8Bit("ï¿½Ô¶ï¿½"));
	    }*/
     }
     else
     {
     		SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/mode", "0");
		//ui->mode->setText(QString::fromLocal8Bit("Ä£Ê½"));
     }

     /*if((SystemScheduler->m_pSystemParameter->sys_ctrl.statebit&0x1)==1)
     {
	    // if (checked)
	    if((0x40==(SystemScheduler->m_pSystemParameter->sys_ctrl.statebit&0x60))||(0x0==(SystemScheduler->m_pSystemParameter->sys_ctrl.statebit&0x60)))
	    {
	        SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/mode", "1");
		 //ui->mode->setText(QString::fromLocal8Bit("ï¿½Ö¶ï¿½"));
	    }
	    else
	    {
	        SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/mode", "2");
		 //ui->mode->setText(QString::fromLocal8Bit("ï¿½Ô¶ï¿½"));
	    }
     }
     else
     {
     		SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/mode", "0");
		//ui->mode->setText(QString::fromLocal8Bit("Ä£Ê½"));
     }*/
    
}

void MainWindow::on_axis1pos_pressed()
{
     //SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/xPMove start");
     (SystemScheduler->m_pSystemParameter->sys_ctrl.sendinput[0])&=~0x2;//clear stopbit
     if(movemode)//ï¿½ï¿½ï¿½ï¿½
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
     else//ï¿½ï¿½ï¿½ï¿½
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
      if(movemode)//ï¿½ï¿½ï¿½ï¿½
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
     else//ï¿½ï¿½ï¿½ï¿½
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
    if(movemode)//ï¿½ï¿½ï¿½ï¿½
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
     else//ï¿½ï¿½ï¿½ï¿½
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
    if(movemode)//ï¿½ï¿½ï¿½ï¿½
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
     else//ï¿½ï¿½ï¿½ï¿½
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
     if(movemode)//ï¿½ï¿½ï¿½ï¿½
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
     else//ï¿½ï¿½ï¿½ï¿½
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
      if(movemode)//ï¿½ï¿½ï¿½ï¿½
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
     else//ï¿½ï¿½ï¿½ï¿½
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
    if(movemode)//ï¿½ï¿½ï¿½ï¿½
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
     else//ï¿½ï¿½ï¿½ï¿½
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
    if(movemode)//ï¿½ï¿½ï¿½ï¿½
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
     else//ï¿½ï¿½ï¿½ï¿½
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

void MainWindow::on_axis5pos_pressed()
{
     (SystemScheduler->m_pSystemParameter->sys_ctrl.sendinput[0])&=~0x2;
    if(movemode)//ï¿½ï¿½ï¿½ï¿½
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
     else//ï¿½ï¿½ï¿½ï¿½
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

void MainWindow::on_axis5neg_pressed()
{
     (SystemScheduler->m_pSystemParameter->sys_ctrl.sendinput[0])&=~0x2;
    if(movemode)//ï¿½ï¿½ï¿½ï¿½
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
     else//ï¿½ï¿½ï¿½ï¿½
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

void MainWindow::on_axis6pos_pressed()
{
     (SystemScheduler->m_pSystemParameter->sys_ctrl.sendinput[0])&=~0x2;
    if(movemode)//ï¿½ï¿½ï¿½ï¿½
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
     else//ï¿½ï¿½ï¿½ï¿½
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

void MainWindow::on_axis6neg_pressed()
{
     (SystemScheduler->m_pSystemParameter->sys_ctrl.sendinput[0])&=~0x2;
    if(movemode)//ï¿½ï¿½ï¿½ï¿½
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
     else//ï¿½ï¿½ï¿½ï¿½
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

void MainWindow::on_axis7pos_pressed()
{
     (SystemScheduler->m_pSystemParameter->sys_ctrl.sendinput[0])&=~0x2;
    if(movemode)//ï¿½ï¿½ï¿½ï¿½
     {
             double moveLen = ui->moveLength->value();
            if(!movecoor)
            {

             if (fabs(moveLen) > 0.0001)
             {
                 SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/mPMotion", QString::number(moveLen));
             }
            }
         else
         {
                SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/mPMotion_world", QString::number(moveLen));
         }
     }
     else//ï¿½ï¿½ï¿½ï¿½
     {
             if(!movecoor)
            {

                 SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/mPMove", "start");
            }
         else
         {
                SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/mPMove_world", "start");
         }
     }
}

void MainWindow::on_axis7neg_pressed()
{
     (SystemScheduler->m_pSystemParameter->sys_ctrl.sendinput[0])&=~0x2;
    if(movemode)//ï¿½ï¿½ï¿½ï¿½
     {
             double moveLen = ui->moveLength->value();
            if(!movecoor)
            {

             if (fabs(moveLen) > 0.0001)
             {
                 SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/mNMotion", QString::number(moveLen));
             }
            }
         else
         {
                SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/mNMotion_world", QString::number(moveLen));
         }
     }
     else//ï¿½ï¿½ï¿½ï¿½
     {
             if(!movecoor)
            {

                 SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/mNMove", "start");
            }
         else
         {
                SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/mNMove_world", "start");
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
    ui->vel->setText(QString::asprintf("%3d%%",value));
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
    (SystemScheduler->m_pSystemParameter->sys_ctrl.sendinput[0])&=~0x2;  // È¡Ïû Í£Ö¹Ö¸Áî

    if(!ui->ckbCycleRun->isChecked()){ // µ¥¸ö
        SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "start motion");
    }else{ // Ñ­»·ÔËÐÐ
        SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "start motion", "cycle");
    }
    //ui->cyclerun->setEnabled(0);
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
      if(movemode)//ï¿½ï¿½ï¿½ï¿½
     {
     	     
     }	
     else//ï¿½ï¿½ï¿½ï¿½
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
       if(movemode)//ï¿½ï¿½ï¿½ï¿½
     {
     	     
     }	
     else//ï¿½ï¿½ï¿½ï¿½
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
       if(movemode)//ï¿½ï¿½ï¿½ï¿½
     {
     	     
     }	
     else//ï¿½ï¿½ï¿½ï¿½
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
      if(movemode)//ï¿½ï¿½ï¿½ï¿½
     {
     	     
     }	
     else//ï¿½ï¿½ï¿½ï¿½
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
      if(movemode)//ï¿½ï¿½ï¿½ï¿½
     {
     	     
     }	
     else//ï¿½ï¿½ï¿½ï¿½
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
     if(movemode)//ï¿½ï¿½ï¿½ï¿½
     {
     	     
     }	
     else//ï¿½ï¿½ï¿½ï¿½
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
	 if(movemode)//ï¿½ï¿½ï¿½ï¿½
     {
     	     
     }	
     else//ï¿½ï¿½ï¿½ï¿½
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
      if(movemode)//ï¿½ï¿½ï¿½ï¿½
     {
     	     
     }	
     else//ï¿½ï¿½ï¿½ï¿½
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

void MainWindow::on_axis5pos_released()
{
     if(movemode)//ï¿½ï¿½ï¿½ï¿½
     {

     }
     else//ï¿½ï¿½ï¿½ï¿½
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

void MainWindow::on_axis5neg_released()
{
      if(movemode)//ï¿½ï¿½ï¿½ï¿½
     {

     }
     else//ï¿½ï¿½ï¿½ï¿½
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

void MainWindow::on_axis6pos_released()
{
     if(movemode)//ï¿½ï¿½ï¿½ï¿½
     {

     }
     else//ï¿½ï¿½ï¿½ï¿½
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

void MainWindow::on_axis6neg_released()
{
      if(movemode)//ï¿½ï¿½ï¿½ï¿½
     {

     }
     else//ï¿½ï¿½ï¿½ï¿½
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

void MainWindow::on_axis7pos_released()
{
     if(movemode)//ï¿½ï¿½ï¿½ï¿½
     {

     }
     else//ï¿½ï¿½ï¿½ï¿½
     {
             if(!movecoor)
            {

                 SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/mPMove", "stop");
            }
         else
         {
                SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/mPMove_world", "stop");
         }
         stop_clicked();
     }
}

void MainWindow::on_axis7neg_released()
{
      if(movemode)//ï¿½ï¿½ï¿½ï¿½
     {

     }
     else//ï¿½ï¿½ï¿½ï¿½
     {
             if(!movecoor)
            {

                 SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/mNMove", "stop");
            }
         else
         {
                SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/mNMove_world", "stop");
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
//	ui->cyclerun->setEnabled(1);
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

void MainWindow::readPendingDatagrams()
 {
    int size=0;
    char buf[4096]={0};

     while (receiver->hasPendingDatagrams()) {
         QByteArray datagram;
         datagram.resize(receiver->pendingDatagramSize());
         size = datagram.size();
         memcpy(buf,datagram.data(),size);
         receiver->readDatagram(buf, size);
         //Êý¾Ý½ÓÊÕÔÚdatagramÀï
         /* readDatagram º¯ÊýÔ­ÐÍ
          qint64 readDatagram(char *data,qint64 maxSize,QHostAddress *address=0,quint16 *port=0)
         */
     }
 }

// »ñÈ¡ ÔË¶¯²ÎÊý, °å¿¨±àºÅ,ËÅ·þPID²ÎÊý
void MainWindow::GetAllSysPara(void)
{
    int i;
    int cnt[2]={0,0};
    int cnt2[7]={0,0,0,0,0,0,0};

    SystemScheduler->m_pSystemParameter->getsysparamfinished = 0;
    SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/getsysparam", "yes");
    while(SystemScheduler->m_pSystemParameter->getsysparamfinished==0){
        cnt[0]++;
        if(cnt[0]>=10){
            SystemScheduler->m_pSystemParameter->getsysparamfinished = 1; //
            break;
        }
        QThread::msleep(50);
    }

    SystemScheduler->m_pSystemParameter->getaxinofinished=0;
    SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/getaxisno", "yes");
    while(SystemScheduler->m_pSystemParameter->getaxinofinished==0){
        cnt[1]++;
        if(cnt[1]>=10){
            SystemScheduler->m_pSystemParameter->getaxinofinished = 1; //
            break;
        }
        QThread::msleep(50);
    }

    //-----------------------------------------------------------------------------
    for(i=0;i<2;i++){  // ÏÖÔÚÎÊÌâ:µÚÒ»´ÎÇý¶¯Æ÷·´À¡µÄÊý¾Ý¿ÉÄÜ»áÊÇÂÒµÄ. ¹Ê¶à¶ÁÒ»´Î. ÏÈ±ÜÃâÕâ¸öÎÊÌâ
        SystemScheduler->m_pSystemParameter->getpidparamfinished[0] = 0;
        SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "getPIDParamer", "15");
        while(SystemScheduler->m_pSystemParameter->getpidparamfinished[0]==0){
            cnt2[0]++;
            if(cnt2[0]>=10){
                SystemScheduler->m_pSystemParameter->getpidparamfinished[0] = 1; //
                break;
            }
            QThread::msleep(50);
        }
    }

    SystemScheduler->m_pSystemParameter->getpidparamfinished[1] = 0;
    SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "getPIDParamer", "25");
    while(SystemScheduler->m_pSystemParameter->getpidparamfinished[1]==0){
        cnt2[1]++;
        if(cnt2[1]>=10){
            SystemScheduler->m_pSystemParameter->getpidparamfinished[1] = 1; //
            break;
        }
        QThread::msleep(50);
    }

    SystemScheduler->m_pSystemParameter->getpidparamfinished[2] = 0;
    SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "getPIDParamer", "35");
    while(SystemScheduler->m_pSystemParameter->getpidparamfinished[2]==0){
        cnt2[2]++;
        if(cnt2[2]>=10){
            SystemScheduler->m_pSystemParameter->getpidparamfinished[2] = 1; //
            break;
        }
        QThread::msleep(50);
    }

    SystemScheduler->m_pSystemParameter->getpidparamfinished[3] = 0;
    SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "getPIDParamer", "45");
    while(SystemScheduler->m_pSystemParameter->getpidparamfinished[3]==0){
        cnt2[3]++;
        if(cnt2[3]>=10){
            SystemScheduler->m_pSystemParameter->getpidparamfinished[3] = 1; //
            break;
        }
        QThread::msleep(50);
    }

    SystemScheduler->m_pSystemParameter->getpidparamfinished[4] = 0;
    SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "getPIDParamer", "55");
    while(SystemScheduler->m_pSystemParameter->getpidparamfinished[4]==0){
        cnt2[4]++;
        if(cnt2[4]>=10){
            SystemScheduler->m_pSystemParameter->getpidparamfinished[4] = 1; //
            break;
        }
        QThread::msleep(50);
    }

    SystemScheduler->m_pSystemParameter->getpidparamfinished[5] = 0;
    SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "getPIDParamer", "65");
    while(SystemScheduler->m_pSystemParameter->getpidparamfinished[5]==0){
        cnt2[5]++;
        if(cnt2[5]>=10){
            SystemScheduler->m_pSystemParameter->getpidparamfinished[5] = 1; //
            break;
        }
        QThread::msleep(50);
    }

    SystemScheduler->m_pSystemParameter->getpidparamfinished[6] = 0;
    SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "getPIDParamer", "75");
    while(SystemScheduler->m_pSystemParameter->getpidparamfinished[6]==0){
        cnt2[6]++;
        if(cnt2[6]>=10){
            SystemScheduler->m_pSystemParameter->getpidparamfinished[6] = 1; //
            break;
        }
        QThread::msleep(50);
    }

}

void MainWindow::updataData()
{
    static int preServoState=-1;
    static int preNetState=-1;
    static int cnt=0;
    static int firstConn = 0; // 0:Î´Á¬½Ó¹ý 1:Ê×´ÎÁ¬½ÓÍøÂç 2:¶à´ÎÁ¬½Ó
    CCommmomUIWidget *curWidget=NULL;
    int bState=0;
    /*
    MotionControllerDevice *pDev=NULL;
    FrameBody frame;
    frame.length = 3;

    pDev = (MotionControllerDevice *)(SystemScheduler->getModulesList()->at(1)->getDevice());
    if(pDev->m_socket2!=NULL){
        //pDev->m_socket2->m_serverip = QHostAddress::Broadcast; // QHostAddress("192.168.3.10"); // QHostAddress::LocalHost;
        //pDev->m_socket2->m_serverport = 53001;
        pDev->m_socket2->Uart_Send(0, (char *)&frame, frame.length);
    }*/

    // ÅÐ¶ÏÍøÂçÊÇ·ñÁ¬½Ó
    if(SystemScheduler->NetIsConnect()){
        bState = 1;
    }else{
        bState = 0;
        cnt++;
    }
    if(preNetState != bState){
        if(bState){
            if(firstConn < 2){
                firstConn++;
            }
            ui->btnNetState->setText(QString::fromLocal8Bit("Í¨"));
            ui->btnNetState->setStyleSheet("QPushButton{"
                                           "background-color: rgb(46, 170, 0);"
                                           "color: white;"
                                           "border-radius: 10px;"
                                           "border: 2px groove gray;"
                                           "border-style: outset;}");
        }else{
            ui->btnNetState->setText(QString::fromLocal8Bit("¶Ï"));
            ui->btnNetState->setStyleSheet("QPushButton{"
                                           "background-color: darkgray;"
                                           "color: white;"
                                           "border-radius: 10px;"
                                           "border: 2px groove gray;"
                                           "border-style: outset;}");
        }
        preNetState = bState;
    }
    if(firstConn == 1){
        firstConn++;
        GetAllSysPara();
    }

    if(SystemScheduler->GetServoState()){ // Ê¹ÄÜ
        bState = 1;
    }else{
        bState = 0;
    }
    if(preServoState != bState){
        ui->start->setEnabled(bState);
//        QPalette pal = ui->btnServoState->palette();
        if(bState){
            ui->btnServoState->setText(QString::fromLocal8Bit("¿ª"));
            ui->btnServoState->setStyleSheet("QPushButton{"
                                             "background-color: rgb(46, 170, 0);"
                                             "color: white;"
                                             "border-radius: 10px;"
                                             "border: 2px groove gray;"
                                             "border-style: outset;}");
        }else{
            ui->btnServoState->setText(QString::fromLocal8Bit("¶Ï"));
            ui->btnServoState->setStyleSheet("QPushButton{"
                                             "background-color: darkgray;"
                                             "color: white;"
                                             "border-radius: 10px;"
                                             "border: 2px groove gray;"
                                             "border-style: outset;}");
        }
        preServoState = bState;
    }


     if(0x40000==(SystemScheduler->m_pSystemParameter->sys_ctrl.statebit&0xc0000))
    {
        //SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/mode", "1");
     ui->mode->setText(QString::fromLocal8Bit("ÊÖ¶¯"));
    }
    else  if(0x80000==(SystemScheduler->m_pSystemParameter->sys_ctrl.statebit&0xc0000))
    {
        //SystemScheduler->recvMsgFromWindows(MOTION_CONTROLLER_ID, "manual/mode", "2");
     ui->mode->setText(QString::fromLocal8Bit("×Ô¶¯"));
    }
    else
    {
        ui->mode->setText(QString::fromLocal8Bit("Ä£Ê½"));
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

    curWidget = dynamic_cast<CCommmomUIWidget *>(ui->uis->currentWidget());
    if(curWidget != NULL){
        curWidget->TimerUpdateViewData();
    }

}

void MainWindow::on_servoOn_clicked()
{
    SystemScheduler->SetServoState(true);
}

void MainWindow::on_servoOff_clicked()
{
    SystemScheduler->SetServoState(false);
}

void MainWindow::on_emgcStop_clicked()
{
    QColor mGreen(46, 170, 0);
    if(ui->emgcStop->palette().color(QPalette::Background) == mGreen){
        (SystemScheduler->m_pSystemParameter->sys_ctrl.sendinput[0])|=0x4;
        ui->emgcStop->setStyleSheet("QPushButton{background-color:rgb(200, 0, 0);color: black;border-radius: 10px;border: 2px groove gray;border-style: outset;font: 20px;}\
                                    QPushButton:hover{color: white;font: 25px;}");
    }
    else{
        (SystemScheduler->m_pSystemParameter->sys_ctrl.sendinput[0])&=~0x4;
        ui->emgcStop->setStyleSheet("QPushButton{background-color:rgb(46, 170, 0);color: black;border-radius: 10px;border: 2px groove gray;border-style: outset;font: 20px;}\
                                    QPushButton:hover{color: white;font: 25px;}");
    }
    QString tmp;
    tmp = ui->groupBox->styleSheet();
    qDebug()<<tmp;
}
