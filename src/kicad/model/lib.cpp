/**
 ** This file is part of the uConfig project.
 ** Copyright 2017-2020 Robotips, Sebastien CAUX (sebcaux)
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation, either version 3 of the License, or
 ** (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with this program. If not, see <http://www.gnu.org/licenses/>.
 **/

#include "lib.h"

#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <utility>

#include <parser/libparser.h>

/**
 * @brief Lib constructor
 * @param name optional library name
 */
Lib::Lib(QString name)
    : _name(std::move(name))
{
}

/**
 * @brief Lib copy operator, copy also components
 * @param other lib to copy
 */
Lib::Lib(const Lib &other)
{
    _name = other._name;

    for (auto component : other._components)
    {
        addComponent(new Component(*component));
    }
}

/**
 * @brief Lib destructor, delete also internal components.
 * Take component with `takeComponent` before delete the lib
 * if you want to keep some components
 */
Lib::~Lib()
{
    clear();
}

/**
 * @brief Lib name getter
 * @return name
 */
const QString &Lib::name() const
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
    {
        delete component;
    }
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
 * @brief Removes and deletes all components of lib
 */
void Lib::clear()
{
    for (auto &component : _components)
    {
        delete component;
    }
    _components.clear();
}

/**
 * @brief Saves the lib to `fileName` path
 * @param fileName file path
 * @return true in case of success, false otherwise
 */
bool Lib::saveTo(const QString &fileName)
{
    return LibParser::saveLib(this, fileName, LibParser::KicadLib);
}

/**
 * @brief Reads a library from the file `fileName`
 * @param fileName file path
 * @return true in case of success, false otherwise
 */
bool Lib::readFrom(const QString &fileName)
{
    return LibParser::loadLib(this, fileName, LibParser::KicadLib) != nullptr;
}
