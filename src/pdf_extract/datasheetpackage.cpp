#include "datasheetpackage.h"

DatasheetPackage::DatasheetPackage()
{
}

DatasheetPackage::~DatasheetPackage()
{
    for (int i=0; i<pins.size(); i++)
        delete pins[i];
}

Component *DatasheetPackage::toComponent() const
{
    Component *comp = new Component();
    if (!icname.isEmpty())
        comp->setName(icname.first());
    else
        comp->setName(name);

    for (int i = 1; i < icname.count(); i++)
        comp->addAlias(icname.at(i));

    foreach (DatasheetPin *dpin, pins)
    {
        Pin *pin = new Pin();
        pin->setName(dpin->name);
        pin->setPadname(QString::number(dpin->pin));
        comp->addPin(pin);
    }
    comp->setDebugInfo(image);

    return comp;
}
