#ifndef TITTLEMAINWINDOW_H
#define TITTLEMAINWINDOW_H

#include <QWidget>
#include <QSignalMapper>
#include <QMap>

namespace Ui {
class TittleMainwindow;
}

class QToolButton;
enum Page{MYFILE, SHARE, TRANKING, TRANSFER, SWITCHUSR};    //各个子页面名称

class TittleMainwindow : public QWidget
{
    Q_OBJECT

public:
    explicit TittleMainwindow(QWidget *parent = 0); //需要父类传参--（父类widget提升为）
    ~TittleMainwindow();

    void changeCurrUser(QString user);  //切换用户

public slots:
    // 按钮处理函数
    void slotButtonClick(Page cur);
    void slotButtonClick(QString text);
    void setParent(QWidget *parent);

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent* event);

signals:
    //父类接收信号
    void sigMyFile();
    void sigShareList();
    void sigDownload();
    void sigTransform();
    void sigSwitchUser();
    void closeWindow();
    void minWindow();
    void maxWindow();

private:
    Ui::TittleMainwindow *ui;

    QPoint m_pos;
    QWidget* m_parent;
    QSignalMapper* m_mapper;
    QToolButton* m_curBtn;
    QMap<QString, QToolButton*> m_btns;
    QMap<Page, QString> m_pages;
};

#endif // TITTLEMAINWINDOW_H
