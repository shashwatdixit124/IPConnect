#-------------------------------------------------
#
# Project created by QtCreator 2016-05-20T13:12:24
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = IPConnect
TEMPLATE = app


SOURCES += main.cpp\
        messenger.cpp \
    connectdialog.cpp \
    client.cpp \
    clientmanager.cpp \
    filetransfer.cpp \
    detaildialog.cpp

HEADERS  += messenger.h \
    connectdialog.h \
    client.h \
    clientmanager.h \
    filetransfer.h \
    detaildialog.h

FORMS    += messenger.ui \
    connectdialog.ui \
    detaildialog.ui
