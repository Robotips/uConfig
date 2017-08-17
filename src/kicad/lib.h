#ifndef LIB_H
#define LIB_H

#include <QList>
#include <QString>
#include "component.h"

class Lib
{
public:
    Lib();

    QString name() const;
    void setName(const QString &name);

    QList<Component> components() const;
    void addComponent(const Component &component);

    bool saveTo(const QString &fileName);

    friend QTextStream& operator<<(QTextStream &stream, const Lib &lib);

private:
    QList<Component> _components;
    QString _name;
};

#endif // LIB_H
