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

#include "pin.h"

#include <QDebug>

Pin::Pin()
    : _direction(Pin::Right), _pinType(Pin::Normal), _electricalType(Pin::Input)
{
    _layer = 1;
    _length = 300;
    _component = Q_NULLPTR;
}

Pin::Pin(const QString &name, const QString &padName)
    : _name(name), _padName(padName), _direction(Pin::Right),
      _pinType(Pin::Normal), _electricalType(Pin::Input)
{
    _layer = 1;
    _length = 300;
    _component = Q_NULLPTR;
}

Pin::Pin(const QString &name, const int padnumber)
{
    Pin(name, QString::number(padnumber));
}

Pin::Pin(const Pin &other)
{
    _name = other._name;
    _pos = other._pos;
    _padName = other._padName;
    _direction = other._direction;
    _pinType = other._pinType;
    _electricalType = other._electricalType;
    _layer = other._layer;
    _length = other._length;
}

QString Pin::name() const
{
    return _name;
}

void Pin::setName(const QString &name)
{
    _name = name;
}

QPoint Pin::pos() const
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

QString Pin::padName() const
{
    return _padName;
}

void Pin::setPadName(const QString &padname)
{
    _padName = padname;
}

Pin::Direction Pin::direction() const
{
    return _direction;
}

QString Pin::directionString() const
{
    switch (_direction)
    {
    case Pin::Down:
        return "D";
    case Pin::Left:
        return "L";
    case Pin::Up:
        return "U";
    case Pin::Right:
        return "R";
    }
    return "R";
}

void Pin::setDirection(const Pin::Direction &direction)
{
    _direction = direction;
}

void Pin::setDirection(char c)
{
    switch (c)
    {
    case 'D':
        _direction = Pin::Down;
        break;
    case 'L':
        _direction = Pin::Left;
        break;
    case 'U':
        _direction = Pin::Up;
        break;
    case 'R':
        _direction = Pin::Right;
        break;
    }
}

Pin::PinType Pin::pinType() const
{
    return _pinType;
}

QString Pin::pinTypeString() const
{
    switch (_pinType)
    {
    case Pin::Normal:
        return "";
    case Pin::NotVisible:
        return "N";
    case Pin::Invert:
        return "I";
    case Pin::Clock:
        return "C";
    case Pin::InvertedClock:
        return "IC";
    case Pin::LowIn:
        return "L";
    case Pin::ClockLow:
        return "CL";
    case Pin::LowOut:
        return "V";
    case Pin::FallingEdge:
        return "F";
    case Pin::NonLogic:
        return "NX";
    }
    return "";
}

QString Pin::pinTypeDesc(const Pin::PinType &pinType)
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
    return QStringLiteral("");
}

void Pin::setPinType(const Pin::PinType &pinType)
{
    _pinType = pinType;
}

void Pin::setPinType(const QString &pinType)
{
    if (pinType == "N")
        _pinType = Pin::NotVisible;
    else if (pinType == "I")
        _pinType = Pin::Invert;
    else if (pinType == "C")
        _pinType = Pin::Clock;
    else if (pinType == "IC")
        _pinType = Pin::InvertedClock;
    else if (pinType == "L")
        _pinType = Pin::LowIn;
    else if (pinType == "CL")
        _pinType = Pin::ClockLow;
    else if (pinType == "V")
        _pinType = Pin::LowOut;
    else if (pinType == "F")
        _pinType = Pin::FallingEdge;
    else if (pinType == "NX")
        _pinType = Pin::NonLogic;
    else
        _pinType = Pin::Normal;
}

Pin::ElectricalType Pin::electricalType() const
{
    return _electricalType;
}

QString Pin::electricalTypeString() const
{
    switch (_electricalType)
    {
    case Pin::Input:
        return "I";
    case Pin::Output:
        return "O";
    case Pin::Bidir:
        return "B";
    case Pin::Tristate:
        return "T";
    case Pin::Passive:
        return "P";
    case Pin::Unspecified:
        return "U";
    case Pin::PowerIn:
        return "W";
    case Pin::PowerOut:
        return "w";
    case Pin::OpenCollector:
        return "C";
    case Pin::OpenEmitter:
        return "E";
    case Pin::NotConnected:
        return "N";
    }
    return "I";
}

QString Pin::electricalTypeDesc(const Pin::ElectricalType &electricalType)
{
    switch (electricalType)
    {
    case Pin::Input:
        return QStringLiteral("input");
    case Pin::Output:
        return QStringLiteral("output");
    case Pin::Bidir:
        return QStringLiteral("bi-directional");
    case Pin::Tristate:
        return QStringLiteral("tristate");
    case Pin::Passive:
        return QStringLiteral("passive");
    case Pin::Unspecified:
        return QStringLiteral("unspecified");
    case Pin::PowerIn:
        return QStringLiteral("power in");
    case Pin::PowerOut:
        return QStringLiteral("power out");
    case Pin::OpenCollector:
        return QStringLiteral("open collector");
    case Pin::OpenEmitter:
        return QStringLiteral("open emitter");
    case Pin::NotConnected:
        return QStringLiteral("not connected");
    }
    return QStringLiteral("input");
}

void Pin::setElectricalType(const ElectricalType &electricalType)
{
    _electricalType = electricalType;
}

void Pin::setElectricalType(char c)
{
    switch (c)
    {
    case 'I':
        _electricalType = Pin::Input;
        break;
    case 'O':
        _electricalType = Pin::Output;
        break;
    case 'B':
        _electricalType = Pin::Bidir;
        break;
    case 'T':
        _electricalType = Pin::Tristate;
        break;
    case 'P':
        _electricalType = Pin::Passive;
        break;
    case 'U':
        _electricalType = Pin::Unspecified;
        break;
    case 'W':
        _electricalType = Pin::PowerIn;
        break;
    case 'w':
        _electricalType = Pin::PowerOut;
        break;
    case 'C':
        _electricalType = Pin::OpenCollector;
        break;
    case 'E':
        _electricalType = Pin::OpenEmitter;
        break;
    case 'N':
        _electricalType = Pin::NotConnected;
        break;
    }
}

int Pin::layer() const
{
    return _layer;
}

void Pin::setLayer(int layer)
{
    _layer = layer;
}

int Pin::length() const
{
    return _length;
}

void Pin::setLength(int length)
{
    _length = length;
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
    return (pin1._padName.rightJustified(4, '0') <
            pin2._padName.rightJustified(4, '0'));
}

bool operator==(const Pin &pin1, const Pin &pin2)
{
    return (pin1._padName == pin2._padName && pin1._name == pin2._name);
}
