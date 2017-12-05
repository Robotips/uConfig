#include "componentpinstableview.h"

#include <QMouseEvent>

ComponentPinsTableView::ComponentPinsTableView(Component *component, QWidget *parent)
    : QTableView(parent)
{
    if (component)
        _model = new ComponentPinsItemModel(component);
    else
        _model = new ComponentPinsItemModel(new Component());

    _sortProxy = new NumericalSortFilterProxyModel();
    _sortProxy->setSourceModel(_model);
    setModel(_sortProxy);
    connect(selectionModel(), &QItemSelectionModel::selectionChanged, this, &ComponentPinsTableView::updateSelect);

    sortByColumn(0, Qt::AscendingOrder);

    setSortingEnabled(true);
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
    if (!pin)
    {
        selectionModel()->clearSelection();
        return;
    }
    const QPersistentModelIndex index = _model->index(pin);
    if (!index.isValid())
        return;
    const QModelIndex &indexPin = _sortProxy->mapFromSource(index);
    selectionModel()->select(indexPin, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
    scrollTo(indexPin);
}

void ComponentPinsTableView::updateSelect(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(deselected)
    if (selected.indexes().count() != 1)
        return;
    if (!selected.indexes().first().isValid())
        return;

    const QModelIndex &indexComponent = _sortProxy->mapToSource(selected.indexes().first());
    emit pinSelected(_model->pin(indexComponent));
}
