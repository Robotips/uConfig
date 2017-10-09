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
    _position = PositionLeft;
    _positionSet = false;
    _sort = SortNone;
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

ClassRule::Position ClassRule::positionValue() const
{
    return _position;
}

QString ClassRule::positionStr() const
{
    return ClassRule::positionEnumStr.at(_position);
}

bool ClassRule::isPositionSet() const
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

bool ClassRule::isSortSet() const
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

bool ClassRule::isSortPatternSet() const
{
    return _sortPatternSet;
}

void ClassRule::setSortPattern(const QString &sortPattern)
{
    _sortPattern = sortPattern;
    _sortPatternSet = true;
}
