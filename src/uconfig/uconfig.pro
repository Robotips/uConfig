QT       += core gui xml widgets

CONFIG += optimize_full c++11

TARGET = uconfig
TEMPLATE = app

PROJECT_ROOT = $$PWD/../..
SOURCE_ROOT = $$PWD/..

DESTDIR = "$$PROJECT_ROOT/bin"
INCLUDEPATH += $$SOURCE_ROOT
DEFINES += KICAD_EXPORT=Q_DECL_IMPORT

SOURCES += $$PWD/uconfig.cpp

HEADERS  +=

unix:{
    QMAKE_LFLAGS_RPATH=
    QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN\'"
}

LIBS += -L"$$PROJECT_ROOT/bin"
LIBS += -lkicad -lpdf_extract
INCLUDEPATH += $$SOURCE_ROOT/kicad
DEPENDPATH += $$SOURCE_ROOT/kicad
