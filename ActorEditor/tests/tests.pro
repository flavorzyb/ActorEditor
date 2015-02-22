#-------------------------------------------------
#
# Project created by QtCreator 2014-10-28T20:18:13
#
#-------------------------------------------------

QT       += testlib widgets

QT       -= gui

TARGET = tests
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

include($$PWD/../app/common.pri)

INCLUDEPATH +=  $$PWD \
                $$PWD/../app

INCLUDEPATH += $$PWD/../

HEADERS +=  $$PWD/include/fileutilstest.h \
            $$PWD/include/publishertest.h \
            $$PWD/include/imagetest.h \
            $$PWD/include/plistparsertest.h \
            $$PWD/include/xmlparsertest.h

SOURCES +=  $$PWD/src/main.cpp \
            $$PWD/src/fileutilstest.cpp \
            $$PWD/src/publishertest.cpp \
            $$PWD/src/imagetest.cpp \
            $$PWD/src/plistparsertest.cpp \
            $$PWD/src/xmlparsertest.cpp

QMAKE_POST_LINK =   rm -rf images && cp -rf $$PWD/images ./ && \
                    rm -rf output && mkdir output && \
                    rm -rf data && cp -rf $$PWD/data ./
