#ifndef CMANUALMACHININGSTATEUI_H
#define CMANUALMACHININGSTATEUI_H

#include "ccommmomuiwidget.h"
#include <QMutex>
#include "cmanualmachiningstateui.h"
#include  "GlobalVariableType.h"

class  SystemSchedule;

namespace Ui {
class CManualMachiningStateUi;
}

class CManualMachiningStateUi : public CCommmomUIWidget
{
    Q_OBJECT
    
public:
    explicit CManualMachiningStateUi(QWidget *parent = 0);
    CManualMachiningStateUi(QWidget *parent, SystemSchedule* schedule);
    ~CManualMachiningStateUi();

 /*   void setPostext(DAxis axis);
    void setToolText(QString toolType, int toolNum);

    void updateAllOutputBnt(unsigned char* a, int n);
    void updateOutputBnt(int index, bool sts);

    void setClampPos(Point axis1, Point axis2);

private:
    bool getIOStatus(unsigned char* a, int bit);*/
    
private slots:


    void updataData();


    void on_O1_0_stateChanged(int arg1);
    void on_O1_1_stateChanged(int arg1);
    void on_O1_2_stateChanged(int arg1);
    void on_O1_3_stateChanged(int arg1);
    void on_O1_4_stateChanged(int arg1);
    void on_O1_5_stateChanged(int arg1);
    void on_O1_6_stateChanged(int arg1);
    void on_O1_7_stateChanged(int arg1);
    void on_O1_8_stateChanged(int arg1);
    void on_O1_9_stateChanged(int arg1);
    void on_O1_10_stateChanged(int arg1);
    void on_O1_11_stateChanged(int arg1);
    void on_O1_12_stateChanged(int arg1);
    void on_O1_13_stateChanged(int arg1);
    void on_O1_14_stateChanged(int arg1);
    void on_O1_15_stateChanged(int arg1);
    void on_O1_16_stateChanged(int arg1);
    void on_O1_17_stateChanged(int arg1);
    void on_O1_18_stateChanged(int arg1);
    void on_O1_19_stateChanged(int arg1);
    void on_O1_20_stateChanged(int arg1);
    void on_O1_21_stateChanged(int arg1);
    void on_O1_22_stateChanged(int arg1);
    void on_O1_23_stateChanged(int arg1);
    void on_O1_24_stateChanged(int arg1);
    void on_O1_25_stateChanged(int arg1);
    void on_O1_26_stateChanged(int arg1);
    void on_O1_27_stateChanged(int arg1);
    void on_O1_28_stateChanged(int arg1);
    void on_O1_29_stateChanged(int arg1);
    void on_O1_30_stateChanged(int arg1);
    void on_O1_31_stateChanged(int arg1);
    void on_O1_32_stateChanged(int arg1);
    void on_O1_33_stateChanged(int arg1);
    void on_O1_34_stateChanged(int arg1);
    void on_O1_35_stateChanged(int arg1);
    void on_O1_36_stateChanged(int arg1);
    void on_O1_37_stateChanged(int arg1);
    void on_O1_38_stateChanged(int arg1);
    void on_O1_39_stateChanged(int arg1);
    void on_O1_40_stateChanged(int arg1);
    void on_O1_41_stateChanged(int arg1);
    void on_O1_42_stateChanged(int arg1);
    void on_O1_43_stateChanged(int arg1);
    void on_O1_44_stateChanged(int arg1);
    void on_O1_45_stateChanged(int arg1);
    void on_O1_46_stateChanged(int arg1);
    void on_O1_47_stateChanged(int arg1);

private:
    Ui::CManualMachiningStateUi *ui;
    SystemSchedule*       m_pScheduler;

    QMutex  mutex;
    DAxis  m_tempAxis;
    QString  m_tempToolType;
    int m_tempToolNum;
};

#endif // CMANUALMACHININGSTATEUI_H



