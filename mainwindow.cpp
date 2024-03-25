#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //去掉窗口标题
    this->setWindowFlag(Qt::FramelessWindowHint);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::painEvent(QPaintEvent *event){
    QPainter qPainter(this);
    qPainter.drawPixmap(0, 0, width(), height(), QPixmap(":/new/prefix1/dist/A2.jpg"));
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

