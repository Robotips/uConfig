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

#include "classrule.h"

QStringList ClassRule::propsName = QStringList()
        <<"position"
        <<"sort"
        <<"sortpattern";

QStringList ClassRule::positionEnumStr = QStringList()
        <<"top"
        <<"bottom"
        <<"left"
        <<"right"
        <<"aside";

QStringList ClassRule::sortEnumStr = QStringList()
        <<"none"
        <<"asc"
        <<"desc";

ClassRule::ClassRule(const QString &selector)
    : Rule(selector)
{
    _position = PositionASide;
    _positionSet = false;
    _sort = SortAsc;
    _sortSet = false;
    _sortPattern = ".*";
    _sortPatternSet = false;
}

Rule::Type ClassRule::type() const
{
    return Rule::ClassRuleType;
}

bool ClassRule::setProperty(const QString &name, const QString &value)
{
    int id = ClassRule::propsName.indexOf(name);

    if (id == -1)
        return false;

    switch (id)
    {
    case 0: // position
        setPosition(value);
        break;
    case 1: // sort
        setSort(value);
        break;
    case 2: // sortpattern
        setSortPattern(value);
        break;
    }

    return true;
}

bool ClassRule::hasPropertySet(const QString &name) const
{
    int id = ClassRule::propsName.indexOf(name);
    switch (id)
    {
    case 0: // position
        return hasPositionSet();
    case 1: // sort
        return hasSortSet();
    case 2: // sortpattern
        return hasSortPatternSet();
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
        setPosition((ClassRule::Position)(id));
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
        setSort((ClassRule::Sort)(id));
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
