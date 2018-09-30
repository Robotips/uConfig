QT += testlib xml

TEMPLATE = app
DESTDIR = "$$PWD/../../bin"

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

LIBS += -L"$$PWD/../../bin"
LIBS += -lkicad -lpdf_extract
INCLUDEPATH += $$PWD/../kicad $$PWD/../pdf_extract
DEPENDPATH += $$PWD/../kicad $$PWD/../pdf_extract

SOURCES += tst_pdf_extract.cpp

unix:{
    QMAKE_LFLAGS_RPATH=
    QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN\'"
}
