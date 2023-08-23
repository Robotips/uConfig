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

#include "pad.h"

#include <QDebug>

Pad::Pad()
    : _angle(0)
{
}

QString Pad::name() const
{
    return _name;
}

void Pad::setName(const QString &name)
{
    _name = name;
}

const QPointF &Pad::pos() const
{
    return _pos;
}

void Pad::setPos(const QPointF &pos)
{
    _pos = pos;
}

const QSizeF &Pad::size() const
{
    return _size;
}

void Pad::setSize(const QSizeF &size)
{
    _size = size;
}

Pad::Shape Pad::shape() const
{
    return _shape;
}

QString Pad::shapeString() const
{
    switch (_shape)
    {
        case Pad::Rect:
            return "R";
        case Pad::Circle:
            return "C";
        case Pad::Ovate:
            return "O";
        case Pad::Trapezoidal:
            return "T";
    }
    return "R";
}

void Pad::setShape(const Shape &shape)
{
    _shape = shape;
}

const QSizeF &Pad::sizeIncrease() const
{
    return _sizeIncrease;
}

void Pad::setSizeIncrease(const QSizeF &sizeIncrease)
{
    _sizeIncrease = sizeIncrease;
}

double Pad::drillDiameter() const
{
    return _drillDiameter;
}

void Pad::setDrillDiameter(double drillDiameter)
{
    _drillDiameter = drillDiameter;
}

const QPointF &Pad::drillOffset() const
{
    return _drillOffset;
}

void Pad::setDrillOffset(const QPointF &drillOffset)
{
    _drillOffset = drillOffset;
}

Pad::Type Pad::type() const
{
    return _type;
}

QString Pad::typeString() const
{
    switch (_shape)
    {
        case Pad::Std:
            return "STD";
        case Pad::Smd:
            return "SMD";
        case Pad::Conn:
            return "CONN";
        case Pad::Hole:
            return "HOLE";
    }
    return "STD";
}

void Pad::setType(const Type &type)
{
    _type = type;
}

double Pad::angle() const
{
    return _angle;
}

void Pad::setAngle(double angle)
{
    _angle = angle;
}

QTextStream &operator<<(QTextStream &stream, const Pad &pad)
{
    // def
    stream << "$PAD" << '\n';

    // pad
    stream << "Sh \"" << pad._name << "\" " << pad.shapeString() << " " << pad._size.width() << " " << pad._size.height() << " " << pad._sizeIncrease.width()
           << " " << pad._sizeIncrease.height() << " " << pad._angle << '\n';

    // drill
    stream << "Dr " << pad._drillDiameter << " " << pad._drillOffset.x() << " " << pad._drillOffset.y() << '\n';

    // attribute
    stream << "At " << pad.typeString() << " N " << pad._layers << '\n';

    // attribute
    stream << "Ne 0 \"\"" << '\n';

    // pos
    stream << "Po " << pad._pos.x() << " " << pad._pos.y() << '\n';

    // end
    stream << "$EndPAD";

    return stream;
}
