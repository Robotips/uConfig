QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += optimize_full

TARGET = uconfig
TEMPLATE = app
DESTDIR = "$$PWD/../../bin"

SOURCES += $$PWD/uconfig.cpp

HEADERS  +=

LIBS += -Wl,-R.

LIBS += -L"$$PWD/../../bin"
LIBS += -lkicad -lpdf_extract
INCLUDEPATH += $$PWD/../kicad
DEPENDPATH += $$PWD/../kicad
