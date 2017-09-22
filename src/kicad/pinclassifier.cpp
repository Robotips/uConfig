#include "pinclassifier.h"

#include <QList>
#include <QDebug>

PinClassifier::PinClassifier(const Component &component) : _component(component)
{
    QList<PinCompare> pins;
    foreach (Pin pin, component.pins())
    {
        foreach (QString pinName, pin.name().split('/'))
        {
            pins.append(PinCompare(pinName));
            qDebug() << pinName << pins.last().internalName();
        }
    }
    qSort(pins);

    int lastCompare = 0;
    QList<QList<PinCompare>> categ;
    categ.append(QList<PinCompare>() << PinCompare(pins[0].name(), true));
    for (int i = 1; i < pins.count(); i++)
    {
        int compare = pins[i].distanceTo(pins[i - 1]);
        if (compare == 0 || (compare <= 2 && lastCompare >= 2))
            categ.last().append(PinCompare(pins[i].name(), true));
        else
            categ.append(QList<PinCompare>()
                         << PinCompare(pins[i].name(), true));
        lastCompare = compare;
    }

    for (int i = 0; i < categ.count(); i++)
    {
        qSort(categ[i]);
    }

    qDebug() << categ;
}

PinCompare::PinCompare(const QString &name, bool number)
{
    _name = name;
    _internalName = name.toUpper();
    if (!number)
    {
        _internalName.replace(QRegExp("([0-9][0-9][0-9])"), "n");
        _internalName.replace(QRegExp("([0-9][0-9])"), "n");
        _internalName.replace(QRegExp("([0-9])"), "n");
    }
    else
    {
        if (_internalName.contains(QRegExp("([0-9][0-9])")))
            _internalName.replace(QRegExp("([0-9][0-9])"), "0\\1");
        else
            _internalName.replace(QRegExp("([0-9])"), "00\\1");
    }
}

int PinCompare::distanceTo(const PinCompare &other)
{
    int dist = qAbs(_internalName.count() - other._internalName.count());
    for (int i = 0; i < _internalName.count(); i++)
    {
        if (_internalName.at(i) != other._internalName.at(i))
            dist++;
    }
    return dist;
}

QDebug operator<<(QDebug dbg, const PinCompare &pin)
{
    // dbg.nospace() << "[" << pin._name << pin._internalName << "]";
    dbg.nospace() << pin._name;
    return dbg;
}

QString PinCompare::internalName() const
{
    return _internalName;
}

void PinCompare::setInternalName(const QString &internalName)
{
    _internalName = internalName;
}

QString PinCompare::name() const
{
    return _name;
}

void PinCompare::setName(const QString &name)
{
    _name = name;
}

bool operator<(const PinCompare &pin1, const PinCompare &pin2)
{
    return (pin1._internalName < pin2._internalName);
}
