#include "pin.h"

#include <QDebug>

Pin::Pin()
    : _direction(Pin::Right), _pinType(Pin::Normal), _electricalType(Pin::Input)
{
    _layer = 1;
    _length = 300;
}

Pin::Pin(const QString &name, const QString &padName)
    : _name(name), _padname(padName), _direction(Pin::Right),
      _pinType(Pin::Normal), _electricalType(Pin::Input)
{
    _layer = 1;
    _length = 300;
}

Pin::Pin(const QString &name, const int padnumber)
{
    Pin(name, QString::number(padnumber));
}

Pin::Pin(const Pin &other)
{
    _name = other._name;
    _pos = other._pos;
    _padname = other._padname;
    _direction = other._direction;
    _pinType = other._pinType;
    _electricalType = other._electricalType;
    _layer = other._layer;
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

QString Pin::padname() const
{
    return _padname;
}

void Pin::setPadname(const QString &padname)
{
    _padname = padname;
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

Pin::PinType Pin::pinType() const
{
    return _pinType;
}

QString Pin::pinTypeString() const
{
    switch (_pinType)
    {
    case Pin::Normal:
        return "C";
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

void Pin::setPinType(const Pin::PinType &pinType)
{
    _pinType = pinType;
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

void Pin::setElectricalType(const ElectricalType &electricalType)
{
    _electricalType = electricalType;
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

QTextStream &operator<<(QTextStream &stream, const Pin &pin)
{
    // http://en.wikibooks.org/wiki/Kicad/file_formats#X_record_.28Pin.29

    // X PIN_NAME PAD_NAME X_POS Y_POS LINE_WIDTH DIRECTION NAME_TEXT_SIZE
    // LABEL_TEXT_SIZE LAYER ?1? ELECTRICAL_TYPE
    stream << "X " << pin._name << " "                      // pin name
           << pin._padname << " "                           // pad name
           << pin._pos.x() << " " << -pin._pos.y() << " "    // x y position
           << pin._length << " "                                  // lenght
           << pin.directionString() << " "                  // pin direction (up/down/left/right)
           << "50" << " "                                   // name text size
           << "50" << " "                                   // pad name text size
           << pin._layer << " "
           << "1" << " "
           << pin.electricalTypeString();
    if (pin._pinType != Pin::Normal)
        stream << " " << pin.pinTypeString();

    return stream;
}

bool operator<(const Pin &pin1, const Pin &pin2)
{
    return (pin1._padname.rightJustified(4, '0') <
            pin2._padname.rightJustified(4, '0'));
}

bool operator==(const Pin &pin1, const Pin &pin2)
{
    return (pin1._padname == pin2._padname && pin1._name == pin2._name);
}
