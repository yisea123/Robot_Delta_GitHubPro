#ifndef CCOMMMOMUIWIDGET_H
#define CCOMMMOMUIWIDGET_H

#include <QWidget>

class CCommmomUIWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CCommmomUIWidget(QWidget *parent = 0);

    virtual void setDisableEdit(bool f);
    virtual void UpdateViewData(void);  // ���� ȫ������
    virtual void TimerUpdateViewData(void);  // ��ʱ���µ�����
    
signals:
    
public slots:
    
};

#endif // CCOMMMOMUIWIDGET_H
