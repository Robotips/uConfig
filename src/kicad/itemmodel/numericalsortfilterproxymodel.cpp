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
    QString leftSortPatern = toNumeric(sourceModel()->data(source_left, Qt::DisplayRole).toString());
    QString rightSortPatern = toNumeric(sourceModel()->data(source_right, Qt::DisplayRole).toString());

    return (leftSortPatern.compare(rightSortPatern, Qt::CaseInsensitive) < 0);
}

QString NumericalSortFilterProxyModel::toNumeric(const QString &str) const
{
    QString sortPatern = str;

    QRegularExpressionMatchIterator numMatchIt = _numPattern.globalMatch(str);
    if (numMatchIt.hasNext())
        sortPatern.clear();
    while (numMatchIt.hasNext())
    {
        QRegularExpressionMatch numMatch = numMatchIt.next();
        sortPatern.append(numMatch.captured(1) + QString('0').repeated(5-numMatch.captured(2).size()) + numMatch.captured(2) + numMatch.captured(3));
    }

    return sortPatern;
}
