QT       += core gui xml widgets

CONFIG += optimize_full c++11

TARGET = uconfig_gui
TEMPLATE = app

PROJECT_ROOT = $$PWD/../..
SOURCE_ROOT = $$PWD/..

DESTDIR = "$$PROJECT_ROOT/bin"
INCLUDEPATH += $$SOURCE_ROOT
DEFINES += KICAD_EXPORT=Q_DECL_IMPORT

SOURCES += \
        $$PWD/uconfig_gui.cpp \
        $$PWD/uconfigmainwindow.cpp \
        $$PWD/importer/pinlistimporter.cpp \
        $$PWD/importer/startwizardpage.cpp \
        $$PWD/importer/filepage.cpp \
        $$PWD/importer/pdffilepage.cpp \
        $$PWD/importer/resultspage.cpp \
        $$PWD/importer/datasheetprocesspage.cpp \
        $$PWD/importer/datasheetthread.cpp \
        $$PWD/importer/componentspage.cpp \
        $$PWD/project/uconfigproject.cpp \
        $$PWD/componentinfoseditor.cpp

HEADERS  += \
        $$PWD/uconfigmainwindow.h \
        $$PWD/importer/pinlistimporter.h \
        $$PWD/importer/startwizardpage.h \
        $$PWD/importer/filepage.h \
        $$PWD/importer/pdffilepage.h \
        $$PWD/importer/resultspage.h \
        $$PWD/importer/datasheetprocesspage.h \
        $$PWD/importer/datasheetthread.h \
        $$PWD/importer/componentspage.h \
        $$PWD/project/uconfigproject.h \
        $$PWD/componentinfoseditor.h

RESOURCES += \
        $$PWD/img.qrc

unix:{
    QMAKE_LFLAGS_RPATH=
    QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN\'"
}

LIBS += -L"$$PROJECT_ROOT/bin"
LIBS += -lkicad -lpdf_extract
INCLUDEPATH += $$SOURCE_ROOT/kicad
DEPENDPATH += $$SOURCE_ROOT/kicad

win32 : RC_FILE = uconfig_gui.rc
