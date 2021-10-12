#include "tittlewg.h"
#include "ui_tittlewg.h"

TittleWg::TittleWg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TittleWg)
{
    ui->setupUi(this);

    m_parent=parent;
    //Logo image
    ui->Logo->setPixmap(QPixmap(":/images/Logo1.png").scaled(40,40));
}

TittleWg::~TittleWg()
{
    delete ui;
}

void TittleWg::mouseMoveEvent(QMouseEvent *e)
{
    //当鼠标左键按下时
    if(e->buttons() & Qt::LeftButton){
        m_parent->move(e->globalPos()-m_p);
    }
}

void TittleWg::mousePressEvent(QMouseEvent *e)
{
    //当鼠标左键按下时，获取当前光标的坐标=当前点击坐标-窗口左上角坐标
    if(e->button()==Qt::LeftButton){
        m_p=e->globalPos()-m_parent->frameGeometry().topLeft();
    }
}

void TittleWg::on_Close_clicked(bool checked)
{
    //发送自定义信号
    emit closeWindow();
}

void TittleWg::on_Min_clicked()
{
    m_parent->showMinimized();
}

void TittleWg::on_Set_clicked()
{
    //发送自定义信号
    emit showSetWg();
}
