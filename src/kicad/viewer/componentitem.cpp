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

#include "componentitem.h"
#include "drawitem.h"

#include <QPainter>
#include <QDebug>

ComponentItem::ComponentItem(Component *component)
{
    setComponent(component);
}

void ComponentItem::paint(QPainter *painter,
                          const QStyleOptionGraphicsItem * /*option*/,
                          QWidget * /*widget*/)
{
    /*painter->setPen(QPen(QColor(132, 0, 0), 2));
    painter->setBrush(QColor(255, 255, 194));
    painter->drawRect(_numRect);*/
}

QRectF ComponentItem::boundingRect() const
{
    return _numRect.adjusted(-10, -10, 10, 10);
}

Component *ComponentItem::component() const
{
    return _component;
}

void ComponentItem::setComponent(Component *component)
{
    prepareGeometryChange();
    _component = component;
    _pinItemMap.clear();

    foreach (Pin *pin, component->pins())
    {
        PinItem *pinItem = new PinItem(pin);
        pinItem->setParentItem(this);
        _pinItemMap.insert(pin, pinItem);
    }
    foreach (Draw *draw, component->draws())
    {
        DrawItem *drawItem = new DrawItem(draw);
        drawItem->setParentItem(this);
    }
    DrawItem *drawItem = new DrawItem(component->refText());
    drawItem->setParentItem(this);
    drawItem = new DrawItem(component->nameText());
    drawItem->setParentItem(this);
    _numRect = QRect(_component->boundingRect().topLeft() / PinItem::ratio, _component->boundingRect().size() / PinItem::ratio).normalized();

    update();
}

PinItem *ComponentItem::pinItem(Pin *pin)
{
    QMap<Pin*, PinItem* >::const_iterator pinIt = _pinItemMap.constFind(pin);
    if (pinIt != _pinItemMap.cend())
        return *pinIt;
    return Q_NULLPTR;
}
