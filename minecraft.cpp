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

#include "minecraft.h"

Minecraft::Minecraft()
{

    name = qgetenv("USER");

    if (name.isEmpty()) {
        name = qgetenv("USERNAME");
        isWindows = true;
    }

    if(isWindows)
        path = "C:/Users/" + name + "/minecraft/installation/";
    else
        path = "/home/" + name + MC_PATH;
}

QString Minecraft::getPath()
{
    return path;
}

void Minecraft::setPath(QString p)
{
    path = p;
}
