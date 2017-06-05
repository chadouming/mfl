#-------------------------------------------------
#
# Project created by QtCreator 2016-02-17T17:19:24
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += script
QT       += widgets

# OpenSSL path (Change to fit your need)
LIBS     += -L"C:/OpenSSL-Win32/bin" -llibcrypto-1_1
LIBS     += -L"C:/OpenSSL-Win32/bin" -llibssl-1_1

TARGET    = Minecraft_Forge_Launcher
TEMPLATE  = app

SOURCES  += main.cpp\
            mainwindow.cpp \
            settings.cpp \
            minecraft.cpp

HEADERS  += mainwindow.h \
            settings.h \
            minecraft.h

FORMS    += mainwindow.ui \
            settings.ui
