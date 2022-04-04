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

#include "componentpinsitemmodel.h"

#include "numericalsortfilterproxymodel.h"

#include <QBrush>
#include <QDebug>

ComponentPinsItemModel::ComponentPinsItemModel(Component *component, QObject *parent)
    : QAbstractItemModel(parent)
{
    setComponent(component);
    _isExpendable = true;
    _higherPin = QString();
}

Component *ComponentPinsItemModel::component() const
{
    return _component;
}

void ComponentPinsItemModel::setComponent(Component *component)
{
    emit layoutAboutToBeChanged();
    _component = component;

    updateHigherPin();

    emit layoutChanged();
}

Pin *ComponentPinsItemModel::pin(const QModelIndex &index) const
{
    if (!index.isValid())
    {
        return nullptr;
    }
    return static_cast<Pin *>(index.internalPointer());
}

QModelIndex ComponentPinsItemModel::index(Pin *pin) const
{
    QModelIndexList list = match(this->index(0, 0), ComponentPinsItemModel::PinNumber, pin->padName(), -1, Qt::MatchRecursive);
    if (list.isEmpty())
    {
        return QModelIndex();
    }
    return list.first();
}

int ComponentPinsItemModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return ColumnCount;
}

QVariant ComponentPinsItemModel::headerData(int section, Qt::Orientation orientation, int role) const
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
                case PinNumber:
                    return QVariant("#");
                case PinName:
                    return QVariant("Name");
                case PinElecType:
                    return QVariant("Elec type");
                case PinStyle:
                    return QVariant("Style");
                case PinClass:
                    return QVariant("Class");
            }
            break;
    }
    return QVariant();
}

QVariant ComponentPinsItemModel::data(const QModelIndex &index, int role) const
{
    if (_component == nullptr)
    {
        return QVariant();
    }

    Pin *pin = nullptr;
    if (index.row() < _component->pins().count())
    {
        pin = _component->pins().at(index.row());
    }

    if (pin == nullptr)
    {
        if ((role == Qt::DisplayRole || role == Qt::EditRole) && index.column() == PinNumber)
        {
            return QVariant(_higherPin);
        }
        if (role == Qt::ForegroundRole)
        {
            return QVariant(QBrush(Qt::gray));
        }
        return QVariant();
    }

    switch (role)
    {
        case Qt::DisplayRole:
            switch (index.column())
            {
                case PinNumber:
                    return QVariant(pin->padName());
                case PinName:
                    return QVariant(pin->name());
                case PinElecType:
                    return QVariant(Pin::electricalTypeDesc(pin->electricalType()));
                case PinStyle:
                    return QVariant(Pin::pinTypeDesc(pin->pinType()));
                case PinClass:
                    return QVariant(pin->className());
            }
            break;
        case Qt::EditRole:
            switch (index.column())
            {
                case PinNumber:
                    return QVariant(pin->padName());
                case PinName:
                    return QVariant(pin->name());
                case PinElecType:
                    return QVariant(pin->electricalType());
                case PinStyle:
                    return QVariant(pin->pinType());
                case PinClass:
                    return QVariant();
            }
            break;
    }
    return QVariant();
}

QModelIndex ComponentPinsItemModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    if (_component == nullptr)
    {
        return QModelIndex();
    }

    if (row >= _component->pins().count())
    {
        return createIndex(row, column, nullptr);
    }
    return createIndex(row, column, _component->pins()[row]);
}

QModelIndex ComponentPinsItemModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child)
    return QModelIndex();
}

int ComponentPinsItemModel::rowCount(const QModelIndex &parent) const
{
    if (_component == nullptr)
    {
        return 0;
    }

    if (!parent.isValid())
    {
        if (_isExpendable)
        {
            return _component->pins().count() + 1;
        }
        return _component->pins().count();
    }
    return 0;
}

bool ComponentPinsItemModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (_component == nullptr)
    {
        return false;
    }

    Pin *pin;
    if (index.row() >= _component->pins().count())
    {
        if (index.column() != PinNumber && index.column() != PinName)
        {
            return false;
        }
        if (value.isNull() || !value.isValid() || value.toString() == QString())
        {
            return false;
        }

        // new pin
        pin = new Pin();
        if (index.column() == PinNumber)
        {
            pin->setPadName(value.toString());
        }
        else if (index.column() == PinName)
        {
            pin->setPadName(_higherPin);
            pin->setName(value.toString());
        }
        else
        {
            return false;
        }
        beginInsertRows(QModelIndex(), _component->pins().count(), _component->pins().count());
        _component->addPin(pin);
        endInsertRows();
        updateHigherPin();
        emit pinInserted(pin);
        return true;
    }
    pin = _component->pins().at(index.row());

    switch (role)
    {
        case Qt::DisplayRole:
        case Qt::EditRole:
            switch (index.column())
            {
                case PinNumber:
                    pin->setPadName(value.toString());
                    updateHigherPin();
                    break;
                case PinName:
                    pin->setName(value.toString());
                    break;
                case PinElecType:
                    pin->setElectricalType(static_cast<Pin::ElectricalType>(value.toInt()));
                    break;
                case PinStyle:
                    pin->setPinType(static_cast<Pin::PinType>(value.toInt()));
                    break;
                case PinClass:
                    return false;
            }
            emit pinModified(pin);
            return true;
    }
    return false;
}

Qt::ItemFlags ComponentPinsItemModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags baseFlags = Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    if (index.column() == PinNumber || index.column() == PinName || index.column() == PinElecType || index.column() == PinStyle)
    {
        return baseFlags | Qt::ItemIsEditable;
    }
    return baseFlags;
}

void ComponentPinsItemModel::remove(const QModelIndex &index)
{
    Pin *mpin = pin(index);
    if (mpin == nullptr)
    {
        return;
    }

    emit layoutAboutToBeChanged();
    emit pinRemoved(mpin);
    _component->removePin(mpin);
    updateHigherPin();
    emit layoutChanged();
}

QString ComponentPinsItemModel::toNumeric(const QString &str)
{
    QString sortPatern = str;

    QRegularExpression numPattern("([^0-9]*)([0-9]+)([^0-9]*)", QRegularExpression::CaseInsensitiveOption);

    QRegularExpressionMatchIterator numMatchIt = numPattern.globalMatch(str);
    if (numMatchIt.hasNext())
    {
        sortPatern.clear();
    }
    while (numMatchIt.hasNext())
    {
        QRegularExpressionMatch numMatch = numMatchIt.next();
        sortPatern.append(numMatch.captured(1) + QString('0').repeated(5 - numMatch.captured(2).size()) + numMatch.captured(2) + numMatch.captured(3));
    }

    return sortPatern;
}

void ComponentPinsItemModel::updateHigherPin()
{
    _higherPin = QString();
    QString higherNumPin = QString();
    if (_component != nullptr)
    {
        for (Pin *pin : _component->pins())
        {
            QString numPin = toNumeric(pin->padName());
            if (numPin > higherNumPin)
            {
                _higherPin = pin->padName();
                higherNumPin = numPin;
            }
        }
        QRegularExpression higherNumPinPattern("([A-Z]*0*)([1-9][0-9]*)", QRegularExpression::CaseInsensitiveOption);
        QRegularExpressionMatchIterator higherNumPinMatchIt = higherNumPinPattern.globalMatch(_higherPin);
        if (higherNumPinMatchIt.hasNext())
        {
            QRegularExpressionMatch numMatch = higherNumPinMatchIt.next();
            _higherPin = numMatch.captured(1) + QString::number(numMatch.captured(2).toInt() + 1);
        }
    }
}
