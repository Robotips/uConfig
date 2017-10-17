#include "lib.h"

#include <QDateTime>
#include <QFile>
#include <QDebug>

Lib::Lib()
{
}

QString Lib::name() const
{
    return _name;
}

void Lib::setName(const QString &name)
{
    _name = name;
}

const QList<Component *> &Lib::components() const
{
    return _components;
}

void Lib::addComponent(Component *component)
{
    _components.append(component);
}

bool Lib::saveTo(const QString &fileName)
{
    QFile output(fileName);
    if (!output.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    QTextStream stream(&output);
    stream << *this;

    output.close();
    return true;
}

bool Lib::readFrom(const QString &fileName)
{
    QFile input(fileName);
    if (!input.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;
    QTextStream stream(&input);

    stream.readLine();

    Component *component;
    do
    {
        component = new Component();
        stream >> *component;
        if (component->isValid())
            addComponent(component);
        else
            delete component;
    } while (!stream.atEnd());

    return true;
}

QTextStream &operator<<(QTextStream &stream, const Lib &lib)
{
    // header
    stream << "EESchema-LIBRARY Version 2.3  Date: "
           << QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss")
           << '\n';
    stream << "#encoding utf-8" << '\n';
    stream << "#created with uConfig by Sebastien CAUX (sebcaux)" << '\n';
    stream << "#https://github.com/Robotips/uConfig" << '\n';

    // components
    foreach (Component *component, lib._components)
    {
        stream << *component << '\n';
    }

    // footer
    stream << "#" << '\n';
    stream << "#End Library";

    return stream;
}

QTextStream &operator>>(QTextStream &stream, Lib &lib)
{
    Q_UNUSED(stream)
    Q_UNUSED(lib)
    // TODO implement me
    return stream;
}
