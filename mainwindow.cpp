#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "./aboutdialog.h"
#include <QDebug>

#pragma execution_character_set("utf-8")

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //去掉窗口标题
    this->setWindowFlag(Qt::FramelessWindowHint);

    this->setFixedSize(this->width(), this->height());
    ui->lineEdit_InputSongs->setFocus();//设置焦点

    networkAccessManager = new QNetworkAccessManager(this);
    musicUrlGeter = new QNetworkAccessManager(this);
    i_pos = 0;

    docTextObject = ui->plainTextEdit_SongList->document();
    ui->plainTextEdit_SongList->setReadOnly(true);

    playerObject = new QMediaPlayer(this);
    audioOutput = new QAudioOutput();
    playerObject->setAudioOutput(audioOutput);
    audioOutput->setVolume(50);
    // playerObject->setSource();
    connect(networkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(handleDataBackFunc(QNetworkReply*)));
    connect(musicUrlGeter, SIGNAL(finished(QNetworkReply*)), this, SLOT(handleMusicUrl(QNetworkReply*)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event){
    QPainter qPainter(this);
    qPainter.drawPixmap(rect(), QPixmap(":/new/prefix1/dist/A2.jpg"), QRect());
}

void MainWindow::handleDataBackFunc(QNetworkReply *pReply){
    qByteArraySearchInfo = pReply->readAll();
    QJsonParseError jsonErrorParser;
    QJsonDocument jsonRecv = QJsonDocument::fromJson(qByteArraySearchInfo, &jsonErrorParser);

    if(jsonErrorParser.error != QJsonParseError::NoError){
        return;
    }
    QJsonObject totalJson = jsonRecv.object();
    QList keys = totalJson.keys();
    if(keys.contains("result")){
        ui->plainTextEdit_SongList->clear();
        QJsonObject resultObjects = totalJson["result"].toObject();
        int cnt = 0;
        if(resultObjects.contains("songs")){
            QJsonArray jsonArray = resultObjects["songs"].toArray();
            for(auto song : jsonArray){

                QJsonObject songObject = song.toObject();
                QString songName = songObject["name"].toString();

                auto musicId = songObject["id"].toInteger();
                QString artistName = songObject["artists"].toArray()[0].toObject()["name"].toString();
                if(!cnt){
                    playMusicById(musicId);
                    cnt++;
                }
                ui->plainTextEdit_SongList->appendPlainText(songName + "   -   " + artistName);
            }
        }
        ui->plainTextEdit_SongList->verticalScrollBar()->setValue(0);

    }
}

void MainWindow::playMusicById(const qint64& musicId){
    QString urlStr = QString("https://music.163.com/song/media/outer/url?id=%0.mp3").arg(musicId);
    QNetworkRequest networkRequest;
    networkRequest.setUrl(urlStr);
    musicUrlGeter->get(networkRequest);
}

void MainWindow::handleMusicUrl(QNetworkReply* pReplay){
    // auto headerLists = pReplay->rawHeader();
    if(pReplay->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() != 302){
        QMessageBox::information(this, "暂不支持VIP音乐播放","",QMessageBox::Yes);
        return;
    }
    // auto musicUrl = pReplay->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();

    auto qByteArrayGetInfo = pReplay->rawHeader("location");
    QString musicUrlStr = qByteArrayGetInfo;
    if(musicUrlStr == "http://music.163.com/404"){
        QMessageBox::information(this, "暂不支持VIP音乐播放","",QMessageBox::Yes);
        return;
    }
    playerObject->setSource(QUrl(musicUrlStr));
    playerObject->play();
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
    // playerObject->
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
    AboutDialog *pAboutDialog = new AboutDialog();
    //模态对话框
    pAboutDialog->exec();
}


void MainWindow::on_pushButton_Search_clicked()
{
    QString str1, str2;
    str1 = ui->lineEdit_InputSongs->text();
    str2 = "https://netease.project.ac.cn/search?keywords=" + str1;
    QNetworkRequest networkRequest;
    networkRequest.setUrl(str2);
    networkAccessManager->get(networkRequest);
}

