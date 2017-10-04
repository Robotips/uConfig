#ifndef LIB_H
#define LIB_H

#include "kicad_global.h"

#include <QList>
#include <QString>
#include "component.h"

class KICAD_EXPORT Lib
{
public:
    Lib();

    QString name() const;
    void setName(const QString &name);

    QList<Component *> components() const;
    void addComponent(Component *component);

    bool saveTo(const QString &fileName);
    bool readFrom(const QString &fileName);

    friend QTextStream &operator<<(QTextStream &stream, const Lib &lib);
    friend QTextStream &operator>>(QTextStream &stream, Lib &lib);

private:
    QList<Component *> _components;
    QString _name;
};

#endif  // LIB_H
