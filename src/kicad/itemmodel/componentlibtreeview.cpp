#include "componentlibtreeview.h"

#include <QDebug>
#include <QMouseEvent>
#include <QMenu>
#include <QMessageBox>

ComponentLibTreeView::ComponentLibTreeView(Lib *lib, QWidget *parent) :
    QTreeView(parent)
{
    if (lib)
        _model = new ComponentLibItemModel(lib);
    else
        _model = new ComponentLibItemModel(new Lib());

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
}

void ComponentLibTreeView::addComponent(Component *component)
{
    _model->addComponent(component);
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

void ComponentLibTreeView::selectAll(bool selected)
{
    _model->selectAll(selected);
}

void ComponentLibTreeView::remove()
{
    if (!_editMode)
        return;

    QModelIndexList selection = selectionModel()->selectedIndexes();
    if (selection.isEmpty())
        return;

    if (selection.size() > 0)
    {
        if (QMessageBox::question(this, tr("Remove components?"), tr("Do you realy want to remove theses %1 components?")
                                 .arg(selection.size() / ComponentLibItemModel::ColumnCount)) != QMessageBox::Yes)
            return;
        QList<QPersistentModelIndex> pindex;
        foreach (QModelIndex selected, selection)
        {
            const QModelIndex &indexComponent = _sortProxy->mapToSource(selected);
            if (!indexComponent.isValid() || indexComponent.column() != 0)
                continue;

            pindex.append(indexComponent);
        }
        selectionModel()->clearSelection();
        emit openedComponent(Q_NULLPTR);
        foreach (QPersistentModelIndex index, pindex)
            _model->remove(index);
    }
}

void ComponentLibTreeView::mouseDoubleClickEvent(QMouseEvent *event)
{
    QTreeView::mouseDoubleClickEvent(event);

    if (!event->buttons().testFlag(Qt::LeftButton))
        return;

    const QPersistentModelIndex index = indexAt(event->pos());
    if (!index.isValid())
        return;

    const QModelIndex &indexComponent = _sortProxy->mapToSource(index);
    if (indexComponent.isValid())
        emit openedComponent(_model->component(indexComponent));
}

void ComponentLibTreeView::contextMenuEvent(QContextMenuEvent *event)
{
    if (!_editMode)
        return;

    const QModelIndex &index = indexAt(event->pos());
    if (!index.isValid())
        return;

    const QModelIndex &indexFile = _sortProxy->mapToSource(index);
    if (!indexFile.isValid())
        return;

    QMenu menu;
    menu.addAction(_removeAction);
    menu.exec(event->globalPos());
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
    connect(_removeAction, SIGNAL(triggered(bool)), this, SLOT(remove()));
    addAction(_removeAction);
}
