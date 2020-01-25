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

#include "datasheetpackage.h"

DatasheetPackage::DatasheetPackage()
{
}

DatasheetPackage::~DatasheetPackage()
{
    for (int i=0; i<pins.size(); i++)
        delete pins[i];
}

Component *DatasheetPackage::toComponent() const
{
    Component *comp = new Component();
    if (!icname.isEmpty())
        comp->setName(icname.first());
    else
        comp->setName(name);

    for (int i = 1; i < icname.count(); i++)
        comp->addAlias(icname.at(i));

    foreach (DatasheetPin *dpin, pins)
    {
        Pin *pin = new Pin();
        pin->setName(dpin->name);
        pin->setElectricalType(dpin->electricalType);
        pin->setPadName(QString::number(dpin->pin));
        comp->addPin(pin);
    }
    comp->setDebugInfo(image);

    return comp;
}
