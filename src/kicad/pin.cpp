#include "pin.h"

#include <QDebug>

Pin::Pin()
{
    Pin(QString());
    _layer = 1;
}

Pin::Pin(const QString &name, const QString &padName)
    : _name(name), _padname(padName), _direction(Pin::Right),
      _pinType(Pin::Normal), _electricalType(Pin::Input)
{
    _layer = 1;
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
    qDebug()<<"copy pin";
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
    default:
        return "R";
    }
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
    case NotVisible:
        return "N";
    case Invert:
        return "I";
    case Clock:
        return "C";
    case InvertedClock:
        return "IC";
    case LowIn:
        return "L";
    case ClockLow:
        return "CL";
    case LowOut:
        return "V";
    case FallingEdge:
        return "F";
    case NonLogic:
        return "NX";
    default:
        return "";
        break;
    }
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
    case Input:
        return "I";
    case Output:
        return "O";
    case Bidir:
        return "B";
    case Tristate:
        return "T";
    case Passive:
        return "P";
    case Unspecified:
        return "U";
    case PowerIn:
        return "W";
    case PowerOut:
        return "w";
    case OpenCollector:
        return "C";
    case OpenEmitter:
        return "E";
    case NotConnected:
        return "N";
    default:
        return "I";
        break;
    }
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

QTextStream &operator<<(QTextStream &stream, const Pin &pin)
{
    // http://en.wikibooks.org/wiki/Kicad/file_formats#X_record_.28Pin.29

    // X PIN_NAME PAD_NAME X_POS Y_POS LINE_WIDTH DIRECTION NAME_TEXT_SIZE
    // LABEL_TEXT_SIZE LAYER ?1? ELECTRICAL_TYPE
    stream << "X " << pin._name << " " << pin._padname << " " << pin._pos.x()
           << " " << pin._pos.y() << " "
           << "300"
           << " " << pin.directionString() << " "
           << "50"
           << " "
           << "50"
           << " " << pin._layer << " "
           << "1"
           << " " << pin.electricalTypeString() << " " << pin.pinTypeString();

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
