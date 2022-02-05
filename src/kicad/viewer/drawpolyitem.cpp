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

#include "drawpolyitem.h"

#include "componentitem.h"

#include <QDebug>
#include <QPainter>

DrawPolyItem::DrawPolyItem(DrawPoly *draw)
    : DrawItem(draw)
{
    setDraw(draw);
    setZValue(-1);
}

void DrawPolyItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    double thickness = draw()->thickness();
    if (thickness == 0)
    {
        thickness = 1 / 0.254;
    }
    painter->setPen(QPen(QColor(132, 0, 0), 0.254 * thickness));

    if (_drawPoly->filled() != DrawPoly::DrawNotFilled)
    {
        painter->setBrush(QColor(255, 255, 194));
    }

    QPolygon poly;
    for (QPoint pt : _drawPoly->points())
    {
        poly.append(pt / ComponentItem::ratio);
    }

    painter->drawPolygon(poly);
}

QRectF DrawPolyItem::boundingRect() const
{
    return _rect.adjusted(-2, -2, 2, 2);
}

void DrawPolyItem::setDraw(DrawPoly *draw)
{
    _drawPoly = draw;
    QRect mrect(0, 0, 1, 1);

    for (QPoint pt : _drawPoly->points())
    {
        mrect = mrect.united(QRect(pt / ComponentItem::ratio, QSize(1, 1)));
    }
    _rect = mrect.normalized();

    setPos(draw->pos() / ComponentItem::ratio);
    update();
}
