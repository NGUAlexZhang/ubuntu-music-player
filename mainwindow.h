#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimediaWidgets>
#include <QTextBlock>
#include <QTimer>
#include <QFileDialog>
#include <QJsonParseError>
#include <QJsonObject>
#include <QLCDNumber>

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
    QMediaPlayer *playerObject;//播放器
    QAudioOutput *audioOutput;
    QByteArray qByteArraySearchInfo;//接收网络请求内容
    QString strLocalMusicPath;//本地音乐路径
    int i_musicID, i_pos;//歌曲id
    QString musicName,singerName;//歌曲歌手名字
    QTextDocument *docTextObject;//富文本
    QTextBlock *qTextLine;//处理文本块的指针
    QNetworkAccessManager *networkAccessManager;//send get music request
    QNetworkAccessManager *musicUrlGeter;//send get music url by id request
    QList<qint64> *musicIdList;
    qint64 nowPlayingId;
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
    void playMusicById(const qint64& musicId);

public slots:
    void handleDataBackFunc(QNetworkReply *pReply);
    void handleMusicUrl(QNetworkReply *pReply);
    void handleLCDNumberTimeChangeFunc(qint64 duration);
    void handleProgressTimeChangeFunc(qint64 duration);
    // void handleSliderChangeFunc(qint64 position);

private slots:
    void on_pushButton_Exit_clicked();

    void on_pushButton_AddSong_clicked();

    void on_pushButton_PlaySong_clicked();

    void on_pushButton_Minimize_clicked();

    void on_pushButton_About_clicked();

    void on_pushButton_Search_clicked();

    void on_pushButton_PauseSong_clicked();

    void on_pushButton_PreviousSong_clicked();

    void on_pushButton_NextSong_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
