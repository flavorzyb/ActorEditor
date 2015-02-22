#-------------------------------------------------
#
# Project created by QtCreator 2014-10-28T14:15:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ActorEditor
TEMPLATE = app

INCLUDEPATH += $$PWD

include($$PWD/common.pri)

HEADERS  += $$PWD/include/mainwindow.h \
            $$PWD/include/utils.h \
            $$PWD/include/publisherthread.h

SOURCES +=  $$PWD/src/main.cpp\
            $$PWD/src/mainwindow.cpp \
            $$PWD/src/utils.cpp \
            $$PWD/src/publisherthread.cpp

macx {
    ICON = $$PWD/../ico/actor.icns
    ##LIBS += -framework CoreFoundation
}
