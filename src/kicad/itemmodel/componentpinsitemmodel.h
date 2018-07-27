/**
 ** This file is part of the uConfig project.
 ** Copyright 2018 Robotips sebastien.caux@robotips.fr.
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

#ifndef COMPONENTPINSITEMMODEL_H
#define COMPONENTPINSITEMMODEL_H

#include "kicad_global.h"

#include <QAbstractItemModel>

#include "model/component.h"

class KICAD_EXPORT ComponentPinsItemModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    ComponentPinsItemModel(Component *component = Q_NULLPTR, QObject *parent = Q_NULLPTR);

    Component *component() const;
    void setComponent(Component *component);

    Pin *pin(const QModelIndex &index) const;
    QModelIndex index(Pin *pin) const;

    enum Column {
        PinNumber,
        PinName,
        PinElecType,
        PinStyle,
        PinClass,
        ColumnCount
    };

    virtual int columnCount(const QModelIndex &parent) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex &child) const;
    virtual int rowCount(const QModelIndex &parent) const;

    virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;

    void remove(const QModelIndex &index);

signals:
    void pinModified(Pin *pin);
    void pinRemoved(Pin *pin);

private:
    Component *_component;
};

#endif // COMPONENTPINSITEMMODEL_H
