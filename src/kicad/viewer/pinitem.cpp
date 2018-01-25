#include "pinitem.h"

#include <QCursor>
#include <QPainter>
#include <QDebug>

#include "model/component.h"

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
    painter->setFont(font);
    double factor = 50.0 / PinItem::ratio / painter->fontMetrics().width('m');
    if ((factor < 1) || (factor > 1.25))
    {
        font.setPointSizeF(font.pointSizeF()*factor);
    }

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

    // line invert draw
    QList<int> linesInvert;
    int i = _pin->name().indexOf('~');
    while (i >= 0)
    {
        linesInvert.append(i-linesInvert.count());
        i = _pin->name().indexOf('~', i+1);
    }
    if (linesInvert.count() % 2 == 1)
        linesInvert.append(_pin->name().length()-linesInvert.count());

    QPolygon polygon;
    QString name = _pin->name();
    name.remove('~');
    switch (_pin->direction())
    {
    case Pin::Up:
        painter->rotate(-90);
    case Pin::Right:
        painter->drawLine(3, 0, _pin->length()/ratio, 0);

        if (_pin->component()->showPadName())
            painter->drawText(QRect(0, 3, _pin->length()/ratio, -painter->fontMetrics().height()).normalized(), Qt::AlignHCenter, _pin->padName());

        switch (_pin->pinType())
        {
        case Pin::InvertedClock:
            painter->drawLine(_pin->length()/ratio+8, 0, _pin->length()/ratio,  5);
            painter->drawLine(_pin->length()/ratio+8, 0, _pin->length()/ratio, -5);
        case Pin::Invert:
            painter->setBrush(Qt::white);
            painter->drawEllipse(QPoint(_pin->length()/ratio-5, 0), 5, 5);
            break;
        case Pin::ClockLow:
            painter->drawLine(_pin->length()/ratio,      0, _pin->length()/ratio-10, -10);
            painter->drawLine(_pin->length()/ratio-10, -10, _pin->length()/ratio-10, 0);
        case Pin::Clock:
            painter->drawLine(_pin->length()/ratio+8, 0, _pin->length()/ratio,  5);
            painter->drawLine(_pin->length()/ratio+8, 0, _pin->length()/ratio, -5);
            break;
        case Pin::LowIn:
            painter->drawLine(_pin->length()/ratio,      0, _pin->length()/ratio-10, -10);
            painter->drawLine(_pin->length()/ratio-10, -10, _pin->length()/ratio-10, 0);
            break;
        case Pin::LowOut:
            painter->drawLine(_pin->length()/ratio, -10, _pin->length()/ratio-10, 0);
            break;
        case Pin::FallingEdge:
            painter->setBrush(Qt::white);
            polygon << QPoint(_pin->length()/ratio, 5) << QPoint(_pin->length()/ratio-10, 0) << QPoint(_pin->length()/ratio, -5);
            painter->drawPolygon(polygon);
            break;
        case Pin::NonLogic:
            painter->drawLine(_pin->length()/ratio+5, -5, _pin->length()/ratio-5,  5);
            painter->drawLine(_pin->length()/ratio+5,  5, _pin->length()/ratio-5, -5);
            break;
        }
        painter->setPen(textColor);
        if (_pin->component()->showPinName())
        {
            painter->drawText(QRect(_pin->length()/ratio+10, -painter->fontMetrics().height()/2, name.count()*50/ratio, painter->fontMetrics().height()).normalized(), Qt::AlignLeft, name);
            for (int l=0; l<linesInvert.count(); l+=2)
            {
                painter->drawLine(_pin->length()/ratio+8 + linesInvert[l]*50/ratio, -painter->fontMetrics().height()/2,
                                  _pin->length()/ratio+8 + linesInvert[l+1]*50/ratio, -painter->fontMetrics().height()/2);
            }
        }
        break;

    case Pin::Down:
        painter->rotate(-90);
    case Pin::Left:
        painter->drawLine(-3, 0, -_pin->length()/ratio, 0);

        if (_pin->component()->showPadName())
            painter->drawText(QRect(0, 3, -_pin->length()/ratio, -painter->fontMetrics().height()).normalized(), Qt::AlignHCenter, _pin->padName());

        switch (_pin->pinType())
        {
        case Pin::InvertedClock:
            painter->drawLine(-_pin->length()/ratio-8, 0, -_pin->length()/ratio,  5);
            painter->drawLine(-_pin->length()/ratio-8, 0, -_pin->length()/ratio, -5);
        case Pin::Invert:
            painter->setBrush(Qt::white);
            painter->drawEllipse(QPoint(-_pin->length()/ratio+5, 0), 5, 5);
            break;
        case Pin::ClockLow:
            painter->drawLine(-_pin->length()/ratio,      0, -_pin->length()/ratio+10, -10);
            painter->drawLine(-_pin->length()/ratio+10, -10, -_pin->length()/ratio+10, 0);
        case Pin::Clock:
            painter->drawLine(-_pin->length()/ratio-8, 0, -_pin->length()/ratio,  5);
            painter->drawLine(-_pin->length()/ratio-8, 0, -_pin->length()/ratio, -5);
            break;
        case Pin::LowIn:
            painter->drawLine(-_pin->length()/ratio,      0, -_pin->length()/ratio+10, -10);
            painter->drawLine(-_pin->length()/ratio+10, -10, -_pin->length()/ratio+10, 0);
            break;
        case Pin::LowOut:
            painter->drawLine(-_pin->length()/ratio, -10, -_pin->length()/ratio+10, 0);
            break;
        case Pin::FallingEdge:
            painter->setBrush(Qt::white);
            polygon << QPoint(-_pin->length()/ratio, 5) << QPoint(-_pin->length()/ratio+10, 0) << QPoint(-_pin->length()/ratio, -5);
            painter->drawPolygon(polygon);
            break;
        case Pin::NonLogic:
            painter->drawLine(-_pin->length()/ratio-5, -5, -_pin->length()/ratio+5,  5);
            painter->drawLine(-_pin->length()/ratio-5,  5, -_pin->length()/ratio+5, -5);
            break;
        }
        painter->setPen(textColor);

        if (_pin->component()->showPinName())
        {
            painter->drawText(QRect(-_pin->length()/ratio-10, -painter->fontMetrics().height()/2, -name.count()*50/ratio, painter->fontMetrics().height()).normalized(), Qt::AlignRight, name);
            for (int l=0; l<linesInvert.count(); l+=2)
            {
                painter->drawLine(-_pin->length()/ratio-8-name.count()*50/ratio + linesInvert[l]*50/ratio, -painter->fontMetrics().height()/2,
                                  -_pin->length()/ratio-8-name.count()*50/ratio + linesInvert[l+1]*50/ratio, -painter->fontMetrics().height()/2);
            }
        }
        break;
    }
}

QRectF PinItem::boundingRect() const
{
    QRectF rect;
    QString name = _pin->name();
    name.remove('~');
    QFont font("monospace");
    font.setPointSizeF(12);
    font.setStyleHint(QFont::Monospace);
    QFontMetrics metrics(font);

    switch (_pin->direction())
    {
    case Pin::Left:
        rect = QRect(4, -metrics.height()-1, -name.count()*50/ratio - _pin->length()/ratio - 14, metrics.height()*3/2+2);
        break;
    case Pin::Right:
        rect = QRect(-4, -metrics.height()-1, name.count()*50/ratio + _pin->length()/ratio + 14, metrics.height()*3/2+2);
        break;
    case Pin::Up:
        rect = QRect(-metrics.height()-1, 4, metrics.height()*3/2+2, -name.count()*50/ratio - _pin->length()/ratio - 14);
        break;
    case Pin::Down:
        rect = QRect(-metrics.height()-1, -4, metrics.height()*3/2+2, name.count()*50/ratio + _pin->length()/ratio + 14);
        break;
    }
    return rect.normalized();
}

QPainterPath PinItem::shape() const
{
    QRectF textRect, numRect;
    QString name = _pin->name();
    name.remove('~');
    QFont font("monospace");
    font.setPointSizeF(12);
    font.setStyleHint(QFont::Monospace);
    QFontMetrics metrics(font);

    switch (_pin->direction())
    {
    case Pin::Left:
        textRect = QRect(-_pin->length()/ratio, -metrics.height()/2, -name.count()*50/ratio - 10, metrics.height());
        numRect = QRect(4, 4, -_pin->length()/ratio-4, -metrics.height());
        break;
    case Pin::Right:
        textRect = QRect(_pin->length()/ratio, -metrics.height()/2, name.count()*50/ratio + 10, metrics.height());
        numRect = QRect(-4, 4, _pin->length()/ratio+4, -metrics.height());
        break;
    case Pin::Up:
        textRect = QRect(-metrics.height()/2, -_pin->length()/ratio, metrics.height(), -name.count()*50/ratio - 10);
        numRect = QRect(4, 4, -metrics.height(), -_pin->length()/ratio-4);
        break;
    case Pin::Down:
        textRect = QRect(-metrics.height()/2, _pin->length()/ratio, metrics.height(), name.count()*50/ratio + 10);
        numRect = QRect(4, -4, -metrics.height(), _pin->length()/ratio+4);
        break;
    }

    QPainterPath path;
    path.addRect(textRect.normalized());
    path.addRect(numRect.normalized());
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

void PinItem::updateData()
{
    prepareGeometryChange();
}
