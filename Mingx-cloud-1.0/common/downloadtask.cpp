#include "downloadtask.h"
#include "downloadlayout.h"

DownloadTask* DownloadTask::instance=new DownloadTask;



DownloadTask *DownloadTask::getInstance()
{
    if(instance==nullptr){
        instance=new DownloadTask;
    }
    return instance;
}
//清空下载列表
void DownloadTask::clearList()
{
    for(int i=0;i<list.size();i++){
        DownloadFileInfo* info=list.takeFirst();
        delete info;
    }
}

bool DownloadTask::isEmpty()
{
    return list.isEmpty();
}

bool DownloadTask::isDownload()
{
    for(auto it:list){
        if(it->isDownload)
            return true;
    }
    return false;
}

bool DownloadTask::isShareTask()
{
    if(isEmpty())
        return false;
    return list.at(0)->isShare;
}
//取出第0个下载任务，如果任务队列没有任务在下载，设置第0个任务下载
DownloadFileInfo *DownloadTask::takeTask()
{
    if(isEmpty())
        return nullptr;
    list.at(0)->isDownload=true;
    return list.at(0);
}
//删除下载完成的任务
void DownloadTask::dealDownloadTask()
{
    //遍历
    for(int i=0;i<list.size();i++){
        //条件成立说明有下载任务
        if(list.at(i)->isDownload){
            //delete
            DownloadFileInfo* fileInfo=list.at(0);
            //获取布局
            DownloadLayout* downloadLayout=DownloadLayout::getInstance();
            QLayout *layout=downloadLayout->getDownloadLayout();

            layout->removeWidget(fileInfo->progress);//移除控件
            delete fileInfo->progress;

            QFile* file=fileInfo->file;
            file->close();
            delete file;

            delete fileInfo;
            return;
        }
    }
}

//参数：info：下载文件信息， filePathName：文件保存路径, isShare: 是否为共享文件下载, 默认为false
//成功：0
//失败：
//  -1: 下载的文件是否已经在下载队列中
//  -2: 打开文件失败
int DownloadTask::appendDownloadList(FileInfo *info, QString filePathName, bool isShare)
{
    //遍历查看，若此文件已经下载则退出
    for(auto it:list){
        if(it->user==info->user && it->filename==info->filename){
            cout<<info->filename<<" 已经在下载队列中";
            return -1;
        }
    }
    QFile* file=new QFile(filePathName);
    if(!file->open(QIODevice::WriteOnly)){
        cout<<"file open error";
        delete file;
        file=nullptr;
        return -2;
    }

    //
    DownloadFileInfo* fileInfo=new DownloadFileInfo;
    fileInfo->user=info->user;
    fileInfo->md5=info->md5;
    fileInfo->url=info->url;
    fileInfo->file=file;
    fileInfo->filename=info->filename;
    fileInfo->isShare=isShare;
    fileInfo->isDownload=false;

    //创建新的下载进度条
    MyProgressBar* progress=new MyProgressBar;
    progress->setFileName(info->filename);

    //获取布局
    DownloadLayout* downloadLayout=DownloadLayout::getInstance();
    QVBoxLayout *layout = (QVBoxLayout*)downloadLayout->getDownloadLayout();

    fileInfo->progress=progress;
    layout->insertWidget(layout->count()-1,progress);

    cout<<info->filename<<" 已经插入到下载队列中";

    list.append(fileInfo);

    return 0;

}

DownloadTask::DownloadTask()
{

}
