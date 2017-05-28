#include "minecraft.h"

Minecraft::Minecraft()
{

    name = qgetenv("USER");

    if (name.isEmpty()) {
        name = qgetenv("USERNAME");
        isWindows = true;
    }

    if(isWindows)
        path = "C:\\Users\\" + name + "\\minecraft\\installation\\";
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
