/**
 ** This file is part of the uConfig project.
 ** Copyright 2018 Robotips sebastien.caux@robotips.fr.
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation, either version 3 of the License, or
 ** (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with this program. If not, see <http://www.gnu.org/licenses/>.
 **/

#ifndef KICADLIBPARSER_H
#define KICADLIBPARSER_H

#include "kicad_global.h"

#include "abstractlibparser.h"

#include <QTextStream>

class KicadLibParser
{
public:
    KicadLibParser();

    virtual Lib *loadLib(const QString &fileName, Lib *lib=Q_NULLPTR);
    virtual bool saveLib(const QString &fileName, Lib *lib);

protected:
    QTextStream _stream;

    void writeLib(Lib *lib);
    void writeComponent(Component *component);
    void writePin(Pin *pin);
    void writeDraw(Draw *draw);

    Component *readComponent();
    Pin *readPin();
    Draw *readDraw(char c);
};

#endif // KICADLIBPARSER_H
