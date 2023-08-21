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

#include "componentitem.h"
#include "drawitem.h"
#include "drawtextitem.h"

#include <QDebug>
#include <QFontDatabase>
#include <QGraphicsScene>
#include <QPainter>

const int ComponentItem::ratio = 5;

ComponentItem::ComponentItem(Component *component, int unit)
{
    setComponent(component, unit);
    _showElectricalType = true;
}

void ComponentItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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

void ComponentItem::setComponent(Component *component, int unit)
{
    prepareGeometryChange();
    _component = component;
    _pinItemMap.clear();
    _unit = unit;

    for (Pin *pin : component->pins())
    {
        if (pin->unit() == _unit || pin->unit() == 0)
        {
            PinItem *pinItem = new PinItem(pin);
            pinItem->setParentItem(this);
            _pinItemMap.insert(pin, pinItem);
        }
    }
    for (Draw *draw : component->draws())
    {
        if (draw->unit() == _unit || draw->unit() == 0)
        {
            DrawItem *drawItem = DrawItem::fromDraw(draw);
            if (drawItem != nullptr)
            {
                drawItem->setParentItem(this);
            }
        }
    }
    DrawItem *drawItem = new DrawTextItem(component->refText(), true);
    drawItem->setParentItem(this);
    drawItem = new DrawTextItem(component->nameText(), true);
    drawItem->setParentItem(this);
    _numRect = QRect(_component->boundingRect().topLeft() / ComponentItem::ratio, _component->boundingRect().size() / ComponentItem::ratio).normalized();

    update();
}

PinItem *ComponentItem::pinItem(Pin *pin)
{
    QMap<Pin *, PinItem *>::const_iterator pinIt = _pinItemMap.constFind(pin);
    if (pinIt != _pinItemMap.cend())
    {
        return *pinIt;
    }
    return nullptr;
}

void ComponentItem::removePin(Pin *pin)
{
    PinItem *mpinItem = pinItem(pin);
    if (mpinItem != nullptr)
    {
        _pinItemMap.remove(pin);
        scene()->removeItem(mpinItem);
    }
}

bool ComponentItem::showElectricalType() const
{
    return _showElectricalType;
}

void ComponentItem::setShowElectricalType(bool showElectricalType)
{
    if (showElectricalType != _showElectricalType)
    {
        for (PinItem *pinItem : _pinItemMap)
        {
            pinItem->setShowElectricalType(showElectricalType);
        }
    }
    _showElectricalType = showElectricalType;
}
