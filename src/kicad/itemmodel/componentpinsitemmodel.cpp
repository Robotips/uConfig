#include "componentpinsitemmodel.h"

ComponentPinsItemModel::ComponentPinsItemModel(Component *component, QObject *parent) :
    QAbstractItemModel(parent)
{
    _component = component;

    if (component)
        _component = component;
    else
        _component = new Component();
}

Component *ComponentPinsItemModel::component() const
{
    return _component;
}

void ComponentPinsItemModel::setComponent(Component *component)
{
    emit layoutAboutToBeChanged();
    _component = component;
    emit layoutChanged();
}

int ComponentPinsItemModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return ColumnCount;
}

QVariant ComponentPinsItemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation==Qt::Vertical)
        return QVariant();
    switch (role)
    {
    case Qt::DisplayRole:
        switch (section)
        {
        case PinNumber:
            return QVariant("Pin number");
        case PinName:
            return QVariant("Pin name");
        }
        break;
    }
    return QVariant();
}

QVariant ComponentPinsItemModel::data(const QModelIndex &index, int role) const
{
    Pin *pin = _component->pins().at(index.row());

    switch (role)
    {
    case Qt::DisplayRole:
        switch (index.column())
        {
        case PinNumber:
            return QVariant(pin->padname());
        case PinName:
            return QVariant(pin->name());
        }
    }
    return QVariant();
}

QModelIndex ComponentPinsItemModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return createIndex(row, column);
}

QModelIndex ComponentPinsItemModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child)
    return QModelIndex();
}

int ComponentPinsItemModel::rowCount(const QModelIndex &parent) const
{
    if(!parent.isValid())
        return _component->pins().count();
    return 0;
}
