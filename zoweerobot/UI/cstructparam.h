#ifndef CSTRUCTPARAM_H
#define CSTRUCTPARAM_H

//#include <QWidget>
#include "ccommmomuiwidget.h"
class  SystemParameter;
class  SystemSchedule;

namespace Ui {
class cstructparam;
}

class cstructparam : public CCommmomUIWidget
{
    Q_OBJECT

public:
    explicit cstructparam(QWidget *parent = 0);
    cstructparam(QWidget *parent, SystemSchedule* schedule);
    ~cstructparam();
    virtual void UpdateViewData(void);
    virtual void TimerUpdateViewData(void);

private slots:
    void updataData();
    void SetEditCtrlReadOnly();

    void on_readBtn_clicked();

    void on_saveBtn_clicked();

    void on_restoreParam_clicked();

private:
    Ui::cstructparam *ui;
    SystemSchedule*       m_pScheduler;

     void refresh_structParam();
};

#endif // CJOINTPARAMSETUI_H
