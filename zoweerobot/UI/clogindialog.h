#ifndef CLOGINDIALOG_H
#define CLOGINDIALOG_H

#include <QDialog>

#include "GlobalVariableType.h"

namespace Ui {
class CLoginDialog;
}

class CLoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CLoginDialog(int *right, QWidget *parent = 0);
    ~CLoginDialog();

    //UserPermission getUserPermission(QString username);    /**< ��ѯ�û���Ȩ����Ϣ */
    UserParameter getUserInfo();    /**< ��ȡ��ǰ�û���Ϣ */

private:
    bool restoreUsersInfo();
    bool readUsersInfo();
    bool checkUserName(QString username, UserPermission permission);
    bool checkUserPassword(QString username, QString password, UserPermission permission);

    void creatUsersInfoFolder();

private slots:

    void on_login_clicked();

private:
    Ui::CLoginDialog *ui;
    int *m_right;
    QString  m_sUserName;
    QString  m_sPassWord;
    UserPermission m_permission;

    QVector<UserParameter> m_userParm;
};

#endif // CLOGINDIALOG_H
