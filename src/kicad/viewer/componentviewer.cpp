#include "componentviewer.h"

#include "componentitem.h"

#include <QDebug>
#include <QWheelEvent>
#include <QGraphicsScene>
#include <QMimeData>
#include <QUrl>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <qmath.h>

ComponentViewer::ComponentViewer(QWidget *parent)
    : QGraphicsView(parent)
{
    setScene(new QGraphicsScene());
    connect(scene(), &QGraphicsScene::selectionChanged, this, &ComponentViewer::selectedItem);
    scale(0.835, 0.835);
    _currentZoomLevel = 1;
    setDragMode(QGraphicsView::ScrollHandDrag);
    setCursor(Qt::ArrowCursor);

    setResizeAnchor(QGraphicsView::AnchorUnderMouse);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setRenderHint(QPainter::Antialiasing, true);
    setRenderHint(QPainter::SmoothPixmapTransform, true);
    setRenderHint(QPainter::TextAntialiasing, true);

    _component = Q_NULLPTR;
}

Component *ComponentViewer::component() const
{
    return _component;
}

void ComponentViewer::setComponent(Component *component)
{
    scene()->clear();
    _component = component;

    _componentItem = new ComponentItem(component);
    scene()->addItem(_componentItem);
    scene()->setSceneRect(_componentItem->boundingRect());
    fitInView(_componentItem, Qt::KeepAspectRatio);
}

void ComponentViewer::selectPin(Pin *pin)
{
    blockSignals(true);
    scene()->clearSelection();
    if (!pin)
        return;
    PinItem *pinItem = _componentItem->pinItem(pin);
    if (pinItem)
        pinItem->setSelected(true);
    blockSignals(false);
}

void ComponentViewer::selectPins(QList<Pin *> pins)
{
    blockSignals(true);
    scene()->clearSelection();
    foreach (Pin *pin, pins)
    {
        PinItem *pinItem = _componentItem->pinItem(pin);
        if (pinItem)
            pinItem->setSelected(true);
    }
    blockSignals(false);
}

void ComponentViewer::wheelEvent(QWheelEvent *event)
{
    int numDegrees = event->delta() / 8;
    int numSteps = numDegrees / 15;

    double mscale = qPow(1.25,numSteps);
    _currentZoomLevel *= mscale;
    scale(mscale, mscale);
}

void ComponentViewer::selectedItem()
{
    Pin *pin;
    if (scene()->selectedItems().isEmpty())
        pin = Q_NULLPTR;
    else
    {
        PinItem *pinItem = qgraphicsitem_cast<PinItem *>(scene()->selectedItems().at(0));
        pin = pinItem->pin();
    }

    emit pinSelected(pin);
}

void ComponentViewer::dragEnterEvent(QDragEnterEvent *event)
{
    if (!event->mimeData()->hasUrls())
        return;

    QString fileName = event->mimeData()->urls().first().toLocalFile();
    if (fileName.endsWith("pdf"))
        event->accept();
}

void ComponentViewer::dragMoveEvent(QDragMoveEvent *event)
{
    event->accept();
}

void ComponentViewer::dropEvent(QDropEvent *event)
{
    event->accept();
    QString fileName = event->mimeData()->urls().first().toLocalFile();
    emit droppedFile(fileName);
}
