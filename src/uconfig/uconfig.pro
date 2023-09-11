QT += core gui widgets
CONFIG += c++11 strict_c++
CONFIG(release, debug|release):CONFIG += optimize_full

# For Appveyor because it dumps includes in the project root
APPVEYOR_BUILD_FOLDER=$$(APPVEYOR_BUILD_FOLDER)
!isEmpty(APPVEYOR_BUILD_FOLDER) {
    INCLUDEPATH += $$APPVEYOR_BUILD_FOLDER
}

TARGET = uconfig
TEMPLATE = app

PROJECT_ROOT = $$PWD/../..
SOURCE_ROOT = $$PWD/..

DESTDIR = "$$PROJECT_ROOT/bin"
INCLUDEPATH += $$SOURCE_ROOT
DEFINES += KICAD_EXPORT=Q_DECL_IMPORT

SOURCES += $$PWD/uconfig.cpp

unix {
    QMAKE_LFLAGS_RPATH=
    QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN\'"
}

LIBS += -L"$$PROJECT_ROOT/bin"
LIBS += -lkicad -lpdf_extract
INCLUDEPATH += $$SOURCE_ROOT/kicad
DEPENDPATH += $$SOURCE_ROOT/kicad
