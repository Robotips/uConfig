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

#ifndef COMPONENTLIBITEMMODEL_H
#define COMPONENTLIBITEMMODEL_H

#include <QAbstractItemModel>
#include <QtCore/qglobal.h>

#include "model/lib.h"

class KICAD_EXPORT ComponentLibItemModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    ComponentLibItemModel(Lib *lib = nullptr, QObject *parent = nullptr);

    Lib *lib() const;
    void setLib(Lib *lib);

    Component *activeComponent() const;
    void setActiveComponent(Component *activeComponent);

    void addComponent(Component *component);
    QList<Component *> components() const;

    QModelIndex index(Component *component, int coulumn = 0);
    Component *component(const QModelIndex &index) const;
    void remove(const QModelIndex &index);

    void selectAll(bool selected = true);
    bool selectedMode() const;
    void setSelectedMode(bool selectedMode);
    const QList<Component *> &selectedComponents() const;

    enum Column
    {
        Name,
        PinCount,
        UnitCount,
        Package,
        ColumnCount
    };

public slots:
    void updateComponent(Component *component);

    // QAbstractItemModel interface
public:
    int columnCount(const QModelIndex &parent) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

private:
    Lib *_lib;
    QList<Component *> _selectedComponents;
    Component *_activeComponent;
    bool _selectedMode;
};

#endif  // COMPONENTLIBITEMMODEL_H
