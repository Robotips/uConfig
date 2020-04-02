/**
 ** This file is part of the uConfig project.
 ** Copyright 2017-2020 Robotips, Sebastien CAUX (sebcaux)
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

#include "rulesset.h"

#include <QDebug>

RulesSet::RulesSet()
{
    ClassRule *top = new ClassRule("top");
    top->setPosition(ClassRule::PositionTop);
    addRule(top);
    ClassRule *bottom = new ClassRule("bottom");
    bottom->setPosition(ClassRule::PositionBottom);
    addRule(bottom);
    ClassRule *left = new ClassRule("left");
    left->setPosition(ClassRule::PositionLeft);
    addRule(left);
    ClassRule *right = new ClassRule("right");
    right->setPosition(ClassRule::PositionRight);
    addRule(right);
}

RulesSet::RulesSet(const RulesSet &other)
{
    for (int i = 0; i < other._classRules.size(); i++)
    {
        addRule(new ClassRule(*other._classRules[i]));
    }
    for (int i = 0; i < other._pinRules.size(); i++)
    {
        addRule(new PinRule(*other._pinRules[i]));
    }
}

RulesSet::~RulesSet()
{
    for (int i = 0; i < _classRules.size(); i++)
    {
        delete _classRules[i];
    }
    for (int i = 0; i < _pinRules.size(); i++)
    {
        delete _pinRules[i];
    }
}

void RulesSet::addRule(Rule *rule)
{
    if (rule->type() == Rule::ClassRuleType)
    {
        _classRules.append(dynamic_cast<ClassRule *>(rule));
    }
    else
    {
        _pinRules.append(dynamic_cast<PinRule *>(rule));
    }
}

QList<ClassRule *> RulesSet::rulesForClass(const QString &className)
{
    QList<ClassRule *> rules;
    for (int i = 0; i < _classRules.size(); i++)
    {
        if (_classRules[i]->matchWithName(className))
        {
            rules.append(_classRules[i]);
        }
    }
    return rules;
}

QList<PinRule *> RulesSet::rulesForPin(const QString &pinName)
{
    QList<PinRule *> rules;
    for (int i = 0; i < _pinRules.size(); i++)
    {
        // const QStringList &vpins = pinName.split('/');
        // for (const QString &vpin : vpins)
        {
            if (_pinRules[i]->matchWithName(pinName))
            {
                rules.append(_pinRules[i]);
            }
        }
    }
    return rules;
}
