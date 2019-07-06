#ifndef CUSERLEVELMANAGE_H
#define CUSERLEVELMANAGE_H

#include <QObject>
#include "GlobalVariableType.h"

#define USERLEVELNUM 4   // 4个用户(0:普通 1:高级 2:超级 3:超级)
class CUserLevelManage : public QObject
{
    Q_OBJECT
public:
    explicit CUserLevelManage(QObject *parent = nullptr);

    static int ModifyUserLevel(int level);
    static void InitUserLevelManage();
    static int SaveUsersInfo();
    static int ReadUsersInfo();

    static int CheckUserPassword(QString psw);
    static int ModifyUserPassword(int curLevel,QString psw);

signals:

public slots:

public:
    static UserParameter m_OptUser[USERLEVELNUM];
    static int m_CurLevel;

private:


};

#endif // CUSERLEVELMANAGE_H
