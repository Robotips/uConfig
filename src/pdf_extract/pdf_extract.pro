QT += core gui widgets
CONFIG += c++11 strict_c++
CONFIG(release, debug|release):CONFIG += optimize_full

# For Appveyor because it dumps includes in the project root
APPVEYOR_BUILD_FOLDER=$$(APPVEYOR_BUILD_FOLDER)
!isEmpty(APPVEYOR_BUILD_FOLDER) {
    INCLUDEPATH += $$APPVEYOR_BUILD_FOLDER
}

TARGET = pdf_extract
TEMPLATE = lib

PROJECT_ROOT = $$PWD/../..
SOURCE_ROOT = $$PWD/..

DESTDIR = "$$PROJECT_ROOT/bin"
INCLUDEPATH += $$SOURCE_ROOT
DEFINES += KICAD_EXPORT=Q_DECL_IMPORT
DEFINES += DATASHEET_EXTRACTOR_EXPORT_LIB

SOURCES += \
    $$PWD/datasheet.cpp \
    $$PWD/datasheetpackage.cpp \
    $$PWD/datasheetpin.cpp \
    $$PWD/datasheetbox.cpp \
    $$PWD/pdfdebugwidget/pdfdebugwidget.cpp \
    $$PWD/pdfdebugwidget/pdfdebugviewer.cpp \
    $$PWD/pdfdebugwidget/pdfdebugscene.cpp \
    $$PWD/pdfdebugwidget/pdfdebugitempage.cpp \
    $$PWD/pdfdebugwidget/pdfdebugitempin.cpp \
    $$PWD/pdfdebugwidget/pdfdebugitemtextbox.cpp \
    $$PWD/model/pdfdatasheet.cpp \
    $$PWD/model/pdfpage.cpp \
    $$PWD/model/pdftextbox.cpp \

HEADERS += \
    $$PWD/pdf_extract_common.h \
    $$PWD/datasheet.h \
    $$PWD/datasheetpackage.h \
    $$PWD/datasheetpin.h \
    $$PWD/datasheetbox.h \
    $$PWD/pdfdebugwidget/pdfdebugwidget.h \
    $$PWD/pdfdebugwidget/pdfdebugviewer.h \
    $$PWD/pdfdebugwidget/pdfdebugscene.h \
    $$PWD/pdfdebugwidget/pdfdebugitempage.h \
    $$PWD/pdfdebugwidget/pdfdebugitempin.h \
    $$PWD/pdfdebugwidget/pdfdebugitemtextbox.h \
    $$PWD/model/pdfdatasheet.h \
    $$PWD/model/pdfpage.h \
    $$PWD/model/pdftextbox.h

LIBS += -L"$$PROJECT_ROOT/bin"
INCLUDEPATH += $$PROJECT_ROOT/
LIBS += -lkicad


macx {
    LIBS += -L /usr/local/lib
    INCLUDEPATH += /usr/local/include 
}

CONFIG += link_pkgconfig
packagesExist(poppler-qt5) {
  PKGCONFIG += poppler-qt5
}
else {
  LIBS += -lpoppler-qt5
}
