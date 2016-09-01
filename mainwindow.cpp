#include "mainwindow.h"
#include "ui_mainwindow.h"
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


bool m_isReady = true;
bool isWindows = false;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QDir dir;

    ui->setupUi(this);
    ui->progressBar->setValue(0);

    name = qgetenv("USER");

    if (name.isEmpty()) {
        name = qgetenv("USERNAME");
        isWindows = true;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
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
    QString mcPath = "";
    QString unzipName = "";
    QString modpackName = "";
    QString dirname = "";
    QString modName = "";

    if(isWindows)
        mcPath = "C:\\Users\\" + name + "\\minecraft\\installation\\";
    else
        mcPath = "/home/" + name + MC_PATH;



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
        qDebug(mcPath.toLatin1());

        if(isWindows) {
        } else {
            QDir().rename("/home/" + name + MC_PATH, "/home/" + name + MC_PATH + "_bak");
            qDebug() << "Should be moved from : /home/" + name + MC_PATH + " to : /home/" + name + MC_PATH + "_bak";
            QDir().mkdir("/home/" + name + MC_PATH);
        }

        QDir().mkpath(mcPath);

        url = QString("http://s3.amazonaws.com/Minecraft.Download/launcher/Minecraft.jar");
        QString mcName = mcPath + "\\minecraft.jar";

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
            forgeName = mcPath + "/forge-installer.jar";
        else
            forgeName = mcPath + "/forge-" + versionid + forgeid.replace("forge", "") + "-installer.jar";

        downloadFile(url, forgeName);

        connect(manager, SIGNAL(finished(QNetworkReply *)), &loop, SLOT(quit()));
        loop.exec();

        mesg.setText("This will download and install forge. \nSimply press ok.");
        mesg.exec();

        forgeProcess->execute("java -jar "+ forgeName);

        while(forgeProcess->isOpen())
            QThread::msleep(1000);

        qDebug() << "Removing forge jar";
        QFile::remove(forgeName);

        return;
}
install_modpack: {
        if(isWindows) {
            QString unzipUrl = "http://stahlworks.com/dev/unzip.exe";
            unzipName = mcPath + "unzip.exe";

            downloadFile(unzipUrl, unzipName);

            connect(manager, SIGNAL(finished(QNetworkReply *)), &loop, SLOT(quit()));
            loop.exec();
        }

        url = QString(ui->modpackUrl->text());
        if(isWindows)
            modpackName = mcPath + "/modpack.zip";
        else
            modpackName = "/home/" + name + MC_PATH + "/modpack.zip";

        downloadFile(url, modpackName);

        connect(manager, SIGNAL(finished(QNetworkReply *)), &loop, SLOT(quit()));
        loop.exec();

        if(isWindows)
            modpackProcess->execute(unzipName + " -q -o " + modpackName + " -d " + mcPath);
        else
            modpackProcess->execute("unzip -o "+ modpackName + " -d " + "/home/" + name + MC_PATH + "/");

        dir.remove(unzipName);
        dir.remove(modpackName);

        while(modpackProcess->isOpen())
            QThread::msleep(1000);

        copy_dir_recursive(mcPath + "overrides", mcPath, true);

        QFile file;
        if(isWindows)
            file.setFileName(mcPath + "/manifest.json");
        else
            file.setFileName("/home/" + name + MC_PATH + "/manifest.json");
        file.open(QIODevice::ReadOnly | QIODevice::Text);

        QString data = QString(file.readAll());

        QScriptEngine engine;
        QScriptValue result = engine.evaluate("(" + data + ")");

        if(isWindows)
            QDir().mkdir(mcPath + "/mods");
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
                        QNetworkRequest req( QUrl( QString("http://minecraft.curseforge.com/mc-mods/" + entry.property("projectID").toString() + "?cookieTest=0") ) );
                        QNetworkReply *reply2 = mgr.get(req);
                        eventLoop.exec(); // blocks stack until "finished()" has been called

                        if (reply2->error() == QNetworkReply::NoError) {

                            QEventLoop eventLoop2;

                            // "quit()" the event-loop, when the network request "finished()"
                            QNetworkAccessManager mgr2;
                            QObject::connect(&mgr2, SIGNAL(finished(QNetworkReply*)), &eventLoop2, SLOT(quit()));

                            // the HTTP request
                            QNetworkRequest req2( QUrl( QString("http://minecraft.curseforge.com" + reply2->rawHeader("location") + "/files/" + entry.property("fileID").toString() + "/download?cookieTest=0") ) );

                            QNetworkReply *reply3 = mgr2.get(req2);
                            eventLoop2.exec(); // blocks stack until "finished()" has been called

                            QString buff = QString(reply3->rawHeader("location"));

                            buff.replace("http://addons.cursecdn.com/","http://addons-origin.cursecdn.com/");

                            url = buff;

                            delete(reply3);

                            QFileInfo f(url.toString());
                            QString s1 = f.fileName();

                            ui->modlabel->setText(s1);

                            if(isWindows)
                                modName = mcPath + "/mods/" + s1;
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

    qDebug(from_dir.toLatin1());

    foreach (QString copy_file, dir.entryList(QDir::Files))
    {
        qDebug("I'M HERE");
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
