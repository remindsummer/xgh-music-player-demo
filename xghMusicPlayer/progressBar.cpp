#include "mainwindow.h"
#include "./ui_mainwindow.h"

void MainWindow::updateProgressBar(qint64 position){
    ui->progressSlider->setValue(position);
    int seconds = position / 1000;
    int minutes = seconds / 60;
    seconds %= 60;
    ui->timeLabel->setText(QString("%1:%2")
                               .arg(minutes, 2, 10, QLatin1Char('0'))
                               .arg(seconds, 2, 10, QLatin1Char('0')));
}

void MainWindow::on_progressSlider_sliderMoved(int position){
    musicPlayer->setPosition(position);
}

void MainWindow::durationChanged(qint64 duration)
{
    ui->progressSlider->setRange(0, duration); // 设置进度条范围

    int totalSeconds = duration / 1000;
    int totalMinutes = totalSeconds / 60;
    totalSeconds %= 60;
    ui->totalTimeLabel->setText(QString("/%1:%2")
                                .arg(totalMinutes, 2, 10, QLatin1Char('0'))
                                .arg(totalSeconds, 2, 10, QLatin1Char('0')));
}

void MainWindow::onListItemClicked(QListWidgetItem *item)
{
    QString filePath = item->data(Qt::UserRole).toString();
    if (filePath.isEmpty() || !QFile::exists(filePath)) {
        qDebug() << "file don't exist:" << filePath;
        return;
    }
    musicPlayer->setSource(QUrl::fromLocalFile(filePath));
    musicPlayer->play();
    ui->pushButton_play->setIcon(QIcon(":/icon/play.png"));
    for(qint64 i=0;i<audioFiles.size();++i){
        if(audioFiles[i] == filePath){
            audio_p = i;
            break;
        }
    }
    showCurrentPlay();
}
