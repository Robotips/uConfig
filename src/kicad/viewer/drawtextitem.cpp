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

#include "drawtextitem.h"

#include "componentitem.h"

#include <QDebug>
#include <QPainter>

DrawTextItem::DrawTextItem(DrawText *draw, bool internal)
    : DrawItem(draw),
      _internal(internal)
{
    _fontText = nullptr;
    setDraw(draw);
    setZValue(10);
}

DrawTextItem::~DrawTextItem()
{
    delete _fontText;
}

void DrawTextItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->setRenderHint(QPainter::Antialiasing);
    painter->setRenderHint(QPainter::TextAntialiasing);
    if (!_internal)
    {
        painter->setPen(QPen(QColor(132, 0, 0), 2));
    }
    else
    {
        painter->setPen(QPen(QColor(0, 132, 132), 2));
    }

    QFont font = _fontText->font();
    if (_drawText->textStyle().testFlag(DrawText::TextBold))
    {
        font.setBold(true);
    }
    if (_drawText->textStyle().testFlag(DrawText::TextItalic))
    {
        font.setItalic(true);
    }
    painter->setFont(font);

    if (_drawText->direction() == DrawText::DirectionVertital)
    {
        painter->rotate(-90);
    }

    _fontText->drawText(painter, _textRect, Qt::AlignLeft, _drawText->text());
}

QRectF DrawTextItem::boundingRect() const
{
    return _rect.adjusted(-20, -20, 20, 20);
}

void DrawTextItem::setDraw(DrawText *draw)
{
    _drawText = draw;

    delete _fontText;
    _fontText = new KicadFont(_drawText->textSize() / ComponentItem::ratio);

    QFont font = _fontText->font();
    if (_drawText->textStyle().testFlag(DrawText::TextBold))
    {
        font.setBold(true);
    }
    if (_drawText->textStyle().testFlag(DrawText::TextItalic))
    {
        font.setItalic(true);
    }
    QFontMetrics fm(font);

    _rect = fm.boundingRect(_drawText->text());
    _rect.setWidth(_fontText->textWidth(_drawText->text()));
    _rect.moveTopLeft(QPoint(0, 0));

    switch (_drawText->textVJustify())
    {
        case DrawText::TextVTop:
            _rect.translate(0, -_rect.height() / 4);
            break;
        case DrawText::TextVCenter:
            _rect.translate(0, -_rect.height() / 2);
            break;
        case DrawText::TextVBottom:
            _rect.translate(0, -_rect.height() / 4 * 3);
            break;
    }

    switch (_drawText->textHJustify())
    {
        case DrawText::TextHLeft:
            _rect.translate(0, 0);
            break;
        case DrawText::TextHCenter:
            _rect.translate(-_rect.width() / 2, 0);
            break;
        case DrawText::TextHRight:
            _rect.translate(-_rect.width(), 0);
            break;
    }

    _textRect = _rect;
    if (_drawText->direction() == DrawText::DirectionVertital)
    {
        int swap = _rect.size().width();
        _rect.setWidth(_rect.height());
        _rect.setHeight(swap);
    }

    setPos(draw->pos() / ComponentItem::ratio);
    update();
}
