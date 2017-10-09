#include "pinrule.h"

QStringList PinRule::propsName = QStringList()
        <<"class"
        <<"elec_type";

PinRule::PinRule(const QString &selector)
    : Rule(selector)
{
}

Rule::Type PinRule::type() const
{
    return Rule::PinRuleType;
}

bool PinRule::setProperty(const QString &name, const QString &value)
{
    int id = PinRule::propsName.indexOf(name);

    if (id == -1)
        return false;

    return true;
}

QString PinRule::className() const
{
    return _className;
}

void PinRule::setClassName(const QString &className)
{
    _className = className;
}
