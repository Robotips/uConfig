#ifndef PINCLASSIFIER_H
#define PINCLASSIFIER_H

#include "kicad_global.h"

#include "component.h"

class QDebug;

class KICADSHARED_EXPORT PinClassifier
{
public:
    PinClassifier(const Component &component);

private:
    const Component &_component;
};

class PinCompare
{
public:
    PinCompare(const QString &name, bool number = false);

    QString internalName() const;
    void setInternalName(const QString &internalName);

    QString name() const;
    void setName(const QString &name);

    int distanceTo(const PinCompare &other);
    friend bool operator<(const PinCompare &pin1, const PinCompare &pin2);

    friend QDebug operator<<(QDebug dbg, const PinCompare &pin);

private:
    QString _name;
    QString _internalName;
};

#endif  // PINCLASSIFIER_H
