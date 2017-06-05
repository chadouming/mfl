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

#ifndef MINECRAFT_H
#define MINECRAFT_H
#include <QString>
#include <QProcess>

#define MC_PATH "/.minecraft"
#define FML_PATH "/.minecraft_fml"

class Minecraft
{
    public:
        explicit Minecraft(QWidget *parent = 0);
        QString getPath();
        void setPath(QString p);
        bool isWindows;

        void setMaxRam(QString n);
        void setMinRam(QString n);
        void setRam(QString min, QString max);
        QString getMaxRam(void);
        QString getMinRam(void);
        void setJVMArgs(QString args);
        QString getJVMArgs();

        void launchMinecraft(QString mcName); //TODO, implement profiles eventually

    private:
        QWidget *parWidget;

        QString path;
        QString name;

        //JVM settings
        QString maxRam;
        QString minRam;

        QString jvmArgs;

};

#endif // MINECRAFT_H
