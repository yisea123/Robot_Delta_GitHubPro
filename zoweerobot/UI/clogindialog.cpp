#include "clogindialog.h"
#include "ui_clogindialog.h"

#include <QMessageBox>
#include <QFile>
#include <QDataStream>
#include "DesEncryptionModel.h"
#include "SystemProcessInfo.h"

#include <QFile>
#include <QtXml>

CLoginDialog::CLoginDialog(int *right, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CLoginDialog),
    m_right(right)
{
    ui->setupUi(this);

    UserParameter defaultUser;
    defaultUser.m_userName = "root";
    defaultUser.m_password = "12345678";
    defaultUser.m_permission = SUPER_USER;
    m_userParm.append(defaultUser);

    readUsersInfo();
//    for (int i=0; i<m_userParm.size(); i++)
//    {
//        SystemProcessInfo::printfMesssage("XMLuserName:"+m_userParm.at(i).m_userName);
//        SystemProcessInfo::printfMesssage("XMLpassvord:"+m_userParm.at(i).m_password);
//        SystemProcessInfo::printfMesssage("XMLpermission:"+QString::number(m_userParm.at(i).m_permission));
//    }
}

CLoginDialog::~CLoginDialog()
{
    delete ui;
}


void CLoginDialog::on_login_clicked()
{
    m_sUserName = ui->user->text();
    m_sPassWord = ui->password->text();
    m_permission = (UserPermission)ui->permission->currentIndex();

    if(m_sPassWord.size() < 8)
    {
        QMessageBox::warning(NULL, "warning", "请输入8位以上密码", QMessageBox::Yes, QMessageBox::Yes);
        return ;
    }

    if (!checkUserName(m_sUserName, m_permission))
    {
        QMessageBox::warning(NULL, "warning", "用户名不存在", QMessageBox::Yes, QMessageBox::Yes);
        return ;
    }

    if (!checkUserPassword(m_sUserName, m_sPassWord, m_permission))
    {
        QMessageBox::warning(NULL, "warning", "密码错误", QMessageBox::Yes, QMessageBox::Yes);
        return ;
    }

    *m_right = 1;

    close();
}

bool CLoginDialog::restoreUsersInfo()
{
    QDomDocument doc;
    QDomProcessingInstruction  instruction;
    instruction = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(instruction);

    QDomElement root = doc.createElement(QString("数控冲压系统用户信息"));
    doc.appendChild(root);

    QDomElement users;
    QDomText text;
    QDomElement userInfo;
    DesEncryptionModel  model;
    QString userName = "";
    QString password = "";
    for(int i = 0; i < m_userParm.size(); i++)
    {
        /* 存储数据前先加密 */
        userName = model.Des_Run(m_userParm.at(i).m_userName, DesEncryptionModel::ENCRYPT);
        password = model.Des_Run(m_userParm.at(i).m_password, DesEncryptionModel::ENCRYPT);

        users = doc.createElement("user" + QString::number(i));

        userInfo = doc.createElement("userName");
        text = doc.createTextNode(userName);
        userInfo.appendChild(text);
        users.appendChild(userInfo);

        userInfo = doc.createElement("password");
        text = doc.createTextNode(password);
        userInfo.appendChild(text);
        users.appendChild(userInfo);

        userInfo = doc.createElement("permission");
        text = doc.createTextNode(QString::number(m_userParm.at(i).m_permission));
        userInfo.appendChild(text);
        users.appendChild(userInfo);

        root.appendChild(users);
    }

    QFile file(".//connect//users.xml");

    if (!file.open(QIODevice::ReadWrite | QIODevice::Truncate))
    {
        return false;
    }
    QTextStream out(&file);
    doc.save(out, 4);
    file.close();

    return true;
}

bool CLoginDialog::readUsersInfo()
{
    creatUsersInfoFolder();

    QFile  usersfile(".//connect//users.xml");
    if (usersfile.exists())
    {
        if (!usersfile.open(QFile::ReadOnly | QFile::Text))
        {
            return false;
        }

        QDomDocument doc;
        if(!(doc.setContent(&usersfile)))
        {
            usersfile.close();
            return false;
        }
        usersfile.close();

        QDomElement users = doc.documentElement();
        QDomNode node = users.firstChild();

        /*用户信息*/
        DesEncryptionModel  model;
        QString userName = "";
        QString password = "";
        int i = 0;
        m_userParm.clear();
        while (node.toElement().tagName() == ("user" + QString::number(i)))
        {
            UserParameter  userparm;
            QDomNodeList list = node.childNodes();
            int listNum = 0;
            if (list.size() >= 3)
            {
                if (list.at(listNum).toElement().tagName() == "userName")
                {
                    userName = list.at(listNum++).toElement().text();
                    userparm.m_userName = model.Des_Run(userName, DesEncryptionModel::DECRYPT);//解密
                    //userparm.m_userName.chop(1);
                }
                else
                {
                    return false;
                }

                if (list.at(listNum).toElement().tagName() == "password")
                {
                    password = list.at(listNum++).toElement().text();
                    password = model.Des_Run(password, DesEncryptionModel::DECRYPT);//解密
                    userparm.m_password = password.left(8);
                }
                else
                {
                    return false;
                }

                if (list.at(listNum).toElement().tagName() == "permission")
                {
                    unsigned int temp = list.at(listNum++).toElement().text().toInt();
                    if (temp <= 2)
                    {
                        userparm.m_permission = (UserPermission)temp;
                    }
                    else
                    {
                        return false;
                    }
                }
                else
                {
                    return false;
                }
            }
            else
            {
                return false;
            }

            m_userParm.append(userparm);
            node = node.nextSibling();
            i++;
        }

        if (i <= 0)
        {
            return false;
        }
        return true;
    }
    else
    {
        return restoreUsersInfo();
    }
}

bool CLoginDialog::checkUserName(QString username, UserPermission permission)
{
    for (int i = 0; i < m_userParm.size(); i++)
    {
        if (permission == m_userParm.at(i).m_permission)
        {
            if (username == m_userParm.at(i).m_userName)
            {
                return true;
            }
        }
    }
    return false;
}

bool CLoginDialog::checkUserPassword(QString username, QString password, UserPermission permission)
{
    for (int i = 0; i < m_userParm.size(); i++)
    {
        if (permission == m_userParm.at(i).m_permission)
        {
            if (username == m_userParm.at(i).m_userName)
            {
                if (password == m_userParm.at(i).m_password)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

//UserPermission CLoginDialog::getUserPermission(QString username)
//{
//    DesEncryptionModel  model;
//    QString userName = model.Des_Run(username, DesEncryptionModel::ENCRYPT);

//    for (int i = 0; i < m_userParm.size(); i++)
//    {
//        if (userName == m_userParm.at(i).m_userName)
//        {
//            return m_userParm.at(i).m_permission;
//        }
//    }

//    return ORDINARY_USER;
//}

UserParameter CLoginDialog::getUserInfo()
{
    for (int i = 0; i < m_userParm.size(); i++)
    {
        if (m_permission == m_userParm.at(i).m_permission)
        {
            if (m_sUserName == m_userParm.at(i).m_userName)
            {
                return m_userParm.at(i);
            }
        }
    }
    return m_userParm.at(0);
}

void CLoginDialog::creatUsersInfoFolder()
{
    QDir data;
    if (!data.exists("./data"))
    {
        data.mkdir("./data");
    }

    QDir connect;
    if (!connect.exists(".//connect"))
    {
        connect.mkdir(".//connect");
    }
}
