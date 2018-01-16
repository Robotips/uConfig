#ifndef COMPONENTSCENE_H
#define COMPONENTSCENE_H

#include "kicad_global.h"

#include <QGraphicsScene>

class KICAD_EXPORT ComponentScene : public QGraphicsScene
{
public:
    ComponentScene(qreal x, qreal y, qreal w, qreal h);

    bool grid() const;
    void setGrid(bool grid);

protected:
    bool _grid;
    int _prevGridSize;

    void drawBackground(QPainter *painter, const QRectF &rect);
};

#endif // COMPONENTSCENE_H
