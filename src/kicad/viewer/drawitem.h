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

#ifndef DRAWITEM_H
#define DRAWITEM_H

#include <QGraphicsItem>
#include <QtCore/qglobal.h>

#include "model/draw.h"

class KICAD_EXPORT DrawItem : public QGraphicsItem
{
public:
    DrawItem(Draw *draw);

    Draw *draw() const;
    QRectF rect() const;

    static DrawItem *fromDraw(Draw *draw);

protected:
    Draw *_draw;
    QRectF _rect;
};

#endif  // DRAWITEM_H
