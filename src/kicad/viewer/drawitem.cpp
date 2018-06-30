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

#include "drawitem.h"

#include "componentitem.h"

#include <QPainter>
#include <QDebug>

#include "drawrectitem.h"
#include "drawtextitem.h"
#include "drawpolyitem.h"

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
    DrawItem *item = Q_NULLPTR;
    switch (draw->type())
    {
    case Draw::TypeDrawArc:
        break;
    case Draw::TypeDrawCircle:
        break;
    case Draw::TypeDrawPoly:
        item = new DrawPolyItem(static_cast<DrawPoly*>(draw));
        break;
    case Draw::TypeDrawRect:
        item = new DrawRectItem(static_cast<DrawRect*>(draw));
        break;
    case Draw::TypeDrawText:
        item = new DrawTextItem(static_cast<DrawText*>(draw));
        break;
    }
    return item;
}
