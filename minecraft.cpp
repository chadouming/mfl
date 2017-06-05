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

Minecraft::Minecraft(QWidget *parent)
{

    parWidget = parent;

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
