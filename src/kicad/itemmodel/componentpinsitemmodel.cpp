#include "componentpinsitemmodel.h"

ComponentPinsItemModel::ComponentPinsItemModel(Component *component, QObject *parent) :
    QAbstractItemModel(parent)
{
    _component = component;
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

Pin *ComponentPinsItemModel::pin(const QModelIndex &index) const
{
    if (!index.isValid())
        return Q_NULLPTR;
    return static_cast<Pin*>(index.internalPointer());
}

QModelIndex ComponentPinsItemModel::index(Pin *pin) const
{
    QModelIndexList list = match(this->index(0, 0), ComponentPinsItemModel::PinNumber, pin->padName(), -1, Qt::MatchRecursive);
    if (list.isEmpty())
        return QModelIndex();
    return list.first();
}

int ComponentPinsItemModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return ColumnCount;
}

QVariant ComponentPinsItemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation==Qt::Vertical)
        return QVariant();
    switch (role)
    {
    case Qt::DisplayRole:
        switch (section)
        {
        case PinNumber:
            return QVariant("#");
        case PinName:
            return QVariant("Pin name");
        }
        break;
    }
    return QVariant();
}

QVariant ComponentPinsItemModel::data(const QModelIndex &index, int role) const
{
    if (!_component)
        return QVariant();

    Pin *pin = _component->pins().at(index.row());

    switch (role)
    {
    case Qt::DisplayRole:
        switch (index.column())
        {
        case PinNumber:
            return QVariant(pin->padName());
        case PinName:
            return QVariant(pin->name());
        }
    }
    return QVariant();
}

QModelIndex ComponentPinsItemModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!_component)
        return QModelIndex();
    Q_UNUSED(parent)
    return createIndex(row, column, _component->pins()[row]);
}

QModelIndex ComponentPinsItemModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child)
    return QModelIndex();
}

int ComponentPinsItemModel::rowCount(const QModelIndex &parent) const
{
    if (!_component)
        return 0;
    if (!parent.isValid())
        return _component->pins().count();
    return 0;
}
