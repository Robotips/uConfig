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

#ifndef COMPONENTSCENE_H
#define COMPONENTSCENE_H

#include <QGraphicsScene>
#include <QtCore/qglobal.h>

#include "componentitem.h"
#include "model/component.h"

class KICAD_EXPORT ComponentScene : public QGraphicsScene
{
public:
    ComponentScene(qreal x = -5000, qreal y = -5000, qreal w = 10000, qreal h = 10000);

    bool grid() const;
    void setGrid(bool grid);

    bool gridFront() const;
    void setGridFront(bool front);

    bool elecType() const;
    void setElecType(bool elecType);

    Component *component() const;
    void setComponent(Component *component, int layer = 1);

    ComponentItem *componentItem() const;
    void setComponentItem(ComponentItem *componentItem);

    void saveAsPdf(const QString &fileName);
    void saveAsImage(const QString &fileName, const QSize &size = QSize(1000, 1000));

protected:
    bool _grid;
    bool _gridFront;
    int _prevGridSize;

    bool _elecType;

    Component *_component;
    ComponentItem *_componentItem;

    void drawGrid(QPainter *painter, const QRectF &rect);
    void drawBackground(QPainter *painter, const QRectF &rect) override;
    void drawForeground(QPainter *painter, const QRectF &rect) override;
};

#endif  // COMPONENTSCENE_H
