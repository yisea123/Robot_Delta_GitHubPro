#ifndef CTEACHPOINTUI_H
#define CTEACHPOINTUI_H

#include "ccommmomuiwidget.h"
#include <QMutex>
#include  "GlobalVariableType.h"
#include <QStandardItemModel>
#include <QVariant>
#include "TDialog.h"
class  SystemSchedule;

namespace Ui {
class cteachpointui;
}

class cteachpointui :  public CCommmomUIWidget
{
    Q_OBJECT

public:
    explicit cteachpointui(QWidget *parent = 0);
    cteachpointui(QWidget *parent, SystemSchedule* schedule);
    ~cteachpointui();
    virtual void TimerUpdateViewData(void);

    void init();
private:
	void stop_clicked();
private slots:


    void updataData();
	
    void on_openteachpoint_clicked();

    void on_addteachpoint_clicked();

    void on_saveteachpoint_clicked();

    void on_deleteteachpoint_clicked();

    void on_getteachpoint_clicked();
    void on_movetopoint_pressed();
    void on_movetopoint_released();

    void on_openuserfile_clicked();

    void on_downuserfile_clicked();

private:
    Ui::cteachpointui *ui;
    SystemSchedule*       m_pScheduler;
    QStandardItemModel* model;
    QList<QStandardItem*> newlist;
    QList<QStandardItem*> oldlist;
    int m_idisplaycnt;
    TDialog * m_pDialog;
    bool m_Saving;
};

#endif // CTEACHPOINTUI_H
