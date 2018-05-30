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

#include "model/drawrect.h"

#include <QPainter>
#include <QDebug>

#include <model/drawtext.h>

const int DrawItem::ratio=5;

DrawItem::DrawItem(Draw *draw)
{
    setDraw(draw);
    setZValue(-1);
}

void DrawItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rect;
    painter->setPen(QPen(QColor(132, 0, 0), 2));
    painter->setBrush(QColor(255, 255, 194));
    if (_draw->type() == Draw::TypeDrawRect)
    {
        DrawRect *drawRect = static_cast<DrawRect*>(_draw);
        rect.setTopLeft(QPoint(0, 0));
        rect.setSize(drawRect->rect().size() / ratio);
        painter->drawRect(rect);
    }
    else if (_draw->type() == Draw::TypeDrawText)
    {
        DrawText *drawText = static_cast<DrawText*>(_draw);
        painter->drawText(QPoint(0, 0), drawText->text());
    }
}

QRectF DrawItem::boundingRect() const
{
    QRectF rect;
    if (_draw->type() == Draw::TypeDrawRect)
    {
        DrawRect *drawRect = static_cast<DrawRect*>(_draw);
        rect.setTopLeft(QPoint(0, 0));
        rect.setSize(drawRect->rect().size() / ratio);
    }
    else if (_draw->type() == Draw::TypeDrawText)
    {
        DrawText *drawText = static_cast<DrawText*>(_draw);
        rect.setTopLeft(QPoint(0, 0));
        rect.setSize(QSize(drawText->text().size() * 50 / ratio, 50));
    }
    return rect.adjusted(-10, -10, 10, 10);
}

Draw *DrawItem::draw() const
{
    return _draw;
}

void DrawItem::setDraw(Draw *draw)
{
    _draw = draw;

    setPos(draw->pos() / ratio);
    update();
}
