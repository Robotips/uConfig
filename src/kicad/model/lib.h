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

#ifndef LIB_H
#define LIB_H

#include <QList>
#include <QString>
#include <QtCore/qglobal.h>

#include "component.h"

class KICAD_EXPORT Lib
{
public:
    Lib(QString name = QString());
    Lib(const Lib &other);
    ~Lib();

    const QString &name() const;
    void setName(const QString &name);

    Component *component(int i) const;
    const QList<Component *> &components() const;
    int componentsCount() const;
    void addComponent(Component *component);
    void removeComponent(Component *component);
    void takeComponent(Component *component);
    void releaseComponents();
    void clear();

    bool saveTo(const QString &fileName);
    bool readFrom(const QString &fileName);

private:
    QList<Component *> _components;
    QString _name;
};

#endif  // LIB_H
