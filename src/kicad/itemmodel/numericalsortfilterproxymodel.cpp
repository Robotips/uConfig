#include "numericalsortfilterproxymodel.h"

#include <QDebug>

NumericalSortFilterProxyModel::NumericalSortFilterProxyModel()
    : _numPattern("([^0-9]*)([0-9]+)([^0-9]*)", QRegularExpression::CaseInsensitiveOption)
{
}

bool NumericalSortFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    Q_UNUSED(source_row)
    Q_UNUSED(source_parent)
    return true;
}

bool NumericalSortFilterProxyModel::filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const
{
    Q_UNUSED(source_column)
    Q_UNUSED(source_parent)
    return true;
}

bool NumericalSortFilterProxyModel::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const
{
    QString leftSortPatern = toNumeric(sourceModel()->data(source_left, filterKeyColumn()).toString());
    QString rightSortPatern = toNumeric(sourceModel()->data(source_right, filterKeyColumn()).toString());

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
