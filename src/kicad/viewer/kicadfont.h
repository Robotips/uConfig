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

#ifndef KICADFONT_H
#define KICADFONT_H

#include <QFont>
#include <QPainter>
#include <QString>
#include <QtCore/qglobal.h>

class KICAD_EXPORT KicadFont
{
public:
    KicadFont(double size = 50);

    double size() const;
    void setSize(double size);

    double charWidth(QChar c) const;
    double textWidth(const QString &text) const;

    QFont font();
    static QFont font(double size);
    void drawText(QPainter *painter, const QRectF &rect, int flags, const QString &text) const;

protected:
    static const int charWidthTable[];
    double _size;
    QFont _font;
};

#endif  // KICADFONT_H
