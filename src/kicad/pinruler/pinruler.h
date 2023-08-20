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

#ifndef PINRULER_H
#define PINRULER_H

#include <QString>
#include <QtCore/qglobal.h>

#include "model/component.h"
#include "pinclass.h"
#include "rulesset.h"

class KICAD_EXPORT PinRuler
{
public:
    PinRuler(RulesSet *ruleSet = nullptr);

    void organize(Component *component);

    RulesSet *ruleSet() const;
    void setRuleSet(RulesSet *ruleSet);

protected:
    RulesSet *_ruleSet;

    PinClass *addClass(const QString &className);
    PinClass *pinClass(const QString &className);
    QMap<QString, PinClass *> _pinClasses;
};

#endif  // PINRULER_H
