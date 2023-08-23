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

#ifndef PIN_H
#define PIN_H

#include <QPoint>
#include <QString>
#include <QTextStream>
#include <QtCore/qglobal.h>

class Component;

class KICAD_EXPORT Pin
{
public:
    explicit Pin();
    Pin(const QString &name, const QString &padName = QString());
    Pin(const QString &name, int padNumber);
    Pin(const Pin &other);

    enum Direction
    {
        Up,
        Left,
        Down,
        Right
    };

    enum ElectricalType
    {
        Input,
        Output,
        Bidir,
        Tristate,
        Passive,
        Unspecified,
        PowerIn,
        PowerOut,
        OpenCollector,
        OpenEmitter,
        NotConnected
    };

    enum PinType
    {
        Normal,
        NotVisible,
        Invert,
        Clock,
        InvertedClock,
        LowIn,
        ClockLow,
        LowOut,
        FallingEdge,
        NonLogic
    };

    const QString &name() const;
    void setName(const QString &name);

    const QPoint &pos() const;
    void setPos(const QPoint &pos);
    void setPos(int x, int y);

    int angle() const;
    void setAngle(int angle);
    Direction direction() const;
    void setDirection(Pin::Direction direction);

    const QString &padName() const;
    void setPadName(const QString &padname);

    PinType pinType() const;
    static QString pinTypeDesc(Pin::PinType pinType);
    void setPinType(PinType pinType);

    ElectricalType electricalType() const;
    static QString electricalTypeDesc(Pin::ElectricalType electricalType);
    void setElectricalType(ElectricalType electricalType);

    int textNameSize() const;
    void setTextNameSize(int textNameSize);

    int textPadSize() const;
    void setTextPadSize(int textPadSize);

    int unit() const;
    void setUnit(int unit);

    int length() const;
    void setLength(int length);

    const QString &className() const;
    void setClassName(const QString &className);

    Component *component() const;
    void setComponent(Component *component);

    friend bool operator<(const Pin &pin1, const Pin &pin2);
    friend bool operator==(const Pin &pin1, const Pin &pin2);

private:
    QString _name;
    QPoint _pos;
    int _angle;
    QString _padName;
    PinType _pinType;
    ElectricalType _electricalType;
    int _textNameSize;
    int _textPadSize;
    int _unit;
    int _length;

    QString _className;

    Component *_component;
};

#endif  // PIN_H
