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

#include "drawcircleitem.h"

#include "componentitem.h"

#include <QDebug>
#include <QPainter>

DrawCircleItem::DrawCircleItem(DrawCircle *draw)
    : DrawItem(draw)
{
    setDraw(draw);
    setZValue(-1);
}

void DrawCircleItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    double thickness = draw()->thickness();
    if (thickness == 0)
    {
        thickness = 1 / 0.254;
    }
    painter->setPen(QPen(QColor(132, 0, 0), 0.254 * thickness));

    if (_drawCircle->filled() != Draw::DrawNotFilled)
    {
        painter->setBrush(QColor(255, 255, 194));
    }

    painter->drawArc(_rect, 0, 5760);
}

QRectF DrawCircleItem::boundingRect() const
{
    return _rect.adjusted(-2, -2, 2, 2);
}

void DrawCircleItem::setDraw(DrawCircle *draw)
{
    _drawCircle = draw;
    double side = _drawCircle->radius() / ComponentItem::ratio;
    _rect.setTopLeft(QPoint(-side, -side));
    _rect.setSize(QSize(side * 2, side * 2));
    _rect = _rect.normalized();

    setPos(draw->pos() / ComponentItem::ratio);
    update();
}
