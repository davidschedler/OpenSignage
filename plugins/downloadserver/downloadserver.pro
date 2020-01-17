#-------------------------------------------------
#
# Project created by QtCreator 2019-09-05T17:48:42
#
#-------------------------------------------------
VERSION = 1.0.0

QT       += network core
QT       -= gui

CONFIG += c++11

TARGET = downloadserver
TEMPLATE = lib
QTPLUGIN = qDownloadServer
DESTDIR = ../_plugins/cms

DEFINES += DOWNLOADSERVER_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH += $PWD "include"
INCLUDEPATH += ../../kernel/corelibrary/include #PluginInterface

LIBS = -L../../kernel/corelibrary

SOURCES += \
        src/downloadserver.cpp \
        src/downloadserverconfig.cpp \
    src/ssltcpserver.cpp

HEADERS += \
        include/downloadserver.h \
        include/downloadserver_global.h \
        include/downloadserverconfig.h \
    include/ssltcpserver.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
