#ifndef CMANUALMACHININGSTATEUI_H
#define CMANUALMACHININGSTATEUI_H

#include "ccommmomuiwidget.h"
#include <QMutex>
#include <QIcon>
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
    virtual void TimerUpdateViewData(void);

 /*   void setPostext(DAxis axis);
    void setToolText(QString toolType, int toolNum);

    void updateAllOutputBnt(unsigned char* a, int n);
    void updateOutputBnt(int index, bool sts);

    void setClampPos(Point axis1, Point axis2);

private:
    bool getIOStatus(unsigned char* a, int bit);*/

private:
    bool InSignalIsvalid(int num,int bit);
    void SetOutSignal(int num,int bit,unsigned char val);
    unsigned char GetOutSignal(int num,int bit);

    QIcon m_IconOn;
    QIcon m_IconOff;

private slots:
    void updataData();

    void on_ckbOut_clicked(bool checked);

private:
    Ui::CManualMachiningStateUi *ui;
    SystemSchedule*       m_pScheduler;

    QMutex  mutex;
    DAxis  m_tempAxis;
    QString  m_tempToolType;
    int m_tempToolNum;
    unsigned char m_InVol; // 输入信号接通时为(0:低 1:高)电平
};

#endif // CMANUALMACHININGSTATEUI_H




