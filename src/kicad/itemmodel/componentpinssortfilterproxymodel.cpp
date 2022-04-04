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

#include "componentpinssortfilterproxymodel.h"

#include "componentpinsitemmodel.h"

ComponentPinsSortFilterProxyModel::ComponentPinsSortFilterProxyModel()
{
}

bool ComponentPinsSortFilterProxyModel::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const
{
    if (source_left.internalPointer() == nullptr)
    {
        return (sortOrder() == Qt::DescendingOrder);
    }
    if (source_right.internalPointer() == nullptr)
    {
        return (sortOrder() == Qt::AscendingOrder);
    }

    if (source_left.column() != ComponentPinsItemModel::PinNumber)
    {
        return QSortFilterProxyModel::lessThan(source_left, source_right);
    }

    return NumericalSortFilterProxyModel::lessThan(source_left, source_right);
}
