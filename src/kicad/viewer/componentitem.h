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

#ifndef COMPONENTITEM_H
#define COMPONENTITEM_H

#include <QGraphicsItem>
#include <QMap>
#include <QtCore/qglobal.h>

#include "model/component.h"
#include "pinitem.h"

class PinItem;

class KICAD_EXPORT ComponentItem : public QGraphicsItem
{
public:
    ComponentItem(Component *component = nullptr, int unit = 1);

    enum
    {
        Type = UserType + 2
    };
    int type() const override
    {
        return Type;
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;

    Component *component() const;
    void setComponent(Component *component, int unit = 1);

    PinItem *pinItem(Pin *pin);
    void removePin(Pin *pin);

    bool showElectricalType() const;
    void setShowElectricalType(bool showElectricalType);

    static const int ratio;

private:
    Component *_component;
    int _unit;
    bool _showElectricalType;

    QRectF _numRect;
    QMap<Pin *, PinItem *> _pinItemMap;
};

#endif  // COMPONENTITEM_H
