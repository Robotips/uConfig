#include "componentlibitemmodel.h"

ComponentLibItemModel::ComponentLibItemModel(Lib *lib, QObject *parent) :
    QAbstractItemModel(parent)
{
    if (lib)
        _lib = lib;
    else
        _lib = new Lib();
}

Lib *ComponentLibItemModel::lib() const
{
    return _lib;
}

void ComponentLibItemModel::setLib(Lib *lib)
{
    emit layoutAboutToBeChanged();
    _lib = lib;
    emit layoutChanged();
}

void ComponentLibItemModel::addComponent(Component *component)
{
    emit layoutAboutToBeChanged();
    _lib->addComponent(component);
    emit layoutChanged();
}

QList<Component *> ComponentLibItemModel::components() const
{
    return _lib->components();
}

Component *ComponentLibItemModel::component(const QModelIndex &index) const
{
    if (!index.isValid())
        return Q_NULLPTR;
    return static_cast<Component*>(index.internalPointer());
}

int ComponentLibItemModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return ColumnCount;
}

QVariant ComponentLibItemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation==Qt::Vertical)
        return QVariant();
    switch (role)
    {
    case Qt::DisplayRole:
        switch (section)
        {
        case Name:
            return QVariant("Name");
        case Package:
            return QVariant("Package");
        case PinCount:
            return QVariant("Pin count");
        }
        break;
    }
    return QVariant();
}

QVariant ComponentLibItemModel::data(const QModelIndex &index, int role) const
{
    Component *component = _lib->components().at(index.row());

    switch (role)
    {
    case Qt::DisplayRole:
        switch (index.column())
        {
        case Name:
            return QVariant(component->name());
        case Package:
            return QVariant(component->footPrints().join(';'));
        case PinCount:
            return QVariant(component->pins().count());
        }
    }
    return QVariant();
}

QModelIndex ComponentLibItemModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return createIndex(row, column, _lib->components()[row]);
}

QModelIndex ComponentLibItemModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child)
    return QModelIndex();
}

int ComponentLibItemModel::rowCount(const QModelIndex &parent) const
{
    if(!parent.isValid())
        return _lib->components().count();
    return 0;
}
