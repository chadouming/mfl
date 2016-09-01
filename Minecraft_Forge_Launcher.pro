#-------------------------------------------------
#
# Project created by QtCreator 2016-02-17T17:19:24
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += script

LIBS    += -lssl
LIBS    += -lcrypto

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Minecraft_Forge_Launcher
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
