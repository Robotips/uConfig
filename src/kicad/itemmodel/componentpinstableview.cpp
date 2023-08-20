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

#include "componentpinstableview.h"

#include <QApplication>
#include <QClipboard>
#include <QMenu>
#include <QMessageBox>
#include <QMouseEvent>

ComponentPinsTableView::ComponentPinsTableView(Component *component, QWidget *parent)
    : QTableView(parent)
{
    _model = new ComponentPinsItemModel(component);

    _sortProxy = new ComponentPinsSortFilterProxyModel();
    _sortProxy->setSourceModel(_model);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setModel(_sortProxy);
    connect(selectionModel(), &QItemSelectionModel::selectionChanged, this, &ComponentPinsTableView::updateSelect);

    sortByColumn(0, Qt::AscendingOrder);
    setSortingEnabled(true);

    _sortProxy->setFilterRole(Qt::DisplayRole);
    _sortProxy->setFilterKeyColumn(ComponentPinsItemModel::PinName);

    _delegate = new ComponentPinDelegate(this);
    setItemDelegate(_delegate);

    createActions();
}

Component *ComponentPinsTableView::component() const
{
    return _model->component();
}

void ComponentPinsTableView::setComponent(Component *component)
{
    _model->setComponent(component);
    resizeColumnsToContents();
}

void ComponentPinsTableView::selectPin(Pin *pin)
{
    if (pin == nullptr)
    {
        selectionModel()->clearSelection();
        return;
    }
    const QPersistentModelIndex index = _model->index(pin);
    if (!index.isValid())
    {
        return;
    }
    const QModelIndex &indexPin = _sortProxy->mapFromSource(index);
    if (!indexPin.isValid())
    {
        return;
    }
    selectionModel()->select(indexPin, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
    scrollTo(indexPin);
}

void ComponentPinsTableView::selectPins(QList<Pin *> pins)
{
    if (pins.isEmpty())
    {
        selectionModel()->clearSelection();
        return;
    }
    if (pins.count() == 1)
    {
        selectPin(pins.first());
        return;
    }
    for (Pin *pin : pins)
    {
        const QPersistentModelIndex index = _model->index(pin);
        if (!index.isValid())
        {
            return;
        }
        const QModelIndex &indexPin = _sortProxy->mapFromSource(index);
        if (!indexPin.isValid())
        {
            return;
        }
        selectionModel()->select(indexPin, QItemSelectionModel::Select | QItemSelectionModel::Rows);
    }
}

void ComponentPinsTableView::setPinFilter(const QString &filter)
{
    _sortProxy->setFilterRegularExpression(QRegularExpression(filter, QRegularExpression::CaseInsensitiveOption));
    _delegate->setSearchPattern(QRegularExpression(filter, QRegularExpression::CaseInsensitiveOption));

    viewport()->update();
}

void ComponentPinsTableView::remove()
{
    QModelIndexList selection = selectionModel()->selectedIndexes();
    if (selection.isEmpty())
    {
        return;
    }

    if (!selection.empty())
    {
        QList<QPersistentModelIndex> pindex;
        for (QModelIndex selected : selection)
        {
            const QModelIndex &indexComponent = _sortProxy->mapToSource(selected);
            if (!indexComponent.isValid())
            {
                continue;
            }

            pindex.append(indexComponent);
        }
        if (QMessageBox::question(
                this, tr("Remove pins?"), tr("Do you realy want to remove theses %1 pins?").arg(pindex.count() / ComponentPinsItemModel::ColumnCount))
            != QMessageBox::Yes)
        {
            return;
        }
        selectionModel()->clearSelection();
        for (const QPersistentModelIndex &index : pindex)
        {
            _model->remove(index);
        }
    }
}

void ComponentPinsTableView::copy()
{
    QString text;
    QItemSelectionRange range = selectionModel()->selection().first();
    for (int i = range.top(); i <= range.bottom(); ++i)
    {
        QStringList rowContents;
        for (int j = range.left(); j <= range.right(); ++j)
        {
            const QModelIndex &indexComponent = _sortProxy->mapToSource(_sortProxy->index(i, j));
            rowContents << indexComponent.data().toString();
        }
        text += rowContents.join("\t");
        text += "\n";
    }
    QApplication::clipboard()->setText(text);
}

void ComponentPinsTableView::updateSelect(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(selected)
    Q_UNUSED(deselected)

    QSet<Pin *> selectedPins;
    for (const QModelIndex &index : selectionModel()->selectedIndexes())
    {
        if (!index.isValid())
        {
            continue;
        }
        const QModelIndex &indexComponent = _sortProxy->mapToSource(index);
        if (!indexComponent.isValid())
        {
            continue;
        }

        selectedPins.insert(_model->pin(indexComponent));
    }
    _removeAction->setEnabled(!selectedPins.isEmpty());
    _copyAction->setEnabled(!selectedPins.isEmpty());

    emit pinSelected(selectedPins.values());
}

void ComponentPinsTableView::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu;
    menu.addAction(_removeAction);
    menu.addAction(_copyAction);
    menu.exec(event->globalPos());
}

void ComponentPinsTableView::createActions()
{
    _removeAction = new QAction(this);
    _removeAction->setText(tr("Remove"));
    _removeAction->setShortcut(QKeySequence::Delete);
    _removeAction->setShortcutContext(Qt::WidgetShortcut);
    _removeAction->setEnabled(false);
    connect(_removeAction, &QAction::triggered, this, &ComponentPinsTableView::remove);
    addAction(_removeAction);

    _copyAction = new QAction(this);
    _copyAction->setText(tr("Copy"));
    _copyAction->setShortcut(QKeySequence::Copy);
    _copyAction->setShortcutContext(Qt::WidgetShortcut);
    _copyAction->setEnabled(false);
    connect(_copyAction, &QAction::triggered, this, &ComponentPinsTableView::copy);
    addAction(_copyAction);
}

ComponentPinsItemModel *ComponentPinsTableView::model() const
{
    return _model;
}
