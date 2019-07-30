#ifndef CPIDPARAMUI_H
#define CPIDPARAMUI_H

//#include <QWidget>
#include <qlineedit.h>
#include <QStandardItemModel>
#include "ccommmomuiwidget.h"
class  SystemParameter;
class  SystemSchedule;

namespace Ui {
class cpidparamui;
}

class cpidparamui : public CCommmomUIWidget
{
    Q_OBJECT

public:
    explicit cpidparamui(QWidget *parent = 0);
      cpidparamui(QWidget *parent, SystemSchedule* schedule);
    ~cpidparamui();
    virtual void UpdateViewData(void);
    virtual void TimerUpdateViewData(void);

private slots:
    void updataData();

    void on_axis1readpid_clicked();

    void on_axis2readpid_clicked();

    void on_axis3readpid_clicked();

    void on_axis4readpid_clicked();

    void on_axis5readpid_clicked();

    void on_axis6readpid_clicked();

    void on_axis7readpid_clicked();

    void on_axis1setpid_clicked();

    void on_axis2setpid_clicked();

    void on_axis3setpid_clicked();

    void on_axis4setpid_clicked();

    void on_axis5setpid_clicked();

    void on_axis6setpid_clicked();

    void on_axis7setpid_clicked();

	void on_setcandebug_clicked();


    void on_btnAsSave1_clicked();
    void on_cmbParaGrp_currentIndexChanged(int index);

    void on_savePid_1_clicked();

    void on_savePid_2_clicked();

    void on_savePid_3_clicked();

    void on_savePid_4_clicked();

    void on_savePid_5_clicked();

    void on_savePid_6_clicked();

    void on_savePid_7_clicked();

    void on_savePid_all_clicked();

    void on_extDevSVBtn_clicked();

    void on_extDevRDBtn_clicked();

private:
    Ui::cpidparamui *ui;
    SystemSchedule*       m_pScheduler;
    QStandardItemModel * m_pModel;

    void UpdateTbvParaGrp();
    void SetCtrlEnabled();

	void refresh_LineParam();
     void refresh_pidParam(int axis);
     void ReadGrpPara(int grp);

     bool CheckEditParaVal(QLineEdit *edt, int &newVal);
     bool CheckAllEditParaVal(int val[]);
     void on_btnAsSaveGrp_clicked(int grp);
     void on_btnReadGrp_clicked(int grp);

     void refresh_extDevParam();
};

#endif // CPIDPARAMUI_H
