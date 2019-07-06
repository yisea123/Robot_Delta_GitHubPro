#include "CUserLevelManage.h"
#include <QtXml>
#include <QFile>
#include <QDataStream>
#include "DesEncryptionModel.h"

UserParameter CUserLevelManage::m_OptUser[USERLEVELNUM];
int CUserLevelManage::m_CurLevel = 0;

CUserLevelManage::CUserLevelManage(QObject *parent) : QObject(parent)
{

}

int CUserLevelManage::ModifyUserLevel(int level)
{
    if(level<ORDINARY_USER || level>SUPER_USER){
        level = ORDINARY_USER;
    }
    m_CurLevel = level;

    return 0;
}

void CUserLevelManage::InitUserLevelManage()
{
    m_OptUser[0].m_userName = "User1";
    m_OptUser[0].m_password = "1234";
    m_OptUser[0].m_permission = ORDINARY_USER;

    m_OptUser[1].m_userName = "User2";
    m_OptUser[1].m_password = "123456";
    m_OptUser[1].m_permission = ADVANCED_USER;

    m_OptUser[2].m_userName = "User3";
    m_OptUser[2].m_password = "12345678";
    m_OptUser[2].m_permission = SUPER_USER;

    m_OptUser[3].m_userName = "User4";
    m_OptUser[3].m_password = "99999999"; // 系统内部
    m_OptUser[3].m_permission = SUPER_USER;    
}

// 检查密码
// psw:密码.
// 返回 2:非法密码 -1:长度错误 0:ORDINARY_USER 1:ADVANCED_USER 2:SUPER_USER
int CUserLevelManage::CheckUserPassword(QString psw)
{
    int i, len;

    len = psw.length();
    if(len!=4 && len!=6 && len!=8 && len!=9){
        return -1;
    }
    for(i=0;i<USERLEVELNUM;i++){
        if(psw==m_OptUser[i].m_password){
            return m_OptUser[i].m_permission;
        }
    }
    return -2;
}

// 更改密码
// curLevel: 当前级别
// psw: 修改密码
// 返回 0:成功 1:级别数错误 2:密码长度不匹配 3:输入非法字符
int CUserLevelManage::ModifyUserPassword(int curLevel,QString psw)
{
    int i, len,len2;
    const char *str=NULL;

    if(curLevel<0 || curLevel>SUPER_USER){
        return 1;
    }
    len = psw.length();
    len2 = m_OptUser[curLevel].m_password.length();
    if(len != len2){
        return 2;
    }

    str = m_OptUser[curLevel].m_password.toStdString().c_str();
    for(i=0;i<len;i++){
        if(str[i]<=32 || str[i]>126){
            return 3;
        }
    }
    m_OptUser[curLevel].m_password = psw;
    SaveUsersInfo();

    return 0;
}

int CUserLevelManage::SaveUsersInfo()
{
    int cnt =USERLEVELNUM;
    QDir data("./data");

    if (!data.exists()){
        if(!data.mkdir("./data")){
            return 1; // 写入文件失败
        }
    }

    QDomDocument doc;
    QDomProcessingInstruction  instruction;
    instruction = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(instruction);

    QDomElement root = doc.createElement(QString::fromLocal8Bit("XX系统权限信息"));
    doc.appendChild(root);

    QDomElement users;
    QDomText text;
    QDomElement userInfo;
    DesEncryptionModel  model;
    QString userName = "";
    QString password = "";
    for(int i = 0; i < cnt; i++)
    {
        // 存储数据前先加密
        //userName = model.Des_Run(m_OptUser[i].m_userName, DesEncryptionModel::ENCRYPT);
        password = model.Des_Run(m_OptUser[i].m_password, DesEncryptionModel::ENCRYPT);
        userName = m_OptUser[i].m_userName;
        //password = m_OptUser[i].m_password;

        users = doc.createElement("Level-" + QString::number(i));

        userInfo = doc.createElement("UserName");
        text = doc.createTextNode(userName);
        userInfo.appendChild(text);
        users.appendChild(userInfo);

        userInfo = doc.createElement("password");
        text = doc.createTextNode(password);
        userInfo.appendChild(text);
        users.appendChild(userInfo);

        userInfo = doc.createElement("permission");
        text = doc.createTextNode(QString::number(m_OptUser[i].m_permission));
        userInfo.appendChild(text);
        users.appendChild(userInfo);

        root.appendChild(users);
    }

    QFile file(".//data//UserLevel.xml");

    if (!file.open(QIODevice::ReadWrite | QIODevice::Truncate)){
        return 2;
    }
    QTextStream out(&file);
    doc.save(out, 4);
    file.close();

    return 0;
}


// 读取 用户权限
// 返回(0:成功 1:目录不存在 2:文件不存在 3:内容为空 4:格式错误)
int CUserLevelManage::ReadUsersInfo()
{
    int i, cnt =USERLEVELNUM;
    UserParameter level[USERLEVELNUM];
    QFile  usersfile(".//data//UserLevel.xml");

    if (!usersfile.exists()){
        return 1;  //
    }

    if (!usersfile.open(QFile::ReadOnly | QFile::Text)){
        return 2;
    }

    QDomDocument doc;
    if(!(doc.setContent(&usersfile))){
        usersfile.close();
        return 3;
    }
    usersfile.close();

    QDomElement users = doc.documentElement();
    cnt = users.childNodes().count();
    if(cnt != USERLEVELNUM){
        return 4;
    }
    QDomNode node = users.firstChild();

    /*用户信息*/
    DesEncryptionModel  model;
    QString userName = "";
    QString password = "";
    QDomNodeList list;
    unsigned int temp;
    for(i=0;i<USERLEVELNUM;i++){
        level[i] = m_OptUser[i];
    }
    i = 0;    
    while (node.toElement().tagName()==("Level-" + QString::number(i)) && i<=4){        
        list = node.childNodes();
        //level[i]
        if (list.size() >= 3){
            if (list.at(0).toElement().tagName() == "UserName"){
                userName = list.at(0).toElement().text();
                //level[i].m_userName = model.Des_Run(userName, DesEncryptionModel::DECRYPT);//解密
                level[i].m_userName = userName;
            }else{
                return 4;
            }

            if (list.at(1).toElement().tagName() == "password"){
                password = list.at(1).toElement().text();
                password = model.Des_Run(password, DesEncryptionModel::DECRYPT);//解密
                level[i].m_password = password.left(8);
            }else{
                return 4;
            }

            if (list.at(2).toElement().tagName() == "permission"){
                temp = list.at(2).toElement().text().toInt();
                if (temp <= SUPER_USER){
                    level[i].m_permission = (UserPermission)temp;
                }else{
                    return 4;
                }
            }else{
                return 4;
            }
        }
        else{
            return 4;
        }
        node = node.nextSibling();
        i++;
    }

    if (i <= 0){
        return 4;
    }
    
    for(i=0;i<USERLEVELNUM;i++){
        m_OptUser[i] = level[i];
    }
    
    return 0;

}




