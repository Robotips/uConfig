#include "rulesset.h"

#include <QDebug>

RulesSet::RulesSet()
{
}

RulesSet::RulesSet(const RulesSet &other)
{
    for(int i=0; i<other._classRules.size(); i++)
        addRule(new ClassRule(*other._classRules[i]));
    for(int i=0; i<other._pinRules.size(); i++)
        addRule(new PinRule(*other._pinRules[i]));
}

RulesSet::~RulesSet()
{
    for(int i=0; i<_classRules.size(); i++)
        delete _classRules[i];
    for(int i=0; i<_pinRules.size(); i++)
        delete _pinRules[i];
}

void RulesSet::addRule(Rule *rule)
{
    if (rule->type() == Rule::ClassRuleType)
        _classRules.append(static_cast<ClassRule*>(rule));
    else
        _pinRules.append(static_cast<PinRule*>(rule));
}

QList<ClassRule*> RulesSet::rulesForClass(const QString &className)
{
    QList<ClassRule*> rules;
    for(int i=0; i<_classRules.size(); i++)
    {
        if (_classRules[i]->matchWithName(className))
        {
            rules.append(_classRules[i]);
        }
    }
    return rules;
}

QList<PinRule*> RulesSet::rulesForPin(const QString &pinName)
{
    QList<PinRule*> rules;
    for(int i=0; i<_pinRules.size(); i++)
    {
        //const QStringList &vpins = pinName.split('/');
        //foreach (const QString &vpin, vpins)
        {
            if (_pinRules[i]->matchWithName(pinName))
            {
                rules.append(_pinRules[i]);
            }
        }
    }
    return rules;
}
