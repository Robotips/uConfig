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

#include "pinclassitem.h"

#include <QCollator>

PinClassItem::PinClassItem(Pin *pin)
    : _pin(pin)
{
    _priority = 0;
}

Pin *PinClassItem::pin() const
{
    return _pin;
}

void PinClassItem::setPin(Pin *pin)
{
    _pin = pin;
}

int PinClassItem::priority() const
{
    return _priority;
}

void PinClassItem::setPriority(int priority)
{
    _priority = priority;
}

QString PinClassItem::sortLabel() const
{
    return _sortLabel;
}

void PinClassItem::setSortLabel(const QString &sortLabel)
{
    _sortLabel = sortLabel;
}

bool PinClassItem::pinLessThan(PinClassItem *pin1, PinClassItem *pin2)
{
    if (pin1->priority() == pin2->priority())
    {
        QCollator col;
        return col.compare(pin1->_sortLabel.toLower(), pin2->_sortLabel.toLower()) < 0;
    }
    return pin1->priority() > pin2->priority();
}

bool PinClassItem::pinGreaterThan(PinClassItem *pin1, PinClassItem *pin2)
{
    if (pin1->priority() == pin2->priority())
    {
        QCollator col;
        return col.compare(pin1->_sortLabel.toLower(), pin2->_sortLabel.toLower()) > 0;
    }
    return pin1->priority() < pin2->priority();
}
