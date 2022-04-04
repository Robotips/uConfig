/**
 ** This file is part of the uConfig project.
 ** Copyright 2017-2020 Robotips, Sebastien CAUX (sebcaux)
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

#include "componentlibitemmodel.h"

#include <QDebug>
#include <QFont>

ComponentLibItemModel::ComponentLibItemModel(Lib *lib, QObject *parent)
    : QAbstractItemModel(parent)
{
    if (lib != nullptr)
    {
        _lib = lib;
    }
    else
    {
        _lib = new Lib();
    }
    _selectedMode = false;
    _activeComponent = nullptr;
}

Lib *ComponentLibItemModel::lib() const
{
    return _lib;
}

void ComponentLibItemModel::setLib(Lib *lib)
{
    emit layoutAboutToBeChanged();
    _activeComponent = nullptr;
    beginResetModel();
    resetInternalData();
    // delete _lib;
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
    if (_lib == nullptr)
    {
        return QList<Component *>();
    }
    return _lib->components();
}

QModelIndex ComponentLibItemModel::index(Component *component, int coulumn)
{
    if (component == nullptr)
    {
        return QModelIndex();
    }

    for (int i = 0; i < _lib->componentsCount(); i++)
    {
        if (_lib->component(i) == component)
        {
            return index(i, coulumn, QModelIndex());
        }
    }
    return QModelIndex();
}

Component *ComponentLibItemModel::component(const QModelIndex &index) const
{
    if (_lib == nullptr)
    {
        return nullptr;
    }
    if (!index.isValid())
    {
        return nullptr;
    }
    return static_cast<Component *>(index.internalPointer());
}

void ComponentLibItemModel::remove(const QModelIndex &index)
{
    Component *mcomponent = component(index);
    if (mcomponent == nullptr)
    {
        return;
    }

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
    if (orientation == Qt::Vertical)
    {
        return QVariant();
    }
    switch (role)
    {
        case Qt::DisplayRole:
            switch (section)
            {
                case Name:
                    return QVariant("Name");
                case PinCount:
                    return QVariant("Pins");
                case UnitCount:
                    return QVariant("Units");
                case Package:
                    return QVariant("Package(s)");
            }
            break;
    }
    return QVariant();
}

QVariant ComponentLibItemModel::data(const QModelIndex &index, int role) const
{
    if (_lib == nullptr)
    {
        return QVariant();
    }
    if (!index.isValid())
    {
        return QVariant();
    }

    Component *component = _lib->components().at(index.row());

    switch (role)
    {
        case Qt::DisplayRole:
            switch (index.column())
            {
                case Name:
                    return QVariant(component->name());
                case PinCount:
                    return QVariant(component->pins().count());
                case UnitCount:
                    return QVariant(component->unitCount());
                case Package:
                    return QVariant(component->footPrints().join(';'));
                default:
                    return QVariant();
            }
        case Qt::CheckStateRole:
            if (index.column() == Name && _selectedMode)
            {
                if (_selectedComponents.contains(component))
                {
                    return Qt::Checked;
                }
                return Qt::Unchecked;
            }
            break;
        case Qt::FontRole:
            if (component == _activeComponent)
            {
                QFont font;
                font.setBold(true);
                return font;
            }
            break;
    }
    return QVariant();
}

bool ComponentLibItemModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (_lib == nullptr)
    {
        return false;
    }
    if (!index.isValid())
    {
        return false;
    }

    Component *component = _lib->components().at(index.row());
    if (role == Qt::CheckStateRole)
    {
        if (value == Qt::Checked)
        {
            if (!_selectedComponents.contains(component))
            {
                _selectedComponents.append(component);
            }
        }
        else
        {
            _selectedComponents.removeOne(component);
        }
        return true;
    }
    return QAbstractItemModel::setData(index, value, role);
}

QModelIndex ComponentLibItemModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    if (_lib == nullptr)
    {
        return QModelIndex();
    }
    if (row >= _lib->components().count())
    {
        return QModelIndex();
    }
    return createIndex(row, column, _lib->components()[row]);
}

QModelIndex ComponentLibItemModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child)
    return QModelIndex();
}

int ComponentLibItemModel::rowCount(const QModelIndex &parent) const
{
    if (_lib == nullptr)
    {
        return 0;
    }
    if (!parent.isValid())
    {
        return _lib->components().count();
    }
    return 0;
}

Qt::ItemFlags ComponentLibItemModel::flags(const QModelIndex &index) const
{
    if (index.column() == 0 && _selectedMode)
    {
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable;
    }
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

Component *ComponentLibItemModel::activeComponent() const
{
    return _activeComponent;
}

void ComponentLibItemModel::setActiveComponent(Component *activeComponent)
{
    QModelIndex oldIndex = index(_activeComponent);
    QModelIndex oldIndex2 = index(_activeComponent, 2);
    if (oldIndex.isValid())
    {
        emit dataChanged(oldIndex, oldIndex2);
    }

    _activeComponent = activeComponent;

    QModelIndex newIndex = index(_activeComponent);
    QModelIndex newIndex2 = index(_activeComponent, 2);
    if (newIndex.isValid())
    {
        emit dataChanged(newIndex, newIndex2);
    }
}

void ComponentLibItemModel::selectAll(bool selected)
{
    emit layoutAboutToBeChanged();
    if (_lib == nullptr)
    {
        return;
    }
    if (selected)
    {
        _selectedComponents = _lib->components();
    }
    else
    {
        _selectedComponents.clear();
    }
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

void ComponentLibItemModel::updateComponent(Component *component)
{
    if (component != nullptr)
    {
        emit dataChanged(index(component, 0), index(component, ColumnCount - 1));
    }
}
