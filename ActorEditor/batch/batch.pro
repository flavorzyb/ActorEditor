#-------------------------------------------------
#
# Project created by QtCreator 2014-10-30T13:59:51
#
#-------------------------------------------------

QT       += core xml

QT       -= gui

TARGET = batch
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

DEFINES += SKIP_ROTATED_ERROR

include($$PWD/../app/common.pri)

INCLUDEPATH +=  $$PWD \
                $$PWD/../app

SOURCES += main.cpp \
    batchapplication.cpp

HEADERS += \
    batchapplication.h
