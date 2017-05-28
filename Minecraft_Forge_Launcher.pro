#-------------------------------------------------
#
# Project created by QtCreator 2016-02-17T17:19:24
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += script

LIBS += -L"E:/OpenSSL-Win32/bin" -llibcrypto-1_1
LIBS += -L"E:/OpenSSL-Win32/bin" -llibssl-1_1

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Minecraft_Forge_Launcher
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    settings.cpp \
    minecraft.cpp

HEADERS  += mainwindow.h \
    settings.h \
    minecraft.h

FORMS    += mainwindow.ui \
    settings.ui
