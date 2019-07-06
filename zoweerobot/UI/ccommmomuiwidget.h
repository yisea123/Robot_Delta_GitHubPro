#ifndef CCOMMMOMUIWIDGET_H
#define CCOMMMOMUIWIDGET_H

#include <QWidget>

class CCommmomUIWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CCommmomUIWidget(QWidget *parent = 0);

    virtual void setDisableEdit(bool f);
    virtual void UpdateViewData(void);  // 更新 全部数据
    virtual void TimerUpdateViewData(void);  // 定时更新的数据
    
signals:
    
public slots:
    
};

#endif // CCOMMMOMUIWIDGET_H
