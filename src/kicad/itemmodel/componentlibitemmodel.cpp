#include "componentlibitemmodel.h"

#include <QDebug>

ComponentLibItemModel::ComponentLibItemModel(Lib *lib, QObject *parent) :
    QAbstractItemModel(parent)
{
    if (lib)
        _lib = lib;
    else
        _lib = new Lib();
    _selectedMode = false;
}

Lib *ComponentLibItemModel::lib() const
{
    return _lib;
}

void ComponentLibItemModel::setLib(Lib *lib)
{
    emit layoutAboutToBeChanged();
    beginResetModel();
    resetInternalData();
    //delete _lib;
    _lib = lib;
    endResetModel();
    emit layoutChanged();
}

void ComponentLibItemModel::addComponent(Component *component)
{
    emit layoutAboutToBeChanged();
    beginResetModel();
    resetInternalData();
    _lib->addComponent(component);
    endResetModel();
    emit layoutChanged();
}

QList<Component *> ComponentLibItemModel::components() const
{
    if (!_lib)
        return QList<Component *>();
    return _lib->components();
}

Component *ComponentLibItemModel::component(const QModelIndex &index) const
{
    if (!_lib)
        return Q_NULLPTR;
    if (!index.isValid())
        return Q_NULLPTR;
    return static_cast<Component*>(index.internalPointer());
}

void ComponentLibItemModel::remove(const QModelIndex &index)
{
    Component *mcomponent = component(index);
    if (!mcomponent)
        return;

    emit layoutAboutToBeChanged();
    _lib->removeComponent(mcomponent);
    emit layoutChanged();
}

int ComponentLibItemModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return ColumnCount;
}

QVariant ComponentLibItemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation==Qt::Vertical)
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
    if (!_lib)
        return QVariant();
    if (!index.isValid())
        return QVariant();

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
    case Qt::CheckStateRole:
        if (index.column() == Name && _selectedMode)
        {
            if (_selectedComponents.contains(component))
                return Qt::Checked;
            else
                return Qt::Unchecked;
        }
        break;
    }
    return QVariant();
}

bool ComponentLibItemModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!_lib)
        return false;
    if (!index.isValid())
        return false;

    Component *component = _lib->components().at(index.row());
    if (role == Qt::CheckStateRole)
    {
        if (value == Qt::Checked)
        {
            if (!_selectedComponents.contains(component))
                _selectedComponents.append(component);
        }
        else
            _selectedComponents.removeOne(component);
        return true;
    }
    return QAbstractItemModel::setData(index, value, role);
}

QModelIndex ComponentLibItemModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    if (!_lib)
        return QModelIndex();
    if (row >= _lib->components().count())
        return QModelIndex();
    return createIndex(row, column, _lib->components()[row]);
}

QModelIndex ComponentLibItemModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child)
    return QModelIndex();
}

int ComponentLibItemModel::rowCount(const QModelIndex &parent) const
{
    if (!_lib)
        return 0;
    if (!parent.isValid())
        return _lib->components().count();
    return 0;
}

Qt::ItemFlags ComponentLibItemModel::flags(const QModelIndex &index) const
{
    if (index.column() == 0 && _selectedMode)
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable;
    else
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

void ComponentLibItemModel::selectAll(bool selected)
{
    emit layoutAboutToBeChanged();
    if (!_lib)
        return;
    if (selected)
        _selectedComponents = _lib->components();
    else
        _selectedComponents.clear();
    emit layoutChanged();
}

bool ComponentLibItemModel::selectedMode() const
{
    return _selectedMode;
}

void ComponentLibItemModel::setSelectedMode(bool selectedMode)
{
    _selectedMode = selectedMode;
}

const QList<Component *> &ComponentLibItemModel::selectedComponents() const
{
    return _selectedComponents;
}
