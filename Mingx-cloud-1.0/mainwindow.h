#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "common/common.h"
#include <QPaintEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void showMainWindow();  // 显示主窗口
    void processAllSignals();//处理所有的信号(在构造函数中调用)
    void loginAgain(); //重新登录

signals:
    void sigChangeUser();   //用户切换信号
    void setUser(QString user);

protected:
    void paintEvent(QPaintEvent* event);

private:
    Ui::MainWindow *ui;

    Common m_com;
};
#endif // MAINWINDOW_H
