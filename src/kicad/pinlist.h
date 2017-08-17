#ifndef PINLIST_H
#define PINLIST_H

#include "pin.h"

#include <QList>

class PinList
{
public:
    PinList();

    const QList<Pin> &pins() const;
    QList<Pin> &pins();
    bool addPin(const Pin &pin);
    bool removePin(const Pin &pin);

    bool existPin(const Pin &newpin);

private:
    QList<Pin> _pins;
};

#endif // PINLIST_H
