QT += testlib

TEMPLATE = app

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

LIBS += -L"$$PWD/../../bin"
LIBS += -lkicad -lpdf_extract
INCLUDEPATH += $$PWD/../kicad $$PWD/../pdf_extract
DEPENDPATH += $$PWD/../kicad $$PWD/../pdf_extract

SOURCES +=  tst_pic32.cpp
