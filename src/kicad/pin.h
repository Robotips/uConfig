#ifndef PIN_H
#define PIN_H

#include "kicad_global.h"

#include <QString>
#include <QPoint>
#include <QTextStream>

class KICAD_EXPORT Pin
{
public:
    explicit Pin();
    Pin(const QString &name, const QString &padName = QString());
    Pin(const QString &name, const int padnumber);
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
        NotVisible,
        Invert,
        Clock,
        InvertedClock,
        LowIn,
        ClockLow,
        LowOut,
        FallingEdge,
        NonLogic,
        Normal
    };

    QString name() const;
    void setName(const QString &name);

    QPoint pos() const;
    void setPos(const QPoint &pos);
    void setPos(int x, int y);

    QString padName() const;
    void setPadName(const QString &padname);

    Direction direction() const;
    QString directionString() const;
    void setDirection(const Direction &direction);
    void setDirection(char c);

    PinType pinType() const;
    QString pinTypeString() const;
    void setPinType(const PinType &pinType);
    void setPinType(const QString &pinType);

    ElectricalType electricalType() const;
    QString electricalTypeString() const;
    void setElectricalType(const ElectricalType &electricalType);
    void setElectricalType(char c);

    int layer() const;
    void setLayer(int layer);

    int length() const;
    void setLength(int length);

    bool isValid() const;

    friend bool operator<(const Pin &pin1, const Pin &pin);
    friend bool operator==(const Pin &pin1, const Pin &pin2);

    friend QTextStream &operator<<(QTextStream &stream, const Pin &pin);
    friend QTextStream &operator>>(QTextStream &stream, Pin &pin);

private:
    QString _name;
    QPoint _pos;
    QString _padName;
    Direction _direction;
    PinType _pinType;
    ElectricalType _electricalType;
    int _layer;
    int _length;
    bool _valid;
};

#endif  // PIN_H
