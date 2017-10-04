#include "componentviewer.h"

#include "componentitem.h"

#include <QDebug>
#include <QWheelEvent>
#include <qmath.h>

ComponentViewer::ComponentViewer(QWidget *parent)
    : QGraphicsView(parent)
{
    setScene(new QGraphicsScene());
    scale(0.835, 0.835);
    _currentZoomLevel = 1;
    setDragMode(QGraphicsView::ScrollHandDrag);

    setResizeAnchor(QGraphicsView::AnchorUnderMouse);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setRenderHint(QPainter::Antialiasing, true);
    setRenderHint(QPainter::SmoothPixmapTransform, true);
    setRenderHint(QPainter::TextAntialiasing, true);
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

void ComponentViewer::wheelEvent(QWheelEvent *event)
{
    int numDegrees = event->delta() / 8;
    int numSteps = numDegrees / 15;

    double mscale = qPow(1.25,numSteps);
    _currentZoomLevel *= mscale;
    scale(mscale, mscale);
}
