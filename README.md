# uConfig

[![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](http://www.gnu.org/licenses/gpl-3.0)
[![Build Status](https://travis-ci.org/Robotips/uConfig.svg?branch=master)](https://travis-ci.org/Robotips/uConfig)

An old personal project resurrected to extract pinout from PDF datasheet and create kicad schematics.

![Screenshot](uConfig.png)

Directly save kicad library files. Work better with huge number of pins.

## TODO

- BGA footprint
- multipages components
- GUI interface

## dependencies
### Qt5

uConfig use Qt5 C++ Framework. Version 5.2 is the minimal supported.

### poppler

poppler is a pdf parser library. v0.41 is the minimal supported.

On linux :
sudo apt install libpoppler-qt5-dev

For windows, build from sources (very annoying) or find a builded version here :
[http://blog.alivate.com.au/poppler-windows/](http://blog.alivate.com.au/poppler-windows/)
