
TARGET = datasheet_extractor
TEMPLATE = lib
DEFINES += DATASHEET_EXTRACTOR_EXPORT_LIB
DESTDIR = $$PWD/../../bin/

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT       += sql xml network

SOURCES += \
    $$PWD/main.cpp\
    $$PWD/mainwindow.cpp \
    $$PWD/datasheetdownloader.cpp \
    $$PWD/datasheet.cpp \
    $$PWD/kicadexport.cpp \
    $$PWD/datasheetpackage.cpp \
    $$PWD/datasheetpin.cpp \
    $$PWD/datasheetbox.cpp

HEADERS += \
    $$PWD/mainwindow.h \
    $$PWD/datasheetdownloader.h \
    $$PWD/datasheet.h \
    $$PWD/kicadexport.h \
    $$PWD/datasheet_extractor_common.h \
    $$PWD/datasheetpackage.h \
    $$PWD/datasheetpin.h \
    $$PWD/datasheetbox.h

FORMS += $$PWD/mainwindow.ui

include(manufacturers/manufacturers.pri)

LIBS += -L$$PWD/../../bin -lpoppler-qt5
INCLUDEPATH += $$PWD/../../
LIBS += -lkicad
