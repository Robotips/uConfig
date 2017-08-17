TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS += kicad
kicad.path = $$PWD/../bin
kicad.files = $$OUT_PWD/kicad/release/kicad.dll
INSTALLS += kicad

SUBDIRS += test
test.depends = kicad
test.path = $$PWD/../bin
test.files = $$OUT_PWD/test/release/test.exe
INSTALLS += test

