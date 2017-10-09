#include "pinclass.h"

PinClass::PinClass(const QString &className)
    : _className(className)
{
    _position = ClassRule::PositionLeft;
}

QString PinClass::className() const
{
    return _className;
}

void PinClass::applyRule(ClassRule *rule)
{
    if (rule->isPositionSet())
        setPosition(rule->position());
}

void PinClass::applyRules(QList<ClassRule *> rules)
{
    foreach (ClassRule *rule, rules)
        applyRule(rule);
}

ClassRule::Position PinClass::position() const
{
    return _position;
}

void PinClass::setPosition(const ClassRule::Position &position)
{
    _position = position;
}

void PinClass::addPin(Pin *pin)
{
    _pins.append(pin);
}

const QList<Pin *> &PinClass::pins() const
{
    return _pins;
}
