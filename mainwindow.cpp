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

bool m_isReady = true;
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

void MainWindow::setProgress(int pct)
{
    ui->progressBar->setValue(pct);
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
    QEventLoop loop;
    QUrl url;
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
        qDebug(mc->getPath().toLatin1());

        if(isWindows) {
        } else {
            QDir().rename("/home/" + name + MC_PATH, "/home/" + name + MC_PATH + "_bak");
            qDebug() << "Should be moved from : /home/" + name + MC_PATH + " to : /home/" + name + MC_PATH + "_bak";
            QDir().mkdir("/home/" + name + MC_PATH);
        }

        QDir().mkpath(mc->getPath());

        url = QString("http://s3.amazonaws.com/Minecraft.Download/launcher/Minecraft.jar");
        QString mcName = mc->getPath() + "/minecraft.jar";

        downloadFile(url, mcName);

        connect(manager, SIGNAL(finished(QNetworkReply *)), &loop, SLOT(quit()));
        loop.exec();

        mcProcess->execute("java -jar "+ mcName);

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

        downloadFile(url, forgeName);

        connect(manager, SIGNAL(finished(QNetworkReply *)), &loop, SLOT(quit()));
        loop.exec();

        mesg.setText("This will download and install forge. \nSimply press ok.");
        mesg.exec();

        forgeProcess->execute("java -jar "+ forgeName);

        while(forgeProcess->isOpen())
            QThread::msleep(1000);

        qDebug() << "Removing forge jar";
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

            downloadFile(unzipUrl, unzipName);

            connect(manager, SIGNAL(finished(QNetworkReply *)), &loop, SLOT(quit()));
            loop.exec();
        }

        url = QString(ui->modpackUrl->text());

        if(isWindows)
            modpackName = mc->getPath() + "modpack.zip";
        else
            modpackName = "/home/" + name + MC_PATH + "/modpack.zip";

        QFile modpack(url.toString());

        if(modpack.exists()) {
            modpack.copy(modpackName);
        } else {
            downloadFile(url, modpackName);
            connect(manager, SIGNAL(finished(QNetworkReply *)), &loop, SLOT(quit()));
            loop.exec();
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
            QDir().mkdir(mc->getPath() + "/mods");
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

                        if (reply2->error() == QNetworkReply::NoError) {

                            QEventLoop eventLoop2;

                            // "quit()" the event-loop, when the network request "finished()"
                            QNetworkAccessManager mgr2;
                            QObject::connect(&mgr2, SIGNAL(finished(QNetworkReply*)), &eventLoop2, SLOT(quit()));

                            // the HTTP request
                            QNetworkRequest req2( QUrl( QString("https://minecraft.curseforge.com" + reply2->rawHeader("Location") + "/files/" + entry.property("fileID").toString() + "/download") ) );

                            qDebug().nospace().noquote() << "\n\n https://minecraft.curseforge.com" << reply2->rawHeader("Location") << "/files/" << entry.property("fileID").toString() << "/download \n\n";
                            qDebug().nospace().noquote() << "\n" << reply2->rawHeader("Location") << "\n\n";

                            QNetworkReply *reply3 = mgr2.get(req2);
                            eventLoop2.exec(); // blocks stack until "finished()" has been called

                            QString buff = QString(reply3->rawHeader("Location"));

                            if(req2.HttpStatusCodeAttribute != 302) {
                                // TODO: tell user which mod doesn't exist.
                                continue;
                            }
                            buff.replace("https://addons.cursecdn.com/","https://addons-origin.cursecdn.com/");

                            url = buff;

                            delete(reply3);

                            QFileInfo f(url.toString());
                            QString s1 = f.fileName();

                            ui->modlabel->setText(s1);

                            if(isWindows)
                                modName = mc->getPath() + "/mods/" + s1;
                            else
                                modName = "/home/" + name + MC_PATH + "/mods/" + s1;

                            downloadFile(url, modName);

                            QEventLoop eventLoop3;
                            connect(manager, SIGNAL(finished(QNetworkReply *)), &eventLoop3, SLOT(quit()));
                            eventLoop3.exec();

                        }
                }

                ui->modlabel->setText("Done");
           }

          goto install_minecraft;
    }
}

void MainWindow::downloadFile(QUrl url, QString fileName)
{
    m_file = new QFile();
    m_file->setFileName(fileName);
    m_file->open(QIODevice::WriteOnly);
    m_isReady = false;
    if (!m_file->isOpen()) {
        m_isReady = true;
        return; // TODO: permission check?
    }

    manager = new QNetworkAccessManager;

    QNetworkRequest request;
    //QSslConfiguration config = QSslConfiguration::defaultConfiguration();
    //config.setProtocol(QSsl::TlsV1_2);
    //request.setSslConfiguration(config);
    request.setUrl(QUrl(url));

    connect(manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(onDownloadFileComplete(QNetworkReply *)));

    reply = manager->get(request);

    connect(reply, SIGNAL(downloadProgress(qint64,qint64)), SLOT(downloadProgress(qint64,qint64)));
}

void MainWindow::onDownloadFileComplete(QNetworkReply *reply) {
    if (!m_file->isWritable()) {
        m_isReady = true;
        return; // TODO: error check
    }

    m_file->write(reply->readAll());
    m_file->close(); // TODO: delete the file from the system later on
    m_isReady = true;
}

void MainWindow::on_PassText_returnPressed()
{
    on_LoginButton_clicked();
}

void MainWindow::on_launchButton_clicked()
{

}

void MainWindow::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    ui->downloadBar->setMaximum(bytesTotal);
    ui->downloadBar->setValue(bytesReceived);
    ui->downloadBar->show();
}

bool copy_dir_recursive(QString from_dir, QString to_dir, bool replace_on_conflit)
{
    QDir dir;
    dir.setPath(from_dir);

    from_dir += QDir::separator();
    to_dir += QDir::separator();

    foreach (QString copy_file, dir.entryList(QDir::Files))
    {
        QString from = from_dir + copy_file;
        QString to = to_dir + copy_file;

        if (QFile::exists(to))
        {
            if (replace_on_conflit)
            {
                if (QFile::remove(to) == false)
                {
                    return false;
                }
            }
            else
            {
                continue;
            }
        }

        if (QFile::copy(from, to) == false)
        {
            return false;
        }
    }

    foreach (QString copy_dir, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
    {
        QString from = from_dir + copy_dir;
        QString to = to_dir + copy_dir;

        if (dir.mkpath(to) == false)
        {
            return false;
        }

        if (copy_dir_recursive(from, to, replace_on_conflit) == false)
        {
            return false;
        }
    }
    QDir dir2(from_dir);

    dir2.removeRecursively();

    return true;
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
