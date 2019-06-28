#ifndef CCOMMMOMUIWIDGET_H
#define CCOMMMOMUIWIDGET_H

#include <QWidget>

class CCommmomUIWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CCommmomUIWidget(QWidget *parent = 0);

    virtual void setDisableEdit(bool f);
    
signals:
    
public slots:
    
};

#endif // CCOMMMOMUIWIDGET_H
