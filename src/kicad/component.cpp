#include "component.h"

#include <QDebug>

Component::Component(const QString &name)
    : _name(name),
      _prefixe("U")
{
}

QString Component::name() const
{
    return _name;
}

void Component::setName(const QString &name)
{
    _name = name;
}

QList<Pin> Component::pins() const
{
    return _pins;
}

void Component::addPin(const Pin &pin)
{
    _pins.append(pin);
}

QString Component::prefixe() const
{
    return _prefixe;
}

void Component::setPrefixe(const QString &prefixe)
{
    _prefixe = prefixe;
}

const QStringList &Component::alias() const
{
    return _alias;
}

void Component::addAlias(const QString &alias)
{
    _alias.append(alias);
}

const QStringList &Component::footPrints() const
{
    return _footPrints;
}

void Component::addFootPrints(const QString &footprint)
{
    _footPrints.append(footprint);
}

void Component::reorganizeToPackageStyle()
{
    qSort(_pins);

    short rightCount = _pins.count()/2;
    short leftCount = _pins.count() - rightCount;

    short leftOffset = leftCount * 100 / 2 - 50;
    short rightOffset = rightCount * 100 / 2 - 50;

    QPoint pos(-1600, -leftOffset);
    int i;
    for(i=0; i<leftCount;i++)
    {
        Pin &pin = _pins[i];

        pin.setDirection(Pin::Right);
        pin.setPos(pos);
        pos += QPoint(0, 100);
    }

    pos = QPoint(1600, rightOffset);
    for(; i<leftCount+rightCount;i++)
    {
        Pin &pin = _pins[i];

        pin.setDirection(Pin::Left);
        pin.setPos(pos);
        pos += QPoint(0, -100);
    }
}

void Component::reorganizeUnderRules(const QList<QRegExp> &rules)
{

}

QTextStream& operator<<(QTextStream &stream, const Component &component)
{
    // http://en.wikibooks.org/wiki/Kicad/file_formats#Description_of_a_component_2

    // comments
    stream << "#" << endl << "# " << component._name << endl << "#" << endl;

    // def
    stream << "DEF " << component._name << " " << component._prefixe << " 0 40 Y Y 1 F N" << endl;
    stream << "F0 \"" << component._prefixe << "\" 750 -1100 50 H V C CNN" << endl;
    stream << "F1 \"" << component._name << "\" 0 0 50 H V C CNN" << endl;
    stream << "F2 \"~\" 0 0 50 H I C CNN" << endl;
    stream << "F3 \"~\" 0 0 50 H I C CNN" << endl;

    // footprints
    if(!component._footPrints.isEmpty())
    {
        stream << "$FPLIST" << endl;
        foreach(QString footPrint, component._footPrints)
        {
            stream << " " << footPrint << endl;
        }
        stream << "$ENDFPLIST" << endl;
    }

    // alias
    if(!component._alias.isEmpty()) stream << "ALIAS " << component._alias.join(" ") << endl;

    stream << "DRAW" << endl;
    // pins
    foreach (Pin pin, component._pins)
    {
        stream << pin << endl;
    }

    // end
    stream << "ENDDRAW" << endl;
    stream << "ENDDEF";

    return stream;
}
