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

#include "drawtext.h"

DrawText::DrawText(const QString &text, const QPoint &pos)
{
    _direction = DirectionVertital;
    _textSize = 50;
    _textStyle = TextNormal;
    _textHJustify = TextHLeft;
    _textVJustify = TextVCenter;
    setPos(pos);
    setText(text);
}

Draw *DrawText::clone() const
{
    return new DrawText(*this);
}

Draw::TypeDraw DrawText::type() const
{
    return TypeDrawText;
}

QString DrawText::text() const
{
    return _text;
}

void DrawText::setText(const QString &text)
{
    _text = text;
}

DrawText::Direction DrawText::direction() const
{
    return _direction;
}

void DrawText::setDirection(const Direction &direction)
{
    _direction = direction;
}

uint DrawText::textSize() const
{
    return _textSize;
}

void DrawText::setTextSize(const uint &textSize)
{
    _textSize = textSize;
}

DrawText::TextStyles DrawText::textStyle() const
{
    return _textStyle;
}

void DrawText::setTextStyle(const TextStyles &textStyle)
{
    _textStyle = textStyle;
}

DrawText::TextHJustify DrawText::textHJustify() const
{
    return _textHJustify;
}

void DrawText::setTextHJustify(const TextHJustify &textHJustify)
{
    _textHJustify = textHJustify;
}

DrawText::TextVJustify DrawText::textVJustify() const
{
    return _textVJustify;
}

void DrawText::setTextVJustify(const TextVJustify &textVJustify)
{
    _textVJustify = textVJustify;
}
