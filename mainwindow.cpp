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
    musicIdList = new QList<qint64>;
    i_pos = 0, nowPlayingId = -1;

    docTextObject = ui->plainTextEdit_SongList->document();
    ui->plainTextEdit_SongList->setReadOnly(true);

    playerObject = new QMediaPlayer(this);
    audioOutput = new QAudioOutput();
    playerObject->setAudioOutput(audioOutput);
    audioOutput->setVolume(50);
    ui->horizontalSlider_Volume->setValue(50);
    // playerObject->setSource();
    connect(networkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(handleDataBackFunc(QNetworkReply*)));
    connect(playerObject, SIGNAL(durationChanged(qint64)), this, SLOT(handleProgressTimeChangeFunc(qint64)));
    connect(playerObject, SIGNAL(positionChanged(qint64)), this, SLOT(handleLCDNumberTimeChangeFunc(qint64)));
    connect(playerObject, SIGNAL(positionChanged(qint64)), this, SLOT(handleChangeSliderFunc(qint64)));
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

void MainWindow::handleProgressTimeChangeFunc(qint64 duration){
    ui->horizontalSlider_PlayProgress->setMaximum(duration);
}

void MainWindow::handleChangeSliderFunc(qint64 duration){
    if(ui->horizontalSlider_PlayProgress->isSliderDown()) return;
    ui->horizontalSlider_PlayProgress->setSliderPosition(duration);
}

void MainWindow::handleLCDNumberTimeChangeFunc(qint64 duration){
    duration /= 1000;
    int nowHour = int(duration / 3600);
    int nowMinute = int((duration - nowHour * 3600) / 60);
    int nowSecond = int(duration - nowHour * 3600 - nowMinute * 60);
    nowSecond %= 60;
    QString songTime = QString::asprintf("%d:%d", nowMinute, nowSecond);
    ui->lcdNumber_PlayTime->display(songTime);
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
        musicIdList->clear();
        i_pos = 0;
        QJsonObject resultObjects = totalJson["result"].toObject();
        if(resultObjects.contains("songs")){
            QJsonArray jsonArray = resultObjects["songs"].toArray();
            bool flag = 0;
            for(auto song : jsonArray){

                QJsonObject songObject = song.toObject();
                QString songName = songObject["name"].toString();

                auto musicId = songObject["id"].toInteger();
                musicIdList->append(musicId);
                QString artistName = songObject["artists"].toArray()[0].toObject()["name"].toString();
                ui->plainTextEdit_SongList->appendPlainText(songName + "   -   " + artistName);
                if(!flag){
                    auto nowPlayingSongName = ui->plainTextEdit_SongList->document()->findBlockByLineNumber(0).text();
                    ui->label_NowPlayingSong->setText(nowPlayingSongName);
                    flag = 1;
                }
            }
        }
        ui->plainTextEdit_SongList->verticalScrollBar()->setValue(0);
    }
}

void MainWindow::playMusicById(const qint64& musicId){
    if(musicId == -1) return;
    if(musicId == nowPlayingId){
        playerObject->play();
        return;
    }
    QString urlStr = QString("https://music.163.com/song/media/outer/url?id=%0.mp3").arg(musicId);
    QNetworkRequest networkRequest;
    networkRequest.setUrl(urlStr);
    musicUrlGeter->get(networkRequest);
}

void MainWindow::handleMusicUrl(QNetworkReply* pReplay){
    if(pReplay->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() != 302){
        QMessageBox::information(this, "暂不支持VIP音乐播放","",QMessageBox::Yes);
        return;
    }

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
    if(musicIdList->empty()){
        return;
    }
    playMusicById(musicIdList->at(i_pos));
    nowPlayingId = musicIdList->at(i_pos);
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


void MainWindow::on_pushButton_PauseSong_clicked()
{
    playerObject->pause();
}


void MainWindow::on_pushButton_PreviousSong_clicked()
{
    if(musicIdList->empty()){
        return;
    }
    i_pos--;
    if(i_pos == -1){
        i_pos = musicIdList->size();
    }
    playMusicById(musicIdList->at(i_pos));
    auto nowPlayingSongName = ui->plainTextEdit_SongList->document()->findBlockByLineNumber(i_pos).text();
    ui->label_NowPlayingSong->setText(nowPlayingSongName);
    nowPlayingId = musicIdList->at(i_pos);
}


void MainWindow::on_pushButton_NextSong_clicked()
{
    if(musicIdList->empty()){
        return;
    }
    i_pos++;
    if(i_pos == musicIdList->size()){
        i_pos = 0;
    }
    playMusicById(musicIdList->at(i_pos));
    auto nowPlayingSongName = ui->plainTextEdit_SongList->document()->findBlockByLineNumber(i_pos).text();
    ui->label_NowPlayingSong->setText(nowPlayingSongName);
    nowPlayingId = musicIdList->at(i_pos);
}


void MainWindow::on_pushButton_Mute_clicked()
{
    if(audioOutput->isMuted()){
        audioOutput->setMuted(0);
        ui->pushButton_Mute->setIcon(QIcon(":/new/prefix1/dist/声音_实体.png"));
    }
    else{
        audioOutput->setMuted(1);
        ui->pushButton_Mute->setIcon(QIcon(":/new/prefix1/dist/mute.png"));
    }
}

void MainWindow::on_horizontalSlider_PlayProgress_valueChanged(int value){
    // if(!playerObject->isPlaying()) return;
    // playerObject->setPosition(value);
}


void MainWindow::on_horizontalSlider_Volume_valueChanged(int value)
{
    audioOutput->setVolume(value);
}

