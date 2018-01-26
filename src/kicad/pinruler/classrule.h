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

#ifndef CLASSRULE_H
#define CLASSRULE_H

#include "kicad_global.h"

#include <QString>
#include <QStringList>

#include "rule.h"

class KICAD_EXPORT ClassRule : public Rule
{
public:
    ClassRule(const QString &selector = QString(".*"));

    virtual Type type() const;

    static QStringList propsName;
    virtual bool setProperty(const QString &name, const QString &value);
    virtual bool hasPropertySet(const QString &name) const;

    // position property [top, bottom, left, right, aside] left default
    enum Position {
        PositionTop,
        PositionBottom,
        PositionLeft,
        PositionRight,
        PositionASide
    };
    static QStringList positionEnumStr;
    Position positionValue() const;
    QString positionStr() const;
    bool hasPositionSet() const;
    void setPosition(Position position);
    void setPosition(const QString &position);

    // position property [none, asc, desc] none default
    enum Sort {
        SortNone,
        SortAsc,
        SortDesc
    };
    static QStringList sortEnumStr;
    Sort sortValue() const;
    QString sortStr() const;
    bool hasSortSet() const;
    void setSort(Sort sort);
    void setSort(const QString &sort);

    // sortPattern property
    QString sortPattern() const;
    bool hasSortPatternSet() const;
    void setSortPattern(const QString &sortPattern);

    // length property
    int length() const;
    bool hasLengthSet() const;
    void setLength(int length);

protected:
    Position _position;
    bool _positionSet;

    Sort _sort;
    bool _sortSet;

    QString _sortPattern;
    bool _sortPatternSet;

    int _length;
    bool _lengthSet;
};

#endif // CLASSRULE_H
