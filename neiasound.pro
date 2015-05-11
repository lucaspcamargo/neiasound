#-------------------------------------------------
#
# Project created by QtCreator 2015-05-11T22:43:48
#
#-------------------------------------------------

QT       -= gui

TARGET = neiasound
TEMPLATE = lib

DEFINES += NEIASOUND_LIBRARY

SOURCES += nsoundroot.cpp

HEADERS += nsoundroot.h\
        neiasound_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
