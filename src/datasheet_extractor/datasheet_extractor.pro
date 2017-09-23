
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
    $$PWD/udatabase.cpp \
    $$PWD/picfile.cpp \
    $$PWD/datasheetdownloader.cpp \
    $$PWD/datasheet.cpp \
    $$PWD/kicadexport.cpp \
    datasheetpackage.cpp \
    datasheetpin.cpp \
    datasheetbox.cpp

HEADERS += \
    $$PWD/mainwindow.h \
    $$PWD/udatabase.h \
    $$PWD/picfile.h \
    $$PWD/datasheetdownloader.h \
    $$PWD/datasheet.h \
    $$PWD/kicadexport.h \
    datasheet_extractor_common.h \
    datasheetpackage.h \
    datasheetpin.h \
    datasheetbox.h

FORMS += $$PWD/mainwindow.ui


include(manufacturers/manufacturers.pri)

LIBS += -L$$PWD/../../bin -lpoppler-qt5
INCLUDEPATH += $$PWD/../../
LIBS += -lkicad
