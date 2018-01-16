#include "componentscene.h"

#include <QPainter>

ComponentScene::ComponentScene(qreal x, qreal y, qreal w, qreal h)
    : QGraphicsScene(x, y, w, h)
{
    _grid = true;
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
    const int gridSize = 20;

    if (_grid)
    {
        qreal left = int(rect.left()) - (int(rect.left()) % gridSize);
        qreal top = int(rect.top()) - (int(rect.top()) % gridSize);

        QVarLengthArray<QLineF, 100> lines;

        for (qreal x = left; x < rect.right(); x += gridSize)
            lines.append(QLineF(x, rect.top(), x, rect.bottom()));
        for (qreal y = top; y < rect.bottom(); y += gridSize)
            lines.append(QLineF(rect.left(), y, rect.right(), y));

        painter->setPen(Qt::darkGray);
        painter->drawLines(lines.data(), lines.size());
    }
}
