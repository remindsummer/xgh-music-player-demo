#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("RemindSummer music player");
    setFixedSize(192*3, 108*4);
    initButton();
    setBackGroundPicture(":/background_image/sea.jpg");

    //创建 QMediaPlayer 和 QAudioOutput
    musicPlayer = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    musicPlayer->setAudioOutput(audioOutput);  // bind audio output
    // QString musicPath = "/home/xgh/Music/The Red Sun Shines on the Frontier.mp3"; // 测试用音乐
    // musicPlayer->setSource(QUrl::fromLocalFile(musicPath));


    connect(ui->pushButton_play, &QPushButton::clicked, this, &MainWindow::playAndPause);
    connect(ui->import_audio, &QMenu::aboutToShow, this,
            [this]()->void {
                getFolderPath();
                audioFiles.clear();
                audio_p = 0;
                getPlayList(folderPath);
                if(folderPath.isEmpty()) return;
                if(audioFiles.empty()){
                    QMessageBox::information(this, "remind", "No audio file in" + folderPath);
                }
    });
    connect(ui->quit, &QMenu::aboutToShow, this, [this]()->void {this->close();});
    connect(ui->pushButton_previous, &QPushButton::clicked, this, &MainWindow::preAudio);
    connect(ui->pushButton_next, &QPushButton::clicked, this, &MainWindow::nextAudio);
    connect(ui->pushButton_playlist, &QPushButton::clicked, this, [this]()->void {
        ui->audio_list->setVisible(!ui->audio_list->isVisible());
    });
    connect(ui->import_audio, &QMenu::aboutToShow, this, [this]()->void {loadAudioFilesToListWidget(ui->audio_list, audioFiles);});
    connect(ui->horizontalSlider, &QSlider::sliderMoved, this, [this](int n)->void{
        volume = n / 100.0f;
        audioOutput->setVolume(volume);
        if(n==0){
            ui->pushButton_volume_mute->setIcon(QIcon(":/icon/mute.png"));
        }
        else{
            ui->pushButton_volume_mute->setIcon(QIcon(":/icon/volume.png"));
        }
    });
    connect(ui->pushButton_volume_mute, &QPushButton::clicked, this, [this](){
        float currentVolume = audioOutput->volume();
        if(currentVolume==.0){
            ui->pushButton_volume_mute->setIcon(QIcon(":/icon/volume.png"));
            audioOutput->setVolume(volume);
        }
        else{
            ui->pushButton_volume_mute->setIcon(QIcon(":/icon/mute.png"));
            audioOutput->setVolume(.0);
        }
    });
    connect(ui->pushButton_volume, &QPushButton::clicked, this, [this]()->void {
        ui->vol_widget->setVisible(!ui->vol_widget->isVisible());
    });
    connect(ui->pushButton_mode, &QPushButton::clicked, this, &MainWindow::updatePlayMode);

    connect(musicPlayer, &QMediaPlayer::positionChanged, this, &MainWindow::updateProgressBar);
    connect(musicPlayer, &QMediaPlayer::durationChanged, this, &MainWindow::durationChanged);
    connect(ui->progressSlider, &QSlider::sliderMoved, this, &MainWindow::on_progressSlider_sliderMoved);
    connect(ui->audio_list, &QListWidget::itemClicked, this, &MainWindow::onListItemClicked);
    connect(musicPlayer, &QMediaPlayer::mediaStatusChanged, this, &MainWindow::onMediaStatusChanged);
}

MainWindow::~MainWindow()
{
    delete musicPlayer;
    delete ui;
}


void MainWindow::setBackGroundPicture(const QString& filename, const int time){
    QPixmap picture(filename);
    QSize windowSize = this->size();
    // adapt manwindow
    QPixmap scalemap = picture.scaled(windowSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    // set picture to manwindow
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Window, QBrush(scalemap));
    this->setPalette(palette);
}

void MainWindow::playAndPause(){
    if(audioFiles.empty()){
        QMessageBox::information(this, "remind", "No audio file, you need to import first.");
        return;
    }

    QMediaPlayer::PlaybackState state = musicPlayer->playbackState();
    if (state == QMediaPlayer::PlayingState) {
        qDebug() << "playing";
        musicPlayer->pause();
        ui->pushButton_play->setIcon(QIcon(":/icon/pause.png"));
    } else if (state == QMediaPlayer::PausedState) {
        qDebug() << "pausing";
        musicPlayer->play();
        ui->pushButton_play->setIcon(QIcon(":/icon/play.png"));
    } else if (state == QMediaPlayer::StoppedState) {
        qDebug() << "stopped";
        QString filepath = audioFiles[audio_p];
        musicPlayer->setSource(filepath);
        musicPlayer->play();
        ui->pushButton_play->setIcon(QIcon(":/icon/play.png"));
    }
    showCurrentPlay();
}

bool MainWindow::getPlayList(const QString& playListPath){

    QDirIterator it(folderPath, {"*.mp3", "*.wav", "*.ogg"}, QDir::Files);
    while (it.hasNext()) {
        audioFiles.append(it.next());
        qDebug() << "找到音频文件:" << it.filePath();
    }
    folderPath.clear();
    if(audioFiles.size()==0) return false;
    return true;
}

void MainWindow::getFolderPath(){
    folderPath = QFileDialog::getExistingDirectory(
        this,
        "选择音频文件夹",
        QDir::homePath(),
        QFileDialog::ShowDirsOnly
    );
}

void MainWindow::preAudio(){
    if(audioFiles.empty()){
        QMessageBox::information(this, "remind", "No audio file");
        return;
    }
    int n = audioFiles.size();
    if(audio_p>0) --audio_p;
    else audio_p = n - 1;
    musicPlayer->setSource(QUrl::fromLocalFile(audioFiles[audio_p]));
    musicPlayer->play();
    ui->pushButton_play->setIcon(QIcon(":/icon/play.png"));
    showCurrentPlay();
}

void MainWindow::nextAudio(){
    if(audioFiles.empty()){
        QMessageBox::information(this, "remind", "No audio file");
        return;
    }
    int n = audioFiles.size();
    if(audio_p<n-1) ++audio_p;
    else audio_p = 0;
    musicPlayer->setSource(QUrl::fromLocalFile(audioFiles[audio_p]));
    musicPlayer->play();
    ui->pushButton_play->setIcon(QIcon(":/icon/play.png"));
    showCurrentPlay();
}

// 辅助函数：格式化时长 (毫秒 -> 00:00:00)
QString formatDuration(int milliseconds)
{
    int seconds = milliseconds / 1000;
    int minutes = seconds / 60;
    int hours = minutes / 60;

    return QString("%1:%2:%3")
        .arg(hours, 2, 10, QLatin1Char('0'))
        .arg(minutes % 60, 2, 10, QLatin1Char('0'))
        .arg(seconds % 60, 2, 10, QLatin1Char('0'));
}

// 辅助函数：格式化文件大小
QString formatFileSize(qint64 bytes)
{
    constexpr qint64 KB = 1024;
    constexpr qint64 MB = KB * 1024;
    constexpr qint64 GB = MB * 1024;

    if (bytes >= GB) {
        return QString("%1 GB").arg(qreal(bytes) / GB, 0, 'f', 2);
    } else if (bytes >= MB) {
        return QString("%1 MB").arg(qreal(bytes) / MB, 0, 'f', 2);
    } else if (bytes >= KB) {
        return QString("%1 KB").arg(qreal(bytes) / KB, 0, 'f', 2);
    } else {
        return QString("%1 字节").arg(bytes);
    }
}

void MainWindow::loadAudioFilesToListWidget(QListWidget* listWidget, const QStringList& audioFiles)
{
    listWidget->clear();

    if (audioFiles.isEmpty()) {
        listWidget->addItem("没有可用的音频文件");
        return;
    }

    for (const QString& filePath : audioFiles) {
        QFileInfo fileInfo(filePath);
        if (!fileInfo.exists() || !fileInfo.isFile()) continue;

        QListWidgetItem* item = new QListWidgetItem(listWidget);
        item->setData(Qt::UserRole, filePath);

        // 创建自定义widget
        QWidget* itemWidget = new QWidget();
        QHBoxLayout* layout = new QHBoxLayout(itemWidget);
        layout->setContentsMargins(5, 5, 5, 5);

        // 文件图标
        QLabel* iconLabel = new QLabel();
        iconLabel->setPixmap(QIcon(":/icons/audio-file.png").pixmap(32, 32));
        layout->addWidget(iconLabel);

        // 文件信息
        QVBoxLayout* infoLayout = new QVBoxLayout();
        QLabel* nameLabel = new QLabel(fileInfo.fileName());
        nameLabel->setStyleSheet("font-weight: bold;");
        QLabel* pathLabel = new QLabel(fileInfo.path());
        pathLabel->setStyleSheet("color: #666; font-size: 10px;");
        infoLayout->addWidget(nameLabel);
        infoLayout->addWidget(pathLabel);
        layout->addLayout(infoLayout, 1);

        // 音频信息
        QString durationStr = "未知时长";
        QString bitrateStr = "未知比特率";

        // 使用QMediaPlayer获取元数据
        QMediaPlayer player;
        QAudioOutput audioOutput;
        player.setAudioOutput(&audioOutput);
        player.setSource(QUrl::fromLocalFile(filePath));

        // 等待元数据加载
        QEventLoop loop;
        QTimer::singleShot(1000, &loop, &QEventLoop::quit);
        QObject::connect(&player, &QMediaPlayer::mediaStatusChanged, &loop, [&](QMediaPlayer::MediaStatus status) {
            if (status == QMediaPlayer::LoadedMedia || status == QMediaPlayer::InvalidMedia) {
                loop.quit();
            }
        });
        loop.exec();

        // 获取元数据
        if (player.mediaStatus() == QMediaPlayer::LoadedMedia) {
            // 获取时长
            durationStr = formatDuration(player.duration());

            // 获取比特率
            auto metaData = player.metaData();
            QVariant bitRateVariant = metaData.value(QMediaMetaData::AudioBitRate);
            if (bitRateVariant.isValid() && !bitRateVariant.isNull()) {
                int bitrate = bitRateVariant.toInt();
                bitrateStr = QString("%1 kbps").arg(bitrate / 1000);
            }
        }

        // 添加音频信息
        QLabel* audioInfoLabel = new QLabel(
            QString("时长: %1\n比特率: %2").arg(durationStr, bitrateStr));
        audioInfoLabel->setStyleSheet("color: #555; font-size: 10px;");
        layout->addWidget(audioInfoLabel);

        // 文件大小
        QLabel* sizeLabel = new QLabel(formatFileSize(fileInfo.size()));
        sizeLabel->setStyleSheet("color: #555; font-size: 10px;");
        layout->addWidget(sizeLabel);

        // 设置列表项
        itemWidget->setLayout(layout);
        item->setSizeHint(itemWidget->sizeHint());
        listWidget->setItemWidget(item, itemWidget);
    }
}

void MainWindow::updatePlayMode() {
    QString iconPath;
    switch (currentPlayMode) {
    case ListLoop:    iconPath = ":/icon/repeat-one.png";     currentPlayMode = SingleLoop;   break;
    case SingleLoop:  iconPath = ":/icon/random.png"; currentPlayMode = RandomPlay;   break;
    case RandomPlay:  iconPath = ":/icon/repeat.png";     currentPlayMode = ListLoop;   break;
    }
    ui->pushButton_mode->setIcon(QIcon(iconPath));
}

void MainWindow::listLoopMode(){

}

void MainWindow::singleLoopMode(){
    QString filePath = audioFiles[audio_p];
    QMediaPlayer::PlaybackState state = musicPlayer->playbackState();
    while(true){
        if(currentPlayMode==SingleLoop){
            if(state==QMediaPlayer::StoppedState){
                musicPlayer->setSource(QUrl::fromLocalFile(filePath));
                musicPlayer->play();
            }
        }
        else{
            break;
        }
    }
}

void MainWindow::randomPlayMode(){
    QMediaPlayer::PlaybackState state = musicPlayer->playbackState();
    while(true){
        if(currentPlayMode==SingleLoop){
            if(state==QMediaPlayer::StoppedState){
                int randomValue = QRandomGenerator::global()->bounded(audioFiles.size());
                musicPlayer->setSource(QUrl::fromLocalFile(audioFiles[randomValue]));
                musicPlayer->play();
            }
        }
        else{
            break;
        }
    }
}

void MainWindow::onMediaStatusChanged(QMediaPlayer::MediaStatus status) { // 播放到结尾检查状态
    if (status == QMediaPlayer::EndOfMedia) {
        switch (currentPlayMode) {
        case ListLoop:
            nextAudio(); // 播放下一首
            break;
        case SingleLoop:
            musicPlayer->setPosition(0); // 回到开头重新播放
            musicPlayer->play();
            break;
        case RandomPlay:
            playRandom(); // 随机播放一首
            break;
        }
    }
}

void MainWindow::playRandom() {
    if (audioFiles.isEmpty()) return;

    audio_p = QRandomGenerator::global()->bounded(audioFiles.size());
    playCurrent();
}

void MainWindow::playCurrent() {
    if (audio_p < 0 || audio_p >= audioFiles.size()) return;

    QString filePath = audioFiles[audio_p];
    musicPlayer->setSource(QUrl::fromLocalFile(filePath));
    musicPlayer->play();
}

void MainWindow::showCurrentPlay(){
    if (audio_p < 0 || audio_p >= audioFiles.size()) return;
    QFileInfo fileInfo(audioFiles[audio_p]);
    QString audioName = fileInfo.fileName();
    ui->currentPlay->setText("正在播放：" + audioName);
}
