#include "component.h"

#include <QDebug>

Component::Component(const QString &name)
    : _name(name),
      _prefixe("U")
{
    _valid = true;
}

QString Component::name() const
{
    return _name;
}

void Component::setName(const QString &name)
{
    _name = name;
}

QList<Pin> &Component::pins()
{
    return _pins;
}

const QList<Pin> &Component::pins() const
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

void Component::sort()
{
    qSort(_pins);
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

bool Component::isValid() const
{
    return _valid;
}

QTextStream &operator>>(QTextStream &stream, Component &component)
{
    QRegExp regexp("^F([0-9]) \"(\\S*)\" (\\-?[0-9]+) (\\-?[0-9]+) ([0-9]+) ([A-Z]) ([A-Z]) ([A-Z]) ([A-Z]+)$");
    bool draw = false;
    do
    {
        QString line = stream.readLine();
        if(line.startsWith("DEF"))
        {
        }
        else if(regexp.indexIn(line)>-1)
        {
            qDebug() << regexp.cap(1) << regexp.cap(2) << regexp.cap(3) << regexp.cap(4) << regexp.cap(5);
        }
        else if(line.startsWith("DRAW"))
        {
            draw = true;
        }
        else if(line.startsWith("ENDDRAW"))
        {
            draw = false;
        }
        else if(line.startsWith("ENDDEF"))
        {
            component._valid = true;
            qDebug()<<line;
            return stream;
        }
        else
        {
            if(draw)
            {
                // stream >>
            }
        }
    } while (!stream.atEnd());

    component._valid = false;
    return stream;
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
