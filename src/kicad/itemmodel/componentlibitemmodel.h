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

    Component *component(const QModelIndex &index) const;

    void selectAll(bool selected=true);
    bool getSelectedMode() const;
    void setSelectedMode(bool selectedMode);
    const QList<Component *> &selectedComponents() const;

    enum Column {
        Name,
        Package,
        PinCount,
        ColumnCount
    };

    // QAbstractItemModel interface
public:
    int columnCount(const QModelIndex &parent) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

private:
    Lib *_lib;
    QList<Component *> _selectedComponents;
    bool _selectedMode;
};

#endif // COMPONENTLIBITEMMODEL_H
