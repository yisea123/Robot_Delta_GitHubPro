#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QButtonGroup>
#include <QListWidget>
#include <QVector>
#include <QLabel>
#include <QMenu>
#include <QAction>

#include "ComonTypes.h"

using std::vector;

namespace Ui {
class MainWindow;
}

class  SystemSchedule;
class  UserParameter;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
//    explicit MainWindow(QWidget *parent = 0);
    explicit MainWindow(UserParameter  parm, QWidget *parent = 0);
    ~MainWindow();

    void createDisplayUIs(QList<InfoUI *> &widgets);
    void showDisplayUIs(QList<InfoUI *> &widgets, UserParameter parm);

    void writeMsgToStatusBar(QString msg, int level);
    void writeMsgToStatusBar(QString msg);


signals:

public slots:
    void updataData();
signals:
//    void sendMsgToSchedule(QString msg);

private slots:
    void goToUiPage(int page);
    void setCurrentUiPage(int id);
//    void currentMenuChanged(int row);
    void stop_clicked();


    void on_enable_clicked(bool checked);
    void on_mode_clicked(bool checked);
    void on_axis1pos_pressed();

    void on_axis1neg_pressed();

    void on_axis2pos_pressed();

    void on_axis2neg_pressed();

    void on_axis3pos_pressed();

    void on_axis3neg_pressed();

    void on_axis4pos_pressed();

    void on_axis4neg_pressed();

    void on_reset_clicked();

    void on_horizontalSlider_valueChanged(int value);

    void on_clear_clicked();

    void on_setorian_clicked();

    void on_start_clicked();

    void on_cyclerun_clicked(bool checked);

	void on_movpos_clicked(bool checked);

	void on_axis1pos_released();
	void on_axis1neg_released();
	void on_axis2pos_released();
	void on_axis2neg_released();
	void on_axis3pos_released();
	void on_axis3neg_released();
	void on_axis4pos_released();
	void on_axis4neg_released();
	void on_carcoor_clicked(bool checked);
	
	

    void on_stop_clicked();

    void on_hold_clicked(bool checked);

    void on_backzero_clicked();

    void on_axis5neg_pressed();

    void on_axis5pos_pressed();

    void on_axis6neg_pressed();

    void on_axis6pos_pressed();

    void on_axis5neg_released();

    void on_axis5pos_released();

    void on_axis6neg_released();

    void on_axis6pos_released();

private:
    void setupMainMenuGroup();
    void createMenuList();
    void connectMenuWithUi(QList<InfoUI *> &widgets);
    void addMenuButton(int nType, QString name,int id);
//    void addMenuText(int nType, QString name);

//    void setDefaultMenuSelection();

    void createHelpText();

    void createStatusBar();

private:
    Ui::MainWindow   *ui;
    QLabel *messageLabel;
    QMenu *homeMenu;
    QAction *minimizedAction;
    QAction *maximizedAction;
    QAction *quitAction;
    QAction *aboutAction;
    QAction *helpAction;
    SystemSchedule   *SystemScheduler;      /**< 系统指锟斤拷 */

    QButtonGroup *pMainMenuGroup;
//    QList<QListWidget *> listWidgets;
//    QVector< QVector<int>* > menus;
    vector<QButtonGroup *>sonMenuGrp; // 页面按键 组(每个界面下对应一个)
    vector<int> sonMenuLastID;  // 最后按下的 页面ID

    int nCurrentPage;
    int movemode;
    int movecoor;
    int cyclerun;
    int enable;
    int mode;
};

#endif // MAINWINDOW_H
