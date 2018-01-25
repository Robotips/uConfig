/**
 ** This file is part of the uConfig project.
 ** Copyright 2018 Robotips sebastien.caux@robotips.fr.
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation, either version 3 of the License, or
 ** (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with this program. If not, see <http://www.gnu.org/licenses/>.
 **/

#include "pinrule.h"

#include <QDebug>

QStringList PinRule::propsName = QStringList()
        <<"class"
        <<"length"
        <<"elec_type";

PinRule::PinRule(const QString &selector)
    : Rule(selector)
{
    _classSet = false;

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

bool PinRule::hasPropertySet(const QString &name) const
{
    int id = PinRule::propsName.indexOf(name);
    switch (id)
    {
    case 0: // class
        return hasClassSet();
    case 1: // length
        return hasLengthSet();
    }
    return false;
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
    _classSet = true;
}

bool PinRule::hasClassSet() const
{
    return _classSet;
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

bool PinRule::hasLengthSet() const
{
    return _lengthSet;
}
