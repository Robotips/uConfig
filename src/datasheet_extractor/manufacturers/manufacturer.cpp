#include "manufacturer.h"

#include "manufacturers.h"

Manufacturer::Manufacturer()
{
}

const QString &Manufacturer::name() const
{
    return _name;
}

Manufacturer *Manufacturer::manufacturerFromName(const QString &name)
{
    if(name=="Microchip") return new Microchip();
    return NULL;
}
