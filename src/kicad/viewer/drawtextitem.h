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

#ifndef DRAWTEXTITEM_H
#define DRAWTEXTITEM_H

#include <QtCore/qglobal.h>

#include "drawitem.h"
#include "kicadfont.h"

#include "model/drawtext.h"

class KICAD_EXPORT DrawTextItem : public DrawItem
{
public:
    DrawTextItem(DrawText *draw, bool internal = false);
    ~DrawTextItem() override;

    enum
    {
        Type = UserType + 3
    };
    int type() const override
    {
        return Type;
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;

    void setDraw(DrawText *draw);

protected:
    DrawText *_drawText;
    QRectF _rect;
    QRectF _textRect;
    KicadFont *_fontText;
    bool _internal;
};

#endif  // DRAWTEXTITEM_H
