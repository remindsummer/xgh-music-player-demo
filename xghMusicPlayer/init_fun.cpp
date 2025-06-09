#include "mainwindow.h"
#include "./ui_mainwindow.h"


void MainWindow::initButton(){
    ui->pushButton_previous->setFixedSize(50,50);
    ui->pushButton_mode->setFixedSize(50,50);
    ui->pushButton_next->setFixedSize(50,50);
    ui->pushButton_playlist->setFixedSize(50,50);
    ui->pushButton_play->setFixedSize(50,50);
    ui->pushButton_volume->setFixedSize(50,50);

    ui->pushButton_previous->setIcon(QIcon(":/icon/previous.png"));
    ui->pushButton_mode->setIcon(QIcon(":/icon/repeat.png"));
    ui->pushButton_next->setIcon(QIcon(":/icon/next.png"));
    ui->pushButton_volume->setIcon(QIcon(":/icon/volume.png"));
    ui->pushButton_play->setIcon(QIcon(":/icon/pause.png"));
    ui->pushButton_playlist->setIcon(QIcon(":/icon/playlist.png"));

    ui->pushButton_previous->setIconSize(QSize(ui->pushButton_previous->width(), ui->pushButton_previous->height()));
    ui->pushButton_mode->setIconSize(QSize(ui->pushButton_mode->width(), ui->pushButton_mode->height()));
    ui->pushButton_next->setIconSize(QSize(ui->pushButton_next->width(), ui->pushButton_next->height()));
    ui->pushButton_volume->setIconSize(QSize(ui->pushButton_volume->width(), ui->pushButton_volume->height()));
    ui->pushButton_play->setIconSize(QSize(ui->pushButton_play->width(), ui->pushButton_play->height()));
    ui->pushButton_playlist->setIconSize(QSize(ui->pushButton_playlist->width(), ui->pushButton_playlist->height()));

    ui->pushButton_previous->setStyleSheet("background-color:transparent");
    ui->pushButton_mode->setStyleSheet("background-color:transparent");
    ui->pushButton_next->setStyleSheet("background-color:transparent");
    ui->pushButton_volume->setStyleSheet("background-color:transparent");
    ui->pushButton_play->setStyleSheet("background-color:transparent");
    ui->pushButton_playlist->setStyleSheet("background-color:transparent");

    ui->audio_list->setVisible(false);
    ui->vol_widget->setVisible(false);

    ui->pushButton_volume_mute->setFixedSize(30,30);
    ui->pushButton_volume_mute->setIcon(QIcon(":/icon/volume.png"));
    ui->pushButton_volume_mute->setIconSize(QSize(ui->pushButton_volume_mute->width(), ui->pushButton_volume_mute->height()));
    ui->pushButton_volume_mute->setStyleSheet("background-color:transparent");

    ui->horizontalSlider->setRange(0, 100);
    ui->horizontalSlider->setValue(50);
    ui->horizontalSlider->setSingleStep(1);
    ui->horizontalSlider->setPageStep(5);

    ui->currentPlay->setFixedSize(192*2,50);
}
