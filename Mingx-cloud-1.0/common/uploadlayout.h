#ifndef UPLOADLAYOUT_H
#define UPLOADLAYOUT_H

#include "common.h"
#include <QVBoxLayout>  //垂直布局类

//封装上传进度布局类，用管理上传时的布局
//同样使用单例模式

class UploadLayout
{
public:
    static UploadLayout* getInstance();

    void setLayout(QWidget *p);   //设置布局
    QLayout* getLayout();   //获取布局

private:
    UploadLayout();
    ~UploadLayout();

    static UploadLayout* instance;
    QLayout* m_layout;
    QWidget* m_wg;
};

#endif // UPLOADLAYOUT_H
