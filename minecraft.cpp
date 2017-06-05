#include "minecraft.h"

Minecraft::Minecraft(QWidget *parent)
{

    parWidget = parent;

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

void Minecraft::setMaxRam(QString n){

    //TODO: check for actual ram quantities
    if( n.right(1) == "M" ||  n.right(1) == "m" || n.right(1) == "G" || n.right(1) == "g" || n.right(1) == "K" || n.right(1) == "k" ||
        n.right(2) == "M\n" ||  n.right(2) == "m\n" || n.right(2) == "G\n" || n.right(2) == "g\n" || n.right(2) == "K\n" || n.right(2) == "k\n" )
        maxRam = n;
    else
        maxRam = n.append('M');
}

void Minecraft::setMinRam(QString n){
    //TODO: check for actual ram quantities
    if( n.right(1) == "M" ||  n.right(1) == "m" || n.right(1) == "G" || n.right(1) == "g" || n.right(1) == "K" || n.right(1) == "k" ||
        n.right(2) == "M\n" ||  n.right(2) == "m\n" || n.right(2) == "G\n" || n.right(2) == "g\n" || n.right(2) == "K\n" || n.right(2) == "k\n" )
        minRam = n;
    else
        minRam = n.append('M');
}

void Minecraft::setRam(QString min, QString max){
    setMaxRam(max);
    if(min < max)
        setMinRam(min);
    else setMinRam("512M");
}

QString Minecraft::getMaxRam(void){
    return maxRam;
}

QString Minecraft::getMinRam(void){
    return minRam;
}

void Minecraft::setJVMArgs(QString args){
    jvmArgs = args;
}

QString Minecraft::getJVMArgs(){
    return jvmArgs;
}

/**
 * Note: might not work yet, be carefull using this.
 * @brief Minecraft::launchMinecraft
 */
void Minecraft::launchMinecraft(QString mcName)
{

    //TODO: make better
    QProcess * mcProcess = new QProcess();
    mcProcess->execute("java  -Xms" + minRam + " -Xmx"+ maxRam + " " + jvmArgs + " -jar " + mcName + " ");

}
