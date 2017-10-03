#include "pinitem.h"

#include <QPainter>

const int ratio=5;

PinItem::PinItem(Pin *pin)
{
    setPin(pin);
}

void PinItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setPen(Qt::red);
    painter->drawEllipse(-3, -3, 6, 6);

    QFont font("monospace");
    font.setStyleHint(QFont::Monospace);
    font.setPixelSize(12);
    painter->setFont(font);
    QFontMetrics metrics(font);

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
        if(line)
            painter->drawLine(-300/ratio-10, -painter->fontMetrics().height()/2, -300/ratio-10-painter->fontMetrics().width(name), -painter->fontMetrics().height()/2);
        break;
    case Pin::Right:
        painter->drawLine(3, 0, 300/ratio, 0);
        painter->drawText(QRect(0, 0, 300/ratio, -painter->fontMetrics().height()).normalized(), Qt::AlignHCenter, _pin->padname());
        painter->setPen(Qt::blue);
        painter->drawText(QRect(300/ratio+10, -painter->fontMetrics().height()/2, painter->fontMetrics().width(name), painter->fontMetrics().height()).normalized(), Qt::AlignCenter, name);
        if(line)
            painter->drawLine(300/ratio+10, -painter->fontMetrics().height()/2, 300/ratio+10+painter->fontMetrics().width(name), -painter->fontMetrics().height()/2);
        break;
    case Pin::Down:
        painter->drawLine(0, -3, 0, -300/ratio);
        break;
    case Pin::Up:
        painter->drawLine(0, 3, 0, 300/ratio);
        break;
    }
}

QRectF PinItem::boundingRect() const
{
    QRectF rect;
    QString name = _pin->name();
    QFont font("monospace");
    font.setPixelSize(12);
    font.setStyleHint(QFont::Monospace);
    QFontMetrics metrics(font);

    switch (_pin->direction())
    {
    case Pin::Left:
        rect = QRect(4, -metrics.height()/2, -metrics.width(name) - 300/ratio - 14, metrics.height());
        break;
    case Pin::Right:
        rect = QRect(-4, -metrics.height()/2, metrics.width(name) + 300/ratio + 14, metrics.height());
        break;
    case Pin::Down:
        rect = QRect(-metrics.height()/2, 0, metrics.height(), -metrics.width(name) - 300/ratio);
        break;
    case Pin::Up:
        rect = QRect(-metrics.height()/2, 0, metrics.height(), metrics.width(name) + 300/ratio);
        break;
    }
    return rect.normalized();
}

QPointF PinItem::base()
{
    switch (_pin->direction())
    {
    case Pin::Left:
        return _pin->pos() + QPointF(-300, 0);
    case Pin::Right:
        return _pin->pos() + QPointF(300, 0);
    case Pin::Down:
        return _pin->pos() + QPointF(0, -300);
    case Pin::Up:
        return _pin->pos() + QPointF(0, 300);
    }
}

Pin *PinItem::pin() const
{
    return _pin;
}

void PinItem::setPin(Pin *pin)
{
    _pin = pin;

    setPos(pin->pos() / ratio);
    update();
}
