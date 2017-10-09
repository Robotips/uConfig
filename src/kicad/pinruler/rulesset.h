#ifndef RULESSET_H
#define RULESSET_H

#include "kicad_global.h"

#include <QString>
#include <QList>

#include "pinrule.h"
#include "classrule.h"

class KICAD_EXPORT RulesSet
{
public:
    RulesSet();
    RulesSet(const RulesSet &other);
    ~RulesSet();

    void addRule(Rule *rule);

    ClassRule *ruleForClass(const QString &className);
    PinRule *ruleForPin(const QString &pinName);

protected:
    QList<ClassRule *> _classRules;
    QList<PinRule *> _pinRules;
};

#endif // RULESSET_H
