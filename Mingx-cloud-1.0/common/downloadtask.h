#ifndef DOWNLOADTASK_H
#define DOWNLOADTASK_H

#include "common.h"
#include <QVBoxLayout>
#include <QFile>
#include "selfWidget/myprogressbar.h"


//下载文件信息
struct DownloadFileInfo
{
    QFile *file;        //文件指针
    QString user;       //下载用户
    QString filename;   //文件名字
    QString md5;        //文件md5
    QUrl url;           //下载网址
    bool isDownload;    //是否已经在下载
    MyProgressBar *progress;   //下载进度控件
    bool isShare;       //是否为共享文件下载
};

//下载任务列表类，单例模式(同理,一个程序只能有一个下载任务列表)
class DownloadTask
{
public:
    static DownloadTask* getInstance();

    void clearList(); //清空上传列表
    bool isEmpty();   //判断上传队列是否为空
    bool isDownload(); //是否有文件正在下载
    bool isShareTask();//第一个任务是否为共享文件的任务
    DownloadFileInfo *takeTask();//取出第0个下载任务，如果任务队列没有任务在下载，设置第0个任务下载
    void dealDownloadTask(); //删除下载完成的任务

    //追加任务到下载队列
    //参数：info：下载文件信息， filePathName：文件保存路径, isShare: 是否为共享文件下载, 默认为false
    //成功：0
    //失败：
    //  -1: 下载的文件是否已经在下载队列中
    //  -2: 打开文件失败
    int appendDownloadList( FileInfo *info, QString filePathName, bool isShare = false);

private:
    DownloadTask();
    ~DownloadTask();

    static DownloadTask* instance;
    QList<DownloadFileInfo*> list;  //下载任务队列
};

#endif // DOWNLOADTASK_H
