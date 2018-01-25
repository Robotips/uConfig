/**
 ** This file is part of the uConfig project.
 ** Copyright 2018 Robotips sebastien.caux@robotips.fr.
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

ComponentScene::ComponentScene(qreal x, qreal y, qreal w, qreal h)
    : QGraphicsScene(x, y, w, h)
{
    _grid = true;
    _prevGridSize = 0;
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

void ComponentScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    if (!_grid)
        return;

    int gridSize = 20;
    if (rect.width() > 4000)
        gridSize = 40;
    if (_prevGridSize != gridSize)
        update();
    _prevGridSize = gridSize;

    qreal left = int(rect.left()) - (int(rect.left()) % gridSize);
    qreal top = int(rect.top()) - (int(rect.top()) % gridSize);

    QVarLengthArray<QLineF, 100> lines;

    for (qreal x = left; x < rect.right(); x += gridSize)
        if (x != 0)
            lines.append(QLineF(x, rect.top(), x, rect.bottom()));
    for (qreal y = top; y < rect.bottom(); y += gridSize)
        if (y != 0)
            lines.append(QLineF(rect.left(), y, rect.right(), y));

    painter->setPen(Qt::lightGray);
    painter->drawLines(lines.data(), lines.size());
    painter->setPen(Qt::darkGray);
    painter->drawLine(QLineF(rect.left(), 0, rect.right(), 0));
    painter->drawLine(QLineF(0, rect.top(), 0, rect.bottom()));
}
