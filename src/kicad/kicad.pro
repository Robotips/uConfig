#-------------------------------------------------
#
# Project created by QtCreator 2014-08-05T17:49:45
#
#-------------------------------------------------

QT       += gui printsupport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += optimize_full c++11

TARGET = kicad
TEMPLATE = lib

PROJECT_ROOT = $$PWD/../..
SOURCE_ROOT = $$PWD/..

DESTDIR = "$$PROJECT_ROOT/bin"
INCLUDEPATH += $$SOURCE_ROOT
DEFINES += KICAD_EXPORT=Q_DECL_EXPORT

SOURCES += \
    $$PWD/model/kicad.cpp \
    $$PWD/model/lib.cpp \
    $$PWD/model/component.cpp \
    $$PWD/model/pin.cpp \
    $$PWD/model/libmodule.cpp \
    $$PWD/model/module.cpp \
    $$PWD/model/pad.cpp \
    $$PWD/model/draw.cpp \
    $$PWD/model/drawtext.cpp \
    $$PWD/model/drawarc.cpp \
    $$PWD/model/drawcircle.cpp \
    $$PWD/model/drawpoly.cpp \
    $$PWD/model/drawrect.cpp \
    $$PWD/viewer/kicadfont.cpp \
    $$PWD/viewer/componentwidget.cpp \
    $$PWD/viewer/componentviewer.cpp \
    $$PWD/viewer/componentscene.cpp \
    $$PWD/viewer/componentitem.cpp \
    $$PWD/viewer/pinitem.cpp \
    $$PWD/viewer/drawitem.cpp \
    $$PWD/viewer/drawtextitem.cpp \
    $$PWD/viewer/drawrectitem.cpp \
    $$PWD/viewer/drawpolyitem.cpp \
    $$PWD/viewer/drawcircleitem.cpp \
    $$PWD/itemmodel/componentlibitemmodel.cpp \
    $$PWD/itemmodel/numericalsortfilterproxymodel.cpp \
    $$PWD/itemmodel/componentlibtreeview.cpp \
    $$PWD/itemmodel/componentpinsitemmodel.cpp \
    $$PWD/itemmodel/componentpinssortfilterproxymodel.cpp \
    $$PWD/itemmodel/componentpinstableview.cpp \
    $$PWD/itemmodel/componentpindelegate.cpp \
    $$PWD/itemmodel/pinlisteditor.cpp \
    $$PWD/pinruler/rulesparser.cpp \
    $$PWD/pinruler/rule.cpp \
    $$PWD/pinruler/pinrule.cpp \
    $$PWD/pinruler/classrule.cpp \
    $$PWD/pinruler/rulesset.cpp \
    $$PWD/pinruler/pinclass.cpp \
    $$PWD/pinruler/pinclassitem.cpp \
    $$PWD/pinruler/pinruler.cpp \
    $$PWD/ksseditor/ksseditor.cpp \
    $$PWD/ksseditor/ksseditormargin.cpp \
    $$PWD/ksseditor/ksssyntax.cpp \
    $$PWD/parser/abstractlibparser.cpp \
    $$PWD/parser/libparser.cpp \
    $$PWD/parser/kicadlibparser.cpp \
    $$PWD/schematicsimport/schematicsimporter.cpp \
    $$PWD/schematicsimport/textimporter.cpp

HEADERS += \
    $$PWD/model/kicad.h \
    $$PWD/model/lib.h \
    $$PWD/model/component.h \
    $$PWD/model/pin.h \
    $$PWD/model/libmodule.h \
    $$PWD/model/module.h \
    $$PWD/model/pad.h \
    $$PWD/model/draw.h \
    $$PWD/model/drawtext.h \
    $$PWD/model/drawarc.h \
    $$PWD/model/drawcircle.h \
    $$PWD/model/drawpoly.h \
    $$PWD/model/drawrect.h \
    $$PWD/viewer/kicadfont.h \
    $$PWD/viewer/componentwidget.h \
    $$PWD/viewer/componentviewer.h \
    $$PWD/viewer/componentscene.h \
    $$PWD/viewer/componentitem.h \
    $$PWD/viewer/pinitem.h \
    $$PWD/viewer/drawitem.h \
    $$PWD/viewer/drawtextitem.h \
    $$PWD/viewer/drawrectitem.h \
    $$PWD/viewer/drawpolyitem.h \
    $$PWD/viewer/drawcircleitem.h \
    $$PWD/itemmodel/componentlibitemmodel.h \
    $$PWD/itemmodel/numericalsortfilterproxymodel.h \
    $$PWD/itemmodel/componentlibtreeview.h \
    $$PWD/itemmodel/componentpinsitemmodel.h \
    $$PWD/itemmodel/componentpinssortfilterproxymodel.h \
    $$PWD/itemmodel/componentpinstableview.h \
    $$PWD/itemmodel/componentpindelegate.h \
    $$PWD/itemmodel/pinlisteditor.h \
    $$PWD/pinruler/rulesparser.h \
    $$PWD/pinruler/rule.h \
    $$PWD/pinruler/pinrule.h \
    $$PWD/pinruler/classrule.h \
    $$PWD/pinruler/rulesset.h \
    $$PWD/pinruler/pinclass.h \
    $$PWD/pinruler/pinclassitem.h \
    $$PWD/pinruler/pinruler.h \
    $$PWD/ksseditor/ksseditor.h \
    $$PWD/ksseditor/ksseditormargin.h \
    $$PWD/ksseditor/ksssyntax.h \
    $$PWD/parser/abstractlibparser.h \
    $$PWD/parser/libparser.h \
    $$PWD/parser/kicadlibparser.h \
    $$PWD/schematicsimport/schematicsimporter.h \
    $$PWD/schematicsimport/textimporter.h

RESOURCES += \
    $$PWD/imgviewer.qrc

unix {
    target.path = /usr/lib
    INSTALLS += target
}
