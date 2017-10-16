#ifndef NUMERICALSORTFILTERPROXYMODEL_H
#define NUMERICALSORTFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>

#include <QRegularExpression>

class NumericalSortFilterProxyModel : public QSortFilterProxyModel
{
public:
    NumericalSortFilterProxyModel();

    // QSortFilterProxyModel interface
protected:
    virtual bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
    virtual bool filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const;
    virtual bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const;

    QString toNumeric(const QString &str) const;

    QRegularExpression _numPattern;
};

#endif // NUMERICALSORTFILTERPROXYMODEL_H
