#ifndef COMPONENTITEM_H
#define COMPONENTITEM_H

#include <QGraphicsItem>
#include "component.h"
#include "pinitem.h"

class ComponentItem : public QGraphicsItem
{
public:
    ComponentItem(Component *component = NULL);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;

    Component *component() const;
    void setComponent(Component *component);

private:
    Component *_component;

    QRectF _numRect;
};

#endif // COMPONENTITEM_H
