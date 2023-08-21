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

#include "drawarc.h"

DrawArc::DrawArc()
{
}

Draw *DrawArc::clone() const
{
    return new DrawArc(*this);
}

Draw::TypeDraw DrawArc::type() const
{
    return TypeDrawArc;
}

uint DrawArc::radius() const
{
    return _radius;
}

void DrawArc::setRadius(uint radius)
{
    _radius = radius;
}

int DrawArc::startAngle() const
{
    return _startAngle;
}

void DrawArc::setStartAngle(int startAngle)
{
    _startAngle = startAngle;
}

QPoint DrawArc::startPos() const
{
    return _startPos;
}

void DrawArc::setStartPos(const QPoint &startPos)
{
    _startPos = startPos;
}

int DrawArc::endAngle() const
{
    return _endAngle;
}

void DrawArc::setEndAngle(int endAngle)
{
    _endAngle = endAngle;
}

QPoint DrawArc::endPos() const
{
    return _endPos;
}

void DrawArc::setEndPos(const QPoint &endPos)
{
    _endPos = endPos;
}
