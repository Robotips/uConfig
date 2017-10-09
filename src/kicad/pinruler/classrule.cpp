#include "classrule.h"

QStringList ClassRule::propsName = QStringList()
        <<"position"
        <<"border";

ClassRule::ClassRule(const QString &selector)
    : Rule(selector)
{
}

Rule::Type ClassRule::type() const
{
    return Rule::ClassRuleType;
}

bool ClassRule::setProperty(const QString &name, const QString &value)
{
    int id = ClassRule::propsName.indexOf(name);

    if (id == -1)
        return false;

    return true;
}
