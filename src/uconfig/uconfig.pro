QT       += core gui xml widgets

CONFIG += optimize_full c++11

TARGET = uconfig
TEMPLATE = app
DESTDIR = "$$PWD/../../bin"

SOURCES += $$PWD/uconfig.cpp

HEADERS  +=

unix:{
    QMAKE_LFLAGS_RPATH=
    QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN\'"
}

LIBS += -L"$$PWD/../../bin"
LIBS += -lkicad -lpdf_extract
INCLUDEPATH += $$PWD/../kicad
DEPENDPATH += $$PWD/../kicad
