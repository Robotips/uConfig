#include "pinclass.h"

#include <QDebug>

PinClass::PinClass(const QString &className)
    : _className(className)
{
    // properties default values
    _position = ClassRule::PositionLeft;
    _positionSet = false;
    _sort = ClassRule::SortNone;
    _sortSet = false;
    _sortPattern = ".*";
    _sortPatternSet = false;
}

QString PinClass::className() const
{
    return _className;
}

void PinClass::applyRule(ClassRule *rule)
{
    if (!_positionSet && rule->isPositionSet())
        setPosition(rule->positionValue());
    if (!_sortSet && rule->isSortSet())
        setSort(rule->sortValue());
    if (!_sortPatternSet && rule->isSortPatternSet())
        setSortPattern(rule->sortPattern());
}

void PinClass::applyRules(QList<ClassRule *> rules)
{
    foreach (ClassRule *rule, rules)
        applyRule(rule);
}

bool PinClass::pinPatterLessThan(QPair<QString, Pin*> pin1, QPair<QString, Pin*> pin2)
{
    return pin1.first.toLower() < pin2.first.toLower();
}

void PinClass::sortPins()
{
    if (_sort == ClassRule::SortNone)
        return;

    QList<QPair<QString, Pin*> > pins;

    QRegularExpression pattern(_sortPattern, QRegularExpression::CaseInsensitiveOption);
    QRegularExpression numPattern("([^0-9]*)([0-9]+)([^0-9]*)", QRegularExpression::CaseInsensitiveOption);
    foreach (Pin *pin, _pins)
    {
        QString sortPatern;

        QRegularExpressionMatch match = pattern.match(pin->name());
        if (match.hasMatch())
            sortPatern = match.captured(0);
        else
            sortPatern = pin->name();

        QRegularExpressionMatchIterator numMatchIt = numPattern.globalMatch(sortPatern);
        if (numMatchIt.hasNext())
            sortPatern.clear();
        while (numMatchIt.hasNext())
        {
            QRegularExpressionMatch numMatch = numMatchIt.next();
            sortPatern.append(numMatch.captured(1) + QString('0').repeated(5-numMatch.captured(2).size()) + numMatch.captured(2) + numMatch.captured(3));
        }

        pins.append(qMakePair(sortPatern, pin));
        if (_sort == ClassRule::SortAsc)
            qSort(pins.begin(), pins.end(), PinClass::pinPatterLessThan);
        if (_sort == ClassRule::SortDesc)
            qSort(pins.rbegin(), pins.rend(), PinClass::pinPatterLessThan);

        _pins.clear();
        for (int i=0; i<pins.size(); i++)
            _pins.append(pins[i].second);
    }

}

ClassRule::Position PinClass::positionValue() const
{
    return _position;
}

QString PinClass::positionStr() const
{
    return ClassRule::positionEnumStr.at(_position);
}

void PinClass::setPosition(const ClassRule::Position &position)
{
    _position = position;
    _positionSet = true;
}

ClassRule::Sort PinClass::sortValue() const
{
    return _sort;
}

QString PinClass::sortStr() const
{
    return ClassRule::sortEnumStr.at(_sort);
}

void PinClass::setSort(const ClassRule::Sort &sort)
{
    _sort = sort;
    _sortSet = true;
}

QString PinClass::sortPattern() const
{
    return _sortPattern;
}

void PinClass::setSortPattern(const QString &sortPattern)
{
    _sortPattern = sortPattern;
    _sortPatternSet = true;
}

void PinClass::addPin(Pin *pin)
{
    _pins.append(pin);
}

const QList<Pin *> &PinClass::pins() const
{
    return _pins;
}
