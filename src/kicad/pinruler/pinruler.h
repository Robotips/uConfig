#ifndef PINRULER_H
#define PINRULER_H

#include "kicad_global.h"

#include <QString>

#include "rulesset.h"
#include "pinclass.h"
#include "model/component.h"

class KICAD_EXPORT PinRuler
{
public:
    PinRuler(RulesSet *ruleSet=Q_NULLPTR);

    void organize(Component *component);

    RulesSet *ruleSet() const;
    void setRuleSet(RulesSet *ruleSet);

protected:
    RulesSet *_ruleSet;

    PinClass *addClass(const QString &className);
    PinClass *pinClass(const QString &className);
    QMap<QString, PinClass *> _pinClasses;
};

#endif // PINRULER_H
