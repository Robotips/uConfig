#ifndef MANUFACTURER_H
#define MANUFACTURER_H

#include <QString>

class Manufacturer
{
public:
    Manufacturer();

    const QString &name() const;

    static Manufacturer *manufacturerFromName(const QString &name);

protected:
    QString _name;
};

#endif // MANUFACTURER_H
