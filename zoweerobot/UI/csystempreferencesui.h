#ifndef CSYSTEMPREFERENCESUI_H
#define CSYSTEMPREFERENCESUI_H

#include "ccommmomuiwidget.h"

class  SystemParameter;
class  SystemSchedule;

namespace Ui {
class CSystemPreferencesUi;
}

class CSystemPreferencesUi : public CCommmomUIWidget
{
    Q_OBJECT
    
public:
    explicit CSystemPreferencesUi(QWidget *parent = 0);
    CSystemPreferencesUi(QWidget *parent, SystemSchedule* schedule);
    ~CSystemPreferencesUi();
    virtual void TimerUpdateViewData(void);

    void init();
    
private slots:
    void updataData();
   
    void on_getversion_clicked();

    void on_getbattery_clicked();

    void on_setbattery_clicked();

private:
    Ui::CSystemPreferencesUi *ui;
    SystemSchedule*       m_pScheduler;
};

#endif // CSYSTEMPREFERENCESUI_H
