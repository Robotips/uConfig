QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += optimize_full

TARGET = uconfig_gui
TEMPLATE = app
DESTDIR = "$$PWD/../../bin"

SOURCES += \
        $$PWD/uconfig_gui.cpp \
        $$PWD/uconfigmainwindow.cpp \
        $$PWD/importer/pinlistimporter.cpp \
        $$PWD/importer/startwizardpage.cpp \
        $$PWD/importer/filepage.cpp \
        $$PWD/importer/datasheetresultspage.cpp

HEADERS  += \
        $$PWD/uconfigmainwindow.h \
        $$PWD/importer/pinlistimporter.h \
        $$PWD/importer/startwizardpage.h \
        $$PWD/importer/filepage.h \
        $$PWD/importer/datasheetresultspage.h

RESOURCES += \
        $$PWD/img.qrc

LIBS += -Wl,-R.

LIBS += -L"$$PWD/../../bin"
LIBS += -lkicad -lpdf_extract
INCLUDEPATH += $$PWD/../kicad
DEPENDPATH += $$PWD/../kicad
