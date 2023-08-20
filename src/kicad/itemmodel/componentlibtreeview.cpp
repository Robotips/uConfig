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

#include "componentlibtreeview.h"

#include <QDebug>
#include <QMenu>
#include <QMessageBox>
#include <QMouseEvent>

ComponentLibTreeView::ComponentLibTreeView(Lib *lib, QWidget *parent)
    : QTreeView(parent)
{
    if (lib != nullptr)
    {
        _model = new ComponentLibItemModel(lib);
    }
    else
    {
        _model = new ComponentLibItemModel(new Lib());
    }

    setSelectionMode(QAbstractItemView::ExtendedSelection);
    _editMode = false;

    _sortProxy = new NumericalSortFilterProxyModel();
    _sortProxy->setSourceModel(_model);
    setModel(_sortProxy);

    sortByColumn(0, Qt::AscendingOrder);

    setSortingEnabled(true);
    createActions();
}

Lib *ComponentLibTreeView::lib() const
{
    return _model->lib();
}

void ComponentLibTreeView::setLib(Lib *lib)
{
    _model->setLib(lib);
    if (lib == _model->lib())
    {
        return;
    }
    resizeColumnToContents(0);
    resizeColumnToContents(1);
}

void ComponentLibTreeView::setActiveComponent(Component *component)
{
    _model->setActiveComponent(component);
}

void ComponentLibTreeView::addComponent(Component *component)
{
    _model->addComponent(component);
    resizeColumnToContents(0);
    resizeColumnToContents(1);
}

QList<Component *> ComponentLibTreeView::components() const
{
    return _model->components();
}

bool ComponentLibTreeView::selectedMode() const
{
    return _model->selectedMode();
}

void ComponentLibTreeView::setSelectedMode(bool selectedMode)
{
    _model->setSelectedMode(selectedMode);
}

const QList<Component *> &ComponentLibTreeView::selectedComponents() const
{
    return _model->selectedComponents();
}

void ComponentLibTreeView::selectAll()
{
    _model->selectAll(true);
}

void ComponentLibTreeView::updateComponent(Component *component)
{
    _model->updateComponent(component);
}

void ComponentLibTreeView::remove()
{
    if (!_editMode)
    {
        return;
    }

    QModelIndexList selection = selectionModel()->selectedIndexes();
    if (selection.isEmpty())
    {
        return;
    }

    if (!selection.empty())
    {
        if (QMessageBox::question(this,
                                  tr("Remove components?"),
                                  tr("Do you realy want to remove theses %1 components?").arg(selection.size() / ComponentLibItemModel::ColumnCount))
            != QMessageBox::Yes)
        {
            return;
        }
        QList<QPersistentModelIndex> pindex;
        for (QModelIndex selected : selection)
        {
            const QModelIndex &indexComponent = _sortProxy->mapToSource(selected);
            if (!indexComponent.isValid() || indexComponent.column() != 0)
            {
                continue;
            }

            pindex.append(indexComponent);
        }
        selectionModel()->clearSelection();
        emit openedComponent(nullptr);
        for (const QPersistentModelIndex &index : pindex)
        {
            _model->remove(index);
        }
    }
}

void ComponentLibTreeView::mouseDoubleClickEvent(QMouseEvent *event)
{
    QTreeView::mouseDoubleClickEvent(event);

    if (!event->buttons().testFlag(Qt::LeftButton))
    {
        return;
    }

    const QPersistentModelIndex index = indexAt(event->pos());
    if (!index.isValid())
    {
        return;
    }

    const QModelIndex &indexComponent = _sortProxy->mapToSource(index);
    if (indexComponent.isValid())
    {
        emit openedComponent(_model->component(indexComponent));
    }
}

void ComponentLibTreeView::contextMenuEvent(QContextMenuEvent *event)
{
    if (!_editMode)
    {
        return;
    }

    const QModelIndex &index = indexAt(event->pos());
    if (!index.isValid())
    {
        return;
    }

    const QModelIndex &indexFile = _sortProxy->mapToSource(index);
    if (!indexFile.isValid())
    {
        return;
    }

    QMenu menu;
    menu.addAction(_removeAction);
    menu.exec(event->globalPos());
}

ComponentLibItemModel *ComponentLibTreeView::model() const
{
    return _model;
}

bool ComponentLibTreeView::editMode() const
{
    return _editMode;
}

void ComponentLibTreeView::setEditMode(bool editMode)
{
    _editMode = editMode;
}

void ComponentLibTreeView::createActions()
{
    _removeAction = new QAction(this);
    _removeAction->setText(tr("Remove"));
    _removeAction->setShortcut(QKeySequence::Delete);
    _removeAction->setShortcutContext(Qt::WidgetShortcut);
    connect(_removeAction, &QAction::triggered, this, &ComponentLibTreeView::remove);
    addAction(_removeAction);
}
