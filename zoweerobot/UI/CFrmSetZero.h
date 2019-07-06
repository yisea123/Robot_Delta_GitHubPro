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
    void on_zerostep1_clicked();

    void on_zerostep2_clicked();

    void on_zerostep3_clicked();

    void on_zerostep4_clicked();

    void on_edtHomeOrder1_editingFinished();

    void on_edtHomeOrder2_editingFinished();

    void on_edtHomeOrder3_editingFinished();

    void on_edtHomeOrder4_editingFinished();

    void on_btnHomeOrder_clicked();

private:
    void on_edtHomeOrder_editingFinished();
    int IsHomeOrderValid(int &line);

private:
    Ui::CFrmSetZero *ui;
    SystemSchedule*       m_pScheduler;
    int m_PreHomeOrder[4];
};

#endif // CFRMSETZERO_H
