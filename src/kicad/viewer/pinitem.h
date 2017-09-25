#ifndef PINITEM_H
#define PINITEM_H

#include <QGraphicsItem>
#include "pin.h"

class PinItem : public QGraphicsItem
{
public:
    PinItem(Pin *pin);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;

    QPointF base();

    Pin *pin() const;
    void setPin(Pin *pin);

private:
    Pin *_pin;
};

#endif // PINITEM_H
