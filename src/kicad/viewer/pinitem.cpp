/**
 ** This file is part of the uConfig project.
 ** Copyright 2017-2020 Robotips, Sebastien CAUX (sebcaux)
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
#include <QDebug>
#include <QPainter>

#include "model/component.h"

PinItem::PinItem(Pin *pin)
{
    _fontPad = nullptr;
    _fontName = nullptr;
    _fontType = nullptr;

    setPin(pin);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setCursor(Qt::CrossCursor);
    _showElectricalType = true;
}

PinItem::~PinItem()
{
    delete _fontPad;
    delete _fontName;
    delete _fontType;
}

void PinItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    // colors
    QColor drawColor = QColor(132, 0, 0);
    QColor textColor = QColor(0, 132, 132);
    if (_pin->pinType() == Pin::NotVisible)
    {
        drawColor = QColor(132, 132, 132);
        textColor = QColor(132, 132, 132);
    }

    // text font
    QFont fontName = _fontName->font();
    QFont fontPad = _fontPad->font();

    // selection modifier
    if (isSelected())
    {
        painter->setPen(QPen(drawColor, 2));
        fontName.setBold(true);
        fontPad.setBold(true);
        textColor = textColor.darker(130);
    }
    else
    {
        painter->setPen(QPen(drawColor, 1));
    }

    painter->drawEllipse(-3, -3, 6, 6);

    // line invert draw
    QList<int> linesInvert;
    int i = _pin->name().indexOf('~');
    while (i >= 0)
    {
        linesInvert.append(i - linesInvert.count());
        i = _pin->name().indexOf('~', i + 1);
    }
    if (linesInvert.count() % 2 == 1)
    {
        linesInvert.append(_pin->name().length() - linesInvert.count());
    }

    QPolygon polygon;
    QString type = Pin::electricalTypeDesc(_pin->electricalType());
    type[0] = type[0].toUpper();
    QString name = _pin->name();
    name.remove('~');
    switch (_pin->direction())
    {
        case Pin::Up:
            painter->rotate(-90);
            [[fallthrough]];
        case Pin::Right:
            painter->drawLine(3, 0, _pin->length() / ComponentItem::ratio, 0);

            if (_pin->component()->showPadName())
            {
                painter->setFont(fontPad);
                _fontPad->drawText(painter, _rectPad, Qt::AlignHCenter, _pin->padName());
            }

            switch (_pin->pinType())
            {
                case Pin::Normal:
                    break;
                case Pin::InvertedClock:
                    painter->drawLine(_pin->length() / ComponentItem::ratio + 8, 0, _pin->length() / ComponentItem::ratio, 5);
                    painter->drawLine(_pin->length() / ComponentItem::ratio + 8, 0, _pin->length() / ComponentItem::ratio, -5);
                    [[fallthrough]];
                case Pin::Invert:
                    painter->setBrush(Qt::white);
                    painter->drawEllipse(QPoint(_pin->length() / ComponentItem::ratio - 5, 0), 5, 5);
                    break;
                case Pin::ClockLow:
                    painter->drawLine(_pin->length() / ComponentItem::ratio, 0, _pin->length() / ComponentItem::ratio - 10, -10);
                    painter->drawLine(_pin->length() / ComponentItem::ratio - 10, -10, _pin->length() / ComponentItem::ratio - 10, 0);
                    [[fallthrough]];
                case Pin::Clock:
                    painter->drawLine(_pin->length() / ComponentItem::ratio + 8, 0, _pin->length() / ComponentItem::ratio, 5);
                    painter->drawLine(_pin->length() / ComponentItem::ratio + 8, 0, _pin->length() / ComponentItem::ratio, -5);
                    break;
                case Pin::LowIn:
                    painter->drawLine(_pin->length() / ComponentItem::ratio, 0, _pin->length() / ComponentItem::ratio - 10, -10);
                    painter->drawLine(_pin->length() / ComponentItem::ratio - 10, -10, _pin->length() / ComponentItem::ratio - 10, 0);
                    break;
                case Pin::LowOut:
                    painter->drawLine(_pin->length() / ComponentItem::ratio, -10, _pin->length() / ComponentItem::ratio - 10, 0);
                    break;
                case Pin::FallingEdge:
                    painter->setBrush(Qt::white);
                    polygon << QPoint(_pin->length() / ComponentItem::ratio, 5) << QPoint(_pin->length() / ComponentItem::ratio - 10, 0)
                            << QPoint(_pin->length() / ComponentItem::ratio, -5);
                    painter->drawPolygon(polygon);
                    break;
                case Pin::NonLogic:
                    painter->drawLine(_pin->length() / ComponentItem::ratio + 5, -5, _pin->length() / ComponentItem::ratio - 5, 5);
                    painter->drawLine(_pin->length() / ComponentItem::ratio + 5, 5, _pin->length() / ComponentItem::ratio - 5, -5);
                    break;
                case Pin::NotVisible:
                    break;
            }
            if (_pin->component()->showPinName())
            {
                painter->setPen(textColor);
                painter->setFont(fontName);
                _fontName->drawText(painter, _rectName, Qt::AlignLeft, name);
                for (int l = 0; l < linesInvert.count(); l += 2)
                {
                    painter->drawLine(_pin->length() / ComponentItem::ratio + 5 + _fontName->textWidth(name.left(linesInvert[l])),
                                      -painter->fontMetrics().height() / 2,
                                      _pin->length() / ComponentItem::ratio + 5 + _fontName->textWidth(name.left(linesInvert[l + 1])),
                                      -painter->fontMetrics().height() / 2);
                }
            }

            if (_showElectricalType)
            {
                painter->setFont(_fontType->font());
                painter->setPen(QColor(0, 0, 255));
                _fontType->drawText(painter, _rectType, Qt::AlignRight, type);
            }
            break;

        case Pin::Down:
            painter->rotate(-90);
            [[fallthrough]];
        case Pin::Left:
            painter->drawLine(-3, 0, -_pin->length() / ComponentItem::ratio, 0);

            if (_pin->component()->showPadName())
            {
                painter->setFont(fontPad);
                _fontPad->drawText(painter, _rectPad, Qt::AlignHCenter, _pin->padName());
            }

            switch (_pin->pinType())
            {
                case Pin::Normal:
                    break;
                case Pin::InvertedClock:
                    painter->drawLine(-_pin->length() / ComponentItem::ratio - 8, 0, -_pin->length() / ComponentItem::ratio, 5);
                    painter->drawLine(-_pin->length() / ComponentItem::ratio - 8, 0, -_pin->length() / ComponentItem::ratio, -5);
                    [[fallthrough]];
                case Pin::Invert:
                    painter->setBrush(Qt::white);
                    painter->drawEllipse(QPoint(-_pin->length() / ComponentItem::ratio + 5, 0), 5, 5);
                    break;
                case Pin::ClockLow:
                    painter->drawLine(-_pin->length() / ComponentItem::ratio, 0, -_pin->length() / ComponentItem::ratio + 10, -10);
                    painter->drawLine(-_pin->length() / ComponentItem::ratio + 10, -10, -_pin->length() / ComponentItem::ratio + 10, 0);
                    [[fallthrough]];
                case Pin::Clock:
                    painter->drawLine(-_pin->length() / ComponentItem::ratio - 8, 0, -_pin->length() / ComponentItem::ratio, 5);
                    painter->drawLine(-_pin->length() / ComponentItem::ratio - 8, 0, -_pin->length() / ComponentItem::ratio, -5);
                    break;
                case Pin::LowIn:
                    painter->drawLine(-_pin->length() / ComponentItem::ratio, 0, -_pin->length() / ComponentItem::ratio + 10, -10);
                    painter->drawLine(-_pin->length() / ComponentItem::ratio + 10, -10, -_pin->length() / ComponentItem::ratio + 10, 0);
                    break;
                case Pin::LowOut:
                    painter->drawLine(-_pin->length() / ComponentItem::ratio, -10, -_pin->length() / ComponentItem::ratio + 10, 0);
                    break;
                case Pin::FallingEdge:
                    painter->setBrush(Qt::white);
                    polygon << QPoint(-_pin->length() / ComponentItem::ratio, 5) << QPoint(-_pin->length() / ComponentItem::ratio + 10, 0)
                            << QPoint(-_pin->length() / ComponentItem::ratio, -5);
                    painter->drawPolygon(polygon);
                    break;
                case Pin::NonLogic:
                    painter->drawLine(-_pin->length() / ComponentItem::ratio - 5, -5, -_pin->length() / ComponentItem::ratio + 5, 5);
                    painter->drawLine(-_pin->length() / ComponentItem::ratio - 5, 5, -_pin->length() / ComponentItem::ratio + 5, -5);
                    break;
                case Pin::NotVisible:
                    break;
            }

            if (_pin->component()->showPinName())
            {
                painter->setPen(textColor);
                painter->setFont(fontName);
                _fontName->drawText(painter, _rectName, Qt::AlignRight, name);
                for (int l = 0; l < linesInvert.count(); l += 2)
                {
                    painter->drawLine(-_pin->length() / ComponentItem::ratio - 5 - _fontName->textWidth(name) + _fontName->textWidth(name.left(linesInvert[l])),
                                      -painter->fontMetrics().height() / 2,
                                      -_pin->length() / ComponentItem::ratio - 5 - _fontName->textWidth(name)
                                          + _fontName->textWidth(name.left(linesInvert[l + 1])),
                                      -painter->fontMetrics().height() / 2);
                }
            }

            if (_showElectricalType)
            {
                painter->setFont(_fontType->font());
                painter->setPen(QColor(0, 0, 255));
                _fontType->drawText(painter, _rectType, Qt::AlignLeft, type);
            }
            break;
    }
}

QRectF PinItem::boundingRect() const
{
    QRectF rect = _brectPad;
    rect = rect.united(_brectName);
    if (_showElectricalType)
    {
        rect = rect.united(_brectType);
    }
    return rect.adjusted(-5, -5, 5, 5);
}

QPainterPath PinItem::shape() const
{
    QPainterPath path;
    path.addRect(_brectPad.normalized());
    path.addRect(_brectName.normalized());
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

    QString type = Pin::electricalTypeDesc(_pin->electricalType());
    type[0] = type[0].toUpper();

    delete _fontPad;
    _fontPad = new KicadFont(_pin->textPadSize() / ComponentItem::ratio);
    QFontMetrics metricsPad(_fontPad->font());
    delete _fontName;
    _fontName = new KicadFont(_pin->textNameSize() / ComponentItem::ratio);
    QFontMetrics metricsName(_fontName->font());
    delete _fontType;
    _fontType = new KicadFont(25.0 / ComponentItem::ratio);
    QFontMetrics metricsType(_fontType->font());

    switch (_pin->direction())
    {
        case Pin::Left:
            _rectName =
                QRect(-_pin->length() / ComponentItem::ratio - 5, -metricsName.height() / 2, -_fontName->textWidth(name), metricsName.height()).normalized();
            _brectName = _rectName;
            _rectPad = QRect(0, 0, -_pin->length() / ComponentItem::ratio, -metricsPad.height()).normalized();
            _brectPad = _rectPad;
            _rectType = QRect(6, metricsType.height() / 2, _fontType->textWidth(type), -metricsType.height()).normalized();
            _brectType = _rectType;
            break;
        case Pin::Right:
            _rectName =
                QRect(_pin->length() / ComponentItem::ratio + 5, -metricsName.height() / 2, _fontName->textWidth(name), metricsName.height()).normalized();
            _brectName = _rectName;
            _rectPad = QRect(0, 0, _pin->length() / ComponentItem::ratio, -metricsPad.height()).normalized();
            _brectPad = _rectPad;
            _rectType = QRect(-6, metricsType.height() / 2, -_fontType->textWidth(type), -metricsType.height()).normalized();
            _brectType = _rectType;
            break;
        case Pin::Up:
            _brectName =
                QRect(-metricsName.height() / 2, -_pin->length() / ComponentItem::ratio - 5, metricsName.height(), -_fontName->textWidth(name)).normalized();
            _rectName =
                QRect(_pin->length() / ComponentItem::ratio + 5, -metricsName.height() / 2, _fontName->textWidth(name), metricsName.height()).normalized();
            _brectPad = QRect(0, 0, -metricsPad.height(), -_pin->length() / ComponentItem::ratio).normalized();
            _rectPad = QRect(0, 0, _pin->length() / ComponentItem::ratio, -metricsPad.height()).normalized();
            _brectType = QRect(metricsType.height() / 2, 6, -metricsType.height(), _fontType->textWidth(type)).normalized();
            _rectType = QRect(-6, metricsType.height() / 2, -_fontType->textWidth(type), -metricsType.height()).normalized();
            break;
        case Pin::Down:
            _brectName =
                QRect(-metricsName.height() / 2, _pin->length() / ComponentItem::ratio + 5, metricsName.height(), _fontName->textWidth(name)).normalized();
            _rectName =
                QRect(-_pin->length() / ComponentItem::ratio - 5, -metricsName.height() / 2, -_fontName->textWidth(name), metricsName.height()).normalized();
            _brectPad = QRect(0, 0, -metricsPad.height(), _pin->length() / ComponentItem::ratio).normalized();
            _rectPad = QRect(0, 0, -_pin->length() / ComponentItem::ratio, -metricsPad.height()).normalized();
            _brectType = QRect(metricsType.height() / 2, -6, -metricsType.height(), -_fontType->textWidth(type)).normalized();
            _rectType = QRect(6, metricsType.height() / 2, _fontType->textWidth(type), -metricsType.height()).normalized();
            break;
    }

    setPos(pin->pos() / ComponentItem::ratio);
    update();
}

bool PinItem::showElectricalType() const
{
    return _showElectricalType;
}

void PinItem::setShowElectricalType(bool showElectricalType)
{
    prepareGeometryChange();
    _showElectricalType = showElectricalType;
}

void PinItem::updateData()
{
    prepareGeometryChange();
    setPin(_pin);
}
