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

    QList<ClassRule*> rulesForClass(const QString &className);
    QList<PinRule*> rulesForPin(const QString &pinName);

protected:
    QList<ClassRule *> _classRules;
    QList<PinRule *> _pinRules;
};

#endif // RULESSET_H
