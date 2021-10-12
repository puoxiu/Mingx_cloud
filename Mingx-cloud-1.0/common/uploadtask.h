#ifndef UPLOADTASK_H
#define UPLOADTASK_H

#include "common.h"
#include <QVBoxLayout>
#include <QFile>
#include "selfWidget/myprogressbar.h"

//上传文件信息-封装到结构体中
struct UploadFileInfo
{
    QString md5;      //文件md5
    QFile *file;      //文件指针
    QString fileName;   //文件名
    qint64 size;    //文件大小
    QString path;   //文加路径
    bool isUpload;  //是否已经在上传
    MyProgressBar* progress;    //本文件的上传进度条
};

//上传文件列表类,使用单例模式(一个程序只能有一个上传列表)
//生产者消费者模型
class UploadTask
{
public:
    static UploadTask* getInstance();   //创建唯一类对象的接口

    //追加上传文件到上传列表中
    //参数：path 上传文件路径
    //返回值：成功为0
    //失败：
    //  -1: 文件大于50m
    //  -2：上传的文件是否已经在上传队列中
    //  -3: 打开文件失败
    //  -4: 获取布局失败
    int appendUploatList(QString path);

    UploadFileInfo * takeTask();    //取出第0个上传任务，如果任务队列没有任务在上传，设置第0个任务上传
    void delUploadTask();       //删除上传完成的任务
    bool isEmpty();     //上传任务队列是否为空
    bool isUpload();    //是否有文件正在上传
    void clearList(); //清空上传列表

private:
    UploadTask();
    ~UploadTask();

    static UploadTask *instance;    //创建的唯一类对象
    QList<UploadFileInfo*> list;    //上传任务队列
};

#endif // UPLOADTASK_H
