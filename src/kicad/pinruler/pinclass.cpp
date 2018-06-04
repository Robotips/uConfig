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

    _length = 200;
    _lengthSet = false;

    _priority = 0;
    _prioritySet = false;

    _visibility = ClassRule::VisibilityVisible;
    _visibilitySet = false;
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
    {
        QString newSortPatern = rule->sortPattern();
        QRegularExpressionMatch match = rule->selector().match(_className, 0, QRegularExpression::NormalMatch);
        if (match.hasMatch())
        {
            const QStringList &captures = (match.capturedTexts());
            if (captures.count()>1)
                newSortPatern.replace("\\1", captures[1]);
            if (captures.count()>2)
                newSortPatern.replace("\\2", captures[2]);
            if (captures.count()>3)
                newSortPatern.replace("\\3", captures[3]);
        }
        setSortPattern(newSortPatern);
    }
    if (!_lengthSet && rule->hasLengthSet())
        setLength(rule->length());
    if (!_prioritySet && rule->hasPrioritySet())
        setPriority(rule->priority());
    if (!_visibilitySet && rule->hasVisibilitySet())
        setVisibility(rule->visibilityValue());
}

void PinClass::applyRules(QList<ClassRule *> rules)
{
    foreach (ClassRule *rule, rules)
        applyRule(rule);
}

void PinClass::sortPins()
{
    if (_sort == ClassRule::SortNone)
        return;

    QRegularExpression pattern(_sortPattern, QRegularExpression::CaseInsensitiveOption);
    QRegularExpression numPattern("([^0-9]*)([0-9]+)([^0-9]*)", QRegularExpression::CaseInsensitiveOption);

    foreach (PinClassItem *pinItem, _pins)
    {
        QString sortPatern;
        QString pinName = pinItem->pin()->name();
        pinName.replace("~", "");

        if (pattern.isValid())
        {
            QRegularExpressionMatch match = pattern.match(pinName);
            if (match.hasMatch())
                sortPatern = match.captured(0);
            else
                sortPatern = pinName;
        }
        else
            sortPatern = pinName;
        //sortPatern = sortPatern.leftJustified(n, ' ');

        QRegularExpressionMatchIterator numMatchIt = numPattern.globalMatch(sortPatern);
        if (numMatchIt.hasNext())
            sortPatern.clear();
        while (numMatchIt.hasNext())
        {
            QRegularExpressionMatch numMatch = numMatchIt.next();
            sortPatern.append(numMatch.captured(1) + QString('0').repeated(5-numMatch.captured(2).size()) + numMatch.captured(2) + numMatch.captured(3));
        }
        QRegularExpressionMatchIterator padMatchIt = numPattern.globalMatch(pinItem->pin()->padName());
        while (padMatchIt.hasNext())
        {
            QRegularExpressionMatch padMatch = padMatchIt.next();
            sortPatern.append(padMatch.captured(1) + QString('0').repeated(5-padMatch.captured(2).size()) + padMatch.captured(2) + padMatch.captured(3));
        }

        pinItem->setSortLabel(sortPatern);
    }

    if (_sort == ClassRule::SortAsc)
        qSort(_pins.begin(), _pins.end(), PinClassItem::pinLessThan);
    if (_sort == ClassRule::SortDesc)
        qSort(_pins.begin(), _pins.end(), PinClassItem::pinGreaterThan);

}

void PinClass::placePins(const QPoint &basePos)
{
    QPoint pos = basePos;
    QPoint offset;
    QPoint translate;
    Pin::Direction direction;

    switch (_position)
    {
    case ClassRule::PositionTop:
        direction = Pin::Down;
        offset = QPoint(100, 0);
        translate = QPoint(0, -_length);
        break;
    case ClassRule::PositionBottom:
        direction = Pin::Up;
        offset = QPoint(100, 0);
        translate = QPoint(0, _length);
        break;
    case ClassRule::PositionLeft:
        direction = Pin::Right;
        offset = QPoint(0, 100);
        translate = QPoint(-_length, 0);
        break;
    case ClassRule::PositionRight:
    case ClassRule::PositionASide:
        direction = Pin::Left;
        offset = QPoint(0, 100);
        translate = QPoint(_length, 0);
        break;
    }
    sortPins();
    foreach (PinClassItem *pinItem, _pins)
    {
        pinItem->pin()->setDirection(direction);
        pinItem->pin()->setPos(pos + translate);
        pinItem->pin()->setLength(_length);
        if (visibilityValue() != VisibilityVisible)
            pinItem->pin()->setPinType(Pin::NotVisible);
        pos += offset;
    }
}

QRect PinClass::rect() const
{
    QRect rect;
    int maxLenght = 0;
    foreach (PinClassItem *pinItem, _pins)
    {
        if (pinItem->pin()->name().size() > maxLenght)
            maxLenght = pinItem->pin()->name().size();
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

void PinClass::addPinItem(PinClassItem *pin)
{
    _pins.append(pin);
}

const QList<PinClassItem *> &PinClass::pins() const
{
    return _pins;
}
