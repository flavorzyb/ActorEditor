QT += gui xml

include($$PWD/../library/pvr/pvr.pri)

INCLUDEPATH += $$PWD

LIBS += -lz

HEADERS +=  $$PWD/include/fileutils.h \
            $$PWD/include/settingsvo.h \
            $$PWD/include/publisher.h \
            $$PWD/include/Rect.h \
            $$PWD/include/MaxRectsBinPack.h \
            $$PWD/include/framevo.h \
            $$PWD/include/image.h \
            $$PWD/include/macros.h \
            $$PWD/include/imageutils.h \
            $$PWD/include/plistparser.h \
            $$PWD/include/ziputils.h \
            $$PWD/include/xmlparser.h

SOURCES +=  $$PWD/src/fileutils.cpp \
            $$PWD/src/settingsvo.cpp \
            $$PWD/src/publisher.cpp \
            $$PWD/src/Rect.cpp \
            $$PWD/src/MaxRectsBinPack.cpp \
            $$PWD/src/framevo.cpp \
            $$PWD/src/image.cpp \
            $$PWD/src/imageutils.cpp \
            $$PWD/src/plistparser.cpp \
            $$PWD/src/ziputils.cpp \
            $$PWD/src/xmlparser.cpp
