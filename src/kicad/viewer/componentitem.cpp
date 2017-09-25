#include "componentitem.h"

#include <QPainter>
#include <QDebug>

const int ratio=5;

ComponentItem::ComponentItem(Component *component)
{
    setComponent(component);
}

void ComponentItem::paint(QPainter *painter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/)
{
    painter->setPen(Qt::red);
    painter->drawRect(_numRect);
}

QRectF ComponentItem::boundingRect() const
{
    return _numRect;
}

Component *ComponentItem::component() const
{
    return _component;
}

void ComponentItem::setComponent(Component *component)
{
    _component = component;
    _numRect = QRectF(0,0,0,0);

    foreach (Pin *pin, component->pins())
    {
        PinItem *item = new PinItem(pin);
        item->setParentItem(this);
        _numRect = _numRect.united(QRectF(item->base()/ratio, QSize(1,1)));
    }
    _numRect.adjust(0,-30,0,30);

    update();
}
