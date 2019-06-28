#ifndef CJOINTPARAMSETUI_H
#define CJOINTPARAMSETUI_H

//#include <QWidget>
#include "ccommmomuiwidget.h"
class  SystemParameter;
class  SystemSchedule;

namespace Ui {
class cjointparamsetui;
}

class cjointparamsetui : public CCommmomUIWidget
{
    Q_OBJECT

public:
    explicit cjointparamsetui(QWidget *parent = 0);
    cjointparamsetui(QWidget *parent, SystemSchedule* schedule);
    ~cjointparamsetui();

private slots:
    void updataData();
    void on_ok_clicked();

    void on_cancle_clicked();
    void on_setlinkparam_clicked();
    void on_renewlinkparam_clicked();
     
    void on_axisetbutton_clicked();
    void on_axigetbutton_clicked();
private:
    Ui::cjointparamsetui *ui;
    SystemSchedule*       m_pScheduler;

     void refresh_systemParam();
};

#endif // CJOINTPARAMSETUI_H
