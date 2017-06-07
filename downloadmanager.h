/*
 * Copyright 2016-2017 MFL Project
 *
 * This is a placeholder until i choose a license
 * that fits my need.
 *
 * Chad Cormier Roussel <chadcormierroussel@gmail.com>
 * Christophe-Andre Gassmann <Christo-Chibougamo@hotmail.com>
 *
 */

#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QFile>
#include <QObject>
#include <QQueue>
#include <QTime>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QEventLoop>
#include <QFileInfo>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QString>
#include <QStringList>
#include <QTimer>
#include <QWaitCondition>
#include <QThread>

struct fileToDownload {
    QUrl url = QUrl("");
    QString name = "";
};

class DownloadManager: public QObject
{    
    Q_OBJECT
public:
    DownloadManager(QObject *parent = 0);
    void append(fileToDownload ftd);
    void downloadFile(fileToDownload ftd);
    void startDownloads();

signals:
    void finished();

private slots:
    void onDownloadFileComplete(QNetworkReply *reply);

private:
    QEventLoop loop;
    QQueue<fileToDownload> fileQueue;
    std::map<QString, QString> filesName;
    fileToDownload dequeue();
    bool isDownloadQueueEmpty();
    int fileCount = 0;
};

#endif
