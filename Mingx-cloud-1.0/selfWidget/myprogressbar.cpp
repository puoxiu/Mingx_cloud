#include "myprogressbar.h"
#include "ui_myprogressbar.h"

MyProgressBar::MyProgressBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyProgressBar)
{
    ui->setupUi(this);
}

MyProgressBar::~MyProgressBar()
{
    delete ui;
}
//设置文件名
void MyProgressBar::setFileName(QString file)
{
    ui->fileName->setText(file+":");
    ui->progressBar->setMinimum(0);
    ui->progressBar->setValue(0);
}
//当前值value,最大值max(=100)
void MyProgressBar::setValueProgress(int value, int max)
{
    ui->progressBar->setValue(value);
    ui->progressBar->setMaximum(max);
}
