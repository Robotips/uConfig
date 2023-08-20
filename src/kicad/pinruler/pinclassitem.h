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

#ifndef PINCLASSITEM_H
#define PINCLASSITEM_H

#include <QtCore/qglobal.h>

#include "model/pin.h"

class KICAD_EXPORT PinClassItem
{
public:
    PinClassItem(Pin *pin = nullptr);

    Pin *pin() const;
    void setPin(Pin *pin);

    QString sortLabel() const;
    void setSortLabel(const QString &sortLabel);

    int priority() const;
    void setPriority(int priority);

public:
    static bool pinLessThan(PinClassItem *pin1, PinClassItem *pin2);
    static bool pinGreaterThan(PinClassItem *pin1, PinClassItem *pin2);

protected:
    Pin *_pin;
    int _priority;
    QString _sortLabel;
};

#endif  // PINCLASSITEM_H
