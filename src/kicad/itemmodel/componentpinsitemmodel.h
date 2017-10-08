#ifndef COMPONENTPINSITEMMODEL_H
#define COMPONENTPINSITEMMODEL_H

#include "kicad_global.h"

#include <QAbstractItemModel>

#include "component.h"

class KICAD_EXPORT ComponentPinsItemModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    ComponentPinsItemModel(Component *component = Q_NULLPTR, QObject *parent = Q_NULLPTR);

    Component *component() const;
    void setComponent(Component *component);

    enum Column {
        PinNumber,
        PinName,
        ColumnCount
    };

    int columnCount(const QModelIndex &parent) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(const QModelIndex &index, int role) const;
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent) const;

private:
    Component *_component;
};

#endif // COMPONENTPINSITEMMODEL_H
