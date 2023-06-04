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

#include "componentscene.h"

#include <QPainter>
#include <QPrinter>

ComponentScene::ComponentScene(qreal x, qreal y, qreal w, qreal h)
    : QGraphicsScene(x, y, w, h)
{
    _grid = true;
    _gridFront = false;
    _prevGridSize = 0;

    setComponent(nullptr);
}

bool ComponentScene::grid() const
{
    return _grid;
}

void ComponentScene::setGrid(bool grid)
{
    _grid = grid;
    update();
}

bool ComponentScene::gridFront() const
{
    return _gridFront;
}

void ComponentScene::setGridFront(bool front)
{
    _gridFront = front;
    update();
}

bool ComponentScene::elecType() const
{
    return _elecType;
}

void ComponentScene::setElecType(bool elecType)
{
    _elecType = elecType;
    if (_componentItem != nullptr)
    {
        _componentItem->setShowElectricalType(elecType);
    }
}

Component *ComponentScene::component() const
{
    return _component;
}

void ComponentScene::setComponent(Component *component, int layer)
{
    clear();
    _component = component;

    if (component == nullptr)
    {
        _componentItem = nullptr;
        return;
    }

    _componentItem = new ComponentItem(component, layer);
    _componentItem->setShowElectricalType(_elecType);
    addItem(_componentItem);
}

ComponentItem *ComponentScene::componentItem() const
{
    return _componentItem;
}

void ComponentScene::setComponentItem(ComponentItem *componentItem)
{
    _componentItem = componentItem;
}

void ComponentScene::saveAsPdf(const QString &fileName)
{
    QPrinter printer(QPrinter::HighResolution);
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setPageOrientation(QPageLayout::Portrait);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);

    QPainter p;

    if (!p.begin(&printer))
    {
        return;
    }

    render(&p);
    p.end();
}

void ComponentScene::saveAsImage(const QString &fileName, const QSize &size)
{
    QImage image(size, QImage::Format_ARGB32);

    QPainter p;

    if (!p.begin(&image))
    {
        return;
    }
    p.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    p.fillRect(QRect(QPoint(0, 0), size), Qt::white);
    render(&p);
    p.end();

    image.save(fileName);
}

void ComponentScene::drawGrid(QPainter *painter, const QRectF &rect)
{
    int gridSize = 20;
    if (rect.width() > 4000)
    {
        gridSize = 40;
    }
    if (_prevGridSize != gridSize)
    {
        update();
    }
    _prevGridSize = gridSize;

    qreal left = int(rect.left()) - (int(rect.left()) % gridSize);
    qreal top = int(rect.top()) - (int(rect.top()) % gridSize);

    QVarLengthArray<QLineF, 100> lines;

    for (qreal x = left; x < rect.right(); x += gridSize)
    {
        if (x != 0)
        {
            lines.append(QLineF(x, rect.top(), x, rect.bottom()));
        }
    }
    for (qreal y = top; y < rect.bottom(); y += gridSize)
    {
        if (y != 0)
        {
            lines.append(QLineF(rect.left(), y, rect.right(), y));
        }
    }

    if (_gridFront)
    {
        painter->setPen(QColor(100, 100, 100, 127));
    }
    else
    {
        painter->setPen(Qt::lightGray);
    }
    painter->drawLines(lines.data(), lines.size());
    painter->setPen(Qt::darkGray);
    painter->drawLine(QLineF(rect.left(), 0, rect.right(), 0));
    painter->drawLine(QLineF(0, rect.top(), 0, rect.bottom()));
}

void ComponentScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    if (!_grid)
    {
        return;
    }

    if (!_gridFront)
    {
        drawGrid(painter, rect);
    }
}

void ComponentScene::drawForeground(QPainter *painter, const QRectF &rect)
{
    if (!_grid)
    {
        return;
    }

    if (_gridFront)
    {
        drawGrid(painter, rect);
    }
}
