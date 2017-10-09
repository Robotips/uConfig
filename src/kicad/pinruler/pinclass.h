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

    ClassRule::Position position() const;
    void setPosition(const ClassRule::Position &position);

    void addPin(Pin *pin);
    const QList<Pin *> &pins() const;

protected:
    QString _className;
    ClassRule::Position _position;

    QList<Pin *> _pins;
};

#endif // PINCLASS_H
