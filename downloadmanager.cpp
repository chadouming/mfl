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

#include "downloadmanager.h"

DownloadManager::DownloadManager(QObject *parent)
    : QObject(parent)
{
}

void DownloadManager::append(fileToDownload ftd) {
    fileQueue.append(ftd);
}

void DownloadManager::startDownloads() {
    while(!fileQueue.isEmpty())
        downloadFile(fileQueue.dequeue());    
}

void DownloadManager::downloadFile(fileToDownload ftd)
{
    if(ftd.name.isEmpty() || ftd.name.isNull() || ftd.url.isEmpty())
        return;

    filesName[ftd.url.toString()] = ftd.name;
    fileCount++;

    QNetworkRequest request;
    request.setUrl(QUrl(ftd.url));

    QNetworkAccessManager networkManager;

    connect(&networkManager, SIGNAL(finished(QNetworkReply *)), this, SLOT(onDownloadFileComplete(QNetworkReply *)));

    networkManager.get(request);

    connect(&networkManager, SIGNAL(finished(QNetworkReply *)), &loop, SLOT(quit()));
    loop.exec();

    networkManager.deleteLater();
}

void DownloadManager::onDownloadFileComplete(QNetworkReply *reply) {
    QFile m_file;
    m_file.setFileName(filesName[reply->url().toString()].replace("%20", " "));
    m_file.open(QIODevice::WriteOnly);

    if (!m_file.isOpen() || !m_file.isWritable()) {
        return;
    }

    m_file.write(reply->readAll());
    m_file.close();
    fileCount--;

    // Delete ALL THE THANGS
    m_file.deleteLater();
    reply->deleteLater();
    filesName.erase(reply->url().toString());
}
