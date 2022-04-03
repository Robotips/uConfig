/**
 ** This file is part of the uConfig project.
 ** Copyright 2017-2020 Robotips, Sebastien CAUX (sebcaux)
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

#include "classrule.h"

QStringList ClassRule::propsName = QStringList() << "position"
                                                 << "sort"
                                                 << "sortpattern"
                                                 << "length"
                                                 << "priority"
                                                 << "visibility"
                                                 << "label"
                                                 << "rect";

QStringList ClassRule::positionEnumStr = QStringList() << "top"
                                                       << "bottom"
                                                       << "left"
                                                       << "right"
                                                       << "aside";

QStringList ClassRule::sortEnumStr = QStringList() << "none"
                                                   << "asc"
                                                   << "desc";

QStringList ClassRule::visibilityEnumStr = QStringList() << "visible"
                                                         << "hidden"
                                                         << "removed";

QStringList ClassRule::boolEnumStr = QStringList() << "false"
                                                   << "true";

ClassRule::ClassRule(const QString &selector)
    : Rule(selector)
{
    _position = PositionASide;
    _positionSet = false;

    _sort = SortAsc;
    _sortSet = false;

    _sortPattern = ".*";
    _sortPatternSet = false;

    _length = 200;
    _lengthSet = false;

    _priority = 0;
    _prioritySet = false;

    _visibility = VisibilityVisible;
    _visibilitySet = false;

    _label = "";
    _labelSet = false;

    _rect = 0;
    _rectSet = false;
}

Rule::Type ClassRule::type() const
{
    return Rule::ClassRuleType;
}

bool ClassRule::setProperty(const QString &name, const QString &value)
{
    int id = ClassRule::propsName.indexOf(name);

    if (id == -1)
    {
        return false;
    }

    switch (id)
    {
        case 0:  // position
            setPosition(value);
            break;
        case 1:  // sort
            setSort(value);
            break;
        case 2:  // sortpattern
            setSortPattern(value);
            break;
        case 3:  // length
            setLength(value.toInt());
            break;
        case 4:  // priority
            setPriority(value.toInt());
            break;
        case 5:  // visibility
            setVisibility(value);
            break;
        case 6:  // label
            setLabel(value);
            break;
        case 7:  // rect
            setRect(value.toInt());
            break;
    }

    return true;
}

bool ClassRule::hasPropertySet(const QString &name) const
{
    int id = ClassRule::propsName.indexOf(name);
    switch (id)
    {
        case 0:  // position
            return hasPositionSet();
        case 1:  // sort
            return hasSortSet();
        case 2:  // sortpattern
            return hasSortPatternSet();
        case 3:  // length
            return hasLengthSet();
        case 4:  // priority
            return hasPrioritySet();
        case 5:  // visibility
            return hasVisibilitySet();
        case 6:  // label
            return hasLabelSet();
        case 7:  // rect
            return hasRectSet();
    }
    return false;
}

ClassRule::Position ClassRule::positionValue() const
{
    return _position;
}

QString ClassRule::positionStr() const
{
    return ClassRule::positionEnumStr.at(_position);
}

bool ClassRule::hasPositionSet() const
{
    return _positionSet;
}

void ClassRule::setPosition(Position position)
{
    _positionSet = true;
    _position = position;
}

void ClassRule::setPosition(const QString &position)
{
    int id = ClassRule::positionEnumStr.indexOf(position);
    if (id != -1)
    {
        setPosition((ClassRule::Position)(id));
    }
}

ClassRule::Sort ClassRule::sortValue() const
{
    return _sort;
}

QString ClassRule::sortStr() const
{
    return ClassRule::positionEnumStr.at(_sort);
}

bool ClassRule::hasSortSet() const
{
    return _sortSet;
}

void ClassRule::setSort(ClassRule::Sort sort)
{
    _sortSet = true;
    _sort = sort;
}

void ClassRule::setSort(const QString &sort)
{
    int id = ClassRule::sortEnumStr.indexOf(sort);
    if (id != -1)
    {
        setSort((ClassRule::Sort)(id));
    }
}

QString ClassRule::sortPattern() const
{
    return _sortPattern;
}

bool ClassRule::hasSortPatternSet() const
{
    return _sortPatternSet;
}

void ClassRule::setSortPattern(const QString &sortPattern)
{
    _sortPattern = sortPattern;
    _sortPatternSet = true;
}

int ClassRule::length() const
{
    return _length;
}

bool ClassRule::hasLengthSet() const
{
    return _lengthSet;
}

void ClassRule::setLength(int length)
{
    if (length < 0)
    {
        length = 0;
    }
    _length = length;
    _lengthSet = true;
}

int ClassRule::priority() const
{
    return _priority;
}

bool ClassRule::hasPrioritySet() const
{
    return _prioritySet;
}

void ClassRule::setPriority(int priority)
{
    _priority = priority;
    _prioritySet = true;
}

ClassRule::Visibility ClassRule::visibilityValue() const
{
    return _visibility;
}

QString ClassRule::visibilityStr() const
{
    return ClassRule::visibilityEnumStr.at(_visibility);
}

bool ClassRule::hasVisibilitySet() const
{
    return _visibilitySet;
}

void ClassRule::setVisibility(Visibility visibility)
{
    _visibility = visibility;
    _visibilitySet = true;
}

void ClassRule::setVisibility(const QString &visibility)
{
    int id = ClassRule::visibilityEnumStr.indexOf(visibility);
    if (id != -1)
    {
        setVisibility((ClassRule::Visibility)(id));
    }
}

QString ClassRule::label() const
{
    return _label;
}

bool ClassRule::hasLabelSet() const
{
    return _labelSet;
}

void ClassRule::setLabel(const QString &label)
{
    _label = label;
    _labelSet = true;
}

bool ClassRule::rect() const
{
    return _rect != 0;
}

void ClassRule::setRect(int rect)
{
    _rect = rect;
    _rectSet = true;
}

void ClassRule::setRect(const QString &rect)
{
    int id = ClassRule::boolEnumStr.indexOf(rect);
    if (id != -1)
    {
        setRect(static_cast<int>(id == 1));
    }
}

bool ClassRule::hasRectSet() const
{
    return _rectSet;
}
