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
    virtual void UpdateViewData(void);
    virtual void TimerUpdateViewData(void);

private slots:
    void updataData();
    void SetEditCtrlReadOnly();
    void on_axisParamRDBtn_clicked();
    void on_axisParamSV_clicked();


//    void on_routeParamRDBtn_clicked();
//    void on_routeParamSVBtn_clicked();

private:
    Ui::cjointparamsetui *ui;
    SystemSchedule*       m_pScheduler;

     void refresh_systemParam();
};

#endif // CJOINTPARAMSETUI_H
