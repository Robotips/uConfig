#-------------------------------------------------
#
# Project created by QtCreator 2014-08-06T09:40:31
#
#-------------------------------------------------

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../kicad/release/ -lkicad
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../kicad/debug/ -lkicad
else:unix: LIBS += -L$$OUT_PWD/../kicad/ -lkicad
INCLUDEPATH += $$PWD/../kicad
DEPENDPATH += $$PWD/../kicad
