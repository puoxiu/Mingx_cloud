#ifndef DOWNLOADLAYOUT_H
#define DOWNLOADLAYOUT_H

#include "common.h"
#include <QVBoxLayout>

class DownloadLayout
{
public:
    static DownloadLayout* getInstance();

    void setDownloadLayout(QWidget *p);   //设置布局
    QLayout* getDownloadLayout();   //获取布局

private:
    DownloadLayout();
    ~DownloadLayout();

    static DownloadLayout* instance;
    QLayout *m_layout;
    QWidget* m_wg;
};

#endif // DOWNLOADLAYOUT_H
