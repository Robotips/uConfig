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

#include "drawitem.h"

#include "componentitem.h"

#include <QDebug>
#include <QPainter>

#include "drawcircleitem.h"
#include "drawpolyitem.h"
#include "drawrectitem.h"
#include "drawtextitem.h"

DrawItem::DrawItem(Draw *draw)
{
    _draw = draw;
    setZValue(-1);
}

Draw *DrawItem::draw() const
{
    return _draw;
}

QRectF DrawItem::rect() const
{
    return _rect;
}

DrawItem *DrawItem::fromDraw(Draw *draw)
{
    DrawItem *item = nullptr;
    switch (draw->type())
    {
        case Draw::TypeDrawArc:
            break;
        case Draw::TypeDrawCircle:
            item = new DrawCircleItem(dynamic_cast<DrawCircle *>(draw));
            break;
        case Draw::TypeDrawPoly:
            item = new DrawPolyItem(dynamic_cast<DrawPoly *>(draw));
            break;
        case Draw::TypeDrawRect:
            item = new DrawRectItem(dynamic_cast<DrawRect *>(draw));
            break;
        case Draw::TypeDrawText:
            item = new DrawTextItem(dynamic_cast<DrawText *>(draw));
            break;
    }
    return item;
}
