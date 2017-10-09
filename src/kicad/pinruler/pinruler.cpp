#include "pinruler.h"

#include <QDebug>

PinRuler::PinRuler(RulesSet *ruleSet)
{
    _ruleSet = ruleSet;
}

void PinRuler::organize(Component *component)
{
    PinClass *defaultClass = pinClass("default");
    foreach (Pin *pin, component->pins())
    {
        const QList<PinRule*> &rules = _ruleSet->rulesForPin(pin->name());
        if (rules.isEmpty())
        {
            pin->setPinType(Pin::NotVisible);
            defaultClass->addPin(pin);
        }
        else
        {
            QString className = rules.first()->className(pin->name());
            PinClass *mpinClass = pinClass(className);
            mpinClass->addPin(pin);
        }
    }

    foreach (PinClass *mpinClass, _pinClasses)
    {
        qDebug()<<">"<<mpinClass->className()<<mpinClass->position();
        foreach (Pin *pin, mpinClass->pins())
        {
            qDebug()<<" - "<<pin->name();
        }
    }
}

PinClass *PinRuler::addClass(const QString &className)
{
    PinClass *pinClass = new PinClass(className);

    const QList<ClassRule*> &rules = _ruleSet->rulesForClass(className);
    pinClass->applyRules(rules);

    _pinClasses.insert(className, pinClass);
    return pinClass;
}

PinClass *PinRuler::pinClass(const QString &className)
{
    QMap<QString, PinClass *>::const_iterator classFind = _pinClasses.constFind(className);
    if (classFind == _pinClasses.cend())
        return addClass(className);
    else
        return *classFind;
}
