/**
 ** This file is part of the uConfig project.
 ** Copyright 2018 Robotips sebastien.caux@robotips.fr.
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

#include "componentpindelegate.h"

#include "componentpinsitemmodel.h"

#include <QDebug>
#include <QComboBox>

ComponentPinDelegate::ComponentPinDelegate(QObject *parent)
    : QItemDelegate(parent)
{
}

QWidget *ComponentPinDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    switch (index.column())
    {
    case ComponentPinsItemModel::PinElecType:
        {
            QComboBox *combobox = new QComboBox(parent);
            for (int i=0; i<=Pin::NotConnected; i++)
                combobox->addItem(Pin::electricalTypeDesc(static_cast<Pin::ElectricalType>(i)), i);
            return combobox;
        }
    case ComponentPinsItemModel::PinStyle:
        {
            QComboBox *combobox = new QComboBox(parent);
            for (int i=0; i<=Pin::NonLogic; i++)
                combobox->addItem(Pin::pinTypeDesc(static_cast<Pin::PinType>(i)), i);
            return combobox;
        }
    default:
        return QItemDelegate::createEditor(parent, option, index);
    }
}

void ComponentPinDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    switch (index.column())
    {
    case ComponentPinsItemModel::PinElecType:
    case ComponentPinsItemModel::PinStyle:
        {
            int value = index.model()->data(index, Qt::EditRole).toInt();
            QComboBox *combobox = static_cast<QComboBox*>(editor);
            int id = combobox->findData(value, Qt::UserRole);
            combobox->setCurrentIndex(id);
            combobox->showPopup();
            break;
        }
    default:
        QItemDelegate::setEditorData(editor, index);
    }
}

void ComponentPinDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    switch (index.column())
    {
    case ComponentPinsItemModel::PinElecType:
    case ComponentPinsItemModel::PinStyle:
        {
            QComboBox *combobox = static_cast<QComboBox*>(editor);
            int value = combobox->currentData(Qt::UserRole).toInt();
            model->setData(index, value, Qt::EditRole);
            break;
        }
    default:
        QItemDelegate::setModelData(editor, model, index);
    }
}

void ComponentPinDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    switch (index.column())
    {
    case ComponentPinsItemModel::PinElecType:
    case ComponentPinsItemModel::PinStyle:
        {
            QRect rect = option.rect;
            if (rect.width() < 150)
                rect.setWidth(150);
            editor->setGeometry(rect);
            break;
        }
    default:
        QItemDelegate::updateEditorGeometry(editor, option, index);
    }
}
