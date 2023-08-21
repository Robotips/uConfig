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

#include "draw.h"

Draw::Draw()
{
    _filled = DrawNotFilled;
    _unit = 0;
    _convert = 0;
    _thickness = 10;
    _component = nullptr;
}

Draw::~Draw()
{
}

const QPoint &Draw::pos() const
{
    return _pos;
}

QPoint &Draw::pos()
{
    return _pos;
}

void Draw::setPos(const QPoint &pos)
{
    _pos = pos;
}

Draw::DrawFill Draw::filled() const
{
    return _filled;
}

void Draw::setFilled(DrawFill filled)
{
    _filled = filled;
}

int Draw::unit() const
{
    return _unit;
}

void Draw::setUnit(int unit)
{
    _unit = unit;
}

int Draw::convert() const
{
    return _convert;
}

void Draw::setConvert(int convert)
{
    _convert = convert;
}

int Draw::thickness() const
{
    return _thickness;
}

void Draw::setThickness(int thickness)
{
    _thickness = thickness;
}

Component *Draw::component() const
{
    return _component;
}

void Draw::setComponent(Component *component)
{
    _component = component;
}
