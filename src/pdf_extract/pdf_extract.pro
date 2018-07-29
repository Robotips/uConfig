QT     += core gui widgets xml

TARGET = pdf_extract
TEMPLATE = lib
DEFINES += DATASHEET_EXTRACTOR_EXPORT_LIB
DESTDIR = "$$PWD/../../bin"

CONFIG(release, debug|release) {
    CONFIG += optimize_full
}

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
    $$PWD/model/pdfdatasheet.cpp \
    $$PWD/model/pdfpage.cpp \
    model/pdfrect.cpp \
    model/pdfpin.cpp \
    model/pdfcomponent.cpp

HEADERS += \
    $$PWD/datasheet_extractor_common.h \
    $$PWD/datasheet.h \
    $$PWD/datasheetpackage.h \
    $$PWD/datasheetpin.h \
    $$PWD/datasheetbox.h \
    $$PWD/pdfdebugwidget/pdfdebugwidget.h \
    $$PWD/pdfdebugwidget/pdfdebugviewer.h \
    $$PWD/pdfdebugwidget/pdfdebugscene.h \
    $$PWD/pdfdebugwidget/pdfdebugitempage.h \
    $$PWD/pdfdebugwidget/pdfdebugitempin.h \
    $$PWD/model/pdfdatasheet.h \
    $$PWD/model/pdfpage.h \
    model/pdfrect.h \
    model/pdfpin.h \
    model/pdfcomponent.h

LIBS += -L"$$PWD/../../bin"
LIBS += -lpoppler-qt5
INCLUDEPATH += $$PWD/../../
LIBS += -lkicad
