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
    //qDebug("\nReturning path : ");
    //qDebug(path.toLatin1());
    return path;
}

void Minecraft::setPath(QString p)
{
    //qDebug("\nOld path was : ");
    //qDebug(path.toLatin1());
    path = p;
    //qDebug("\nNew path is : ");
    //qDebug(path.toLatin1());
    //qDebug("\nIs supposed to be : ");
    //qDebug(p.toLatin1());
}
