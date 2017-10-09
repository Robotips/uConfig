#ifndef PINRULER_H
#define PINRULER_H

#include "kicad_global.h"

#include <QString>

#include "rulesset.h"
#include "pinclass.h"
#include "component.h"

class KICAD_EXPORT PinRuler
{
public:
    PinRuler(RulesSet *ruleSet);

    void organize(Component *component);

protected:
    RulesSet *_ruleSet;

    PinClass *addClass(const QString &className);
    PinClass *pinClass(const QString &className);
    QMap<QString, PinClass *> _pinClasses;
};

#endif // PINRULER_H
