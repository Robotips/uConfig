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
#include "drawtextitem.h"

#include <QPainter>
#include <QDebug>
#include <QFontDatabase>

const int ComponentItem::ratio=5;

ComponentItem::ComponentItem(Component *component, int layer)
{
    setComponent(component, layer);
    _showElectricalType = true;
}

void ComponentItem::paint(QPainter *painter,
                          const QStyleOptionGraphicsItem * option,
                          QWidget * widget)
{
    Q_UNUSED(painter)
    Q_UNUSED(option)
    Q_UNUSED(widget)
}

QRectF ComponentItem::boundingRect() const
{
    return _numRect.adjusted(-100, -100, 100, 100);
}

Component *ComponentItem::component() const
{
    return _component;
}

void ComponentItem::setComponent(Component *component, int layer)
{
    prepareGeometryChange();
    _component = component;
    _pinItemMap.clear();
    _layer = layer;

    foreach (Pin *pin, component->pins())
    {
        if (pin->layer() == _layer || pin->layer() == 0)
        {
            PinItem *pinItem = new PinItem(pin);
            pinItem->setParentItem(this);
            _pinItemMap.insert(pin, pinItem);
        }
    }
    foreach (Draw *draw, component->draws())
    {
        if (draw->unit() == _layer || draw->unit() == 0)
        {
            DrawItem *drawItem = DrawItem::fromDraw(draw);
            if (drawItem)
                drawItem->setParentItem(this);
        }
    }
    DrawItem *drawItem = new DrawTextItem(component->refText(), true);
    drawItem->setParentItem(this);
    drawItem = new DrawTextItem(component->nameText(), true);
    drawItem->setParentItem(this);
    _numRect = QRect(_component->boundingRect().topLeft() / ComponentItem::ratio,
                     _component->boundingRect().size() / ComponentItem::ratio).normalized();

    update();
}

PinItem *ComponentItem::pinItem(Pin *pin)
{
    QMap<Pin*, PinItem* >::const_iterator pinIt = _pinItemMap.constFind(pin);
    if (pinIt != _pinItemMap.cend())
        return *pinIt;
    return Q_NULLPTR;
}

bool ComponentItem::showElectricalType() const
{
    return _showElectricalType;
}

void ComponentItem::setShowElectricalType(bool showElectricalType)
{
    if (showElectricalType != _showElectricalType)
    {
        foreach (PinItem *pinItem, _pinItemMap)
            pinItem->setShowElectricalType(showElectricalType);
    }
    _showElectricalType = showElectricalType;
}

QFont ComponentItem::font(double size)
{
    QFont font("monospace");
    font.setStyleHint(QFont::Monospace);
    font.setPointSizeF(12);
    QFontMetricsF fm(font);
    double factor = 60.0 / ComponentItem::ratio / fm.width('m') * (size / 12.7);
    font.setPointSizeF(font.pointSizeF() * factor);
    return font;
}
