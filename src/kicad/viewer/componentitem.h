#ifndef COMPONENTITEM_H
#define COMPONENTITEM_H

#include "kicad_global.h"

#include <QGraphicsItem>
#include <QMap>

#include "model/component.h"
#include "pinitem.h"

class PinItem;

class KICAD_EXPORT ComponentItem : public QGraphicsItem
{
public:
    ComponentItem(Component *component = Q_NULLPTR);

    enum { Type = UserType + 2 };
    int type() const {return Type;}

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;

    Component *component() const;
    void setComponent(Component *component);

    PinItem *pinItem(Pin *pin);

private:
    Component *_component;

    QRectF _numRect;
    QMap<Pin*, PinItem* > _pinItemMap;
};

#endif // COMPONENTITEM_H
