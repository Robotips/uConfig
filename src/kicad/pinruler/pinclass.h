#ifndef PINCLASS_H
#define PINCLASS_H

#include "kicad_global.h"

#include <QString>
#include <QList>

#include "pin.h"
#include "classrule.h"

class KICAD_EXPORT PinClass
{
public:
    PinClass(const QString &className);

    QString className() const;

    void applyRule(ClassRule *rule);
    void applyRules(QList<ClassRule *> rules);

    static bool pinPatterLessThan(QPair<QString, Pin *> pin1, QPair<QString, Pin *> pin2);
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

    void addPin(Pin *pin);
    const QList<Pin *> &pins() const;

protected:
    QString _className;

    ClassRule::Position _position;
    bool _positionSet;

    ClassRule::Sort _sort;
    bool _sortSet;

    QString _sortPattern;
    bool _sortPatternSet;

    QList<Pin *> _pins;
};

#endif // PINCLASS_H
