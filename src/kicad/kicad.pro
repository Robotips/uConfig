#-------------------------------------------------
#
# Project created by QtCreator 2014-08-05T17:49:45
#
#-------------------------------------------------

# QT       -= gui

TARGET = kicad
TEMPLATE = lib

DEFINES += KICAD_LIBRARY

SOURCES += kicad.cpp \
        lib.cpp \
        component.cpp \
        pin.cpp \
        libmodule.cpp \
        module.cpp \
        pad.cpp \
        pinlist.cpp \
        pinclassifier.cpp

HEADERS += kicad.h \
        kicad_global.h \
        lib.h \
        component.h \
        pin.h \
        libmodule.h \
        module.h \
        pad.h \
        pinlist.h \
        pinclassifier.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
