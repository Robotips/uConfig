#include "componentlibtreeview.h"

ComponentLibTreeView::ComponentLibTreeView(Lib *lib, QWidget *parent) :
    QTreeView(parent)
{
    if (lib)
        _model = new ComponentLibItemModel(lib);
    else
        _model = new ComponentLibItemModel(new Lib());
    setModel(_model);
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
