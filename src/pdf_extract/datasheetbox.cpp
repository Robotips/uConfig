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

#include "datasheetbox.h"

#include "poppler/qt5/poppler-qt5.h"

#include <QDebug>

int DatasheetBox::created = 0;

DatasheetBox::DatasheetBox()
    : page(0), associated(false)
{
    created++;
}

DatasheetBox::DatasheetBox(const Poppler::TextBox &textBox)
    : text(textBox.text()), pos(textBox.boundingBox()), page(0), associated(false)
{
    created++;
}

DatasheetBox::~DatasheetBox()
{
    created--;
}

qreal DatasheetBox::distanceToPoint(const QPointF &center) const
{
    qreal dist, newdist;

    dist = (center - pos.center()).manhattanLength();

    newdist = (center - pos.bottomLeft()).manhattanLength();
    if (newdist < dist)
        dist = newdist;

    newdist = (center - pos.topRight()).manhattanLength();
    if (newdist < dist)
        dist = newdist;
    newdist = (center - QPointF(pos.left(), pos.center().y())).manhattanLength();
    if (newdist < dist)
        dist = newdist;

    newdist = (center - pos.bottomRight()).manhattanLength();
    if (newdist < dist)
        dist = newdist;

    newdist = (center - pos.topLeft()).manhattanLength();
    if (newdist < dist)
        dist = newdist;

    return dist;
}

bool DatasheetBox::isAlign(const DatasheetBox &label, const DatasheetBox &number)
{
    qreal marge;
    if (label.text.count() <= 2) // small label test align
    {
        marge = label.pos.height() / 2.0;
        if (label.pos.top() - marge < number.pos.top() &&
            label.pos.bottom() + marge > number.pos.bottom())
            return true;
        marge = label.pos.width() / 2.0;
        if (label.pos.left() - marge < number.pos.left() &&
            label.pos.right() + marge > number.pos.right())
            return true;
        return false;
    }
    if (label.pos.width() > label.pos.height())  // Horizontal
    {
        if (label.pos.height() > number.pos.height() * 2
         || number.pos.height() > label.pos.height() * 2)
            return false;
        marge = label.pos.height();
        if (label.pos.top() - marge < number.pos.top() &&
            label.pos.bottom() + marge > number.pos.bottom())
            return true;
        else
            return false;
    }
    else  // Vertical
    {
        marge = label.pos.width();
        if (label.pos.left() - marge < number.pos.left() &&
            label.pos.right() + marge > number.pos.right())
            return true;
        else
            return false;
    }
}
