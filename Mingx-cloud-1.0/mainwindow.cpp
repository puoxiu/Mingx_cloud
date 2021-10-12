#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "common/logininfoinstance.h"
#include <QPainter>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 去掉边框
    this->setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    //给菜单窗口传参
    ui->tittle_mainWin_wg->setParent(this);
    //处理所有的信号
    processAllSignals();
    //默认显示我的文件窗口
    ui->stackedWidget->setCurrentWidget(ui->myFile_page);

    //changeCurrUser不是槽函数？
    connect(this,&MainWindow::setUser,ui->tittle_mainWin_wg,&TittleMainwindow::changeCurrUser);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showMainWindow()
{
    m_com.moveToCenter(this);   //居中显示
    //切换到我的文件页面
    ui->stackedWidget->setCurrentWidget(ui->myFile_page);

    //刷新用户文件列表--(必须将my_file_page提升为MyFileWg类)
    ui->myFile_page->refreshFiles();

    // 发送信号，告诉登陆窗口，初始化当前登录用户
    LoginInfoInstance* login=LoginInfoInstance::getInstance();
    emit setUser(login->getUser());
}
//处理所有的信号
void MainWindow::processAllSignals()
{
    //最小化、最大化、退出
    connect(ui->tittle_mainWin_wg,&TittleMainwindow::closeWindow,this,[=](){
        this->close();
    });
    connect(ui->tittle_mainWin_wg,&TittleMainwindow::minWindow,this,[=](){
        this->showMinimized();
    });
    connect(ui->tittle_mainWin_wg,&TittleMainwindow::maxWindow,this,[=](){
        static bool flag=false;
        if(!flag){
            this->showMaximized();
        }else{
            this->showNormal();
        }
        flag=!flag;
    });
    //窗口切换
    // 我的文件
    connect(ui->tittle_mainWin_wg,&TittleMainwindow::sigMyFile,[=](){
       ui->stackedWidget->setCurrentWidget(ui->myFile_page);
       //刷新文件列表
       ui->myFile_page->refreshFiles();
    });
    // 分享列表
    connect(ui->tittle_mainWin_wg,&TittleMainwindow::sigShareList,[=](){
        ui->stackedWidget->setCurrentWidget(ui->sharedList_page);
        //刷新分享列表
        ui->sharedList_page->refreshFiles();

    });
    // 下载榜
    connect(ui->tittle_mainWin_wg,&TittleMainwindow::sigDownload,[=](){
        ui->stackedWidget->setCurrentWidget(ui->ranking_page);
        //刷新
        ui->ranking_page->refreshList();
    });
    // 传输列表
    connect(ui->tittle_mainWin_wg,&TittleMainwindow::sigTransform,[=](){
        ui->stackedWidget->setCurrentWidget(ui->transformList_page);
    });
    // 切换用户
    connect(ui->tittle_mainWin_wg,&TittleMainwindow::sigSwitchUser,[=](){
        //
        cout<<"Bye Bye...";
        loginAgain();
    });
    //stackWidget窗口切换
    connect(ui->myFile_page,&MyFileWg::gotoTransfer,[=](TransferStatus status){
        ui->tittle_mainWin_wg->slotButtonClick(Page::TRANSFER);
        if(status==TransferStatus::Uplaod){
            ui->transformList_page->showUpload();
        }else if(status==TransferStatus::Download){
            ui->transformList_page->showDownload();
        }
    });
    // 信号传递
    connect(ui->sharedList_page, &ShareList::gotoTransfer, ui->myFile_page, &MyFileWg::gotoTransfer);
}
//重新登录
void MainWindow::loginAgain()
{
    //发送信号给登录窗口，切换用户
    emit sigChangeUser();
    //清空上一个用户的上传下载任务
    ui->myFile_page->clearAllTask();

    //清空上一个用户的文件信息
    ui->myFile_page->clearFileList();
    ui->myFile_page->clearItems();
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    //QPixmap bk(":/images/title_bk3.jpg");
    //painter.drawPixmap(0, 0, width(), height(), bk);
}
