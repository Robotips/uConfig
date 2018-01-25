#include "lib.h"

#include <QDateTime>
#include <QFile>
#include <QDebug>

/**
 * @brief Lib constructor
 * @param name optional library name
 */
Lib::Lib(const QString &name)
    : _name(name)
{
}

/**
 * @brief Lib copy operator, copy also components
 * @param other lib to copy
 */
Lib::Lib(const Lib &other)
{
    _name = other._name;

    for (int i=0; i<other._components.size(); i++)
        addComponent(new Component(*other._components[i]));
}

/**
 * @brief Lib destructor, delete also internal components.
 * Take component with `takeComponent` before delete the lib
 * if you want to keep some components
 */
Lib::~Lib()
{
    for (int i=0; i<_components.size(); i++)
        delete _components[i];
}

/**
 * @brief Lib name getter
 * @return name
 */
QString Lib::name() const
{
    return _name;
}

/**
 * @brief Lib name setter
 * @param name name to give
 */
void Lib::setName(const QString &name)
{
    _name = name;
}

/**
 * @brief Return component pointer at position i
 * @param i position of component
 * @return pointer to component
 */
Component *Lib::component(int i) const
{
    return _components[i];
}

/**
 * @brief Gives the list of internal components
 * @return const reference to the list
 */
const QList<Component *> &Lib::components() const
{
    return _components;
}

/**
 * @brief Returns the count of components
 * @return count of components
 */
int Lib::componentsCount() const
{
    return _components.count();
}

/**
 * @brief Adds a component to the lib
 * @param component component to add
 */
void Lib::addComponent(Component *component)
{
    _components.append(component);
}

/**
 * @brief Removes a component to the list and delete it
 * @param component component to remove
 * @see takeComponent
 */
void Lib::removeComponent(Component *component)
{
    if (_components.removeOne(component))
        delete component;
}

/**
 * @brief Removes a component to the list without delete it
 * @param component component to remove
 * @see removeComponent
 */
void Lib::takeComponent(Component *component)
{
    _components.removeOne(component);
}

/**
 * @brief Removes all components whithout delete it
 */
void Lib::releaseComponents()
{
    _components.clear();
}

/**
 * @brief Saves the lib to `fileName` path
 * @param fileName file path
 * @return true in case of success, false otherwise
 */
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

/**
 * @brief Reads a library from the file `fileName`
 * @param fileName file path
 * @return true in case of success, false otherwise
 */
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

/**
 * @brief Operator to serialise the lib in Kicad format
 * @param stream out stream
 * @param lib lib to serialise
 * @return stream
 */
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
