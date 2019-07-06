#ifndef CUSERCOORSETUI_H
#define CUSERCOORSETUI_H

//#include <QWidget>
#include "ccommmomuiwidget.h"
#include "GlobalDefine.h"
class  SystemParameter;
class  SystemSchedule;


namespace Ui {
class cusercoorsetui;
}

class cusercoorsetui : public CCommmomUIWidget
{
    Q_OBJECT

public:
    explicit cusercoorsetui(QWidget *parent = 0);
    cusercoorsetui(QWidget *parent, SystemSchedule* schedule);
    ~cusercoorsetui();

    virtual void TimerUpdateViewData(void);

private:
    Ui::cusercoorsetui *ui;
    SystemSchedule*       m_pScheduler;
    int stepnum[20];
    int m_userindex;
    CartesianPose m_user1;
    CartesianPose m_user2;
    CartesianPose m_cpos1;
    CartesianPose m_cpos2;
    CartesianPose m_usertrans;
private slots:
    void updataData();
    void on_biaoding1_clicked();
    void on_biaoding2_clicked();
	
private:
    void  init();
    int GetUserTrans(IN  CartesianPose* i_user1,IN  CartesianPose*  i_user2,IN  CartesianPose* i_CPos_1, IN  CartesianPose* i_CPos_2, 
	      OUT CartesianPose* i_usertrans);
};

#endif // CUSERCOORSETUI_H
