#ifndef LIB_H
#define LIB_H

#include "../kicad_global.h"

#include <QList>
#include <QString>
#include "component.h"

class KICAD_EXPORT Lib
{
public:
    Lib(const QString &name=QString());
    Lib(const Lib &other);
    ~Lib();

    QString name() const;
    void setName(const QString &name);

    Component *component(int i) const;
    const QList<Component *> &components() const;
    int componentsCount() const;
    void addComponent(Component *component);
    void removeComponent(Component *component);
    void takeComponent(Component *component);
    void releaseComponents();

    bool saveTo(const QString &fileName);
    bool readFrom(const QString &fileName);

    friend QTextStream &operator<<(QTextStream &stream, const Lib &lib);

private:
    QList<Component *> _components;
    QString _name;
};

#endif  // LIB_H
