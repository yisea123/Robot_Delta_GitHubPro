#include "TDialog.h"
#include "ui_TDialog.h"

TDialog::TDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TDialog)
{
    Qt::WindowType type;

    type = windowType();
    setWindowFlags(type|Qt::FramelessWindowHint);
    ui->setupUi(this);
}

TDialog::~TDialog()
{
    delete ui;
}

void TDialog::UpdateProgressBar(int progress)
{
    ui->progressBar->setValue(progress);
    ui->label->setText(QString::fromLocal8Bit("ДІРэжа...")+progress+"%");
}

