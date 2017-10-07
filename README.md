# uConfig

[![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](http://www.gnu.org/licenses/gpl-3.0)
[![Build Status](https://travis-ci.org/Robotips/uConfig.svg?branch=master)](https://travis-ci.org/Robotips/uConfig)
[![Build status](https://ci.appveyor.com/api/projects/status/6nh5atkhvi9w3dfy?svg=true)](https://ci.appveyor.com/project/sebcaux/uconfig)

An old personal project resurrected to extract pinout from PDF datasheet and create Kicad schematics.

![Screenshot](uConfig.png)

Directly save Kicad library files. Work better with huge number of pins.

## How does it work

The extraction of pin mapping from PDF files is done by parsing datasheet. Poppler is used to extract blocks of text and with magic rules,
it sorts the pin numbers and pin labels. Labels and numbers are associated by most relevant pair to create pins. Then, the list of pins is
also sorted and associated by packages. This list of package could be save as a kicad library.

Actually, it exists a command line tool to do that :

```bash
uconfig.exe datasheet.pdf -o lib1.lib
```

It will be save in `lib1.lib` all the schematics of components found in `datasheet.pdf`.

## TODO

- GUI interface
- rules to organize pins on schematics
- non regression tests
- BGA footprint
- multi-pages components

## Bugs

If any component extraction does not work, send an issue in [issues](https://github.com/Robotips/uConfig/issues)
and give the link to the datasheet and page of the rebel component.

## Binaries downloads

For convenience, a pre-built project is available at :

https://ci.appveyor.com/api/projects/sebcaux/uconfig/artifacts/uconfig-win32-v0.zip

## Dependencies and building
### Qt5

uConfig use Qt5 C++ Framework. Version 5.2 is the minimal supported.

To know your Qt version :

```bash
qmake -query QT_VERSION
```

### poppler

poppler is a pdf parser library under GPL v3 license. v0.41 is the minimal supported with qt5 frontend.

To install on linux :

```bash
sudo apt install libpoppler-qt5-dev
```

For windows, build from sources (very annoying) or find a built version here :
[http://blog.alivate.com.au/poppler-windows/](http://blog.alivate.com.au/poppler-windows/)

### build

```bash
cd uConfig
mkdir build
cd build
qmake ../src/uConfig.pro
make -j4
```