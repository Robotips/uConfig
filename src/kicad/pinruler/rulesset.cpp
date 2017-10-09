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

ClassRule *RulesSet::ruleForClass(const QString &className)
{
    for(int i=0; i<_classRules.size(); i++)
    {
        qDebug()<<_classRules[i]->selector()<<className;
        if (_classRules[i]->matchWithName(className))
        {
            return _classRules[i];
        }
    }
    return Q_NULLPTR;
}

PinRule *RulesSet::ruleForPin(const QString &pinName)
{
    for(int i=0; i<_pinRules.size(); i++)
    {
        if (_pinRules[i]->matchWithName(pinName))
        {
            return _pinRules[i];
        }
    }
    return Q_NULLPTR;
}
