#include "uploadtask.h"
#include "uploadlayout.h"
#include <QFileInfo>

//初始化静态成员变量
UploadTask* UploadTask::instance=nullptr;

//创建唯一类对象的接口
UploadTask *UploadTask::getInstance()
{
    if(instance!=nullptr){
        return instance;
    }
    instance=new UploadTask();
    if(instance==nullptr){
        cout<<"new UploadTask() error";
        return nullptr;
    }
    return instance;
}
//返回值：成功为0
//失败：
//  -1: 文件大于50m
//  -2：上传的文件是否已经在上传队列中
//  -3: 打开文件失败
//  -4: 获取布局失败
int UploadTask::appendUploatList(QString path)
{
    /*QFileInfo类，可以获取文件属性
     *
     *
    */
    qint64 size=QFileInfo(path).size();
    if(size>50*1020*1024){
        cout<<"this file is too big!";
        return -1;
    }
    //遍历当前的上传队列，查看文件是否已经在上传队列中
    for(auto it:list){
        if(it->path==path){
            cout<<"this file already exist in uploadlist";
            return -2;
        }
    }
    QFile* file=new QFile(path);
    if(!file->open(QIODevice::ReadOnly)){
        cout<<"file open error";
        delete file;
        file=nullptr;
        return -3;
    }
    //获取文件属性
    QFileInfo info(path);
    Common m_com;

    UploadFileInfo* fileInfo=new UploadFileInfo();
    if(fileInfo==nullptr){
        cout<<"new fileInfo error!";
        return -4;
    }
    fileInfo->file=file;
    fileInfo->md5=m_com.getFileMd5(path);
    fileInfo->path=path;
    fileInfo->size=size;
    fileInfo->fileName=info.fileName();
    fileInfo->isUpload=false;   //文件还没有被上传

    //创建新的进度条
    MyProgressBar *progress=new MyProgressBar();
    if(progress==nullptr){
        cout<<"new progress error!";
        return -4;
    }
    progress->setFileName(info.fileName());
    fileInfo->progress=progress;

    //获取布局
    UploadLayout* pUploadlayout=UploadLayout::getInstance();
    if(pUploadlayout==nullptr){
        cout<<"playout geInstance error!";
        return -4;
    }
    QVBoxLayout *layout = (QVBoxLayout*)pUploadlayout->getLayout();
    // 添加到布局, 最后一个是弹簧, 插入到弹簧上边
    layout->insertWidget(layout->count()-1,progress);
    cout<<fileInfo->fileName.toUtf8().data()<<" 已经在上传列表中";

    list.append(fileInfo);  //将新的上传任务添加到列表中（生产者）

    return 0;
}
//取出第0个上传任务，如果任务队列没有任务在上传，设置第0个任务上传
UploadFileInfo *UploadTask::takeTask()
{
    if(list.isEmpty())
        return nullptr;
    //取出最开始的任务
    UploadFileInfo *t=list.at(0);
    list.at(0)->isUpload=true;

    return t;
}
//删除上传完成的任务
void UploadTask::delUploadTask()
{
    for(int i=0;i<list.size();i++){
        if(list.at(i)->isUpload){
            //delete
            UploadFileInfo* t=list.takeAt(i);

            //获取布局
            UploadLayout* pUploadLayout=UploadLayout::getInstance();
            QLayout* layout=pUploadLayout->getLayout();
            layout->removeWidget(t->progress);  //移除进度条控件

            //关闭打开的文件指针
            QFile* file=t->file;
            file->close();
            delete file;

            delete t->progress;
            delete t;

            return;
        }
    }
}

bool UploadTask::isEmpty()
{
    return list.isEmpty();
}

bool UploadTask::isUpload()
{
    //遍历
    for(auto it:list){
        if(it->isUpload)
             return true;
    }
    return false;
}

void UploadTask::clearList()
{
    for(int i=0;i<list.size();i++){
        UploadFileInfo* t=list.takeFirst();
        delete t;
    }
}

UploadTask::UploadTask()
{

}
