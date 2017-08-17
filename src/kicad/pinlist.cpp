#include "pinlist.h"

PinList::PinList()
{
}

const QList<Pin> &PinList::pins() const
{
    return _pins;
}

QList<Pin> &PinList::pins()
{
    return _pins;
}

bool PinList::addPin(const Pin &pin)
{
    if(existPin(pin)) return false;
    _pins.append(pin);
    return true;
}

bool PinList::removePin(const Pin &pin)
{
    QList<Pin>::iterator it;
    for(it=_pins.begin(); it!=_pins.end(); ++it)
    {
        if(pin==*it) _pins.erase(it);
    }
    return true;
}

bool PinList::existPin(const Pin &newpin)
{
    foreach (const Pin &pin, _pins)
    {
        if(newpin==pin) return true;
    }
    return false;
}
