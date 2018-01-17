#include "pinitem.h"

#include <QCursor>
#include <QPainter>
#include <QDebug>

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
        qDebug()<<i;
    }
    if (linesInvert.count() % 2 == 1)
        linesInvert.append(_pin->name().length()-linesInvert.count());

    QString name = _pin->name();
    name.remove('~');
    qDebug()<<linesInvert<<_pin->name()<<name;
    switch (_pin->direction())
    {
    case Pin::Left:
        painter->drawLine(-3, 0, -_pin->length()/ratio, 0);
        painter->drawText(QRect(0, 3, -_pin->length()/ratio, -painter->fontMetrics().height()).normalized(), Qt::AlignHCenter, _pin->padName());
        painter->setPen(textColor);
        painter->drawText(QRect(-_pin->length()/ratio-10, -painter->fontMetrics().height()/2, -name.count()*50/ratio, painter->fontMetrics().height()).normalized(), Qt::AlignRight, name);
        for (int l=0; l<linesInvert.count(); l+=2)
        {
            painter->drawLine(-_pin->length()/ratio-8-name.count()*50/ratio + linesInvert[l]*50/ratio, -painter->fontMetrics().height()/2,
                              -_pin->length()/ratio-8-name.count()*50/ratio + linesInvert[l+1]*50/ratio, -painter->fontMetrics().height()/2);
        }
        break;
    case Pin::Right:
        painter->drawLine(3, 0, _pin->length()/ratio, 0);
        painter->drawText(QRect(0, 3, _pin->length()/ratio, -painter->fontMetrics().height()).normalized(), Qt::AlignHCenter, _pin->padName());
        painter->setPen(textColor);
        painter->drawText(QRect(_pin->length()/ratio+10, -painter->fontMetrics().height()/2, name.count()*50/ratio, painter->fontMetrics().height()).normalized(), Qt::AlignLeft, name);
        for (int l=0; l<linesInvert.count(); l+=2)
        {
            painter->drawLine(_pin->length()/ratio+8 + linesInvert[l]*50/ratio, -painter->fontMetrics().height()/2,
                              _pin->length()/ratio+8 + linesInvert[l+1]*50/ratio, -painter->fontMetrics().height()/2);
        }
        break;
    case Pin::Up:
        painter->drawLine(0, -3, 0, -_pin->length()/ratio);
        painter->rotate(-90);
        painter->drawText(QRect(0, 3, _pin->length()/ratio, -painter->fontMetrics().height()).normalized(), Qt::AlignHCenter, _pin->padName());
        painter->setPen(textColor);
        painter->drawText(QRect(_pin->length()/ratio+10, -painter->fontMetrics().height()/2, name.count()*50/ratio, painter->fontMetrics().height()).normalized(), Qt::AlignLeft, name);
        for (int l=0; l<linesInvert.count(); l+=2)
        {
            painter->drawLine(_pin->length()/ratio+8 + linesInvert[l]*50/ratio, -painter->fontMetrics().height()/2,
                              _pin->length()/ratio+8 + linesInvert[l+1]*50/ratio, -painter->fontMetrics().height()/2);
        }
        painter->rotate(90);
        break;
    case Pin::Down:
        painter->drawLine(0, 3, 0, _pin->length()/ratio);
        painter->rotate(90);
        painter->drawText(QRect(0, 3, _pin->length()/ratio, -painter->fontMetrics().height()).normalized(), Qt::AlignHCenter, _pin->padName());
        painter->setPen(textColor);
        painter->drawText(QRect(_pin->length()/ratio+10, -painter->fontMetrics().height()/2, name.count()*50/ratio, painter->fontMetrics().height()).normalized(), Qt::AlignLeft, name);
        for (int l=0; l<linesInvert.count(); l+=2)
        {
            painter->drawLine(_pin->length()/ratio+8 + linesInvert[l]*50/ratio, -painter->fontMetrics().height()/2,
                              _pin->length()/ratio+8 + linesInvert[l+1]*50/ratio, -painter->fontMetrics().height()/2);
        }
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

void PinItem::updateData()
{
    prepareGeometryChange();
}
