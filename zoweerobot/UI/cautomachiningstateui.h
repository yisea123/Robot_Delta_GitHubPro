#ifndef CAUTOMACHININGSTATEUI_H
#define CAUTOMACHININGSTATEUI_H

#include "ccommmomuiwidget.h"
#include <QWidget>
#include <QMutex>
#include "UI/cautomachiningstateui.h"
#include "UI/cemulationcanvs.h"

class  SystemSchedule;

namespace Ui {
class CAutoMachiningStateUi;
}

class CAutoMachiningStateUi : public CCommmomUIWidget
{
    Q_OBJECT
    
public:

    typedef enum  _MotionRunStatue//ϵͳ״̬
    {
        MOTION_EMULATION_IDLE,
        EMULATION_RUN,
        EMULATION_PAUSE,
        EMULATION_RUN_TO_BREAKPOINT,
        EMULATION_BREAKPOINT_PAUSE,
        MOTION_EMULATION_RUN,
        MOTION_EMULATION_PAUSE,
        MOTION_RUN_TO_BREAKPOINT,
        MOTION_BREAKPOINT_PAUSE,
        MOTION_UNCONNECT_NET,
        MOTION_STEP_RUN,
        MOTION_RECONNECT_NET
    }MotionRunStatue;

    explicit CAutoMachiningStateUi(QWidget *parent = 0);
    CAutoMachiningStateUi(QWidget *parent, SystemSchedule* schedule);
    ~CAutoMachiningStateUi();

    CEmulationCanvs* getCanvsHandle();

    void setProcessBarValue(int v);

    void setIsEmulationRunning(MotionRunStatue b);
    void setCurrentNGCodeLine(int lineNumber);
    void setPostext(DAxis axis);
    void setToolText(QString toolType, int toolNum);
    int getBreakpoint();

    void updateAllOutputBnt(unsigned char* a, int n);
    void updateOutputBnt(int index, bool sts);

    virtual void setDisableEdit(bool f);

    void setClampPos(Point axis1, Point axis2);
    
private slots:
    void updataData();

    void on_openfile_clicked();

    void on_savefile_clicked();

    void on_createfile_clicked();

    void on_editfile_clicked(bool checked);

    void on_addtechpoint_clicked();

private:
    void  init();
    void  readNGFile(QString f);
    void  showFileContext(QString f);

    void motionRun();
    void motionPause();
    void motionStop();

    bool getIOStatus(unsigned char* a, int bit);


private:
    Ui::CAutoMachiningStateUi *ui;
    SystemSchedule*       m_pScheduler;
    MotionRunStatue    m_bIsEmulationRunning;

    int m_nProcessValue;
    int m_nToolID;
    QString m_sToolType;
    DAxis  m_dAxis;
    int  m_nLineNumber;
    QMutex mutex;
    bool m_bIsNetConnect;
};

#endif // CAUTOMACHININGSTATEUI_H


