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

#ifndef COMPONENTPINSITEMMODEL_H
#define COMPONENTPINSITEMMODEL_H

#include <QAbstractItemModel>

#include "model/component.h"

class KICAD_EXPORT ComponentPinsItemModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    ComponentPinsItemModel(Component *component = nullptr, QObject *parent = nullptr);

    Component *component() const;
    void setComponent(Component *component);

    Pin *pin(const QModelIndex &index) const;
    QModelIndex index(Pin *pin) const;

    enum Column
    {
        PinNumber,
        PinName,
        PinElecType,
        PinStyle,
        PinClass,
        ColumnCount
    };

    int columnCount(const QModelIndex &parent) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    void remove(const QModelIndex &index);

signals:
    void pinModified(Pin *pin);
    void pinRemoved(Pin *pin);
    void pinInserted(Pin *pin);

protected:
    static QString toNumeric(const QString &str);

private:
    Component *_component;
    bool _isExpendable;

    QString _higherPin;
    void updateHigherPin();
};

#endif  // COMPONENTPINSITEMMODEL_H
