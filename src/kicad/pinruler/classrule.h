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
    bool isPositionSet() const;
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
    bool isSortSet() const;
    void setSort(Sort sort);
    void setSort(const QString &sort);

    // sortPattern property
    QString sortPattern() const;
    bool isSortPatternSet() const;
    void setSortPattern(const QString &sortPattern);

protected:
    Position _position;
    bool _positionSet;

    Sort _sort;
    bool _sortSet;

    QString _sortPattern;
    bool _sortPatternSet;
};

#endif // CLASSRULE_H
