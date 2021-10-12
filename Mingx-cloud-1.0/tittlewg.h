#ifndef TITTLEWG_H
#define TITTLEWG_H

#include <QWidget>
#include <QMouseEvent>

namespace Ui {
class TittleWg;
}

class TittleWg : public QWidget
{
    Q_OBJECT

public:
    explicit TittleWg(QWidget *parent = nullptr);
    ~TittleWg();

protected:
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *event);

signals:
    void showSetWg();
    void closeWindow();

private slots:
    void on_Close_clicked(bool checked);
    void on_Min_clicked();
    void on_Set_clicked();

private:
    Ui::TittleWg *ui;

    QWidget *m_parent;
    QPoint m_p;
};

#endif // TITTLEWG_H
