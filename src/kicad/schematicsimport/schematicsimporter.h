/**
 ** This file is part of the uConfig project.
 ** Copyright 2017-2020 Robotips, Sebastien CAUX (sebcaux)
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

#ifndef SCHEMATICSIMPORTER_H
#define SCHEMATICSIMPORTER_H

#include <QtCore/qglobal.h>

#include "model/component.h"

class KICAD_EXPORT SchematicsImporter
{
public:
    SchematicsImporter();

    virtual bool import(const QString &fileName);

    const QList<Component *> &components() const;

protected:
    QList<Component *> _components;
};

#endif  // SCHEMATICSIMPORTER_H
