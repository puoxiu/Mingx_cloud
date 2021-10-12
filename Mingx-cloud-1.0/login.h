#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QPaintEvent>
#include <QMouseEvent>
#include "common/common.h"
#include "mainwindow.h"

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

protected:
    void paintEvent(QPaintEvent *e);

private slots:
    void on_registerBtn_clicked();
    void on_registerBtn_2_clicked();
    void on_setSave_clicked();
    void on_setReturn_clicked();
    void on_regReturn_clicked();
    void on_loginBtn_clicked();

private:
    // 读取配置信息，设置默认登录状态，默认设置信息--初始化登陆界面
    void readCfg();
    //保存配置文件信息--在common类中封装writeWebInfo方法
    //void saveWebConf(QString ip,QString port,QString path);

    //注册信息 -->Json
    QByteArray setRegisterJson(QString user,QString pwd,QString phone,QString mail);
    //登录信息 -->Json
    QByteArray setLoginJson(QString user,QString pwd);

private:
    Ui::Login *ui;

    MainWindow* m_mainWin;
    Common m_com;
};

#endif // LOGIN_H
