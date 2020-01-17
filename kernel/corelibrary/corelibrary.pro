#-------------------------------------------------
#
# Project created by QtCreator 2019-09-04T22:09:13
#
#-------------------------------------------------
VERSION = 1.0.0

QT       += network core
QT       -= gui

CONFIG += c++11

TARGET = corelibrary
TEMPLATE = lib

DEFINES += CORELIBRARY_LIBRARY

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

SOURCES += \
        src/corelibrary.cpp \
        src/customevent.cpp \
        src/eventmanager.cpp \
        src/plugininterface.cpp \
        src/pluginmanager.cpp

HEADERS += \
        include/corelibrary.h \
        include/corelibrary_global.h \
        include/customevent.h \
        include/eventmanager.h \
        include/plugininterface.h \
        include/pluginmanager.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
