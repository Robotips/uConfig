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

#include "componentpindelegate.h"

#include "componentpinsitemmodel.h"

#include <QComboBox>
#include <QDebug>
#include <QPainter>

ComponentPinDelegate::ComponentPinDelegate(QObject *parent)
    : QItemDelegate(parent)
{
}

QRegularExpression ComponentPinDelegate::searchPattern() const
{
    return _searchPattern;
}

void ComponentPinDelegate::setSearchPattern(const QRegularExpression &searchPattern)
{
    _searchPattern = searchPattern;
}

QWidget *ComponentPinDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    switch (index.column())
    {
        case ComponentPinsItemModel::PinElecType:
        {
            QComboBox *combobox = new QComboBox(parent);
            for (int i = 0; i <= Pin::NotConnected; i++)
            {
                combobox->addItem(Pin::electricalTypeDesc(static_cast<Pin::ElectricalType>(i)), i);
            }
            return combobox;
        }
        case ComponentPinsItemModel::PinStyle:
        {
            QComboBox *combobox = new QComboBox(parent);
            for (int i = 0; i <= Pin::NonLogic; i++)
            {
                combobox->addItem(Pin::pinTypeDesc(static_cast<Pin::PinType>(i)), i);
            }
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
            QComboBox *combobox = dynamic_cast<QComboBox *>(editor);
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
            QComboBox *combobox = dynamic_cast<QComboBox *>(editor);
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
            {
                rect.setWidth(150);
            }
            editor->setGeometry(rect);
            break;
        }
        default:
            QItemDelegate::updateEditorGeometry(editor, option, index);
    }
}

void ComponentPinDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.column() != ComponentPinsItemModel::PinName)
    {
        QItemDelegate::paint(painter, option, index);
        return;
    }

    drawBackground(painter, option, index);
    drawFocus(painter, option, option.rect);

    // compute colors and margins
    QPalette::ColorGroup cg = ((option.state & QStyle::State_Enabled) != 0u) ? QPalette::Normal : QPalette::Disabled;
    if (cg == QPalette::Normal && !(option.state & QStyle::State_Active))
    {
        cg = QPalette::Inactive;
    }
    if (option.state.testFlag(QStyle::State_Selected) /*&& option.widget->hasFocus()*/)
    {
        painter->setPen(option.palette.color(cg, QPalette::HighlightedText));
    }
    else
    {
        painter->setPen(option.palette.color(cg, QPalette::Text));
    }
    int margin = option.widget->style()->pixelMetric(QStyle::PM_FocusFrameHMargin, nullptr, option.widget) + 1;

    // draw text
    QRect textRect = option.rect.adjusted(margin, 0, -margin, 0);
    QString text = index.model()->data(index).toString();
    if (!_searchPattern.isValid() || _searchPattern.pattern().isEmpty())
    {
        painter->drawText(textRect, static_cast<int>(option.displayAlignment), text);
    }
    else
    {
        int start = 0;
        QFont bold = option.font;
        bold.setBold(true);

        QRegularExpressionMatch match = _searchPattern.match(text, start);
        while (match.hasMatch())
        {
            QString pre = text.mid(start, match.capturedStart() - start);
            painter->setFont(option.font);
            painter->drawText(textRect, static_cast<int>(option.displayAlignment), pre);
#if QT_VERSION < QT_VERSION_CHECK(5, 11, 0)
            textRect.adjust(painter->fontMetrics().width(pre), 0, 0, 0);
#else
            textRect.adjust(painter->fontMetrics().horizontalAdvance(pre), 0, 0, 0);
#endif

            QString part = match.captured();
            painter->setFont(bold);
            painter->drawText(textRect, static_cast<int>(option.displayAlignment), part);
#if QT_VERSION < QT_VERSION_CHECK(5, 11, 0)
            textRect.adjust(painter->fontMetrics().width(part), 0, 0, 0);
#else
            textRect.adjust(painter->fontMetrics().horizontalAdvance(part), 0, 0, 0);
#endif

            start = match.capturedEnd();
            match = _searchPattern.match(text, start);
        }
        QString post = text.mid(start);
        painter->setFont(option.font);
        painter->drawText(textRect, static_cast<int>(option.displayAlignment), post);
    }
}
