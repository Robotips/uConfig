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

#ifndef PINCLASS_H
#define PINCLASS_H

#include "kicad_global.h"

#include <QString>
#include <QList>
#include <QRect>

#include "classrule.h"
#include "pinclassitem.h"

class KICAD_EXPORT PinClass
{
public:
    PinClass(const QString &className);

    QString className() const;

    void applyRule(ClassRule *rule);
    void applyRules(QList<ClassRule *> rules);

    void sortPins();

    void placePins(const QPoint &pos);

    QRect rect() const;

    ClassRule::Position positionValue() const;
    QString positionStr() const;
    void setPosition(const ClassRule::Position &position);

    ClassRule::Sort sortValue() const;
    QString sortStr() const;
    void setSort(const ClassRule::Sort &sort);

    QString sortPattern() const;
    void setSortPattern(const QString &sortPattern);

    int length() const;
    void setLength(int length);

    int priority() const;
    void setPriority(int priority);

    void addPinItem(PinClassItem *pin);
    const QList<PinClassItem *> &pins() const;

protected:
    QString _className;

    ClassRule::Position _position;
    bool _positionSet;

    ClassRule::Sort _sort;
    bool _sortSet;

    QString _sortPattern;
    bool _sortPatternSet;

    int _length;
    bool _lengthSet;

    int _priority;
    bool _prioritySet;

    QList<PinClassItem *> _pins;
};

#endif // PINCLASS_H
