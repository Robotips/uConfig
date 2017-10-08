#ifndef PINITEM_H
#define PINITEM_H

#include "kicad_global.h"

#include <QGraphicsItem>
#include "pin.h"

class KICAD_EXPORT PinItem : public QGraphicsItem
{
public:
    PinItem(Pin *pin);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;

    QPointF base();

    Pin *pin() const;
    void setPin(Pin *pin);

    static const int ratio;

private:
    Pin *_pin;
};

#endif // PINITEM_H
