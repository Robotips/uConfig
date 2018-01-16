#include "componentpinstableview.h"

#include <QMouseEvent>
#include <QDebug>

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
    if (!indexPin.isValid())
        return;
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
    foreach (Pin *pin, pins)
    {
        const QPersistentModelIndex index = _model->index(pin);
        if (!index.isValid())
            return;
        const QModelIndex &indexPin = _sortProxy->mapFromSource(index);
        if (!indexPin.isValid())
            return;
        selectionModel()->select(indexPin, QItemSelectionModel::Select | QItemSelectionModel::Rows);
    }
}

void ComponentPinsTableView::updateSelect(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(selected)
    Q_UNUSED(deselected)

    QSet<Pin*> selectedPins;

    foreach (const QModelIndex &index, selectionModel()->selectedIndexes())
    {
        if (!index.isValid())
            continue;
        const QModelIndex &indexComponent = _sortProxy->mapToSource(index);
        if (!indexComponent.isValid())
            continue;

        selectedPins.insert(_model->pin(indexComponent));
    }

    if (!selectedPins.isEmpty())
        emit pinSelected(selectedPins.toList());
}

ComponentPinsItemModel *ComponentPinsTableView::model() const
{
    return _model;
}
