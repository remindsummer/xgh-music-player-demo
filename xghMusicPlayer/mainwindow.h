#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QAudioOutput>  // 必须包含这个头文件
#include <QPushButton>
#include <QVector>
#include <QFileDialog>
#include <QStringList>
#include <QDirIterator>
#include <QMessageBox>
#include <QLabel>
#include <QTimer>
#include <QListWidget>
#include <QFileInfo>
#include <QMediaMetaData>
#include <QPixmap>
#include <QRandomGenerator>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void playAndPause();
    bool getPlayList(const QString& playListPath);
    void getFolderPath();
    void preAudio();
    void nextAudio();
    void loadAudioFilesToListWidget(QListWidget* listWidget, const QStringList& audioFiles);
    void updateProgressBar(qint64 position);
    void on_progressSlider_sliderMoved(int position);
    void durationChanged(qint64 duration);
    void onListItemClicked(QListWidgetItem *item);
    void updatePlayMode();
    void listLoopMode();
    void singleLoopMode();
    void randomPlayMode();
    void onMediaStatusChanged(QMediaPlayer::MediaStatus status);
private:
    Ui::MainWindow *ui;
    void setBackGroundPicture(const QString& filename, const int time=0);
    void initButton();

    QMediaPlayer * musicPlayer = nullptr;
    QAudioOutput *audioOutput = nullptr;
    QString folderPath;
    QStringList audioFiles;
    int audio_p = 0;
    float volume = .5;
    enum PlayMode {
        ListLoop,    // 列表循环（默认）
        SingleLoop,  // 单曲循环
        RandomPlay   // 随机播放
    };
    PlayMode currentPlayMode = ListLoop;

    void playRandom();
    void playCurrent();
    void showCurrentPlay();
};
#endif // MAINWINDOW_H
