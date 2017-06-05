/*
 * Copyright 2016-2017 MFL Project
 *
 * This is a placeholder until i choose a license
 * that fits my need.
 *
 * Chad Cormier Roussel <chadcormierroussel@gmail.com>
 * Christophe-Andre Gassman <Christo-Chibougamo@hotmail.com>
 *
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>
#include <QStringList>
#include "ui_mainwindow.h"
#include "minecraft.h"
#include "downloadmanager.h"
#include "settings.h"

namespace Ui {    
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QFile *m_file;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QString mcPath;

private slots:
    void on_LoginButton_clicked();
    void on_dwnldButton_clicked();
    void on_PassText_returnPressed();
    void on_launchButton_clicked();
    void on_Browse_clicked();

    void on_actionSettings_triggered();
    void on_actionExit_triggered();

    private:
    // Variable
    DownloadManager manager;
    fileToDownload ftd;
    Minecraft *mc;
    QString name;
    QNetworkReply *reply;
    Settings *sett;
    Ui::MainWindow *ui;

    // Methods
    int login();
};

#endif // MAINWINDOW_H
