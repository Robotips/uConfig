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
    $$PWD/pdfdebugwidget/pdfdebugitemtextbox.cpp \
    $$PWD/model/pdfdatasheet.cpp \
    $$PWD/model/pdfpage.cpp \
    $$PWD/model/pdftextbox.cpp \
    $$PWD/model/pdfpin.cpp \
    $$PWD/model/pdfcomponent.cpp \
    $$PWD/controller/pdfloader.cpp

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
    $$PWD/model/pdftextbox.h \
    $$PWD/model/pdfpin.h \
    $$PWD/model/pdfcomponent.h \
    $$PWD/controller/pdfloader.h

LIBS += -L"$$PWD/../../bin"
LIBS += -lpoppler-qt5
INCLUDEPATH += $$PWD/../../
LIBS += -lkicad

ifneq ($(OS),Windows_NT)
    UNAME_S = $(shell uname -s)
    ifeq ($(UNAME_S),Darwin)
        LIBS += -L /usr/local/lib
        INCLUDEPATH += /usr/local/include
    endif
endif
