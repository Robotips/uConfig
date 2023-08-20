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

#ifndef DRAWRECTITEM_H
#define DRAWRECTITEM_H

#include <QtCore/qglobal.h>

#include "drawitem.h"

#include "model/drawrect.h"

class KICAD_EXPORT DrawRectItem : public DrawItem
{
public:
    DrawRectItem(DrawRect *draw);

    enum
    {
        Type = UserType + 4
    };
    int type() const override
    {
        return Type;
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;

    void setDraw(DrawRect *draw);

protected:
    DrawRect *_drawRect;
    QRectF _rect;
};

#endif  // DRAWRECTITEM_H
