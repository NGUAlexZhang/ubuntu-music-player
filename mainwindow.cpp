#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#pragma execution_character_set("utf-8")

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //去掉窗口标题
    this->setWindowFlag(Qt::FramelessWindowHint);
    // setMouseTracking(true);
    // ui->centralwidget->setMouseTracking(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event){
    QPainter qPainter(this);
    qPainter.drawPixmap(rect(), QPixmap(":/new/prefix1/dist/A2.jpg"), QRect());
}

//关闭窗口
void MainWindow::on_pushButton_Exit_clicked(){
    this->close();
}


void MainWindow::on_pushButton_AddSong_clicked()
{

}


void MainWindow::on_pushButton_PlaySong_clicked()
{

}


void MainWindow::on_pushButton_Minimize_clicked()
{
    this->setWindowState(Qt::WindowMinimized);
}

//拖动窗口
void MainWindow::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        m_MouseDrag = true;

        mouseStartPoint = event->globalPosition();

        windowTopleftPoint = this->frameGeometry().topLeft();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event){
    if(m_MouseDrag){
        QPointF m_Distance = event->globalPosition() - mouseStartPoint;
        this->move(windowTopleftPoint + m_Distance.toPoint());
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        m_MouseDrag = false;
    }
}

void MainWindow::on_pushButton_About_clicked()
{
    // QMessageBox::information(this, "提示", "关于对话框", QMessageBox::Yes);
    AboutDialog *pAboutDialog = new AboutDialog();
    //模态对话框
    pAboutDialog->exec();
}

