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

#include "pin.h"

#include <utility>

Pin::Pin()
    : _pinType(Pin::Normal),
      _electricalType(Pin::Input)
{
    _angle = 0;
    _unit = 1;
    _length = 300;
    _textNameSize = 50;
    _textPadSize = 50;
    _component = nullptr;
}

Pin::Pin(const QString &name, const QString &padName)
    : _name(name),
      _padName(padName),
      _pinType(Pin::Normal),
      _electricalType(Pin::Input)
{
    _angle = 0;
    _unit = 1;
    _length = 300;
    _textNameSize = 50;
    _textPadSize = 50;
    _component = nullptr;
}

Pin::Pin(const QString &name, const int padNumber)
    : Pin(name, QString::number(padNumber))
{
}

Pin::Pin(const Pin &other)
{
    _name = other._name;
    _pos = other._pos;
    _padName = other._padName;
    _angle = other._angle;
    _pinType = other._pinType;
    _electricalType = other._electricalType;
    _unit = other._unit;
    _length = other._length;

    _textNameSize = other._textNameSize;
    _textPadSize = other._textPadSize;
}

const QString &Pin::name() const
{
    return _name;
}

void Pin::setName(const QString &name)
{
    _name = name;
}

const QPoint &Pin::pos() const
{
    return _pos;
}

void Pin::setPos(const QPoint &pos)
{
    _pos = pos;
}

void Pin::setPos(int x, int y)
{
    _pos = QPoint(x, y);
}

Pin::Direction Pin::direction() const
{
    if (_angle > 315 || _angle <= 45)
    {
        return Right;
    }
    if (_angle > 45 && _angle <= 135)
    {
        return Up;
    }
    if (_angle > 135 && _angle <= 225)
    {
        return Left;
    }
    return Down;
}

void Pin::setDirection(Direction direction)
{
    switch (direction)
    {
        case Right:
            _angle = 0;
            break;
        case Up:
            _angle = 90;
            break;
        case Left:
            _angle = 180;
            break;
        case Down:
            _angle = 270;
            break;
    }
}

int Pin::angle() const
{
    return _angle;
}

void Pin::setAngle(int angle)
{
    _angle = angle;
}

const QString &Pin::padName() const
{
    return _padName;
}

void Pin::setPadName(const QString &padname)
{
    _padName = padname;
}

Pin::PinType Pin::pinType() const
{
    return _pinType;
}

QString Pin::pinTypeDesc(PinType pinType)
{
    switch (pinType)
    {
        case Pin::Normal:
            return QStringLiteral("normal");
        case Pin::NotVisible:
            return QStringLiteral("not visible");
        case Pin::Invert:
            return QStringLiteral("inverted");
        case Pin::Clock:
            return QStringLiteral("clock");
        case Pin::InvertedClock:
            return QStringLiteral("inverted clock");
        case Pin::LowIn:
            return QStringLiteral("low in");
        case Pin::ClockLow:
            return QStringLiteral("clock low");
        case Pin::LowOut:
            return QStringLiteral("low out");
        case Pin::FallingEdge:
            return QStringLiteral("falling edge");
        case Pin::NonLogic:
            return QStringLiteral("non logic");
    }
    return QLatin1String("");
}

void Pin::setPinType(PinType pinType)
{
    _pinType = pinType;
}

Pin::ElectricalType Pin::electricalType() const
{
    return _electricalType;
}

QString Pin::electricalTypeDesc(ElectricalType electricalType)
{
    switch (electricalType)
    {
        case Pin::Input:
            return QStringLiteral("input");
        case Pin::Output:
            return QStringLiteral("output");
        case Pin::Bidir:
            return QStringLiteral("bidirectional");
        case Pin::Tristate:
            return QStringLiteral("tri-state");
        case Pin::Passive:
            return QStringLiteral("passive");
        case Pin::Unspecified:
            return QStringLiteral("unspecified");
        case Pin::PowerIn:
            return QStringLiteral("power input");
        case Pin::PowerOut:
            return QStringLiteral("power output");
        case Pin::OpenCollector:
            return QStringLiteral("open collector");
        case Pin::OpenEmitter:
            return QStringLiteral("open emitter");
        case Pin::NotConnected:
            return QStringLiteral("not connected");
    }
    return QStringLiteral("input");
}

void Pin::setElectricalType(ElectricalType electricalType)
{
    _electricalType = electricalType;
}

int Pin::textNameSize() const
{
    return _textNameSize;
}

void Pin::setTextNameSize(int textNameSize)
{
    if (textNameSize > 0)
    {
        _textNameSize = textNameSize;
    }
}

int Pin::textPadSize() const
{
    return _textPadSize;
}

void Pin::setTextPadSize(int textPadSize)
{
    if (textPadSize > 0)
    {
        _textPadSize = textPadSize;
    }
}

int Pin::unit() const
{
    return _unit;
}

void Pin::setUnit(int unit)
{
    if (unit > 0)
    {
        _unit = unit;
    }
}

int Pin::length() const
{
    return _length;
}

void Pin::setLength(int length)
{
    if (length > 0)
    {
        _length = length;
    }
}

const QString &Pin::className() const
{
    return _className;
}

void Pin::setClassName(const QString &className)
{
    _className = className;
}

Component *Pin::component() const
{
    return _component;
}

void Pin::setComponent(Component *component)
{
    _component = component;
}

bool operator<(const Pin &pin1, const Pin &pin2)
{
    return (pin1._padName.rightJustified(4, '0') < pin2._padName.rightJustified(4, '0'));
}

bool operator==(const Pin &pin1, const Pin &pin2)
{
    return (pin1._padName == pin2._padName && pin1._name == pin2._name);
}
