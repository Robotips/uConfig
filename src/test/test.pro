#-------------------------------------------------
#
# Project created by QtCreator 2014-08-06T09:40:31
#
#-------------------------------------------------

QT       += core gui printsupport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += optimize_full c++11

TARGET = test
TEMPLATE = app

PROJECT_ROOT = $$PWD/../..
SOURCE_ROOT = $$PWD/..

DESTDIR = "$$PROJECT_ROOT/bin"
INCLUDEPATH += $$SOURCE_ROOT
DEFINES += KICAD_EXPORT=Q_DECL_IMPORT

SOURCES += $$PWD/main.cpp \
    $$PWD/test_libkicad.cpp

HEADERS  +=

LIBS += -L"$$PROJECT_ROOT/bin"
LIBS += -lkicad -lpdf_extract
INCLUDEPATH += $$SOURCE_ROOT/kicad $$SOURCE_ROOT/pdf_extract
DEPENDPATH += $$SOURCE_ROOT/kicad $$SOURCE_ROOT/pdf_extract
