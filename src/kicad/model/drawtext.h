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

#ifndef DRAWTEXT_H
#define DRAWTEXT_H

#include "../kicad_global.h"

#include "draw.h"

#include <QString>

class KICAD_EXPORT DrawText : public Draw
{
public:
    DrawText(const QString &text = QString(), const QPoint &pos = QPoint());
    Draw *clone() const override;

    TypeDraw type() const override;

    QString text() const;
    void setText(const QString &text);

    enum Direction
    {
        DirectionHorizontal,
        DirectionVertital
    };
    Direction direction() const;
    void setDirection(const Direction &direction);

    uint textSize() const;
    void setTextSize(const uint &textSize);

    enum TextStyle
    {
        TextNormal,
        TextBold = 0x01,
        TextItalic = 0x02
    };
    Q_DECLARE_FLAGS(TextStyles, TextStyle)
    DrawText::TextStyles textStyle() const;
    void setTextStyle(const TextStyles &textStyle);

    bool isVisible() const;
    void setVisible(bool visible);

    enum TextHJustify
    {
        TextHCenter,
        TextHLeft,
        TextHRight
    };
    TextHJustify textHJustify() const;
    void setTextHJustify(const TextHJustify &textHJustify);

    enum TextVJustify
    {
        TextVCenter,
        TextVBottom,
        TextVTop
    };
    TextVJustify textVJustify() const;
    void setTextVJustify(const TextVJustify &textVJustify);

protected:
    QString _text;
    Direction _direction;
    uint _textSize;
    TextStyles _textStyle;
    bool _visible;
    TextHJustify _textHJustify;
    TextVJustify _textVJustify;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(DrawText::TextStyles)

#endif  // DRAWTEXT_H
