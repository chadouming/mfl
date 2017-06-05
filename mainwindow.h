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
#include "settings.h"
#include "downloaderrorui.h"

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
    void onDownloadFileComplete(QNetworkReply *reply);
    void on_launchButton_clicked();
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);

    void on_Browse_clicked();

    void on_actionSettings_triggered();
    void on_actionExit_triggered();

    private:
    // Variable
    Minecraft *mc;
    Settings *sett;
    Ui::MainWindow *ui;
    QString name;
    QNetworkAccessManager *manager;
    QNetworkReply *reply;

    // Methods
    int login();
    void setProgress(int);
    void downloadFile(QUrl url, QString fileName);

};

bool copy_dir_recursive(QString from_dir, QString to_dir, bool replace_on_conflit);

#endif // MAINWINDOW_H
