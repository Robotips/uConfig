#include "componentpinstableview.h"

ComponentPinsTableView::ComponentPinsTableView(Component *component, QWidget *parent)
    : QTableView(parent)
{
    if (component)
        _model = new ComponentPinsItemModel(component);
    else
        _model = new ComponentPinsItemModel(new Component());
    setModel(_model);
}

Component *ComponentPinsTableView::component() const
{
    return _model->component();
}

void ComponentPinsTableView::setComponent(Component *component)
{
    _model->setComponent(component);
}
