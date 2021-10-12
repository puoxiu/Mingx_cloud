#include "uploadlayout.h"

UploadLayout* UploadLayout::instance=new UploadLayout;

UploadLayout *UploadLayout::getInstance()
{
    return instance;
}
//设置布局
// 给当前布局添加窗口
void UploadLayout::setLayout(QWidget *p)
{
    m_wg = new QWidget(p);
    QLayout* layout = p->layout();
    layout->addWidget(m_wg);
    layout->setContentsMargins(0, 0, 0, 0);
    QVBoxLayout* vlayout = new QVBoxLayout;
    // 布局设置给窗口
    m_wg->setLayout(vlayout);
    // 边界间隔
    vlayout->setContentsMargins(0, 0, 0, 0);
    m_layout = vlayout;

    // 添加弹簧
    vlayout->addStretch();  //添加一弹簧

}
//获取布局
QLayout *UploadLayout::getLayout()
{
    return m_layout; // 垂直布局
}

UploadLayout::UploadLayout()
{
}
