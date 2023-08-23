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

#ifndef PAD_H
#define PAD_H

#include <QPointF>
#include <QSizeF>
#include <QString>
#include <QTextStream>
#include <QtCore/qglobal.h>

class KICAD_EXPORT Pad
{
public:
    Pad();

    enum Shape
    {
        Rect,
        Circle,
        Ovate,
        Trapezoidal
    };

    enum Type
    {
        Std,
        Smd,
        Conn,
        Hole
    };

    enum Layer
    {
        BackSolder = 0x00000001,
        Inner14 = 0x00000002,
        Inner13 = 0x00000004,
        Inner12 = 0x00000008,
        Inner11 = 0x00000010,
        Inner10 = 0x00000020,
        Inner9 = 0x00000040,
        Inner8 = 0x00000080,
        Inner7 = 0x00000100,
        Inner6 = 0x00000200,
        Inner5 = 0x00000400,
        Inner4 = 0x00000800,
        Inner3 = 0x00001000,
        Inner2 = 0x00002000,
        Inner1 = 0x00004000,
        FrontComponent = 0x00008000,
        GlueBack = 0x00010000,
        GlueFront = 0x00020000,
        SolderPasteBack = 0x00040000,
        SolderPasteFront = 0x00080000,
        SilkScreenBack = 0x00100000,
        SilkScreenFront = 0x00200000,
        SolderMaskBack = 0x00400000,
        SolderMaskFront = 0x00800000,
        Drawings = 0x01000000,
        Comments = 0x02000000,
        ECO1 = 0x04000000,
        ECO2 = 0x08000000,
        EdgeCuts = 0x10000000
    };

    QString name() const;
    void setName(const QString &name);

    // pad
    const QPointF &pos() const;
    void setPos(const QPointF &pos);

    const QSizeF &size() const;
    void setSize(const QSizeF &size);

    Shape shape() const;
    QString shapeString() const;
    void setShape(const Shape &shape);

    const QSizeF &sizeIncrease() const;
    void setSizeIncrease(const QSizeF &sizeIncrease);

    double angle() const;
    void setAngle(double angle);

    // drill
    double drillDiameter() const;
    void setDrillDiameter(double drillDiameter);

    const QPointF &drillOffset() const;
    void setDrillOffset(const QPointF &drillOffset);

    // attributes
    Type type() const;
    QString typeString() const;
    void setType(const Type &type);

    friend QTextStream &operator<<(QTextStream &stream, const Pad &pad);

private:
    QString _name;

    // pad
    QPointF _pos;
    QSizeF _size;
    Shape _shape;
    QSizeF _sizeIncrease;
    double _angle;

    // drill
    double _drillDiameter;
    QPointF _drillOffset;

    // attributes
    Type _type;
    int _layers;
};

#endif  // PAD_H
