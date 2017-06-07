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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include "minecraft.h"

namespace Ui {
    class Settings;
}

class Settings : public QDialog
{
        Q_OBJECT

    public:
        explicit Settings(QWidget *parent = 0, Minecraft* mc = 0);
        ~Settings();

    private slots:
        void on_Browse_pressed();

        void on_exit_btn_clicked();

        void on_save_btn_clicked();

    private:
        Ui::Settings *ui;
        Minecraft *minecraft;
};

#endif // SETTINGS_H
