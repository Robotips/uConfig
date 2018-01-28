#include "pinclassitem.h"

PinClassItem::PinClassItem(Pin *pin)
    : _pin(pin)
{
    _priority = 0;
}

Pin *PinClassItem::pin() const
{
    return _pin;
}

void PinClassItem::setPin(Pin *pin)
{
    _pin = pin;
}

int PinClassItem::priority() const
{
    return _priority;
}

void PinClassItem::setPriority(int priority)
{
    _priority = priority;
}

QString PinClassItem::sortLabel() const
{
    return _sortLabel;
}

void PinClassItem::setSortLabel(const QString &sortLabel)
{
    _sortLabel = sortLabel;
}

bool PinClassItem::pinLessThan(PinClassItem *pin1, PinClassItem *pin2)
{
    if (pin1->priority() == pin2->priority())
        return pin1->_sortLabel.toLower() < pin2->_sortLabel.toLower();
    return pin1->priority() > pin2->priority();
}

bool PinClassItem::pinGreaterThan(PinClassItem *pin1, PinClassItem *pin2)
{
    if (pin1->priority() == pin2->priority())
        return pin1->_sortLabel.toLower() > pin2->_sortLabel.toLower();
    return pin1->priority() < pin2->priority();
}
