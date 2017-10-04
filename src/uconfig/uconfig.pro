QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += optimize_full

TARGET = uconfig
TEMPLATE = app
DESTDIR = "$$PWD/../../bin-$$QMAKE_PLATFORM"

SOURCES += uconfig.cpp

HEADERS  +=

LIBS += -Wl,-R.

LIBS += -L"$$PWD/../../bin-$$QMAKE_PLATFORM"
LIBS += -lkicad -lpdf_extract
INCLUDEPATH += $$PWD/../kicad
DEPENDPATH += $$PWD/../kicad
