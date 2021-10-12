#include "tittleMainwindow.h"
#include "ui_tittleMainwindow.h"

#include <QToolButton>
#include <QDebug>
#include <QPainter>
#include <QMouseEvent>

TittleMainwindow::TittleMainwindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TittleMainwindow)
{
    ui->setupUi(this);

    m_mapper = new QSignalMapper(this);
    m_curBtn = ui->myfile;
    m_curBtn->setStyleSheet("color:red");

    // key:value == 按钮显示内容：按钮指针
    m_btns.insert(ui->myfile->text(), ui->myfile);
    m_btns.insert(ui->sharelist->text(), ui->sharelist);
    m_btns.insert(ui->download->text(), ui->download);
    m_btns.insert(ui->transform->text(), ui->transform);
    m_btns.insert(ui->switchuser->text(), ui->switchuser);

    m_pages.insert(Page::MYFILE, ui->myfile->text());
    m_pages.insert(Page::SHARE, ui->sharelist->text());
    m_pages.insert(Page::TRANKING, ui->download->text());
    m_pages.insert(Page::TRANSFER, ui->transform->text());
    m_pages.insert(Page::SWITCHUSR, ui->switchuser->text());

    // 设置按钮信号映射
    //QMap<QString, QToolButton*>::iterator it = m_btns.begin();
    auto it=m_btns.begin();

    for(; it != m_btns.end(); ++it){
        m_mapper->setMapping(it.value(), it.value()->text());
        connect(it.value(), SIGNAL(clicked(bool)), m_mapper, SLOT(map()));
    }
    connect(m_mapper, SIGNAL(mapped(QString)), this, SLOT(slotButtonClick(QString)));

    // 关闭
    connect(ui->close, &QToolButton::clicked, [=]()
    {
        emit closeWindow();
    });
    // 最大化
    connect(ui->max, &QToolButton::clicked, [=]()
    {
        //更改最大化或最小化图标
        static bool fl = false;
        if(!fl)
        {
            ui->max->setIcon(QIcon(":/images/fullScream.png"));
        }
        else
        {
            ui->max->setIcon(QIcon(":/images/exitFull.png"));
        }
        fl = !fl;
        emit maxWindow();
    });
    // 最小化
    connect(ui->min, &QToolButton::clicked, [=]()
    {
        emit minWindow();
    });
}

TittleMainwindow::~TittleMainwindow()
{
    delete ui;
}

// 当前登录用户初始化
void TittleMainwindow::changeCurrUser(QString user)
{
    ui->login_usr->setText(user);
}

void TittleMainwindow::slotButtonClick(Page cur)
{
    QString text = m_pages[cur];
    slotButtonClick(text);
}

void TittleMainwindow::slotButtonClick(QString text)
{
    qDebug() << "+++++++++++++++" << text;
    QToolButton* btn = m_btns[text];
    if(btn == m_curBtn && btn != ui->switchuser)
    {
        return;
    }
    m_curBtn->setStyleSheet("color:black");
    btn->setStyleSheet("color:red");
    m_curBtn = btn;

    // 发信号--父类接收
    if(text == ui->myfile->text())
    {
        emit sigMyFile();
    }
    else if(text == ui->sharelist->text())
    {
        emit sigShareList();
    }
    else if(text == ui->download->text())
    {
        emit sigDownload();
    }
    else if(text == ui->transform->text())
    {
        emit sigTransform();
    }
    else if(text == ui->switchuser->text())
    {
        emit sigSwitchUser();
    }
}

void TittleMainwindow::setParent(QWidget *parent)
{
    m_parent = parent;
}

void TittleMainwindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    QPixmap bk(":/images/tittleMainwindow.png");
    painter.drawPixmap(0, 0, width(), height(), bk);
}

void TittleMainwindow::mousePressEvent(QMouseEvent *event)
{
    // 如果是左键, 计算窗口左上角, 和当前按钮位置的距离
    if(event->button() == Qt::LeftButton)
    {
        // 计算和窗口左上角的相对位置
        m_pos = event->globalPos() - m_parent->geometry().topLeft();
    }
}

void TittleMainwindow::mouseMoveEvent(QMouseEvent *event)
{
    // 移动是持续的状态, 需要使用buttons
    if(event->buttons() & Qt::LeftButton)
    {
        QPoint pos = event->globalPos() - m_pos;
        m_parent->move(pos);
    }
}
