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

#include "componentviewer.h"

#include "componentitem.h"

#include <QDebug>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QGraphicsScene>
#include <QMimeData>
#include <QUrl>
#include <QWheelEvent>
#include <qmath.h>

ComponentViewer::ComponentViewer(QWidget *parent)
    : QGraphicsView(parent)
{
    _scene = new ComponentScene(-5000, -5000, 10000, 10000);
    setScene(_scene);
    scale(0.835, 0.835);
    setDragMode(QGraphicsView::ScrollHandDrag);
    setCursor(Qt::ArrowCursor);

    setResizeAnchor(QGraphicsView::AnchorUnderMouse);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setRenderHint(QPainter::Antialiasing, true);
    setRenderHint(QPainter::SmoothPixmapTransform, true);
    setRenderHint(QPainter::TextAntialiasing, true);
}

Component *ComponentViewer::component() const
{
    return _scene->component();
}

void ComponentViewer::setComponent(Component *component, int layer)
{
    disconnect(scene(), &QGraphicsScene::selectionChanged, this, &ComponentViewer::selectedItem);
    _scene->setComponent(component, layer);
    if (component != nullptr)
    {
        scene()->setSceneRect(_scene->componentItem()->boundingRect());
    }
    // fitInView(_scene->componentItem(), Qt::KeepAspectRatio);

    connect(scene(), &QGraphicsScene::selectionChanged, this, &ComponentViewer::selectedItem);
}

void ComponentViewer::selectPin(Pin *pin)
{
    QSignalBlocker(this);

    scene()->clearSelection();
    if (pin == nullptr)
    {
        return;
    }

    PinItem *pinItem = _scene->componentItem()->pinItem(pin);
    if (pinItem != nullptr)
    {
        pinItem->setSelected(true);
    }
}

void ComponentViewer::selectPins(const QList<Pin *> &pins)
{
    if (_scene->componentItem() == nullptr)
    {
        return;
    }

    blockSignals(true);
    scene()->clearSelection();
    PinItem *pinItem = nullptr;
    for (Pin *pin : pins)
    {
        pinItem = _scene->componentItem()->pinItem(pin);
        if (pinItem != nullptr)
        {
            pinItem->setSelected(true);
        }
    }
    if (pinItem != nullptr)
    {
        ensureVisible(pinItem, 5, 5);
    }
    blockSignals(false);
}

void ComponentViewer::updatePin(Pin *pin)
{
    if (_scene->componentItem() == nullptr)
    {
        return;
    }

    PinItem *pinItem = _scene->componentItem()->pinItem(pin);
    if (pinItem != nullptr)
    {
        pinItem->updateData();
    }
}

void ComponentViewer::removePin(Pin *pin)
{
    if (_scene->componentItem() == nullptr)
    {
        return;
    }
    _scene->componentItem()->removePin(pin);
}

void ComponentViewer::setGridVisible(bool visible)
{
    _scene->setGrid(visible);
}

void ComponentViewer::setGridFront(bool front)
{
    _scene->setGridFront(front);
}

void ComponentViewer::setElecTypeVisible(bool visible)
{
    _scene->setElecType(visible);
}

void ComponentViewer::wheelEvent(QWheelEvent *event)
{
    double numSteps = event->angleDelta().y() / 8.0 / 15.0;
    double mscale = qPow(1.25, numSteps);

    if (transform().m11() < 0.1 && mscale < 1)
    {
        return;
    }
    if (transform().m11() > 20 && mscale > 1)
    {
        return;
    }

    scale(mscale, mscale);
}

void ComponentViewer::selectedItem()
{
    QList<Pin *> selectedPins;

    for (QGraphicsItem *item : scene()->selectedItems())
    {
        PinItem *pinItem = qgraphicsitem_cast<PinItem *>(item);
        selectedPins.append(pinItem->pin());
    }

    emit pinsSelected(selectedPins);
}

void ComponentViewer::dragEnterEvent(QDragEnterEvent *event)
{
    if (!event->mimeData()->hasUrls())
    {
        return;
    }

    QString fileName = event->mimeData()->urls().first().toLocalFile();
    if (fileName.endsWith(".pdf", Qt::CaseInsensitive) || fileName.endsWith(".lib", Qt::CaseInsensitive))
    {
        event->accept();
    }
}

void ComponentViewer::dragMoveEvent(QDragMoveEvent *event)
{
    QGraphicsView::dragMoveEvent(event);
    event->accept();
}

void ComponentViewer::dropEvent(QDropEvent *event)
{
    QGraphicsView::dropEvent(event);
    event->accept();
    QString fileName = event->mimeData()->urls().first().toLocalFile();
    emit droppedFile(fileName);
}
