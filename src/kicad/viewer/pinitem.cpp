#include "pinitem.h"

#include <QPainter>

const int ratio=5;

PinItem::PinItem(Pin *pin)
{
    setPin(pin);
}

void PinItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    QFontMetrics metrics(QFont("Times"));
    painter->setPen(Qt::red);
    painter->drawEllipse(-3,-3,6,6);

    QString name = _pin->name();
    bool line=false;
    if(name.indexOf("~")>-1)
    {
        name.remove("~");
        line = true;
    }

    switch (_pin->direction())
    {
    case Pin::Left:
        painter->drawLine(-3, 0, -300/ratio, 0);
        painter->drawText(QRect(0, 0, -300/ratio, -painter->fontMetrics().height()).normalized(), Qt::AlignHCenter, _pin->padname());
        painter->setPen(Qt::blue);
        painter->drawText(QRect(-300/ratio-10, -painter->fontMetrics().height()/2, -painter->fontMetrics().width(name), painter->fontMetrics().height()).normalized(), Qt::AlignCenter, name);
        if(line) painter->drawLine(-300/ratio-10, -painter->fontMetrics().height()/2, -300/ratio-10-painter->fontMetrics().width(name), -painter->fontMetrics().height()/2);
        break;
    case Pin::Right:
        painter->drawLine(3, 0, 300/ratio, 0);
        painter->drawText(QRect(0, 0, 300/ratio, -painter->fontMetrics().height()).normalized(), Qt::AlignHCenter, _pin->padname());
        painter->setPen(Qt::blue);
        painter->drawText(QRect(300/ratio+10, -painter->fontMetrics().height()/2, painter->fontMetrics().width(name), painter->fontMetrics().height()).normalized(), Qt::AlignCenter, name);
        if(line) painter->drawLine(300/ratio+10, -painter->fontMetrics().height()/2, 300/ratio+10+painter->fontMetrics().width(name), -painter->fontMetrics().height()/2);
        break;
    case Pin::Down:
        painter->drawLine(0, -3, 0, -300/ratio);
        break;
    case Pin::Up:
        painter->drawLine(0, 3, 0, 300/ratio);
        break;
    default:
        break;
    }
}

QRectF PinItem::boundingRect() const
{
    QString name = _pin->name();
    QFontMetrics metrics(QFont("Times"));
    switch (_pin->direction())
    {
    case Pin::Left:
        return QRect(0,-metrics.height()/2,-metrics.width(name)-300/ratio,metrics.height());
    case Pin::Right:
        return QRect(0,-metrics.height()/2,metrics.width(name)+300/ratio,metrics.height());
    case Pin::Down:
        return QRect(-metrics.height()/2,0,metrics.height(),-metrics.width(name)-300/ratio);
    case Pin::Up:
        return QRect(-metrics.height()/2,0,metrics.height(),metrics.width(name)+300/ratio);
    default:
        return QRect(0,0,0,0);
    }
}

QPointF PinItem::base()
{
    switch (_pin->direction())
    {
    case Pin::Left:
        return _pin->pos()+QPointF(-300, 0);
    case Pin::Right:
        return _pin->pos()+QPointF(300, 0);
    case Pin::Down:
        return _pin->pos()+QPointF(0, -300);
    case Pin::Up:
        return _pin->pos()+QPointF(0, 300);
    default:
        return _pin->pos();
    }
}

Pin *PinItem::pin() const
{
    return _pin;
}

void PinItem::setPin(Pin *pin)
{
    _pin = pin;

    setPos(pin->pos()/ratio);
    update();
}
