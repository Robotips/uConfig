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

#include "numericalsortfilterproxymodel.h"

NumericalSortFilterProxyModel::NumericalSortFilterProxyModel()
    : _numPattern("([^0-9]*)([0-9]+)([^0-9]*)", QRegularExpression::CaseInsensitiveOption)
{
}

bool NumericalSortFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    return QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
}

bool NumericalSortFilterProxyModel::filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const
{
    Q_UNUSED(source_column)
    Q_UNUSED(source_parent)
    return true;
}

bool NumericalSortFilterProxyModel::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const
{
    int compare = compareColumn(source_left, source_right, source_left.column());
    if (compare != 0)
    {
        return (compare < 0);
    }

    /*for (int column = 0; column < columnCount(); column++)
    {
        if (column == source_left.column())
            continue;
        compare = compareColumn(source_left, source_right, column);
        if (compare != 0)
            return (compare < 0);
    }*/

    return source_left.internalId() < source_right.internalId();
}

int NumericalSortFilterProxyModel::compareColumn(const QModelIndex &source_left, const QModelIndex &source_right, const int column) const
{
    QString leftStr = sourceModel()->data(sourceModel()->index(source_left.row(), column, source_left.parent()), Qt::DisplayRole).toString();
    QString rightStr = sourceModel()->data(sourceModel()->index(source_right.row(), column, source_right.parent()), Qt::DisplayRole).toString();

    QString leftSortPatern = toNumeric(leftStr);
    QString rightSortPatern = toNumeric(rightStr);

    return leftSortPatern.compare(rightSortPatern, Qt::CaseInsensitive);
}

QString NumericalSortFilterProxyModel::toNumeric(const QString &str) const
{
    QString sortPatern = str;

    QRegularExpressionMatchIterator numMatchIt = _numPattern.globalMatch(str);
    if (numMatchIt.hasNext())
    {
        sortPatern.clear();
    }
    while (numMatchIt.hasNext())
    {
        QRegularExpressionMatch numMatch = numMatchIt.next();
        sortPatern.append(numMatch.captured(1) + QString('0').repeated(5 - numMatch.captured(2).size()) + numMatch.captured(2) + numMatch.captured(3));
    }

    return sortPatern;
}
