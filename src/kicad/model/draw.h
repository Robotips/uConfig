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

#ifndef DRAW_H
#define DRAW_H

#include <QPoint>
#include <QtCore/qglobal.h>

class Component;

class KICAD_EXPORT Draw
{
public:
    Draw();
    virtual ~Draw();
    virtual Draw *clone() const = 0;

    enum TypeDraw
    {
        TypeDrawArc,
        TypeDrawCircle,
        TypeDrawPoly,
        TypeDrawRect,
        TypeDrawText
    };
    virtual TypeDraw type() const = 0;

    QPoint &pos();
    const QPoint &pos() const;
    void setPos(const QPoint &pos);

    enum DrawFill
    {
        DrawNotFilled,
        DrawFilledForeGround,
        DrawFilledBackGround
    };
    DrawFill filled() const;
    void setFilled(DrawFill filled);

    int unit() const;
    void setUnit(int unit);

    int convert() const;
    void setConvert(int convert);

    int thickness() const;
    void setThickness(int thickness);

    Component *component() const;
    void setComponent(Component *component);

protected:
    QPoint _pos;
    DrawFill _filled;
    int _unit;
    int _convert;
    int _thickness;
    Component *_component;
};

#endif  // DRAW_H
