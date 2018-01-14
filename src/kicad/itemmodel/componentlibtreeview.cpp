#include "componentlibtreeview.h"

#include <QDebug>
#include <QMouseEvent>

ComponentLibTreeView::ComponentLibTreeView(Lib *lib, QWidget *parent) :
    QTreeView(parent)
{
    if (lib)
        _model = new ComponentLibItemModel(lib);
    else
        _model = new ComponentLibItemModel(new Lib());

    _sortProxy = new NumericalSortFilterProxyModel();
    _sortProxy->setSourceModel(_model);
    setModel(_sortProxy);

    sortByColumn(0, Qt::AscendingOrder);

    setSortingEnabled(true);
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

bool ComponentLibTreeView::getSelectedMode() const
{
    return _model->getSelectedMode();
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
