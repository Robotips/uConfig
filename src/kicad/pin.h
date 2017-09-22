#ifndef PIN_H
#define PIN_H

#include "kicad_global.h"

#include <QString>
#include <QPoint>
#include <QTextStream>

class KICADSHARED_EXPORT Pin
{
public:
    explicit Pin();
    Pin(const QString &name, const QString &padName = QString());
    Pin(const QString &name, const int padnumber);

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

    QString padname() const;
    void setPadname(const QString &padname);

    Direction direction() const;
    QString directionString() const;
    void setDirection(const Direction &direction);

    PinType pinType() const;
    QString pinTypeString() const;
    void setPinType(const PinType &pinType);

    ElectricalType electricalType() const;
    QString electricalTypeString() const;
    void setElectricalType(const ElectricalType &electricalType);

    int layer() const;
    void setLayer(int layer);

    friend bool operator<(const Pin &pin1, const Pin &pin);
    friend bool operator==(const Pin &pin1, const Pin &pin2);

    friend QTextStream &operator<<(QTextStream &stream, const Pin &pin);

private:
    QString _name;
    QPoint _pos;
    QString _padname;
    Direction _direction;
    PinType _pinType;
    ElectricalType _electricalType;
    int _layer;
};

#endif  // PIN_H
