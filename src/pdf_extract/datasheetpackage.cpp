#include "datasheetpackage.h"

DatasheetPackage::DatasheetPackage()
{
}

DatasheetPackage::~DatasheetPackage()
{
}

Component DatasheetPackage::toComponent() const
{
    Component comp;
    if (!icname.isEmpty())
        comp.setName(icname.first());
    for (int i = 1; i < icname.count(); i++) comp.addAlias(icname.at(i));

    foreach (const DatasheetPin &dpin, pins)
    {
        Pin *pin = new Pin();
        pin->setName(dpin.name);
        pin->setPadname(QString::number(dpin.pin));
        comp.addPin(pin);
    }

    return comp;
}
