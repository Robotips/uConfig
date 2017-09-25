#ifndef PAD_H
#define PAD_H

#include "kicad_global.h"

#include <QString>
#include <QPointF>
#include <QSizeF>
#include <QTextStream>

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
    QPointF pos() const;
    void setPos(const QPointF &pos);

    QSizeF size() const;
    void setSize(const QSizeF &size);

    Shape shape() const;
    QString shapeString() const;
    void setShape(const Shape &shape);

    QSizeF sizeIncrease() const;
    void setSizeIncrease(const QSizeF &sizeIncrease);

    double angle() const;
    void setAngle(double angle);

    // drill
    double drillDiameter() const;
    void setDrillDiameter(double drillDiameter);

    QPointF drillOffset() const;
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
