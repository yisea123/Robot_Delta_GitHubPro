#ifndef CFRMUSERLEVEL_H
#define CFRMUSERLEVEL_H

#include <QWidget>
#include "ccommmomuiwidget.h"

class  SystemSchedule;

namespace Ui {
class CFrmUserLevel;
}

class CFrmUserLevel : public CCommmomUIWidget
{
    Q_OBJECT

public:
    explicit CFrmUserLevel(QWidget *parent = 0);
    CFrmUserLevel(QWidget *parent, SystemSchedule* schedule);
    ~CFrmUserLevel();

private slots:

    void on_btnOK_clicked();

    void on_ckbCollectMotionData_clicked(bool checked);

    void on_btnModify_clicked();

    void on_edtUserPsw_returnPressed();

    void on_edtModityPsw2_returnPressed();

private:
    Ui::CFrmUserLevel *ui;
    SystemSchedule*       m_pScheduler;
};

#endif //
