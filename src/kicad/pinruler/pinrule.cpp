#include "pinrule.h"

#include <QDebug>

QStringList PinRule::propsName = QStringList()
        <<"class"
        <<"length"
        <<"elec_type";

PinRule::PinRule(const QString &selector)
    : Rule(selector)
{
    _length = 200;
    _lengthSet = false;
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

    switch (id)
    {
    case 0: // class
        setClassName(value);
        break;
    case 1: // length
        setLength(value.toInt());
        break;
    }

    return true;
}

QString PinRule::className(const QString &pinName) const
{
    if (pinName.isEmpty())
        return _className;
    if (!_selector.isValid())
        return _className;
    if (!_className.contains("\\"))
        return _className;

    QRegularExpressionMatch match = _selector.match(pinName, 0, QRegularExpression::NormalMatch);
    if (match.hasMatch())
    {
        QString newClassName;
        const QStringList &captures = (match.capturedTexts());
        newClassName = _className;
        if (captures.count()>1)
            newClassName.replace("\\1", captures[1]);
        if (captures.count()>2)
            newClassName.replace("\\2", captures[2]);
        if (captures.count()>3)
            newClassName.replace("\\3", captures[3]);
        return newClassName;
    }
    return _className;
}

void PinRule::setClassName(const QString &className)
{
    _className = className;
}

int PinRule::length() const
{
    return _length;
}

void PinRule::setLength(int lenght)
{
    _length = lenght;
    _lengthSet = true;
}

bool PinRule::lengthSet() const
{
    return _lengthSet;
}
