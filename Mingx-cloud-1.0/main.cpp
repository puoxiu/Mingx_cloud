#include "mainwindow.h"
#include "login.h"
#include "tittlewg.h"
#include "tittleMainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
    //      TittleMainwindow t;
    //      t.show();

    Login l;
    l.show();   //开始显示登录页面



    return a.exec();
}
