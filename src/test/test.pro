#-------------------------------------------------
#
# Project created by QtCreator 2014-08-06T09:40:31
#
#-------------------------------------------------

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += optimize_full

TARGET = test
TEMPLATE = app
DESTDIR = "$$PWD/../../bin"

SOURCES += $$PWD/main.cpp \
    $$PWD/test_libkicad.cpp

HEADERS  +=

LIBS += -L"$$PWD/../../bin"
LIBS += -lkicad -lpdf_extract
INCLUDEPATH += $$PWD/../kicad
DEPENDPATH += $$PWD/../kicad
