#include "classrule.h"

QStringList ClassRule::propsName = QStringList()
        <<"position"
        <<"border";

ClassRule::ClassRule(const QString &selector)
    : Rule(selector)
{
    _position = PositionLeft;
    _positionSet = false;
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

    return true;
}

ClassRule::Position ClassRule::position() const
{
    return _position;
}

bool ClassRule::isPositionSet() const
{
    return _positionSet;
}

void ClassRule::setPosition(const ClassRule::Position &position)
{
    _positionSet = true;
    _position = position;
}

void ClassRule::setPosition(const QString &position)
{
    QStringList propsValue = QStringList()<<"top"<<"bottom"<<"left"<<"right"<<"aside";

    setPosition((ClassRule::Position)(propsValue.indexOf(position)));
}
