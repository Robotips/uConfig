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
    $$PWD/viewer/componentviewer.cpp \
    $$PWD/viewer/componentitem.cpp \
    $$PWD/viewer/pinitem.cpp \
    $$PWD/itemmodel/componentlibitemmodel.cpp \
    $$PWD/itemmodel/componentpinsitemmodel.cpp \
    $$PWD/itemmodel/componentlibtreeview.cpp \
    $$PWD/itemmodel/componentpinstableview.cpp \
    $$PWD/itemmodel/numericalsortfilterproxymodel.cpp \
    $$PWD/pinruler/rulesparser.cpp \
    $$PWD/pinruler/rule.cpp \
    $$PWD/pinruler/pinrule.cpp \
    $$PWD/pinruler/classrule.cpp \
    $$PWD/pinruler/rulesset.cpp \
    $$PWD/pinruler/pinclass.cpp \
    $$PWD/pinruler/pinruler.cpp \
    $$PWD/ksseditor/ksseditor.cpp

HEADERS += \
    $$PWD/kicad.h \
    $$PWD/kicad_global.h \
    $$PWD/lib.h \
    $$PWD/component.h \
    $$PWD/pin.h \
    $$PWD/libmodule.h \
    $$PWD/module.h \
    $$PWD/pad.h \
    $$PWD/viewer/componentviewer.h \
    $$PWD/viewer/componentitem.h \
    $$PWD/viewer/pinitem.h \
    $$PWD/itemmodel/componentlibitemmodel.h \
    $$PWD/itemmodel/componentpinsitemmodel.h \
    $$PWD/itemmodel/componentlibtreeview.h \
    $$PWD/itemmodel/componentpinstableview.h \
    $$PWD/itemmodel/numericalsortfilterproxymodel.h \
    $$PWD/pinruler/rulesparser.h \
    $$PWD/pinruler/rule.h \
    $$PWD/pinruler/pinrule.h \
    $$PWD/pinruler/classrule.h \
    $$PWD/pinruler/rulesset.h \
    $$PWD/pinruler/pinclass.h \
    $$PWD/pinruler/pinruler.h \
    $$PWD/ksseditor/ksseditor.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
