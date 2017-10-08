#-------------------------------------------------
#
# Project created by QtCreator 2014-08-05T17:49:45
#
#-------------------------------------------------

QT       += gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = kicad
TEMPLATE = lib
DESTDIR = "$$PWD/../../bin"

DEFINES += KICAD_EXPORT_LIB

SOURCES += \
    $$PWD/kicad.cpp \
    $$PWD/lib.cpp \
    $$PWD/component.cpp \
    $$PWD/pin.cpp \
    $$PWD/libmodule.cpp \
    $$PWD/module.cpp \
    $$PWD/pad.cpp \
    $$PWD/pinlist.cpp \
    $$PWD/pinclassifier.cpp \
    $$PWD/viewer/componentviewer.cpp \
    $$PWD/viewer/componentitem.cpp \
    $$PWD/viewer/pinitem.cpp \
    $$PWD/pinruler/rulesparser.cpp \
    $$PWD/itemmodel/componentlibitemmodel.cpp \
    $$PWD/itemmodel/componentpinsitemmodel.cpp \
    $$PWD/itemmodel/componentlibtreeview.cpp \
    $$PWD/itemmodel/componentpinstableview.cpp

HEADERS += \
    $$PWD/kicad.h \
    $$PWD/kicad_global.h \
    $$PWD/lib.h \
    $$PWD/component.h \
    $$PWD/pin.h \
    $$PWD/libmodule.h \
    $$PWD/module.h \
    $$PWD/pad.h \
    $$PWD/pinlist.h \
    $$PWD/pinclassifier.h \
    $$PWD/viewer/componentviewer.h \
    $$PWD/viewer/componentitem.h \
    $$PWD/viewer/pinitem.h \
    $$PWD/pinruler/rulesparser.h \
    $$PWD/itemmodel/componentlibitemmodel.h \
    $$PWD/itemmodel/componentpinsitemmodel.h \
    $$PWD/itemmodel/componentlibtreeview.h \
    $$PWD/itemmodel/componentpinstableview.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
