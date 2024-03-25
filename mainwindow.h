#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>
#include "aboutdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    bool m_MouseDrag;
    QPointF mouseStartPoint;//鼠标位置
    QPoint windowTopleftPoint;//窗口初始位置
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //自定义成员
public:
    //重绘背景图
    void paintEvent(QPaintEvent *event);

    //拖动功能
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
private slots:
    void on_pushButton_Exit_clicked();

    void on_pushButton_AddSong_clicked();

    void on_pushButton_PlaySong_clicked();

    void on_pushButton_Minimize_clicked();

    void on_pushButton_About_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
