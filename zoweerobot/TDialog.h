#ifndef TDIALOG_H
#define TDIALOG_H

#include <QDialog>

namespace Ui {
class TDialog;
}

class TDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TDialog(QWidget *parent = 0);
    ~TDialog();
    void UpdateProgressBar(int progress);

private:
    Ui::TDialog *ui;
};

#endif // TDIALOG_H
