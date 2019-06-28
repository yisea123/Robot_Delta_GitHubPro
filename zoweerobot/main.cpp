#include <QApplication>
#include <QTextCodec>
#include <QDebug>

#include "mainwindow.h"
#include "clogindialog.h"

#include "DesEncryptionModel.h"
#include "SystemProcessInfo.h"

#include "GlobalVariableType.h"


UserParameter checkRight(int *right)
{
    CLoginDialog loginDialog(right);
    loginDialog.exec();

    return loginDialog.getUserInfo();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("System"));


    int loginResult = 0;
    UserParameter user;// = checkRight(&loginResult);
    user.m_password="12345678";
    user.m_permission=SUPER_USER;
    user.m_userName="root";
    loginResult=1;
    MainWindow w(user);
    w.show();

    return a.exec();
}
