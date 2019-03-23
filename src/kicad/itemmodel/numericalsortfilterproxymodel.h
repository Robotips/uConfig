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

#ifndef NUMERICALSORTFILTERPROXYMODEL_H
#define NUMERICALSORTFILTERPROXYMODEL_H

#include "kicad_global.h"

#include <QSortFilterProxyModel>

#include <QRegularExpression>

class KICAD_EXPORT NumericalSortFilterProxyModel : public QSortFilterProxyModel
{
public:
    NumericalSortFilterProxyModel();

    // QSortFilterProxyModel interface
protected:
    virtual bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
    virtual bool filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const;
    virtual bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const;
    int compareColumn(const QModelIndex &source_left, const QModelIndex &source_right, const int column) const;

    QString toNumeric(const QString &str) const;

    QRegularExpression _numPattern;
};

#endif // NUMERICALSORTFILTERPROXYMODEL_H
