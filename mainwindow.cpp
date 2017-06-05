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

#include "downloadmanager.h"
#include "helper.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settings.h"
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QMessageBox>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDir>
#include <QFile>
#include <QProcess>
#include <QMessageBox>
#include <QThread>
#include <QSsl>
#include <QScriptEngine>
#include <QScriptValue>
#include <QScriptValueIterator>
#include <QDebug>
#include <QFileDialog>
#include <qiterator.h>

bool isWindows = false;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QDir dir;

    mc = new Minecraft(this);
    isWindows = mc->isWindows;

    sett = new Settings(this, mc);

    ui->setupUi(this);
    ui->progressBar->setValue(0);
    connect(ui->Browse, SIGNAL(clicked(bool)), this, SLOT(on_Browse_clicked()));

}

MainWindow::~MainWindow()
{
    sett->saveSettings();

    delete ui;
    delete sett;
}

void MainWindow::on_LoginButton_clicked()
{
    if(ui->LoginText->text() == "")
        ui->ErrorLabel->setText("Need a login (email)");
    else if(ui->PassText->text() == "")
        ui->ErrorLabel->setText("Need to enter your password please");
    else
        MainWindow::login();
}

int MainWindow::login()
{
    QVariantMap jsonArray;

    jsonArray.insert("username", ui->LoginText->text());
    jsonArray.insert("password", ui->PassText->text());

    const QJsonDocument doc = QJsonDocument::fromVariant(jsonArray);

    QByteArray postData = doc.toJson();
    QString strData = doc.toJson();

    QByteArray postDataSize = QByteArray::number(postData.size());

    QUrl req("https://authserver.mojang.com/authenticate");

    QNetworkRequest request(req);

    request.setRawHeader("User-Agent", "Test");
    request.setRawHeader("X-Custom-User-Agent", "Test");
    request.setRawHeader("Content-Type", "application/json");
    request.setRawHeader("Content-Length", postDataSize);

    QNetworkAccessManager test;

    QEventLoop loop;
    QObject::connect(&test, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));
    QNetworkReply *reply = test.post(request, postData);
    loop.exec();

    QString response = (QString)reply->readAll();


    QJsonDocument jsonResponse = QJsonDocument::fromJson(response.toUtf8());
    QJsonObject jsonObj = jsonResponse.object();

    delete reply;

    return 0;
}

void MainWindow::on_dwnldButton_clicked()
{
    QDir dir;
    QString url;
    QMessageBox mesg;
    QProcess * mcProcess = new QProcess(this);
    QProcess * forgeProcess = new QProcess(this);
    QProcess * modpackProcess = new QProcess(this);
    QString forgeid = "";
    QString versionid = "";
    QString unzipName = "";
    QString modpackName = "";
    QString dirname = "";
    QString modName = "";

    mesg.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    mesg.setText("This will download, launch then leave Minecraft. \nForge setup will continue after.");

    int agree = mesg.exec();
    if(agree == QMessageBox::Ok) {

        if(ui->modpackUrl->text().isEmpty()){
            ui->progressBar->setValue(10);
            goto install_minecraft;
         } else {
           ui->progressBar->setValue(30);
           //backed = true;
           goto install_modpack;
        }
    } else {
        return;
    }


install_minecraft: {
        QDir().mkpath(mc->getPath());

        ftd.url = "https://s3.amazonaws.com/Minecraft.Download/indexes/" + versionid.split(".")[0] + "." + versionid.split(".")[1] + ".json";
        ftd.name = mc->getPath() + "resources.json";

        manager.append(ftd);
        manager.startDownloads();

        QFile file2;
        if(isWindows)
            file2.setFileName(mc->getPath() + "resources.json");
        else
            file2.setFileName("/home/" + name + MC_PATH + "manifest.json");

        file2.open(QIODevice::ReadOnly | QIODevice::Text);

        auto data2 = file2.readAll();
        QJsonDocument jdoc = QJsonDocument::fromJson(data2);

        QJsonObject root = jdoc.object();

        QJsonValue objects = root.value("objects");
        QVariantMap map = objects.toVariant().toMap();

        ui->progressBar->setMaximum(map.count());

        for (QVariantMap::const_iterator iter = map.begin(); iter != map.end(); ++iter)
        {
            QVariant variant = iter.value();
            QVariantMap nested_objects = variant.toMap();

            for (QVariantMap::const_iterator nested_iter = nested_objects.begin();
                 nested_iter != nested_objects.end(); ++nested_iter)
            {
                QString key = nested_iter.key();
                QVariant value = nested_iter.value();

                QString hash = "";

                if (key == "hash")
                {
                    hash = value.toString();
                } else
                    continue;

                if(hash.isEmpty() || hash.isNull())
                    continue;

                QDir dir2(mc->getPath() + "resources");
                if(!dir2.exists())
                    dir2.mkpath(".");

                QFile resourceFile(mc->getPath() + "resources/" + iter.key());
                if(!QFileInfo(resourceFile).absoluteDir().exists())
                    QFileInfo(resourceFile).absoluteDir().mkpath(".");

                ftd.url = "https://resources.download.minecraft.net/" + hash.left(2) + "/" + hash;
                ftd.name = mc->getPath() + "/resources/" + iter.key();

                manager.append(ftd);
                ui->progressBar->setValue(ui->progressBar->value() + 1);
            }
            manager.startDownloads();
        }

        while(mcProcess->isOpen())
            QThread::msleep(1000);

        goto install_forge;

        return;
}
install_forge: {
        if(ui->modpackUrl->text().isEmpty())
            url = QString("http://files.minecraftforge.net/maven/net/minecraftforge/forge/1.8.9-11.15.1.1855/forge-1.8.9-11.15.1.1855-installer.jar");
        else
            url = QString("http://files.minecraftforge.net/maven/net/minecraftforge/forge/" + versionid + forgeid.replace("forge","") + "/forge-" + versionid + forgeid.replace("forge", "") + "-installer.jar");

        QString forgeName = "";

        if(ui->modpackUrl->text().isEmpty())
            forgeName = mc->getPath() + "/forge-installer.jar";
        else
            forgeName = mc->getPath() + "/forge-" + versionid + forgeid.replace("forge", "") + "-installer.jar";

        ftd.url = url;
        ftd.name = forgeName;

        manager.append(ftd);
        manager.startDownloads();

        mesg.setText("This will download and install forge. \nSimply press ok.");
        mesg.exec();

        forgeProcess->execute("java -jar "+ forgeName);

        while(forgeProcess->isOpen())
            QThread::msleep(1000);

        //QFile::remove(forgeName);

        return;
}
install_modpack: {

        QDir dir(mc->getPath());
        if(!dir.exists()) {
            dir.mkpath(".");
        }

        if(isWindows) {
            QString unzipUrl = "http://stahlworks.com/dev/unzip.exe";
            unzipName = mc->getPath() + "/unzip.exe";

            ftd.url = unzipUrl;
            ftd.name = unzipName;

            manager.append(ftd);
            manager.startDownloads();

        }

        url = QString(ui->modpackUrl->text());

        if(isWindows)
            modpackName = mc->getPath() + "modpack.zip";
        else
            modpackName = "/home/" + name + MC_PATH + "/modpack.zip";

        QFile modpack(url);

        if(modpack.exists()) {
            modpack.copy(modpackName);
        } else {
            ftd.url = url;
            ftd.name = modpackName;

            manager.append(ftd);
            manager.startDownloads();
        }

        if(isWindows)
            modpackProcess->execute(unzipName + " -q -o " + modpackName + " -d " + mc->getPath());
        else
            modpackProcess->execute("unzip -o "+ modpackName + " -d " + "/home/" + name + MC_PATH + "/");

        //dir.remove(unzipName);
        //dir.remove(modpackName);

        while(modpackProcess->isOpen())
            QThread::msleep(1000);

        copy_dir_recursive(mc->getPath() + "overrides", mc->getPath(), true);

        QFile file;
        if(isWindows)
            file.setFileName(mc->getPath() + "manifest.json");
        else
            file.setFileName("/home/" + name + MC_PATH + "manifest.json");

        file.open(QIODevice::ReadOnly | QIODevice::Text);

        QString data = QString(file.readAll());

        QScriptEngine engine;
        QScriptValue result = engine.evaluate("(" + data + ")");

        if(isWindows)
            QDir().mkdir(mc->getPath() + "mods");
        else
            QDir().mkdir("/home/" + name + MC_PATH + "/mods");

        QScriptValue entries = result.property("minecraft").property("modLoaders");
        QScriptValueIterator it(entries);

          while (it.hasNext()) {
              it.next();
              QScriptValue entry = it.value();
              if(entry.property("id").toString() != "")
                forgeid = entry.property("id").toString();
          }

          entries = result.property("minecraft");
          QScriptValueIterator it3(entries);

            while (it3.hasNext()) {
                it3.next();
                QScriptValue entry = it3.value();
                if(it3.name() == "version")
                  versionid = entry.toString();
            }

          entries = result.property("files");
          QScriptValueIterator it2(entries);
          while (it2.hasNext()) {
             it2.next();
              QScriptValue entry = it2.value();
                if(entry.property("projectID").toString() != "") {

                    // create custom temporary event loop on stack
                        QEventLoop eventLoop;

                        // "quit()" the event-loop, when the network request "finished()"
                        QNetworkAccessManager mgr;
                        QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

                        // the HTTP request
                        QNetworkRequest req( QUrl( QString("https://minecraft.curseforge.com/projects/" + entry.property("projectID").toString()) ) );
                        QNetworkReply *reply2 = mgr.get(req);
                        eventLoop.exec(); // blocks stack until "finished()" has been called
                        eventLoop.deleteLater();

                        if (reply2->error() == QNetworkReply::NoError) {

                            QEventLoop eventLoop2;

                            // "quit()" the event-loop, when the network request "finished()"
                            QNetworkAccessManager mgr2;
                            QObject::connect(&mgr2, SIGNAL(finished(QNetworkReply*)), &eventLoop2, SLOT(quit()));

                            // the HTTP request
                            QNetworkRequest req2( QUrl( QString("https://minecraft.curseforge.com" + reply2->rawHeader("Location") + "/files/" + entry.property("fileID").toString() + "/download?cookieTest=1") ) );

                            reply2->deleteLater();

                            QNetworkReply *reply3 = mgr2.get(req2);
                            eventLoop2.exec(); // blocks stack until "finished()" has been called
                            eventLoop2.deleteLater();

                            QString buff = QString(reply3->rawHeader("Location"));

                            reply3->deleteLater();

                            if(req2.HttpStatusCodeAttribute == 404) {
                                // TODO: tell user which mod doesn't exist.
                                continue;
                            }
                            buff.replace("https://addons.cursecdn.com/","https://addons-origin.cursecdn.com/");

                            url = buff;

                            QFileInfo f(url);
                            QString s1 = f.fileName();

                            if(isWindows)
                                modName = mc->getPath() + "/mods/" + s1;
                            else
                                modName = "/home/" + name + MC_PATH + "/mods/" + s1;

                            ftd.url = url;
                            ftd.name = modName;

                            manager.append(ftd);
                        }
                }
           }
          manager.startDownloads();

          goto install_minecraft;
    }
}

void MainWindow::on_PassText_returnPressed()
{
    on_LoginButton_clicked();
}

void MainWindow::on_launchButton_clicked()
{

}

void MainWindow::on_Browse_clicked()
{
    QString packZip = QFileDialog::getOpenFileName(this, tr("Open pack archive"), "/home", tr("Archives (*.zip)"));
    ui->modpackUrl->setText(packZip);
}

void MainWindow::on_actionSettings_triggered()
{
    Settings *set = new Settings(this, mc);
    set->show();
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}
