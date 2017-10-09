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

    virtual bool setProperty(const QString &name, const QString &value);

    enum Position {
        PositionTop,
        PositionBottom,
        PositionLeft,
        PositionRight,
        PositionASide
    };
    Position position() const;
    bool isPositionSet() const;
    void setPosition(const Position &position);
    void setPosition(const QString &position);

    static QStringList propsName;

protected:
    Position _position;
    bool _positionSet;
};

#endif // CLASSRULE_H
