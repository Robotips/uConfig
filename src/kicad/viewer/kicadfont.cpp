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

#include "kicadfont.h"

#include "componentitem.h"

#include <QDebug>
#include <QFontMetricsF>
#include <qmath.h>

const int KicadFont::charWidthTable[] = {35, 20, 28, 38, 35, 42, 45, 20, 26, 26, 29, 45, 20, 44, 20, 39, 35, 36, 35, 36, 35, 36, 35, 35,
                                         36, 36, 19, 20, 45, 45, 45, 32, 46, 32, 38, 36, 38, 34, 32, 37, 39, 19, 29, 38, 30, 42, 39, 39,
                                         36, 39, 37, 36, 29, 39, 32, 42, 35, 33, 35, 26, 26, 26, 23, 29, 16, 34, 34, 33, 34, 32, 22, 34,
                                         34, 20, 20, 30, 22, 48, 34, 34, 34, 33, 25, 30, 23, 34, 29, 39, 31, 29, 31, 26, 35, 26};

KicadFont::KicadFont(double size)
{
    setSize(size);
}

double KicadFont::charWidth(QChar c) const
{
    if (c < ' ' || c > '}')
    {
        return 0;
    }
    return (double)charWidthTable[(int)c.cell() - 32] / ComponentItem::ratio * 1.27 * _size / 9.4;
}

double KicadFont::textWidth(const QString &text) const
{
    double width = 0;
    for (QChar c : text)
    {
        width += charWidth(c);
    }
    return width;
}

QFont KicadFont::font()
{
    return _font;
}

QFont KicadFont::font(double size)
{
    QFont font("monospace");
    font.setStyleHint(QFont::SansSerif);
    font.setPointSizeF(12);
    QFontMetricsF fm(font);
    double factor = 65.0 / ComponentItem::ratio / fm.horizontalAdvance('m') * (size / 12.7);
    font.setPointSizeF(font.pointSizeF() * factor);
    return font;
}

void KicadFont::drawText(QPainter *painter, const QRectF &rect, int flags, const QString &text) const
{
    QRectF textRect = rect;
    double mtextWidth = textWidth(text);
    if ((flags & Qt::AlignLeft) != 0)
    {
        textRect.setWidth(qMin(mtextWidth, rect.width()));
    }
    else if ((flags & Qt::AlignRight) != 0)
    {
        textRect.adjust(rect.width() - qMin(mtextWidth, rect.width()), 0, 0, 0);
    }
    else if ((flags & Qt::AlignCenter) != 0)
    {
        double offset = qMax(rect.width() - mtextWidth, 0.0);
        textRect.adjust(offset / 2, 0, -offset / 2, 0);
    }
    flags = (flags & 0xFFFFFF00) | Qt::AlignCenter;

    // painter->drawRect(textRect);

    QRectF charRect = textRect;
    double x = textRect.x();
    for (auto c : text)
    {
        double mcharWidth = charWidth(c);
        charRect.setX(x - mcharWidth / 2);
        charRect.setWidth(mcharWidth * 2);
        painter->drawText(charRect, flags, c);
        x += mcharWidth;
    }
}

double KicadFont::size() const
{
    return _size;
}

void KicadFont::setSize(double size)
{
    _size = size;
    _font = KicadFont::font(_size);
}
