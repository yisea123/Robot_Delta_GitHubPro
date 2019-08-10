#include <QApplication>
#include <QTextCodec>
//#include <QtSingleApplication>
#include <QDebug>
#include <QSharedMemory>
#include "mainwindow.h"
#include "clogindialog.h"

#include "DesEncryptionModel.h"
#include "SystemProcessInfo.h"

#include "GlobalVariableType.h"
#include "CUserLevelManage.h"

#ifdef TRS_LOC
#include <QTranslator>
#endif


UserParameter checkRight(int *right)
{
    CLoginDialog loginDialog(right);
    loginDialog.exec();

    return loginDialog.getUserInfo();
}

int main(int argc, char *argv[])
{
    int level;

    QSharedMemory shared("ZoBo Robot4");//
    qDebug()<<QObject::tr("zobo robot");
    if (shared.attach()){  // 只运行一个实例。 临时方案,
            return 0;
    }
    shared.create(1);

    QApplication a(argc, argv);
#ifdef TRS_LOC
    QTranslator *translator = new QTranslator;
    translator->load("../zoweerobot/translations/zoweerobot_zh_CN.qm");
    a.installTranslator(translator);
    qDebug()<<QObject::tr("translator is ok!");
#endif
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("System"));


    int loginResult = 0;
    UserParameter user;// = checkRight(&loginResult);
    //user.m_password="12345678";
    //user.m_permission=SUPER_USER;
    //user.m_userName="root";

    CUserLevelManage::InitUserLevelManage();
    CUserLevelManage::ReadUsersInfo();
    level = ADVANCED_USER; // SUPER_USER;
    user = CUserLevelManage::m_OptUser[level];
    //CUserLevelManage::SaveUsersInfo();
    loginResult=1;
    MainWindow w(user);
    w.show();

    return a.exec();
}
