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

#include "settings.h"
#include "ui_settings.h"
#include "mainwindow.h"
#include <QFileDialog>


Settings::Settings(QWidget *parent, Minecraft* mc) : QDialog(parent), ui(new Ui::Settings)
{
    minecraft = mc;

    QFile settings(setPath);

    if(settings.exists()){
        readSettings();
    } else {
        saveSettings();
        readSettings();
    }

    ui->setupUi(this);

    if(minecraft!=0){
        ui->mcPathBox->setText(minecraft->getPath());
        ui->maxRamLine->setText(minecraft->getMaxRam());
        ui->minRamLine->setText(minecraft->getMinRam());
        ui->jvmArgsLine->setPlainText(minecraft->getJVMArgs());
    } else {
        ui->mcPathBox->setText("[Settings] Error getting minecraft internal class o.O");
    }

}

Settings::~Settings()
{
    delete ui;
}

bool Settings::saveSettings()
{
    QFile settings(setPath);

    qDebug("[Settings] Saving settings to disc");
    settings.open(QIODevice::WriteOnly);

    QString tempPath = "mcPath:"+minecraft->getPath()+"\n";
    settings.write(tempPath.toLatin1(), strlen(tempPath.toLatin1()));
    QString tempMaxMem = "maxRam:"+minecraft->getMaxRam()+"\n";
    settings.write(tempMaxMem.toLatin1(), strlen(tempMaxMem.toLatin1()));
    QString tempMinMem = "minRam:"+minecraft->getMinRam()+"\n";
    settings.write(tempMinMem.toLatin1(), strlen(tempMinMem.toLatin1()));
    QString tempJVMArgs = "jvmArg:"+minecraft->getJVMArgs()+"\n";
    settings.write(tempJVMArgs.toLatin1(), strlen(tempJVMArgs.toLatin1()));
    settings.close();
    return true;
}

bool Settings::readSettings()
{
    QFile settings(setPath);
    char tempStr[500] = {0};

    settings.open(QIODevice::ReadOnly);
    while(!settings.atEnd()){

        settings.readLine(tempStr, 500);
        QString tempLine(tempStr);
        if(tempLine.startsWith("mcPath:")){
            tempLine.remove(0, 7).trimmed().simplified();
            minecraft->setPath(tempLine);
        }else if(tempLine.startsWith("maxRam:")){
            tempLine.remove(0, 7).trimmed().simplified();
            minecraft->setMaxRam(tempLine);
        }else if(tempLine.startsWith("minRam:")){
            tempLine.remove(0, 7).trimmed().simplified();
            minecraft->setMinRam(tempLine);
        }else if(tempLine.startsWith("jvmArg:")){
            tempLine.remove(0, 7).trimmed().simplified();
            minecraft->setJVMArgs(tempLine);
        }else{
            qDebug("[Settings] unknown line read...");
        }

    }
    settings.close();
    return true;
}

void Settings::on_Browse_pressed()
{
    QString path = QFileDialog::getExistingDirectory();
    ui->mcPathBox->setText(path);
}

void Settings::on_exit_btn_clicked()
{
    //saveSettings
    qDebug("[Settings] exiting settings, saving...");
    if(minecraft != 0){
        minecraft->setPath(ui->mcPathBox->text());
        minecraft->setMaxRam(ui->maxRamLine->text());
        minecraft->setMinRam(ui->minRamLine->text());
        minecraft->setJVMArgs(ui->jvmArgsLine->toPlainText());
        saveSettings();
    }
    else qDebug("[Settings] minecraft does not exist, this isn't normal");

    //exit
    this->close();
}
