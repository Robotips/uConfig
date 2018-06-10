/**
 ** This file is part of the uConfig project.
 ** Copyright 2018 Robotips sebastien.caux@robotips.fr.
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation, either version 3 of the License, or
 ** (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with this program. If not, see <http://www.gnu.org/licenses/>.
 **/

#include "pinitem.h"

#include "componentitem.h"

#include <QCursor>
#include <QPainter>
#include <QDebug>

#include "model/component.h"

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

    /*painter->drawRect(boundingRect());
    painter->drawPath(shape());*/

    // text font
    QFont fontName = _fontName;
    QFont fontPad = _fontPad;

    // selection modifier
    if (isSelected())
    {
        painter->setPen(QPen(drawColor, 2));
        fontName.setBold(true);
        fontPad.setBold(true);
    }
    else
        painter->setPen(QPen(drawColor, 1));

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
        painter->drawLine(3, 0, _pin->length() / ComponentItem::ratio, 0);

        if (_pin->component()->showPadName())
        {
            painter->setFont(fontPad);
            painter->drawText(QRect(0, 3, _pin->length() / ComponentItem::ratio,
                                    -painter->fontMetrics().height()).normalized(),
                                    Qt::AlignHCenter, _pin->padName());
        }

        switch (_pin->pinType())
        {
        case Pin::Normal:
            break;
        case Pin::InvertedClock:
            painter->drawLine(_pin->length() / ComponentItem::ratio+8, 0,
                              _pin->length() / ComponentItem::ratio,   5);
            painter->drawLine(_pin->length() / ComponentItem::ratio+8, 0,
                              _pin->length() / ComponentItem::ratio,  -5);
        case Pin::Invert:
            painter->setBrush(Qt::white);
            painter->drawEllipse(QPoint(_pin->length() / ComponentItem::ratio-5, 0), 5, 5);
            break;
        case Pin::ClockLow:
            painter->drawLine(_pin->length() / ComponentItem::ratio,     0,
                              _pin->length() / ComponentItem::ratio-10, -10);
            painter->drawLine(_pin->length() / ComponentItem::ratio-10, -10,
                              _pin->length() / ComponentItem::ratio-10,  0);
        case Pin::Clock:
            painter->drawLine(_pin->length() / ComponentItem::ratio+8, 0,
                              _pin->length() / ComponentItem::ratio,   5);
            painter->drawLine(_pin->length() / ComponentItem::ratio+8, 0,
                              _pin->length() / ComponentItem::ratio,  -5);
            break;
        case Pin::LowIn:
            painter->drawLine(_pin->length() / ComponentItem::ratio,     0,
                              _pin->length() / ComponentItem::ratio-10, -10);
            painter->drawLine(_pin->length() / ComponentItem::ratio-10, -10,
                              _pin->length() / ComponentItem::ratio-10,  0);
            break;
        case Pin::LowOut:
            painter->drawLine(_pin->length() / ComponentItem::ratio,   -10,
                              _pin->length() / ComponentItem::ratio-10, 0);
            break;
        case Pin::FallingEdge:
            painter->setBrush(Qt::white);
            polygon << QPoint(_pin->length() / ComponentItem::ratio, 5)
                    << QPoint(_pin->length() / ComponentItem::ratio-10, 0)
                    << QPoint(_pin->length() / ComponentItem::ratio, -5);
            painter->drawPolygon(polygon);
            break;
        case Pin::NonLogic:
            painter->drawLine(_pin->length() / ComponentItem::ratio+5, -5,
                              _pin->length() / ComponentItem::ratio-5, 5);
            painter->drawLine(_pin->length() / ComponentItem::ratio+5, 5,
                              _pin->length() / ComponentItem::ratio-5, -5);
            break;
        case Pin::NotVisible:
            break;
        }
        painter->setPen(textColor);
        if (_pin->component()->showPinName())
        {
            painter->setFont(fontName);
            painter->drawText(QRect(_pin->length() / ComponentItem::ratio+10, -painter->fontMetrics().height()/2,
                                    painter->fontMetrics().width(name), painter->fontMetrics().height()).normalized(),
                                    Qt::AlignLeft, name);
            for (int l=0; l<linesInvert.count(); l+=2)
            {
                painter->drawLine(_pin->length() / ComponentItem::ratio+8 + linesInvert[l] * 50 / ComponentItem::ratio, -painter->fontMetrics().height() / 2,
                                  _pin->length() / ComponentItem::ratio+8 + linesInvert[l+1] * 50 / ComponentItem::ratio, -painter->fontMetrics().height() / 2);
            }
        }
        break;

    case Pin::Down:
        painter->rotate(-90);
    case Pin::Left:
        painter->drawLine(-3, 0, -_pin->length() / ComponentItem::ratio, 0);

        if (_pin->component()->showPadName())
        {
            painter->setFont(fontPad);
            painter->drawText(QRect(0, 3, -_pin->length() / ComponentItem::ratio, -painter->fontMetrics().height()).normalized(),
                              Qt::AlignHCenter, _pin->padName());
        }

        switch (_pin->pinType())
        {
        case Pin::Normal:
            break;
        case Pin::InvertedClock:
            painter->drawLine(-_pin->length() / ComponentItem::ratio-8, 0,
                              -_pin->length() / ComponentItem::ratio,   5);
            painter->drawLine(-_pin->length() / ComponentItem::ratio-8, 0,
                              -_pin->length() / ComponentItem::ratio,  -5);
        case Pin::Invert:
            painter->setBrush(Qt::white);
            painter->drawEllipse(QPoint(-_pin->length() / ComponentItem::ratio+5, 0), 5, 5);
            break;
        case Pin::ClockLow:
            painter->drawLine(-_pin->length() / ComponentItem::ratio,    0,
                              -_pin->length() / ComponentItem::ratio+10, -10);
            painter->drawLine(-_pin->length() / ComponentItem::ratio+10, -10,
                              -_pin->length() / ComponentItem::ratio+10, 0);
        case Pin::Clock:
            painter->drawLine(-_pin->length() / ComponentItem::ratio-8, 0,
                              -_pin->length() / ComponentItem::ratio,   5);
            painter->drawLine(-_pin->length() / ComponentItem::ratio-8, 0,
                              -_pin->length() / ComponentItem::ratio,  -5);
            break;
        case Pin::LowIn:
            painter->drawLine(-_pin->length() / ComponentItem::ratio,    0,
                              -_pin->length() / ComponentItem::ratio+10, -10);
            painter->drawLine(-_pin->length() / ComponentItem::ratio+10, -10,
                              -_pin->length() / ComponentItem::ratio+10, 0);
            break;
        case Pin::LowOut:
            painter->drawLine(-_pin->length() / ComponentItem::ratio,  -10,
                              -_pin->length() / ComponentItem::ratio+10, 0);
            break;
        case Pin::FallingEdge:
            painter->setBrush(Qt::white);
            polygon << QPoint(-_pin->length() / ComponentItem::ratio,    5)
                    << QPoint(-_pin->length() / ComponentItem::ratio+10, 0)
                    << QPoint(-_pin->length() / ComponentItem::ratio,   -5);
            painter->drawPolygon(polygon);
            break;
        case Pin::NonLogic:
            painter->drawLine(-_pin->length() / ComponentItem::ratio-5, -5,
                              -_pin->length() / ComponentItem::ratio+5,  5);
            painter->drawLine(-_pin->length() / ComponentItem::ratio-5, 5,
                              -_pin->length() / ComponentItem::ratio+5, -5);
            break;
        case Pin::NotVisible:
            break;
        }
        painter->setPen(textColor);

        if (_pin->component()->showPinName())
        {
            painter->setFont(fontName);
            painter->drawText(QRect(-_pin->length() / ComponentItem::ratio-10, -painter->fontMetrics().height()/2,
                                    -painter->fontMetrics().width(name), painter->fontMetrics().height()).normalized(),
                                    Qt::AlignRight, name);
            for (int l=0; l<linesInvert.count(); l+=2)
            {
                painter->drawLine(-_pin->length() / ComponentItem::ratio-8-name.count()*50 / ComponentItem::ratio + linesInvert[l]*50 / ComponentItem::ratio, -painter->fontMetrics().height()/2,
                                  -_pin->length() / ComponentItem::ratio-8-name.count()*50 / ComponentItem::ratio + linesInvert[l+1]*50 / ComponentItem::ratio, -painter->fontMetrics().height()/2);
            }
        }
        break;
    }
}

QRectF PinItem::boundingRect() const
{
    QRectF rect = _rectPad;
    rect = rect.united(_rectName);
    return rect.normalized();
}

QPainterPath PinItem::shape() const
{
    QPainterPath path;
    path.addRect(_rectPad.normalized());
    path.addRect(_rectName.normalized());
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
    QString name = _pin->name();
    name.remove('~');

    _fontPad = ComponentItem::font(_pin->textPadSize() / ComponentItem::ratio);
    _fontName = ComponentItem::font(_pin->textNameSize() / ComponentItem::ratio);
    QFontMetrics metricsPad(_fontPad);
    QFontMetrics metricsName(_fontName);

    switch (_pin->direction())
    {
    case Pin::Left:
        _rectName = QRect(-_pin->length() / ComponentItem::ratio, -metricsName.height()/2,
                          -metricsName.width(name) - 10, metricsName.height());
        _rectPad = QRect(4, 4, -_pin->length() / ComponentItem::ratio-4, -metricsPad.height());
        break;
    case Pin::Right:
        _rectName = QRect(_pin->length() / ComponentItem::ratio, -metricsName.height()/2,
                          metricsName.width(name) + 10, metricsName.height());
        _rectPad = QRect(-4, 4, _pin->length() / ComponentItem::ratio+4, -metricsPad.height());
        break;
    case Pin::Up:
        _rectName = QRect(-metricsName.height()/2, -_pin->length() / ComponentItem::ratio,
                          metricsName.height(), -metricsName.width(name) - 10);
        _rectPad = QRect(4, 4, -metricsPad.height(), -_pin->length() / ComponentItem::ratio-4);
        break;
    case Pin::Down:
        _rectName = QRect(-metricsName.height()/2, _pin->length() / ComponentItem::ratio,
                          metricsName.height(), metricsName.width(name) + 10);
        _rectPad = QRect(4, -4, -metricsPad.height(), _pin->length() / ComponentItem::ratio+4);
        break;
    }

    setPos(pin->pos() / ComponentItem::ratio);
    update();
}

void PinItem::updateData()
{
    prepareGeometryChange();
}
