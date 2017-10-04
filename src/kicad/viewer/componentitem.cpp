#include "componentitem.h"

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
    painter->setPen(QColor(132, 0, 0));
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
    _numRect = QRectF(0, 0, 0, 0);

    foreach (Pin *pin, component->pins())
    {
        PinItem *item = new PinItem(pin);
        item->setParentItem(this);
        _numRect = _numRect.united(QRectF(item->base() / PinItem::ratio, QSize(1, 1)));
    }
    _numRect.adjust(1, -20, -2, 20);

    update();
}
