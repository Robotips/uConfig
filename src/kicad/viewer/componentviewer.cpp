#include "componentviewer.h"

#include "componentitem.h"

#include <QDebug>

ComponentViewer::ComponentViewer(QWidget *parent)
    : QGraphicsView(parent)
{
    setScene(new QGraphicsScene());
}

Component *ComponentViewer::component() const
{
    return _component;
}

void ComponentViewer::setComponent(Component *component)
{
    _component = component;

    ComponentItem *componentItem = new ComponentItem(component);
    scene()->addItem(componentItem);
}
