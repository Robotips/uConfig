#include "pinitem.h"

#include <QCursor>
#include <QPainter>

const int PinItem::ratio=5;

PinItem::PinItem(Pin *pin)
{
    setPin(pin);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setCursor(Qt::CrossCursor);
}

void PinItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    // colors
    QColor drawColor = QColor(132, 0, 0);
    QColor textColor = QColor(0, 132, 132);
    if (_pin->pinType() == Pin::NotVisible)
    {
        drawColor = QColor(132, 132, 132);
        textColor = QColor(132, 132, 132);
    }

    // text font
    QFont font("monospace");
    font.setStyleHint(QFont::Monospace);
    font.setPointSizeF(12);

    // selection modifier
    if (isSelected())
    {
        painter->setPen(QPen(drawColor, 2));
        font.setBold(true);
    }
    else
        painter->setPen(QPen(drawColor, 1));

    painter->setFont(font);
    painter->drawEllipse(-3, -3, 6, 6);

    QString name = _pin->name();
    switch (_pin->direction())
    {
    case Pin::Left:
        painter->drawLine(-3, 0, -_pin->length()/ratio, 0);
        painter->drawText(QRect(0, 3, -_pin->length()/ratio, -painter->fontMetrics().height()).normalized(), Qt::AlignHCenter, _pin->padname());
        painter->setPen(textColor);
        painter->drawText(QRect(-_pin->length()/ratio-10, -painter->fontMetrics().height()/2, -name.count()*50/ratio, painter->fontMetrics().height()).normalized(), Qt::AlignRight, name);
        break;
    case Pin::Right:
        painter->drawLine(3, 0, _pin->length()/ratio, 0);
        painter->drawText(QRect(0, 3, _pin->length()/ratio, -painter->fontMetrics().height()).normalized(), Qt::AlignHCenter, _pin->padname());
        painter->setPen(textColor);
        painter->drawText(QRect(_pin->length()/ratio+10, -painter->fontMetrics().height()/2, name.count()*50/ratio, painter->fontMetrics().height()).normalized(), Qt::AlignLeft, name);
        break;
    case Pin::Up:
        painter->drawLine(0, -3, 0, -_pin->length()/ratio);
        painter->rotate(-90);
        painter->drawText(QRect(0, 3, _pin->length()/ratio, -painter->fontMetrics().height()).normalized(), Qt::AlignHCenter, _pin->padname());
        painter->setPen(textColor);
        painter->drawText(QRect(_pin->length()/ratio+10, -painter->fontMetrics().height()/2, name.count()*50/ratio, painter->fontMetrics().height()).normalized(), Qt::AlignLeft, name);
        painter->rotate(90);
        break;
    case Pin::Down:
        painter->drawLine(0, 3, 0, _pin->length()/ratio);
        painter->rotate(90);
        painter->drawText(QRect(0, 3, _pin->length()/ratio, -painter->fontMetrics().height()).normalized(), Qt::AlignHCenter, _pin->padname());
        painter->setPen(textColor);
        painter->drawText(QRect(_pin->length()/ratio+10, -painter->fontMetrics().height()/2, name.count()*50/ratio, painter->fontMetrics().height()).normalized(), Qt::AlignLeft, name);
        painter->rotate(-90);
        break;
    }
}

QRectF PinItem::boundingRect() const
{
    QRectF rect;
    QString name = _pin->name();
    QFont font("monospace");
    font.setPointSizeF(12);
    font.setStyleHint(QFont::Monospace);
    QFontMetrics metrics(font);

    switch (_pin->direction())
    {
    case Pin::Left:
        rect = QRect(4, -metrics.height(), -name.count()*50/ratio - _pin->length()/ratio - 14, metrics.height()*3/2);
        break;
    case Pin::Right:
        rect = QRect(-4, -metrics.height(), name.count()*50/ratio + _pin->length()/ratio + 14, metrics.height()*3/2);
        break;
    case Pin::Up:
        rect = QRect(-metrics.height(), 4, metrics.height()*3/2, -name.count()*50/ratio - _pin->length()/ratio - 14);
        break;
    case Pin::Down:
        rect = QRect(-metrics.height()/2, -4, metrics.height()*3/2, name.count()*50/ratio + _pin->length()/ratio + 14);
        break;
    }
    return rect.normalized();
}

QPainterPath PinItem::shape() const
{
    QRectF rect;
    QString name = _pin->name();
    QFont font("monospace");
    font.setPointSizeF(12);
    font.setStyleHint(QFont::Monospace);
    QFontMetrics metrics(font);

    switch (_pin->direction())
    {
    case Pin::Left:
        rect = QRect(4, -metrics.height()/2, -name.count()*50/ratio - _pin->length()/ratio - 14, metrics.height());
        break;
    case Pin::Right:
        rect = QRect(-4, -metrics.height()/2, name.count()*50/ratio + _pin->length()/ratio + 14, metrics.height());
        break;
    case Pin::Up:
        rect = QRect(-metrics.height()/2, 0, metrics.height(), -name.count()*50/ratio - _pin->length()/ratio - 14);
        break;
    case Pin::Down:
        rect = QRect(-metrics.height()/2, 0, metrics.height(), name.count()*50/ratio + _pin->length()/ratio + 14);
        break;
    }

    QPainterPath path;
    path.addRect(rect.normalized());
    return path;
}

QPointF PinItem::base()
{
    switch (_pin->direction())
    {
    case Pin::Left:
        return _pin->pos() + QPointF(-_pin->length(), 0);
    case Pin::Right:
        return _pin->pos() + QPointF(_pin->length(), 0);
    case Pin::Up:
        return _pin->pos() + QPointF(0, -_pin->length());
    case Pin::Down:
        return _pin->pos() + QPointF(0, _pin->length());
    }
    return QPointF();
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
