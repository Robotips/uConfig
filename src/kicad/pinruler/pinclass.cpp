#include "pinclass.h"

#include <QDebug>

PinClass::PinClass(const QString &className)
    : _className(className)
{
    // properties default values
    _position = ClassRule::PositionASide;
    _positionSet = false;
    _sort = ClassRule::SortAsc;
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
    if (!_positionSet && rule->hasPositionSet())
        setPosition(rule->positionValue());
    if (!_sortSet && rule->hasSortSet())
        setSort(rule->sortValue());
    if (!_sortPatternSet && rule->hasSortPatternSet())
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
        QRegularExpressionMatchIterator padMatchIt = numPattern.globalMatch(pin->padName());
        while (padMatchIt.hasNext())
        {
            QRegularExpressionMatch padMatch = padMatchIt.next();
            sortPatern.append(padMatch.captured(1) + QString('0').repeated(5-padMatch.captured(2).size()) + padMatch.captured(2) + padMatch.captured(3));
        }

        pins.append(qMakePair(sortPatern, pin));
        qSort(pins.begin(), pins.end(), PinClass::pinPatterLessThan);

        _pins.clear();
        if (_sort == ClassRule::SortAsc)
            for (int i=0; i<pins.size(); i++)
                _pins.append(pins[i].second);
        if (_sort == ClassRule::SortDesc)
            for (int i=pins.size()-1; i>=0; i--)
                _pins.append(pins[i].second);
    }

}

void PinClass::placePins(const QPoint &basePos)
{
    QPoint pos = basePos;
    QPoint offset;
    Pin::Direction direction;

    switch (_position)
    {
    case ClassRule::PositionTop:
        direction = Pin::Down;
        offset = QPoint(100, 0);
        break;
    case ClassRule::PositionBottom:
        direction = Pin::Up;
        offset = QPoint(100, 0);
        break;
    case ClassRule::PositionLeft:
        direction = Pin::Right;
        offset = QPoint(0, 100);
        break;
    case ClassRule::PositionRight:
        direction = Pin::Left;
        offset = QPoint(0, 100);
        break;
    case ClassRule::PositionASide:
        direction = Pin::Left;
        offset = QPoint(0, 100);
        break;
    }
    sortPins();
    foreach (Pin *pin, _pins)
    {
        pin->setDirection(direction);
        pin->setPos(pos);
        pos += offset;
    }
}

QRect PinClass::rect() const
{
    QRect rect;
    int maxLenght = 0;
    foreach (Pin *pin, _pins)
    {
        if (pin->name().size() > maxLenght)
            maxLenght = pin->name().size();
    }

    switch (_position)
    {
    case ClassRule::PositionTop:
    case ClassRule::PositionBottom:
        rect = QRect(0, 0, _pins.size() * 100, maxLenght*50);
        break;
    case ClassRule::PositionLeft:
    case ClassRule::PositionRight:
    case ClassRule::PositionASide:
        rect = QRect(0, 0, maxLenght*50, _pins.size() * 100);
        break;
    }
    return rect;
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
