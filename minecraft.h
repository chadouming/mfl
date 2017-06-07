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

#define MC_PATH "/.minecraft"
#define FML_PATH "/.minecraft_fml"

class Minecraft
{
    public:
        Minecraft();
        QString getPath();
        void setPath(QString);
        bool isWindows;

    private:
        QString path;
        QString name;
};

#endif // MINECRAFT_H
