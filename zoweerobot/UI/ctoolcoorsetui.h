#ifndef CTOOLCOORSETUI_H
#define CTOOLCOORSETUI_H

//#include <QWidget>
#include "ccommmomuiwidget.h"
class  SystemParameter;
class  SystemSchedule;

namespace Ui {
class CToolCoorSetUi;
}

class CToolCoorSetUi : public CCommmomUIWidget
{
    Q_OBJECT

public:
    explicit CToolCoorSetUi(QWidget *parent = 0);
    CToolCoorSetUi(QWidget *parent, SystemSchedule* schedule);
    ~CToolCoorSetUi();


private:
    Ui::CToolCoorSetUi *ui;
    SystemSchedule*       m_pScheduler;
    int stepnum[20];
    int m_curtoolindex;
private slots:
    void updataData();
   /* void on_biaoding_clicked();

    void on_currenttool_valueChanged(int arg1);

    void on_zerostep1_clicked();

    void on_zerostep2_clicked();

    void on_zerostep3_clicked();

    void on_zerostep4_clicked();*/

    void on_setzeroaxis1_clicked();

    void on_setzeroaxis2_clicked();

    void on_setzeroaxis3_clicked();

    void on_setzeroaxis4_clicked();

    void on_setzeroaxis5_clicked();

    void on_setzeroaxis6_clicked();

private:
    void  init();
	
};

#endif // CTOOLCOORSETUI_H

