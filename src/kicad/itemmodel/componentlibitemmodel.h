#ifndef COMPONENTLIBITEMMODEL_H
#define COMPONENTLIBITEMMODEL_H

#include "kicad_global.h"

#include <QAbstractItemModel>

#include "lib.h"

class KICAD_EXPORT ComponentLibItemModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    ComponentLibItemModel(Lib *lib = Q_NULLPTR, QObject *parent = Q_NULLPTR);

    Lib *lib() const;
    void setLib(Lib *lib);

    void addComponent(Component *component);
    QList<Component *> components() const;

    enum Column {
        Name,
        Package,
        PinCount,
        ColumnCount
    };

    int columnCount(const QModelIndex &parent) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(const QModelIndex &index, int role) const;
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent) const;

private:
    Lib *_lib;
};

#endif // COMPONENTLIBITEMMODEL_H
