#ifndef MYPROGRESSBAR_H
#define MYPROGRESSBAR_H

#include <QWidget>

namespace Ui {
class MyProgressBar;
}

//自制上传和下载的进度条控件
class MyProgressBar : public QWidget
{
    Q_OBJECT

public:
    explicit MyProgressBar(QWidget *parent = nullptr);
    ~MyProgressBar();

    void setFileName(QString file="test");  //设置文件名
    void setValueProgress(int value=0,int max=100);  //当前值0,最大值100
private:
    Ui::MyProgressBar *ui;
};

#endif // MYPROGRESSBAR_H
