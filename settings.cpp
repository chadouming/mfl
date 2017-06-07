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

#include "settings.h"
#include "ui_settings.h"
#include "mainwindow.h"
#include <QFileDialog>

Settings::Settings(QWidget *parent, Minecraft* mc) : QDialog(parent), ui(new Ui::Settings)
{
    ui->setupUi(this);

    minecraft = mc;

    if(minecraft!=0){
        ui->mcPathBox->setText(mc->getPath());
    } else {
        ui->mcPathBox->setText("Error getting minecraft internal class o.O");
    }
}

Settings::~Settings()
{
    delete ui;
}

void Settings::on_Browse_pressed()
{
    QString path = QFileDialog::getExistingDirectory();
    ui->mcPathBox->setText(path);
}

void Settings::on_exit_btn_clicked()
{
    this->close();
}

void Settings::on_save_btn_clicked()
{
    if(minecraft != 0) minecraft->setPath(ui->mcPathBox->text());
}
