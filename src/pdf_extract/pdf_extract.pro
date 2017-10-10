
TARGET = pdf_extract
TEMPLATE = lib
DEFINES += DATASHEET_EXTRACTOR_EXPORT_LIB
DESTDIR = "$$PWD/../../bin"

CONFIG += optimize_full

QT       += core gui

QT       += xml

SOURCES += \
    $$PWD/datasheet.cpp \
    $$PWD/datasheetpackage.cpp \
    $$PWD/datasheetpin.cpp \
    $$PWD/datasheetbox.cpp

HEADERS += \
    $$PWD/datasheet_extractor_common.h \
    $$PWD/datasheet.h \
    $$PWD/datasheetpackage.h \
    $$PWD/datasheetpin.h \
    $$PWD/datasheetbox.h

LIBS += -L"$$PWD/../../bin"
LIBS += -lpoppler-qt5
INCLUDEPATH += $$PWD/../../
LIBS += -lkicad
