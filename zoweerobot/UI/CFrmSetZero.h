#ifndef CFRMSETZERO_H
#define CFRMSETZERO_H

#include <QWidget>
#include "ccommmomuiwidget.h"

class  SystemSchedule;

namespace Ui {
class CFrmSetZero;
}

class CFrmSetZero : public CCommmomUIWidget
{
    Q_OBJECT

public:
    explicit CFrmSetZero(QWidget *parent = 0);
    CFrmSetZero(QWidget *parent, SystemSchedule* schedule);
    ~CFrmSetZero();

    virtual void UpdateViewData(void);
    virtual void TimerUpdateViewData(void);  // 定时更新的数据

private slots:

    void on_setzeroaxis1_clicked();

    void on_setzeroaxis2_clicked();

    void on_setzeroaxis3_clicked();

    void on_setzeroaxis4_clicked();

    void on_setzeroaxis5_clicked();

    void on_setzeroaxis6_clicked();

    void on_setzeroaxis7_clicked();

private:

private:
    Ui::CFrmSetZero *ui;
    SystemSchedule*       m_pScheduler;
};

#endif // CFRMSETZERO_H
